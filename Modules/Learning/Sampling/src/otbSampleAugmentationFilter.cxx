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

#include "otbSampleAugmentationFilter.h"
#include "stdint.h" //needed for uintptr_t

namespace otb
{

SampleAugmentationFilter
::SampleAugmentationFilter() : m_ClassFieldName{"class"}, m_Layer{0}, m_Label{1},
                               m_Strategy{SampleAugmentationFilter::Strategy::Replicate},
                               m_NumberOfSamples{100}, m_StdFactor{10.0}, 
                               m_SmoteNeighbors{5}, m_Seed{0}
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
  auto inSamples = this->ExtractSamples(inputDS, m_Layer,
                                        m_ClassFieldName,
                                        m_Label,
                                        m_ExcludedFields);
  SampleVectorType newSamples;
  switch (m_Strategy)
    {
    case Strategy::Replicate:
    {
    sampleAugmentation::ReplicateSamples(inSamples, m_NumberOfSamples,
                                         newSamples);
    }
    break;
    case Strategy::Jitter:
    {
    sampleAugmentation::JitterSamples(inSamples, m_NumberOfSamples,
                                      newSamples,
                                      m_StdFactor,
                                      m_Seed);
    }
    break;
    case Strategy::Smote:
    {
    sampleAugmentation::Smote(inSamples, m_NumberOfSamples,
                              newSamples,
                              m_SmoteNeighbors,
                              m_Seed);
    }
    break;
    }
  this->SampleToOGRFeatures(inputDS, outputDS, newSamples, m_Layer,
                            m_ClassFieldName,
                            m_Label,
                            m_ExcludedFields);


  //  this->SetNthOutput(0,outputDS);
}

/** Extracts the samples of a single class from the vector data to a
* vector and excludes some unwanted features.
*/
SampleAugmentationFilter::SampleVectorType 
SampleAugmentationFilter
::ExtractSamples(const ogr::DataSource::Pointer vectors, 
                 size_t layerName,
                 const std::string& classField, const int label,
                 const std::vector<std::string>& excludedFields)
{
  ogr::Layer layer = vectors->GetLayer(layerName);
  auto featureIt = layer.begin();
  if(featureIt==layer.end())
    {
    itkExceptionMacro("Layer " << layerName << " of input sample file is empty.\n");
    }
  int cFieldIndex = (*featureIt).ogr().GetFieldIndex( classField.c_str() );
  if( cFieldIndex < 0 )
    {
    itkExceptionMacro( "The field name for class label (" << classField
                       << ") has not been found in the vector file " );
    }

  auto numberOfFields = (*featureIt).ogr().GetFieldCount();
  auto excludedIds = this->GetExcludedFieldsIds(excludedFields, layer);
  SampleVectorType samples;
  int sampleCount{0};
  while( featureIt!=layer.end() )
    {
    // Retrieve all the features for each field in the ogr layer.
    if((*featureIt).ogr().GetFieldAsInteger(classField.c_str()) == label)
      {

      SampleType mv;
      for(auto idx=0; idx<numberOfFields; ++idx)
        {
        if(excludedIds.find(idx) == excludedIds.cend() &&
           this->IsNumericField((*featureIt), idx))
          mv.push_back((*featureIt).ogr().GetFieldAsDouble(idx));
        }
      samples.push_back(mv); 
      ++sampleCount;
      }
    ++featureIt;
    }
  if(sampleCount==0)
    {
    itkExceptionMacro("Could not find any samples in layer " << layerName <<
                      " with label " << label << '\n');
    }
  return samples;
}

void 
SampleAugmentationFilter
::SampleToOGRFeatures(const ogr::DataSource::Pointer& vectors,
                      ogr::DataSource* output, 
                      const SampleAugmentationFilter::SampleVectorType& samples,
                      const size_t layerName,
                      const std::string& classField, int label,
                      const std::vector<std::string>& excludedFields)
{

  auto inputLayer = vectors->GetLayer(layerName);
  auto excludedIds = this->GetExcludedFieldsIds(excludedFields, inputLayer);

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
  auto templateFeature = this->SelectTemplateFeature(inputLayer, classField, label);
  for(const auto& sample : samples)
    {
    ogr::Feature dstFeature(outputLayer.GetLayerDefn());
    dstFeature.SetFrom( templateFeature, TRUE );
    dstFeature.SetFID(++featureCount);
    auto sampleFieldCounter = 0;
    for (int k=0 ; k < layerDefn.GetFieldCount() ; k++)
      {
      if(excludedIds.find(k) == excludedIds.cend() &&
         this->IsNumericField(dstFeature, k))
        {
        dstFeature.ogr().SetField(k, sample[sampleFieldCounter++]);
        }
      }
    outputLayer.CreateFeature( dstFeature );
    }
}

std::set<size_t> 
SampleAugmentationFilter
::GetExcludedFieldsIds(const std::vector<std::string>& excludedFields,
                       const ogr::Layer& inputLayer)
{
  auto feature = *(inputLayer).begin();
  std::set<size_t> excludedIds;
  if( excludedFields.size() != 0)
    {
    for(const auto& fieldName : excludedFields)
      {
      auto idx = feature.ogr().GetFieldIndex( fieldName.c_str() );
      excludedIds.insert(idx);
      }
    }
  return excludedIds;
}

bool 
SampleAugmentationFilter
::IsNumericField(const ogr::Feature& feature,
                 const int idx)
{
  OGRFieldType fieldType = feature.ogr().GetFieldDefnRef(idx)->GetType();
  return (fieldType == OFTInteger 
          || ogr::version_proxy::IsOFTInteger64( fieldType ) 
          || fieldType == OFTReal);
}

ogr::Feature
SampleAugmentationFilter
::SelectTemplateFeature(const ogr::Layer& inputLayer, 
                        const std::string& classField, int label)
{
  auto wh = std::find_if(inputLayer.begin(), inputLayer.end(),
                         [&](auto& featureIt) 
                         { 
                           return featureIt.ogr().GetFieldAsInteger(classField.c_str()) == 
                           label; 
                         });
  return (wh == inputLayer.end()) ? *(inputLayer.begin()) : *wh;

}
} // end namespace otb

