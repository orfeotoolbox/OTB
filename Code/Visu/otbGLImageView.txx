#ifndef otbGLImageView_txx
#define otbGLImageView_txx

#include "otbGLImageView.h"

#include <math.h>

namespace otb
{

template <class ImagePixelType, class OverlayPixelType>
GLImageView<ImagePixelType, OverlayPixelType>::
//GLImageView(int x, int y, int w, int h, const char *l):
//ImageView<ImagePixelType>(x, y, w, h, l), Fl_Gl_Window(x, y, w, h, l)
GLImageView() : ImageView<ImagePixelType>(), Fl_Gl_Window(0,0,0,0,0)
  {
  when(FL_WHEN_NOT_CHANGED | FL_WHEN_ENTER_KEY);
  cValidOverlayData     = false;
  this->cViewOverlayData      = false;
  cViewOverlayCallBack  = NULL;
  cOverlayOpacity       = 0.0;
  cWinOverlayData       = NULL;
  cColorTable = ColorTableType::New();
  //cColorTable.useGray();
  cColorTable->useDiscrete();
  cOverlayColorIndex = 7;  //default white
  }

template <class ImagePixelType, class OverlayPixelType>
GLImageView<ImagePixelType, OverlayPixelType>::
~GLImageView()
{

}
 
template <class ImagePixelType, class OverlayPixelType>
void
GLImageView<ImagePixelType, OverlayPixelType>::
Init(int x, int y, int w, int h, const char * l)
{
        //Methode sur Fl_Gl_Window
        this->resize(x,y,w,h);
        //Et le "l" !!! (THOMAS)
}
  
template <class ImagePixelType, class OverlayPixelType>
void
GLImageView<ImagePixelType, OverlayPixelType>::
SetInput( const ImageType * image )
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0, 
                                   const_cast< ImageType *>( image ) );
}


//
// Set the input image to be displayed
// Warning: the current overlay is destroyed if the size of the image
// is different from the size of the overlay.
//
template <class ImagePixelType, class OverlayPixelType>
void 
GLImageView<ImagePixelType, OverlayPixelType>::
SetInputImage(ImageType * newImData)
  {
  RegionType region = newImData->GetLargestPossibleRegion();
  if( region.GetNumberOfPixels() == 0 ) 
    {
    return;
    }

  SizeType   imSize   = region.GetSize();

  // If the overlay has been set and the size is different from the new image,
  // it is removed.
  if( cValidOverlayData)
    {  
    SizeType overlay_size = cOverlayData->GetLargestPossibleRegion().GetSize();
    
    if((overlay_size[0] != imSize[0])
      ||  (overlay_size[1] != imSize[1])
      ||  (overlay_size[2] != imSize[2]))
      {
       if(cWinOverlayData != NULL)
         {
         delete [] cWinOverlayData;
         }
       cWinOverlayData       = NULL;
       cValidOverlayData     = false;
      }       
    }

//  this->cImData = newImData;
  this->SetInput( newImData );
  this->cDimSize[0]=imSize[0];
  this->cDimSize[1]=imSize[1];
  this->cDimSize[2]=imSize[2];
  this->cSpacing[0]=this->GetInput()->GetSpacing()[0];
  this->cSpacing[1]=this->GetInput()->GetSpacing()[1];
  this->cSpacing[2]=this->GetInput()->GetSpacing()[2];
  this->cOrigin[0]=this->GetInput()->GetOrigin()[0];
  this->cOrigin[1]=this->GetInput()->GetOrigin()[1];
  this->cOrigin[2]=this->GetInput()->GetOrigin()[2];
    
  //calculating cDataMax and cDataMin    
  IndexType ind;
  ind[0] = 0; 
  ind[1] = 0; 
  ind[2] = 0;
  
  this->cDataMax = this->GetInput()->GetPixel(ind);
  this->cDataMin = this->cDataMax;
  ImagePixelType tf;
  
  
  for( unsigned int i=0; i<this->cDimSize[0]; i++ )
    {
    ind[0] = i;
    for(unsigned int j=0; j<this->cDimSize[1]; j++ )
      {
      ind[1] = j;
      for( unsigned int k=0; k<this->cDimSize[2]; k++ )
        {
        ind[2] = k;
        tf = this->GetInput()->GetPixel(ind);
        if(tf > this->cDataMax) 
          {
          this->cDataMax = tf;
          }
        else 
          {
          if(tf < this->cDataMin)
            {
            this->cDataMin = tf;
            }
          }
        }
      }
    }
  
  this->cIWMin      = this->cDataMin;
  this->cIWMax      = this->cDataMax;
  this->cIWModeMin  = IW_MIN;
  this->cIWModeMax  = IW_MAX;
  
  this->cImageMode = IMG_VAL;
  
  this->cWinZoom = 1;
  
  this->cWinOrientation = 2;
  this->cWinOrder[0] = 0;
  this->cWinOrder[1] = 1;
  this->cWinOrder[2] = 2;
  
  this->cWinCenter[0] = this->cDimSize[0]/2;
  this->cWinCenter[1] = this->cDimSize[1]/2;
  this->cWinCenter[2] = 0;
  
  this->cWinMinX  = 0;
  this->cWinSizeX = this->cDimSize[0];
  if(this->cWinSizeX<this->cDimSize[1])
    {
    this->cWinSizeX = this->cDimSize[1];
    }
  if(this->cWinSizeX<this->cDimSize[2])
    {
    this->cWinSizeX = this->cDimSize[2];
    }
  this->cWinMaxX  = this->cWinSizeX - 1;
  
  this->cWinMinY  = 0;
  this->cWinSizeY = this->cWinSizeX;
  this->cWinMaxY  = this->cWinSizeY - 1;
  
  this->cWinDataSizeX = this->cWinMaxX;//this->cDimSize[0];
  this->cWinDataSizeY = this->cWinMaxY;//this->cDimSize[1];
  
  if(this->cWinImData != NULL)
    {
    delete [] this->cWinImData;
    }
  this->cWinImData = new unsigned char[ this->cWinDataSizeX * this->cWinDataSizeY ];

  if(this->cWinZBuffer != NULL) 
    {
    delete [] this->cWinZBuffer;
    }
  this->cWinZBuffer = new unsigned short[ this->cWinDataSizeX * this->cWinDataSizeY ];
  
  this->cViewImData  = true;
  this->cValidImData = true;
  
  }




