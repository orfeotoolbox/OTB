/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbSamplingRateCalculatorList.h"
#include "otbStatisticsXMLFileReader.h"

namespace otb
{
namespace Wrapper
{

class MultiImageSamplingRate : public Application
{
public:
  /** Standard class typedefs. */
  typedef MultiImageSamplingRate        Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(MultiImageSamplingRate, otb::Application);

  /** typedef */
  typedef otb::SamplingRateCalculatorList           RateCalculatorListType;
  
  typedef RateCalculatorListType::ClassCountMapType      ClassCountMapType;
  typedef RateCalculatorListType::MapRateType           MapRateType;

  typedef itk::VariableLengthVector<float> MeasurementType;
  typedef otb::StatisticsXMLFileReader<MeasurementType> XMLReaderType;

private:
  MultiImageSamplingRate()
    {
    m_CalculatorList = RateCalculatorListType::New();
    }

  void DoInit()
  {
    SetName("MultiImageSamplingRate");
    SetDescription("Compute sampling rate for an input set of images.");

    // Documentation
    SetDocName("Multi-image sampling rate estimation");
    SetDocLongDescription("The application computes sampling rates for a set of"
      " input images. Before calling this application, each pair of image and "
      "training vectors has to be analysed with the application "
      "PolygonClassStatistics. The statistics file is then used to compute the "
      "sampling rates for each class in each image. Several types of sampling "
      " are implemented. Each one is a combination of a mono-image strategy "
      "and a multi-image mode. The mono-image strategies are :\n"
      "  - smallest (default) : select the same number of sample in each " 
      "class so that the smallest one is fully sampled.\n"
      "  - constant : select the same number of samples N in each class " 
      "(with N below or equal to the size of the smallest class).\n"
      "  - byclass : set the required number for each class manually, with an "
      "input CSV file (first column is class name, second one is the required "
      "samples number).\n"
      "The multi-image modes are :\n"
      "  - proportional : we try to split proportionally the number of required"
      " samples in each image\n"
      "  - equal : we split equally the number of required samples in each "
      "image.\n"
      "  - custom : the user chooses how to split the number of required "
      "samples for each image.\n"
      );
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Learning);

    AddParameter(ParameterType_InputFilenameList,  "il",   "Input statistics");
    SetParameterDescription("il", "List of statistics files for each input image.");

    AddParameter(ParameterType_OutputFilename, "out", "Output sampling rates");
    SetParameterDescription("out","Output filename storing sampling rates (CSV "
      "format with class name, required samples, total samples, and rate). "
      "The given filename will be used with a suffix to indicate the "
      "corresponding input index (for instance: rates.csv will give rates_1.csv"
      ", rates_2.csv, ...).");

    AddParameter(ParameterType_Choice, "strategy", "Sampling strategy");

    AddChoice("strategy.byclass","Set samples count for each class");
    SetParameterDescription("strategy.byclass","Set samples count for each class");

    AddParameter(ParameterType_InputFilenameList, "strategy.byclass.in", "Number of samples by class");
    SetParameterDescription("strategy.byclass.in", "Number of samples by class "
      "(CSV format with class name in 1st column and required samples in the 2nd)."
      "In the case of the custom multi-image mode, several inputs may be given for each image.");

    AddChoice("strategy.constant","Set the same samples counts for all classes");
    SetParameterDescription("strategy.constant","Set the same samples counts for all classes");

    AddParameter(ParameterType_String, "strategy.constant.nb", "Number of samples for all classes");
    SetParameterDescription("strategy.constant.nb", "Number of samples for all classes."
      "In the case of the custom multi-image mode, several values can be given for each image.");

    AddChoice("strategy.smallest","Set same number of samples for all classes, with the smallest class fully sampled");
    SetParameterDescription("strategy.smallest","Set same number of samples for all classes, with the smallest class fully sampled");

    AddChoice("strategy.all","Take all samples");
    SetParameterDescription("strategy.all","Take all samples");

    // Default strategy : smallest
    SetParameterString("strategy","smallest");

    AddParameter(ParameterType_Choice, "mim", "Multi-Image Mode");

    AddChoice("mim.proportional", "Proportional");
    SetParameterDescription("mim.proportional","Split proportionally the required number of samples");

    AddChoice("mim.equal", "equal");
    SetParameterDescription("mim.equal","Split equally the required number of samples");

    AddChoice("mim.custom", "Custom");
    SetParameterDescription("mim.custom","Split the required number of samples following user choice.");

    // Doc example parameter settings
    SetDocExampleParameterValue("il", "stats_1.xml stats_2.xml");
    SetDocExampleParameterValue("out", "rates.csv");
    SetDocExampleParameterValue("strategy", "smallest");
    SetDocExampleParameterValue("mode","proportional");
  }

  void DoUpdateParameters()
  {
  }

