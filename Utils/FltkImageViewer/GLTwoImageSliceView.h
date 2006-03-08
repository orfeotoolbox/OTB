/*=========================================================================

 Program:   Insight Segmentation & Registration Toolkit
 Module:    $RCSfile: GLTwoImageSliceView.h,v $
 Language:  C++
 Date:      $Date: 2005/10/16 15:02:54 $
 Version:   $Revision: 1.9 $
 
  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.
  
   This software is distributed WITHOUT ANY WARRANTY; without even 
   the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
   PURPOSE.  See the above copyright notices for more information.
   
=========================================================================*/
#ifndef GLTWOIMAGESLICEVIEW_H
#define GLTWOIMAGESLICEVIEW_H

#include <FL/gl.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Gl_Window.H>

#include "SliceView.h"
#include "GLSliceView.h"

#include <math.h>

namespace itk {
  
/**
* GLSliceView : Derived from abstract class SliceView and Fl_Gl_Window
* See SliceView.h for details...
  **/
template <class ImagePixelType, class OverlayPixelType>
class GLTwoImageSliceView 
  : public GLSliceView<ImagePixelType, OverlayPixelType> 
  {
  public:


    typedef itk::Image< ImagePixelType, 3> ImageType;
    typedef typename ImageType::Pointer ImagePointer;
    typedef typename ImageType::ConstPointer ImageConstPointer;
    typedef GLSliceView<ImagePixelType, OverlayPixelType> Superclass;
    typedef typename Superclass::RegionType RegionType;
    typedef typename Superclass::SizeType SizeType;
    typedef typename Superclass::IndexType IndexType;

    /*! FLTK required constructor - must use imData() to complete 
     *  definition */
    GLTwoImageSliceView(int x, int y, int w, int h, const char *l);
    
    /*! Specify the 3D image to view slice by slice */
    virtual void SetInputImage(ImageType * newImData);
    virtual void SetSecondInputImage(ImageType * newImData);

    virtual void clickSelect(float newX, float newY, float newZ);
    virtual void size(int w, int h);
    
    virtual void resize(int x, int y, int w, int h);

    virtual void update();

    virtual void draw();
    
    virtual int handle(int event);
    
    void        iw2Min(float newIWMin);
    float       iw2Min(void);
    void        iw2Max(float newIWMax);
    float       iw2Max(void);

  protected:

    float         cClickSelectR;
    float         cClickSelectG;
    float         cClickSelectB;

    ImagePointer  cIm2Data;
    bool          cValidIm2Data;

    unsigned long cIm2DimSize[3];

    double        cIm2DataMin;
    double        cIm2DataMax;
    float         cIW2Min;
    float         cIW2Max;
  };
  
template <class ImagePixelType, class OverlayPixelType>
GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
GLTwoImageSliceView(int x, int y, int w, int h, const char *l)
:GLSliceView<ImagePixelType, OverlayPixelType>(x, y, w, h, l)
  {
  cClickSelectR = 0;
  cClickSelectG = 0;
  cClickSelectB = 0;
  cIm2Data = NULL;
  cIm2DataMin = 0;
  cIm2DataMax = 0;
  cValidIm2Data = false;
  }
  
//
//
template <class ImagePixelType, class OverlayPixelType>
void GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
SetInputImage(ImageType * newImData)
  {
  GLSliceView<ImagePixelType, OverlayPixelType>::SetInputImage(newImData);

  if(this->cWinImData != NULL)
    {
    delete [] this->cWinImData;
    }
  this->cWinImData = new unsigned char[ this->cWinDataSizeX * this->cWinDataSizeY * 3 ];
  
  this->cValidIm2Data = false;
  }

//
//
template <class ImagePixelType, class OverlayPixelType>
void GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
SetSecondInputImage(ImageType * newImData)
{
  this->cIm2Data = newImData;
  this->cValidIm2Data = true;

  SizeType imSize = this->cIm2Data->GetLargestPossibleRegion().GetSize();
  this->cIm2DimSize[0] = imSize[0];
  this->cIm2DimSize[1] = imSize[1];
  this->cIm2DimSize[2] = imSize[2];

  IndexType ind;
  ind[0] = 0; 
  ind[1] = 0; 
  ind[2] = 0;
  
  this->cIm2DataMax = this->cIm2Data->GetPixel(ind);
  this->cIm2DataMin = this->cIm2DataMax;
  this->cIW2Min     = this->cIm2DataMin;
  this->cIW2Max     = this->cIm2DataMax;
  ImagePixelType tf;
  
  for( unsigned int i=0; i<this->cIm2DimSize[0]; i++ )
    {
    ind[0] = i;
    for(unsigned int j=0; j<this->cIm2DimSize[1]; j++ )
      {
      ind[1] = j;
      for( unsigned int k=0; k<this->cIm2DimSize[2]; k++ )
        {
        ind[2] = k;
        tf = this->cIm2Data->GetPixel(ind);
        if(tf > this->cIm2DataMax) 
          {
          this->cIm2DataMax = tf;
          }
        else 
          {
          if(tf < this->cIm2DataMin)
            {
            this->cIm2DataMin = tf;
            }
          }
        }
      }
    }

  this->cIW2Min     = this->cIm2DataMin;
  this->cIW2Max     = this->cIm2DataMax;
}

//
//
template <class ImagePixelType, class OverlayPixelType>
void GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
clickSelect(float newX, float newY, float newZ)
  {    
  if(!this->cImData)
     {
     return;
     }

  this->cClickSelect[0] = newX;
  if(this->cClickSelect[0]<0) 
    {
    this->cClickSelect[0] = 0;
    }
  if(this->cClickSelect[0] >= this->cDimSize[0])
    {
    this->cClickSelect[0] = this->cDimSize[0]-1;
    }

  this->cClickSelect[1] = newY;
  if(this->cClickSelect[1]<0)
    {
    this->cClickSelect[1] = 0;
    }
  if(this->cClickSelect[1] >= this->cDimSize[1])
    {
    this->cClickSelect[1] = this->cDimSize[1]-1;
    }

  this->cClickSelect[2] = newZ;

  if(this->cClickSelect[2]<0)
    {
    this->cClickSelect[2] = 0;
    }
  if(this->cClickSelect[2] >= this->cDimSize[2])
    {
    this->cClickSelect[2] = this->cDimSize[2]-1;
    }

  IndexType ind;

  ind[0] = (unsigned long)this->cClickSelect[0];
  ind[1] = (unsigned long)this->cClickSelect[1];
  ind[2] = (unsigned long)this->cClickSelect[2];
  this->cClickSelectV = this->cImData->GetPixel(ind);
  this->cClickSelectR = this->cImData->GetPixel(ind);
  itk::Point<double, 3> pnt;
  IndexType ind2;
  this->cImData->TransformIndexToPhysicalPoint(ind, pnt);
  if(this->cIm2Data)
    {
    if( this->cIm2Data->TransformPhysicalPointToIndex(pnt, ind2) )
      {
      this->cClickSelectG = this->cIm2Data->GetPixel(ind2);
      }
    }
      
  /*if length of list is equal to max, remove the earliest point stored */
  if((this->maxClickPoints>0)&&(this->cClickedPoints.size() == this->maxClickPoints))
    {
    this->cClickedPoints.pop_back();
    }
  
  ClickPoint point( this->cClickSelect[0], 
                    this->cClickSelect[1], 
                    this->cClickSelect[2], 
                    this->cClickSelectV     );

  this->cClickedPoints.push_front( point );

  if(this->cClickSelectCallBack != NULL)
    {
      this->cClickSelectCallBack(this->cClickSelect[0], this->cClickSelect[1], 
                           this->cClickSelect[2], this->cClickSelectV);
    }
  if(this->cClickSelectArgCallBack != NULL)
    {
    this->cClickSelectArgCallBack(this->cClickSelect[0], this->cClickSelect[1], 
                              this->cClickSelect[2], this->cClickSelectV,
                              this->cClickSelectArg);
    }

  if(this->cViewValue || this->cViewCrosshairs)
    {
    this->redraw();
    }
  }


template <class ImagePixelType, class OverlayPixelType>
void GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
size(int w, int h)
  {
  GLSliceView<ImagePixelType, OverlayPixelType>::size(w, h);
  }
    
template <class ImagePixelType, class OverlayPixelType>
void GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
resize(int x, int y, int w, int h)
  {
  GLSliceView<ImagePixelType, OverlayPixelType>::resize(x, y, w, h);
  }

//
template <class ImagePixelType, class OverlayPixelType>
void GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
update()
  {
  if( !this->cValidImData ) 
    {
    return;
    }
  
  double zoomBase = this->cW/(this->cDimSize[this->cWinOrder[0]]*(fabs(this->cSpacing[this->cWinOrder[0]])
                        /fabs(this->cSpacing[0])));
  if(zoomBase >
      this->cH/(this->cDimSize[this->cWinOrder[1]]*(fabs(this->cSpacing[this->cWinOrder[1]])
          /fabs(this->cSpacing[0]))))
    {
    zoomBase = this->cH/(this->cDimSize[this->cWinOrder[1]]*(fabs(this->cSpacing[this->cWinOrder[1]])
                   /fabs(this->cSpacing[0])));
    }
  double scale0 = this->cWinZoom * zoomBase * fabs(this->cSpacing[this->cWinOrder[0]])
                                             /fabs(this->cSpacing[0]);
  double scale1 = this->cWinZoom * zoomBase * fabs(this->cSpacing[this->cWinOrder[1]])
                                             /fabs(this->cSpacing[0]);

  if(this->cWinZoom>1)
    {
    this->cWinSizeX = (int)( this->cW / scale0 );
    this->cWinMinX = (int)( (int)this->cWinCenter[ this->cWinOrder[0] ] - this->cWinSizeX/2);
    this->cWinMaxX = (int)( (int)this->cWinCenter[ this->cWinOrder[0] ] + this->cWinSizeX/2);
    }
  else
    {
    this->cWinSizeX = (int)(this->cDimSize[ this->cWinOrder[0] ]);
    this->cWinMinX = 0;
    this->cWinMaxX = (int)( (int)(this->cDimSize[ this->cWinOrder[0] ]) - 1 );
    this->cWinCenter[this->cWinOrder[0]] = (int)( this->cDimSize[ this->cWinOrder[0] ] / 2);
    }
  if( this->cWinMinX <= - (int) this->cDimSize[ this->cWinOrder[0] ] ) 
    {
    this->cWinMinX = -(int)this->cDimSize[ this->cWinOrder[0] ] + 1;
    }
  else if(this->cWinMinX >= (int)this->cDimSize[ this->cWinOrder[0] ]) 
    {
    this->cWinMinX = (int)this->cDimSize[ this->cWinOrder[0] ] - 1;
    }
  if( this->cWinMaxX >= (int)( this->cDimSize[ this->cWinOrder[0] ] ) )
    {
    this->cWinMaxX = (int)this->cDimSize[ this->cWinOrder[0] ] - 1;
    }
  
  if(this->cWinZoom>1)
    {
    this->cWinSizeY = (int)( this->cH / scale1 );
    this->cWinMinY = (int)( (int)(this->cWinCenter[ this->cWinOrder[1] ]) - this->cWinSizeY/2 );
    this->cWinMaxY = (int)( (int)(this->cWinCenter[ this->cWinOrder[1] ]) + this->cWinSizeY/2 );
    }
  else
    {
    this->cWinSizeY = (int)(this->cDimSize[ this->cWinOrder[1] ]);
    this->cWinMinY = 0;
    this->cWinMaxY = (int)( (int)(this->cDimSize[ this->cWinOrder[1] ]) - 1 );
    this->cWinCenter[this->cWinOrder[1]] = (int)( this->cDimSize[ this->cWinOrder[1] ] / 2);
    }
  if( this->cWinMinY <= - (int)( this->cDimSize[ this->cWinOrder[1] ] ) ) 
    {
    this->cWinMinY = -(int)this->cDimSize[ this->cWinOrder[1] ] + 1;
    }
  else if( this->cWinMinY >= (int)(this->cDimSize[ this->cWinOrder[1] ] ) ) 
    {
    this->cWinMinY = this->cDimSize[ this->cWinOrder[1] ] - 1;
    } 
  if( this->cWinMaxY >= (int)( this->cDimSize[ this->cWinOrder[1] ] ) ) 
    {
    this->cWinMaxY = this->cDimSize[ this->cWinOrder[1] ] - 1;
    }

  memset( this->cWinImData, 0, this->cWinDataSizeX*this->cWinDataSizeY*3 );
  if( this->cValidOverlayData ) 
    {
    memset(this->cWinOverlayData, 0, this->cWinDataSizeX*this->cWinDataSizeY*4);
    }
  
  IndexType ind;
  IndexType ind2;
  itk::Point<double, 3> pnt;
  
  int l, m;
  
  float tf[3];
  float tfv[3];
  
  ind[ this->cWinOrder[ 2 ] ] = this->cWinCenter[ this->cWinOrder[ 2 ] ];
  int startK = this->cWinMinY;
  if(startK<0)
    startK = 0;
  int startJ = this->cWinMinX;
  if(startJ<0)
    startJ = 0;
  for(int k=startK; k <= this->cWinMaxY; k++)
    {
    ind[this->cWinOrder[1]] = k;
    
    if(k-this->cWinMinY >= (int)this->cWinDataSizeY)
      continue;

    for(int j=startJ; j <= this->cWinMaxX; j++) 
      {
      ind[this->cWinOrder[0]] = j;
      
      if(j-this->cWinMinX >= (int)this->cWinDataSizeX)
         continue;

      tfv[0] = this->cImData->GetPixel(ind);
      if(this->cValidIm2Data)
        {
        this->cImData->TransformIndexToPhysicalPoint(ind, pnt);
        if(this->cIm2Data->TransformPhysicalPointToIndex(pnt, ind2))
          {
          tfv[1] = (this->cIm2Data->GetPixel(ind2));
          }
        else
          {
          tfv[1] = this->cIm2DataMin;
          }
        }
      else
        {
        tfv[1] = 0;
        }
      tfv[2] = 0;
      switch( this->cImageMode ) 
        {
        default:
        case IMG_VAL:
          tf[0] = (float)(((float)tfv[0]-this->cIWMin)/(this->cIWMax-this->cIWMin)*255);
          tf[1] = (float)(((float)tfv[1]-this->cIW2Min)/(this->cIW2Max-this->cIW2Min)*255);
          break;
        case IMG_INV:
          tf[0] = (float)((this->cIWMax-(float)tfv[0])/(this->cIWMax-this->cIWMin)*255);
          tf[1] = (float)((this->cIWMax-(float)tfv[1])/(this->cIWMax-this->cIWMin)*255);
          break;
        case IMG_LOG:
          tf[0] = (float)(log((float)tfv[0]-this->cIWMin+0.00000001)
                          /log(this->cIWMax-this->cIWMin+0.00000001)*255);
          tf[1] = (float)(log((float)tfv[1]-this->cIWMin+0.00000001)
                          /log(this->cIWMax-this->cIWMin+0.00000001)*255);
          break;
        case IMG_DX:
          if(ind[0]>0) 
            {
            tf[0] = (float)(((float)tfv[0]-this->cIWMin)/(this->cIWMax-this->cIWMin)*255);
            tf[1] = (float)(((float)tfv[1]-this->cIWMin)/(this->cIWMax-this->cIWMin)*255);
            ind[0]--;
            tfv[0] = this->cImData->GetPixel(ind);
            if(this->cValidIm2Data )
              {
              this->cImData->TransformIndexToPhysicalPoint(ind, pnt);
              if( this->cIm2Data->TransformPhysicalPointToIndex(pnt, ind2) )
                {
                tfv[1] = (this->cIm2Data->GetPixel(ind2)-this->cIm2DataMin)
                         / (this->cIm2DataMax-this->cIm2DataMin);
                tfv[1] = tfv[1] * (this->cDataMax-this->cDataMin) + this->cDataMin;
                }
               else
                 {
                 tfv[1] = this->cDataMin;
                 }
              }
            tf[0] -= (float)(((float)tfv[0]-this->cIWMin)/(this->cIWMax-this->cIWMin)*255);
            tf[1] -= (float)(((float)tfv[1]-this->cIWMin)/(this->cIWMax-this->cIWMin)*255);
            ind[0]++;
            tf[0] += 128;
            tf[1] += 128;
            } 
          else
            {
            tf[0] = 128;
            tf[1] = 128;
            }
          break;
        case IMG_DY:
          if(ind[1]>0) 
            {
            tf[0] = (float)(((float)tfv[0]-this->cIWMin)/(this->cIWMax-this->cIWMin)*255);
            tf[1] = (float)(((float)tfv[1]-this->cIWMin)/(this->cIWMax-this->cIWMin)*255);
            ind[1]--;
            tfv[0] = this->cImData->GetPixel(ind);
            if(this->cValidIm2Data)
              {
              this->cImData->TransformIndexToPhysicalPoint(ind, pnt);
              if( this->cIm2Data->TransformPhysicalPointToIndex(pnt, ind2) )
                {
                tfv[1] = (this->cIm2Data->GetPixel(ind2)-this->cIm2DataMin)
                         / (this->cIm2DataMax-this->cIm2DataMin);
                tfv[1] = tfv[1] * (this->cDataMax-this->cDataMin) + this->cDataMin;
                }
               else
                 {
                 tfv[1] = this->cDataMin;
                 }
              }
            tf[0] -= (float)(((float)tfv[0]-this->cIWMin)/(this->cIWMax-this->cIWMin)*255);
            tf[1] -= (float)(((float)tfv[1]-this->cIWMin)/(this->cIWMax-this->cIWMin)*255);
            ind[1]++;
            tf[0] += 128;
            tf[1] += 128;
            }
          else
            {
            tf[0] = 128;
            tf[1] = 128;
            }
          break;
        case IMG_DZ:
          if(ind[2]>0) 
            {
            tf[0] = (float)(((float)tfv[0]-this->cIWMin)/(this->cIWMax-this->cIWMin)*255);
            tf[1] = (float)(((float)tfv[1]-this->cIWMin)/(this->cIWMax-this->cIWMin)*255);
            ind[2]--;
            tfv[0] = this->cImData->GetPixel(ind);
            if(this->cValidIm2Data)
              {
              this->cImData->TransformIndexToPhysicalPoint(ind, pnt);
              if( this->cIm2Data->TransformPhysicalPointToIndex(pnt, ind2) )
                {
                tfv[1] = (cIm2Data->GetPixel(ind2)-this->cIm2DataMin)
                         / (this->cIm2DataMax-this->cIm2DataMin);
                tfv[1] = tfv[1] * (this->cDataMax-this->cDataMin) + this->cDataMin;
                }
              else
                {
                tfv[1] = this->cDataMin;
                }
              }
            tf[0] -= (float)(((float)tfv[0]-this->cIWMin)/(this->cIWMax-this->cIWMin)*255);
            tf[1] -= (float)(((float)tfv[1]-this->cIWMin)/(this->cIWMax-this->cIWMin)*255);
            ind[2]++;
            tf[0] += 128;
            tf[1] += 128;
            }
          else
            {
            tf[0] = 128;
            tf[1] = 128;
            }
          break;
        case IMG_BLEND:
          {
          const int tempval = (int)this->cWinCenter[this->cWinOrder[2]]-1;
          int tmpI = ind[this->cWinOrder[2]];
          ind[this->cWinOrder[2]] = (tempval < 0 ) ? 0 : tempval;
          tfv[0] = this->cImData->GetPixel(ind);
          if(this->cValidIm2Data )
            {
            this->cImData->TransformIndexToPhysicalPoint(ind, pnt);
            if( this->cIm2Data->TransformPhysicalPointToIndex(pnt, ind2) )
              {
              tfv[1] = (this->cIm2Data->GetPixel(ind2)-this->cIm2DataMin)
                       / (this->cIm2DataMax-this->cIm2DataMin);
              tfv[1] = tfv[1] * (this->cDataMax-this->cDataMin) + this->cDataMin;
              }
            else
              {
              tfv[1] = this->cDataMin;
              }
            }
          tf[0] = (float)(tfv[0]);
          tf[1] = (float)(tfv[1]);
          
          ind[this->cWinOrder[2]] = this->cWinCenter[this->cWinOrder[2]];
          tfv[0] = this->cImData->GetPixel(ind);
          if(this->cValidIm2Data)
            {
            this->cImData->TransformIndexToPhysicalPoint(ind, pnt);
            if( this->cIm2Data->TransformPhysicalPointToIndex(pnt, ind2) )
              {
              tfv[1] = (this->cIm2Data->GetPixel(ind2)-this->cIm2DataMin)
                       / (this->cIm2DataMax-this->cIm2DataMin);
              tfv[1] = tfv[1] * (this->cDataMax-this->cDataMin) + this->cDataMin;
              }
            else
              {
              tfv[1] = this->cDataMin;
              }
            }
          tf[0] += (float)(tfv[0])*2;
          tf[1] += (float)(tfv[1])*2;
          
          const int tempval1 = (int)this->cDimSize[this->cWinOrder[2]]-1;
          const int tempval2 = (int)this->cWinCenter[this->cWinOrder[2]]+1;
          ind[this->cWinOrder[2]] = (tempval1 < tempval2 ) ? tempval1 : tempval2;
          tfv[0] = this->cImData->GetPixel(ind);
          if(this->cValidIm2Data )
            {
            this->cImData->TransformIndexToPhysicalPoint(ind, pnt);
            if( this->cIm2Data->TransformPhysicalPointToIndex(pnt, ind2) )
              {
              tfv[1] = (this->cIm2Data->GetPixel(ind2)-this->cIm2DataMin)
                       / (this->cIm2DataMax-this->cIm2DataMin);
              tfv[1] = tfv[1] * (this->cDataMax-this->cDataMin) + this->cDataMin;
              }
            else
              {
              tfv[1] = this->cDataMin;
              }
            }
          tf[0] += (float)(tfv[0]);
          tf[1] += (float)(tfv[1]);
          
          tf[0] = (float)((tf[0]/4-this->cIWMin)/(this->cIWMax-this->cIWMin)*255);
          tf[1] = (float)((tf[1]/4-this->cIWMin)/(this->cIWMax-this->cIWMin)*255);
          ind[this->cWinOrder[2]] = tmpI;
          break;
          }
        case IMG_MIP:
          tf[0] = this->cIWMin;
          tf[1] = this->cIWMin;
          m = (j-this->cWinMinX) + (k-this->cWinMinY)*this->cWinDataSizeX;
          this->cWinZBuffer[m] = 0;
          int tmpI = ind[this->cWinOrder[2]];
          float tfp = 0;
          float tft = tf[0]+tf[1];
          for(l=0; l<(int)this->cDimSize[this->cWinOrder[2]]; l++) 
            {
            ind[this->cWinOrder[2]] = l;        
            tfv[0] = this->cImData->GetPixel(ind);
            if(this->cValidIm2Data )
              {
              this->cImData->TransformIndexToPhysicalPoint(ind, pnt);
              if( this->cIm2Data->TransformPhysicalPointToIndex(pnt, ind2) )
                {
                tfv[1] = (this->cIm2Data->GetPixel(ind2)-this->cIm2DataMin)
                         / (this->cIm2DataMax-this->cIm2DataMin);
                tfv[1] = tfv[1] * (this->cDataMax-this->cDataMin) + this->cDataMin;
                }
              else
                {
                tfv[1] = this->cDataMin;
                }
              }
            tfp = (float)tfv[0] + tfv[1] ;
            if(tfp > tft) 
              {
              tf[0] = (float)(tfv[0]);
              tf[1] = (float)(tfv[1]);
              this->cWinZBuffer[m] = (unsigned short)l;
              }
            }
          tf[0] = (float)((tf[0]-this->cIWMin)/(this->cIWMax-this->cIWMin)*255);
          tf[1] = (float)((tf[1]-this->cIWMin)/(this->cIWMax-this->cIWMin)*255);
          ind[this->cWinOrder[2]] = tmpI;
          break;
        }
      
      int c;
      for(c=0; c<2; c++)
        {
        if( tf[c] > 255 )
          {
          switch(this->cIWModeMax) 
            {
            case IW_MIN:
              tf[c] = 0;
              break;
            default:
            case IW_MAX:
              tf[c] = 255;
              break;
            case IW_FLIP:
              tf[c] = 512-tf[c];
              if(tf[c]<0) 
                {
                tf[c] = 0;
                }
              break;
            }
          }
        else 
          {
          if( tf[c] < 0 )
            {
            switch(this->cIWModeMin) 
              {
              default:
              case IW_MIN:
                tf[c] = 0;
                break;
              case IW_MAX:
                tf[c] = 255;
                break;
              case IW_FLIP:
                tf[c] = -tf[c];
                if(tf[c]>255)
                  {
                  tf[c] = 255;
                  }
                break;
              }
            }
          }
        }
        
      l = (j-this->cWinMinX)*3 + (k-this->cWinMinY)*this->cWinDataSizeX*3;
      this->cWinImData[l+0] = (unsigned char)tf[0];
      this->cWinImData[l+1] = (unsigned char)tf[1];
      this->cWinImData[l+2] = (unsigned char)tf[2];
      
      if( this->cValidOverlayData ) 
        {
        l = (j-this->cWinMinX)*4 + (k-this->cWinMinY)*this->cWinDataSizeX*4;
        if(this->cImageMode == IMG_MIP)
          {
          ind[this->cWinOrder[2]] = this->cWinZBuffer[(j-this->cWinMinX) + 
                              (k-this->cWinMinY)*this->cWinDataSizeX];
          }
        
        if( sizeof( OverlayPixelType ) == 1 )
          {
          m = (int)*((unsigned char *)&(this->cOverlayData->GetPixel(ind)));
          if( m > 0 ) {
            m = m - 1;
            this->cWinOverlayData[l+0] = 
              (unsigned char)(this->cColorTable->GetColorComponent(m, 'r')*255);
            this->cWinOverlayData[l+1] = 
              (unsigned char)(this->cColorTable->GetColorComponent(m, 'g')*255);
            this->cWinOverlayData[l+2] = 
              (unsigned char)(this->cColorTable->GetColorComponent(m, 'b')*255);
            this->cWinOverlayData[l+3] = 
              (unsigned char)(this->cOverlayOpacity*255);
            }
          }
        else 
          {
          if(((unsigned char *)&(this->cOverlayData->GetPixel(ind)))[0]
            + ((unsigned char *)&(this->cOverlayData->GetPixel(ind)))[1]
            + ((unsigned char *)&(this->cOverlayData->GetPixel(ind)))[2] > 0)
            {
            if( sizeof( OverlayPixelType ) == 3 )
              {
              this->cWinOverlayData[l+0] = 
                ((unsigned char *)&(this->cOverlayData->GetPixel(ind)))[0];
              this->cWinOverlayData[l+1] = 
                ((unsigned char *)&(this->cOverlayData->GetPixel(ind)))[1];
              this->cWinOverlayData[l+2] = 
                ((unsigned char *)&(this->cOverlayData->GetPixel(ind)))[2];
              this->cWinOverlayData[l+3] = 
                (unsigned char)(this->cOverlayOpacity*255);
              }
            else 
              {
              if( sizeof( OverlayPixelType ) == 4 ) 
                {
                this->cWinOverlayData[l+0] = 
                  ((unsigned char *)&(this->cOverlayData->GetPixel(ind)))[0];
                this->cWinOverlayData[l+1] = 
                  ((unsigned char *)&(this->cOverlayData->GetPixel(ind)))[1];
                this->cWinOverlayData[l+2] = 
                  ((unsigned char *)&(this->cOverlayData->GetPixel(ind)))[2];
                this->cWinOverlayData[l+3] = 
                  (unsigned char)(((unsigned char *)
                  &(this->cOverlayData->GetPixel(ind)))[3]*this->cOverlayOpacity);
                }
              }
            }
          }
        }
      }
    }
  
  this->redraw();
  
  }

//
//
template <class ImagePixelType, class OverlayPixelType>
void GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
draw()
  {
  if( !this->valid() )
    {
    glClearColor((float)0.0, (float)0.0, (float)0.0, (float)0.0);          
    glShadeModel(GL_FLAT);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  //if you don't include this
    //image size differences distort
    //glPixelStorei(GL_PACK_ALIGNMENT, 1);
    }
  else
    {
    glClear(GL_COLOR_BUFFER_BIT);    //this clears and paints to black
    
    glMatrixMode(GL_MODELVIEW);    //clear previous 3D draw params
    glLoadIdentity();
    
    glMatrixMode(GL_PROJECTION);
    this->ortho();
    
    if( !this->cImData ) 
      {
      return;
      }
    
    double zoomBase = this->cW/(this->cDimSize[this->cWinOrder[0]]*(fabs(this->cSpacing[this->cWinOrder[0]])
                                                  /fabs(this->cSpacing[0])));
    if(zoomBase >
       this->cH/(this->cDimSize[this->cWinOrder[1]]*(fabs(this->cSpacing[this->cWinOrder[1]])
                                   /fabs(this->cSpacing[0]))))
      {
      zoomBase = this->cH/(this->cDimSize[this->cWinOrder[1]]*(fabs(this->cSpacing[this->cWinOrder[1]])
                                             /fabs(this->cSpacing[0])));
      }

    double scale0 = this->cWinZoom * zoomBase * fabs(this->cSpacing[this->cWinOrder[0]])
                             / fabs(this->cSpacing[0]);
    double scale1 = this->cWinZoom * zoomBase * fabs(this->cSpacing[this->cWinOrder[1]]) 
                             / fabs(this->cSpacing[0]);

    int originX = 0;
    int originY = 0;
    if(this->cWinZoom<=1)
      {
      if(this->cW-scale0*this->cDimSize[this->cWinOrder[0]]>0)
        {
        originX = (int)((this->cW-scale0*this->cDimSize[this->cWinOrder[0]])/2.0);
        }
      if(this->cH-scale1*this->cDimSize[this->cWinOrder[1]]>0)
        {
        originY = (int)((this->cH-scale1*this->cDimSize[this->cWinOrder[1]])/2.0);
        }
      }
    glRasterPos2i((this->cFlipX[this->cWinOrientation])?this->cW-originX:originX,
      (this->cFlipY[this->cWinOrientation])?this->cH-originY:originY);  
    glPixelZoom((this->cFlipX[this->cWinOrientation])?-scale0:scale0,
      (this->cFlipY[this->cWinOrientation])?-scale1:scale1);
    
    if( this->cValidImData && this->cViewImData )
      {
      glDrawPixels( this->cWinDataSizeX, this->cWinDataSizeY, 
                    GL_RGB, GL_UNSIGNED_BYTE, 
                    this->cWinImData );
      }
    
    if( this->cValidOverlayData && this->cViewOverlayData ) 
      {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDrawPixels(this->cWinDataSizeX, this->cWinDataSizeY, GL_RGBA, 
        GL_UNSIGNED_BYTE, this->cWinOverlayData);
      glDisable(GL_BLEND);
      }
    
    if( this->cViewAxisLabel ) 
      {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(0.2, 0.2, 0.78, (float)0.75);
      gl_font(FL_TIMES_BOLD, 12);
      
      if( !this->cFlipX[this->cWinOrientation] )
        {
        const int y = static_cast<int>(  this->cH/2-gl_height()/2  );
        gl_draw( this->cAxisLabelX[this->cWinOrientation],
          this->cW-(gl_width(this->cAxisLabelX[this->cWinOrientation])+10), 
          static_cast<float>( y ) );
        }
      else
        {
        const int y = static_cast<int>( this->cH/2-gl_height()/2  );
        gl_draw( this->cAxisLabelX[this->cWinOrientation],
          (gl_width(this->cAxisLabelX[this->cWinOrientation])+10),
          static_cast<float>( y ));
        }
      
      if(!this->cFlipY[this->cWinOrientation])
        {
        const int y = static_cast<int>( this->cH-gl_height()-10 ) ;
        gl_draw( this->cAxisLabelY[this->cWinOrientation],
          this->cW/2-(gl_width(this->cAxisLabelY[this->cWinOrientation])/2),
          static_cast<float>(y) );
        }
      else
        {
        const int y = static_cast<int>( gl_height()+10 );
        gl_draw( this->cAxisLabelY[this->cWinOrientation], 
          this->cW/2-(gl_width(this->cAxisLabelY[this->cWinOrientation])/2),
          static_cast<float>(y));
        }
      
      glDisable(GL_BLEND);
      }
    if( this->cViewValue ) 
      {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(0.1, 0.64, 0.2, (float)0.75);
      gl_font(FL_TIMES_BOLD, 12);
      char s[80];
      float px, py, pz;
      float val = this->cClickSelectV;
      char * suffix = "";
      if( this->cViewValuePhysicalUnits )
        {
        px = this->cOrigin[0]+this->cSpacing[0]*this->cClickSelect[0];
        py = this->cOrigin[1]+this->cSpacing[1]*this->cClickSelect[1];
        pz = this->cOrigin[2]+this->cSpacing[2]*this->cClickSelect[2];
        suffix = this->cPhysicalUnitsName;
        }
       else
        {
        px = this->cClickSelect[0];
        py = this->cClickSelect[1];
        pz = this->cClickSelect[2];
        }
      if((ImagePixelType)1.5==1.5)
        {
        sprintf(s, "(%0.1f%s,  %0.1f%s,  %0.1f%s) = %0.3f", 
                px, suffix,
                py, suffix,
                pz, suffix,
                val);
        }
      else
        {
        sprintf(s, "(%0.1f%s,  %0.1f%s,  %0.1f%s) = %d", 
                px, suffix,
                py, suffix,
                pz, suffix,
                (int)val);
        }
      gl_draw( s,
        (int)(this->cW-(gl_width(s)+2)), 2);
      glDisable(GL_BLEND);
      
      }
    if( this->cViewDetails )
      {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(0.9, 0.4, 0.1, (float)0.75);
      gl_font(FL_TIMES_BOLD, 12);
      char s[80];
      if(this->cWinOrientation == 0)
        sprintf(s, "X - Slice: %3d", this->cWinCenter[0]);
      else if(this->cWinOrientation == 1)
        sprintf(s, "Y - Slice: %3d", this->cWinCenter[1]);
      else
        sprintf(s, "Z - Slice: %3d", this->cWinCenter[2]);
      gl_draw( s, 2, 2+7*(gl_height()+2) );
      sprintf(s, "Dims: %3d x %3d x %3d", 
        (int)this->cDimSize[0], (int)this->cDimSize[1], (int)this->cDimSize[2]);
      gl_draw( s, 2, 2+6*(gl_height()+2) );
      sprintf(s, "Voxel: %0.3f x %0.3f x %0.3f", 
        this->cSpacing[0], this->cSpacing[1], this->cSpacing[2]);
      gl_draw( s, 2, 2+5*(gl_height()+2) );
      sprintf(s, "Int. Range: %0.3f - %0.3f", (float)this->cDataMin, 
              (float)this->cDataMax);
      gl_draw( s, 2, 2+4*(gl_height()+2) );
      sprintf(s, "Int. Range2: %0.3f - %0.3f", (float)this->cIm2DataMin, 
              (float)this->cIm2DataMax);
      gl_draw( s, 2, 2+3*(gl_height()+2) );
       sprintf(s, "Int. Window: %0.3f(%s) - %0.3f(%s)", 
        (float)this->cIWMin, IWModeTypeName[this->cIWModeMin], 
        (float)this->cIWMax, IWModeTypeName[this->cIWModeMax]);
      gl_draw( s, 2, 2+2*(gl_height()+2) );
      sprintf(s, "Int. Window2: %0.3f(%s) - %0.3f(%s)", 
        (float)this->cIW2Min, IWModeTypeName[this->cIWModeMin], 
        (float)this->cIW2Max, IWModeTypeName[this->cIWModeMax]);
       gl_draw( s, 2, 2+1*(gl_height()+2) );
      sprintf(s, "View Mode: %s", ImageModeTypeName[this->cImageMode]);
      gl_draw( s, 2, 2+0*(gl_height()+2) );
      glDisable(GL_BLEND);
      }
    if( this->cViewCrosshairs 
      && static_cast<int>(this->cClickSelect[this->cWinOrder[2]]) == 
         static_cast<int>( this->sliceNum() ) )
      {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(0.1, 0.64, 0.2, (float)0.75);
      int x;
      if(this->cFlipX[this->cWinOrientation])
        {
        x = (int)(this->cW - (this->cClickSelect[this->cWinOrder[0]] - this->cWinMinX) * scale0 
                     - originX);
        }
      else
        {
        x = (int)((this->cClickSelect[this->cWinOrder[0]] - this->cWinMinX) * scale0 + originX);
        }
      int y;
      if(this->cFlipY[this->cWinOrientation])
        {
        y = (int)(this->cH - (this->cClickSelect[this->cWinOrder[1]] - this->cWinMinY) * scale1
                     - originY);
        }
      else
        {
        y = (int)((this->cClickSelect[this->cWinOrder[1]] - this->cWinMinY) * scale1 + originY);
        }
      glBegin(GL_LINES);
      glVertex2d(0, y);
      glVertex2d(x-2, y);
      glVertex2d(x+2, y);
      glVertex2d(this->cW-1, y);
      glVertex2d(x, 0);
      glVertex2d(x, y-2);
      glVertex2d(x, y+2);
      glVertex2d(x, this->cH-1);
      glEnd();
      glDisable(GL_BLEND);
      }
    
    }
  }

template <class ImagePixelType, class OverlayPixelType>
int GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
handle(int event)
{
  int key;

  switch(event)
    {
    case FL_SHORTCUT:
      {  

      int eventState = Fl::event_state();
      key = Fl::event_text()[0];
      
      // Alt + 'a' etc.. window levels the second dataset independent of the first
      // Alt + 'r' resets second dataset independent of the fist..
      // 
      if( eventState & FL_ALT )
        {
        switch(key)
          {
          case 'a':
            iw2Min(cIW2Min-(float)0.02*(cIm2DataMax-cIm2DataMin));
            this->update();
            return 1;
          case 'q':
            iw2Max(cIW2Max-(float)0.02*(cIm2DataMax-cIm2DataMin));
            this->update();
            return 1;
          case 'w':
            {
            iw2Max(cIW2Max+(float)0.02*(cIm2DataMax-cIm2DataMin));
            this->update();
            return 1;
            }
          case 's':
            {
            iw2Min(cIW2Min+(float)0.02*(cIm2DataMax-cIm2DataMin));
            this->update();
            return 1;
            }
          case 'r':
            {
            this->winZoom(1);
            this->winCenter();
            this->imageMode(IMG_VAL);
            iw2Max(cIm2DataMax);
            iw2Min(cIm2DataMin);
            this->update();
            return 1;
            }
          case 'h':
            {
            std::string a(" \
  SliceViewer\n \
  ===========\n \
  \
  This is a multi-dimensional image viewer.  It displays one 2D slice or \n \
  a MIP view of that data. A variety of viewing options exist...\n \
  \n \
  Options: (press a key in the window)\n \
  \n \
    0 - View slices along the x-axis\n \
    1 - View slices along the y-axis\n \
    2 - View slices along the z-axis (default)\n \
    \n \
    < , - View the next slice\n \
    < , - View the previous slice\n \
    \n \
    r      - reset all options, (W/L settings reset only for the first dataset)\n \
    Alt +r - reset all options, (W/L settings reset only for the second dataset)\n \
    \n");
    std::string b(" \
        h - help (this document)\n \
    \n \
    x - Flip the x-axis\n \
    y - Flip the y-axis\n \
    z - Flip the z-axis\n \
    \n \
    q w       - Decrease, Increase upper limit of intensity window for first dataset\n \
    Alt + q w - Decrease, Increase upper limit of intensity window for second dataset\n \
    e - Toggle between clipping and setting-to-black values above IW upper limit\n \
    \n \
    a s       - Decrease, Increase lower limit of intensity window for first dataset\n \
    Alt + a s - Decrease, Increase lower limit of intensity window for second dataset\n \
    d - Toggle between clipping and setting-to-white values below IW lower limit\n \
    \n \
    + = - Zoom-in by a factor of 2 \n \
    - _ - Zoom-out by a factor of 2 \n \
    \n \
    i j k m - Shift the image in the corresponding direction\n \
    \n \
    t - Transpose the axis of the slice being viewed\n \
    \n \
    A - View axis labels: P=posterior, L=left, S=superior\n \
    C - View crosshairs that illustrate last user-click in the window\n \
    I - View image values as the user clicks in the window\n \
    T - Toggle display of clicked points\n \
    P - Toggle coordinates display between index and physical units\n \
    D - View image details as an overlay on the image\n \
    O - View a color overlay (application dependent)\n \
    \n \
    p - Save the clicked points in a file \n \
    \n \
    l - Toggle how the data is the window is viewed:\n \
          Modes cycle between the following views: \n \
          Intensity\n \
          Inverse\n \
          Log\n \
          Derivative wrt x\n \
          Derivative wrt y\n \
          Derivative wrt z\n \
          Blend with previous and next slice\n \
          MIP\n ");
            a += b;
            ifuShowText(a.c_str());
            return 1;
            }
          default:
            break;
          }
        }
      }

    default:
      break;
    }

  return GLSliceView<ImagePixelType, OverlayPixelType>::handle(event);
}


template <class ImagePixelType, class OverlayPixelType>
void GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
iw2Min(float newIWMin)
{
  cIW2Min = newIWMin;
}
 
template <class ImagePixelType, class OverlayPixelType>
void GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
iw2Max(float newIWMax)
{
  cIW2Max = newIWMax;
}

template <class ImagePixelType, class OverlayPixelType>
float GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
iw2Max(void)
{
  return cIW2Max;
}

template <class ImagePixelType, class OverlayPixelType>
float GLTwoImageSliceView<ImagePixelType, OverlayPixelType>::
iw2Min(void)
{
  return cIW2Min;
}



}; //namespace
#endif