template <class ImagePixelType, class OverlayPixelType>
typename GLImageView<ImagePixelType, OverlayPixelType>::ImageType * 
GLImageView<ImagePixelType, OverlayPixelType>::
GetInput(void)
  {
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return ( static_cast< ImageType * >
     (this->itk::ProcessObject::GetInput(0) ) ) ;
//  return (ImagePointer)cImData;
  }


/*
//
//
template <class ImagePixelType, class OverlayPixelType>
const typename itk::Image<ImagePixelType,3>::Pointer &
GLImageView<ImagePixelType, OverlayPixelType>
::GetInputImage(void) const
  {
  return (*this->GetInput());
  }

*/

//
//
template <class ImagePixelType, class OverlayPixelType>
void 
GLImageView<ImagePixelType, OverlayPixelType>
::SetInputOverlay( OverlayType * newOverlayData )
  {
  RegionType newoverlay_region = 
    newOverlayData->GetLargestPossibleRegion();

  SizeType   newoverlay_size   = newoverlay_region.GetSize();

  if( this->cValidImData 
      &&  (newoverlay_size[0] == this->cDimSize[0])
      &&  (newoverlay_size[1] == this->cDimSize[1])
      &&  (newoverlay_size[2] == this->cDimSize[2])
    )
    {
    this->cOverlayData = newOverlayData;
    
    this->cViewOverlayData  = true;
    this->cValidOverlayData = true;
    this->cOverlayOpacity   = (float)1.0;
    
    if(this->cWinOverlayData != NULL) 
      {
      delete [] this->cWinOverlayData;
      }
    

    const unsigned long bufferSize = this->cWinDataSizeX * this->cWinDataSizeY * 4;
    this->cWinOverlayData = new unsigned char[ bufferSize ];
    }
  else // return a warning
    {
      if(!this->cValidImData)
        {
        std::cout << "GLImageView: Please set the input image before overlay"  
                  << std::endl;
        std::cout << "GLImageView: Overlay not set." << std::endl;
        }
      else if((newoverlay_size[0] != this->cDimSize[0])
      ||  (newoverlay_size[1] != this->cDimSize[1])
      ||  (newoverlay_size[2] != this->cDimSize[2])
      )
        {
        std::cout << "GLImageView: Error: overlay and input images should be the same size" 
                  << std::endl;
        std::cout << "GLImageView: Overlay not set." << std::endl;
        }
      
    }
  }





