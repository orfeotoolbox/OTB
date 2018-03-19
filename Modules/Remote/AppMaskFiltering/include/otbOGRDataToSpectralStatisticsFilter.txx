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
  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput(0,TInputImage::New());
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::SetOutputSamples(ogr::DataSource* data)
{
  this->SetNthOutput(1,data);
}

template<class TInputImage, class TMaskImage>
ogr::DataSource*
PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::GetOutputSamples()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  return static_cast<ogr::DataSource *>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::Reset(void)
{
  m_PolygonSizeThread.clear();
  m_PolygonMaxThread.clear();
  m_PolygonMinThread.clear();
  m_PolygonFirstOrderThread.clear();
  m_PolygonSecondOrderThread.clear();
  m_CurrentFID.clear();
  
  m_PolygonSizeThread.resize(this->GetNumberOfThreads());
  m_PolygonMaxThread.resize(this->GetNumberOfThreads());
  m_PolygonMinThread.resize(this->GetNumberOfThreads());
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
  PolygonVectorMapType PolygonMin;
  PolygonVectorMapType PolygonMax;
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
    PolygonVectorMapType::iterator itMax = m_PolygonMaxThread[threadId].begin();
    PolygonVectorMapType::iterator itMin = m_PolygonMinThread[threadId].begin();
    PolygonMatrixMapType::iterator itSecondOrder = m_PolygonSecondOrderThread[threadId].begin();
    
    for (itSize ; itSize != m_PolygonSizeThread[threadId].end(); itSize++, itFirstOrder++, itSecondOrder++, itMax++, itMin++)
    {
      unsigned int fid = itSize->first;
      if (!PolygonSize.count(fid))
      {
        PolygonSize[fid] = itSize->second;
        PolygonFirstOrderComponent[fid] = itFirstOrder->second;
        PolygonSecondOrderComponent[fid] = itSecondOrder->second;
        PolygonMin[fid] = itMin->second;
        PolygonMax[fid] = itMax->second;
        std::cout << "1! " << fid << threadId << std::endl; 
      }
      else
      {
        std::cout << "2! " << fid << threadId << std::endl; 
        PolygonSize[fid] += itSize->second;
        for (unsigned int i =0; i < numberOfComponents ; i++)
        {
          PolygonFirstOrderComponent[fid][i] += itFirstOrder->second[i];
          if ( itMin->second[i] < PolygonMin[fid][i])
          {
            PolygonMin[fid][i] = itMin->second[i];
          }
          if ( itMax->second[i] > PolygonMax[fid][i])
          {
            PolygonMax[fid][i] = itMax->second[i]; 
          }
          
        }
        for (unsigned int r =0; r < numberOfComponents ; r++)
        {
          for (unsigned int c =0; c < numberOfComponents ; c++)
          {
            PolygonSecondOrderComponent[fid][r][c] += itSecondOrder->second[r][c];
            
          }
        }
      }
    }
  }
  
  // Final computation
  
  PolygonSizeMapType::iterator itSizeFull = PolygonSize.begin();
  PolygonVectorMapType::iterator itMean = PolygonFirstOrderComponent.begin();
  PolygonMatrixMapType::iterator itCov = PolygonSecondOrderComponent.begin();
  
  //ogr::DataSource* inputDS = const_cast<ogr::DataSource*>(this->GetOGRData());
  
  
  for (itMean; itMean !=  PolygonFirstOrderComponent.end(); itMean++,itSizeFull++, itCov++)
  {
    // Retrieve the feature corresponding to the fid in the output layer
    int fid = itMean->first;
    
    // Size computation
    int sizePolygon =  itSizeFull->second;

    if (sizePolygon > 0)
    {
      // Mean Computation 
      PolygonMeanComponent[itMean->first].SetSize(numberOfComponents);
      for (unsigned int i =0; i < numberOfComponents ; i++)
      {
        PolygonMeanComponent[fid][i] = itMean->second[i] / sizePolygon;
        
      }
      
      // Covariance computation
      PolygonCovComponent[itMean->first].SetSize(numberOfComponents,numberOfComponents);
      for (unsigned int r =0; r < numberOfComponents ; r++)
      {
        for (unsigned int c =0; c < numberOfComponents ; c++)
        {
          PolygonCovComponent[itMean->first][r][c] = itCov->second[r][c] / sizePolygon - PolygonMeanComponent[itMean->first][r]*PolygonMeanComponent[itMean->first][c];
          
          
          //std::cout << sizePolygon << " " << PolygonMeanComponent[itMean->first][r] << " " << PolygonMeanComponent[itMean->first][c] << " " << itCov->second[r][c] << std::endl;
          //PolygonCovComponent[itMean->first][r][c] /= sizePolygon;
        }
      }
    }
    
  }
  /*

  ogr::DataSource* inputDS = const_cast<ogr::DataSource*>(this->GetOGRData());
  ogr::DataSource* output  = this->GetOutputSamples();
  
  otb::ogr::Layer inLayer =  inputDS->GetLayer(0);
  otb::ogr::Layer outLayer = output->GetLayer(0);
  */
  /*
  otb::ogr::Layer::iterator it = outLayer.begin();
  otb::ogr::Layer::iterator itEnd = outLayer.end();
  //for (int i = 0; i <4; i++)
  for(; it!=itEnd; ++it)
  {
    std::cout << it->GetFID() << std::endl;
    //ogr::Feature feat = it->GetNextFeature();
    (*it)["mean"].SetValue<double>(0.);
    /*ogr::Feature dstFeature(outLayer.GetLayerDefn());
    dstFeature.SetFrom( *it, TRUE );
    dstFeature.SetFID(it->GetFID());
    dstFeature["mean"].SetValue<double>(0.);
    //outLayer.SetFeature( dstFeature );*/
  //}
  
  
  
  /*
  /// Retrieve layer
  otb::ogr::Layer outLayer = this->GetInMemoryOutput(threadid);
  
  
  ogr::Feature dstFeature(outputLayer.GetLayerDefn());
  dstFeature.SetFrom( *featIt, TRUE );
  dstFeature.SetFID(featIt->GetFID());*/
  
  
  
  // initialize additional fields for output
  this->InitializeFields();

  // initialize output DataSource
  ogr::DataSource* inputDS = const_cast<ogr::DataSource*>(this->GetOGRData());
  ogr::DataSource* output  = this->GetOutputSamples();
  
  this->InitializeOutputDataSource(inputDS,output);
  
  otb::ogr::Layer outLayer = output->GetLayer(0);
  
  PolygonSizeMapType::iterator itSizeout = PolygonSize.begin();
  PolygonVectorMapType::iterator itFirstOrderout = PolygonFirstOrderComponent.begin();
  PolygonVectorMapType::iterator itMeanout = PolygonMeanComponent.begin();
  PolygonVectorMapType::iterator itMaxout = PolygonMax.begin();
  PolygonVectorMapType::iterator itMinout = PolygonMin.begin();
  PolygonMatrixMapType::iterator itCovout = PolygonCovComponent.begin();
  
  
  for (itSizeout ; itSizeout != PolygonSize.end(); itSizeout++, itFirstOrderout++, itMeanout++, itCovout++, itMaxout++, itMinout++)
  {
    // Show results ! (temporary cout)
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
  
    // Write features in the ouput shapefile
    int fid = itMeanout->first;
    ogr::Feature outFeat = outLayer.GetFeature(fid);
    
    outFeat["NbPixels"].SetValue<int>(itSizeout->second);
    for (unsigned int i =0; i < numberOfComponents ; i++)
    {
      outFeat["mean_"+std::to_string(i)].SetValue<double>(itMeanout->second[i]);
      outFeat["min_"+std::to_string(i)].SetValue<double>(itMinout->second[i]);
      outFeat["max_"+std::to_string(i)].SetValue<double>(itMaxout->second[i]);
    }
    for (unsigned int r =0; r < numberOfComponents ; r++)
    {
      for (unsigned int c =0; c < numberOfComponents ; c++)
      {
        outFeat["cov_"+std::to_string(r)+"_"+std::to_string(c)].SetValue<double>(itCovout->second[r][c]);
      }
    }    
  
    outLayer.SetFeature(outFeat);
    
  }
  
  
}


