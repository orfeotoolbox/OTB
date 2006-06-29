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


