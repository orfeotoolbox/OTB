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
#ifndef __otbImageLayerRenderingModel_txx
#define __otbImageLayerRenderingModel_txx

#include "otbImageLayerRenderingModel.h"
#include "otbMacro.h"
#include "itkTimeProbe.h"

namespace otb
{

template <class TOutputImage>
ImageLayerRenderingModel<TOutputImage>
::ImageLayerRenderingModel() : m_Name("Default"), m_RasterizedQuicklook(), 
		       m_HasQuicklook(false),m_RasterizedExtract(),m_HasExtract(false),
		       m_ExtractRegion(), m_RasterizedScaledExtract(), m_HasScaledExtract(false),
		       m_ScaledExtractRegion(), m_QuicklookBlendingFilterList(), m_ExtractBlendingFilterList(), 
		       m_ScaledExtractBlendingFilterList()

{
  // Initalize the blending filter list 
  m_QuicklookBlendingFilterList = BlendingFilterListType::New();
  m_ExtractBlendingFilterList = BlendingFilterListType::New();
  m_ScaledExtractBlendingFilterList = BlendingFilterListType::New();  
}

template <class TOutputImage>
ImageLayerRenderingModel<TOutputImage>
::~ImageLayerRenderingModel()
{}

template <class TOutputImage>
void
ImageLayerRenderingModel<TOutputImage>
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
ImageLayerRenderingModel<TOutputImage>
::RenderVisibleLayers()
{
  // Render all visible layers
  for(LayerIteratorType it = this->GetLayers()->Begin();
      it != this->GetLayers()->End(); ++it)
    {
    // If the layer is visible
    if(it.Get()->GetVisible())
      {
      // Set the extracted region
      m_ExtractRegion = this->ConstrainRegion(m_ExtractRegion,it.Get()->GetExtent());
      it.Get()->SetExtractRegion(m_ExtractRegion);
      // Set the scaled extracted region
      m_ScaledExtractRegion = this->ConstrainRegion(m_ScaledExtractRegion,m_ExtractRegion);
      it.Get()->SetScaledExtractRegion(m_ScaledExtractRegion);
      // Render it
      otbMsgDevMacro(<<"ImageLayerRenderingModel::RenderVisibleLayers(): Rendering layer "<<it.Get()->GetName()<<" with regions ("<<m_ExtractRegion.GetIndex()<<" "<<m_ExtractRegion.GetSize()<<") ("<<m_ScaledExtractRegion.GetIndex()<<" "<<m_ScaledExtractRegion.GetSize()<<")");
      it.Get()->Render();
      }
    }
}

template <class TOutputImage>
void
ImageLayerRenderingModel<TOutputImage>
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
  LayerIteratorType it = this->GetLayers()->Begin();

  bool visible = false;

  while(!visible && it != this->GetLayers()->End())
    {
    visible = it.Get()->GetVisible();
    ++it;
    }

  if(!visible)
    {
    // no visible layers, returning
    return;
    }
  --it;
  
  // base layer
  typename LayerType::Pointer baseLayer = it.Get();

  otbMsgDevMacro(<<"ImageLayerRenderingModel::RasterizeVisibleLayers(): Found base layer named "<<it.Get()->GetName());
  
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

  // Move to the next layer
  ++it;
 
