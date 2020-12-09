/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbVectorPrediction_hxx
#define otbVectorPrediction_hxx

#include "otbVectorPrediction.h"

namespace otb
{
namespace Wrapper
{

template <bool RegressionMode>
void           VectorPrediction<RegressionMode>::DoInit()
{
  DoInitSpecialization();

  // Assert that the all needed parameters have ben definied in DoInitSpecialization
  assert(GetParameterByKey("in") != nullptr);
  assert(GetParameterByKey("instat") != nullptr);
  assert(GetParameterByKey("model") != nullptr);
  assert(GetParameterByKey("cfield") != nullptr);
  assert(GetParameterByKey("feat") != nullptr);
  assert(GetParameterByKey("out") != nullptr);
}

template <bool RegressionMode>
void           VectorPrediction<RegressionMode>::DoUpdateParameters()
{
  if (HasValue("in"))
  {
    auto shapefileName = GetParameterString("in");

    auto            ogrDS     = otb::ogr::DataSource::New(shapefileName, otb::ogr::DataSource::Modes::Read);
    auto            layer     = ogrDS->GetLayer(0);
    OGRFeatureDefn& layerDefn = layer.GetLayerDefn();

    ClearChoices("feat");

    FieldParameter::TypeFilterType typeFilter = GetTypeFilter("feat");
    for (int iField = 0; iField < layerDefn.GetFieldCount(); iField++)
    {
      auto        fieldDefn = layerDefn.GetFieldDefn(iField);
      std::string item      = fieldDefn->GetNameRef();
      std::string key(item);
      key.erase(std::remove_if(key.begin(), key.end(), [](char c) { return !std::isalnum(c); }), key.end());
      std::transform(key.begin(), key.end(), key.begin(), tolower);
      auto fieldType = fieldDefn->GetType();

      if (typeFilter.empty() || std::find(typeFilter.begin(), typeFilter.end(), fieldType) != std::end(typeFilter))
      {
        std::string tmpKey = "feat." + key;
        AddChoice(tmpKey, item);
      }
    }
  }
}

template <bool RegressionMode>
typename VectorPrediction<RegressionMode>::ListSampleType::Pointer
VectorPrediction<RegressionMode>
::ReadInputListSample(otb::ogr::Layer const& layer)
{
  typename ListSampleType::Pointer input = ListSampleType::New();

  const auto nbFeatures = GetSelectedItems("feat").size();
  input->SetMeasurementVectorSize(nbFeatures);
  std::vector<int> featureFieldIndex(nbFeatures, -1);

  ogr::Layer::const_iterator it_feat = layer.cbegin();
  for (unsigned int i = 0; i < nbFeatures; i++)
  {
    try
    {
    featureFieldIndex[i] = (*it_feat).GetFieldIndex(GetChoiceNames("feat")[GetSelectedItems("feat")[i]]);
    }
    catch(...)
    {
    otbAppLogFATAL("The field name for feature " << GetChoiceNames("feat")[GetSelectedItems("feat")[i]] << " has not been found" << std::endl);
    }
  }

  for (auto const& feature : layer)
  {
    MeasurementType mv(nbFeatures);
    for (unsigned int idx = 0; idx < nbFeatures; ++idx)
    {
      auto field = feature[featureFieldIndex[idx]];
      switch (field.GetType())
      {
      case OFTInteger:
      case OFTInteger64:
        mv[idx] = static_cast<ValueType>(field.template GetValue<int>());
        break;
      case OFTReal:
        mv[idx] = static_cast<ValueType>(field.template GetValue<double>());
        break;
      default:
        itkExceptionMacro(<< "incorrect field type: " << field.GetType() << ".");
      }
    }
    input->PushBack(mv);
  }
  return input;
}


template <bool                                                     RegressionMode>
typename VectorPrediction<RegressionMode>::ListSampleType::Pointer VectorPrediction<RegressionMode>::NormalizeListSample(ListSampleType::Pointer input)
{
  const int nbFeatures = GetSelectedItems("feat").size();

  // Statistics for shift/scale
  MeasurementType meanMeasurementVector;
  MeasurementType stddevMeasurementVector;
  if (HasValue("instat") && IsParameterEnabled("instat"))
  {
    typename StatisticsReader::Pointer statisticsReader = StatisticsReader::New();
    std::string                        XMLfile          = GetParameterString("instat");
    statisticsReader->SetFileName(XMLfile);
    meanMeasurementVector   = statisticsReader->GetStatisticVectorByName("mean");
    stddevMeasurementVector = statisticsReader->GetStatisticVectorByName("stddev");
  }
  else
  {
    meanMeasurementVector.SetSize(nbFeatures);
    meanMeasurementVector.Fill(0.);
    stddevMeasurementVector.SetSize(nbFeatures);
    stddevMeasurementVector.Fill(1.);
  }

  typename ShiftScaleFilterType::Pointer trainingShiftScaleFilter = ShiftScaleFilterType::New();
  trainingShiftScaleFilter->SetInput(input);
  trainingShiftScaleFilter->SetShifts(meanMeasurementVector);
  trainingShiftScaleFilter->SetScales(stddevMeasurementVector);
  trainingShiftScaleFilter->Update();
  otbAppLogINFO("mean used: " << meanMeasurementVector);
  otbAppLogINFO("standard deviation used: " << stddevMeasurementVector);

  otbAppLogINFO("Loading model");

  return trainingShiftScaleFilter->GetOutput();
}


template <bool                RegressionMode>
otb::ogr::DataSource::Pointer VectorPrediction<RegressionMode>::ReopenDataSourceInUpdateMode(ogr::DataSource::Pointer source, ogr::Layer& layer,
                                                                                             ogr::DataSource::Pointer buffer)
{
  ogr::DataSource::Pointer output;
  // Update mode
  otbAppLogINFO("Update input vector data.");
  // fill temporary buffer for the transfer
  otb::ogr::Layer inputLayer = layer;
  layer                      = buffer->CopyLayer(inputLayer, std::string("Buffer"));
  // close input data source
  source->Clear();
  // Re-open input data source in update mode
  output = otb::ogr::DataSource::New(GetParameterString("in"), otb::ogr::DataSource::Modes::Update_LayerUpdate);
  return output;
}

template <bool                RegressionMode>
otb::ogr::DataSource::Pointer VectorPrediction<RegressionMode>::CreateOutputDataSource(ogr::Layer& layer)
{
  ogr::DataSource::Pointer output;
  // Create new OGRDataSource
  output                   = ogr::DataSource::New(GetParameterString("out"), ogr::DataSource::Modes::Overwrite);
  otb::ogr::Layer newLayer = output->CreateLayer(GetParameterString("out"), const_cast<OGRSpatialReference*>(layer.GetSpatialRef()), layer.GetGeomType());
  // Copy existing fields
  OGRFeatureDefn& inLayerDefn = layer.GetLayerDefn();
  for (int k = 0; k < inLayerDefn.GetFieldCount(); k++)
  {
    OGRFieldDefn fieldDefn(inLayerDefn.GetFieldDefn(k));
    newLayer.CreateField(fieldDefn);
  }
  return output;
}

template <bool RegressionMode>
void VectorPrediction<RegressionMode>::AddPredictionField(otb::ogr::Layer& outLayer, otb::ogr::Layer const& layer, bool computeConfidenceMap)
{
  OGRFeatureDefn& layerDefn = layer.GetLayerDefn();

  const OGRFieldType labelType = RegressionMode ? OFTReal : OFTInteger;

  int idx = layerDefn.GetFieldIndex(GetParameterString("cfield").c_str());
  if (idx >= 0)
  {
    if (layerDefn.GetFieldDefn(idx)->GetType() != labelType)
      itkExceptionMacro("Field name " << GetParameterString("cfield") << " already exists with a different type!");
  }
  else
  {
    OGRFieldDefn   predictedField(GetParameterString("cfield").c_str(), labelType);
    ogr::FieldDefn predictedFieldDef(predictedField);
    outLayer.CreateField(predictedFieldDef);
  }

  // Add confidence field in the output layer
  if (computeConfidenceMap)
  {
    idx = layerDefn.GetFieldIndex(confFieldName.c_str());
    if (idx >= 0)
    {
      if (layerDefn.GetFieldDefn(idx)->GetType() != OFTReal)
        itkExceptionMacro("Field name " << confFieldName << " already exists with a different type!");
    }
    else
    {
      OGRFieldDefn confidenceField(confFieldName.c_str(), OFTReal);
      confidenceField.SetWidth(confidenceField.GetWidth());
      confidenceField.SetPrecision(confidenceField.GetPrecision());
      ogr::FieldDefn confFieldDefn(confidenceField);
      outLayer.CreateField(confFieldDefn);
    }
  }
}

template <bool RegressionMode>
void VectorPrediction<RegressionMode>::FillOutputLayer(otb::ogr::Layer& outLayer, otb::ogr::Layer const& layer, typename LabelListSampleType::Pointer target,
                                                       typename ConfidenceListSampleType::Pointer quality, bool updateMode, bool computeConfidenceMap)
{
  unsigned int count          = 0;
  std::string  classfieldname = GetParameterString("cfield");
  for (auto const& feature : layer)
  {
    ogr::Feature dstFeature(outLayer.GetLayerDefn());
    dstFeature.SetFrom(feature, TRUE);
    dstFeature.SetFID(feature.GetFID());
    auto field = dstFeature[classfieldname];
    switch (field.GetType())
    {
    case OFTInteger64:
    case OFTInteger:
      field.template SetValue<int>(target->GetMeasurementVector(count)[0]);
      break;
    case OFTReal:
      field.template SetValue<double>(target->GetMeasurementVector(count)[0]);
      break;
    case OFTString:
      field.template SetValue<std::string>(std::to_string(target->GetMeasurementVector(count)[0]));
      break;
    default:
      itkExceptionMacro(<< "incorrect field type: " << field.GetType() << ".");
    }
    if (computeConfidenceMap)
      dstFeature[confFieldName].template SetValue<double>(quality->GetMeasurementVector(count)[0]);
    if (updateMode)
    {
      outLayer.SetFeature(dstFeature);
    }
    else
    {
      outLayer.CreateFeature(dstFeature);
    }
    count++;
  }
}

template <bool RegressionMode>
void           VectorPrediction<RegressionMode>::DoExecute()
{
  m_Model = MachineLearningModelFactoryType::CreateMachineLearningModel(GetParameterString("model"), MachineLearningModelFactoryType::ReadMode);

  if (m_Model.IsNull())
  {
    otbAppLogFATAL(<< "Error when loading model " << GetParameterString("model") << " : unsupported model type");
  }

  m_Model->SetRegressionMode(RegressionMode);

  m_Model->Load(GetParameterString("model"));
  otbAppLogINFO("Model loaded");

  auto shapefileName = GetParameterString("in");

  ogr::DataSource::Pointer source  = ogr::DataSource::New(shapefileName, ogr::DataSource::Modes::Read);
  auto layer  = source->GetLayer(0);
  auto input = ReadInputListSample(layer);

  ListSampleType::Pointer listSample = NormalizeListSample(input);
  typename LabelListSampleType::Pointer target;

  // The quality listSample containing confidence values is defined here, but is only used when
  // computeConfidenceMap evaluates to true. This listSample is also used in FillOutputLayer(...)
  const bool                                 computeConfidenceMap = shouldComputeConfidenceMap();
  typename ConfidenceListSampleType::Pointer quality;

  if (computeConfidenceMap)
  {
    quality = ConfidenceListSampleType::New();
    target  = m_Model->PredictBatch(listSample, quality);
  }
  else
  {
    target = m_Model->PredictBatch(listSample);
  }

  const bool updateMode = !(IsParameterEnabled("out") && HasValue("out"));

  ogr::DataSource::Pointer buffer;
  ogr::DataSource::Pointer output;

  if (updateMode)
  {
    // in update mode, output is added to input data source.
    // buffer needs to be allocated here, as its life-cycle is bound to "layer"
    buffer = ogr::DataSource::New();
    output = ReopenDataSourceInUpdateMode(source, layer, buffer);
  }
  else
  {
    output = CreateOutputDataSource(layer);
  }

  otb::ogr::Layer outLayer = output->GetLayer(0);

  OGRErr errStart = outLayer.ogr().StartTransaction();
  if (errStart != OGRERR_NONE)
  {
    itkExceptionMacro(<< "Unable to start transaction for OGR layer " << outLayer.ogr().GetName() << ".");
  }

  AddPredictionField(outLayer, layer, computeConfidenceMap);
  FillOutputLayer(outLayer, layer, target, quality, updateMode, computeConfidenceMap);

  if (outLayer.ogr().TestCapability("Transactions"))
  {
    const OGRErr errCommitX = outLayer.ogr().CommitTransaction();
    if (errCommitX != OGRERR_NONE)
    {
      itkExceptionMacro(<< "Unable to commit transaction for OGR layer " << outLayer.ogr().GetName() << ".");
    }
  }

  output->SyncToDisk();
}

} // end namespace wrapper
} // end namespace otb

#endif
