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
#ifndef otbGLVectorImageViewClick_txx
#define otbGLVectorImageViewClick_txx


#include "itkExceptionObject.h"
#include "itkImageRegionConstIterator.h"

#include "otbGLVectorImageViewClick.h"
#include "otbImageFileReader.h"
#include "otbMacro.h"

#include <math.h>

namespace otb
{

template <class TPixel, class TPixelOverlay>
GLVectorImageViewClick<TPixel,TPixelOverlay>::
GLVectorImageViewClick() : VectorImageView<TPixel>(), Fl_Gl_Window(0,0,0,0,0)
{
  when(FL_WHEN_NOT_CHANGED | FL_WHEN_ENTER_KEY);
  this->ClearSelectChannels();
  m_ViewRectangle = true;
  m_Label = "";
  cOverlay = false ;
  cValidOverlayData     = false;
  cViewOverlayData      = false;
  cViewOverlayCallBack  = NULL;
  cOverlayOpacity       = 0.0;
  cWinOverlayData       = NULL;
  cOverlayColorIndex = 7;  //default white
	
  cClickMode = CM_SELECT;
  cClickSelect[0] = 0;
  cClickSelect[1] = 0;
  cClickSelect[2] = 0;
  cClickSelectV = 0;
  cClickSelectCallBack = NULL;
  cClickSelectArg = NULL;
  cClickSelectArgCallBack = NULL;
  cClickBoxCallBack = NULL;
  cClickBoxArg = NULL;
  cClickBoxArgCallBack = NULL;
  this->cViewClickedPoints = false;
  maxClickPoints = 0;
  
  cColorTable = ColorTableType::New();
  //cColorTable->useDiscrete();

  
}

template <class TPixel, class TPixelOverlay>
GLVectorImageViewClick<TPixel,TPixelOverlay>::
~GLVectorImageViewClick()
{

}


template <class TPixel, class TPixelOverlay>
void
GLVectorImageViewClick<TPixel,TPixelOverlay>::
Init(int x, int y, int w, int h, const char * l)
{
        //Methode sur Fl_Gl_Window
        this->resize(x,y,w,h);
        //Et le "l" !!! (THOMAS)
}

//
// Set the input image to be displayed
//
// Initialise la class a partir du "m_ViewImageRegion" préalablement initialisé
template <class TPixel, class TPixelOverlay>
void 
GLVectorImageViewClick<TPixel,TPixelOverlay>::
FinalizeInitialisation(void)
{
  m_CenterPointImage = GetCenterRegion(this->m_ViewImageRegion);

  SizeType   imSize   = this->GetInput()->GetRequestedRegion().GetSize();

  this->cDimSize[0]=imSize[0];
  this->cDimSize[1]=imSize[1];

  this->CalculeDataMinMax(this->m_ViewImageRegion,this->cDataMin,this->cDataMax);

  this->cIWMin      = this->cDataMin;
  this->cIWMax      = this->cDataMax;

  this->cWinDataSizeX = imSize[0];
  if( this->cWinDataSizeX < imSize[1] )
  {
        this->cWinDataSizeX = imSize[1];
  }
  this->cWinDataSizeY = this->cWinDataSizeX;
  
  if(this->cWinImData != NULL)
    {
    delete [] this->cWinImData;
    }
  this->cWinImData = new unsigned char[ this->cWinDataSizeX * this->cWinDataSizeY * this->m_NbDim ];

  this->cViewImData  = true;
  this->cValidImData = true;
  
}


template <class TPixel, class TPixelOverlay>
typename GLVectorImageViewClick<TPixel,TPixelOverlay>::IndexType
GLVectorImageViewClick<TPixel,TPixelOverlay>::
WindowCoord2ImageCoord( const IndexType & index )const
{
        IndexType lCoord;
        RegionType lRegion = this->GetViewImageRegion();
        for( int i=0 ; i < 2 ; i++)
        {
                lCoord[i] = (unsigned long)(index[i]/this->cWinZoom) + lRegion.GetIndex()[i];
        }
        return lCoord;
} 
template <class TPixel, class TPixelOverlay>
typename GLVectorImageViewClick<TPixel,TPixelOverlay>::SizeType
GLVectorImageViewClick<TPixel,TPixelOverlay>::
WindowSize2ImageSize( const SizeType & size )const
{
        SizeType lSize;
        for( int i=0 ; i < 2 ; i++)
        {
                lSize[i] = (unsigned long)(size[i]/this->cWinZoom);
        }
        return lSize;
} 

template <class TPixel, class TPixelOverlay>
typename GLVectorImageViewClick<TPixel,TPixelOverlay>::RegionType
GLVectorImageViewClick<TPixel,TPixelOverlay>::
WindowZone2ImageRegion( const RegionType & zone )const
{
        RegionType lRegion;
        SizeType  lSize  = this->WindowSize2ImageSize(zone.GetSize());
        IndexType lIndex = this->WindowCoord2ImageCoord(zone.GetIndex());
        lRegion.SetIndex( lIndex );
        lRegion.SetSize( lSize );
        return lRegion;
} 


template <class TPixel, class TPixelOverlay>
typename GLVectorImageViewClick<TPixel,TPixelOverlay>::IndexType
GLVectorImageViewClick<TPixel,TPixelOverlay>::
GetCenterRegion( const RegionType & zone )
{
        IndexType lCenter;
        lCenter[0] = zone.GetIndex()[0] + zone.GetSize()[0] / 2; 
        lCenter[1] = zone.GetIndex()[1] + zone.GetSize()[1] / 2; 
        return lCenter;
} 

template <class TPixel, class TPixelOverlay>
typename GLVectorImageViewClick<TPixel,TPixelOverlay>::RegionType
GLVectorImageViewClick<TPixel,TPixelOverlay>::
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


template <class TPixel, class TPixelOverlay>
typename GLVectorImageViewClick<TPixel,TPixelOverlay>::RegionType
GLVectorImageViewClick<TPixel,TPixelOverlay>::
GetViewRegion( const IndexType & centerPointImage )
{
        return( GetViewRegion( this->GetInput()->GetRequestedRegion(), centerPointImage) );
}


template <class TPixel, class TPixelOverlay>
typename GLVectorImageViewClick<TPixel,TPixelOverlay>::RegionType
GLVectorImageViewClick<TPixel,TPixelOverlay>::
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
        return(lNewViewRegion);
}
template <class TPixel, class TPixelOverlay>
typename GLVectorImageViewClick<TPixel,TPixelOverlay>::RegionType
GLVectorImageViewClick<TPixel,TPixelOverlay>::
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