template<class TInputImage,class TMaskImage>
void
PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::InitializeFields()
{
  // Get number of components, to determine of many field will be created
  const TInputImage* img = this->GetInput();
  unsigned int numberOfComponents = img->GetNumberOfComponentsPerPixel();
    
  this->ClearAdditionalFields();
  
  this->CreateAdditionalField("NbPixels",OFTInteger,24,15);
  
  // Create min fields
  for (unsigned int i=0; i < numberOfComponents; i++)
  {
    this->CreateAdditionalField("min_"+std::to_string(i) ,OFTReal,24,15);
  }
  
  // Create max fields
  for (unsigned int i=0; i < numberOfComponents; i++)
  {
    this->CreateAdditionalField("max_"+std::to_string(i) ,OFTReal,24,15);
  }
  for (unsigned int i=0; i < numberOfComponents; i++)
  
  // Create mean fields
  {
    this->CreateAdditionalField("mean_"+std::to_string(i) ,OFTReal,24,15);
  }
  
  for (unsigned int r=0; r < numberOfComponents; r++)
  {
    for (unsigned int c=0; c < numberOfComponents; c++)
    {
      this->CreateAdditionalField("cov_"+std::to_string(r)+"_"+std::to_string(c),OFTReal,24,15);
    }
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
    
    // Initialize accumulator sizes
    m_PolygonFirstOrderThread[threadid][fid].SetSize(numberOfComponents);
    m_PolygonMinThread[threadid][fid].SetSize(numberOfComponents);
    m_PolygonMaxThread[threadid][fid].SetSize(numberOfComponents);
    m_PolygonSecondOrderThread[threadid][fid].SetSize(numberOfComponents, numberOfComponents);

    /*
    // Initialize Min and Max accumulators with extreme values
    for (unsigned int i =0; i < numberOfComponents ; i++)
    {
      m_PolygonMaxThread[threadid][fid][i] = ;
      m_PolygonMinThread[threadid][fid][i] = itk::NumericTraits<double>::max();
    }*/
    
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
          if (VectorValue[i] > std::max(itk::NumericTraits<double>::NonpositiveMin(), m_PolygonMaxThread[threadid][fid][i]))
          {
            m_PolygonMaxThread[threadid][fid][i] = VectorValue[i];
          }
          if (VectorValue[i] < std::min(itk::NumericTraits<double>::max(), m_PolygonMinThread[threadid][fid][i]))
          {
            m_PolygonMinThread[threadid][fid][i] = VectorValue[i];
          }
        }
        for (unsigned int r =0; r < numberOfComponents ; r++)
        {
          for (unsigned int c =0; c < numberOfComponents ; c++)
          {
            m_PolygonSecondOrderThread[threadid][fid][r][c] += VectorValue[r]*VectorValue[c];
            if  (std::isnan( VectorValue[r] ) || std::isnan( VectorValue[c] )) std::cout <<  fid << " " << std::endl;;
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
::SetOutputSamples( OGRDataType::Pointer data)
{
  this->GetFilter()->SetOutputSamples(data);
}

template<class TInputImage, class TMaskImage>
const otb::ogr::DataSource*
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::GetOutputSamples()
{
  return this->GetFilter()->GetOutputSamples();
}


/*
template<class TInputImage, class TMaskImage>
void
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::SetOGRLayer(const otb::ogr::Layer & layer)
{
  this->GetFilter()->SetOGRLayer(layer);
}

template<class TInputImage, class TMaskImage>
const otb::ogr::Layer &
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::GetOGRLayer()
{
  return this->GetFilter()->GetOGRLayer();
}
*/
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

