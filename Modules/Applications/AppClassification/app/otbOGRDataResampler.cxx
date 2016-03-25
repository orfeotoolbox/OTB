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
#include "otbSamplingRateCalculator.h"
#include "otbOGRDataToResampledOGRData.h"
#include "otbStatisticsXMLFileReader.h"

namespace otb
{
namespace Wrapper
{

class OGRDataResampler : public Application
{
public:
  /** Standard class typedefs. */
  typedef OGRDataResampler        Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(OGRDataResampler, otb::Application);

  /** typedef */
  typedef otb::OGRDataToResampledOGRData<FloatVectorImageType,UInt8ImageType> ResamplerFilterType;  
  typedef otb::SamplingRateCalculator RateCalculatorype;
  
  typedef std::map<std::string, unsigned long>      ClassCountMapType;
  typedef itk::VariableLengthVector<float> MeasurementType;
  typedef otb::StatisticsXMLFileReader<MeasurementType> ReaderType;

private:
  OGRDataResampler()
    {
   
    }

  void DoInit()
  {
    SetName("OGRDataResampler");
    SetDescription("Selects samples from a training vector data set.");

    // Documentation
    SetDocName("Polygon Class Statistics");
    SetDocLongDescription("The application selects a set of samples from geometries "
      "intended for training (they should have a field giving the associated "
      "class). \n\nFirst of all, the geometries must be analyzed by the PolygonClassStatistics application "
      "to compute statistics about the geometries, which are summarized in an xml file. "
      "\nThen, this xml file must be given as input to this application (parameter instats).\n\n"
      "There are three different strategies to select samples (parameter sampstrat) :\n"
      "  - minnbsamp (default) : randomly select samples in such a way that the final sampled\n" 
      "    classes have the size of the smallest one.\n"
      "  - nbsampallclasses : randomly select samples in such a way that the final sampled\n" 
      "    classes have a size of N (with N below or equal to the size of the smallest class).\n"
      "  - nbsampbyclass : set the required number for each class manually with the\n" 
      "    following pattern : \"class_name1:nb_samples1 class_name2:nb_samples2 ...\".\n"
      "Once the strategy is selected, the application outputs samples which are spatially uniformly distributed "
      "(parameter out).\n\n"
      
      "In addition to the parameters instats, sampstrat, and out, some others must be compulsorily specified:\n"
      "  - in : an input image must be provided to this application (parameter in). Here, the values of its pixels are not used;\n"
      "    the purpose is rather to provide geometry information so that the selected samples can be outputted as physical points.\n"
      "  - vec : input training vector data set, from where to pick samples.\n"
      "  - layer : index specifying from which layer to pick samples.\n"
      "  - field : set the field of the selected samples.\n"
      
      "\nSome other optional arguments can also be passed to the application:\n"
      "  - mask : an optional raster mask can be used to discard samples.\n"
      "  - outrates : allows to output a txt file that summarizes the sampling rates for each class.\n"
      "  - outsampvec / insampvec : internally, this application uses a random lists of bits to decide\n"
      "    whether to select a sample or not. If users wish they could exactly repeat the selection,\n "
      "    they should output these lists in a txt file, and reload it the next time. This is the purpose of these two parameters.\n"
      "  - maxsampvecsize : allows to limit the size of the lists of bits mentioned above, to optimize the memory footprint.\n"
      
      "\nAs with the PolygonClassStatistics application, different types  of geometry are supported : "
      "polygons, lines, points. \nThe behavior is different for each type of geometry :\n"
      "  - polygon: select points whose center is inside the polygon\n"
      "  - lines  : select points intersecting the line\n"
      "  - points : select points pixel to the point\n");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Learning);

    AddParameter(ParameterType_InputImage,  "in",   "InputImage");
    SetParameterDescription("in", "Support image that will be classified");

    AddParameter(ParameterType_InputImage,  "mask",   "InputMask");
    SetParameterDescription("mask", "Validity mask (only pixels corresponding to a mask value greater than 0 will be used for statistics)");
    MandatoryOff("mask");
    
    AddParameter(ParameterType_InputFilename, "vec", "Input vectors");
    SetParameterDescription("vec","Input geometries to analyse");
    
    AddParameter(ParameterType_OutputFilename, "out", "Output vectors");
    SetParameterDescription("out","Output resampled geometries");
    
    AddParameter(ParameterType_InputFilename, "instats", "Input Statistics");
    SetParameterDescription("instats","Input file storing statistics (XML format)");
    
    AddParameter(ParameterType_InputFilename, "insampvec", "Input sampling vectors");
    SetParameterDescription("insampvec","Input sampling vectors");
    MandatoryOff("insampvec");
    
    AddParameter(ParameterType_OutputFilename, "outsampvec", "Output sampling vectors");
    SetParameterDescription("outsampvec","Output sampling vectors");
    MandatoryOff("outsampvec");
    
    AddParameter(ParameterType_OutputFilename, "outrates", "Output rates");
    SetParameterDescription("outrates","Output rates");
    MandatoryOff("outrates");
    
    AddParameter(ParameterType_Choice, "sampstrat", "Sampling strategy");
    AddChoice("sampstrat.nbsampbyclass","Set nb of samples by class");
    SetParameterDescription("sampstrat.nbsampbyclass","Set nb of samples by class");
    
    AddChoice("sampstrat.nbsampallclasses","Set same nb of samples for all classes");
    SetParameterDescription("sampstrat.nbsampallclasses","Set same nb of samples for all classes");
    
