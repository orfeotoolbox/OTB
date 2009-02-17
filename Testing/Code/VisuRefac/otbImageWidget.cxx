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
#define OTB_DISABLE_GL_USE_ACCEL
#include "otbImageWidget.h"
#include "otbImageFileReader.h"
#include <FL/Fl.H>

int otbImageWidget( int argc, char * argv[] )
{
  const char * infname   =      argv[1];
  const bool   useAccel  = atoi(argv[2]);

  // typedefs
  typedef otb::ImageWidget<> WidgetType;
  typedef WidgetType::InputImageType ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  
  // Reading the input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->Update();
  ImageType::RegionType region = reader->GetOutput()->GetLargestPossibleRegion();

  // Create a widget
  WidgetType::Pointer widget = WidgetType::New();
  // Set the acceleration mode
  widget->SetUseGlAcceleration(useAccel);

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
  // Read only a quater of the image
  ImageType::RegionType::SizeType size = region.GetSize();
  size[0]/=2;
  region.SetSize(size);
   // Read the OpenGl buffer
  widget->ReadBuffer(reader->GetOutput(),region);
  // Redraw it
  widget->redraw();
  // Refresh display
  Fl::check();
  // Adding an offset
  ImageType::RegionType::IndexType index = region.GetIndex();
  index[0]+=size[0];
  region.SetIndex(index);
  // Read the OpenGl buffer
  widget->ReadBuffer(reader->GetOutput(),region);
  // Redraw it
  widget->redraw();
  // Refresh display
  Fl::check();
  // Reading full image, but in half window
  region = reader->GetOutput()->GetLargestPossibleRegion();
  widget->resize(0,0,region.GetSize()[0],region.GetSize()[1]/2);
  // Read the OpenGl buffer
  widget->ReadBuffer(reader->GetOutput(),region);
  // Redraw it
  widget->redraw();
  // Refresh display
  Fl::check();
  // Reading full image, but in zoomed in widget
  widget->resize(0,0,region.GetSize()[0],region.GetSize()[1]);
  widget->SetIsotropicZoom(2.);
  // Read the OpenGl buffer
  widget->ReadBuffer(reader->GetOutput(),region);
  // Redraw it
  widget->redraw();
  // Refresh display
  Fl::check();
  // Reading full image, but in zoomed out widget
  widget->resize(0,0,region.GetSize()[0],region.GetSize()[1]);
  widget->SetIsotropicZoom(0.5);
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
