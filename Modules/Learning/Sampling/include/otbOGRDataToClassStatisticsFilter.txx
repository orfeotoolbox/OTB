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
#ifndef __otbOGRDataToClassStatisticsFilter_txx
#define __otbOGRDataToClassStatisticsFilter_txx

#include "otbOGRDataToClassStatisticsFilter.h"

namespace otb
{
// --------- otb::PersistentOGRDataToClassStatisticsFilter ---------------------

template<class TInputImage, class TMaskImage>
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::PersistentOGRDataToClassStatisticsFilter()
{
  this->SetNumberOfRequiredOutputs(3);
  this->SetNthOutput(0,TInputImage::New());
  this->SetNthOutput(1,ClassCountObjectType::New());
  this->SetNthOutput(2,PolygonSizeObjectType::New());
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::Synthetize(void)
{
  otb::ogr::DataSource* vectors = const_cast<otb::ogr::DataSource*>(this->GetOGRData());
  vectors->GetLayer(this->GetLayerIndex()).SetSpatialFilter(NULL);
  
  ClassCountMapType &classCount = this->GetClassCountOutput()->Get();
  PolygonSizeMapType &polygonSize = this->GetPolygonSizeOutput()->Get();
  
  // Reset outputs
  classCount.clear();
  polygonSize.clear();
  // Copy temporary stats to outputs
  for (unsigned int k=0 ; k < this->GetNumberOfThreads() ; k++)
    {
    ClassCountMapType::iterator itClass = m_ElmtsInClassThread[k].begin();
    for (; itClass != m_ElmtsInClassThread[k].end() ; ++itClass)
      {
      if (classCount.count(itClass->first))
        {
        classCount[itClass->first] += itClass->second;
        }
      else
        {
        classCount[itClass->first] = itClass->second;
        }
      }
    PolygonSizeMapType::iterator itPoly = m_PolygonThread[k].begin();
    for (; itPoly != m_PolygonThread[k].end() ; ++itPoly)
      {
      if (polygonSize.count(itPoly->first))
        {
        polygonSize[itPoly->first] += itPoly->second;
        }
      else
        {
        polygonSize[itPoly->first] = itPoly->second;
        }
      }
    }

  m_ElmtsInClassThread.clear();
  m_PolygonThread.clear();
  m_NbPixelsThread.clear();
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::Reset(void)
{
  m_ElmtsInClassThread.clear();
  m_PolygonThread.clear();
  m_NbPixelsThread.clear();

  m_ElmtsInClassThread.resize(this->GetNumberOfThreads());
  m_PolygonThread.resize(this->GetNumberOfThreads());
  m_NbPixelsThread.resize(this->GetNumberOfThreads());
  m_CurrentClass.resize(this->GetNumberOfThreads());
  m_CurrentFID.resize(this->GetNumberOfThreads());
}

template<class TInputImage, class TMaskImage>
const typename PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>::ClassCountObjectType*
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetClassCountOutput() const
{
  if (this->GetNumberOfOutputs()<2)
    {
    return 0;
    }
  return static_cast<const ClassCountObjectType *>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage, class TMaskImage>
typename PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>::ClassCountObjectType* 
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetClassCountOutput()
{
  if (this->GetNumberOfOutputs()<2)
    {
    return 0;
    }
  return static_cast<ClassCountObjectType *>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage, class TMaskImage>
const typename PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>::PolygonSizeObjectType*
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetPolygonSizeOutput() const
{
  if (this->GetNumberOfOutputs()<3)
    {
    return 0;
    }
  return static_cast<const PolygonSizeObjectType *>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage, class TMaskImage>
typename PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>::PolygonSizeObjectType*
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetPolygonSizeOutput()
{
    if (this->GetNumberOfOutputs()<3)
    {
    return 0;
    }
  return static_cast<PolygonSizeObjectType *>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage, class TMaskImage>
itk::DataObject::Pointer
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::MakeOutput(DataObjectPointerArraySizeType idx)
{
  switch (idx)
    {
    case 0:
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    case 1:
      return static_cast<itk::DataObject*>(ClassCountObjectType::New().GetPointer());
      break;
    case 2:
      return static_cast<itk::DataObject*>(PolygonSizeObjectType::New().GetPointer());
      break;
    default:
      // might as well make an image
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    }
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::ProcessSample(
  const ogr::Feature&,
  typename TInputImage::IndexType&,
  typename TInputImage::PointType&,
  itk::ThreadIdType& threadid)
{
  std::string& className = m_CurrentClass[threadid];
  unsigned long& fId = m_CurrentFID[threadid];

  m_ElmtsInClassThread[threadid][className]++;
  m_PolygonThread[threadid][fId]++;
  m_NbPixelsThread[threadid]++;
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::PrepareFeature(const ogr::Feature& feature,
                 itk::ThreadIdType& threadid)
{
  std::string className(feature.ogr().GetFieldAsString(this->GetFieldIndex()));
  unsigned long fId = feature.ogr().GetFID();
  if (!m_ElmtsInClassThread[threadid].count(className))
    {
    m_ElmtsInClassThread[threadid][className] = 0;
    }
  if (!m_PolygonThread[threadid].count(fId))
    {
    m_PolygonThread[threadid][fId] = 0;
    }
  m_CurrentClass[threadid] = className;
  m_CurrentFID[threadid] = fId;
}

// -------------- otb::OGRDataToClassStatisticsFilter --------------------------

template<class TInputImage, class TMaskImage>
void
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::SetInput(const TInputImage* image)
{
  this->GetFilter()->SetInput(image);
}

template<class TInputImage, class TMaskImage>
const TInputImage*
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetInput()
{
  return this->GetFilter()->GetInput();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::SetOGRData(const otb::ogr::DataSource* data)
{
  this->GetFilter()->SetOGRData(data);
}

template<class TInputImage, class TMaskImage>
const otb::ogr::DataSource*
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetOGRData()
{
  return this->GetFilter()->GetOGRData();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::SetMask(const TMaskImage* mask)
{
  this->GetFilter()->SetMask(mask);
}

template<class TInputImage, class TMaskImage>
const TMaskImage*
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetMask()
{
  return this->GetFilter()->GetMask();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::SetFieldName(std::string &key)
{
  this->GetFilter()->SetFieldName(key);
}

template<class TInputImage, class TMaskImage>
std::string
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetFieldName()
{
  return this->GetFilter()->GetFieldName();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::SetLayerIndex(int index)
{
  this->GetFilter()->SetLayerIndex(index);
}

template<class TInputImage, class TMaskImage>
int
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetLayerIndex()
{
  return this->GetFilter()->GetLayerIndex();
}

template<class TInputImage, class TMaskImage>
const typename OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>::ClassCountObjectType*
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetClassCountOutput() const
{
  return this->GetFilter()->GetClassCountOutput();
}

template<class TInputImage, class TMaskImage>
typename OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>::ClassCountObjectType*
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetClassCountOutput()
{
  return this->GetFilter()->GetClassCountOutput();
}

template<class TInputImage, class TMaskImage>
const typename OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>::PolygonSizeObjectType*
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetPolygonSizeOutput() const
{
  return this->GetFilter()->GetPolygonSizeOutput();
}

template<class TInputImage, class TMaskImage>
typename OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>::PolygonSizeObjectType*
OGRDataToClassStatisticsFilter<TInputImage,TMaskImage>
::GetPolygonSizeOutput()
{
  return this->GetFilter()->GetPolygonSizeOutput();
}


} // end of namespace otb

#endif
