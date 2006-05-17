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
        m_PrincipalLeftImage = NULL;
        m_PrincipalRightImage = NULL;
        m_InitPrincipalWinSizeMax = 300;
        //m_ExtractROIFilter = ExtractROIFilterType::New();
        
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
::BuildHMI(void) 
{
	m_InputLeftImage->Update();
	m_InputRightImage->Update();
	m_PrincipalLeftImage = m_InputLeftImage;
	m_PrincipalRightImage = m_InputRightImage;
	this->ClearSelectChannels();
	this->GenerateLeftChannelsInformations();
	this->GenerateRightChannelsInformations();
	this->SetChannelsInformationsToImagesView();
  	this->Superclass::Update();
  	static bool firstTime = true;
	
        // Fenetre d'affichage AVANT classif
        //----------------------------------------
	/*std::cout << "Taille input image : " << m_InputLeftImage->GetLargestPossibleRegion().GetSize()[0] 
	<< " " << m_InputLeftImage->GetLargestPossibleRegion().GetSize()[1] << std::endl ; 
	std::cout << "Taille principal image : " << m_PrincipalViewBeforeClassif->sizeX()
	<< " " << m_PrincipalViewBeforeClassif->sizeY() << std::endl;*/
	
	/*m_ShrinkImageFilter->SetShrinkFactors(0);
        m_ShrinkImageFilter->SetInput( m_InputImage );
        m_ShrinkImageFilter->Update();
        m_PrincipalImage = m_ShrinkImageFilter->GetOutput();*/
	
		m_PrincipalViewBeforeClassif->SetClickable(true);
        m_PrincipalViewBeforeClassif->SetInput( m_PrincipalLeftImage );
        m_PrincipalViewBeforeClassif->SetDoubleWindow( iviewWindowPrincipal );
        m_PrincipalViewBeforeClassif->SetViewer( this );
        m_PrincipalViewBeforeClassif->SetLabel( m_Label + m_SuffixPrincipalTitle );
  		m_PrincipalViewBeforeClassif->Show();
	
        // Fenetre d'affichage APRES classif
        //----------------------------------------
        m_PrincipalViewAfterClassif->SetClickable(false);
		m_PrincipalViewAfterClassif->SetInput( m_PrincipalRightImage );
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
        SetLabel("Application classification supervisée");
}



template <class TPixel>
void
ImageViewerAS<TPixel>
::SetLeftImage(itk::ImageBase<2> * img)
{
  m_InputLeftImage = dynamic_cast<ImageType *>( img );
  this->Modified();
}