  void DoExecute()
    {
    // Clear state
    m_CalculatorList->Clear();
    std::vector<std::string> inputs = this->GetParameterStringList("il");
    unsigned int nbInputs = inputs.size();
    XMLReaderType::Pointer statReader = XMLReaderType::New();
    for (unsigned int i=0 ; i<nbInputs ; i++ )
      {
      m_CalculatorList->PushBack(otb::SamplingRateCalculator::New());
      statReader->SetFileName(inputs[i]);
      ClassCountMapType classCount = statReader->GetStatisticMapByName<ClassCountMapType>("samplesPerClass");
      m_CalculatorList->SetNthClassCount(i,classCount);
      }

    // Cautions : direct mapping between the enum PartitionType and the choice order
    RateCalculatorListType::PartitionType partitionMode =
      static_cast<RateCalculatorListType::PartitionType>(this->GetParameterInt("mim"));

    unsigned int minParamSize = 1;
    if (partitionMode == RateCalculatorListType::CUSTOM)
      {
      // Check we have enough inputs for the custom mode
      minParamSize = nbInputs;
      }
    
    switch (this->GetParameterInt("strategy"))
      {
      // byclass
      case 0:
        {
        std::vector<std::string> requiredFiles = this->GetParameterStringList("strategy.byclass.in");
        std::vector<ClassCountMapType> requiredCounts;
        if (requiredFiles.size() < minParamSize)
          {
          otbAppLogFATAL("Missing arguments in strategy.byclass.in to process sampling rates");
          }
        otbAppLogINFO("Sampling strategy : set number of samples for each class");
        for (unsigned int i=0 ; i<minParamSize ; i++)
          {
          requiredCounts.push_back(otb::SamplingRateCalculator::ReadRequiredSamples(requiredFiles[i]));
          }
        m_CalculatorList->SetNbOfSamplesByClass(requiredCounts, partitionMode);
        }
      break;
      // constant
      case 1:
        {
        std::vector<itksys::String> parts = itksys::SystemTools::SplitString(this->GetParameterString("strategy.constant.nb"),' ');
        std::vector<unsigned long> countList;
        for (unsigned int i=0 ; i<parts.size() ; i++)
          {
          if (!parts[i].empty())
            {
            std::string::size_type pos1 = parts[i].find_first_not_of(" \t");
            std::string::size_type pos2 = parts[i].find_last_not_of(" \t");
            std::string value(parts[i].substr(pos1, pos2 - pos1 + 1));
            countList.push_back(boost::lexical_cast<unsigned long>(parts[i]));
            }
          }
        if (countList.size() < minParamSize)
          {
          otbAppLogFATAL("Missing arguments in strategy.constant.nb to process sampling rates");
          }
        otbAppLogINFO("Sampling strategy : set a constant number of samples for all classes");
        m_CalculatorList->SetNbOfSamplesAllClasses(countList, partitionMode);
        }
      break;
      // smallest class
      case 2:
        {
        otbAppLogINFO("Sampling strategy : fit the number of samples based on the smallest class");
        m_CalculatorList->SetMinimumNbOfSamplesByClass(partitionMode);
        }
      break;
      // all samples
      case 3:
        {
        otbAppLogINFO("Sampling strategy : take all samples");
        m_CalculatorList->SetAllSamples(partitionMode);
        }
      break;
      default:
        otbAppLogFATAL("Strategy mode unknown :"<<this->GetParameterString("strategy"));
      break;
      }

    std::ostringstream oss;    
    std::string outputPath(this->GetParameterString("out"));
    std::string outputBase = outputPath.substr(0, outputPath.find_last_of('.'));
    std::string outputExt = outputPath.substr(outputPath.find_last_of('.'), std::string::npos);
    for (unsigned int i=0 ; i<nbInputs ; i++ )
      {
      // Print results
      oss.str(std::string(""));
      oss << " className  requiredSamples  totalSamples  rate" << std::endl;
      MapRateType rates = m_CalculatorList->GetRatesByClass(i);
      MapRateType::const_iterator itRates = rates.begin();
      for(; itRates != rates.end(); ++itRates)
        {
        otb::SamplingRateCalculator::TripletType tpt = itRates->second;
        oss << itRates->first << "\t" << tpt.Required << "\t" << tpt.Tot << "\t" << tpt.Rate;
        if (tpt.Required > tpt.Tot)
          {
          oss << "\t[OVERFLOW]";
          }
        oss << std::endl;
        }
      otbAppLogINFO("Sampling rates for image "<< i+1 <<" : " << oss.str());
      // Output results to disk
      oss.str(std::string(""));
      oss << outputBase << "_" << i+1 << outputExt;
      m_CalculatorList->GetNthElement(i)->Write(oss.str());
      }
  }

  RateCalculatorListType::Pointer m_CalculatorList;
};

} // end namespace Wrapper
} // end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::MultiImageSamplingRate)
