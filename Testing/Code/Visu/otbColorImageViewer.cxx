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

#include "otbImageFileReader.h"
#include "itkImage.h"
#include "itkRGBPixel.h"

#include "otbColorImageViewer.h"

int main( int argc, char ** argv )
{
  try 
    { 
        const char * inputFilename  = argv[1];

        typedef otb::ColorImageViewer<unsigned char,double>    ViewerType;
        ViewerType                      lViewer;

        typedef itk::Image< itk::RGBPixel<unsigned char>, 3 > ImageType;
        typedef otb::ImageFileReader< ImageType > VolumeReaderType;
        VolumeReaderType::Pointer       lReader = VolumeReaderType::New();
        

        lReader->SetFileName(inputFilename);
        lReader->Update();
        lViewer.SetLabel( "Input Image" );
        lViewer.SetImage( lReader->GetOutput() );  
//        lViewer.Update();
        lViewer.Show();
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