template <class ImagePixelType, class OverlayPixelType>
const typename GLImageView<ImagePixelType, 
OverlayPixelType>::OverlayPointer &
GLImageView<ImagePixelType, OverlayPixelType>::GetInputOverlay( void ) 
const
  {
  return this->cOverlayData;
  }




//
//
template <class ImagePixelType, class OverlayPixelType>
void 
GLImageView<ImagePixelType, OverlayPixelType>::
ViewOverlayData( bool newViewOverlayData)
  {
  
  this->cViewOverlayData = newViewOverlayData;
  
  if( this->cViewOverlayCallBack != NULL )
    {
    cViewOverlayCallBack();
    }
  
  this->redraw();
  
  }

template <class ImagePixelType, class OverlayPixelType>
bool 
GLImageView<ImagePixelType, OverlayPixelType>::
ViewOverlayData(void)
  {
  
  return this->cViewOverlayData;
  
  }


template <class ImagePixelType, class OverlayPixelType>
void 
GLImageView<ImagePixelType, OverlayPixelType>::
ViewOverlayCallBack( void (* newViewOverlayCallBack)(void) )
  {
  this->cViewOverlayCallBack = newViewOverlayCallBack;
  }


template <class ImagePixelType, class OverlayPixelType>
void 
GLImageView<ImagePixelType, OverlayPixelType>::
ViewClickedPoints( bool newViewClickedPoints )
{
    this->cViewClickedPoints = newViewClickedPoints;

    this->redraw();
}

template <class ImagePixelType, class OverlayPixelType>
bool
GLImageView<ImagePixelType, OverlayPixelType>::
ViewClickedPoints()
{
    return this->cViewClickedPoints;
}


//
//
template <class ImagePixelType, class OverlayPixelType>
void 
GLImageView<ImagePixelType, OverlayPixelType>::
OverlayOpacity( float newOverlayOpacity )
  {
  this->cOverlayOpacity = newOverlayOpacity;
  
  if(this->cViewOverlayCallBack != NULL) 
    {
    this->cViewOverlayCallBack();
    }
  }




template <class ImagePixelType, class OverlayPixelType>
float 
GLImageView<ImagePixelType, OverlayPixelType>::
OverlayOpacity(void)
  {
  return this->cOverlayOpacity;
  }




//
//
//
template <class ImagePixelType, class OverlayPixelType>
typename GLImageView<ImagePixelType, OverlayPixelType>::ColorTablePointer 
GLImageView<ImagePixelType, OverlayPixelType>::
GetColorTable(void)
  {
  return this->cColorTable;
  }



//
//
//
template <class ImagePixelType, class OverlayPixelType>
void
GLImageView<ImagePixelType, OverlayPixelType>::
SetColorTable(typename 
              GLImageView<ImagePixelType, OverlayPixelType>::ColorTablePointer 
              newColorTable)
  {
  cColorTable = newColorTable;
  }




