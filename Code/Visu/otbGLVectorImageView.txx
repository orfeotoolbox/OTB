#ifndef otbGLVectorImageView_txx
#define otbGLVectorImageView_txx

#include "otbGLVectorImageView.h"
#include "itkVariableLengthVector.h"

#include <math.h>

namespace otb
{

template <class TPixel, class OverlayPixelType>
GLVectorImageView<TPixel, OverlayPixelType>::
//GLImageView(int x, int y, int w, int h, const char *l):
//VectorImageView<TPixel>(x, y, w, h, l), Fl_Gl_Window(x, y, w, h, l)
GLVectorImageView() : GLVectorImageViewBase<TPixel, OverlayPixelType>()
  {
        m_IdentWindowView = PRINCIPAL_IMAGE_VIEW;
        m_ViewRectangle = true;
  }

template <class TPixel, class OverlayPixelType>
GLVectorImageView<TPixel, OverlayPixelType>::
~GLVectorImageView()
{

}
 
template <class TPixel, class OverlayPixelType>
void
GLVectorImageView<TPixel, OverlayPixelType>::
Init(int x, int y, int w, int h, const char * l)
{
        //Methode sur Fl_Gl_Window
        this->resize(x,y,w,h);
        //Et le "l" !!! (THOMAS)
}

//
// Set the input image to be displayed
// Warning: the current overlay is destroyed if the size of the image
// is different from the size of the overlay.
//
// Initialise la class a partir du "m_ViewImageRegion" préalablement initialisé
template <class TPixel, class OverlayPixelType>
void 
GLVectorImageView<TPixel, OverlayPixelType>::
FinaliseInitialisation(void)
{
/*        this->m_ViewImageRegion = this->GetInput()->GetRequestedRegion();
  
        //  RegionType region = this->GetInput()->GetLargestPossibleRegion();
        if( this->m_ViewImageRegion.GetNumberOfPixels() == 0 ) 
        {
                return;
        }
*/
        m_CenterPointImage = GetCenterRegion(this->m_ViewImageRegion);


//THOMAS calcul quand meme sur toute l'image
//  SizeType   imSize   = this->m_ViewImageRegion.GetSize();
//  IndexType  imIndex  = this->m_ViewImageRegion.GetIndex();
  SizeType   imSize   = this->GetInput()->GetRequestedRegion().GetSize();
  IndexType  imIndex  = this->GetInput()->GetRequestedRegion().GetIndex();

//  this->cImData = newImData;
//  this->SetInput( newImData );
  //Determine les canaux
//  this->GenerateChannelsInformations();

  this->cDimSize[0]=imSize[0];
  this->cDimSize[1]=imSize[1];


//  this->cDimSize[2]=imSize[2];
  this->cSpacing[0]=this->GetInput()->GetSpacing()[0];
  this->cSpacing[1]=this->GetInput()->GetSpacing()[1];
//  this->cSpacing[2]=this->GetInput()->GetSpacing()[2];
  this->cOrigin[0]=this->GetInput()->GetOrigin()[0];
  this->cOrigin[1]=this->GetInput()->GetOrigin()[1];
//  this->cOrigin[2]=this->GetInput()->GetOrigin()[2];
    

  this->CalculeDataMinMax(      this->m_ViewImageRegion,  
                                this->cDataMin, 
                                this->cDataMax);

  this->cIWMin      = this->cDataMin;
  this->cIWMax      = this->cDataMax;
  this->cIWModeMin  = IW_MIN;
  this->cIWModeMax  = IW_MAX;
  
  this->cImageMode = IMG_VAL;
  
//  this->winZoom(1);
  
/*  this->cWinOrientation = 2;
  this->cWinOrder[0] = 0;
  1 = 1;
  this->cWinOrder[2] = 2;
*/  
  this->cWinCenter[0] = this->cDimSize[0]/2;
  this->cWinCenter[1] = this->cDimSize[1]/2;
//  this->cWinCenter[2] = 0;
  
  this->cWinMinX  = 0;
  this->cWinSizeX = this->cDimSize[0];
  if(this->cWinSizeX<this->cDimSize[1])
    {
    this->cWinSizeX = this->cDimSize[1];
    }
/*  if(this->cWinSizeX<this->cDimSize[2])
    {
    this->cWinSizeX = this->cDimSize[2];
    }*/
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
  this->cWinImData = new unsigned char[ this->cWinDataSizeX * this->cWinDataSizeY * this->m_NbDim ];

  this->cViewImData  = true;
  this->cValidImData = true;
  
}


template <class TPixel, class OverlayPixelType>
typename GLVectorImageView<TPixel, OverlayPixelType>::IndexType
GLVectorImageView<TPixel, OverlayPixelType>::
WindowCoord2ImageCoord( const IndexType & index )const
{
        IndexType lCoord;
        RegionType lRegion = this->GetViewImageRegion();
        for( int i=0 ; i < 2 ; i++)
        {
                lCoord[i] = (unsigned long)(index[i]/this->cWinZoom) + lRegion.GetIndex()[i];
//                lCoord[i] = (unsigned long)(index[i]/this->cWinZoom);
        }
        return lCoord;
} 
template <class TPixel, class OverlayPixelType>
typename GLVectorImageView<TPixel, OverlayPixelType>::SizeType
GLVectorImageView<TPixel, OverlayPixelType>::
WindowSize2ImageSize( const SizeType & size )const
{
        SizeType lSize;
        for( int i=0 ; i < 2 ; i++)
        {
                lSize[i] = (unsigned long)(size[i]/this->cWinZoom);
        }
        return lSize;
} 

template <class TPixel, class OverlayPixelType>
typename GLVectorImageView<TPixel, OverlayPixelType>::RegionType
GLVectorImageView<TPixel, OverlayPixelType>::
WindowZone2ImageRegion( const RegionType & zone )const
{
        RegionType lRegion;
        SizeType  lSize  = this->WindowSize2ImageSize(zone.GetSize());
        IndexType lIndex = this->WindowCoord2ImageCoord(zone.GetIndex());
        lRegion.SetIndex( lIndex );
        lRegion.SetSize( lSize );
        return lRegion;
} 


template <class TPixel, class OverlayPixelType>
typename GLVectorImageView<TPixel, OverlayPixelType>::IndexType
GLVectorImageView<TPixel, OverlayPixelType>::
GetCenterRegion( const RegionType & zone )
{
        IndexType lCenter;
        lCenter[0] = zone.GetIndex()[0] + zone.GetSize()[0] / 2; 
        lCenter[1] = zone.GetIndex()[1] + zone.GetSize()[1] / 2; 
        return lCenter;
} 

template <class TPixel, class OverlayPixelType>
typename GLVectorImageView<TPixel, OverlayPixelType>::RegionType
GLVectorImageView<TPixel, OverlayPixelType>::
ShrinkRegion( const RegionType & zone, const float shrinkfactor )
{
        RegionType lRegion;
        SizeType  lSize;
        IndexType lIndex;
        for( int i=0 ; i<2 ; i++)
        {
                lSize[i]  = (int) (shrinkfactor*zone.GetSize()[i]);
                lIndex[i] = (int) (shrinkfactor*zone.GetIndex()[i]);
        }
        lRegion.SetIndex( lIndex );
        lRegion.SetSize( lSize );
        return (lRegion);
}


template <class TPixel, class OverlayPixelType>
typename GLVectorImageView<TPixel, OverlayPixelType>::RegionType
GLVectorImageView<TPixel, OverlayPixelType>::
GetViewRegion( const IndexType & centerPointImage )
{
        return( GetViewRegion( this->GetInput()->GetRequestedRegion(), centerPointImage) );
#if 0
        RegionType lNewViewRegion;
/*        if(this->cWinZoom>1)
        {*/
                //Region de l'Image actuellement visualisee
                RegionType lImageRegionComplete = this->GetInput()->GetRequestedRegion();
                SizeType lSize;
                IndexType lIndex = centerPointImage;
                double scale0 = this->cWinZoom;
                double scale1 = this->cWinZoom;
                lSize[0] = (int)( this->cW / scale0 );
                lSize[1] = (int)( this->cH / scale1 );

                //Reajuste le centre si la zone zoom est sur les bords
                for( int i=0 ; i < 2 ; i++)
                {
                        lIndex[i] = centerPointImage[i] - lSize[i]/2;
                        if ( lIndex[i] < lImageRegionComplete.GetIndex()[i] ) 
                        {
                                lIndex[i] = lImageRegionComplete.GetIndex()[i];
                        }
                        else if( lIndex[i] + lSize[i] > lImageRegionComplete.GetIndex()[i] + lImageRegionComplete.GetSize()[i] )
                        {
                                lIndex[i] = lImageRegionComplete.GetIndex()[i] + lImageRegionComplete.GetSize()[i] - lSize[i];
                        }
                        //Si la fenetre est plus grande en pixel) que l'image reelle, on réajuste la taille de l'image
                        if( lSize[i] >  lImageRegionComplete.GetSize()[i] )
                        {
                                lSize[i] = lImageRegionComplete.GetSize()[i];
                        }
/*                        if( centerPointImage[i] + lSize[i]/2 > lViewRegion.GetIndex()[i] + lViewRegion.GetSize()[i] )
                        {
                                lIndex[i] = lViewRegion.GetIndex()[i] + lViewRegion.GetSize()[i] - lSize[i]/2;
                        }
                        else if ( centerPointImage[i] - lSize[i]/2 < lViewRegion.GetIndex()[i] ) 
                        {
                                lIndex[i]  = lViewRegion.GetIndex()[i] + lSize[i]/2;
                        }*/
                }
                lNewViewRegion.SetIndex( lIndex );
                lNewViewRegion.SetSize(  lSize );
/*        }
        else
        {
                lNewViewRegion = this->GetViewImageRegion();
        }*/
        return(lNewViewRegion);
#endif
}


template <class TPixel, class OverlayPixelType>
typename GLVectorImageView<TPixel, OverlayPixelType>::RegionType
GLVectorImageView<TPixel, OverlayPixelType>::
GetViewRegion( const RegionType & region, const IndexType & centerPointImage )
{
        RegionType lNewViewRegion;
        RegionType lImageRegionComplete = region;
        SizeType lSize;
        IndexType lIndex;
        double scale0 = this->cWinZoom;
        double scale1 = this->cWinZoom;
        lSize[0] = (int)( this->cW / scale0 );
        lSize[1] = (int)( this->cH / scale1 );
//std::cout <<"-------------------------------------"<<std::endl;
//std::cout <<"ImageEntree = "<<region<<std::endl;
//std::cout <<"centerPointImage = "<<centerPointImage<<std::endl;

        //Reajuste le centre si la zone zoom est sur les bords
        for( int i=0 ; i < 2 ; i++)
        {
                lIndex[i] = centerPointImage[i] - lSize[i]/2;
                if ( lIndex[i] < lImageRegionComplete.GetIndex()[i] ) 
                {
                        lIndex[i] = lImageRegionComplete.GetIndex()[i];
                }
                else if( lIndex[i] + lSize[i] > lImageRegionComplete.GetIndex()[i] + lImageRegionComplete.GetSize()[i] )
                {
                        lIndex[i] = lImageRegionComplete.GetIndex()[i] + lImageRegionComplete.GetSize()[i] - lSize[i];
                }
                //Si la fenetre est plus grande en pixel) que l'image reelle, on réajuste la taille de l'image
                if( lSize[i] >  lImageRegionComplete.GetSize()[i] )
                {
                        lSize[i] = lImageRegionComplete.GetSize()[i];
                }
        }
        lNewViewRegion.SetIndex( lIndex );
        lNewViewRegion.SetSize(  lSize );
//std::cout <<"lNewViewRegion = "<<lNewViewRegion<<std::endl;
        return(lNewViewRegion);
}
template <class TPixel, class OverlayPixelType>
typename GLVectorImageView<TPixel, OverlayPixelType>::RegionType
GLVectorImageView<TPixel, OverlayPixelType>::
TranslateImageRegionToWindowRegion( const RegionType & imageRegion, const  RegionType & sousimageRegion, const int windowWidth, const int windowHeight)
{
        RegionType lRegion;
        SizeType   lSize;
        IndexType  lIndex ;
        for( int i=0 ; i < 2 ; i++)
        {
                lIndex[i] = sousimageRegion.GetIndex()[i] - imageRegion.GetIndex()[i];
                lSize[i] = sousimageRegion.GetSize()[i];
        }
        lRegion.SetIndex( lIndex );
        lRegion.SetSize(  lSize );
        return(lRegion);
}

template <class TPixel, class OverlayPixelType>
void
GLVectorImageView<TPixel, OverlayPixelType>::
MajViewRegion(void)
{
        //Position du centre de l'image en coordonnees image
        this->SetViewImageRegion( GetViewRegion(m_CenterPointImage) );
//        m_CenterPointImage      = GetCenterRegion(this->m_ViewImageRegion);
}

template <class TPixel, class OverlayPixelType>
void
GLVectorImageView<TPixel, OverlayPixelType>::
BuildWithImageRegion()
{
        this->SetViewImageRegion( this->GetInput()->GetRequestedRegion() );
//        m_CenterPointImage      = GetCenterRegion(this->m_ViewImageRegion);
//        this->winZoom((float)(1.));
        this->cWinZoom = 1.;
        //Retaille la fenetre
//        this->size( this->GetViewImageRegion().GetSize()[0], this->GetViewImageRegion().GetSize()[1] );
        //Termine l'initialisation 
        FinaliseInitialisation();
}

template <class TPixel, class OverlayPixelType>
void
GLVectorImageView<TPixel, OverlayPixelType>::
BuildWithWindowRegion(const int zoom)
{
        RegionType lRegion;
        RegionType lWindow;
        SizeType lSize;
        IndexType lIndex;
        
        lSize[0] = this->cW;
        lSize[1] = this->cH;
        lWindow.SetSize(lSize);
        for( int i=0 ; i < 2 ; i++)
        {
                lSize[i]  =   lWindow.GetSize()[i] / zoom;
                lIndex[i] =   this->GetInput()->GetRequestedRegion().GetIndex()[i]
                            + this->GetInput()->GetRequestedRegion().GetSize()[i] / 2 
                            - lSize[i] / 2;
        }
        lRegion.SetIndex(lIndex);
        lRegion.SetSize(lSize);
        this->m_ViewImageRegion = lRegion;
//        m_CenterPointImage      = GetCenterRegion(lRegion);
//        this->winZoom((float)zoom);
        this->cWinZoom = (float)zoom;

        //Termine l'initialisation 
        FinaliseInitialisation();
}

template <class TPixel, class OverlayPixelType>
void
GLVectorImageView<TPixel, OverlayPixelType>::
PrintInfos(void)
{
std::cout << this->GetNameOfClass()<< std::endl;
std::cout << "- cW, cH               :   ["<<this->cW<<","<<this->cH<<"]"<< std::endl;
std::cout << "- zoom                 :   "<<this->winZoom()<< std::endl;
std::cout << "- zoom                 :   "<<this->cWinZoom<< std::endl;
std::cout << "- m_CenterPointImage   :   "<<m_CenterPointImage<< std::endl;
std::cout << "- m_ViewImageRegion    :   "<<this->m_ViewImageRegion<< std::endl;
}

//
//
template <class TPixel, class OverlayPixelType>
void 
GLVectorImageView<TPixel, OverlayPixelType>::
update()
{
        if( this->cValidImData == false ) 
        {
                return;
        }

//THOMAS
        this->MajViewRegion();

//        this->PrintInfos();

        //Update d'ITK
        this->Update();


        memset( this->cWinImData, 0, this->cWinDataSizeX*this->cWinDataSizeY*this->m_NbDim );
        if( this->cValidOverlayData==true ) 
        {
                memset(this->cWinOverlayData, 0, this->cWinDataSizeX*this->cWinDataSizeY*4);
        }
    
        //Initialisation de la region à visualiser, determinee en fonction du Zoom
//        this->m_ViewImageRegion = this->InitViewRegion();
  
        //Lecture des pixels et initialisation du buffer
        this->SetWinImData( this->m_ViewImageRegion );

        this->redraw();
  
  
}


template <class TPixel, class OverlayPixelType>
void 
GLVectorImageView<TPixel, OverlayPixelType>::
ViewClickedPoints( bool newViewClickedPoints )
{
    this->cViewClickedPoints = newViewClickedPoints;

    this->redraw();
}

template <class TPixel, class OverlayPixelType>
bool
GLVectorImageView<TPixel, OverlayPixelType>::
ViewClickedPoints()
{
    return this->cViewClickedPoints;
}




//
//
//
template <class TPixel, class OverlayPixelType>
typename GLVectorImageView<TPixel, OverlayPixelType>::ColorTablePointer 
GLVectorImageView<TPixel, OverlayPixelType>::
GetColorTable(void)
  {
  return this->cColorTable;
  }



//
//
//
template <class TPixel, class OverlayPixelType>
void
GLVectorImageView<TPixel, OverlayPixelType>::
SetColorTable(typename 
              GLVectorImageView<TPixel, OverlayPixelType>::ColorTablePointer 
              newColorTable)
  {
  this->cColorTable = newColorTable;
  }




template <class TPixel, class OverlayPixelType>
void GLVectorImageView<TPixel, OverlayPixelType>::
clickSelect(float x, float y/*, float z*/)
  {
  VectorImageView<TPixel>::clickSelect(x, y/*, z*/);
  if(this->cViewValue || this->cViewCrosshairs)
    {
    this->redraw();
    }
  }

template <class TPixel, class OverlayPixelType>
void GLVectorImageView<TPixel, OverlayPixelType>::size(int w, int h)
  {
  VectorImageView<TPixel>::size(w, h);
  Fl_Gl_Window::size(w, h);
  this->update();
  this->redraw();
  }




template <class TPixel, class OverlayPixelType>
void 
GLVectorImageView<TPixel, OverlayPixelType>::
resize(int x, int y, int w, int h)
  {
  VectorImageView<TPixel>::resize(x, y, w, h);
  Fl_Gl_Window::resize(x, y, w, h);
  this->update();
  this->redraw();
  }




//
//
template <class TPixel, class OverlayPixelType>
void GLVectorImageView<TPixel, OverlayPixelType>::draw(void)
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
    
