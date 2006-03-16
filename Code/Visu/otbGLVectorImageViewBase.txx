#ifndef otbGLVectorImageViewBase_txx
#define otbGLVectorImageViewBase_txx

#include "itkExceptionObject.h"
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
GenerateChannelsInformations(void)
{

        m_ChannelsWorks.clear();
        
        //Controle que l'utilisateur a bien selectionne de canaux coherent
        if( m_GrayLevelChannel != -1 )
        {
                if( (m_RedChannel != -1 ) || (m_GreenChannel != -1 ) || (m_BlueChannel != -1 ) )
                {
                        itkExceptionMacro(<< "Vous avez selectionne un canal pour le niveau de gris, mais aussi un (ou des) canaux en RGB" );
                }
                
                m_ModeView = GRAY_LEVEL;
        }
        else
        {
               m_ModeView = RGB_LEVEL;
               if( (m_RedChannel == -1 ) && (m_GreenChannel == -1 ) && (m_BlueChannel == -1 ) )
               {
                        //itkExceptionMacro(<< "Aucun canal n'a ete selectionne" );
                        // On les choisi automatiquement,si c'est possible
                        int nbCanauxInputImage = this->GetInput()->GetNumberOfComponentsPerPixel();
                        
                        std::cout << "Nb canaux de l'image : "<<nbCanauxInputImage<<std::endl;

                        switch ( nbCanauxInputImage )
                        {
                                case 1 :
                                        m_GrayLevelChannel = 1;
                                        m_ModeView = GRAY_LEVEL;
                                        break;
                                default :
                                case 3 : 
                                        m_BlueChannel = 3;
                                        m_ModeView = RGB_LEVEL;
                                case 2 : 
                                        m_GreenChannel = 2;
                                        m_RedChannel = 1;
                                        m_ModeView = RGB_LEVEL;
                                        break;
                        }
               }
               
        }
        // On selectionne dans "m_ChannelsWorks" que les canaux valide, a lire dans l'image d'entree
        switch( m_ModeView )
        {
                case GRAY_LEVEL :       m_ChannelsWorks.push_back( m_GrayLevelChannel );
                        break;
                case RGB_LEVEL :        if ( m_RedChannel != -1 ) m_ChannelsWorks.push_back( m_RedChannel );
                                        if ( m_GreenChannel != -1 ) m_ChannelsWorks.push_back( m_GreenChannel );
                                        if ( m_BlueChannel != -1 ) m_ChannelsWorks.push_back( m_BlueChannel );
                        break;
        
        }
        //Controle que les canaux selection existe bien dans l'image d'entree
        for ( int nbChannels = 0 ; nbChannels < m_ChannelsWorks.size() ; nbChannels++)
        {
                if ( (m_ChannelsWorks[nbChannels] <= 0) || (m_ChannelsWorks[nbChannels] > this->GetInput()->GetNumberOfComponentsPerPixel()) )
                {
                        itkExceptionMacro(<< "L'image d'entree possede "<<this->GetInput()->GetNumberOfComponentsPerPixel()<< "\n Vous devez selectionner des canaux dans [1..."<<this->GetInput()->GetNumberOfComponentsPerPixel()<<"]." );
                }
        }
        m_NbDim = (int)m_ModeView;
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

}; //namespace

#endif