//
//
template <class ImagePixelType, class OverlayPixelType>
void 
GLImageView<ImagePixelType, OverlayPixelType>::
update()
  {
  
  if( !this->cValidImData ) 
    {
    return;
    }

  double zoomBase = this->cW / (this->cDimSize[this->cWinOrder[0]]
                                * (fabs(this->cSpacing[this->cWinOrder[0]])
                                   /fabs(this->cSpacing[0])));
  if(zoomBase > this->cH / (this->cDimSize[this->cWinOrder[1]]
                            * (fabs(this->cSpacing[this->cWinOrder[1]])
                              / fabs(this->cSpacing[0]))))
    {
    zoomBase = this->cH / (this->cDimSize[this->cWinOrder[1]]
                           * (fabs(this->cSpacing[this->cWinOrder[1]])
                              / fabs(this->cSpacing[0])));
    }
  double scale0 = this->cWinZoom * zoomBase 
                                 * fabs(this->cSpacing[this->cWinOrder[0]])
                                 / fabs(this->cSpacing[0]);
  double scale1 = this->cWinZoom * zoomBase 
                                 * fabs(this->cSpacing[this->cWinOrder[1]]) 
                                 / fabs(this->cSpacing[0]);

  if(this->cWinZoom>1)
    {
    this->cWinSizeX = (int)( this->cW / scale0 );
    this->cWinMinX = (int)( (int)this->cWinCenter[ this->cWinOrder[0] ] 
                            - this->cWinSizeX/2 );
    this->cWinMaxX = (int)( (int)this->cWinCenter[ this->cWinOrder[0] ] 
                            + this->cWinSizeX/2 );
    }
  else
    {
    this->cWinSizeX = (int)(this->cDimSize[ this->cWinOrder[0] ]);
    this->cWinMinX = 0;
    this->cWinMaxX = (int)( (int)(this->cDimSize[ this->cWinOrder[0] ]) - 1 );
    this->cWinCenter[this->cWinOrder[0]] = 
                     (int)( this->cDimSize[ this->cWinOrder[0] ] / 2);
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
    this->cWinMinY = (int)( (int)(this->cWinCenter[ this->cWinOrder[1] ]) 
                             - this->cWinSizeY/2 );
    this->cWinMaxY = (int)( (int)(this->cWinCenter[ this->cWinOrder[1] ]) 
                             + this->cWinSizeY/2 );
    }
  else
    {
    this->cWinSizeY = (int)(this->cDimSize[ this->cWinOrder[1] ]);
    this->cWinMinY = 0;
    this->cWinMaxY = (int)( (int)(this->cDimSize[ this->cWinOrder[1] ]) - 1 );
    this->cWinCenter[this->cWinOrder[1]] = 
                     (int)( this->cDimSize[ this->cWinOrder[1] ] / 2);
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
  
  memset( this->cWinImData, 0, this->cWinDataSizeX*this->cWinDataSizeY );
  if( this->cValidOverlayData ) 
    {
    memset(this->cWinOverlayData, 0, this->cWinDataSizeX*this->cWinDataSizeY*4);
    }
  
  IndexType ind;
  
  int l, m;
  
  float tf;
  
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

      switch( this->cImageMode ) 
        {
        default:
        case IMG_VAL:
          tf = (float)((this->GetInput()->GetPixel(ind)-this->cIWMin) 
                       / (this->cIWMax-this->cIWMin)*255);
          break;
        case IMG_INV:
          tf = (float)((this->cIWMax-this->GetInput()->GetPixel(ind)) 
                       / (this->cIWMax-this->cIWMin)*255);
          break;
        case IMG_LOG:
          tf = (float)(log(this->GetInput()->GetPixel(ind)-this->cIWMin+0.00000001)
                       /log(this->cIWMax-this->cIWMin+0.00000001)*255);
          break;
        case IMG_DX:
          if(ind[0]>0) 
            {
            tf = (float)((this->GetInput()->GetPixel(ind)-this->cIWMin)
                         / (this->cIWMax-this->cIWMin)*255);
            ind[0]--;
            tf -= (float)((this->GetInput()->GetPixel(ind)-this->cIWMin) 
                         / (this->cIWMax-this->cIWMin)*255);
            ind[0]++;
            tf += 128;
            } 
          else
            {
            tf = 128;
            }
          break;
        case IMG_DY:
          if(ind[1]>0) 
            {
            tf = (float)((this->GetInput()->GetPixel(ind)-this->cIWMin) 
                         / (this->cIWMax-this->cIWMin)*255);
            ind[1]--;
            tf -= (float)((this->GetInput()->GetPixel(ind)-this->cIWMin) 
                          / (this->cIWMax-this->cIWMin)*255);
            ind[1]++;
            tf += 128;
            }
          else
            {
            tf = 128;
            }
          break;
        case IMG_DZ:
          if(ind[2]>0) 
            {
            tf = (float)((this->GetInput()->GetPixel(ind)-this->cIWMin)
                         / (this->cIWMax-this->cIWMin)*255);
            ind[2]--;
            tf -= (float)((this->GetInput()->GetPixel(ind)-this->cIWMin)
                          / (this->cIWMax-this->cIWMin)*255);
            ind[2]++;
            tf += 128;
            }
          else
            {
            tf = 128;
            }
          break;
        case IMG_BLEND:
          {
          const int tempval = (int)this->cWinCenter[this->cWinOrder[2]]-1;
          int tmpI = ind[this->cWinOrder[2]];
          ind[this->cWinOrder[2]] = (tempval < 0 ) ? 0 : tempval;
          tf = (float)(this->GetInput()->GetPixel(ind));
          
          ind[this->cWinOrder[2]] = this->cWinCenter[this->cWinOrder[2]];
          tf += (float)(this->GetInput()->GetPixel(ind))*2;
          
          const int tempval1 = (int)this->cDimSize[this->cWinOrder[2]]-1;
          const int tempval2 = (int)this->cWinCenter[this->cWinOrder[2]]+1;
          ind[this->cWinOrder[2]] = (tempval1 < tempval2 ) ? tempval1 : tempval2;
          tf += (float)(this->GetInput()->GetPixel(ind));
          
          tf = (float)((tf/4-this->cIWMin)/(this->cIWMax-this->cIWMin)*255);
          ind[this->cWinOrder[2]] = tmpI;
          break;
          }
        case IMG_MIP:
          tf = this->cIWMin;
          m = (j-this->cWinMinX) + (k-this->cWinMinY)*this->cWinDataSizeX;
          this->cWinZBuffer[m] = 0;
          int tmpI = ind[this->cWinOrder[2]];
          for(l=0; l<(int)this->cDimSize[this->cWinOrder[2]]; l++) 
            {
            ind[this->cWinOrder[2]] = l;        
            if(this->GetInput()->GetPixel(ind) > tf) 
              {
              tf = (float)(this->GetInput()->GetPixel(ind));
              this->cWinZBuffer[m] = (unsigned short)l;
              }
            }
          tf = (float)((tf-this->cIWMin)/(this->cIWMax-this->cIWMin)*255);
          ind[this->cWinOrder[2]] = tmpI;
          break;
        }
      
      if( tf > 255 )
        {
        switch(this->cIWModeMax) 
          {
          case IW_MIN:
            tf = 0;
            break;
          default:
          case IW_MAX:
            tf = 255;
            break;
          case IW_FLIP:
            tf = 512-tf;
            if(tf<0) 
              {
              tf = 0;
              }
            break;
          }
        }
      else 
        {
        if( tf < 0 )
          {
          switch(this->cIWModeMin) 
            {
            default:
            case IW_MIN:
              tf = 0;
              break;
            case IW_MAX:
              tf = 255;
              break;
            case IW_FLIP:
              tf = -tf;
              if(tf>255)
                {
                tf = 255;
                }
              break;
            }
          }
        }
      
      l = (j-this->cWinMinX) + (k-this->cWinMinY)*this->cWinDataSizeX;
      this->cWinImData[l] = (unsigned char)tf;
      unsigned int overlayColorIndex = 0;
      
      if( this->cValidOverlayData ) 
        {
        l = l * 4;
        if(this->cImageMode == IMG_MIP)
          {
          ind[this->cWinOrder[2]] = this->cWinZBuffer[(j-this->cWinMinX) + 
            (k-this->cWinMinY)*this->cWinDataSizeX];
          }
        
        if( sizeof( OverlayPixelType ) == 1  ||
            sizeof( OverlayPixelType ) == 2      )
          {
          if (sizeof( OverlayPixelType ) == 1)
            {
            m = (int)*((unsigned char *)&(cOverlayData->GetPixel(ind)));
            }
          else
            {
            m = (int)*((unsigned short *)&(cOverlayData->GetPixel(ind)));
            }
          if( m >= (int)cColorTable->GetNumberOfColors() ) 
            { 
            m = cColorTable->GetNumberOfColors() - 1;
            }
          if( m > 0 ) {
            overlayColorIndex = m-1;
            if( static_cast<unsigned int>(m) > cOverlayColorIndex )
              {
              overlayColorIndex = cOverlayColorIndex;
              }
            cWinOverlayData[l+0] = 
              (unsigned char)(cColorTable->GetColorComponent(overlayColorIndex,
                                                             'r') * 255);
            cWinOverlayData[l+1] = 
              (unsigned char)(cColorTable->GetColorComponent(overlayColorIndex,
                                                             'g') * 255);
            cWinOverlayData[l+2] = 
              (unsigned char)(cColorTable->GetColorComponent(overlayColorIndex,
                                                             'b') * 255);
            cWinOverlayData[l+3] = 
              (unsigned char)(cOverlayOpacity*255);
            }
          }
        else 
          {
          if(((unsigned char *)&(cOverlayData->GetPixel(ind)))[0]
            + ((unsigned char *)&(cOverlayData->GetPixel(ind)))[1]
            + ((unsigned char *)&(cOverlayData->GetPixel(ind)))[2] > 0)
            {
            if( sizeof( OverlayPixelType ) == 3 )
              {
              cWinOverlayData[l+0] = 
                ((unsigned char *)&(cOverlayData->GetPixel(ind)))[0];
              cWinOverlayData[l+1] = 
                ((unsigned char *)&(cOverlayData->GetPixel(ind)))[1];
              cWinOverlayData[l+2] = 
                ((unsigned char *)&(cOverlayData->GetPixel(ind)))[2];
              cWinOverlayData[l+3] = 
                (unsigned char)(cOverlayOpacity*255);
              }
            else 
              {
              if( sizeof( OverlayPixelType ) == 4 ) 
                {
                cWinOverlayData[l+0] = 
                  ((unsigned char *)&(cOverlayData->GetPixel(ind)))[0];
                cWinOverlayData[l+1] = 
                  ((unsigned char *)&(cOverlayData->GetPixel(ind)))[1];
                cWinOverlayData[l+2] = 
                  ((unsigned char *)&(cOverlayData->GetPixel(ind)))[2];
                cWinOverlayData[l+3] = 
                  (unsigned char)(((unsigned char *)
                  &(cOverlayData->GetPixel(ind)))[3]*cOverlayOpacity);
                }
              }
            }
          }
        }
    }
  }
  
  this->redraw();
  
  
}