template <class TPixel, class TPixelOverlay>
void
GLVectorImageViewClick<TPixel,TPixelOverlay>::
MajViewRegion(void)
{
        //Position du centre de l'image en coordonnees image
        this->SetViewImageRegion( GetViewRegion(m_CenterPointImage) );
}

template <class TPixel, class TPixelOverlay>
void
GLVectorImageViewClick<TPixel,TPixelOverlay>::
BuildWithImageRegion()
{
        this->SetViewImageRegion( this->GetInput()->GetRequestedRegion() );
        this->cWinZoom = 1.;
        //Termine l'initialisation 
        FinalizeInitialisation();
		
		
	//PrintInfos();
}

template <class TPixel, class TPixelOverlay>
void
GLVectorImageViewClick<TPixel,TPixelOverlay>::
BuildWithWindowRegion(const int zoom)
{
        //otbMsgDevMacro( << "BUILDWITHWINDOWREGION" ) ;
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
        this->cWinZoom = (float)zoom;

        //Termine l'initialisation 
        FinalizeInitialisation();
}

template <class TPixel, class TPixelOverlay>
void
GLVectorImageViewClick<TPixel,TPixelOverlay>::
PrintInfos(void)
{
otbMsgDevMacro(  << this->GetNameOfClass() );
otbMsgDevMacro(  << "- cW, cH               :  ["<<this->cW<<","<<this->cH<<"]" );
otbMsgDevMacro(  << "- zoom                 :   "<<this->winZoom() );
otbMsgDevMacro(  << "- zoom                 :   "<<this->cWinZoom );
otbMsgDevMacro(  << "- m_CenterPointImage   :   "<<m_CenterPointImage);
otbMsgDevMacro(  << "- m_ViewImageRegion    :   "<<this->m_ViewImageRegion);
}

