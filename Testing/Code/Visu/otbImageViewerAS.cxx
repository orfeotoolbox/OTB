/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - R.GARRIGUES
  Language  :   C++
  Date      :   27 avril 2006
  Version   :   
  Role      :   
  $Id$

=========================================================================*/

#include <stdlib.h>
#include <iostream>
#include "itkExceptionObject.h"

#include "otbImageFileReader.h"
#include "otbImageViewerAS.h"

int main( int argc, char ** argv )
{
  try 
    { 
        int lRunFLTK(1);
        if( argc < 2 || argc > 3 )
        {
                std::cout <<" Usage : InputImage"<<std::endl;
        }
        
        //Parametre utilise pour le lancement du test de facon automatique, sans intervention de l'utilisateur
        if( argc == 3 )
        {
                lRunFLTK = atoi(argv[2]);
        }
        
        const char * inputFilename  = argv[1];
        
        typedef otb::ImageViewerAS<unsigned char>    ViewerType;
        ViewerType::Pointer  lViewer = ViewerType::New();

        typedef ViewerType::ImageType ImageType;
        typedef otb::ImageFileReader< ImageType > VolumeReaderType;
        VolumeReaderType::Pointer       lReader = VolumeReaderType::New();
        lReader->SetFileName(inputFilename);
        lReader->Update();
	
        lViewer->SetLabel( "Label" );
        lViewer->SetImage( lReader->GetOutput() );  

//        lViewer->SetGrayLevelChannel(1);
//        lViewer->SetRGBChannels(1,2,3);
//        lViewer->SetRedChannel(3);
//        lViewer->SetGreenChannel(2);
//        lViewer->SetBlueChannel(1);
//        lViewer->Update();
	lViewer->Show();

        if( lRunFLTK == 1 )
        {
                Fl::run();
        }

    } 

  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "Exception levee inconnue !" << std::endl; 
    return EXIT_FAILURE;
    } 


  return EXIT_SUCCESS;
}


