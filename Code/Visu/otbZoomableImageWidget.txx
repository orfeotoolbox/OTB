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
#ifndef __otbZoomableImageWidget_txx
#define __otbZoomableImageWidget_txx

#include "otbZoomableImageWidget.h"
#include "otbMacro.h"


namespace otb
{
/**
 * Constructor.
 */
template <class TPixel>
ZoomableImageWidget<TPixel>
::ZoomableImageWidget()
{
  m_ZoomUpperLeftCorner.Fill(0);
}

/**
 * Destructor.
 */
template <class TPixel>
ZoomableImageWidget<TPixel>
::~ZoomableImageWidget()
{}
/**
 * Initialize the widget.
 */
template <class TPixel>
void
ZoomableImageWidget<TPixel>
::Init(int x, int y, int w, int h, const char * l)
{
  if (!this->GetInput())
    {
    itkExceptionMacro("No input image!");
    }
  else
    {
    Superclass::Init(x, y, w, h, l);
//       if(this->GetImageOverlayVisible())
//   {
//     this->GetInputOverlay()->Update();
//   }
    this->label(l);
    this->resize(x, y, w, h);
    }
}
/**
 * Resize the widget.
 */
template <class TPixel>
void
ZoomableImageWidget<TPixel>
::resize(int x, int y, int w, int h)
{
  //otbMsgDebugMacro(<<"resize: "<<x<<" "<<y<<" "<<w<<" "<<h);
  SizeType size;
  size[0] = static_cast<unsigned int>((double) w / this->GetOpenGlIsotropicZoom()) + 1;
  size[1] = static_cast<unsigned int>((double) h / this->GetOpenGlIsotropicZoom()) + 1;
  RegionType region;
  region.SetSize(size);
  region.SetIndex(m_ZoomUpperLeftCorner);
  region.Crop(this->GetInput()->GetLargestPossibleRegion());
  this->SetViewedRegion(region);
  this->Fl_Gl_Window::resize(x, y, w, h);
  this->redraw();
}
/**
 * Test if the buffer has to be updated.
 */
template <class TPixel>
bool
ZoomableImageWidget<TPixel>
::UpdateOpenGlBufferedRegionRequested(void)
{
  /*
  RegionType viewed = this->GetViewedRegion();
  RegionType buffered = this->GetBufferedRegion();
  IndexType  viewedULCorner = viewed.GetIndex();
  IndexType  bufferedULCorner = buffered.GetIndex();
  IndexType  viewedRDCorner = viewed.GetIndex() + viewed.GetSize();
  IndexType  bufferedRDCorner = buffered.GetIndex() + buffered.GetSize();
  return ( viewedULCorner[0]<bufferedULCorner[0]
           ||viewedULCorner[1]<bufferedULCorner[1]
           ||viewedRDCorner[0]>bufferedRDCorner[0]
           ||viewedRDCorner[1]>bufferedRDCorner[1] );
  */
  return true;
}

/**
 * Update OpenGlBuffer.
 */
template <class TPixel>
void
ZoomableImageWidget<TPixel>
::UpdateOpenGlBufferedRegion(void)
{
  //otbMsgDebugMacro(<<"UpdateOpenGlBufferedRegion: "<<this->GetViewedRegion());
  this->SetBufferedRegion(this->GetViewedRegion());
}
/**
 * Set a new zoom factor (>1).
 * \param zoomFactor The new zoom factor.
 */
template <class TPixel>
void
ZoomableImageWidget<TPixel>
::SetZoomFactor(double zoomFactor)
{
  if (zoomFactor < 1) itkExceptionMacro(<< "Zoom factor must be >1 !");

  RegionType region = this->GetViewedRegion();

  SizeType   newSize;
  RegionType newRegion;

  newSize[0] = static_cast<unsigned int>((double) this->w() / zoomFactor);
  newSize[1] = static_cast<unsigned int>((double) this->h() / zoomFactor);
//  otbMsgDebugMacro(<<"SetZoomFactor: newSize ->"<<newSize);
  newRegion.SetIndex(m_ZoomUpperLeftCorner);
  newRegion.SetSize(newSize);
  /// Bug correction, segfault zooming out too much
  if (this->GetInput() && this->GetInput()->GetLargestPossibleRegion().IsInside(newRegion))
    {
    this->SetViewedRegion(newRegion);
    this->SetOpenGlIsotropicZoom(zoomFactor);
    }
}
/**
 * Set the zoom upper-left corner.
 * \param index The upper-left corner index.
 */
template <class TPixel>
void
ZoomableImageWidget<TPixel>
::SetZoomUpperLeftCorner(IndexType index)
{
  SizeType   size;
  RegionType newRegion;

  size = this->GetViewedRegion().GetSize();
//  otbMsgDebugMacro(<<"SetZoomCenter: Size ->"<<size);

//  otbMsgDebugMacro(<<"SetZoomCenter: newIndex ->"<<index);
  newRegion.SetIndex(index);
  newRegion.SetSize(size);
  this->SetViewedRegion(newRegion);
  m_ZoomUpperLeftCorner = index;
}

} // end namespace otb
#endif
