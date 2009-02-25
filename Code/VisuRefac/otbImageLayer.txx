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
#ifndef __otbImageLayer_txx
#define __otbImageLayer_txx

#include "itkImageRegionConstIterator.h"

namespace otb
{

template <class TImage, class TOutputImage>
ImageLayer<TImage,TOutputImage>
::ImageLayer() : m_Quicklook(), m_Image(), m_Histogram(), m_RenderingFunction(),
		 m_NumberOfHistogramBins(255), m_AutoMinMax(true), m_AutoMinMaxUpToDate(false), m_AutoMinMaxQuantile(0.02),
		 m_QuicklookRenderingFilter(), m_ExtractRenderingFilter(), m_ScaledExtractRenderingFilter(),
		 m_ExtractFilter(), m_ScaledExtractFilter()
{
 // Rendering filters
  m_QuicklookRenderingFilter = RenderingFilterType::New();
  m_ExtractRenderingFilter = RenderingFilterType::New(); 
  m_ScaledExtractRenderingFilter = RenderingFilterType::New();
 
  // Default rendering function
  m_RenderingFunction = DefaultRenderingFunctionType::New();
  m_QuicklookRenderingFilter->SetRenderingFunction(m_RenderingFunction);
  m_ExtractRenderingFilter->SetRenderingFunction(m_RenderingFunction);
  m_ScaledExtractRenderingFilter->SetRenderingFunction(m_RenderingFunction);
  
  // Extract filters 
  m_ExtractFilter = ExtractFilterType::New();
  m_ScaledExtractFilter = ExtractFilterType::New();

  // Wiring
  m_ExtractRenderingFilter->SetInput(m_ExtractFilter->GetOutput());
  m_ScaledExtractRenderingFilter->SetInput(m_ScaledExtractFilter->GetOutput());
}

template <class TImage, class TOutputImage>
ImageLayer<TImage,TOutputImage>
::~ImageLayer()
{}


template <class TImage, class TOutputImage>
void
ImageLayer<TImage,TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

template <class TImage, class TOutputImage>
void
ImageLayer<TImage,TOutputImage>
::Render()
{
  // Render the histogram
  this->RenderHistogram();
  
  // If required, use histogram for auto min/max
  if(m_AutoMinMax)
    {
    this->AutoMinMaxRenderingFunctionSetup();
    }

  // Render images
  this->RenderImages();
}

template <class TImage, class TOutputImage>
void
ImageLayer<TImage,TOutputImage>
::RenderImages()
{
  // Render quicklook
  if(this->GetHasQuicklook())
    {
    m_QuicklookRenderingFilter->Update();
    this->SetRenderedQuicklook(m_QuicklookRenderingFilter->GetOutput());
    }
  // If there are pixels to render
  if(this->GetExtractRegion().GetNumberOfPixels() > 0)
    {
    
    m_ExtractRenderingFilter->Update();
    this->SetRenderedExtract(m_ExtractRenderingFilter->GetOutput());
    }
  else
    {
    this->SetHasExtract(false);
    }
  // Render scaled extract
  // If there are pixels to render
  if(this->GetScaledExtractRegion().GetNumberOfPixels() > 0)
      {
      m_ScaledExtractRenderingFilter->Update();
      this->SetRenderedScaledExtract(m_ScaledExtractRenderingFilter->GetOutput());
      this->SetHasScaledExtract(true);
      }
  else
    {
    this->SetHasScaledExtract(false);
    }
}

template <class TImage, class TOutputImage>
void
ImageLayer<TImage,TOutputImage>
::RenderHistogram()
{
  // Declare the source of the histogram
  ImagePointerType histogramSource;

  // if there is a quicklook, use it for histogram generation
  if(m_Quicklook.IsNotNull())
    {
    histogramSource = m_Quicklook;
    }
  else
    {
    // Else use the full image (update the data)
    histogramSource = m_Image;
    }

  // Check if we need to generate the histogram again
  if( !m_Histogram || (histogramSource->GetUpdateMTime() < histogramSource->GetPipelineMTime()) )
    {
    m_AutoMinMaxUpToDate = false;

    // Update the histogram source
    histogramSource->Update();
    
    // Iterate on the image
    itk::ImageRegionConstIterator<ImageType> it(histogramSource,histogramSource->GetLargestPossibleRegion());
    
    // declare a list to store the samples
    typename ListSampleType::Pointer listSample = ListSampleType::New();
    
    // Set the measurement vector size
    listSample->SetMeasurementVectorSize(histogramSource->GetNumberOfComponentsPerPixel());
    
    // Fill the samples list
    it.GoToBegin();
    while(!it.IsAtEnd())
      {
      listSample->PushBack(it.Get());
      ++it;
      }
    
    // Create the histogram generation filter 
    typename HistogramFilterType::Pointer histogramFilter = HistogramFilterType::New();
    histogramFilter->SetListSample(listSample);
    
    typename HistogramFilterType::HistogramSizeType binSizes(histogramSource->GetNumberOfComponentsPerPixel());
    binSizes.Fill(m_NumberOfHistogramBins);
    
    histogramFilter->SetNumberOfBins(binSizes);
    
    // Generate
    histogramFilter->Update();
    
    // Retrieve the histogram
    m_Histogram = histogramFilter->GetOutput();
    }
}

template <class TImage, class TOutputImage>
void
ImageLayer<TImage,TOutputImage>
::AutoMinMaxRenderingFunctionSetup()
{
  // Check for an existing histogram
  if(m_Histogram.IsNull())
    {
    itkExceptionMacro(<<"Empty histogram, can not use auto min/max evaluation.");
    }

  const unsigned int nbComps = m_Image->GetNumberOfComponentsPerPixel();
  typename RenderingFunctionType::ExtremaVectorType min, max;

  // For each components, use the histogram to compute min and max
  for(unsigned int comp = 0; comp < nbComps;++comp)
    {
    // Compute quantiles
    min.push_back(m_Histogram->Quantile(comp,m_AutoMinMaxQuantile));
    max.push_back(m_Histogram->Quantile(comp,1.-m_AutoMinMaxQuantile));
    }

  // Setup rendering function
  m_RenderingFunction->SetMinimum(min);
  m_RenderingFunction->SetMaximum(max);

  m_QuicklookRenderingFilter->Modified();
  m_ExtractRenderingFilter->Modified();
  m_ScaledExtractRenderingFilter->Modified();
  m_AutoMinMaxUpToDate = true;
}
}
#endif
