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

#ifndef otbSampleAugmentationFilter_txx
#define otbSampleAugmentationFilter_txx

#include "otbSampleAugmentationFilter.h"
#include "stdint.h" //needed for uintptr_t

namespace otb
{

SampleAugmentationFilter
::SampleAugmentationFilter() : m_ClassFieldName("class")
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);
  this->ProcessObject::SetNthOutput(0, this->MakeOutput(0) );
}


typename SampleAugmentationFilter::DataObjectPointer
SampleAugmentationFilter
::MakeOutput(DataObjectPointerArraySizeType itkNotUsed(idx))
{
  return static_cast< DataObjectPointer >(OGRDataSourceType::New().GetPointer());
}

const typename SampleAugmentationFilter::OGRDataSourceType *
SampleAugmentationFilter
::GetOutput()
{
  return static_cast< const OGRDataSourceType *>(
    this->ProcessObject::GetOutput(0));
}

void
SampleAugmentationFilter
::SetInput(const otb::ogr::DataSource* ds)
{
  this->Superclass::SetNthInput(0, const_cast<otb::ogr::DataSource *>(ds));
}

const typename SampleAugmentationFilter::OGRDataSourceType *
SampleAugmentationFilter
::GetInput(unsigned int idx)
{
  return static_cast<const OGRDataSourceType *>
    (this->itk::ProcessObject::GetInput(idx));
}

void
SampleAugmentationFilter
::SetOutputSamples(ogr::DataSource* data)
{
  this->SetNthOutput(0,data);
}


void
SampleAugmentationFilter
::GenerateData(void)
{

  OGRDataSourcePointerType inputDS = dynamic_cast<OGRDataSourceType*>(this->itk::ProcessObject::GetInput(0));
  auto outputDS = static_cast<ogr::DataSource *>(this->itk::ProcessObject::GetOutput(0));
  auto inSamples = this->extractSamples(inputDS, m_Layer,
                                        m_ClassFieldName,
                                        m_Label,
                                        m_ExcludedFeatures);
  SampleVectorType newSamples;
  switch (m_Strategy)
    {
    case Strategy::Replicate:
    {
    sampleAugmentation::replicateSamples(inSamples, m_NumberOfSamples,
                                         newSamples);
    }
    break;
    case Strategy::Jitter:
    {
    sampleAugmentation::jitterSamples(inSamples, m_NumberOfSamples,
                                      newSamples,
                                      m_StdFactor,
                                      m_Seed);
    }
    break;
    case Strategy::Smote:
    {
    sampleAugmentation::smote(inSamples, m_NumberOfSamples,
                              newSamples,
                              m_SmoteNeighbors,
                              m_Seed);
    }
    break;
    }
  this->sampleToOGRFeatures(inputDS, outputDS, newSamples, m_Layer,
                            m_ClassFieldName,
                            m_Label,
                            m_ExcludedFeatures);


  //  this->SetNthOutput(0,outputDS);
}

/** Extracts the samples of a single class from the vector data to a
* vector and excludes some unwanted features.
*/
SampleAugmentationFilter::SampleVectorType 
SampleAugmentationFilter
::extractSamples(const ogr::DataSource::Pointer vectors, 
                 size_t layerName,
                 const std::string& classField, const int label,
                 const std::vector<std::string>& excludedFeatures)
{
  ogr::Layer layer = vectors->GetLayer(layerName);
  ogr::Feature feature = layer.ogr().GetNextFeature();
  if(feature.addr() == 0)
    {
    itkExceptionMacro("Layer " << layerName << " of input sample file is empty.\n");
    }
  int cFieldIndex = feature.ogr().GetFieldIndex( classField.c_str() );
  if( cFieldIndex < 0 )
    {
    itkExceptionMacro( "The field name for class label (" << classField
                       << ") has not been found in the vector file " );
    }

  auto numberOfFields = feature.ogr().GetFieldCount();
  auto excludedIds = this->getExcludedFeaturesIds(excludedFeatures, layer);
  SampleVectorType samples;
  bool goesOn{feature.addr() != 0};
  while( goesOn )
    {
    // Retrieve all the features for each field in the ogr layer.
    if(feature.ogr().GetFieldAsInteger(classField.c_str()) == label)
      {

      SampleType mv;
      for(auto idx=0; idx<numberOfFields; ++idx)
        {
        if(excludedIds.find(idx) == excludedIds.cend() &&
           this->isNumericField(feature, idx))
          mv.push_back(feature.ogr().GetFieldAsDouble(idx));
        }
      samples.push_back(mv); 
      }
    feature = layer.ogr().GetNextFeature();
    goesOn = feature.addr() != 0;
    }
  return samples;
}