    AddChoice("sampstrat.minnbsamp","Set same nb of samples for all classes, with the smallest class fully sampled");
    SetParameterDescription("sampstrat.minnbsamp","Set same nb of samples for all classes, with the smallest class fully sampled");
    SetParameterInt("sampstrat",2);

    AddParameter(ParameterType_String, "nbsampbyclass", "Number of samples by class");
    SetParameterDescription("nbsampbyclass", "Number of samples by class");
    MandatoryOff("nbsampbyclass");
    
    AddParameter(ParameterType_Int, "nbsampallclasses", "Number of samples for all classes");
    SetParameterDescription("nbsampallclasses", "Number of samples for all classes");
    MandatoryOff("nbsampallclasses");
    
    AddParameter(ParameterType_Int, "maxsampvecsize", "Maximum size of the sampling vectors");
    SetParameterDescription("maxsampvecsize", "Maximum size of the sampling vectors");
    MandatoryOff("maxsampvecsize");

    AddParameter(ParameterType_String, "field", "Field Name");
    SetParameterDescription("field","Name of the field carrying the class name in the input vectors.");
    MandatoryOff("field");
    SetParameterString("field", "class");
    
    AddParameter(ParameterType_Int, "layer", "Layer Index");
    SetParameterDescription("layer", "Layer index to read in the input vector file.");
    MandatoryOff("layer");
    SetDefaultParameterInt("layer",0);
    
    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "support_image.tif");
    SetDocExampleParameterValue("vec", "variousVectors.sqlite");
    SetDocExampleParameterValue("field", "label");
    SetDocExampleParameterValue("instats","apTvClPolygonClassStatisticsOut.xml");
    SetDocExampleParameterValue("sampstrat","nbsampbyclass");
    SetDocExampleParameterValue("nbsampbyclass","\"1:98 2:100 3:23\"");
    SetDocExampleParameterValue("out","resampledVectors.sqlite");
  }

  void DoUpdateParameters()
  {
    GetParameterByKey("nbsampbyclass")->SetActive(false);
    MandatoryOff("nbsampbyclass");
    GetParameterByKey("nbsampallclasses")->SetActive(false);
    MandatoryOff("nbsampallclasses");
  
    if (IsParameterEnabled("sampstrat.nbsampbyclass") && HasValue("sampstrat.nbsampbyclass"))
    { 
       GetParameterByKey("nbsampbyclass")->SetActive(true);
       MandatoryOn("nbsampbyclass");
    }


    if (IsParameterEnabled("sampstrat.nbsampallclasses") && HasValue("sampstrat.nbsampallclasses"))
    { 
       GetParameterByKey("nbsampallclasses")->SetActive(true);
       MandatoryOn("nbsampallclasses");
    }
  }

  void DoExecute()
  {
      m_rateCalculator = RateCalculatorype::New();
      m_resampler = ResamplerFilterType::New();
      m_reader = ReaderType::New();
      
      m_reader->SetFileName(this->GetParameterString("instats"));
      ClassCountMapType classCount = m_reader->GetStatisticMapByName<ClassCountMapType>("samplesPerClass");
      
      m_rateCalculator->SetClassCount(classCount);
          
      switch (GetParameterInt("sampstrat"))
      {
        case 0: 
            m_rateCalculator->setNbofSamplesByClass(GetParameterString("nbsampbyclass"));
        break;
        case 1: 
            m_rateCalculator->setNbofSamplesAllClasses(GetParameterInt("nbsampallclasses"));
        break;
        case 2: 
            m_rateCalculator->setMinimumNbofSamplesByClass();
        break;
      }
      
      RateCalculatorype::mapRateType ratesbyClass = m_rateCalculator->GetRatesbyClass(); 
      
      if (IsParameterEnabled("outrates") && HasValue("outrates"))
      {
        m_rateCalculator->write(this->GetParameterString("outrates"));
      }
      
      m_vectors = otb::ogr::DataSource::New(this->GetParameterString("vec"));
      
      if (IsParameterEnabled("mask") && HasValue("mask"))
      {
        m_resampler->SetMask(this->GetParameterImage<UInt8ImageType>("mask"));
      }
      if (IsParameterEnabled("insampvec") && HasValue("insampvec"))
      {
        m_resampler->SetInputSamplingVectorsPath(this->GetParameterString("insampvec"));
      }
      if (IsParameterEnabled("outsampvec") && HasValue("outsampvec"))
      {
        m_resampler->SetOutputSamplingVectorsPath(this->GetParameterString("outsampvec"));
      }
      if (IsParameterEnabled("maxsampvecsize") && HasValue("maxsampvecsize"))
      {
        m_resampler->SetMaxSamplingVecSize(this->GetParameterInt("maxsampvecsize"));
      }
      
      m_resampler->SetInput(this->GetParameterImage("in"));
      m_resampler->SetOGRData(m_vectors);
      m_resampler->SetOutputVectorDataPath(this->GetParameterString("out"));
      m_resampler->SetRatesbyClass(ratesbyClass);
      m_resampler->SetFieldName(this->GetParameterString("field"));
      m_resampler->SetLayerIndex(this->GetParameterInt("layer"));
      
      m_resampler->Update(); 
  
  }

  RateCalculatorype::Pointer m_rateCalculator;
  ResamplerFilterType::Pointer m_resampler;
  ReaderType::Pointer m_reader;
  otb::ogr::DataSource::Pointer m_vectors;

};

} // end of namespace Wrapper
} // end of namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::OGRDataResampler)
