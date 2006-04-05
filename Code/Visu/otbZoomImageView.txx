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
  int key;
 
  switch(event)
    {
    case FL_HIDE :
                this->GetViewer()->Hide();
                return 0;
                break;
    case FL_FOCUS :
                this->update();
                this->GetViewer()->ZoomAction();
                return 1;
                break;
    case FL_KEYBOARD:
    case FL_SHORTCUT:
    key = Fl::event_text()[0];
      switch(key)
        {
        case 'u':
        case 'U':
                this->update();
                this->GetViewer()->ZoomAction();
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

