/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "otbOGRDataSourceWrapper.h"
#include "otbOGRFeatureWrapper.h"
#include "otbStatisticsXMLFileWriter.h"
#include "itkVariableLengthVector.h"

#include <time.h>

namespace otb
{
namespace Wrapper
{
class ComputeOGRLayersFeaturesStatistics : public Application
{
public:
  typedef ComputeOGRLayersFeaturesStatistics Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  itkNewMacro(Self)
;

  itkTypeMacro(ComputeOGRLayersFeaturesStatistics, otb::Application)
;

private:
  void DoInit() override
  {
    SetName("ComputeOGRLayersFeaturesStatistics");
    SetDescription("Compute statistics of the features in a set of OGR Layers");

    SetDocLongDescription("Compute statistics (mean and standard deviation) of the features in a set of OGR Layers, and write them in an XML file. This XML file can then be used by the training application.");
    SetDocLimitations("Experimental. For now only shapefiles are supported.");
    SetDocAuthors("David Youssefi during internship at CNES");
    SetDocSeeAlso("OGRLayerClassifier,TrainVectorClassifier");
    AddDocTag(Tags::Segmentation);

    AddParameter(ParameterType_InputVectorData, "inshp", "Vector Data");
    SetParameterDescription("inshp","Name of the input shapefile");

    AddParameter(ParameterType_OutputFilename, "outstats", "Output XML file");
    SetParameterDescription("outstats", "XML file containing mean and variance of each feature.");

    AddParameter(ParameterType_ListView,  "feat", "Feature");
    SetParameterDescription("feat","List of features to consider for statistics.");

    // Doc example parameter settings
    SetDocExampleParameterValue("inshp", "vectorData.shp");
    SetDocExampleParameterValue("outstats", "results.xml");
    SetDocExampleParameterValue("feat", "perimeter");

  SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    if ( HasValue("inshp") )
      {
      std::string shapefile = GetParameterString("inshp");

       otb::ogr::DataSource::Pointer ogrDS;
       otb::ogr::Layer layer(nullptr, false);

       OGRSpatialReference oSRS("");
       std::vector<std::string> options;
       
       ogrDS = otb::ogr::DataSource::New(shapefile, otb::ogr::DataSource::Modes::Read);
       std::string layername = itksys::SystemTools::GetFilenameName(shapefile);
       layername = layername.substr(0,layername.size()-4);
       layer = ogrDS->GetLayer(0);

       otb::ogr::Feature feature = layer.ogr().GetNextFeature();
       ClearChoices("feat");

       std::vector<std::string> choiceKeys;

       for(int iField=0; iField<feature.ogr().GetFieldCount(); iField++)
         {
           std::string key, item = feature.ogr().GetFieldDefnRef(iField)->GetNameRef();
           key = item;

           // Transform chain : lowercase and alphanumerical
           key.erase(std::remove_if(key.begin(), key.end(), std::not1(std::ptr_fun(::isalnum))), key.end());
           std::transform(key.begin(), key.end(), key.begin(), tolower);

           // Key must be unique
           choiceKeys = GetChoiceKeys("feat");
           while(choiceKeys.end() != std::find(choiceKeys.begin(), choiceKeys.end(), key) )
             key.append("0");

           key="feat."+key;
           AddChoice(key,item);
         }
      }
  }

  void DoExecute() override
  {
      clock_t tic = clock();

      std::string shapefile = GetParameterString("inshp");
      std::string XMLfile = GetParameterString("outstats");

      otb::ogr::DataSource::Pointer source = otb::ogr::DataSource::New(shapefile, otb::ogr::DataSource::Modes::Read);
  
      otb::ogr::Layer layer = source->GetLayer(0);
      bool goesOn = true;
      otb::ogr::Feature feature = layer.ogr().GetNextFeature();
  
      typedef double ValueType;
      typedef itk::VariableLengthVector<ValueType> MeasurementType;
      std::vector<MeasurementType> featValue;

      const std::vector<int>::size_type nbFeatures = GetSelectedItems("feat").size();
      
      if(feature.addr())
       while(goesOn)
         {
           MeasurementType mv; mv.SetSize(nbFeatures);
           
           for(unsigned int idx=0; idx < nbFeatures; ++idx)
             mv[idx] = feature.ogr().GetFieldAsDouble(GetSelectedItems("feat")[idx]);
           
           featValue.push_back(mv);
           feature = layer.ogr().GetNextFeature();
           goesOn = feature.addr() != nullptr;
         }
  
      MeasurementType mean; mean.SetSize(nbFeatures);
      MeasurementType stddev; stddev.SetSize(nbFeatures);

      for(unsigned int featIt=0; featIt<nbFeatures; featIt++){
       double sum = 0.0; for(unsigned add=0; add<featValue.size(); add++)  sum += featValue[add][featIt];
       mean[featIt] =  sum / featValue.size();
       double accum = 0.0; for(unsigned add=0; add<featValue.size(); add++) accum += (featValue[add][featIt] - mean[featIt]) * (featValue[add][featIt] - mean[featIt]);
       stddev[featIt] = sqrt(accum / (featValue.size()-1)); }
  
      typedef otb::StatisticsXMLFileWriter<MeasurementType> StatisticsWriter;
      StatisticsWriter::Pointer writer = StatisticsWriter::New();
      writer->SetFileName(XMLfile);
      writer->AddInput("mean", mean);
      writer->AddInput("stddev", stddev);
      writer->Update();

      clock_t toc = clock();
      otbAppLogINFO( "Elapsed: "<< ((double)(toc - tic) / CLOCKS_PER_SEC)<<" seconds.");
  }

};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ComputeOGRLayersFeaturesStatistics)