//
//
template <class TPixel, class TPixelOverlay>
void 
GLVectorImageViewClick<TPixel,TPixelOverlay>::
update()
{
    if( this->cValidImData == false ) 
    {
        return;
    }

    //Mise a jour de la fenetre 
    //this->MajViewRegion();

    //Update d'ITK
    this->Update();
		
    memset( this->cWinImData, 0, this->cWinDataSizeX*this->cWinDataSizeY*this->m_NbDim );
    
    //Lecture des pixels et initialisation du buffer
    this->SetWinImData( this->m_ViewImageRegion );

	//Mise à jour de l'overlay
	if (cOverlay)
	{
	double zoomBase = (double)this->cW / (this->cDimSize[0]);
    if(zoomBase > (double)this->cH / (this->cDimSize[1]))
    {
    	zoomBase = (double)this->cH / (this->cDimSize[1]);
    }

	double scale0 = zoomBase;
	double scale1 = zoomBase;
  
    this->cWinSizeX = (int)(this->cDimSize[0]);
	this->cWinMinX = 0;
    this->cWinMaxX = (int)( (int)(this->cDimSize[0]) - 1 );
	this->cWinCenter[0] = (int)( this->cDimSize[0] / 2);

    if( this->cWinMinX <= - (int) this->cDimSize[0] ) 
    {
    	this->cWinMinX = -(int)this->cDimSize[0] + 1;
    }
  	else if(this->cWinMinX >= (int)this->cDimSize[0]) 
   	{
    	this->cWinMinX = (int)this->cDimSize[0] - 1;
    }
  	if( this->cWinMaxX >= (int)( this->cDimSize[0] ) )
 	{
	    this->cWinMaxX = (int)this->cDimSize[0] - 1;
    }
    this->cWinSizeY = (int)(this->cDimSize[1]);
    this->cWinMinY = 0;
    this->cWinMaxY = (int)( (int)(this->cDimSize[1]) - 1 );
    this->cWinCenter[1] = (int)( this->cDimSize[1] / 2);
    
  	if( this->cWinMinY <= - (int)( this->cDimSize[1] ) ) 
    {
    	this->cWinMinY = -(int)this->cDimSize[1] + 1;
    }
  	else if( this->cWinMinY >= (int)(this->cDimSize[1] ) ) 
    {
    	this->cWinMinY = this->cDimSize[1] - 1;
    } 
  	if( this->cWinMaxY >= (int)( this->cDimSize[1] ) ) 
    {
    	this->cWinMaxY = this->cDimSize[1] - 1;
    }
  
 	//memset( this->cWinImData, 0, this->cWinDataSizeX*this->cWinDataSizeY );
  	if( this->cValidOverlayData ) 
    {
    	memset(this->cWinOverlayData, 0, this->cWinDataSizeX*this->cWinDataSizeY*4);
    }

  	IndexType ind;
  
  	int l, m;
  
  	//float tf;
  
 	int startK = this->cWinMinY;
  	if(startK<0)
    	startK = 0;
	int startJ = this->cWinMinX;
  	if(startJ<0)
    	startJ = 0;
		
	l=0;
		
  	for(int k=startK; k <= this->cWinMaxY; k++)
    {
    	ind[1] = k;
    
    	//if(k-this->cWinMinY >= (int)this->cWinDataSizeY)
    	//  continue;
	
    	for(int j=startJ; j <= this->cWinMaxX; j++) 
      	{
      		ind[0] = j;
      
	  		m = (int)*((unsigned char *)&(cOverlayData->GetPixel(ind)));
			cWinOverlayData[l+0] = 
               		((unsigned char *)&(cOverlayData->GetPixel(ind)))[0];
            cWinOverlayData[l+1] = 
              		((unsigned char *)&(cOverlayData->GetPixel(ind)))[1];
            cWinOverlayData[l+2] = 
              	    ((unsigned char *)&(cOverlayData->GetPixel(ind)))[2];
	  		if (cWinOverlayData[l+0]==255)
				cWinOverlayData[l+3] = (unsigned char)(0);
			else
				cWinOverlayData[l+3] = (unsigned char)(255);
				
	  		l+=4;
      		//if(j-this->cWinMinX >= (int)this->cWinDataSizeX)
         	//	continue;

            
       }
  }
  }

  this->redraw();
}

template <class TPixel, class TPixelOverlay>
void GLVectorImageViewClick<TPixel,TPixelOverlay>::size(int w, int h)
  {
  VectorImageView<TPixel>::size(w, h);
  Fl_Gl_Window::size(w, h);
  this->update();
  this->redraw();
  }


template <class TPixel, class TPixelOverlay>
void 
GLVectorImageViewClick<TPixel,TPixelOverlay>::
resize(int x, int y, int w, int h)
  {
  VectorImageView<TPixel>::resize(x, y, w, h); 						
  Fl_Gl_Window::resize(x, y, w, h);
  this->update();
  this->redraw();
  }

