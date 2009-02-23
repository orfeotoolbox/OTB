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
#ifndef __otbImageViewerModel_txx
#define __otbImageViewerModel_txx

#include "otbImageViewerModel.h"

namespace otb
{

template <class TOutputImage>
ImageViewerModel<TOutputImage>
::ImageViewerModel()
{
  // Intializing the layer list
  m_Layers = LayerListType::New();
  // Viewer name
  m_Name = "Default";
  
  // Default: nothing is available
  m_HasQuicklook     = false;
  m_HasExtract       = false;
  m_HasScaledExtract = false;
}

template <class TOutputImage>
ImageViewerModel<TOutputImage>
::~ImageViewerModel()
{

}

template <class TOutputImage>
unsigned int
ImageViewerModel<TOutputImage>
::AddLayer(LayerType * layer)
{
  // Push back and return the size-1
  m_Layers->PushBack(layer);
  return (m_Layers->Size()-1);
}

template <class TOutputImage>
typename ImageViewerModel<TOutputImage>
::LayerType *
ImageViewerModel<TOutputImage>
::GetLayer(unsigned int index)
{
  // Check if not out of bound and return the ith element
  if(index >= m_Layers->Size())
    {
    return NULL;
    }
  else
    {
    return m_Layers->GetNthElement(index);
    }
}
template <class TOutputImage>
bool
ImageViewerModel<TOutputImage>
::DeleteLayer(unsigned int index)
{
// Check if not out of bound and delete the ith element
  if(index >= m_Layers->Size())
    {
    return false;
    }
  else
    {
    m_Layers->Erase(index);
    return true;
    }
}

template <class TOutputImage>
typename ImageViewerModel<TOutputImage>
::LayerType *
ImageViewerModel<TOutputImage>
::GetLayerByName(std::string name)
{
  LayerType * resp = NULL;
  LayerIteratorType it = m_Layers->Begin();
  bool found  = false;

  // Look for the layer named after name
  while(it!=m_Layers->End() && !found)
    {
    if(it.Get()->GetName() == name)
      {
      resp = it.Get();
      found = true;
      }
    }
  return resp;
}

template <class TOutputImage>
bool
ImageViewerModel<TOutputImage>
::DeleteLayerByName(std::string name)
{
  LayerIteratorType it = m_Layers->Begin();
  bool found  = false;
  unsigned int index = 0;

  // Look for the layer named after name
  while(it!=m_Layers->End() && !found)
    {
    if(it.Get()->GetName() == name)
      {
      found = true;
      }
    ++index;
    }
  
  if(found)
    {
    m_Layers->Erase(index-1);
    }

  return found;
}

template <class TOutputImage>
void
ImageViewerModel<TOutputImage>
::ClearLayers()
{
  // Clear layers list
  m_Layers->Clear();
}

template <class TOutputImage>
unsigned int
ImageViewerModel<TOutputImage>
::GetNumberOfLayers(void)
{
  // return layer list size
  return m_Layers->Size();
}

template <class TOutputImage>
void
ImageViewerModel<TOutputImage>
::Update()
{
  // Render all visible layers
  this->RenderVisibleLayers();
  // Rasterize all visible layers
  this->RasterizeVisibleLayers();
  // Notify all listeners
  this->NotifyAll();
}

template <class TOutputImage>
void
ImageViewerModel<TOutputImage>
::RenderVisibleLayers()
{
  // Render all visible layers
  for(LayerIteratorType it = m_Layers->Begin();
      it != m_Layers->End(); ++it)
    {
    // If the layer is visible
    if(it.Get()->GetVisible())
      {
      // Set the extracted region
      it.Get()->SetExtractRegion(m_ExtractRegion);
      // Set the scaled extracted region
      it.Get()->SetScaledExtractRegion(m_ScaledExtractRegion);
      // Render it
      it.Get()->Render();
      }
    }
}

template <class TOutputImage>
void
ImageViewerModel<TOutputImage>
::RasterizeVisibleLayers()
{
  // If there are no layer to render
  if(this->GetNumberOfLayers() == 0)
    {
    // Ensure nothing is available
    m_HasQuicklook     = false;
    m_HasExtract       = false;
    m_HasScaledExtract = false;
    // and return without doing anything
    return;
    }
  
  // Get the lowest layer
  LayerIteratorType it = m_Layers->Begin();
  
  // Base layer
  typename LayerType::Pointer baseLayer = it.Get();
  
  // Configure base layer rasterization
  if(baseLayer->GetHasQuicklook())
    {
    m_HasQuicklook = true;
    m_RasterizedQuicklook = baseLayer->GetRenderedQuicklook();
    }

  if(baseLayer->GetHasExtract())
    {
    m_HasExtract = true;
    m_RasterizedExtract = baseLayer->GetRenderedExtract();
    }

  if(baseLayer->GetHasScaledExtract())
    {
    m_HasScaledExtract = true;
    m_RasterizedScaledExtract = baseLayer->GetRenderedScaledExtract();
    }
  
  // Walk the remaining layers
  while(it!=m_Layers->End())
    {
    // If a layer is visible
    if(it.Get()->GetVisible())
      {
      // If quicklook is activated and available for this layer 
      if(m_HasQuicklook && it.Get()->GetHasQuicklook())
	{
        // Blend it with the current rasterized quicklook
	typename BlendingFilterType::Pointer blender = BlendingFilterType::New();
	// Using the blending function of the layer
	blender->SetBlendingFunction(it.Get()->GetBlendingFunction());
	blender->SetInput1(m_RasterizedQuicklook);
	blender->SetInput2(it.Get()->GetRenderedQuicklook());
	blender->Update();
	// Store the result as being the current rasterized quicklook
	m_RasterizedQuicklook = blender->GetOutput();
	}
      
      // If extract is activated and available for this layer 
      if(m_HasExtract && it.Get()->GetHasExtract())
	{
        // Blend it with the current rasterized extract
	typename BlendingFilterType::Pointer blender = BlendingFilterType::New();
	// Using the blending function of the layer
	blender->SetBlendingFunction(it.Get()->GetBlendingFunction());
	blender->SetInput1(m_RasterizedExtract);
	blender->SetInput2(it.Get()->GetRenderedExtract());
	blender->Update();
	// Store the result as being the current rasterized extract
	m_RasterizedExtract = blender->GetOutput();
	}

      // If scaledExtract is activated and available for this layer 
      if(m_HasScaledExtract && it.Get()->GetHasScaledExtract())
	{
	// Blend it with the current rasterized scaledExtract
	typename BlendingFilterType::Pointer blender = BlendingFilterType::New();
	// Using the blending function of the layer
	blender->SetBlendingFunction(it.Get()->GetBlendingFunction());
	blender->SetInput1(m_RasterizedScaledExtract);
	blender->SetInput2(it.Get()->GetRenderedScaledExtract());
	blender->Update();
	// Store the result as being the current rasterized scaledExtract
	m_RasterizedScaledExtract = blender->GetOutput();
	}
      }
    ++it;
    }
}

template <class TOutputImage>
void
ImageViewerModel<TOutputImage>
::Notify(ListenerType * listener)
{
  listener->ImageViewerNotify();
}

template <class TOutputImage>
void
ImageViewerModel<TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
  os<<indent<<"Viewer "<<m_Name<<": "<<std::endl;
  for(LayerIteratorType it = m_Layers->Begin();
      it != m_Layers->End(); ++it)
    {
    os<<indent<<it.Get()<<std::endl;
    }
}

} // end namespace otb

#endif 
