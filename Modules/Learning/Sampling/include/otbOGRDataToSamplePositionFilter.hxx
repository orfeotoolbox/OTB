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

#ifndef otbOGRDataToSamplePositionFilter_hxx
#define otbOGRDataToSamplePositionFilter_hxx

#include "otbOGRDataToSamplePositionFilter.h"

namespace otb
{
// --------- otb::PersistentOGRDataToSamplePositionFilter ---------------------

template <class TInputImage, class TMaskImage, class TSampler>
PersistentOGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::PersistentOGRDataToSamplePositionFilter()
{
  this->SetNumberOfRequiredOutputs(2);
  m_OriginFieldName = std::string("originfid");
  m_UseOriginField  = true;
}

template <class TInputImage, class TMaskImage, class TSampler>
void PersistentOGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::Reset(void)
{
  // Reset samplers
  for (unsigned int i = 0; i < m_Samplers.size(); ++i)
  {
    for (typename SamplerMapType::iterator iter = m_Samplers[i].begin(); iter != m_Samplers[i].end(); ++iter)
    {
      iter->second->Reset();
    }
  }

  // Add an extra field for the original FID
  this->ClearAdditionalFields();
  m_UseOriginField = (this->GetOriginFieldName().size() > 0);
  if (m_UseOriginField)
  {
    this->CreateAdditionalField(this->GetOriginFieldName(), OFTInteger, 12);
  }

  // compute label mapping
  this->ComputeClassPartition();

  // Prepare outputs
  ogr::DataSource* inputDS = const_cast<ogr::DataSource*>(this->GetOGRData());
  for (unsigned int k = 0; k < this->GetNumberOfLevels(); k++)
  {
    otb::ogr::DataSource* output = this->GetOutputPositionContainer(k);
    if (output)
    {
      this->InitializeOutputDataSource(inputDS, output);
    }
  }
}

template <class TInputImage, class TMaskImage, class TSampler>
typename PersistentOGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::SamplerMapType&
PersistentOGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::GetSamplers(unsigned int level)
{
  if (m_Samplers.size() < (level + 1))
  {
    itkExceptionMacro("The sampler level " << level << " doesn't exist !");
  }
  return m_Samplers[level];
}

template <class TInputImage, class TMaskImage, class TSampler>
void PersistentOGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::SetOutputPositionContainerAndRates(
    otb::ogr::DataSource* data, const SamplingRateCalculator::MapRateType& map, unsigned int level)
{
  // set the output dataset
  this->SetNthOutput(1 + level, data);
  // instantiate the corresponding samplers
  if (m_Samplers.size() < (level + 1))
  {
    m_Samplers.resize(level + 1);
  }
  m_Samplers[level].clear();
  for (SamplingRateCalculator::MapRateType::const_iterator it = map.begin(); it != map.end(); ++it)
  {
    SamplerPointerType sampler = SamplerType::New();
    sampler->SetNumberOfElements(it->second.Required, it->second.Tot);
    m_Samplers[level][it->first] = sampler;
  }
}

template <class TInputImage, class TMaskImage, class TSampler>
otb::ogr::DataSource* PersistentOGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::GetOutputPositionContainer(unsigned int level)
{
  if (this->GetNumberOfOutputs() < (level + 2))
  {
    return 0;
  }
  return static_cast<otb::ogr::DataSource*>(this->itk::ProcessObject::GetOutput(level + 1));
}

template <class TInputImage, class TMaskImage, class TSampler>
unsigned int PersistentOGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::GetNumberOfLevels()
{
  return (this->GetNumberOfOutputs() - 1);
}

template <class TInputImage, class TMaskImage, class TSampler>
void PersistentOGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::ClearOutputs()
{
  // remove output position containers
  for (unsigned int i = (this->GetNumberOfOutputs() - 1); i > 0; --i)
  {
    this->itk::ProcessObject::RemoveOutput(i);
  }
  // remove samplers
  m_Samplers.clear();
}

template <class TInputImage, class TMaskImage, class TSampler>
itk::DataObject::Pointer PersistentOGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::MakeOutput(DataObjectPointerArraySizeType idx)
{
  if (idx == 0)
  {
    // the output image
    return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
  }
  else if (idx > 0)
  {
    // sample position containers
    return static_cast<itk::DataObject*>(otb::ogr::DataSource::New().GetPointer());
  }
  // might as well make an image
  return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
}

template <class TInputImage, class TMaskImage, class TSampler>
void PersistentOGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::ProcessSample(const ogr::Feature& feature, typename TInputImage::IndexType&,
                                                                                               typename TInputImage::PointType& imgPoint,
                                                                                               itk::ThreadIdType&               threadid)
{
  std::string className(feature.ogr().GetFieldAsString(this->GetFieldIndex()));
  for (unsigned int i = 0; i < this->GetNumberOfLevels(); ++i)
  {
    if (m_Samplers[i][className]->TakeSample())
    {
      OGRPoint ogrTmpPoint;
      ogrTmpPoint.setX(imgPoint[0]);
      ogrTmpPoint.setY(imgPoint[1]);

      ogr::Layer   outputLayer = this->GetInMemoryOutput(threadid, i);
      ogr::Feature feat(outputLayer.GetLayerDefn());
      feat.SetFrom(feature);
      if (m_UseOriginField)
      {
        feat[this->GetOriginFieldName()].SetValue(static_cast<int>(feature.GetFID()));
      }
      feat.SetGeometry(&ogrTmpPoint);
      outputLayer.CreateFeature(feat);
      break;
    }
  }
}

template <class TInputImage, class TMaskImage, class TSampler>
void PersistentOGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::DispatchInputVectors()
{
  TInputImage*     outputImage = this->GetOutput();
  ogr::DataSource* vectors     = const_cast<ogr::DataSource*>(this->GetOGRData());
  ogr::Layer       inLayer     = vectors->GetLayer(this->GetLayerIndex());

  const RegionType&            requestedRegion = outputImage->GetRequestedRegion();
  itk::ContinuousIndex<double> startIndex(requestedRegion.GetIndex());
  itk::ContinuousIndex<double> endIndex(requestedRegion.GetUpperIndex());
  startIndex[0] += -0.5;
  startIndex[1] += -0.5;
  endIndex[0] += 0.5;
  endIndex[1] += 0.5;
  itk::Point<double, 2> startPoint;
  itk::Point<double, 2> endPoint;
  outputImage->TransformContinuousIndexToPhysicalPoint(startIndex, startPoint);
  outputImage->TransformContinuousIndexToPhysicalPoint(endIndex, endPoint);

  // create geometric extent
  OGRPolygon    tmpPolygon;
  OGRLinearRing ring;
  ring.addPoint(startPoint[0], startPoint[1], 0.0);
  ring.addPoint(startPoint[0], endPoint[1], 0.0);
  ring.addPoint(endPoint[0], endPoint[1], 0.0);
  ring.addPoint(endPoint[0], startPoint[1], 0.0);
  ring.addPoint(startPoint[0], startPoint[1], 0.0);
  tmpPolygon.addRing(&ring);

  inLayer.SetSpatialFilter(&tmpPolygon);

  unsigned int            numberOfThreads = this->GetNumberOfThreads();
  std::vector<ogr::Layer> tmpLayers;
  for (unsigned int i = 0; i < numberOfThreads; i++)
  {
    tmpLayers.push_back(this->GetInMemoryInput(i));
  }

  OGRFeatureDefn&            layerDefn = inLayer.GetLayerDefn();
  ogr::Layer::const_iterator featIt    = inLayer.begin();
  std::string                className;
  for (; featIt != inLayer.end(); ++featIt)
  {
    ogr::Feature dstFeature(layerDefn);
    dstFeature.SetFrom(*featIt, TRUE);
    dstFeature.SetFID(featIt->GetFID());
    className = featIt->ogr().GetFieldAsString(this->GetFieldIndex());
    tmpLayers[m_ClassPartition[className]].CreateFeature(dstFeature);
  }

  inLayer.SetSpatialFilter(nullptr);
}

template <class TInputImage, class TMaskImage, class TSampler>
void PersistentOGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::ComputeClassPartition(void)
{
  m_ClassPartition.clear();
  typedef std::map<std::string, unsigned long> ClassCountMapType;
  ClassCountMapType classCounts;
  for (typename SamplerMapType::const_iterator it = m_Samplers[0].begin(); it != m_Samplers[0].end(); ++it)
  {
    classCounts[it->first] = it->second->GetTotalElements();
  }

  int                                numberOfThreads = this->GetNumberOfThreads();
  typedef std::vector<unsigned long> LoadVectorType;
  LoadVectorType                     currentLoad;
  currentLoad.resize(numberOfThreads, 0UL);

  ClassCountMapType::iterator largestClass;
  unsigned long               minLoad;
  unsigned int                destThread;
  while (classCounts.size())
  {
    // find class with highest number of total elements
    largestClass = classCounts.begin();
    for (ClassCountMapType::iterator iter = classCounts.begin(); iter != classCounts.end(); ++iter)
    {
      if (iter->second > largestClass->second)
      {
        largestClass = iter;
      }
    }

    // affect this class to the thread with lowest load
    minLoad    = currentLoad[0];
    destThread = 0;
    for (unsigned int k = 0; k < currentLoad.size(); ++k)
    {
      if (currentLoad[k] < minLoad)
      {
        minLoad    = currentLoad[k];
        destThread = k;
      }
    }
    m_ClassPartition[largestClass->first] = destThread;
    currentLoad[destThread] += largestClass->second;

    // remove class from classCounts
    classCounts.erase(largestClass);
  }
}

template <class TInputImage, class TMaskImage, class TSampler>
void PersistentOGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::FillOneOutput(unsigned int outIdx, ogr::DataSource* outDS, bool update)
{
  ogr::Layer outLayer = outDS->GetLayersCount() == 1 ? outDS->GetLayer(0) : outDS->GetLayer(this->GetOutLayerName());

  OGRErr err = outLayer.ogr().StartTransaction();
  if (err != OGRERR_NONE)
  {
    itkExceptionMacro(<< "Unable to start transaction for OGR layer " << outLayer.ogr().GetName() << ".");
  }

  // output vectors sorted by class
  for (auto& label : m_ClassPartition)
  {
    ogr::Layer inLayer = this->GetInMemoryOutput(label.second, outIdx);
    if (!inLayer)
    {
      continue;
    }

    // This test only uses 1 input, not compatible with multiple OGRData inputs
    for (auto tmpIt = inLayer.begin(); tmpIt != inLayer.end(); ++tmpIt)
    {
      if (label.first.compare(tmpIt->ogr().GetFieldAsString(this->GetFieldIndex())) != 0)
        continue;
      if (update)
      {
        outLayer.SetFeature(*tmpIt);
      }
      else
      {
        ogr::Feature dstFeature(outLayer.GetLayerDefn());
        dstFeature.SetFrom(*tmpIt, TRUE);
        outLayer.CreateFeature(dstFeature);
      }
    }
  }

  err = outLayer.ogr().CommitTransaction();
  if (err != OGRERR_NONE)
  {
    itkExceptionMacro(<< "Unable to commit transaction for OGR layer " << outLayer.ogr().GetName() << ".");
  }
}

// -------------- otb::OGRDataToSamplePositionFilter --------------------------

template <class TInputImage, class TMaskImage, class TSampler>
void OGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::SetInput(const TInputImage* image)
{
  this->GetFilter()->SetInput(image);
}

template <class TInputImage, class TMaskImage, class TSampler>
const TInputImage* OGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::GetInput()
{
  return this->GetFilter()->GetInput();
}

template <class TInputImage, class TMaskImage, class TSampler>
void OGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::SetOGRData(const otb::ogr::DataSource* data)
{
  this->GetFilter()->SetOGRData(data);
}

template <class TInputImage, class TMaskImage, class TSampler>
const otb::ogr::DataSource* OGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::GetOGRData()
{
  return this->GetFilter()->GetOGRData();
}

template <class TInputImage, class TMaskImage, class TSampler>
void OGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::SetMask(const TMaskImage* mask)
{
  this->GetFilter()->SetMask(mask);
}

template <class TInputImage, class TMaskImage, class TSampler>
const TMaskImage* OGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::GetMask()
{
  return this->GetFilter()->GetMask();
}

template <class TInputImage, class TMaskImage, class TSampler>
void OGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::SetFieldName(std::string key)
{
  this->GetFilter()->SetFieldName(key);
}

template <class TInputImage, class TMaskImage, class TSampler>
std::string OGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::GetFieldName()
{
  return this->GetFilter()->GetFieldName();
}

template <class TInputImage, class TMaskImage, class TSampler>
void OGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::SetLayerIndex(int index)
{
  this->GetFilter()->SetLayerIndex(index);
}

template <class TInputImage, class TMaskImage, class TSampler>
int OGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::GetLayerIndex()
{
  return this->GetFilter()->GetLayerIndex();
}

template <class TInputImage, class TMaskImage, class TSampler>
void OGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::SetSamplerParameters(SamplerParameterType param, unsigned int level)
{
  SamplerMapType& samplers = this->GetSamplers(level);
  for (typename SamplerMapType::iterator it = samplers.begin(); it != samplers.end(); ++it)
  {
    it->second->SetParameters(param);
  }
}

template <class TInputImage, class TMaskImage, class TSampler>
typename OGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::SamplerMapType&
OGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::GetSamplers(unsigned int level)
{
  return this->GetFilter()->GetSamplers(level);
}

template <class TInputImage, class TMaskImage, class TSampler>
void OGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::SetOutputPositionContainerAndRates(otb::ogr::DataSource* data,
                                                                                                          const SamplingRateCalculator::MapRateType& map,
                                                                                                          unsigned int                               level)
{
  this->GetFilter()->SetOutputPositionContainerAndRates(data, map, level);
}

template <class TInputImage, class TMaskImage, class TSampler>
otb::ogr::DataSource* OGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::GetOutputPositionContainer(unsigned int level)
{
  return this->GetFilter()->GetOutputPositionContainer(level);
}

template <class TInputImage, class TMaskImage, class TSampler>
void OGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::SetOriginFieldName(std::string key)
{
  this->GetFilter()->SetOriginFieldName(key);
}

template <class TInputImage, class TMaskImage, class TSampler>
std::string OGRDataToSamplePositionFilter<TInputImage, TMaskImage, TSampler>::GetOriginFieldName()
{
  return this->GetFilter()->GetOriginFieldName();
}

} // end of namespace otb

#endif
