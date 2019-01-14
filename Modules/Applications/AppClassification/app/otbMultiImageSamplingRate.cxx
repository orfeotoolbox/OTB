/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

  void DoInit() override
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
      "and a multi-image mode. The mono-image strategies are:\n\n"

      "* smallest (default): select the same number of sample in each "
      "class so that the smallest one is fully sampled.\n"
      "* constant: select the same number of samples N in each class "
      "(with N below or equal to the size of the smallest class).\n"
      "* byclass: set the required number for each class manually, with an "
      "input CSV file (first column is class name, second one is the required "
      "samples number).\n\n"
      "The multi-image modes (mim) are proportional, equal and custom. The custom "
      "mode lets the users choose the distribution of samples among the "
      "images. The different behaviours are described below. Ti(c) and Ni(c) "
      " refers resp. to the total number and needed number of samples in "
      "image i for class c. Let's call L the total number of images.\n\n"
      "* strategy = all\n\n"
      "    - Same behaviour for all modes: take all samples\n\n"
      "* strategy = constant:"
      " let's call M the global number of samples required per class."
      " For each image i and each class c:\n\n"
      "    - if mim = proportional, then Ni( c ) = M * Ti( c ) / sum_k( Tk(c) )\n\n"
      "    - if mim = equal       , then Ni( c ) = M / L\n\n"
      "    - if mim = custom      , then Ni( c ) = Mi where Mi is the custom requested number of samples for image i\n\n"
      "* strategy = byClass :"
      " let's call M(c) the global number of samples for class c)."
      " For each image i and each class c:\n\n"
      "    - if mim = proportional, then Ni( c ) = M(c) * Ti( c ) / sum_k( Tk(c) )\n\n"
      "    - if mim = equal       , then Ni( c ) = M(c) / L\n\n"
      "    - if mim = custom      , then Ni( c ) = Mi(c) where Mi(c) is the custom requested number of samples for image i and class c\n\n"
      "* strategy = percent :"
      " For each image i and each class c:\n\n"
      "    - if mim = proportional, then Ni( c ) = p * Ti( c ) where p is the global percentage of samples\n\n"
      "    - if mim = equal       , then Ni( c ) = p * sum_k(Tk(c)]/L where p is the global percentage of samples\n\n"
      "    - if mim = custom      , then Ni( c ) = p(i) * Ti(c) where p(i) is the percentage of samples for image i. c\n\n"
      "* strategy = total :"
      " For each image i and each class c:\n\n"
      "    - if mim = proportional, then Ni( c ) = total * (sum_k(Ti(k))/sum_kl(Tl(k))) * (Ti(c)/sum_k(Ti(k))) where total is the total number of samples specified.\n\n"
      "    - if mim = equal       , then Ni( c ) = (total / L) * (Ti(c)/sum_k(Ti(k))) where total is the total number of samples specified.\n\n"
      "    - if mim = custom      , then Ni( c ) = total(i) * (Ti(c)/sum_k(Ti(k))) where total(i) is the total number of samples specified for image i. \n\n"                          
      "* strategy = smallest class\n\n"
      "    - if mim = proportional, then the smallest class size (computed globally) is used for the strategy constant+proportional.\n\n"
      "    - if mim = equal       , then the smallest class size (computed globally) is used for the strategy constant+equal.\n\n"
      "    - if mim = custom      , then the smallest class is computed and used for each image separately."
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

    AddChoice("strategy.percent","Use a percentage of the samples available for each class");
    SetParameterDescription("strategy.percent","Use a percentage of the samples available for each class");

    AddParameter(ParameterType_String,"strategy.percent.p","The percentage(s) to use");
    SetParameterDescription("strategy.percent.p","The percentage(s) to use " "In the case of the custom multi-image mode, several values can be given for each image.");

    AddChoice("strategy.total","Set the total number of samples to generate, and use class proportions.");
    SetParameterDescription("strategy.total","Set the total number of samples to generate, and use class proportions.");

    AddParameter(ParameterType_String,"strategy.total.v","The number of samples to generate");
    SetParameterDescription("strategy.total.v","The number of samples to generate" "In the case of the custom multi-image mode, several values can be given for each image.");
 
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
    SetDocExampleParameterValue("mim","proportional");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
  }

  void DoExecute() override
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
      // percent
      case 3:
        {
        std::vector<itksys::String> parts = itksys::SystemTools::SplitString(this->GetParameterString("strategy.percent.p"),' ');
        std::vector<double> percentList;
        for (unsigned int i=0 ; i<parts.size() ; i++)
          {
          if (!parts[i].empty())
            {
            std::string::size_type pos1 = parts[i].find_first_not_of(" \t");
            std::string::size_type pos2 = parts[i].find_last_not_of(" \t");
            std::string value(parts[i].substr(pos1, pos2 - pos1 + 1));
            percentList.push_back(boost::lexical_cast<double>(parts[i]));

            if(percentList.back()<0 || percentList.back()>1)
              {
              otbAppLogFATAL("Percent parameter should be in range [0,1]");
              } 
            }
          }
        if (percentList.size() < minParamSize)
          {
          otbAppLogFATAL("Missing arguments in strategy.percent.p to process sampling rates");
          }
        otbAppLogINFO("Sampling strategy : set a percentage of samples to be used.");
        m_CalculatorList->SetPercentageOfSamples(percentList, partitionMode);        
        }
      break;

      // total
      case 4:
        {
        std::vector<itksys::String> parts = itksys::SystemTools::SplitString(this->GetParameterString("strategy.total.v"),' ');
        std::vector<unsigned long> totalList;
        for (unsigned int i=0 ; i<parts.size() ; i++)
          {
          if (!parts[i].empty())
            {
            std::string::size_type pos1 = parts[i].find_first_not_of(" \t");
            std::string::size_type pos2 = parts[i].find_last_not_of(" \t");
            std::string value(parts[i].substr(pos1, pos2 - pos1 + 1));
            totalList.push_back(boost::lexical_cast<unsigned long>(parts[i]));
            }
          }
        if (totalList.size() < minParamSize)
          {
          otbAppLogFATAL("Missing arguments in strategy.total.v to process sampling rates");
          }
        otbAppLogINFO("Sampling strategy : set a constant number of samples for all classes");
        m_CalculatorList->SetTotalNumberOfSamples(totalList, partitionMode);
        }
      break;
      // all samples
      case 5:
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
    unsigned int overflowCount = 0;
    bool noSamples=true;
    for (unsigned int i=0 ; i<nbInputs ; i++ )
      {
      // Print results
      oss.str(std::string(""));
      oss << " className  requiredSamples  totalSamples  rate\n";
      MapRateType rates = m_CalculatorList->GetRatesByClass(i);
      if(!rates.empty())
        {
        noSamples = false;
        }
      MapRateType::const_iterator itRates = rates.begin();
      for(; itRates != rates.end(); ++itRates)
        {
        otb::SamplingRateCalculator::TripletType tpt = itRates->second;
        oss << itRates->first << "\t" << tpt.Required << "\t" << tpt.Tot << "\t" << tpt.Rate;
        if (tpt.Required > tpt.Tot)
          {
          overflowCount++;
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
    if (noSamples)
      {
      otbAppLogFATAL("No samples found in the inputs!");
      }
    if (overflowCount)
      {
      std::string plural(overflowCount>1?"s":"");
      otbAppLogWARNING(<< overflowCount << " case"<<plural<<" of overflow detected! (requested number of samples higher than total available samples)");
      }
  }

  RateCalculatorListType::Pointer m_CalculatorList;
};

} // end namespace Wrapper
} // end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::MultiImageSamplingRate)