template <class TPixel, class TPixelOverlay>
void GLVectorImageViewClick<TPixel,TPixelOverlay>::draw(void)
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
    
    double zoomBase = (double)this->cW / (this->cDimSize[0]);
    if(zoomBase > (double)this->cH / (this->cDimSize[1]))
    {
    	zoomBase = (double)this->cH / (this->cDimSize[1]);
    }
 
    double scale0 = zoomBase;
    double scale1 = zoomBase;
    
    int originX = 0;
    int originY = 0;

    if( this->GetViewImageRegion().GetSize()[0]*scale0 < this->cW )
    {
	originX = static_cast<int>( (this->cW - this->GetViewImageRegion().GetSize()[0]*scale0)/2);
    }
    if( this->GetViewImageRegion().GetSize()[1]*scale1 < this->cH )
    {
        originY = static_cast<int>( (this->cH - this->GetViewImageRegion().GetSize()[1]*scale1)/2);
    }
    
    cOffSetX = originX;
    cOffSetY = originY;
    cScale = zoomBase;
    
    glRasterPos2i(      originX ,  this->cH - originY );  
    glPixelZoom( scale0 , -scale1 );
    
    if( (this->cValidImData==true) && (this->cViewImData == true) )
    {
         if ( this->m_ModeView == GRAY_LEVEL )
         { 
	     	glDrawPixels(this->cWinDataSizeX,this->cWinDataSizeY, 
                          GL_LUMINANCE,GL_UNSIGNED_BYTE,this->cWinImData );
         }
         else
         { 
 	     	glDrawPixels(this->cWinDataSizeX,this->cWinDataSizeY, 
                         GL_RGB,GL_UNSIGNED_BYTE,this->cWinImData );
         }
      }

      // Affichage de l'overlay
      if(cOverlay && cValidOverlayData && cViewOverlayData) 
      {
	  glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDrawPixels(this->cWinDataSizeX, this->cWinDataSizeY, GL_RGBA, 
        GL_UNSIGNED_BYTE, this->cWinOverlayData);
      glDisable(GL_BLEND);
      }

  // Selection Box display
  if (this->cSelectRectangle)
  {
	float xI = this->coord[0]+this->cOffSetX;
	float yI = this->coord[1]+this->cOffSetY;
	float xF = this->coord[2]+this->cOffSetX;
	float yF = this->coord[3]+this->cOffSetY;
	
  	glColor3f( 0.0, 1.0, 0.0 );
        glPointSize( 3.0 );
        glBegin(GL_LINE_LOOP); 
		glVertex2f(xI,this->cH -yI);
		glVertex2f(xI,this->cH -yF);
		glVertex2f(xF,this->cH -yF);
		glVertex2f(xF,this->cH -yI);
	glEnd();
   }      

  // Clicked Points display 
  if( this->cViewClickedPoints )
    {
	glPointSize( 3.0 );
        {
            for ( int i = 0; i < this->numClickedPointsStored(); i++ )
            {
      		ClickPoint p;
				
                this->getClickedPoint( i, p );
					
                float pts[4] = { p.x+cOffSetX, p.y+cOffSetY, p.w, p.h };
		
		if (p.color == COLOR_BLUE)
		    glColor3f( 0.0, 0.0, 1.0 );
		else if (p.color == COLOR_RED)
		    glColor3f( 1.0, 0.0, 0.0 );		
		
		if ( !(p.isForBox))
		{
		    // Point selection case
		    glBegin(GL_POINTS);		
		    	glVertex2f( pts[0], this->cH - pts[1]);
		    glEnd();
		}
		else
		{
		    // Box selection case
		    glBegin(GL_QUADS);
		    	glVertex2f( pts[0],this->cH - pts[1]);
				glVertex2f( pts[0],this->cH - pts[1]-pts[3]); 
		    	glVertex2f( pts[0]+pts[2],this->cH - pts[1]-pts[3]); 
		    	glVertex2f( pts[0]+pts[2],this->cH - pts[1]);
		    glEnd();
		}
            }
        }
        
    }
  }    
}


template <class TPixel, class TPixelOverlay>
void
GLVectorImageViewClick<TPixel,TPixelOverlay>::
ClearSelectChannels(void)
{ 
        m_RedChannel            = -1;
        m_GreenChannel          = -1;
        m_BlueChannel           = -1;
        m_GrayLevelChannel      = -1;
        m_ChannelsWorks.clear();
 
}



template <class TPixel, class TPixelOverlay>
void 
GLVectorImageViewClick<TPixel,TPixelOverlay>::
CalculeDataMinMax(const RegionType & region, double & pMin, double & pMax)
{
        typedef itk::ImageRegionConstIterator<ImageType> InputIterator;
        InputIterator inIt(this->GetInput(), region);

        // Parcours des canaux a traiter
        unsigned int channelIn(0);
        unsigned int channelOut(0);
        unsigned int nbChannels(0);

        PixelType  pixelInput;
        pixelInput = inIt.Get();
        channelOut = 0;
        //Initialisation du Max/Min
        for ( nbChannels = 0 ; nbChannels < this->m_ChannelsWorks.size() ; nbChannels++)
        {
                channelIn = this->m_ChannelsWorks[nbChannels] - 1;
                pMax = static_cast<double>(pixelInput[channelIn]);
                pMin = pMax;
        }
        ++inIt;
        while( !inIt.IsAtEnd() )
        {
                pixelInput = inIt.Get();
                channelOut = 0;
                for ( nbChannels = 0 ; nbChannels < this->m_ChannelsWorks.size() ; nbChannels++)
                {
                        channelIn = this->m_ChannelsWorks[nbChannels] - 1;
                        if ( pixelInput[channelIn] > pMax )
                        {
                                pMax = static_cast<double>(pixelInput[channelIn]);
                        }
                        else if ( pixelInput[channelIn] < pMin )
                        {
                                pMin = static_cast<double>(pixelInput[channelIn]);
                        }
                        channelOut++;
                }
                ++inIt; 
        }
}
  
