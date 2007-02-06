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
#ifndef _otbFixedSizeFullImageWidget_txx
#define _otbFixedSizeFullImageWidget_txx

#include "otbFixedSizeFullImageWidget.h"
#include "otbMacro.h"
#include "itkMacro.h"

namespace otb
{
/** 
 * Constructor.
 */
template <class TPixel>
FixedSizeFullImageWidget<TPixel>
::FixedSizeFullImageWidget()
{
  IndexType index;
  m_WindowSize.Fill(256);
  index.Fill(0);
  RegionType region;
  region.SetIndex(index);
  region.SetSize(m_WindowSize);
  this->SetViewedRegion(region);
  m_ImageLoaded=false;
  m_ImageOverlayLoaded=false;
}

/** 
 * Destructor.
 */
template <class TPixel>
FixedSizeFullImageWidget<TPixel>
::~FixedSizeFullImageWidget()
{}
/** 
 * Initialize the widget.
 */
template <class TPixel>
void 
FixedSizeFullImageWidget<TPixel>
::Init(int x, int y, int w, int h, const char * l)
{
  if(!this->GetInput())
    {
      itkExceptionMacro("No input image!");
    }
  else
    {
      this->GetInput()->Update();
      this->SetViewedRegion(this->GetInput()->GetLargestPossibleRegion());
      this->label(l);
      this->resize(x, y, w, h);
    }
}

/** 
 * Resize the widget.
 */
template <class TPixel>
void 
FixedSizeFullImageWidget<TPixel>
::resize(int x, int y, int w, int h)
{
  //otbMsgDebugMacro(<<"resize: "<<x<<" "<<y<<" "<<w<<" "<<h);
  m_WindowSize[0]=w;
  m_WindowSize[1]=h;

  SizeType viewedSize = this->GetViewedRegion().GetSize();
  
  double zoom1 = (double)w/(double)viewedSize[0];
  double zoom2 = (double)h/(double)viewedSize[1];

  this->SetOpenGlIsotropicZoom(zoom1 < zoom2 ? zoom1 : zoom2);

  this->Fl_Gl_Window::resize(x,y,w,h);
  this->redraw();
}
/** 
 * Test if the buffer has to be updated.
 */
template <class TPixel>
bool 
FixedSizeFullImageWidget<TPixel>
::UpdateOpenGlBufferedRegionRequested(void)
{
  // RegionType viewed = this->GetViewedRegion();
//   RegionType buffered = this->GetBufferedRegion();
//   IndexType viewedULCorner = viewed.GetIndex();
//   IndexType bufferedULCorner = buffered.GetIndex();
//   IndexType viewedRDCorner = viewed.GetIndex()+viewed.GetSize();
//   IndexType bufferedRDCorner = buffered.GetIndex()+buffered.GetSize();
//   return ( viewedULCorner[0]<bufferedULCorner[0]
// 	   ||viewedULCorner[1]<bufferedULCorner[1]
// 	   ||viewedRDCorner[0]>bufferedRDCorner[0]
// 	   ||viewedRDCorner[1]>bufferedRDCorner[1]);
  if(m_ImageLoaded)
    {
      return false;
    }
  else
    {
      m_ImageLoaded=true;
      return true;
    }
}
/** 
 * Update OpenGlBuffer. 
 */
template <class TPixel>
void 
FixedSizeFullImageWidget<TPixel>
::UpdateOpenGlBufferedRegion(void)
{
  //otbMsgDebugMacro(<<"UpdateOpenGlBufferedRegion: "<<this->GetViewedRegion());
  this->SetBufferedRegion(this->GetViewedRegion());
}
/** 
 * Test if the buffer has to be updated.
 */
template <class TPixel>
bool 
FixedSizeFullImageWidget<TPixel>
::UpdateOpenGlImageOverlayBufferedRegionRequested(void)
{
if(m_ImageOverlayLoaded)
    {
      
      return false;
    }
  else
    {
      m_ImageOverlayLoaded=true;
      return true;
    }
}
/** 
 * Update OpenGlBuffer. 
 */
template <class TPixel>
void 
FixedSizeFullImageWidget<TPixel>
::UpdateOpenGlImageOverlayBufferedRegion(void)
{
  //otbMsgDebugMacro(<<"UpdateOpenGlBufferedRegion: "<<this->GetViewedRegion());
  this->SetImageOverlayBufferedRegion(this->GetViewedRegion());
}
} // end namespace otb
#endif
