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


namespace otb
{

template <class TImage, class TOutputImage>
ImageLayer<TImage,TOutputImage>
::ImageLayer()
{
  m_HistogramList = HistogramListType::New();
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
}

#endif
