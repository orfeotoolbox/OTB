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
#include "otbSamplingRateCalculator.h"
#include "otbOGRDataToSamplePositionFilter.h"
#include "otbStatisticsXMLFileReader.h"
#include "otbRandomSampler.h"
#include "otbGeometriesProjectionFilter.h"
#include "otbGeometriesSet.h"
#include "otbWrapperElevationParametersHandler.h"

namespace otb
{
namespace Wrapper
{

/** Utility function to negate std::isalnum */
bool IsNotAlphaNum(char c)
  {
  return !std::isalnum(c);
  }

class SampleSelection : public Application
{
public:
  /** Standard class typedefs. */
  typedef SampleSelection        Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SampleSelection, otb::Application);

  /** typedef */
  typedef otb::OGRDataToSamplePositionFilter<
    FloatVectorImageType,
    UInt8ImageType,
    otb::PeriodicSampler>                           PeriodicSamplerType;
  typedef otb::OGRDataToSamplePositionFilter<
    FloatVectorImageType,
    UInt8ImageType,
    otb::RandomSampler>                             RandomSamplerType;
  typedef otb::SamplingRateCalculator               RateCalculatorType;
  
  typedef std::map<std::string, unsigned long>      ClassCountMapType;
  typedef RateCalculatorType::MapRateType           MapRateType;
  typedef itk::VariableLengthVector<float> MeasurementType;
  typedef otb::StatisticsXMLFileReader<MeasurementType> XMLReaderType;

  typedef otb::GeometriesSet GeometriesType;

  typedef otb::GeometriesProjectionFilter ProjectionFilterType;

private:
  SampleSelection()
    {
    m_ReaderStat = XMLReaderType::New();
    m_RateCalculator = RateCalculatorType::New();
    }