template <class ImagePixelType, class OverlayPixelType>
void GLImageView<ImagePixelType, OverlayPixelType>::
clickSelect(float x, float y, float z)
  {
  ImageView<ImagePixelType>::clickSelect(x, y, z);
  if(this->cViewValue || this->cViewCrosshairs)
    {
    this->redraw();
    }
  }

template <class ImagePixelType, class OverlayPixelType>
void GLImageView<ImagePixelType, OverlayPixelType>::size(int w, int h)
  {
  ImageView<ImagePixelType>::size(w, h);
  Fl_Gl_Window::size(w, h);
  this->update();
  this->redraw();
  }




template <class ImagePixelType, class OverlayPixelType>
void 
GLImageView<ImagePixelType, OverlayPixelType>::
resize(int x, int y, int w, int h)
  {
  ImageView<ImagePixelType>::resize(x, y, w, h);
  Fl_Gl_Window::resize(x, y, w, h);
  this->update();
  this->redraw();
  }




//
//
template <class ImagePixelType, class OverlayPixelType>
void GLImageView<ImagePixelType, OverlayPixelType>::draw(void)
  {
  if( !valid() )
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
    ortho();
    
    if( !this->GetInput() ) 
      {
      return;
      }
    
    double zoomBase = this->cW / (this->cDimSize[this->cWinOrder[0]]
                                  * (fabs(this->cSpacing[this->cWinOrder[0]])
                                     / fabs(this->cSpacing[0])));
    if(zoomBase > this->cH / (this->cDimSize[this->cWinOrder[1]]
                              * (fabs(this->cSpacing[this->cWinOrder[1]])
                                 / fabs(this->cSpacing[0]))))
      {
      zoomBase = this->cH / (this->cDimSize[this->cWinOrder[1]]
                             * (fabs(this->cSpacing[this->cWinOrder[1]])
                                / fabs(this->cSpacing[0])));
      }

    double scale0 = this->cWinZoom * zoomBase 
                                   * fabs(this->cSpacing[this->cWinOrder[0]])
                                   / fabs(this->cSpacing[0]);
    double scale1 = this->cWinZoom * zoomBase 
                                   * fabs(this->cSpacing[this->cWinOrder[1]])
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
                    GL_LUMINANCE, GL_UNSIGNED_BYTE, 
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

    if( this->cViewClickedPoints )
    {
        glColor3f( 0.8, 0.4, 0.4 );
        glPointSize( 3.0 );
        glBegin(GL_POINTS);
        {
            for ( int i = 0; i < this->numClickedPointsStored(); i++ )
            {
                ClickPoint p;
                this->getClickedPoint( i, p );
                float pts[3] = { p.x, p.y, p.z };

                if ( static_cast<int>( pts[this->cWinOrder[2]] ) ==
                     (int)this->sliceNum() )
                {
                    float x;
                    if(this->cFlipX[this->cWinOrientation])
                    {
                        x = this->cW - (pts[this->cWinOrder[0]] 
                                        - this->cWinMinX) * scale0
                            - originX;
                    }
                    else
                    {
                        x = (pts[this->cWinOrder[0]] - this->cWinMinX) * scale0
                            + originX;
                    }

                    float y;
                    if(this->cFlipY[this->cWinOrientation])
                    {
                        y = this->cH - (pts[this->cWinOrder[1]] 
                                        - this->cWinMinY) * scale1
                            - originY;
                    }
                    else
                    {
                        y = (pts[this->cWinOrder[1]] - this->cWinMinY) * scale1
                             + originY;
                    }
                    glVertex2f( x, y );
                }
            }
        }
        glEnd();
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
      float px, py, pz, val = this->cClickSelectV;
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
      gl_draw( s, 2, 2+5*(gl_height()+2) );
      sprintf(s, "Dims: %3d x %3d x %3d", 
        (int)this->cDimSize[0], (int)this->cDimSize[1], (int)this->cDimSize[2]);
      gl_draw( s, 2, 2+4*(gl_height()+2) );
      sprintf(s, "Voxel: %0.3f x %0.3f x %0.3f", 
        this->cSpacing[0], this->cSpacing[1], this->cSpacing[2]);
      gl_draw( s, 2, 2+3*(gl_height()+2) );
      sprintf(s, "Int. Range: %0.3f - %0.3f", (float)this->cDataMin, 
              (float)this->cDataMax);
      gl_draw( s, 2, 2+2*(gl_height()+2) );
      sprintf(s, "Int. Window: %0.3f(%s) - %0.3f(%s)", 
        (float)this->cIWMin, IWModeTypeName[this->cIWModeMin], 
        (float)this->cIWMax, IWModeTypeName[this->cIWModeMax]);
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
        x = (int)(this->cW - (this->cClickSelect[this->cWinOrder[0]] 
                           - this->cWinMinX) * scale0 - originX);
        }
      else
        {
        x = (int)((this->cClickSelect[this->cWinOrder[0]] 
                   - this->cWinMinX) * scale0 + originX);
        }
      int y;
      if(this->cFlipY[this->cWinOrientation])
        {
        y = (int)(this->cH - (this->cClickSelect[this->cWinOrder[1]] 
                           - this->cWinMinY) * scale1 - originY);
        }
      else
        {
        y = (int)((this->cClickSelect[this->cWinOrder[1]] 
                   - this->cWinMinY) * scale1 + originY);
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





//
//
template <class ImagePixelType, class OverlayPixelType>
int GLImageView<ImagePixelType, OverlayPixelType>::handle(int event)
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
              make_current();
              fl_overlay_clear();
              fl_overlay_rect(boxX, boxY, x-boxY, y-boxY);
              }
            else
              {
              make_current();
              fl_overlay_clear();
              }
            }
          }
        }
      break;
    default:
      break;
    }
  
  return ImageView<ImagePixelType>::handle(event);
  }

}; //namespace
#endif

