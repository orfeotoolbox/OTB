#ifndef otbScrollImageView_txx
#define otbScrollImageView_txx

#include "otbScrollImageView.h"


namespace otb
{

template <class TPixel, class OverlayPixelType>
ScrollImageView<TPixel, OverlayPixelType>::
//GLImageView(int x, int y, int w, int h, const char *l):
//VectorImageView<TPixel>(x, y, w, h, l), Fl_Gl_Window(x, y, w, h, l)
ScrollImageView() : GLVectorImageViewBase<TPixel, OverlayPixelType>()
  {
  }

template <class TPixel, class OverlayPixelType>
ScrollImageView<TPixel, OverlayPixelType>::
~ScrollImageView()
{

}

template <class TPixel, class OverlayPixelType>
int
ScrollImageView<TPixel, OverlayPixelType>::handle(int event)
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
//std::cout << "GLVectorImageView<TPixel, OverlayPixelType>::handle DRAG "  << std::endl;
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
    case FL_PUSH:
        IndexType lCenter;
        IndexType lCenterPointImage;
        lCenter[0] = x;
        lCenter[1] = y;
        //Position du centre de l'image en coordonnees image
        lCenterPointImage = this->WindowCoord2ImageCoord(lCenter);
//std::cout << " Scroll Window    : clicked souris -> ecran  : "<<lCenter<< std::endl;
//std::cout << "                                   -> image  : "<<lCenterPointImage<< std::endl;
      // Mise a jour de la fenetre Zoom
      this->GetViewer()->ScrollAction(lCenterPointImage);
    break;
    case FL_KEYUP:
    break;
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
    default:
      break;
      }
  }
  
  return 0;
  
}
 
}; //namespace
#endif

