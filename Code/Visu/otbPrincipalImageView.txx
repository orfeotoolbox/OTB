#ifndef otbPrincipalImageView_txx
#define otbPrincipalImageView_txx

#include "otbPrincipalImageView.h"


namespace otb
{

template <class TPixel, class OverlayPixelType>
PrincipalImageView<TPixel, OverlayPixelType>::
//GLImageView(int x, int y, int w, int h, const char *l):
//VectorImageView<TPixel>(x, y, w, h, l), Fl_Gl_Window(x, y, w, h, l)
PrincipalImageView() : GLVectorImageViewBase<TPixel, OverlayPixelType>()
  {
  }

template <class TPixel, class OverlayPixelType>
PrincipalImageView<TPixel, OverlayPixelType>::
~PrincipalImageView()
{

}

template <class TPixel, class OverlayPixelType>
int
PrincipalImageView<TPixel, OverlayPixelType>::handle(int event)
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
#if 0
  static int fastMov = 0;
  int pace;

  int imgShiftSize = (int)(this->cWinSizeX/10/this->cWinZoom);
  if(imgShiftSize<1)
    {
    imgShiftSize = 1;
    }

  double zoomBase = this->cW/(this->cDimSize[0 /*this->cWinOrder[0]*/]*(fabs(this->cSpacing[0 /*this->cWinOrder[0]*/])/fabs(this->cSpacing[0])));
  if(zoomBase >
      this->cH/(this->cDimSize[1 /*this->cWinOrder[1]*/]*(fabs(this->cSpacing[1 /*this->cWinOrder[1]*/])/fabs(this->cSpacing[0]))))
    {
    zoomBase = this->cH/(this->cDimSize[1 /*this->cWinOrder[1]*/]*(fabs(this->cSpacing[1 /*this->cWinOrder[1]*/])/fabs(this->cSpacing[0])));
    }

  double scale0 = this->cWinZoom * zoomBase * fabs(this->cSpacing[0 /*this->cWinOrder[0]*/])/fabs(this->cSpacing[0]);
  double scale1 = this->cWinZoom * zoomBase * fabs(this->cSpacing[1 /*this->cWinOrder[1]*/])/fabs(this->cSpacing[0]);
#endif

