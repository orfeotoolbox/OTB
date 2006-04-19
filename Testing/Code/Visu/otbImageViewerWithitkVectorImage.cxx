/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CNES - J. Inglada
  Language  :   C++
  Date      :   14 March 2005
  Version   :   
  Role      :   
  $Id$

=========================================================================*/
#include <stdlib.h>
#include "itkExceptionObject.h"

#include "otbImageFileReader.h"
#include "itkImage.h"
#include "itkVectorImage.h"

#include "otbImageViewer.h"

int main( int argc, char ** argv )
{



  if( argc != 2)
    {
    std::cout <<" Usage : InputImage"<<std::endl;
    }
    
  const char * inputFilename  = argv[1];

  typedef itk::VectorImage< unsigned char, 2 > ImageType;
  typedef otb::ImageViewer<ImageType::InternalPixelType>    ViewerType;
  typedef otb::ImageFileReader< ImageType > ReaderType;    

  ViewerType::Pointer  lViewer = ViewerType::New();
  ReaderType::Pointer       lReader = ReaderType::New();
  lReader->SetFileName(inputFilename);
  lReader->Update();

  lViewer->SetLabel( "My Image" );
  lViewer->SetImage( lReader->GetOutput() );  
  lViewer->Show();

  Fl::run();

  return EXIT_SUCCESS;
}


