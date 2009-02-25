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
::ImageLayer() : m_NumberOfHistogramBins(255), m_AutoMinMax(true), m_AutoMinMaxQuantile(0.02)
{
  m_RenderingFunction = DefaultRenderingFunctionType::New();
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
    typename RenderingFilterType::Pointer renderer = RenderingFilterType::New();
    renderer->SetRenderingFunction(m_RenderingFunction);
    renderer->SetInput(m_Quicklook);
    renderer->Update();
    this->SetRenderedQuicklook(renderer->GetOutput());
    }
  // Render extract
  if(this->GetHasExtract())
    {
    // If there are pixels to render
    if(this->GetExtractRegion().GetNumberOfPixels() > 0)
      {

      // Extracting region
      typename ExtractFilterType::Pointer extract = ExtractFilterType::New();
      extract->SetInput(m_Image);
      extract->SetExtractionRegion(this->GetExtractRegion());
      // Rendering
      typename RenderingFilterType::Pointer renderer = RenderingFilterType::New();
      renderer->SetRenderingFunction(m_RenderingFunction);
      renderer->SetInput(extract->GetOutput());
      renderer->Update();
      this->SetRenderedExtract(renderer->GetOutput());
      }
    else
      {
      this->SetHasExtract(false);
      }
    }
  // Render scaled extract
  if(this->GetHasScaledExtract())
    {
    // If there are pixels to render
    if(this->GetScaledExtractRegion().GetNumberOfPixels() > 0)
      {
      // Extracting region
      typename ExtractFilterType::Pointer extract = ExtractFilterType::New();
      extract->SetInput(m_Image);
      extract->SetExtractionRegion(this->GetScaledExtractRegion());
      // Rendering
      typename RenderingFilterType::Pointer renderer = RenderingFilterType::New();
      renderer->SetRenderingFunction(m_RenderingFunction);
      renderer->SetInput(extract->GetOutput());
      renderer->Update();
      this->SetRenderedScaledExtract(renderer->GetOutput());
      }
    else
      {
      this->SetHasScaledExtract(false);
      }
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
}
}
#endif
