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
::ImageViewerModel() : m_Name("Default"), m_Layers(), m_RasterizedQuicklook(), 
		       m_HasQuicklook(false),m_RasterizedExtract(),m_HasExtract(false),
		       m_ExtractRegion(), m_SubsampledExtractRegion(), m_RasterizedScaledExtract(), m_HasScaledExtract(false),
		       m_ScaledExtractRegion()

{
  // Intializing the layer list
  m_Layers = LayerListType::New();

  
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
  // Multiple concurrent update guards
  if(!m_Updating)
    {
    m_Updating = true;
    // Render all visible layers
    this->RenderVisibleLayers();
    // Rasterize all visible layers
    this->RasterizeVisibleLayers();
    // Notify all listeners
    this->NotifyAll();
    m_Updating = false;
    }
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
   
    // Update the subsampled extract region 
    m_SubsampledExtractRegion = m_ExtractRegion;
    typename RegionType::SizeType size = m_SubsampledExtractRegion.GetSize();
    typename RegionType::IndexType index = m_SubsampledExtractRegion.GetIndex();
    size[0]/=baseLayer->GetQuicklookSubsamplingRate();
    size[1]/=baseLayer->GetQuicklookSubsamplingRate();
    index[0]/=baseLayer->GetQuicklookSubsamplingRate();
    index[1]/=baseLayer->GetQuicklookSubsamplingRate();
    m_SubsampledExtractRegion.SetIndex(index);
    m_SubsampledExtractRegion.SetSize(size);
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

  // Move to the next layer
  ++it;
  
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
  // Notify the listener
  listener->ImageViewerNotify();
}


template <class TOutputImage>
void
ImageViewerModel<TOutputImage>
::SetScaledExtractRegionCenter(const IndexType & index)
{
  // Set the center of the scaled extract region
  IndexType newIndex = index;
  newIndex[0]-=m_ScaledExtractRegion.GetSize()[0]/2;
  newIndex[1]-=m_ScaledExtractRegion.GetSize()[1]/2;
  m_ScaledExtractRegion.SetIndex(newIndex);
}

template <class TOutputImage>
void
ImageViewerModel<TOutputImage>
::SetExtractRegionCenter(const IndexType & index)
{
  // Set the center of the extract region
  IndexType newIndex = index;
  newIndex[0]-=m_ExtractRegion.GetSize()[0]/2;
  newIndex[1]-=m_ExtractRegion.GetSize()[1]/2;
  m_ExtractRegion.SetIndex(newIndex);
}

template <class TOutputImage>
typename ImageViewerModel<TOutputImage>
::RegionType
ImageViewerModel<TOutputImage>
::ConstrainRegion(const RegionType & region, const RegionType & largest)
{
  // First check if the case is trivial
  if(region.GetNumberOfPixels() == 0 || largest.IsInside(region))
    {
    return region;
    }
  
  RegionType resp = region;
  typename RegionType::IndexType index = resp.GetIndex();
  typename RegionType::SizeType size   = resp.GetSize();

  for(unsigned int dim = 0; dim<RegionType::ImageDimension;++dim)
    {
    const int offset = index[dim]+size[dim] - largest.GetIndex()[dim] + largest.GetSize()[dim];
    // If the region is not larger than the largest, wen can constrain
    if(largest.GetSize()[dim] > size[dim])
      {
      // If the region is to the left, push left
      if(index[dim] < largest.GetIndex()[dim])
	{
	index[dim] = largest.GetIndex()[dim];
	}
      	// If the region is to the right, push right
      else if(offset > 0)
	{
	index[dim]-=offset;
	}
      }
    else
      {
      // else crop
      index[dim] = largest.GetIndex()[dim];
      size[dim]   = largest.GetSize()[dim];
      }
      }
  resp.SetIndex(index);
  resp.SetSize(size);

  return resp;
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