template <class TPixel, class TPixelOverlay>
void 
GLVectorImageViewClick<TPixel,TPixelOverlay>::
SetWinImData(const RegionType & zone) 
{
  IndexType ind;
  int l;
//  int m;
//  float tf;

  //otbMsgDevMacro(  << "Zone : " << zone );
  int lWinMinX = zone.GetIndex()[0];
  int lWinMinY = zone.GetIndex()[1];
  int lWinMaxX = lWinMinX + zone.GetSize()[0] - 1;
  int lWinMaxY = lWinMinY + zone.GetSize()[1] - 1;

  int startK = zone.GetIndex()[1];
    if(startK<0)
    startK = 0;
  int startJ = zone.GetIndex()[0];
  if(startJ<0)
    startJ = 0;
    
  float lCoef = (float)(255./(this->cIWMax-this->cIWMin));
  ImagePointer lInpuPtr = this->GetInput();
  PixelType  pixelInput;
  itk::VariableLengthVector<float> pixelOutput;
  int nbChannels(0);
  int channelIn(0);
  for(int k=startK; k <= lWinMaxY; k++)
    {
    ind[1] = k;
    
    if(k-lWinMinY >= (int)this->cWinDataSizeY)
      continue;

    for(int j=startJ; j <= lWinMaxX; j++) 
      {
      ind[0] = j;
      
      if(j-lWinMinX >= (int)this->cWinDataSizeX)
         continue;

        pixelInput = lInpuPtr->GetPixel(ind);
        pixelOutput.Reserve( lInpuPtr->GetVectorLength() );
        // Traitement sur les canaux selectionnes.
        for ( nbChannels = 0 ; nbChannels < this->m_ChannelsWorks.size() ; nbChannels++)
        {
                channelIn = this->m_ChannelsWorks[nbChannels] - 1;
                pixelOutput[channelIn] = static_cast<float>(pixelInput[channelIn]);
                pixelOutput[channelIn] = (float)( (pixelOutput[channelIn]-this->cIWMin) * lCoef);
        }
        // On s'assure que les valeurs sont dans [0,255]
        for ( nbChannels = 0 ; nbChannels < this->m_ChannelsWorks.size() ; nbChannels++)
        {
                channelIn = this->m_ChannelsWorks[nbChannels] - 1;
                if ( pixelOutput[channelIn] > 255. ) pixelOutput[channelIn] = 255.;
                else if ( pixelOutput[channelIn] < 0. ) pixelOutput[channelIn] = 0.;

        }
      
      l = (j-lWinMinX)*this->m_NbDim + (k-lWinMinY)*this->cWinDataSizeX*this->m_NbDim;
      
      if( this->m_ModeView == GRAY_LEVEL )
      {
                this->cWinImData[l] = (unsigned char)pixelOutput[this->m_GrayLevelChannel - 1];
       }
       else
       {
                if ( this->m_RedChannel != -1 )         this->cWinImData[l+0] = (unsigned char)pixelOutput[this->m_RedChannel - 1];
                else                                    this->cWinImData[l+0] = (unsigned char)(0);
                if ( this->m_GreenChannel != -1 )       this->cWinImData[l+1] = (unsigned char)pixelOutput[this->m_GreenChannel - 1];
                else                                    this->cWinImData[l+1] = (unsigned char)(0);
                if ( this->m_BlueChannel != -1 )        this->cWinImData[l+2] = (unsigned char)pixelOutput[this->m_BlueChannel - 1];
                else                                    this->cWinImData[l+2] = (unsigned char)(0);
       }
     }

   }
}

template <class TPixel, class TPixelOverlay>
void 
GLVectorImageViewClick<TPixel,TPixelOverlay>::
ViewClickedPoints( bool newViewClickedPoints )
{
    this->cViewClickedPoints = newViewClickedPoints;

    this->redraw();
}

