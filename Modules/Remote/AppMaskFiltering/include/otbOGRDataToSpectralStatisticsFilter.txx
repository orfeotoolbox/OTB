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

#ifndef otbOGRDataToSpectralStatisticsFilter_txx
#define otbOGRDataToSpectralStatisticsFilter_txx

#include "otbOGRDataToSpectralStatisticsFilter.h"

namespace otb
{
// --------- otb::PersistentOGRDataToSpectralStatisticsFilter ---------------------

template<class TInputImage, class TMaskImage>
PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::PersistentOGRDataToSpectralStatisticsFilter()
{
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::Reset(void)
{
  m_PolygonSizeThread.clear();
  m_PolygonFirstOrderThread.clear();
  m_PolygonSecondOrderThread.clear();
  m_CurrentFID.clear();
  
  m_PolygonSizeThread.resize(this->GetNumberOfThreads());
  m_PolygonFirstOrderThread.resize(this->GetNumberOfThreads());
  m_PolygonSecondOrderThread.resize(this->GetNumberOfThreads());
  m_CurrentFID.resize(this->GetNumberOfThreads());
}


template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::Synthetize(void)
{
  PolygonSizeMapType PolygonSize;
  PolygonVectorMapType PolygonFirstOrderComponent;
  PolygonVectorMapType PolygonMeanComponent;
  PolygonMatrixMapType PolygonSecondOrderComponent;
  PolygonMatrixMapType PolygonCovComponent;
  
  const TInputImage* img = this->GetInput();
  unsigned int numberOfComponents = img->GetNumberOfComponentsPerPixel();
    
  
  // Accumulate Results from all thread
  const itk::ThreadIdType numberOfThreads = this->GetNumberOfThreads();
  for (itk::ThreadIdType threadId = 0; threadId < numberOfThreads; ++threadId) 
  {
    PolygonSizeMapType::iterator itSize = m_PolygonSizeThread[threadId].begin();
    PolygonVectorMapType::iterator itFirstOrder = m_PolygonFirstOrderThread[threadId].begin();
    PolygonMatrixMapType::iterator itSecondOrder = m_PolygonSecondOrderThread[threadId].begin();
    
    
    for (itSize ; itSize != m_PolygonSizeThread[threadId].end(); itSize++, itFirstOrder++, itSecondOrder++)
    {
      unsigned int fid = itSize->first;
      if (!PolygonSize.count(fid))
      {
        PolygonSize[fid] = itSize->second;
        PolygonFirstOrderComponent[fid] = itFirstOrder->second;
        PolygonSecondOrderComponent[fid] = itSecondOrder->second;
      }
      else
      {
        PolygonSize[fid] += itSize->second;
        for (unsigned int i =0; i < numberOfComponents ; i++)
        {
          PolygonFirstOrderComponent[fid][i] = itFirstOrder->second[i];
        }
        for (unsigned int r =0; r < numberOfComponents ; r++)
        {
          for (unsigned int c =0; c < numberOfComponents ; c++)
          {
            PolygonSecondOrderComponent[fid][r][c] = itSecondOrder->second[r][c];
          }
        }
      }
    }
  }
  
  // Final computation
  
  PolygonSizeMapType::iterator itSizeFull = PolygonSize.begin();
  PolygonVectorMapType::iterator itMean = PolygonFirstOrderComponent.begin();
  PolygonMatrixMapType::iterator itCov = PolygonSecondOrderComponent.begin();
  
  for (itMean; itMean !=  PolygonFirstOrderComponent.end(); itMean++,itSizeFull++, itCov++)
  {
    int sizePolygon =  itSizeFull->second;
    if (sizePolygon > 0)
    {
      PolygonMeanComponent[itMean->first].SetSize(numberOfComponents);
      for (unsigned int i =0; i < numberOfComponents ; i++)
      {
        PolygonMeanComponent[itMean->first][i] = itMean->second[i] / sizePolygon;
      }
      
      PolygonCovComponent[itMean->first].SetSize(numberOfComponents,numberOfComponents);
      for (unsigned int r =0; r < numberOfComponents ; r++)
      {
        for (unsigned int c =0; c < numberOfComponents ; c++)
        {
          PolygonCovComponent[itMean->first][r][c] = itCov->second[r][c] + PolygonMeanComponent[itMean->first][r]*PolygonMeanComponent[itMean->first][c] / sizePolygon;
          PolygonCovComponent[itMean->first][r][c] /= sizePolygon;
        }
      }
    }
  }
  
  
  
  // Show results ! (temporary cout)
  PolygonSizeMapType::iterator itSizeout = PolygonSize.begin();
  PolygonVectorMapType::iterator itFirstOrderout = PolygonFirstOrderComponent.begin();
  PolygonVectorMapType::iterator itMeanout = PolygonMeanComponent.begin();
  PolygonMatrixMapType::iterator itCovout = PolygonCovComponent.begin();
  for (itSizeout ; itSizeout != PolygonSize.end(); itSizeout++, itFirstOrderout++, itMeanout++, itCovout++)
  {
    std::cout << "FID : " << itSizeout->first << ",Size : " << itSizeout->second << ",sum : ";
    for (unsigned int i =0; i < numberOfComponents ; i++)
    {
      std::cout << itFirstOrderout->second[i] << " ";
    }
    std::cout << "mean : ";
    for (unsigned int i =0; i < numberOfComponents ; i++)
    {
      std::cout << itMeanout->second[i] << " ";
    }
    std::cout << "cov : ";
    for (unsigned int r =0; r < numberOfComponents ; r++)
    {
      std::cout << "r" << r <<" " ;
      for (unsigned int c =0; c < numberOfComponents ; c++)
      {
        std::cout << itCovout->second[r][c] << " ";
      }
    }
    std::cout << std::endl;
  }
  
  
}



template <class TInputImage, class TMaskImage>
void
PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::ProcessPolygon(const ogr::Feature& feature,
                 OGRPolygon* polygon,
                 RegionType& region,
                 itk::ThreadIdType& threadid)
{
  const TInputImage* img = this->GetInput();
  TMaskImage* mask = const_cast<TMaskImage*>(this->GetMask());
  typename TInputImage::IndexType imgIndex;
  typename TInputImage::PointType imgPoint;
  OGRPoint tmpPoint;

  if (mask)
    {
    // For pixels in consideredRegion and not masked
    typedef MaskedIteratorDecorator<
      itk::ImageRegionConstIterator<TMaskImage>,
      itk::ImageRegionConstIterator<TMaskImage> > MaskedIteratorType;
    MaskedIteratorType it(mask, mask, region);
    it.GoToBegin();
    while (!it.IsAtEnd())
      {
      imgIndex = it.GetIndex();
      img->TransformIndexToPhysicalPoint(imgIndex,imgPoint);
      tmpPoint.setX(imgPoint[0]);
      tmpPoint.setY(imgPoint[1]);
      bool isInside = this->IsSampleInsidePolygon(polygon,&tmpPoint);
      if (isInside)
        {
        this->ProcessSample(feature,imgIndex, imgPoint, threadid);
        }
      ++it;
      }
    }
  else
    {
    typedef itk::ImageRegionConstIterator<TInputImage> ValueIteratorType;
    ValueIteratorType it(img,region);
    
    unsigned int numberOfComponents = img->GetNumberOfComponentsPerPixel();
    
    
    it.GoToBegin();
    int fid = m_CurrentFID[threadid];
    
    // Initialize first and second order accumulators
    m_PolygonFirstOrderThread[threadid][fid].SetSize(numberOfComponents);
    m_PolygonSecondOrderThread[threadid][fid].SetSize(numberOfComponents, numberOfComponents);
    while (!it.IsAtEnd())
      {
      imgIndex = it.GetIndex();
      img->TransformIndexToPhysicalPoint(imgIndex,imgPoint);
      tmpPoint.setX(imgPoint[0]);
      tmpPoint.setY(imgPoint[1]);
      
      bool isInside = this->IsSampleInsidePolygon(polygon,&tmpPoint);
      if (isInside)
        {
        PixelType VectorValue = it.Get();
        m_PolygonSizeThread[threadid][fid] ++;
       
        //std::cout << threadid << std::endl;
        for (unsigned int i =0; i < numberOfComponents ; i++)
        {
           m_PolygonFirstOrderThread[threadid][fid][i] += VectorValue[i];
        }
        for (unsigned int r =0; r < numberOfComponents ; r++)
        {
          for (unsigned int c =0; c < numberOfComponents ; c++)
          {
            m_PolygonSecondOrderThread[threadid][fid][r][c] += VectorValue[r]*VectorValue[c];
          }
        }
        
        
        //std::cout <<  it.Get()[0] << " " << it.Get()[1] << " " << m_PolygonSizeThread[threadid][m_CurrentFID[threadid]] << std::endl;
        //this->ProcessSample(feature,imgIndex, imgPoint, threadid);
        }
      ++it;
      }
      //m_PolygonFirstOrderThread[threadid][m_CurrentFID[threadid]] /= m_PolygonSizeThread[threadid][m_CurrentFID[threadid]];
      
    }
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::ProcessSample(
  const ogr::Feature&,
  typename TInputImage::IndexType& imgIndex,
  typename TInputImage::PointType& imgPoint,
  itk::ThreadIdType& threadid)
{
  std::cout << "P: " << imgPoint[0] << " " << imgPoint[1] << std::endl;
  std::cout << "I: " << imgIndex[0] << " " << imgIndex[1] << std::endl;
   
  //unsigned long& fId = m_CurrentFID[threadid];
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::PrepareFeature(const ogr::Feature& feature,
                 itk::ThreadIdType& threadid)
{
  std::string className(feature.ogr().GetFieldAsString(this->GetFieldIndex()));
  unsigned long fId = feature.ogr().GetFID();
  if (!m_PolygonSizeThread[threadid].count(fId))
    {
    m_PolygonSizeThread[threadid][fId] = 0;
    }
  m_CurrentFID[threadid] = fId;
}
// -------------- otb::OGRDataToSpectralStatisticsFilter --------------------------

template<class TInputImage, class TMaskImage>
void
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::SetInput(const TInputImage* image)
{
  this->GetFilter()->SetInput(image);
}

template<class TInputImage, class TMaskImage>
const TInputImage*
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::GetInput()
{
  return this->GetFilter()->GetInput();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::SetOGRData(const otb::ogr::DataSource* data)
{
  this->GetFilter()->SetOGRData(data);
}

template<class TInputImage, class TMaskImage>
const otb::ogr::DataSource*
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::GetOGRData()
{
  return this->GetFilter()->GetOGRData();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::SetMask(const TMaskImage* mask)
{
  this->GetFilter()->SetMask(mask);
}

template<class TInputImage, class TMaskImage>
const TMaskImage*
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::GetMask()
{
  return this->GetFilter()->GetMask();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::SetFieldName(std::string &key)
{
  this->GetFilter()->SetFieldName(key);
}

template<class TInputImage, class TMaskImage>
std::string
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::GetFieldName()
{
  return this->GetFilter()->GetFieldName();
}

} // end of namespace otb

#endif