template <class TPixel>
void
ImageViewerAS<TPixel>
::SetRightImage(itk::ImageBase<2> * img)
{
  m_InputRightImage = dynamic_cast<ImageType *>( img );
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
::GenerateLeftChannelsInformations(void)
{

        m_LeftChannelsWorks.clear();
        
        //Controle que l'utilisateur a bien selectionne de canaux coherent
        if( m_LeftGrayLevelChannel != -1 )
        {
                if( (m_LeftRedChannel != -1 ) || (m_LeftGreenChannel != -1 ) || (m_LeftBlueChannel != -1 ) )
                {
                        itkExceptionMacro(<< "Vous avez selectionne un canal pour le niveau de gris, mais aussi un (ou des) canaux en RGB" );
                }
               m_LeftModeView = ImageViewBaseType::GRAY_LEVEL;
        }
        else
        {
               m_LeftModeView = ImageViewBaseType::RGB_LEVEL;
               if( (m_LeftRedChannel == -1 ) && (m_LeftGreenChannel == -1 ) && (m_LeftBlueChannel == -1 ) )
               {
                        //itkExceptionMacro(<< "Aucun canal n'a ete selectionne" );
                        // On les choisi automatiquement,si c'est possible
                        int nbCanauxInputImage = m_PrincipalLeftImage->GetNumberOfComponentsPerPixel();
                       						
                        switch ( nbCanauxInputImage )
                        {
                                case 1 :
									    m_LeftGrayLevelChannel = 1;
                                        m_LeftModeView = ImageViewBaseType::GRAY_LEVEL;
                                        break;
                                default :
                                case 3 : 
                                        m_LeftBlueChannel = 3;
                                        m_LeftModeView = ImageViewBaseType::RGB_LEVEL;
                                case 2 : 
                                        m_LeftGreenChannel = 2;
                                        m_LeftRedChannel = 1;
                                        m_LeftModeView = ImageViewBaseType::RGB_LEVEL;
                                        break;
                        }
			    }
               
        }
        // On selectionne dans "m_ChannelsWorks" que les canaux valide, a lire dans l'image d'entree
        switch( m_LeftModeView )
        {
                case ImageViewBaseType::GRAY_LEVEL :  m_LeftChannelsWorks.push_back( m_LeftGrayLevelChannel );
                        break;
                case ImageViewBaseType::RGB_LEVEL :							    
                                        if ( m_LeftRedChannel != -1 ) m_LeftChannelsWorks.push_back( m_LeftRedChannel );
                                        if ( m_LeftGreenChannel != -1 ) m_LeftChannelsWorks.push_back( m_LeftGreenChannel );
                                        if ( m_LeftBlueChannel != -1 ) m_LeftChannelsWorks.push_back( m_LeftBlueChannel );
                        break;
        
        }
			
        //Controle que les canaux selection existe bien dans l'image d'entree
        for ( int nbChannels = 0 ; nbChannels < m_LeftChannelsWorks.size() ; nbChannels++)
        {
                if ( (m_LeftChannelsWorks[nbChannels] <= 0) ||
				(m_LeftChannelsWorks[nbChannels] > m_PrincipalLeftImage->GetNumberOfComponentsPerPixel()) )
                {
                       	itkExceptionMacro(<< "L'image d'entree possede"
					                      <<m_PrincipalLeftImage->GetNumberOfComponentsPerPixel()
										  << "\n Vous devez selectionner des canaux dans [1..."<<m_PrincipalLeftImage->GetNumberOfComponentsPerPixel()<<"]." );
                }
        }
        m_LeftNbDim = (int)m_LeftModeView;
}



template <class TPixel>
void 
ImageViewerAS<TPixel>
::GenerateRightChannelsInformations(void)
{

        m_RightChannelsWorks.clear();
        
        //Controle que l'utilisateur a bien selectionne de canaux coherent
        if( m_RightGrayLevelChannel != -1 )
        {
                if( (m_RightRedChannel != -1 ) || (m_RightGreenChannel != -1 ) || (m_RightBlueChannel != -1 ) )
                {
                        itkExceptionMacro(<< "Vous avez selectionne un canal pour le niveau de gris, mais aussi un (ou des) canaux en RGB" );
                }
               m_RightModeView = ImageViewBaseType::GRAY_LEVEL;
        }
        else
        {
               m_RightModeView = ImageViewBaseType::RGB_LEVEL;
               if( (m_RightRedChannel == -1 ) && (m_RightGreenChannel == -1 ) && (m_RightBlueChannel == -1 ) )
               {
                        //itkExceptionMacro(<< "Aucun canal n'a ete selectionne" );
                        // On les choisi automatiquement,si c'est possible
                        int nbCanauxInputImage = m_PrincipalRightImage->GetNumberOfComponentsPerPixel();
                     						
                        switch ( nbCanauxInputImage )
                        {
                                case 1 :
									    m_RightGrayLevelChannel = 1;
                                        m_RightModeView = ImageViewBaseType::GRAY_LEVEL;
                                        break;
                                default :
                                case 3 : 
                                        m_RightBlueChannel = 3;
                                        m_RightModeView = ImageViewBaseType::RGB_LEVEL;
                                case 2 : 
                                        m_RightGreenChannel = 2;
                                        m_RightRedChannel = 1;
                                        m_RightModeView = ImageViewBaseType::RGB_LEVEL;
                                        break;
                        }
			    }
               
        }
        // On selectionne dans "m_ChannelsWorks" que les canaux valide, a lire dans l'image d'entree
        switch( m_RightModeView )
        {
                case ImageViewBaseType::GRAY_LEVEL :  m_RightChannelsWorks.push_back( m_RightGrayLevelChannel );
                        break;
                case ImageViewBaseType::RGB_LEVEL :							    
                                        if ( m_RightRedChannel != -1 ) m_RightChannelsWorks.push_back( m_RightRedChannel );
                                        if ( m_RightGreenChannel != -1 ) m_RightChannelsWorks.push_back( m_RightGreenChannel );
                                        if ( m_RightBlueChannel != -1 ) m_RightChannelsWorks.push_back( m_RightBlueChannel );
                        break;
        
        }
			
        //Controle que les canaux selection existe bien dans l'image d'entree
        for ( int nbChannels = 0 ; nbChannels < m_RightChannelsWorks.size() ; nbChannels++)
        {
                if ( (m_RightChannelsWorks[nbChannels] <= 0) ||
				(m_RightChannelsWorks[nbChannels] > m_PrincipalRightImage->GetNumberOfComponentsPerPixel()) )
                {
                       	itkExceptionMacro(<< "L'image d'entree possede"
					                      <<m_PrincipalRightImage->GetNumberOfComponentsPerPixel()
										  << "\n Vous devez selectionner des canaux dans [1..."<<m_PrincipalLeftImage->GetNumberOfComponentsPerPixel()<<"]." );
                }
        }
        m_RightNbDim = (int)m_RightModeView;
}




template <class TPixel>
void 
ImageViewerAS<TPixel>::
SetChannelsInformationsToImagesView(void)
{
	m_PrincipalViewBeforeClassif->SetChannelsWorks(m_LeftChannelsWorks);
	m_PrincipalViewBeforeClassif->SetModeView(m_LeftModeView);
	m_PrincipalViewBeforeClassif->SetNbDim(m_LeftNbDim);
    m_PrincipalViewBeforeClassif->SetGrayLevelChannel(m_LeftGrayLevelChannel);
    m_PrincipalViewBeforeClassif->SetRGBChannels(m_LeftRedChannel,m_LeftGreenChannel,m_LeftBlueChannel);
	
	m_PrincipalViewAfterClassif->SetChannelsWorks(m_RightChannelsWorks);
	m_PrincipalViewAfterClassif->SetModeView(m_RightModeView);
	m_PrincipalViewAfterClassif->SetNbDim(m_RightNbDim);
    m_PrincipalViewAfterClassif->SetGrayLevelChannel(m_RightGrayLevelChannel);
    m_PrincipalViewAfterClassif->SetRGBChannels(m_RightRedChannel,m_RightGreenChannel,m_RightBlueChannel);
}


template <class TPixel>
void 
ImageViewerAS<TPixel>::
ClearSelectChannels(void)
{ 
        m_LeftRedChannel            = -1;
        m_LeftGreenChannel          = -1;
        m_LeftBlueChannel           = -1;
        m_LeftGrayLevelChannel      = -1;
        m_LeftChannelsWorks.clear();
		m_RightRedChannel            = -1;
        m_RightGreenChannel          = -1;
        m_RightBlueChannel           = -1;
        m_RightGrayLevelChannel      = -1;
        m_RightChannelsWorks.clear();
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
	m_PrincipalViewBeforeClassif->LearnStep();
}

template <class TPixel>
void 
ImageViewerAS<TPixel>::ClassificationAlgorythm()
{
	m_PrincipalViewAfterClassif->ClassificationStep();
}

template <class TPixel>
void 
ImageViewerAS<TPixel>::DisplayFirstClass()
{
	m_PrincipalViewAfterClassif->ActivateOverlay(true);
	this->Update();  
}

template <class TPixel>
void 
ImageViewerAS<TPixel>::DisplaySecondClass()
{
	m_PrincipalViewAfterClassif->ActivateOverlay(false);
	this->Update();  
}

template <class TPixel>
void 
ImageViewerAS<TPixel>::LoadLeftImage()
{
	const char* filename = fl_file_chooser("Pick an image file", "*.*",
	".");
	if(filename == NULL || strlen(filename)<1)
    {
        return ;
    }
    
	typedef otb::ImageViewerAS<unsigned char>    ViewerType;
    typedef ViewerType::ImageType ImageType;
    typedef otb::ImageFileReader< ImageType > VolumeReaderType;

    VolumeReaderType::Pointer lReader = VolumeReaderType::New();
    lReader->SetFileName(filename);
    lReader->Update();
    SetLabel( "Label" );
    SetLeftImage( lReader->GetOutput() );
	this->Update();    
}

template <class TPixel>
void 
ImageViewerAS<TPixel>::LoadRightImage()
{
	const char* filename = fl_file_chooser("Pick an image file", "*.*",
	 ".");
	if(filename == NULL || strlen(filename)<1)
    {
        return ;
    }
	
	typedef otb::ImageViewerAS<unsigned char>    ViewerType;
    typedef ViewerType::ImageType ImageType;
	
    typedef otb::ImageFileReader< ImageType > VolumeReaderType;
    VolumeReaderType::Pointer lReader = VolumeReaderType::New();
    lReader->SetFileName(filename);
    lReader->Update();
		
    SetLabel( "Label" );
    SetRightImage( lReader->GetOutput() );  
	this->Update();    
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

