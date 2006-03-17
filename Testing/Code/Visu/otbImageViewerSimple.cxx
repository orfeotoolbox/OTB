/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   11 janvier 2005
  Version   :   
  Role      :   
  $Id$

=========================================================================*/
#include <stdlib.h>
#include "itkExceptionObject.h"

#include "itkImageFileReader.h"
#include "otbVectorImageViewerSimple.h"

int main( int argc, char ** argv )
{
  try 
    { 
        const char * inputFilename  = argv[1];

        typedef otb::VectorImageViewerSimple<unsigned char,double>    ViewerType;
        ViewerType::Pointer  lViewer = ViewerType::New();

        typedef ViewerType::ImageType ImageType;
        typedef itk::ImageFileReader< ImageType > VolumeReaderType;
        VolumeReaderType::Pointer       lReader = VolumeReaderType::New();
        lReader->SetFileName(inputFilename);
        lReader->Update();
        std::cout << " GetNumberOfComponentsPerPixel = "<<lReader->GetOutput()->GetNumberOfComponentsPerPixel()<<std::endl;


        lViewer->SetLabel( "Input Image" );
        lViewer->SetImage( lReader->GetOutput() );  
//        lViewer->SetGrayLevelChannel(1);
        lViewer->SetRGBChannels(1,2,3);
//        lViewer->SetRedChannel(3);
//        lViewer->SetGreenChannel(2);
//        lViewer->SetBlueChannel(1);
//        lViewer->Update();
        lViewer->Show();
        
        Fl::run();

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