  void DoInit() override
  {
    SetName("SampleSelection");
    SetDescription("Selects samples from a training vector data set.");

    // Documentation
    SetDocName("Sample Selection");
    SetDocLongDescription("The application selects a set of samples from geometries "
      "intended for training (they should have a field giving the associated "
      "class). \n\nFirst of all, the geometries must be analyzed by the PolygonClassStatistics application "
      "to compute statistics about the geometries, which are summarized in an xml file. "
      "\nThen, this xml file must be given as input to this application (parameter instats).\n\n"
      "The input support image and the input training vectors shall be given in "
      "parameters 'in' and 'vec' respectively. Only the sampling grid (origin, size, spacing)"
      "will be read in the input image.\n"
      "There are several strategies to select samples (parameter strategy) : \n\n"
      "  - smallest (default) : select the same number of sample in each class" 
      " so that the smallest one is fully sampled.\n"
      "  - constant : select the same number of samples N in each class" 
      " (with N below or equal to the size of the smallest class).\n"
      "  - byclass : set the required number for each class manually, with an input CSV file"
      " (first column is class name, second one is the required samples number).\n\n"
      "  - percent: set a target global percentage of samples to use. Class proportions will be respected. \n\n"
      "  - total: set a target total number of samples to use. Class proportions will be respected. \n\n"
      "There is also a choice on the sampling type to performs : \n\n"
      "  - periodic : select samples uniformly distributed\n"
      "  - random : select samples randomly distributed\n\n"
      "Once the strategy and type are selected, the application outputs samples positions"
      "(parameter out).\n\n"
      
      "The other parameters to look at are : \n\n"
      "  - layer : index specifying from which layer to pick geometries.\n"
      "  - field : set the field name containing the class.\n"
      "  - mask : an optional raster mask can be used to discard samples.\n"
      "  - outrates : allows outputting a CSV file that summarizes the sampling rates for each class.\n"
      
      "\nAs with the PolygonClassStatistics application, different types  of geometry are supported : "
      "polygons, lines, points. \nThe behavior of this application is different for each type of geometry : \n\n"
      "  - polygon: select points whose center is inside the polygon\n"
      "  - lines  : select points intersecting the line\n"
      "  - points : select closest point to the provided point");
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

    AddParameter(ParameterType_OutputFilename, "outrates", "Output rates");
    SetParameterDescription("outrates","Output rates (CSV formatted)");
    MandatoryOff("outrates");

    AddParameter(ParameterType_Choice, "sampler", "Sampler type");
    SetParameterDescription("sampler", "Type of sampling (periodic, pattern based, random)");

    AddChoice("sampler.periodic","Periodic sampler");
    SetParameterDescription("sampler.periodic","Takes samples regularly spaced");

    AddParameter(ParameterType_Int, "sampler.periodic.jitter","Jitter amplitude");
    SetParameterDescription("sampler.periodic.jitter", "Jitter amplitude added during sample selection (0 = no jitter)");
    SetDefaultParameterInt("sampler.periodic.jitter",0);
    MandatoryOff("sampler.periodic.jitter");

    AddChoice("sampler.random","Random sampler");
    SetParameterDescription("sampler.random","The positions to select are randomly shuffled.");

    AddParameter(ParameterType_Choice, "strategy", "Sampling strategy");

    AddChoice("strategy.byclass","Set samples count for each class");
    SetParameterDescription("strategy.byclass","Set samples count for each class");

    AddParameter(ParameterType_InputFilename, "strategy.byclass.in", "Number of samples by class");
    SetParameterDescription("strategy.byclass.in", "Number of samples by class "
      "(CSV format with class name in 1st column and required samples in the 2nd.");

    AddChoice("strategy.constant","Set the same samples counts for all classes");
    SetParameterDescription("strategy.constant","Set the same samples counts for all classes");

    AddParameter(ParameterType_Int, "strategy.constant.nb", "Number of samples for all classes");
    SetParameterDescription("strategy.constant.nb", "Number of samples for all classes");

    AddChoice("strategy.percent","Use a percentage of the samples available for each class");
    SetParameterDescription("strategy.percent","Use a percentage of the samples available for each class");

    AddParameter(ParameterType_Float,"strategy.percent.p","The percentage to use");
    SetParameterDescription("strategy.percent.p","The percentage to use");
    SetMinimumParameterFloatValue("strategy.percent.p",0);
    SetMaximumParameterFloatValue("strategy.percent.p",1);
    SetDefaultParameterFloat("strategy.percent.p",0.5);

    AddChoice("strategy.total","Set the total number of samples to generate, and use class proportions.");
    SetParameterDescription("strategy.total","Set the total number of samples to generate, and use class proportions.");

    AddParameter(ParameterType_Int,"strategy.total.v","The number of samples to generate");
    SetParameterDescription("strategy.total.v","The number of samples to generate");
    SetMinimumParameterIntValue("strategy.total.v",1);
    SetDefaultParameterInt("strategy.total.v",1000);
    
    AddChoice("strategy.smallest","Set same number of samples for all classes, with the smallest class fully sampled");
    SetParameterDescription("strategy.smallest","Set same number of samples for all classes, with the smallest class fully sampled");

    AddChoice("strategy.all","Take all samples");
    SetParameterDescription("strategy.all","Take all samples");

    // Default strategy : smallest
    SetParameterString("strategy","smallest");

    AddParameter(ParameterType_ListView, "field", "Field Name");
    SetParameterDescription("field","Name of the field carrying the class name in the input vectors.");
    SetListViewSingleSelectionMode("field",true);

    AddParameter(ParameterType_Int, "layer", "Layer Index");
    SetParameterDescription("layer", "Layer index to read in the input vector file.");
    MandatoryOff("layer");
    SetDefaultParameterInt("layer",0);

    ElevationParametersHandler::AddElevationParameters(this, "elev");

    AddRANDParameter();

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "support_image.tif");
    SetDocExampleParameterValue("vec", "variousVectors.sqlite");
    SetDocExampleParameterValue("field", "label");
    SetDocExampleParameterValue("instats","apTvClPolygonClassStatisticsOut.xml");
    SetDocExampleParameterValue("out","resampledVectors.sqlite");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
 if ( HasValue("vec") )
      {
      std::string vectorFile = GetParameterString("vec");
      ogr::DataSource::Pointer ogrDS =
        ogr::DataSource::New(vectorFile, ogr::DataSource::Modes::Read);
      ogr::Layer layer = ogrDS->GetLayer(this->GetParameterInt("layer"));
      ogr::Feature feature = layer.ogr().GetNextFeature();

      ClearChoices("field");
      
      for(int iField=0; iField<feature.ogr().GetFieldCount(); iField++)
        {
        std::string key, item = feature.ogr().GetFieldDefnRef(iField)->GetNameRef();
        key = item;
        std::string::iterator end = std::remove_if(key.begin(),key.end(),IsNotAlphaNum);
        std::transform(key.begin(), end, key.begin(), tolower);
        
        OGRFieldType fieldType = feature.ogr().GetFieldDefnRef(iField)->GetType();
        
        if(fieldType == OFTString || fieldType == OFTInteger || fieldType == OFTInteger64)
          {
          std::string tmpKey="field."+key.substr(0, end - key.begin());
          AddChoice(tmpKey,item);
          }
        }
      }
  }

