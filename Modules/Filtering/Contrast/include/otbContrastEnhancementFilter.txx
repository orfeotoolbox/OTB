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

#ifndef otbContrastEnhancementFilter_txx
#define otbContrastEnhancementFilter_txx

#include "otbContrastEnhancementFilter.h"
#include "itkThresholdImageFilter.h"
#include "itkStatisticsImageFilter.h"
#include "itkImageRegionIterator.h"

#include <limits>


namespace otb
{

template <class TInputImage, class TOutputImage >
ContrastEnhancementFilter < TInputImage , TOutputImage >
::ContrastEnhancementFilter()
{
	m_gainImage = ImageGainType::New();
  m_binImage = ImageBinType::New();
	m_gainMultiplyer = MultiplyImageFilterType::New() ;
	m_wThumbnail = 0;
	m_hThumbnail = 0;
	m_threshFactor = 3;
  m_thresh = false;
	m_lowThresh = 0;
	m_upThresh = INT_MAX;
  m_hSize = 256;
  m_NoData = std::numeric_limits<double>::quiet_NaN();
  m_nbPixel.resize( 1 , 0 );
}

template <class TInputImage, class TOutputImage >
void ContrastEnhancementFilter < TInputImage , TOutputImage >
::equalized( const std::vector< int > & inputHisto ,
             const std::vector< int > & targetHisto ,
	           std::vector< int > & lut)
{
	int countMapValue = 0;
  int countValue = 0;
  lut[countValue] = 0; // Black stays black
  ++countValue;
  int countInput  = inputHisto[ 0 ] + inputHisto[ countValue ];
  lut[lut.size() - 1 ] = lut.size() - 1 ; // White stays white
  int countTarget = targetHisto[ countMapValue ];

  while ( countMapValue< (m_hSize) && countValue< ( m_hSize-1 ) )
    {
    if (countInput > countTarget)
      {
      ++countMapValue;
      countTarget += targetHisto[ countMapValue ];
      }
    else
      {	
      lut[countValue] =  countMapValue ;
      ++countValue;
      countInput  += inputHisto[ countValue ];
      }
    }
  for (uint i = 0 ; i < lut.size() ; i++)
  	{
  	if (lut[i] == -1)
  		{
  		lut[i] = lut[i-1];	
  		}	
  	}
}

template <class TInputImage, class TOutputImage >
void ContrastEnhancementFilter < TInputImage , TOutputImage >
::equalized( const std::vector< std::vector < int > > & gridHisto , 
             const std::vector< std::vector < int > > & gridTarget,
						 std::vector< std::vector < int > > & gridLut ,
						 int nW , 
						 int nH )
{
  for (int i = 0 ; i< nW ; i++)
    {
    for ( int j = 0 ; j<nH ; j++ )
      {
      equalized( gridHisto[ i + j * nW ] , gridTarget[ i+ j * nW ] ,
                 gridLut[ i+ j * nW ]);
      }
    }
}


template <class TInputImage, class TOutputImage >
void ContrastEnhancementFilter < TInputImage , TOutputImage >
::computehisto( std::vector< std::vector < int > > & gridHisto ,
								int nW )
{
  typename itk::ImageRegionConstIterator < ImageBinType > 
      bit( m_binImage , m_binImage->GetRequestedRegion() );

  bit.GoToBegin();
  int i = 0;
  int j = 0;
  while( !bit.IsAtEnd() )
    {
    if ( bit.Get() != -1 )
      {
      i = bit.GetIndex()[0];
      j = bit.GetIndex()[1];
      ++gridHisto[ (i / m_wThumbnail)  + ( j / m_hThumbnail ) * nW] \
                 [ bit.Get() ];
      }
    ++bit;
    }
}

template <class TInputImage, class TOutputImage >
void ContrastEnhancementFilter < TInputImage , TOutputImage >
::createTarget( std::vector < std::vector < int > > & gridTarget ,
                int nW ,
                int nH)
{
  typename InputImageType::ConstPointer input = this->GetInput();
  typename itk::ImageRegionConstIterator < InputImageType > 
      it( input , input->GetRequestedRegion() );

  it.GoToBegin();
  bool isFirst = true;
  int index = 0;
  while ( !it.IsAtEnd() )
    {
    if ( it.Get() != m_NoData)
      {
      if ( isFirst )
        {
        m_min = it.Get();
        m_max = it.Get();
        isFirst = false;
        }
      index =  it.GetIndex()[0] / m_wThumbnail \
            +  (it.GetIndex()[1] / m_hThumbnail) * nW;
      if (m_min>it.Get())
        m_min = it.Get();
      if (m_max<it.Get())
        m_max = it.Get();
      ++m_nbPixel[ index ];
      }
    ++it;
    }
  int rest = 0;
  int offset = 0;
  for (int i = 0 ; i < nW * nH ; i++)
    {
    gridTarget[i].resize( m_hSize , m_nbPixel[ i ] / m_hSize);
    rest = m_nbPixel[i]%m_hSize;
    offset = (m_hSize - rest ) / 2;
    for (int j = 0 ; j < rest ; j++)
      {
      ++gridTarget[i][j + offset];
      }
    ////////////////////////DEBUG////////////////////////
    // std::cout<<"size ="<<m_hSize<<std::endl;
    // std::cout<<"height ="<<m_nbPixel[ i ] / m_hSize<<std::endl;
    // std::cout<<"targetHisto[middle] ="<<gridTarget[i][m_hSize/2]<<std::endl;
    }
}

template <class TInputImage, class TOutputImage >
float ContrastEnhancementFilter < TInputImage , TOutputImage >
::interpoleGain( const std::vector< std::vector < int > > & gridLut,
								 int pixelValue ,
                 typename TInputImage::IndexType index ,
                 int nW ,
                 int nH )
{
  int lutX = index[0]/m_wThumbnail;
  int lutY = index[1]/m_hThumbnail;
  float x = static_cast< float >(index[0]%m_wThumbnail) \
            / static_cast< float >(m_wThumbnail);
  float y = static_cast< float >(index[1]%m_hThumbnail) \
            / static_cast< float >(m_hThumbnail);
  float disty = std::abs(y - 0.5);
  float distx = std::abs(x - 0.5);
  float w = (1 - distx )*(1 - disty );
  float gain = postprocess( gridLut[lutX + lutY * nW ] , pixelValue ) * \
              (1 - distx ) * (1 - disty ) ;
  bool right = x>=0.5 && lutX<(nW - 1) ;
  bool up = y<=0.5 && lutY>0;
  bool left = x<=0.5 && lutX>0;
  bool down = y>=0.5 && lutY<(nH - 1);
  if ( right )
    {
    gain += postprocess( gridLut[lutX + 1 + lutY * nW ] , pixelValue ) \
            * (1 - disty ) * distx;
    w += (1 - disty ) * distx;
    }
  if ( left )
    {
    gain += postprocess( gridLut[lutX + lutY * nW - 1 ] , pixelValue ) \
            * (1 - disty ) * distx;
    w += (1 - disty ) * distx;
    }
  if ( up )
    {
    gain += postprocess( gridLut[lutX + (lutY - 1) * nW ] , pixelValue ) \
            * disty * (1 - distx );
    w += disty * (1 - distx );
    }
  if ( down )
    {
    gain += postprocess( gridLut[lutX + (lutY + 1) * nW ] , pixelValue ) \
            * disty * (1 - distx );
    w += (y - 0.5) * (1 - distx );
    }
  if ( up && left )
    {
    gain += postprocess( gridLut[( lutX - 1 ) + (lutY - 1) * nW ] , pixelValue ) \
            * disty * distx;
    w += disty * distx;
    }
  if ( down && left )
    {
    gain += postprocess( gridLut[( lutX - 1 ) + (lutY + 1) * nW ] , pixelValue ) \
            * disty * distx;
    w += disty * distx;
    }
  if ( up && right )
    {
    gain += postprocess( gridLut[( lutX + 1 ) + (lutY - 1) * nW ] , pixelValue ) \
            * disty * distx;
    w += disty * distx ;
    }
  if ( down && right )
    {
    gain += postprocess( gridLut[( lutX + 1 ) + (lutY + 1) * nW ] , pixelValue ) \
            * disty * distx;
    w += disty * distx;
    }

  return gain/w;
}

template <class TInputImage, class TOutputImage >
void ContrastEnhancementFilter < TInputImage , TOutputImage >
::histoLimiteContrast( std::vector< std::vector < int > > & gridHisto ,
											 const std::vector < int > & hThresh,
											 int nW,
											 int nH )
{
  int histoLength = gridHisto[0].size();
  int nbHisto = nW * nH;
  int toThresh = 0;
  int npx = 0;
  int rest = 0;
  for ( int i = 0 ; i < nbHisto ; i++)
    {
    toThresh = 0;
    for (int j = 0 ; j < histoLength ; j++)
      {
      if ( gridHisto[i][j] > hThresh[i] )
        {
          toThresh+= gridHisto[i][j] - hThresh[i];
          gridHisto[i][j] = hThresh[i] ;
        }
      }
    if (toThresh != 0 )
      {
      npx = toThresh / histoLength;
      rest = toThresh%histoLength;
      for (int j = 0 ; j < histoLength ; j++)
       {
        gridHisto[i][j] += npx ;
        if (rest>0)
          {
          ++gridHisto[i][j];
          --rest;
          }
        }
      }
    }
}


template <class TInputImage, class TOutputImage >
void ContrastEnhancementFilter < TInputImage , TOutputImage >
::gainLimiteContrast()
{
  typedef itk::ThresholdImageFilter< ImageGainType > ThresholdFilterType;
  ThresholdFilterType::Pointer thresholdFilter ( ThresholdFilterType::New() );
  thresholdFilter->SetInput( m_gainImage );
  thresholdFilter->SetOutsideValue (m_upThresh);
  thresholdFilter->ThresholdAbove( m_upThresh );
  thresholdFilter->Update();
  thresholdFilter->SetInput( thresholdFilter->GetOutput() );
  thresholdFilter->SetOutsideValue (m_lowThresh);
  thresholdFilter->ThresholdBelow( m_lowThresh );
  thresholdFilter->Update();
  m_gainImage = thresholdFilter->GetOutput();
}

template <class TInputImage, class TOutputImage >
void ContrastEnhancementFilter < TInputImage , TOutputImage >
::preprocess()
{
  typename InputImageType::ConstPointer input = this->GetInput();

  m_binImage->SetRegions( input->GetLargestPossibleRegion() );
  m_binImage->Allocate();
  m_binImage->SetRequestedRegion( input->GetRequestedRegion() );

  assert( m_hSize > 1 );
  m_step = static_cast<double>( m_max - m_min ) \
                / static_cast<double>( m_hSize -1 );

  typename itk::ImageRegionConstIterator < InputImageType > 
      it( input , input->GetRequestedRegion() );
  typename itk::ImageRegionIterator < ImageBinType > 
      bit ( m_binImage , m_binImage->GetRequestedRegion() );

  it.GoToBegin();
  bit.GoToBegin();
  // std::cout<<"min ="<<m_min<<std::endl;
  // std::cout<<"max ="<<m_max<<std::endl;
  // std::cout<<"step ="<<m_step<<std::endl;
  int pixelValue = 0;
  while( !bit.IsAtEnd() )
  {
  if ( it.Get() != m_NoData )
    {
    pixelValue = static_cast<int>( std::floor( ( it.Get() - m_min ) / m_step ) );
    bit.Set( pixelValue );
    }
  else
    {
    bit.Set(-1);
    }
  ++it;
  ++bit;
  }
}

template <class TInputImage, class TOutputImage >
float ContrastEnhancementFilter < TInputImage , TOutputImage >
::postprocess( const std::vector < int >  & lut ,
               int pixelValue )
{ 
  float denum = pixelValue * m_step + m_min;
  if ( denum == 0 )
    denum = 1.0;
  return (lut[ pixelValue ] * m_step + m_min) \
          / denum ;
}

template <class TInputImage, class TOutputImage >
void ContrastEnhancementFilter < TInputImage , TOutputImage >
::GenerateData()
{
  assert(m_hSize>=0);
  assert(m_threshFactor>0);
	typename OutputImageType::Pointer     output = this->GetOutput();
  typename InputImageType::ConstPointer input = this->GetInput();

  // output->SetRegions( input->GetLargestPossibleRegion() );
  // output->Allocate();
  // output->SetOrigin( input->GetOrigin() );
  // output->SetSpacing( input->GetSpacing() );

  if ( input->GetLargestPossibleRegion().GetSize()[1]%m_hThumbnail != 0 )
    {
    std::cout<<"error : hThumbnail = "<<m_hThumbnail<<" is not a divider of the "
    "input's height"<<std::endl;
    std::cout<<"Image Height = "<<input->GetLargestPossibleRegion().GetSize()[1]
      <<std::endl;
    }
  if ( input->GetLargestPossibleRegion().GetSize()[0]%m_wThumbnail != 0 )
    {
    std::cout<<"error : wThumbnail = "<<m_wThumbnail<<"is not a divider of the "
    "input's width"<<std::endl;
    std::cout<<"Image Width = "<<input->GetLargestPossibleRegion().GetSize()[0]
      <<std::endl;
    }

  int nW = input->GetLargestPossibleRegion().GetSize()[0]/m_wThumbnail;
  int nH = input->GetLargestPossibleRegion().GetSize()[1]/m_hThumbnail;
  
  m_gainImage->SetRegions( input->GetRequestedRegion() );
  m_gainImage->Allocate();
  m_gainImage->SetOrigin( input->GetOrigin() );
  m_gainImage->SetSpacing( input->GetSpacing() );

  std::vector< std::vector < int > > gridHisto( (nH)*(nW) );
  std::vector< std::vector < int > > gridLut( (nH)*(nW) );
  std::vector< std::vector < int > > gridTarget( (nH)*(nW) );
  m_nbPixel.resize( (nH)*(nW) , 0 );

  // Initialize gridLut and gridHisto to zero and -1
  for (int i = 0 ; i<(nH)*(nW) ; i++)
    {
      gridLut[i].resize( m_hSize , -1 );
      gridHisto[i].resize( m_hSize , 0 );
      // gridTarget[i].resize( m_hSize , 0 );
    }

  createTarget( gridTarget , nW , nH );

  preprocess();

  computehisto( gridHisto , nW );


  if (m_thresh)
    {
    std::vector < int > histoTresh(nW*nH);
    for (int i = 0 ; i < nW*nH ; i++ )
      {
      histoTresh[ i ] = m_threshFactor * gridTarget[ i ][0];
      }
    histoLimiteContrast( gridHisto , histoTresh , nW , nH );
    }
  

  for (int i : gridHisto[0])
  { 
    if (i<0)
      std::cout<<"warning histo value "<<i<<std::endl;
  }

  // std::cout<<gridHisto[0][m_hSize/2]<<std::endl;
  // std::cout<<"========one==========="<<std::endl;

	equalized( gridHisto , gridTarget , gridLut , nW , nH );
  
  // std::cout<<gridHisto[0][m_hSize/2]<<std::endl;
  // std::cout<<"========three==========="<<std::endl;

  // ///////////////////Debug ///////////////////
  // std::cout<<"=======LUT======="<<std::endl;
  // for (int i : gridLut[0])
  // {
  //   std::cout<<i<<std::endl;
  // }
  // std::cout<<"=======Histo======="<<std::endl;
  // for (int i : gridHisto[0])
  // { 
  //   if (i<0)
  //     std::cout<<i<<std::endl;
  // }
  

	float gainValue = 0.0;
  typename InputImageType::IndexType index;
  typename itk::ImageRegionConstIterator < ImageBinType > 
      bit( m_binImage , m_binImage->GetRequestedRegion() );
  typename itk::ImageRegionIterator < ImageGainType > 
      git ( m_gainImage , m_gainImage->GetRequestedRegion() );

  git.GoToBegin();
  bit.GoToBegin();
  while ( !bit.IsAtEnd() )
    {
    if ( bit.Get() != -1 )
      {
      index = bit.GetIndex();
      gainValue = interpoleGain( gridLut , bit.Get() ,
                                 index , nW , nH );
      git.Set( gainValue );
      }
    else
      {
      git.Set(1.0);
      }
    // if (i>2000 && i<3000 && j>2000 && j<2010)
    //   std::cout<<gainValue<<std::endl;
    // if (gainValue < 0.0 )
    //   std::cout<<"WARNING "<<std::endl;
    // output->SetPixel(index , static_cast<int>(gainValue * (float)input->GetPixel(index)));
    ++bit;
    ++git;
    }
    
  m_gainMultiplyer->SetInput1( input );
  m_gainMultiplyer->SetInput2( m_gainImage );
  m_gainMultiplyer->Update();
  this->GraftOutput( m_gainMultiplyer -> GetOutput () );
  // GetOutput() = gainImage  input;
  // GetOutput()->SetOrigin( input->GetOrigin() );
  // GetOutput()->SetSpacing( input->GetSpacing() );
}

}

#endif