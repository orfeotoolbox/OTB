#ifndef _otbImageViewerAS_txx
#define _otbImageViewerAS_txx

#include "otbImageViewerAS.h"
#include <iostream>

#define MAX(_A,_B) ((_A) > (_B) ? (_A) : (_B))
#define MIN(_A,_B) ((_A) < (_B) ? (_A) : (_B))



namespace otb
{
 
template <class TPixel>
const std::string ImageViewerAS<TPixel>::m_SuffixPrincipalTitle = " - Principal Window";
 
template <class TPixel>
ImageViewerAS<TPixel>
::ImageViewerAS()
{
        m_Label = "Image";
        this->ClearSelectChannels();
        m_PrincipalImage = NULL;
   
        m_InitPrincipalWinSizeMax = 300;
        m_ExtractROIFilter = ExtractROIFilterType::New();
        
        CreateGUI();
}

  
template <class TPixel>
ImageViewerAS<TPixel>
::~ImageViewerAS()
{

}

template <class TPixel>
void
ImageViewerAS<TPixel>
::PrincipalAction(const IndexType & index) 
{
	//Si une action est faite sur la fenetre principale (ex : agrandir al fenetre, dans le cas ou il la fenetre scroll existe) 
  	m_PrincipalViewBeforeClassif->update();
	m_PrincipalViewAfterClassif->update();
}


template <class TPixel>
void
ImageViewerAS<TPixel>
::ExtractImagePrincipal(const RegionType & zone) 
{
        m_ExtractROIFilter->SetSizeX( zone.GetSize()[0]);
        m_ExtractROIFilter->SetSizeY( zone.GetSize()[1]);
        m_ExtractROIFilter->SetStartX( zone.GetIndex()[0] );
        m_ExtractROIFilter->SetStartY( zone.GetIndex()[1] );
        m_ExtractROIFilter->UpdateLargestPossibleRegion();
        m_ExtractROIFilter->Update();
        m_PrincipalImage = m_ExtractROIFilter->GetOutput();
}


/*template <class TPixel>
void
ImageViewerAS<TPixel>
::PrincipalResize() */







template <class TPixel>
void
ImageViewerAS<TPixel>
::BuildHMI(void) 
{
	m_InputImage->Update();
	m_PrincipalImage = m_InputImage;
	this->GenerateChannelsInformations();
	this->SetChannelsInformationsToImagesView();
  	this->Superclass::Update();
  	static bool firstTime = true;
	
        // Fenetre d'affichage AVANT classif
        //----------------------------------------
	std::cout << "Taille input image : " << m_InputImage->GetLargestPossibleRegion().GetSize()[0] 
	<< " " << m_InputImage->GetLargestPossibleRegion().GetSize()[1] << std::endl ; 
	std::cout << "Taille principal image : " << m_PrincipalViewBeforeClassif->sizeX()
	<< " " << m_PrincipalViewBeforeClassif->sizeY() << std::endl;
	
	/*m_ShrinkImageFilter->SetShrinkFactors(0);
        m_ShrinkImageFilter->SetInput( m_InputImage );
        m_ShrinkImageFilter->Update();
        m_PrincipalImage = m_ShrinkImageFilter->GetOutput();*/
	
	
	
        m_PrincipalViewBeforeClassif->SetInput( m_PrincipalImage );
        m_PrincipalViewBeforeClassif->SetDoubleWindow( iviewWindowPrincipal );
        m_PrincipalViewBeforeClassif->SetViewer( this );
        m_PrincipalViewBeforeClassif->SetLabel( m_Label + m_SuffixPrincipalTitle );
  	m_PrincipalViewBeforeClassif->Show();
	
        // Fenetre d'affichage APRES classif
        //----------------------------------------
        m_PrincipalViewAfterClassif->SetInput( m_PrincipalImage );
        m_PrincipalViewAfterClassif->SetDoubleWindow( iviewWindowPrincipal );
        m_PrincipalViewAfterClassif->SetViewer( this );
        m_PrincipalViewAfterClassif->SetLabel( m_Label + m_SuffixPrincipalTitle );
  	m_PrincipalViewAfterClassif->Show();
	
       	if( firstTime == true)
  	{
    		firstTime = false;
    		Fl::check();
    		m_PrincipalViewBeforeClassif->redraw();
		m_PrincipalViewAfterClassif->redraw();
    		Fl::check();
  	}
        SetLabel("aaa");
}



template <class TPixel>
void
ImageViewerAS<TPixel>
::SetImage(itk::ImageBase<2> * img)
{
  m_InputImage = dynamic_cast<ImageType *>( img );
  this->Modified();
}

template <class TPixel>
void 
ImageViewerAS<TPixel>
::Show(void)
{
  this->Update();
}


template <class TPixel>
void 
ImageViewerAS<TPixel>
::Hide(void)
{
  iviewWindowPrincipal->hide();
}


template <class TPixel>
void 
ImageViewerAS<TPixel>
::Update(void)
{
  this->BuildHMI();
}

template <class TPixel>
void
ImageViewerAS<TPixel>
::CreatePrincipalImageViewBeforeClassif( Fl_Group * group, Fl_Gl_Window * original )
{
  int x = original->x();
  int y = original->y();
  int w = original->w();
  int h = original->h();

  const char * label = original->label();

  this->m_PrincipalViewBeforeClassif = PrincipalImageViewType::New();
  this->m_PrincipalViewBeforeClassif->Init(x,y,w,h,label);
  this->m_PrincipalViewBeforeClassif->box( FL_EMBOSSED_BOX );

  group->remove( original );

  delete original;
}


template <class TPixel>
void
ImageViewerAS<TPixel>
::CreatePrincipalImageViewAfterClassif( Fl_Group * group, Fl_Gl_Window * original )
{
  int x = original->x();
  int y = original->y();
  int w = original->w();
  int h = original->h();

  const char * label = original->label();

  this->m_PrincipalViewAfterClassif = PrincipalImageViewType::New();
  this->m_PrincipalViewAfterClassif->Init(x,y,w,h,label);
  this->m_PrincipalViewAfterClassif->box( FL_EMBOSSED_BOX );

  group->remove( original );

  delete original;
}



template <class TPixel>
void 
ImageViewerAS<TPixel>
::ClickSelectCallBack( void (*newClickSelectArgCallBack)(float, float,
                                                         float,
                                                         void *),
                                                         void * newClickSelectArg)
{
//THOMAS
/*
  m_PrincipalViewBeforeClassif->clickSelectCallBack( newClickSelectArgCallBack, 
                                    newClickSelectArg           ); 
  m_ScrollView->clickSelectCallBack( newClickSelectArgCallBack, 
                                    newClickSelectArg           ); 
  m_ZoomView->clickSelectCallBack( newClickSelectArgCallBack, 
                                    newClickSelectArg           ); 
*/
}

template <class TPixel>
void 
ImageViewerAS<TPixel>
::GenerateChannelsInformations(void)
{

        m_ChannelsWorks.clear();
        
        //Controle que l'utilisateur a bien selectionne de canaux coherent
        if( m_GrayLevelChannel != -1 )
        {
                if( (m_RedChannel != -1 ) || (m_GreenChannel != -1 ) || (m_BlueChannel != -1 ) )
                {
                        itkExceptionMacro(<< "Vous avez selectionne un canal pour le niveau de gris, mais aussi un (ou des) canaux en RGB" );
                }
                
                m_ModeView = ImageViewBaseType::GRAY_LEVEL;
        }
        else
        {
               m_ModeView = ImageViewBaseType::RGB_LEVEL;
               if( (m_RedChannel == -1 ) && (m_GreenChannel == -1 ) && (m_BlueChannel == -1 ) )
               {
                        //itkExceptionMacro(<< "Aucun canal n'a ete selectionne" );
                        // On les choisi automatiquement,si c'est possible
                        int nbCanauxInputImage = m_PrincipalImage->GetNumberOfComponentsPerPixel();
                        
                        switch ( nbCanauxInputImage )
                        {
                                case 1 :
                                        m_GrayLevelChannel = 1;
                                        m_ModeView = ImageViewBaseType::GRAY_LEVEL;
                                        break;
                                default :
                                case 3 : 
                                        m_BlueChannel = 3;
                                        m_ModeView = ImageViewBaseType::RGB_LEVEL;
                                case 2 : 
                                        m_GreenChannel = 2;
                                        m_RedChannel = 1;
                                        m_ModeView = ImageViewBaseType::RGB_LEVEL;
                                        break;
                        }
               }
               
        }
        // On selectionne dans "m_ChannelsWorks" que les canaux valide, a lire dans l'image d'entree
        switch( m_ModeView )
        {
                case ImageViewBaseType::GRAY_LEVEL :       m_ChannelsWorks.push_back( m_GrayLevelChannel );
                        break;
                case ImageViewBaseType::RGB_LEVEL :        
                                        if ( m_RedChannel != -1 ) m_ChannelsWorks.push_back( m_RedChannel );
                                        if ( m_GreenChannel != -1 ) m_ChannelsWorks.push_back( m_GreenChannel );
                                        if ( m_BlueChannel != -1 ) m_ChannelsWorks.push_back( m_BlueChannel );
                        break;
        
        }
        //Controle que les canaux selection existe bien dans l'image d'entree
        for ( int nbChannels = 0 ; nbChannels < m_ChannelsWorks.size() ; nbChannels++)
        {
                if ( (m_ChannelsWorks[nbChannels] <= 0) || (m_ChannelsWorks[nbChannels] > m_PrincipalImage->GetNumberOfComponentsPerPixel()) )
                {
                        itkExceptionMacro(<< "L'image d'entree possede "<<m_PrincipalImage->GetNumberOfComponentsPerPixel()<< "\n Vous devez selectionner des canaux dans [1..."<<m_PrincipalImage->GetNumberOfComponentsPerPixel()<<"]." );
                }
        }
        m_NbDim = (int)m_ModeView;
}


template <class TPixel>
void 
ImageViewerAS<TPixel>::
SetChannelsInformationsToImagesView(void)
{
	m_PrincipalViewBeforeClassif->SetChannelsWorks(m_ChannelsWorks);
	m_PrincipalViewBeforeClassif->SetModeView(m_ModeView);
	m_PrincipalViewBeforeClassif->SetNbDim(m_NbDim);
        m_PrincipalViewBeforeClassif->SetGrayLevelChannel(m_GrayLevelChannel);
        m_PrincipalViewBeforeClassif->SetRGBChannels(m_RedChannel,m_GreenChannel,m_BlueChannel);
	
	m_PrincipalViewAfterClassif->SetChannelsWorks(m_ChannelsWorks);
	m_PrincipalViewAfterClassif->SetModeView(m_ModeView);
	m_PrincipalViewAfterClassif->SetNbDim(m_NbDim);
        m_PrincipalViewAfterClassif->SetGrayLevelChannel(m_GrayLevelChannel);
        m_PrincipalViewAfterClassif->SetRGBChannels(m_RedChannel,m_GreenChannel,m_BlueChannel);
}


template <class TPixel>
void 
ImageViewerAS<TPixel>::
ClearSelectChannels(void)
{ 
        m_RedChannel            = -1;
        m_GreenChannel          = -1;
        m_BlueChannel           = -1;
        m_GrayLevelChannel      = -1;
        m_ChannelsWorks.clear();
}

// Gestion des événements boutons

template <class TPixel>
void 
ImageViewerAS<TPixel>::EraseLastClickedPoint()
{
	m_PrincipalViewBeforeClassif->UndoPt();
	m_PrincipalViewAfterClassif->UndoPt();
}

template <class TPixel>
void 
ImageViewerAS<TPixel>::EraseAllClickedPoints()
{
	m_PrincipalViewBeforeClassif->ResetPts();
	m_PrincipalViewAfterClassif->ResetPts();
}

template <class TPixel>
void 
ImageViewerAS<TPixel>::LearnAlgorythm()
{
	std::cout << "LEARN" << std::endl;
}

template <class TPixel>
void 
ImageViewerAS<TPixel>::ClassificationAlgorythm()
{
	std::cout << "CLASSIF" << std::endl;
}

template <class TPixel>
void 
ImageViewerAS<TPixel>::DisplayFirstClass()
{
	std::cout << "FIRST CLASS" << std::endl;
}

template <class TPixel>
void 
ImageViewerAS<TPixel>::DisplaySecondClass()
{
	std::cout << "SECOND CLASS" << std::endl;
}

template <class TPixel>
void 
ImageViewerAS<TPixel>::LoadImage()
{
	std::cout << "LOAD" << std::endl;
}

template <class TPixel>
void 
ImageViewerAS<TPixel>::SynchronizeClickedPoints(float x,float y,float z,
ColorType color)
{
  m_PrincipalViewAfterClassif->ViewClickedPoints(true);
  m_PrincipalViewAfterClassif->clickSelect(x,y,z,color);
}

template <class TPixel>
void 
ImageViewerAS<TPixel>::SynchronizeClickedPointsBox(float x,float y,float z,
ColorType color, int width, int height)
{
  m_PrincipalViewAfterClassif->ViewClickedPoints(true);
  m_PrincipalViewAfterClassif->clickSelectBox(x,y,z,color,width,height);
}

} // end namespace otb
#endif