  // Walk the remaining layers
  unsigned int count = 0;
  while(it!=this->GetLayers()->End())
    {
    // Populate Blending filter list if needed
    if(count >= m_QuicklookBlendingFilterList->Size())
      {
      m_QuicklookBlendingFilterList->PushBack(BlendingFilterType::New());
      }
     if(count >= m_ExtractBlendingFilterList->Size())
      {
      m_ExtractBlendingFilterList->PushBack(BlendingFilterType::New());
      }
    if(count >= m_ScaledExtractBlendingFilterList->Size())
      {
      m_ScaledExtractBlendingFilterList->PushBack(BlendingFilterType::New());
      }

    // If a layer is visible
    if(it.Get()->GetVisible())
      {
      itk::TimeProbe probe;
      probe.Start();
      // If quicklook is activated and available for this layer 
      if(m_HasQuicklook && it.Get()->GetHasQuicklook())
	{
        // Blend it with the current rasterized quicklook
	typename BlendingFilterType::Pointer blender = m_QuicklookBlendingFilterList->GetNthElement(count);
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
	typename BlendingFilterType::Pointer blender =  m_ExtractBlendingFilterList->GetNthElement(count);
	// Using the blending function of the layer
	blender->SetBlendingFunction(it.Get()->GetBlendingFunction());
	blender->SetInput1(m_RasterizedExtract);
	blender->SetInput2(it.Get()->GetRenderedExtract());
	blender->GetOutput()->SetRequestedRegion(m_ExtractRegion);
	blender->Update();
	// Store the result as being the current rasterized extract
	m_RasterizedExtract = blender->GetOutput();
	}

      // If scaledExtract is activated and available for this layer 
      if(m_HasScaledExtract && it.Get()->GetHasScaledExtract())
	{
	// Blend it with the current rasterized scaledExtract
	typename BlendingFilterType::Pointer blender =  m_ScaledExtractBlendingFilterList->GetNthElement(count);
	// Using the blending function of the layer
	blender->SetBlendingFunction(it.Get()->GetBlendingFunction());
	blender->SetInput1(m_RasterizedScaledExtract);
	blender->SetInput2(it.Get()->GetRenderedScaledExtract());
	blender->GetOutput()->SetRequestedRegion(m_ScaledExtractRegion);
	blender->Update();
	// Store the result as being the current rasterized scaledExtract
	m_RasterizedScaledExtract = blender->GetOutput();
	}
      probe.Stop();
      otbMsgDevMacro("ImageLayerRenderingModel::RasterizeVisibleLayers(): Previous layer rasterized with layer "<<it.Get()->GetName()<<" ( "<<probe.GetMeanTime()<<" s.)");

      }
    ++it;
    ++count;
    }
}

template <class TOutputImage>
void
ImageLayerRenderingModel<TOutputImage>
::Notify(ListenerType * listener)
{
  // Notify the listener
  otbMsgDevMacro(<<"ImageLayerRenderingModel::Notify(): Notifying listener");
  listener->ImageLayerRenderingModelNotify();
}


template <class TOutputImage>
void
ImageLayerRenderingModel<TOutputImage>
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
ImageLayerRenderingModel<TOutputImage>
::SetExtractRegionCenter(const IndexType & index)
{
  // Set the center of the extract region
  IndexType newIndex = index;

// Update Scaled extract center as well
  this->SetScaledExtractRegionCenter(newIndex);

  // Update extract region
  newIndex[0]-=m_ExtractRegion.GetSize()[0]/2;
  newIndex[1]-=m_ExtractRegion.GetSize()[1]/2;
  m_ExtractRegion.SetIndex(newIndex);
}

template <class TOutputImage>
unsigned int
ImageLayerRenderingModel<TOutputImage>
::GetSubsamplingRate()
{
  if(this->GetNumberOfLayers() < 1)
    {
    return 1;
    }
  // Get the lowest layer
  LayerIteratorType it = this->GetLayers()->Begin();
  // Base layer
  typename LayerType::Pointer baseLayer = it.Get();
  return baseLayer->GetQuicklookSubsamplingRate();
}

template <class TOutputImage>
typename ImageLayerRenderingModel<TOutputImage>
::RegionType
ImageLayerRenderingModel<TOutputImage>
::ConstrainRegion(const RegionType & small, const RegionType & big)
{
  RegionType resp = small;
// If not small is larger than big, then crop
  if (small.GetSize()[0]>big.GetSize()[0]
      ||small.GetSize()[1]>big.GetSize()[1])
    {
    resp.Crop(big);
    }
  else
    {
    // Else we can constrain it
    IndexType index = resp.GetIndex();
    typename RegionType::SizeType size = resp.GetSize();

    // For each dimension
    for(unsigned int dim = 0; dim < RegionType::ImageDimension; ++dim)
      {
      // push left if necessary
      if (small.GetIndex()[dim]<big.GetIndex()[dim])
	{
	index[dim]=big.GetIndex()[dim];
	}
      // push right if necessary
      if (index[dim]+size[dim]>=big.GetIndex()[dim]+big.GetSize()[dim])
	{
	index[dim]=big.GetIndex()[dim]+big.GetSize()[dim]-size[dim];
	}
      }
    resp.SetSize(size);
    resp.SetIndex(index);
    }
  return resp;
}

template <class TOutputImage>
void
ImageLayerRenderingModel<TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
  os<<indent<<"Viewer "<<m_Name<<": "<<std::endl;
  for(LayerIteratorType it = this->GetLayers()->Begin();
      it != this->GetLayers()->End(); ++it)
    {
    os<<indent<<it.Get()<<std::endl;
    }
}

} // end namespace otb

#endif 
