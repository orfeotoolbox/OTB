#ifndef otbZoomImageView_txx
#define otbZoomImageView_txx

#include "otbZoomImageView.h"


namespace otb
{

template <class TPixel, class OverlayPixelType>
ZoomImageView<TPixel, OverlayPixelType>::
ZoomImageView() : GLVectorImageViewBase<TPixel, OverlayPixelType>()
  {
        this->m_ViewRectangle = false;
  }

template <class TPixel, class OverlayPixelType>
ZoomImageView<TPixel, OverlayPixelType>::
~ZoomImageView()
{

}

template <class TPixel, class OverlayPixelType>
int
ZoomImageView<TPixel, OverlayPixelType>::handle(int event)
  {

  int x = Fl::event_x();
  int y = Fl::event_y();
  int button;
  static int boxX, boxY;
/*
  switch(event)
    {
    case FL_PUSH:
    case FL_DRAG:
    case FL_RELEASE:
      button = Fl::event_button()-1;       
      if(button <= 0) 
        {
        if(this->cClickMode == CM_BOX) 
          {
          if(event == FL_PUSH)
            {
            boxX = x;
            boxY = y;
            }
          else
            {
            if(event == FL_DRAG)
              {
              this->make_current();
              fl_overlay_clear();
              fl_overlay_rect(boxX, boxY, x-boxY, y-boxY);
              }
            else
              {
              this->make_current();
              fl_overlay_clear();
              }
            }
          }
        }
      break;
    default:
      break;
    }
*/
  int key;
 
  switch(event)
    {
    case FL_HIDE :
                this->GetViewer()->Hide();
                return 0;
                break;
    case FL_KEYBOARD:
    case FL_SHORTCUT:
    key = Fl::event_text()[0];
      switch(key)
        {
        case 'u':
        case 'U':
              this->update();
              return 1;
              break;
        case '+':
        case '=':
          this->winZoom(this->cWinZoom*2);
          this->MajViewRegion();
          this->update();
          this->GetViewer()->ZoomAction();
          return 1;
          break;
        case '-':
        case '_':
          this->winZoom(this->cWinZoom*0.5);
          this->MajViewRegion();
          this->update();
          this->GetViewer()->ZoomAction();
          return 1;
          break;
            default:
              break;
      }
    default:
      break;
  }
  
  return 0;
  
}
 
}; //namespace
#endif

