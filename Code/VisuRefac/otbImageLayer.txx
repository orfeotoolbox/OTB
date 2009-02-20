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
::ImageLayer() : m_NumberOfHistogramBins(255)
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
  this->RenderHistogram();
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
  // Render scaled extract
  if(this->GetHasScaledExtract())
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
}
}
#endif
