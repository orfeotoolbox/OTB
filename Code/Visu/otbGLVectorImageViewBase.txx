#ifndef otbGLVectorImageViewBase_txx
#define otbGLVectorImageViewBase_txx

#include "itkExceptionObject.h"
#include "itkImageRegionConstIterator.h"

#include "otbGLVectorImageViewBase.h"

#include <math.h>

namespace otb
{

template <class TPixel, class OverlayPixelType>
GLVectorImageViewBase<TPixel, OverlayPixelType>::
GLVectorImageViewBase() : VectorImageView<TPixel>(), Fl_Gl_Window(0,0,0,0,0)
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
  
  this->ClearSelectChannels();
}

template <class TPixel, class OverlayPixelType>
GLVectorImageViewBase<TPixel, OverlayPixelType>::
~GLVectorImageViewBase()
{

}


template <class TPixel, class OverlayPixelType>
void
GLVectorImageViewBase<TPixel, OverlayPixelType>::
ClearSelectChannels(void)
{ 
        m_RedChannel            = -1;
        m_GreenChannel          = -1;
        m_BlueChannel           = -1;
        m_GrayLevelChannel      = -1;
        m_ChannelsWorks.clear();
 
}



template <class TPixel, class OverlayPixelType>
void 
GLVectorImageViewBase<TPixel, OverlayPixelType>::
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
  
template <class TPixel, class OverlayPixelType>
void 
GLVectorImageViewBase<TPixel, OverlayPixelType>::
SetWinImData(const RegionType & zone) 
{

  IndexType ind;
  
  int l, m;
  
  float tf;

//std::cout << " Region Image - Requested      : Star("<<this->GetInput()->GetRequestedRegion().GetIndex()[0]<<","<<this->GetInput()->GetRequestedRegion().GetIndex()[1]<<") Size("<<this->GetInput()->GetRequestedRegion().GetSize()[0]<<","<<this->GetInput()->GetRequestedRegion().GetSize()[1]<<") "<<std::endl;
//std::cout << "              - Buffer display : Star("<<zone.GetIndex()[0]<<","<<zone.GetIndex()[1]<<") Size("<<zone.GetSize()[0]<<","<<zone.GetSize()[1]<<") "<<std::endl;
//std::cout << "                                 Centre ("<<this->cWinCenter[0]<<","<<this->cWinCenter[1]<<")"<<std::endl;
  
//  ind[ this->cWinOrder[ 2 ] ] = this->cWinCenter[ this->cWinOrder[ 2 ] ];
  int lWinMinX = zone.GetIndex()[0];
  int lWinMinY = zone.GetIndex()[1];
  int lWinMaxX = lWinMinX + zone.GetSize()[0] - 1;
  int lWinMaxY = lWinMinY + zone.GetSize()[1] - 1;


//  int startK = this->cWinMinY;
  int startK = zone.GetIndex()[1];
    if(startK<0)
    startK = 0;
//  int startJ = this->cWinMinX;
  int startJ = zone.GetIndex()[0];
  if(startJ<0)
    startJ = 0;
    
  float lCoef = (float)(255./(this->cIWMax-this->cIWMin));
  ImagePointer lInpuPtr = this->GetInput();
  PixelType  pixelInput;
//  PixelType  pixelOutput;
  itk::VariableLengthVector<float> pixelOutput;
  int nbChannels(0);
  int channelIn(0);
  for(int k=startK; k <= lWinMaxY; k++)
//  for(int k=lWinMaxY ; k >= startK; k--)
    {
//    ind[this->cWinOrder[1]] = k;
    ind[1] = k;
    
    if(k-lWinMinY >= (int)this->cWinDataSizeY)
      continue;

    for(int j=startJ; j <= lWinMaxX; j++) 
      {
//      ind[this->cWinOrder[0]] = j;
      ind[0] = j;
      
      if(j-lWinMinX >= (int)this->cWinDataSizeX)
         continue;

      switch( this->cImageMode ) 
        {
        default:
        case IMG_VAL:
                pixelInput = lInpuPtr->GetPixel(ind);
                pixelOutput.Reserve( lInpuPtr->GetVectorLength() );
                // Traitement sur les canaux selectionnes.
                for ( nbChannels = 0 ; nbChannels < this->m_ChannelsWorks.size() ; nbChannels++)
                {
                        channelIn = this->m_ChannelsWorks[nbChannels] - 1;
                        pixelOutput[channelIn] = static_cast<float>(pixelInput[channelIn]);
                        pixelOutput[channelIn] = (float)( (pixelOutput[channelIn]-this->cIWMin) * lCoef);
                }
          break;
        }
        // On s'assure que les valeurs sont dans [0,255]
        for ( nbChannels = 0 ; nbChannels < this->m_ChannelsWorks.size() ; nbChannels++)
        {
                channelIn = this->m_ChannelsWorks[nbChannels] - 1;
                if ( pixelOutput[channelIn] > 255. ) pixelOutput[channelIn] = 255.;
                else if ( pixelOutput[channelIn] < 0. ) pixelOutput[channelIn] = 0.;

        }
      
//      l = ((j-lWinMinX) + (k-lWinMinY)*this->cWinDataSizeX)*this->m_NbDim;
      l = (j-lWinMinX)*this->m_NbDim + (k-lWinMinY)*this->cWinDataSizeX*this->m_NbDim;
      
//                this->cWinImData[l] = (unsigned char)tf;
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

template <class TPixel, class OverlayPixelType>
void 
GLVectorImageViewBase<TPixel, OverlayPixelType>::
DrawRectangle(const RegionType & zone) 
{

}

}; //namespace

#endif