template <class TPixel, class TPixelOverlay>
bool
GLVectorImageViewClick<TPixel,TPixelOverlay>::
ViewClickedPoints()
{
    return this->cViewClickedPoints;
}


  
// Overlay
template <class TPixel, class TPixelOverlay>
void 
GLVectorImageViewClick<TPixel, TPixelOverlay>
::SetInputOverlay( OverlayPointer newOverlayData,
		   OverlayPointer newOverlayDataClassRed, 
		   OverlayPointer newOverlayDataClassBlue)
  {
  RegionType newoverlay_region = 
    newOverlayData->GetLargestPossibleRegion();

  SizeType   newoverlay_size   = newoverlay_region.GetSize();

   if( this->cValidImData 
      &&  (newoverlay_size[0] == this->cDimSize[0])
      &&  (newoverlay_size[1] == this->cDimSize[1])
    )
    {
    this->cOverlayData          = newOverlayData;
    this->cOverlayDataClassRed  = newOverlayDataClassRed;
    this->cOverlayDataClassBlue = newOverlayDataClassBlue;
    
    //this->cViewOverlayData  = true;
    this->cValidOverlayData = true;
    this->cOverlayOpacity   = (float)0.0;
    
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
        itkExceptionMacro(<< "GLVectorImageViewClick: Please set the input image before overlay"  
                  	 << std::endl
       				 << "GLVectorImageViewClick: Overlay not set."
		             << std::endl);
		}
      else if((newoverlay_size[0] != this->cDimSize[0])
      ||  (newoverlay_size[1] != this->cDimSize[1])
      )
        {
   		itkExceptionMacro(<< "GLVectorImageViewClick: Error: overlay and input images should be the same size"
					 << std::endl 
		 			 << "Image size : " << this->cDimSize[0]<< " " << this->cDimSize[1] 
		 			 << std::endl
					 << "Overlay size : " << newoverlay_size[0] << " " << newoverlay_size[1]
		 			 << std::endl
					 << "GLVectorImageViewClick: Overlay not set."
					 << std::endl);
		 }
      
    }
  }
  
  template <class TPixel, class TPixelOverlay>
  const typename GLVectorImageViewClick<TPixel, 
  TPixelOverlay>::OverlayPointer &
  GLVectorImageViewClick<TPixel, TPixelOverlay>::GetInputOverlay( void ) 
  const
  {
  	return this->cOverlayData;
  }
  
  template <class TPixel, class TPixelOverlay>
  void 
  GLVectorImageViewClick<TPixel, TPixelOverlay>::
  ViewOverlayData( bool newViewOverlayData)
  {
    	this->cViewOverlayData = newViewOverlayData;
    	if( this->cViewOverlayCallBack != NULL )
    	{
    		cViewOverlayCallBack();
    	}
  
  		this->redraw();  
  }
  
  template <class TPixel, class TPixelOverlay>
bool 
GLVectorImageViewClick<TPixel, TPixelOverlay>::
ViewOverlayData(void)
  {
  
  return this->cViewOverlayData;
  
  }

template <class TPixel, class TPixelOverlay>
void 
GLVectorImageViewClick<TPixel, TPixelOverlay>::
ViewOverlayCallBack( void (* newViewOverlayCallBack)(void) )
  {
  this->cViewOverlayCallBack = newViewOverlayCallBack;
  }


template <class TPixel, class TPixelOverlay>
void 
GLVectorImageViewClick<TPixel, TPixelOverlay>::
OverlayOpacity( float newOverlayOpacity )
  {
  this->cOverlayOpacity = newOverlayOpacity;
  
  if(this->cViewOverlayCallBack != NULL) 
    {
    this->cViewOverlayCallBack();
    }
  }

template <class TPixel, class TPixelOverlay>  
float 
GLVectorImageViewClick<TPixel, TPixelOverlay>::
OverlayOpacity(void)
  {
  return this->cOverlayOpacity;
  }


// For Click

template <class TPixel, class TPixelOverlay>
void GLVectorImageViewClick<TPixel,TPixelOverlay>::clickMode(ClickModeType newClickMode)
  {
  cClickMode = newClickMode;
  }




template <class TPixel, class TPixelOverlay>
ClickModeType GLVectorImageViewClick<TPixel,TPixelOverlay>::clickMode(void)
  {
  return cClickMode;
  }




template <class TPixel, class TPixelOverlay>
void GLVectorImageViewClick<TPixel,TPixelOverlay>::
clickSelectCallBack(void 
                    (*newClickSelectCallBack)(float _x, float _y, 
                                              float _z, float _v))
  {
  cClickSelectCallBack = newClickSelectCallBack;
  }