  void DoExecute() override
    {
    // Clear state
    m_RateCalculator->ClearRates();

    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");

    // Get field name
    std::vector<int> selectedCFieldIdx = GetSelectedItems("field");
    
    if(selectedCFieldIdx.empty())
      {
      otbAppLogFATAL(<<"No field has been selected for data labelling!");
      }
    
    std::vector<std::string> cFieldNames = GetChoiceNames("field");  
    std::string fieldName = cFieldNames[selectedCFieldIdx.front()];
    
    m_ReaderStat->SetFileName(this->GetParameterString("instats"));
    ClassCountMapType classCount = m_ReaderStat->GetStatisticMapByName<ClassCountMapType>("samplesPerClass");
    m_RateCalculator->SetClassCount(classCount);
    
    switch (this->GetParameterInt("strategy"))
      {
      // byclass
      case 0:
        {
        otbAppLogINFO("Sampling strategy : set number of samples for each class");
        ClassCountMapType requiredCount = 
          otb::SamplingRateCalculator::ReadRequiredSamples(this->GetParameterString("strategy.byclass.in"));
        m_RateCalculator->SetNbOfSamplesByClass(requiredCount);
        }
      break;
      // constant
      case 1:
        {
        otbAppLogINFO("Sampling strategy : set a constant number of samples for all classes");
        m_RateCalculator->SetNbOfSamplesAllClasses(GetParameterInt("strategy.constant.nb"));
        }
      break;
      // percent
      case 2:
      {
      otbAppLogINFO("Sampling strategy: set a percentage of samples for each class.");
      m_RateCalculator->SetPercentageOfSamples(this->GetParameterFloat("strategy.percent.p"));
      }
      break;
      // total
      case 3:
      {
      otbAppLogINFO("Sampling strategy: set the total number of samples to generate, use classes proportions.");
      m_RateCalculator->SetTotalNumberOfSamples(this->GetParameterInt("strategy.total.v"));
      }
      break;

      // smallest class
      case 4:
        {
        otbAppLogINFO("Sampling strategy : fit the number of samples based on the smallest class");
        m_RateCalculator->SetMinimumNbOfSamplesByClass();
        }
      break;
      // all samples
      case 5:
        {
        otbAppLogINFO("Sampling strategy : take all samples");
        m_RateCalculator->SetAllSamples();
        }
      break;
      default:
        otbAppLogFATAL("Strategy mode unknown :"<<this->GetParameterString("strategy"));
      break;
      }
      
    if (IsParameterEnabled("outrates") && HasValue("outrates"))
      {
      m_RateCalculator->Write(this->GetParameterString("outrates"));
      }
    
    MapRateType rates = m_RateCalculator->GetRatesByClass();
    std::ostringstream oss;
    oss << " className  requiredSamples  totalSamples  rate" << std::endl;
    MapRateType::const_iterator itRates = rates.begin();
    unsigned int overflowCount = 0;
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
    otbAppLogINFO("Sampling rates : " << oss.str());
    if (overflowCount)
      {
      std::string plural(overflowCount>1?"s":"");
      otbAppLogWARNING(<< overflowCount << " case"<<plural<<" of overflow detected! (requested number of samples higher than total available samples)");
      }

    // Open input geometries
    otb::ogr::DataSource::Pointer vectors =
      otb::ogr::DataSource::New(this->GetParameterString("vec"));

    // Reproject geometries
    FloatVectorImageType::Pointer inputImg = this->GetParameterImage("in");
    std::string imageProjectionRef = inputImg->GetProjectionRef();
    FloatVectorImageType::ImageKeywordlistType imageKwl =
      inputImg->GetImageKeywordlist();
    std::string vectorProjectionRef =
      vectors->GetLayer(GetParameterInt("layer")).GetProjectionRef();

    otb::ogr::DataSource::Pointer reprojVector = vectors;
    GeometriesType::Pointer inputGeomSet;
    ProjectionFilterType::Pointer geometriesProjFilter;
    GeometriesType::Pointer outputGeomSet;
    bool doReproj = true;
    const OGRSpatialReference imgOGRSref = 
        OGRSpatialReference( imageProjectionRef.c_str() );
    const OGRSpatialReference vectorOGRSref = 
        OGRSpatialReference( vectorProjectionRef.c_str() );
    // don't reproject for these cases
    if (  vectorProjectionRef.empty()
       || ( imgOGRSref.IsSame( &vectorOGRSref ) )
       || ( imageProjectionRef.empty() && imageKwl.GetSize() == 0) )
      doReproj = false;
  
    if (doReproj)
      {
      inputGeomSet = GeometriesType::New(vectors);
      reprojVector = otb::ogr::DataSource::New();
      outputGeomSet = GeometriesType::New(reprojVector);
      // Filter instantiation
      geometriesProjFilter = ProjectionFilterType::New();
      geometriesProjFilter->SetInput(inputGeomSet);
      if (imageProjectionRef.empty())
        {
        geometriesProjFilter->SetOutputKeywordList(inputImg->GetImageKeywordlist()); // nec qd capteur
        }
      geometriesProjFilter->SetOutputProjectionRef(imageProjectionRef);
      geometriesProjFilter->SetOutput(outputGeomSet);
      otbAppLogINFO("Reprojecting input vectors...");
      geometriesProjFilter->Update();
      }

    // Create output dataset for sample positions
    otb::ogr::DataSource::Pointer outputSamples =
      otb::ogr::DataSource::New(this->GetParameterString("out"),otb::ogr::DataSource::Modes::Overwrite);
    
    switch (this->GetParameterInt("sampler"))
      {
      // periodic
      case 0:
        {
        PeriodicSamplerType::SamplerParameterType param;
        param.Offset = 0;
        param.MaxJitter = this->GetParameterInt("sampler.periodic.jitter");
        param.MaxBufferSize = 100000000UL;
        PeriodicSamplerType::Pointer periodicFilt = PeriodicSamplerType::New();
        periodicFilt->SetInput(this->GetParameterImage("in"));
        periodicFilt->SetOGRData(reprojVector);
        periodicFilt->SetOutputPositionContainerAndRates(outputSamples, rates);
        periodicFilt->SetFieldName(fieldName);
        periodicFilt->SetLayerIndex(this->GetParameterInt("layer"));
        periodicFilt->SetSamplerParameters(param);
        if (IsParameterEnabled("mask") && HasValue("mask"))
          {
          periodicFilt->SetMask(this->GetParameterImage<UInt8ImageType>("mask"));
          }
        periodicFilt->GetStreamer()->SetAutomaticTiledStreaming(this->GetParameterInt("ram"));
        AddProcess(periodicFilt->GetStreamer(),"Selecting positions with periodic sampler...");
        periodicFilt->Update();
        }
      break;
      // random
      case 1:
        {
        RandomSamplerType::Pointer randomFilt = RandomSamplerType::New();
        randomFilt->SetInput(this->GetParameterImage("in"));
        randomFilt->SetOGRData(reprojVector);
        randomFilt->SetOutputPositionContainerAndRates(outputSamples, rates);
        randomFilt->SetFieldName(fieldName);
        randomFilt->SetLayerIndex(this->GetParameterInt("layer"));
        if (IsParameterEnabled("mask") && HasValue("mask"))
          {
          randomFilt->SetMask(this->GetParameterImage<UInt8ImageType>("mask"));
          }
        randomFilt->GetStreamer()->SetAutomaticTiledStreaming(this->GetParameterInt("ram"));
        AddProcess(randomFilt->GetStreamer(),"Selecting positions with random sampler...");
        randomFilt->Update();

        randomFilt = RandomSamplerType::New();
        }
      break;
      default:
        otbAppLogFATAL("Sampler type unknown :"<<this->GetParameterString("sampler"));
      break;
      }
  }

  RateCalculatorType::Pointer m_RateCalculator;
  XMLReaderType::Pointer m_ReaderStat;
};

} // end of namespace Wrapper
} // end of namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::SampleSelection)
