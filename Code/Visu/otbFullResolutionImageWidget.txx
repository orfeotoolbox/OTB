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
#ifndef _otbFullResolutionImageWidget_txx
#define _otbFullResolutionImageWidget_txx

#include "otbFullResolutionImageWidget.h"
#include "otbMacro.h"
#include "itkMacro.h"

namespace otb
{
/** 
 * Constructor.
 */
template <class TPixel>
FullResolutionImageWidget<TPixel>
::FullResolutionImageWidget()
{
  SizeType size;
  size.Fill(256);
  m_UpperRightCorner.Fill(0);
  RegionType region;
  region.SetIndex(m_UpperRightCorner);
  region.SetSize(size);
  this->SetViewedRegion(region);
}

/** 
 * Destructor.
 */
template <class TPixel>
FullResolutionImageWidget<TPixel>
::~FullResolutionImageWidget()
{}
/** 
 * Initialize the widget.
 */
template <class TPixel>
void 
FullResolutionImageWidget<TPixel>
::Init(int x, int y, int w, int h, const char * l)
{
  if(!this->GetInput())
    {
      itkExceptionMacro("No input image!");
    }
  else
    {
      this->GetInput()->Update();
      if(this->GetImageOverlayVisible())
	{
	  this->GetInputOverlay()->Update();
	}
      this->label(l);
      this->resize(x, y, w, h);
    }
}
/** 
 * Resize the widget.
 */
template <class TPixel>
void 
FullResolutionImageWidget<TPixel>
::resize(int x, int y, int w, int h)
{
  //otbMsgDebugMacro(<<"resize: "<<x<<" "<<y<<" "<<w<<" "<<h);
  IndexType index;
  SizeType size;
  size[0]=w;
  size[1]=h;
  RegionType region;
  region.SetSize(size);
  region.SetIndex(m_UpperRightCorner);
  region.Crop(this->GetInput()->GetLargestPossibleRegion());
  this->SetViewedRegion(region);
  this->Fl_Gl_Window::resize(region.GetIndex()[0],
	       region.GetIndex()[1], 
	       region.GetSize()[0],
	       region.GetSize()[1]);
  this->redraw();
}
/** 
 * Test if the buffer has to be updated.
 */
template <class TPixel>
bool 
FullResolutionImageWidget<TPixel>
::UpdateOpenGlBufferedRegionRequested(void)
{
  RegionType viewed = this->GetViewedRegion();
  RegionType buffered = this->GetBufferedRegion();
  IndexType viewedULCorner = viewed.GetIndex();
  IndexType bufferedULCorner = buffered.GetIndex();
  IndexType viewedRDCorner = viewed.GetIndex()+viewed.GetSize();
  IndexType bufferedRDCorner = buffered.GetIndex()+buffered.GetSize();
  return ( viewedULCorner[0]<bufferedULCorner[0]
	   ||viewedULCorner[1]<bufferedULCorner[1]
	   ||viewedRDCorner[0]>bufferedRDCorner[0]
	   ||viewedRDCorner[1]>bufferedRDCorner[1]);
}

/** 
 * Test if the buffer has to be updated.
 */
template <class TPixel>
bool 
FullResolutionImageWidget<TPixel>
::UpdateOpenGlImageOverlayBufferedRegionRequested(void)
{
  RegionType viewed = this->GetViewedRegion();
  RegionType buffered = this->GetImageOverlayBufferedRegion();
  IndexType viewedULCorner = viewed.GetIndex();
  IndexType bufferedULCorner = buffered.GetIndex();
  IndexType viewedRDCorner = viewed.GetIndex()+viewed.GetSize();
  IndexType bufferedRDCorner = buffered.GetIndex()+buffered.GetSize();
  return ( viewedULCorner[0]<bufferedULCorner[0]
	   ||viewedULCorner[1]<bufferedULCorner[1]
	   ||viewedRDCorner[0]>bufferedRDCorner[0]
	   ||viewedRDCorner[1]>bufferedRDCorner[1]);
}
/** 
 * Update OpenGlBuffer. 
 */
template <class TPixel>
void 
FullResolutionImageWidget<TPixel>
::UpdateOpenGlBufferedRegion(void)
{
  //otbMsgDebugMacro(<<"UpdateOpenGlBufferedRegion: "<<this->GetViewedRegion());
  this->SetBufferedRegion(this->GetViewedRegion());
}
/** 
 * Update OpenGlBuffer. 
 */
template <class TPixel>
void 
FullResolutionImageWidget<TPixel>
::UpdateOpenGlImageOverlayBufferedRegion(void)
{
  //otbMsgDebugMacro(<<"UpdateOpenGlBufferedRegion: "<<this->GetViewedRegion());
  this->SetImageOverlayBufferedRegion(this->GetViewedRegion());
}
/**
 * Set upper right corner position in image.
 * \param index the upper right corner index.
 */
template <class TPixel>
void 
FullResolutionImageWidget<TPixel>
::SetUpperRightCorner(IndexType index)
{
  m_UpperRightCorner=index;
  RegionType region = this->GetViewedRegion();
  region.SetIndex(index);
  region.Crop(this->GetInput()->GetLargestPossibleRegion());
  this->SetViewedRegion(region);
}
} // end namespace otb
#endif