template <class TPixel, class TPixelOverlay>
void GLVectorImageViewClick<TPixel,TPixelOverlay>::
clickSelectCallBack(void 
                    (*newClickSelectArgCallBack)(float _x, float _y, 
                                                 float _z, float _v, 
                                                 void *clickSelectArg), 
                    void * newClickSelectArg)
  {
  cClickSelectArg = newClickSelectArg;
  cClickSelectArgCallBack = newClickSelectArgCallBack;
  }




//
//
template <class TPixel, class TPixelOverlay>
void GLVectorImageViewClick<TPixel,TPixelOverlay>::clickSelect(float newX, float newY,
						 float newZ, ColorType color)
  {    

  cClickSelect[0] = newX;
  if(cClickSelect[0]<0)
    cClickSelect[0] = 0;
  /*if(cClickSelect[0] >= this->cDimSize[0])
    cClickSelect[0] = this->cDimSize[0]-1;*/
  
  cClickSelect[1] = newY;
  if(cClickSelect[1]<0)
    cClickSelect[1] = 0;
 /* if(cClickSelect[1] >= this->cDimSize[1])
    cClickSelect[1] = this->cDimSize[1]-1;*/
  
  cClickSelect[2] = newZ;
  if(cClickSelect[2]<0)
    cClickSelect[2] = 0;
  if(cClickSelect[2] >= this->cDimSize[2])
    cClickSelect[2] = this->cDimSize[2]-1;
  
  typename ImageType::IndexType ind;
  
  ind[0] = (unsigned long)cClickSelect[0];
  ind[1] = (unsigned long)cClickSelect[1];
  ind[2] = (unsigned long)cClickSelect[2];
  //cClickSelectV = (float)GetInput()->GetPixel(ind);
//  (const_cast< ImageType * >
//   (this->itk::ProcessObject::GetInput(0) ) )->GetPixel(ind);
  
  /*if length of list is equal to max, remove the ea   cClickedPoints.pop_front();rliest point stored */
  if((maxClickPoints>0)&&(cClickedPoints.size() == maxClickPoints))
    cClickedPoints.pop_back();

  ClickPoint clickedPoint(cClickSelect[0], cClickSelect[1], cClickSelect[2], 
  				cClickSelectV,color);
  cClickedPoints.push_front( clickedPoint ); 
  
  if(cClickSelectCallBack != NULL)
    cClickSelectCallBack(cClickSelect[0], cClickSelect[1], 
    cClickSelect[2], cClickSelectV);
  if(cClickSelectArgCallBack != NULL)
    cClickSelectArgCallBack(cClickSelect[0], cClickSelect[1], 
    cClickSelect[2], cClickSelectV,
    cClickSelectArg);
    
  this->redraw();
  }


template <class TPixel, class TPixelOverlay>
void GLVectorImageViewClick<TPixel,TPixelOverlay>::clickSelectBox(float newX, float newY,
						 float newZ, ColorType color,
						 int width, int height)
  {    

  cClickSelect[0] = newX;
  if(cClickSelect[0]<0)
    cClickSelect[0] = 0;
   
  cClickSelect[1] = newY;
  if(cClickSelect[1]<0)
    cClickSelect[1] = 0;
  
  cClickSelect[2] = newZ;
  
  typename ImageType::IndexType ind;
  
  ind[0] = (unsigned long)cClickSelect[0];
  ind[1] = (unsigned long)cClickSelect[1];
  ind[2] = (unsigned long)cClickSelect[2];
  //cClickSelectV = (float)GetInput()->GetPixel(ind);
//  (const_cast< ImageType * >
//   (this->itk::ProcessObject::GetInput(0) ) )->GetPixel(ind);
  
  /*if length of list is equal to max, remove the earliest point stored */
  if((maxClickPoints>0)&&(cClickedPoints.size() == maxClickPoints))
    cClickedPoints.pop_back();

  ClickPoint clickedPoint(cClickSelect[0], cClickSelect[1], cClickSelect[2], 
  				cClickSelectV,color,width,height);
   cClickedPoints.pop_front();
   cClickedPoints.push_front( clickedPoint ); 

   
  if(cClickSelectCallBack != NULL)
    cClickSelectCallBack(cClickSelect[0], cClickSelect[1], 
    cClickSelect[2], cClickSelectV);
  if(cClickSelectArgCallBack != NULL)
    cClickSelectArgCallBack(cClickSelect[0], cClickSelect[1], 
    cClickSelect[2], cClickSelectV,
    cClickSelectArg);
    
  this->redraw();
  }


template <class TPixel, class TPixelOverlay>
void GLVectorImageViewClick<TPixel,TPixelOverlay>::maxClickedPointsStored(unsigned int i)
  {
  maxClickPoints = i;
  }