    if( !this->GetInput() ) 
      {
      return;
      }
    
    double scale0 = this->cWinZoom;
    double scale1 = this->cWinZoom;
    
    int originX = 0;
    int originY = 0;
//THOMAS
    if( this->GetViewImageRegion().GetSize()[0]*scale0 < this->cW )
    {
        originX = static_cast<int>( (this->cW - this->GetViewImageRegion().GetSize()[0]*scale0)/2);
    }
    if( this->GetViewImageRegion().GetSize()[1]*scale1 < this->cH )
    {
        originY = static_cast<int>( (this->cH - this->GetViewImageRegion().GetSize()[1]*scale1)/2);
    }

    glRasterPos2i(      originX ,  this->cH - originY );  
    glPixelZoom(        scale0 , - scale1 );
    
    if( (this->cValidImData==true) && (this->cViewImData == true) )
      {
                if ( this->m_ModeView == Superclass::GRAY_LEVEL )
                { 
                        glDrawPixels(           this->cWinDataSizeX, 
                                                this->cWinDataSizeY, 
                                                GL_LUMINANCE,
                                                GL_UNSIGNED_BYTE, 
                                                this->cWinImData );
                }
                else
                { 
                        glDrawPixels(           this->cWinDataSizeX,
                                                this->cWinDataSizeY, 
                                                GL_RGB,
                                                GL_UNSIGNED_BYTE, 
                                                this->cWinImData );
                }
      }
    
    if( (this->cValidOverlayData==true) && (this->cViewOverlayData==true) ) 
      {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDrawPixels(this->cWinDataSizeX, this->cWinDataSizeY, GL_RGBA, 
        GL_UNSIGNED_BYTE, this->cWinOverlayData);
      glDisable(GL_BLEND);
      }


    if( m_ViewRectangle == true ) 
      {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glColor4f(1., 0., 0., (float)0.75);
                gl_font(FL_TIMES_BOLD, 12);
                RegionType lDrawRectangle = TranslateImageRegionToWindowRegion( this->GetViewImageRegion(), m_DrawViewRectangle, this->cW, this->cH);
//std::cout <<" Draw rectangle : "<<lDrawRectangle<<std::endl;
                
                gl_rect(        lDrawRectangle.GetIndex()[0], 
                                this->cH - lDrawRectangle.GetIndex()[1] - lDrawRectangle.GetSize() [1], 
                                lDrawRectangle.GetSize() [0], 
                                lDrawRectangle.GetSize() [1]);
                glDisable(GL_BLEND);
      }

   }    
}


}; //namespace
#endif

