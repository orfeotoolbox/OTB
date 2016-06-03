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
#ifndef __otbOGRDataToSamplePositionFilter_txx
#define __otbOGRDataToSamplePositionFilter_txx

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
  this->SetNthOutput(0,TInputImage::New());
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

  // compute label mapping
  this->ComputeClassPartition();

  // Prepare outputs
  // TODO : forward all fields from input vectors
  for (unsigned int k=0 ; k < this->GetNumberOfLevels() ; k++)
    {
    otb::ogr::DataSource* output = this->GetOutputPositionContainer(k);
    if (output)
      {
      std::string projectionRefWkt = this->GetInput()->GetProjectionRef();
      bool projectionInformationAvailable = !projectionRefWkt.empty();
      OGRSpatialReference * oSRS = NULL;
      if(projectionInformationAvailable)
        {
        oSRS = static_cast<OGRSpatialReference *>(OSRNewSpatialReference(projectionRefWkt.c_str()));
        }

      output->CreateLayer(this->GetOutLayerName(), oSRS ,wkbPoint, this->GetOGRLayerCreationOptions());
      OGRFieldDefn field(this->GetFieldName().c_str(),OFTString);

      //Handle the case of shapefile. A shapefile is a layer and not a datasource.
      //The layer name in a shapefile is the shapefile's name.
      //This is not the case for a database as sqlite or PG.
      if (output->GetLayersCount() == 1)
        {
        output->GetLayer(0).CreateField(field, true);
        }
      else
        {
        output->GetLayer(this->GetOutLayerName()).CreateField(field, true);
        }
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
      feat[this->GetFieldName()].SetValue(className);
      feat.SetGeometry(&ogrTmpPoint);
      outputLayer.CreateFeature(feat);
      break;
      }
    }
}

template<class TInputImage, class TMaskImage, class TSampler>
void
PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler>
::PrepareInputVectors(void)
{
  TInputImage* outputImage = this->GetOutput();
  ogr::DataSource* vectors = const_cast<ogr::DataSource*>(this->GetOGRData());
  ogr::Layer inLayer = vectors->GetLayer(this->GetLayerIndex());

  const RegionType& requestedRegion = outputImage->GetRequestedRegion();
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
  OGRPolygon tmpPolygon;
  OGRLinearRing ring;
  ring.addPoint(startPoint[0],startPoint[1],0.0);
  ring.addPoint(startPoint[0],endPoint[1]  ,0.0);
  ring.addPoint(endPoint[0]  ,endPoint[1]  ,0.0);
  ring.addPoint(endPoint[0]  ,startPoint[1],0.0);
  ring.addPoint(startPoint[0],startPoint[1],0.0);
  tmpPolygon.addRing(&ring);

  int numberOfThreads = this->GetNumberOfThreads();

  // prepare temporary input : split input features according to the class partition
  this->m_InMemoryInputs.clear();
  std::string tmpLayerName("thread");
  std::string inLayerName(inLayer.GetName());
  std::ostringstream oss;
  for (unsigned int i=0 ; i < numberOfThreads ; i++)
    {
    oss.str("");
    oss << "SELECT * FROM \"" << inLayerName << "\" ";
    oss << "WHERE "<<this->GetFieldName()<<" IN (";
    bool isFirst = true;
    for (ClassPartitionType::iterator iter = this->m_ClassPartition.begin() ;
         iter != this->m_ClassPartition.end() ;
         ++iter)
      {
      if (iter->second == i)
        {
        if (isFirst)
          {
          isFirst = false;
          }
        else
          {
          oss << ",";
          }
        oss <<"'"<<iter->first<<"'";
        }
      }
    oss << ")";
    OGRDataPointer tmpInput = ogr::DataSource::New();
    if (isFirst)
      {
      // no class to process for this thread : store an empty layer
      tmpInput->CreateLayer(tmpLayerName);
      }
    else
      {
      // Extract classes to process in this thread
      ogr::Layer search = vectors->ExecuteSQL(oss.str().c_str(), &tmpPolygon, NULL);
      tmpInput->CopyLayer(search,tmpLayerName);
      }
    this->m_InMemoryInputs.push_back(tmpInput);
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

} // end of namespace otb

#endif
