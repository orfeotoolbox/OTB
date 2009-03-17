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
#include "otbStandardImageViewer.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"

int otbStandardImageViewer( int argc, char * argv[] )
{
  bool run = atoi(argv[2]);

  typedef otb::VectorImage<double,2> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::StandardImageViewer<ImageType> ViewerType;

  ReaderType::Pointer reader = ReaderType::New();
  ViewerType::Pointer viewer = ViewerType::New();

  reader->SetFileName(argv[1]);

  viewer->SetImage(reader->GetOutput());
  viewer->Update();
  
  if(run)
    {
    Fl::run();
    }
  else
    {
    Fl::check();
    }

  return EXIT_SUCCESS;
}
