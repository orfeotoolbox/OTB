/*=========================================================================

  Program:   Monteverdi2
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

//#include "ConfigureMonteverdi.h"

//
// System includes (sorted by alphabetic order)
//#include <cstdlib>

//
// Qt includes (sorted by alphabetic order)

// Monteverdi includes (sorted by alphabetic order)
#include "mvdMainWindow.h"
#include "mvdApplication.h"

//
// OTB includes (sorted by alphabetic order)
#include "itkRGBAPixel.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
//

//
// Main functions definitions.
//

//
// MAIN
//
int
main( int argc, char* argv[] )
{
  mvd::Application application( argc, argv );
  
  // image reader 
  typedef itk::RGBAPixel<unsigned char>   PixelType;
  typedef otb::Image<PixelType, 2>        ImageType;
  typedef ImageType::RegionType           RegionType;
  
  typedef otb::ImageFileReader<ImageType>   ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();
  
  ImageType::Pointer image = reader->GetOutput();
  RegionType region = image->GetLargestPossibleRegion();

  mvd::MainWindow  main_window;// = new mvd::MainWindow;
  main_window.GetGLImageWidget()->SetIsotropicZoom(1);
  main_window.GetGLImageWidget()->ReadBuffer(image, region);
  main_window.resize( region.GetSize()[0], region.GetSize()[1] );
  main_window.show();

  return application.exec();
}

//
// Main function implementation.
//
