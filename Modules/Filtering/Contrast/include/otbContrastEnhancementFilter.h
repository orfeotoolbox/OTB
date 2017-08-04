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

#ifndef otbContrastEnhancementFilter_h
#define otbContrastEnhancementFilter_h

#include "itkImageToImageFilter.h"
#include "otbImage.h"

#include "itkMultiplyImageFilter.h"

namespace otb
{

template < class TInputImage, class TOutputImage >
class ITK_EXPORT ContrastEnhancementFilter :
  public itk::ImageToImageFilter< TInputImage , TOutputImage >
{
public:

  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;

  /** typedef for standard classes. */
  typedef ContrastEnhancementFilter Self;
  typedef itk::ImageToImageFilter< InputImageType, OutputImageType > Superclass;
  typedef itk::SmartPointer< Self > Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  typedef otb::Image< float , 2 > ImageGainType;
  typedef otb::Image< int , 2 > ImageBinType;

  typedef typename InputImageType::PixelType PixelType;

  itkNewMacro(Self);

  itkTypeMacro(ContrastEnhancementFilter, ImageToImageFilter);

  void
  SetThumbnailSize( int wThumbnail , int hThumbnail )
  {
    m_hThumbnail = hThumbnail;
    m_wThumbnail = wThumbnail;
  }

  void
  SetHistoThreshFactor( float threshFactor )
  {
    m_threshFactor = threshFactor;
    m_thresh = true;
  }

  void
  SetHistoSize( int size )
  {
    m_hSize = size;
  }

  void
  SetGainThresh( float lowThresh , float upThresh)
  {
    m_upThresh = upThresh;
    m_lowThresh = lowThresh;
  }

  void
  SetNoData( PixelType noData)
  {
    m_NoData = noData;
  }

  void
  SetHistoThresh(bool thresh)
  {
    m_thresh = thresh;
  }

protected:
  ContrastEnhancementFilter();
  ~ContrastEnhancementFilter() ITK_OVERRIDE {}
  // void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

  typedef itk::MultiplyImageFilter< InputImageType, ImageGainType, OutputImageType > MultiplyImageFilterType;
  typename MultiplyImageFilterType::Pointer m_gainMultiplyer;
  typename ImageBinType::Pointer m_binImage;
  typename ImageGainType::Pointer m_gainImage;

  double m_step;
  PixelType m_min;
  PixelType m_max;
  std::vector< int > m_nbPixel;
  int m_hSize;
  int m_wThumbnail;
  int m_hThumbnail;
  float m_threshFactor;
  bool m_thresh;
  float m_lowThresh;
  float m_upThresh;
  PixelType m_NoData;


  void GenerateData();

private:
  ContrastEnhancementFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  // Equalized a simple histogram with respect to a target and fill the lut 
  void 
  equalized( const std::vector < int > & inputHisto ,
             const std::vector< int > & targetHisto ,
             std::vector < int > & lut );
  
  // Framework to equalized each histogram
  void
  equalized( const std::vector < std::vector < int > > & gridHisto , 
             const std::vector< std::vector < int > > & gridTarget,
             std::vector < std::vector < int > > & gridLut ,
             int nW ,
             int nH );
  
  // Compute histogram with respect of the thumbnail frameworrk
  void
  computehisto( std::vector < std::vector < int > > & gridHisto ,
                int nW );

// Create the target for each thumbnail and compute the min and max
// of the image without taking into account the nodata
  void
  createTarget( std::vector < std::vector < int > > & gridTarget ,
                int nW ,
                int nH );

  // Interpole gain with the nearest neighbours (bilineare)
  float
  interpoleGain( const std::vector < std::vector < int > > & gridLut ,
                 int pixelValue ,
                 typename TInputImage::IndexType index ,
                 int nW ,
                 int nH );

  // Limite the contrast by clipping histogram (See the CLAHE original algorithm
  // for extensive description)
  void
  histoLimiteContrast( std::vector < std::vector < int > > & gridHisto ,
                       const std::vector < int > & hThresh ,
                       int nW ,
                       int nH );
  
  // Limite the contrast by thresholding the gain image (not used)
  void
  gainLimiteContrast();

  // Compute a "binimage" representing that has bin value instead of pixel value
  void
  preprocess();

  // Compute the real gain thanks to the "bin gain"
  float
  postprocess( const std::vector < int > & lut ,
               int pixelValue );

};


}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbContrastEnhancementFilter.txx"
#endif

#endif