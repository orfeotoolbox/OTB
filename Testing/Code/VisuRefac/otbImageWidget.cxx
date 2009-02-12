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
#include "otbImageWidget.h"
#include "otbImageFileReader.h"
#include <FL/Fl.H>

int otbImageWidget( int argc, char * argv[] )
{
  // typedefs
  typedef otb::ImageWidget<> WidgetType;
  typedef WidgetType::InputImageType ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  
  // Reading the input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();
  ImageType::RegionType region = reader->GetOutput()->GetLargestPossibleRegion();

  // Create a widget
  WidgetType::Pointer widget = WidgetType::New();
  // Resize it
  widget->resize(0,0,region.GetSize()[0],region.GetSize()[1]);
  // Show it
  widget->show();
  // Refresh display
  Fl::check();
  // Read the OpenGl buffer
  widget->ReadBuffer(reader->GetOutput(),region);
  // Redraw it
  widget->redraw();
  // Refresh display
  Fl::check();

  // Print out widget status
  std::cout<<widget<<std::endl;

  return EXIT_SUCCESS;
}
