#ifndef otbZoomImageView_txx
#define otbZoomImageView_txx

#include "otbZoomImageView.h"


namespace otb
{

template <class TPixel, class OverlayPixelType>
ZoomImageView<TPixel, OverlayPixelType>::
//GLImageView(int x, int y, int w, int h, const char *l):
//VectorImageView<TPixel>(x, y, w, h, l), Fl_Gl_Window(x, y, w, h, l)
ZoomImageView() : GLVectorImageView<TPixel, OverlayPixelType>()
  {
        this->m_IdentWindowView = Superclass::ZOOM_IMAGE_VIEW;
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

  int key;
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

 
  switch(event)
    {
    case FL_PUSH:
    case FL_DRAG:
      button = Fl::event_button()-1;
//std::cout << " Zoom Window : clicked souris -> ecran  : "<<x<<","<<y<< std::endl;
//std::cout << "                              -> region : "<<this->m_ViewImageRegion.GetIndex()[0] + x<<","<<this->m_ViewImageRegion.GetIndex()[1] + y<< std::endl;
      
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
          p[1] = this->cWinMinY + ( (y-originY) )          / scale1;

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
            * this->cWinDataSizeX];*/
          if(this->cClickMode == CM_SELECT)
            this->clickSelect(p[0], p[1]);
          else if(this->cClickMode == CM_BOX)
            if(event == FL_PUSH)
              this->boxMin(p[0], p[1]);
            return 1;
          }
        }
      return 0;
      break;
    case FL_RELEASE:
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
        p[0] = this->cWinMinX + ((this->cW-x-originX) ) / scale0;
        p[1] = this->cWinMinY + ((y-originY)  )         / scale1;

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
      return 0;
      break;
    case FL_KEYUP:
    //when pressing down ">" or "<" key, scrolling will go faster
    //when the key is released, scrolling speed go back to 1
    fastMov = 0;
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
//std::cout << " this->cWinZoom  = "<<this->cWinZoom<<std::endl;
              return 1;
              break;
        case 'r':
          {
          this->winZoom(1);
          this->winCenter();
          this->imageMode(IMG_VAL);
          this->iwMax(this->cDataMax);
          this->iwMin(this->cDataMin);
          this->update();
          return 1;
          break;
          }
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
          case 'q':
              this->iwMax(this->cIWMax-(float)0.02*(this->cDataMax-this->cDataMin));
              this->update();
              return 1;
              break;
          case 'w':
              this->iwMax(this->cIWMax+(float)0.02*(this->cDataMax-this->cDataMin));
              this->update();
              return 1;
              break;
          case 'a':
              this->iwMin(this->cIWMin-(float)0.02*(this->cDataMax-this->cDataMin));
              this->update();
              return 1;
              break;
          case 's':
              this->iwMin(this->cIWMin+(float)0.02*(this->cDataMax-this->cDataMin));
              this->update();
              return 1;
              break;
          case 'i':
              {
              int newY;
/*              if(this->cFlipY[this->cWinOrientation])
                newY = this->cWinCenter[this->cWinOrder[1]]-imgShiftSize;
              else*/
                newY = this->cWinCenter[1 /*this->cWinOrder[1]*/]+imgShiftSize;
              this->cWinCenter[1 /*this->cWinOrder[1]*/] = newY;
              this->winCenter(this->cWinCenter[0], this->cWinCenter[1]/*, this->cWinCenter[2]*/);
              this->update();
              return 1;
              break;
              }
          case 'm':
              {
              int newY;
/*              if(this->cFlipY[this->cWinOrientation])
                newY = this->cWinCenter[this->cWinOrder[1]]+imgShiftSize;
              else*/
                newY = this->cWinCenter[1 /*this->cWinOrder[1]*/]-imgShiftSize;
              this->cWinCenter[1 /*this->cWinOrder[1]*/] = newY;
              this->winCenter(this->cWinCenter[0], this->cWinCenter[1]/*, this->cWinCenter[2]*/);
              this->update();
              return 1;
              break;
              }
          case 'j':
              {
              int newX;
/*              if(this->cFlipX[this->cWinOrientation])
                newX = this->cWinCenter[this->cWinOrder[0]]+imgShiftSize;
              else*/
                newX = this->cWinCenter[0 /*this->cWinOrder[0]*/]-imgShiftSize;
              this->cWinCenter[0 /*this->cWinOrder[0]*/] = newX;
              winCenter(this->cWinCenter[0], this->cWinCenter[1]/*, this->cWinCenter[2]*/);
              this->update();
              return 1;
              break;
              }
          case 'k':
              {
              int newX;
/*              if(this->cFlipX[this->cWinOrientation])
                newX = this->cWinCenter[this->cWinOrder[0]]-imgShiftSize;
              else*/
                newX = this->cWinCenter[0 /*this->cWinOrder[0]*/]+imgShiftSize;
              this->cWinCenter[0 /*this->cWinOrder[0]*/] = newX;
              this->winCenter(this->cWinCenter[0], this->cWinCenter[1]/*, this->cWinCenter[2]*/);
              this->update();
              return 1;
              break;
              }
          case 'A':
              this->cViewAxisLabel = !this->cViewAxisLabel;
              this->update();
              return 1;
              break;
          case 'C':
              this->cViewCrosshairs = !this->cViewCrosshairs;
              this->update();
              return 1;
              break;
          case 'V':
              this->cViewValue = !this->cViewValue;
              this->update();
              return 1;
              break;
          case 'P':
              this->cViewValuePhysicalUnits = !this->cViewValuePhysicalUnits;
              this->update();
              return 1;
              break;
          case 'D':
              this->cViewDetails = !this->cViewDetails;
              this->update();
              return 1;
              break;
          case 'O':
              this->cViewOverlayData = !this->cViewOverlayData;
              this->update();
              return 1;
              break;
          case 'p':
              this->saveClickedPointsStored();
              return 1;
              break;
          case 'T':
              this->cViewClickedPoints = !this->cViewClickedPoints;
              this->update();
              return 1;
              break;
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

