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

#include "otbScalarBufferToImageFileWriter.h"

int otbImageWidgetImageDump(int argc, char * argv[])
{
  const char * infname    = argv[1];
  const bool   useAccel   = atoi(argv[2]);
  const char * filename   = argv[3];


  // typedefs
  typedef otb::ImageWidget<>                                               WidgetType;
  typedef WidgetType::InputImageType                                       ImageType;
  typedef otb::ImageFileReader<ImageType>                                  ReaderType;
  typedef otb::ScalarBufferToImageFileWriter<unsigned char, unsigned char> DumperType;

  // Reading the input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->Update();
  ImageType::RegionType region = reader->GetOutput()->GetLargestPossibleRegion();
  ImageType::RegionType::SizeType size = region.GetSize();

  if( argc == 5 )
    {
    size[0] /= atoi(argv[4]);
    size[1] /= atoi(argv[4]);
    }
  region.SetSize(size);

  // Create a widget
  WidgetType::Pointer widget = WidgetType::New();
  // Set the acceleration mode
  widget->SetUseGlAcceleration(useAccel);

  // Resize it
  widget->resize(0, 0, region.GetSize()[0], region.GetSize()[1]);
  // Show it
  widget->show();
  // Refresh display
  Fl::check();

  // Read the OpenGl buffer
  widget->ReadBuffer(reader->GetOutput(), region);
  // Redraw it
  widget->redraw();
  // Refresh display
  Fl::check();

 // Dump image
  DumperType::Pointer dumper = DumperType::New();

  dumper->SetNumberOfChannels(3);
  dumper->SetBuffer( widget->GetOpenGlBuffer() );
  dumper->SetImageSize(widget->GetOpenGlBufferedRegion().GetSize());
  dumper->SetFileName(filename);
  dumper->SetInverseXSpacing(true);
  dumper->Update();

  return EXIT_SUCCESS;
}