void 
SampleAugmentationFilter
::sampleToOGRFeatures(const ogr::DataSource::Pointer& vectors,
                      ogr::DataSource* output, 
               const SampleAugmentationFilter::SampleVectorType& samples,
               const size_t layerName,
                  const std::string& classField, int label,
                  const std::vector<std::string>& excludedFeatures)
{

  auto inputLayer = vectors->GetLayer(layerName);
  auto excludedIds = this->getExcludedFeaturesIds(excludedFeatures, inputLayer);

  OGRSpatialReference * oSRS = nullptr;
  if (inputLayer.GetSpatialRef())
    {
    oSRS = inputLayer.GetSpatialRef()->Clone();
    }
  OGRFeatureDefn &layerDefn = inputLayer.GetLayerDefn();

  auto outputLayer = output->CreateLayer(inputLayer.GetName(), oSRS, 
                                         inputLayer.GetGeomType());
  for (int k=0 ; k < layerDefn.GetFieldCount() ; k++)
    {
    OGRFieldDefn originDefn(layerDefn.GetFieldDefn(k));
    ogr::FieldDefn fieldDefn(originDefn);
    outputLayer.CreateField(fieldDefn);
    }

  auto featureCount = outputLayer.GetFeatureCount(false);
  auto templateFeature = this->selectTemplateFeature(inputLayer, classField, label);
  for(const auto& sample : samples)
    {
    ogr::Feature dstFeature(outputLayer.GetLayerDefn());
    dstFeature.SetFrom( templateFeature, TRUE );
    dstFeature.SetFID(++featureCount);
    auto sampleFieldCounter = 0;
    for (int k=0 ; k < layerDefn.GetFieldCount() ; k++)
      {
      if(excludedIds.find(k) == excludedIds.cend() &&
         this->isNumericField(dstFeature, k))
        {
        dstFeature.ogr().SetField(k, sample[sampleFieldCounter++]);
        }
      }
    outputLayer.CreateFeature( dstFeature );
    }
}

               std::set<size_t> 
               SampleAugmentationFilter
               ::getExcludedFeaturesIds(const std::vector<std::string>& excludedFeatures,
                                        const ogr::Layer& inputLayer)
                  {
                    auto feature = *(inputLayer).begin();
                    std::set<size_t> excludedIds;
                    if( excludedFeatures.size() != 0)
                      {
                      for(const auto& fieldName : excludedFeatures)
                        {
                        auto idx = feature.ogr().GetFieldIndex( fieldName.c_str() );
                        excludedIds.insert(idx);
                        }
                      }
                    return excludedIds;
                  }

               bool 
SampleAugmentationFilter
::isNumericField(const ogr::Feature& feature,
                 const int idx)
{
  OGRFieldType fieldType = feature.ogr().GetFieldDefnRef(idx)->GetType();
    return (fieldType == OFTInteger 
            || ogr::version_proxy::IsOFTInteger64( fieldType ) 
            || fieldType == OFTReal);
}

ogr::Feature
SampleAugmentationFilter
::selectTemplateFeature(const ogr::Layer& inputLayer, 
                        const std::string& classField, int label)
{
  auto featureIt = inputLayer.begin();
  bool goesOn{(*featureIt).addr() != 0};
  while( goesOn )
    {
    if((*featureIt).ogr().GetFieldAsInteger(classField.c_str()) == label)
      {
      return *featureIt;
      }
    ++featureIt;
    }
  return *(inputLayer.begin());
}
} // end namespace otb

#endif
