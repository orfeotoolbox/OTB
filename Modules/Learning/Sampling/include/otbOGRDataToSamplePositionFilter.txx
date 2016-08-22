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
#ifndef otbOGRDataToSamplePositionFilter_txx
#define otbOGRDataToSamplePositionFilter_txx

#include "otbOGRDataToSamplePositionFilter.h"
#include "itkTimeProbe.h"

namespace otb
{
// --------- otb::PersistentOGRDataToSamplePositionFilter ---------------------

template<class TInputImage, class TMaskImage, class TSampler>
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::PersistentOGRDataToSamplePositionFilter()
{
  this->SetNumberOfRequiredOutputs(2);
  m_OriginFieldName = std::string("originfid");
}

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::Synthetize(void)
{
  // clear temporary outputs
  this->m_InMemoryOutputs.clear();
}

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::Reset(void)
{
  // Reset samplers
  for (unsigned int i=0 ; i < m_Samplers.size() ; ++i)
    {
    for (typename SamplerMapType::iterator iter = m_Samplers[i].begin();
         iter != m_Samplers[i].end();
         ++iter)
      {
      iter->second->Reset();
      }
    }

  // Add an extra field for the original FID
  this->ClearAdditionalFields();
  this->CreateAdditionalField(this->GetOriginFieldName(),OFTInteger,12);

  // compute label mapping
  this->ComputeClassPartition();

  // Prepare outputs
  ogr::DataSource* inputDS = const_cast<ogr::DataSource*>(this->GetOGRData());
  for (unsigned int k=0 ; k < this->GetNumberOfLevels() ; k++)
    {
    otb::ogr::DataSource* output = this->GetOutputPositionContainer(k);
    if (output)
      {
      this->InitializeOutputDataSource(inputDS, output);
      }
    }
}

template<class TInputImage, class TMaskImage, class TSampler>
typename PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>::SamplerMapType&
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetSamplers(unsigned int level)
{
  if (m_Samplers.size() < (level + 1))
    {
    itkExceptionMacro("The sampler level "<< level << " doesn't exist !");
    }
  return m_Samplers[level];
}

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::SetOutputPositionContainerAndRates(
    otb::ogr::DataSource* data,
    const SamplingRateCalculator::MapRateType& map,
    unsigned int level)
{
  // set the output dataset
  this->SetNthOutput(1+level,data);
  // instanciate the corresponding samplers
  if (m_Samplers.size() < (level + 1))
    {
    m_Samplers.resize(level + 1);
    }
  m_Samplers[level].clear();
  for (SamplingRateCalculator::MapRateType::const_iterator it = map.begin() ;
       it != map.end();
       ++it)
    {
    SamplerPointerType sampler = SamplerType::New();
    sampler->SetNumberOfElements(it->second.Required,it->second.Tot);
    m_Samplers[level][it->first] = sampler;
    }
}

template<class TInputImage, class TMaskImage, class TSampler>
otb::ogr::DataSource*
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetOutputPositionContainer(unsigned int level)
{
  if (this->GetNumberOfOutputs() < (level + 2))
    {
    return 0;
    }
  return static_cast<otb::ogr::DataSource *>(this->itk::ProcessObject::GetOutput(level + 1));
}

template<class TInputImage, class TMaskImage, class TSampler>
unsigned int
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetNumberOfLevels()
{
  return (this->GetNumberOfOutputs() - 1);
}

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::ClearOutputs()
{
  // remove output position containers
  for (unsigned int i = (this->GetNumberOfOutputs() - 1) ; i > 0 ; --i)
    {
    this->itk::ProcessObject::RemoveOutput(i);
    }
  // remove samplers
  m_Samplers.clear();
}

template<class TInputImage, class TMaskImage, class TSampler>
itk::DataObject::Pointer
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::MakeOutput(DataObjectPointerArraySizeType idx)
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

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::ProcessSample(const ogr::Feature& feature,
                typename TInputImage::IndexType&,
                typename TInputImage::PointType& imgPoint,
                itk::ThreadIdType& threadid)
{
  std::string className(feature.ogr().GetFieldAsString(this->GetFieldIndex()));
  for (unsigned int i=0 ; i<this->GetNumberOfLevels() ; ++i)
    {
    if (m_Samplers[i][className]->TakeSample())
      {
      OGRPoint ogrTmpPoint;
      ogrTmpPoint.setX(imgPoint[0]);
      ogrTmpPoint.setY(imgPoint[1]);

      ogr::Layer outputLayer = this->m_InMemoryOutputs[threadid][i]->GetLayerChecked(0);
      ogr::Feature feat(outputLayer.GetLayerDefn());
      feat.SetFrom(feature);
      feat[this->GetOriginFieldName()].SetValue(static_cast<int>(feature.GetFID()));
      feat.SetGeometry(&ogrTmpPoint);
      outputLayer.CreateFeature(feat);
      break;
      }
    }
}

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::DispatchInputVectors(ogr::Layer &inLayer, std::vector<ogr::Layer> &tmpLayers)
{
  OGRFeatureDefn &layerDefn = inLayer.GetLayerDefn();
  ogr::Layer::const_iterator featIt = inLayer.begin();
  std::string className;
  for(; featIt!=inLayer.end(); ++featIt)
    {
    ogr::Feature dstFeature(layerDefn);
    dstFeature.SetFrom( *featIt, TRUE );
    dstFeature.SetFID(featIt->GetFID());
    className = featIt->ogr().GetFieldAsString(this->GetFieldIndex());
    tmpLayers[m_ClassPartition[className]].CreateFeature( dstFeature );
    }
}

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::ComputeClassPartition(void)
{
  m_ClassPartition.clear();
  typedef std::map<std::string, unsigned long> ClassCountMapType;
  ClassCountMapType classCounts;
  for (typename SamplerMapType::const_iterator it = m_Samplers[0].begin() ;
       it != m_Samplers[0].end();
       ++it)
    {
    classCounts[it->first] = it->second->GetTotalElements();
    }

  int numberOfThreads = this->GetNumberOfThreads();
  typedef std::vector<unsigned long> LoadVectorType;
  LoadVectorType currentLoad;
  currentLoad.resize(numberOfThreads, 0UL);
  
  ClassCountMapType::iterator largestClass;
  unsigned long minLoad;
  unsigned int destThread;
  while (classCounts.size())
    {
    // find class with highest number of total elements
    largestClass = classCounts.begin();
    for (ClassCountMapType::iterator iter = classCounts.begin() ;
         iter != classCounts.end() ;
         ++iter)
      {
      if (iter->second > largestClass->second)
        {
        largestClass = iter;
        }
      }

    // affect this class to the thread with lowest load
    minLoad = currentLoad[0];
    destThread = 0;
    for (unsigned int k = 0 ; k < currentLoad.size() ; ++k)
      {
      if (currentLoad[k] < minLoad)
        {
        minLoad = currentLoad[k];
        destThread = k;
        }
      }
    m_ClassPartition[largestClass->first] = destThread;
    currentLoad[destThread] += largestClass->second;

    // remove class from classCounts
    classCounts.erase(largestClass);
    }
}

// -------------- otb::OGRDataToSamplePositionFilter --------------------------

template<class TInputImage, class TMaskImage, class TSampler>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::SetInput(const TInputImage* image)
{
  this->GetFilter()->SetInput(image);
}

template<class TInputImage, class TMaskImage, class TSampler>
const TInputImage*
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetInput()
{
  return this->GetFilter()->GetInput();
}

template<class TInputImage, class TMaskImage, class TSampler>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::SetOGRData(const otb::ogr::DataSource* data)
{
  this->GetFilter()->SetOGRData(data);
}

template<class TInputImage, class TMaskImage, class TSampler>
const otb::ogr::DataSource*
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetOGRData()
{
  return this->GetFilter()->GetOGRData();
}

template<class TInputImage, class TMaskImage, class TSampler>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::SetMask(const TMaskImage* mask)
{
  this->GetFilter()->SetMask(mask);
}

template<class TInputImage, class TMaskImage, class TSampler>
const TMaskImage*
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetMask()
{
  return this->GetFilter()->GetMask();
}

template<class TInputImage, class TMaskImage, class TSampler>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::SetFieldName(std::string key)
{
  this->GetFilter()->SetFieldName(key);
}

template<class TInputImage, class TMaskImage, class TSampler>
std::string
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetFieldName()
{
  return this->GetFilter()->GetFieldName();
}

template<class TInputImage, class TMaskImage, class TSampler>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::SetLayerIndex(int index)
{
  this->GetFilter()->SetLayerIndex(index);
}

template<class TInputImage, class TMaskImage, class TSampler>
int
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetLayerIndex()
{
  return this->GetFilter()->GetLayerIndex();
}

template<class TInputImage, class TMaskImage, class TSampler>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::SetSamplerParameters(SamplerParameterType param, unsigned int level)
{
  SamplerMapType &samplers = this->GetSamplers(level);
  for (typename SamplerMapType::iterator it = samplers.begin() ; it != samplers.end() ; ++it)
    {
    it->second->SetParameters(param);
    }
}

template<class TInputImage, class TMaskImage, class TSampler>
typename OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>::SamplerMapType&
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetSamplers(unsigned int level)
{
  return this->GetFilter()->GetSamplers(level);
}

template<class TInputImage, class TMaskImage, class TSampler>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::SetOutputPositionContainerAndRates(
  otb::ogr::DataSource* data,
  const SamplingRateCalculator::MapRateType& map,
  unsigned int level)
{
  this->GetFilter()->SetOutputPositionContainerAndRates(data,map,level);
}

template<class TInputImage, class TMaskImage, class TSampler>
otb::ogr::DataSource*
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetOutputPositionContainer(unsigned int level)
{
  return this->GetFilter()->GetOutputPositionContainer(level);
}

template<class TInputImage, class TMaskImage, class TSampler>
void
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::SetOriginFieldName(std::string key)
{
  this->GetFilter()->SetOriginFieldName(key);
}

template<class TInputImage, class TMaskImage, class TSampler>
std::string
OGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::GetOriginFieldName()
{
  return this->GetFilter()->GetOriginFieldName();
}

} // end of namespace otb

#endif