template <class TPixel, class TPixelOverlay>
unsigned int GLVectorImageViewClick<TPixel,TPixelOverlay>::maxClickedPointsStored()
  {
  return maxClickPoints;
  }




template <class TPixel, class TPixelOverlay>
float GLVectorImageViewClick<TPixel,TPixelOverlay>::clickSelectX(void)
  {
  return cClickSelect[0];
  }




template <class TPixel, class TPixelOverlay>
float GLVectorImageViewClick<TPixel,TPixelOverlay>::clickSelectY(void)
  {
  return cClickSelect[1];
  }




template <class TPixel, class TPixelOverlay>
float GLVectorImageViewClick<TPixel,TPixelOverlay>::clickSelectZ(void)
  {
  return cClickSelect[2];
  }




template <class TPixel, class TPixelOverlay>
bool GLVectorImageViewClick<TPixel,TPixelOverlay>::getClickedPoint(unsigned int index, ClickPoint & point)
  {
  if(index >= cClickedPoints.size())
    {
    return false;
    }
  std::list<ClickPoint>::const_iterator j = cClickedPoints.begin();
  
  for(int i=0;i<static_cast<int>(index);i++,j++);
    point = *j;
  return true;
  }
  
template <class TPixel, class TPixelOverlay>
void GLVectorImageViewClick<TPixel,TPixelOverlay>::
clickBoxCallBack(void (*newClickBoxCallBack)(float minX, float minY, 
                                             float minZ, float maxX, 
                                             float maxY, float maxZ))
  {
  cClickBoxCallBack = newClickBoxCallBack;
  }




template <class TPixel, class TPixelOverlay>
void GLVectorImageViewClick<TPixel,TPixelOverlay>::
clickBoxCallBack(void (*newClickBoxArgCallBack)(float minX, float minY, 
                                                float minZ, float maxX, 
                                                float maxY, float maxZ, 
                                                void * clickBoxArg), 
                 void * newClickBoxArg)
  {
  cClickBoxArg = newClickBoxArg;
  cClickBoxArgCallBack = newClickBoxArgCallBack;
  }




template <class TPixel, class TPixelOverlay>
void GLVectorImageViewClick<TPixel,TPixelOverlay>::boxMin(float minX, float minY, float minZ)
  {
  cBoxMin[0] = minX;
  cBoxMin[1] = minY;
  cBoxMin[2] = minZ;
  }




template <class TPixel, class TPixelOverlay>
void GLVectorImageViewClick<TPixel,TPixelOverlay>::boxMax(float x, float y, float z)
  {
  float x0, y0, z0;
  float x1, y1, z1;
  
  x0 = (cBoxMin[0]<x) ? cBoxMin[0] : x;
  y0 = (cBoxMin[1]<y) ? cBoxMin[1] : y;
  z0 = (cBoxMin[2]<z) ? cBoxMin[2] : z;
  
  x1 = (cBoxMin[0]<x) ? x : cBoxMin[0];
  y1 = (cBoxMin[1]<y) ? y : cBoxMin[1];
  z1 = (cBoxMin[2]<z) ? z : cBoxMin[2];
  
  cBoxMin[0] = x0;
  cBoxMin[1] = y0;
  cBoxMin[2] = z0;
  
  cBoxMax[0] = x1;
  cBoxMax[1] = y1;
  cBoxMax[2] = z1;
  
  if(cClickBoxCallBack != NULL)
    cClickBoxCallBack(cBoxMin[0], cBoxMin[1], cBoxMin[2],
    cBoxMax[0], cBoxMax[1], cBoxMax[2]);
  if(cClickBoxArgCallBack != NULL)
    cClickBoxArgCallBack(cBoxMin[0], cBoxMin[1], cBoxMin[2],
    cBoxMax[0], cBoxMax[1], cBoxMax[2],
    cClickBoxArg);
  }
  
  
template <class TPixel, class TPixelOverlay>
void
GLVectorImageViewClick<TPixel, TPixelOverlay>::
ActivateOverlay(bool b)
{
	cOverlay = b ;
}
  



template <class TPixel, class TPixelOverlay>
typename GLVectorImageViewClick<TPixel, TPixelOverlay>::ColorTablePointer 
GLVectorImageViewClick<TPixel, TPixelOverlay>::
GetColorTable(void)
  {
  return this->cColorTable;
  } 
  
template <class TPixel, class TPixelOverlay>
void
GLVectorImageViewClick<TPixel, TPixelOverlay>::
SetColorTable(typename 
              GLVectorImageViewClick<TPixel, TPixelOverlay>::ColorTablePointer 
              newColorTable)
  {
  cColorTable = newColorTable;
  } 
  
  
}; //namespace

#endif