  switch(event)
    {
    case FL_PUSH:
    case FL_DRAG:
        IndexType lCenter;
        IndexType lCenterPointImage;
        lCenter[0] = x;
        lCenter[1] = y;
        //Position du centre de l'image en coordonnees image
        lCenterPointImage = this->WindowCoord2ImageCoord(lCenter);
        // Mise a jour de la fenetre Zoom
        this->GetViewer()->PrincipalAction(lCenterPointImage);
#if 0

//      button = Fl::event_button()-1;
      if(button <= 0)
        {
        if(this->cClickMode == CM_SELECT || this->cClickMode == CM_BOX) 
          {
          double originX = 0;
          double originY = 0;
          if(this->cWinZoom<=1)
            {
            if(this->cW-scale0*this->cDimSize[0 /*this->cWinOrder[0]*/]>0)
              {
              originX = (int)((this->cW-scale0*this->cDimSize[0 /*this->cWinOrder[0]*/])/2.0);
              }
            if(this->cH-scale1*this->cDimSize[1 /*this->cWinOrder[1]*/]>0)
              {
              originY = (int)((this->cH-scale1*this->cDimSize[1 /*this->cWinOrder[1]*/])/2.0);
              }
            }
          float p[2];
/*          p[this->cWinOrder[0]] = this->cWinMinX + ( (1-this->cFlipX[this->cWinOrientation])*(x-originX) 
            + (this->cFlipX[this->cWinOrientation])*(this->cW-x-originX) ) 
            / scale0;
          p[this->cWinOrder[1]] = this->cWinMinY + (this->cFlipY[this->cWinOrientation]*(y-originY) 
            + (1-this->cFlipY[this->cWinOrientation])*(this->cH-y-originY)) 
            / scale1;*/
          p[0] = this->cWinMinX + ( (this->cW-x-originX) ) / scale0;
          p[1] = this->cWinMinY + ( (y-originY) )/ scale1;
          if(p[0 /*this->cWinOrder[0]*/]<this->cWinMinX) 
            p[0 /*this->cWinOrder[0]*/] = this->cWinMinX;
          if(p[0 /*this->cWinOrder[0]*/]>this->cWinMaxX) 
            p[0 /*this->cWinOrder[0]*/] = this->cWinMaxX;
          if(p[1 /*this->cWinOrder[1]*/]<this->cWinMinY) 
            p[1 /*this->cWinOrder[1]*/] = this->cWinMinY;
          if(p[1 /*this->cWinOrder[1]*/]>this->cWinMaxY) 
            p[1 /*this->cWinOrder[1]*/] = this->cWinMaxY;
/*          if(this->cImageMode != IMG_MIP)
            p[this->cWinOrder[2]] = this->cWinCenter[this->cWinOrder[2]];
          else
            p[this->cWinOrder[2]] = this->cWinZBuffer[(int)p[this->cWinOrder[0]]
            - this->cWinMinX 
            + ((int)p[this->cWinOrder[1]]
            - this->cWinMinY)
            * this->cWinDataSizeX];
*/
          if(this->cClickMode == CM_SELECT)
            this->clickSelect(p[0], p[1]/*, p[2]*/);
          else if(this->cClickMode == CM_BOX)
            if(event == FL_PUSH)
              this->boxMin(p[0], p[1]/*, p[2]*/);

            return 1;
          }
        }
      return 0;
#endif
      break;
    case FL_HIDE :
                this->GetViewer()->Hide();
                return 0;
                break;
    case FL_RELEASE:
#if 0
      if(this->cClickMode == CM_BOX)
        {
        double originX = 0;
        double originY = 0;
        if(this->cWinZoom<=1)
          {
          if(this->cW-scale0*this->cDimSize[0 /*this->cWinOrder[0]*/]>0)
            {
            originX = (int)((this->cW-scale0*this->cDimSize[0 /*this->cWinOrder[0]*/])/2.0);
            }
          if(this->cH-scale1*this->cDimSize[1 /*this->cWinOrder[1]*/]>0)
            {
            originY = (int)((this->cH-scale1*this->cDimSize[1 /*this->cWinOrder[1]*/])/2.0);
            }
          }
        float p[2];
/*        p[this->cWinOrder[0]] = this->cWinMinX + (this->cFlipX[this->cWinOrientation]*(this->cW-x-originX) 
          + (1-this->cFlipX[this->cWinOrientation])*(x-originX)) 
          / scale0;
        p[this->cWinOrder[1]] = this->cWinMinY + (this->cFlipY[this->cWinOrientation]*(y-originY) 
          + (1-this->cFlipY[this->cWinOrientation])*(this->cH-y-originY)) 
          / scale1;*/
        p[0] = this->cWinMinX + ((this->cW-x-originX) + (0)*(x-originX)) / scale0;
        p[1] = this->cWinMinY + ((y-originY) + (0)*(this->cH-y-originY)) / scale1;

        if(p[0 /*this->cWinOrder[0]*/]<this->cWinMinX) 
          p[0 /*this->cWinOrder[0]*/] = this->cWinMinX;
        if(p[0 /*this->cWinOrder[0]*/]>this->cWinMaxX) 
          p[0 /*this->cWinOrder[0]*/] = this->cWinMaxX;
        
        if(p[1 /*this->cWinOrder[1]*/]<this->cWinMinY) 
          p[1 /*this->cWinOrder[1]*/] = this->cWinMinY;
        if(p[1 /*this->cWinOrder[1]*/]>this->cWinMaxY) 
          p[1 /*this->cWinOrder[1]*/] = this->cWinMaxY;
        
/*        if(this->cImageMode != IMG_MIP)
          p[this->cWinOrder[2]] = this->cWinCenter[this->cWinOrder[2]];
        else
          p[this->cWinOrder[2]] = this->cWinZBuffer[(int)p[this->cWinOrder[0]]
          - this->cWinMinX 
          + ((int)p[this->cWinOrder[1]]
          - this->cWinMinY)
          * this->cWinDataSizeX];*/
        this->boxMax(p[0], p[1]/*, p[2]*/);
        return 1;
        }
#endif
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
    default:
      break;
  }
  
  return 0;
  
}

 
}; //namespace
#endif

