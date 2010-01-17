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
#include "itkExceptionObject.h"
#include "otbImageFileReader.h"
#include "otbImageViewer.h"
#include "otbMacro.h"

int otbDoubleImageViewerWithLink( int argc, char * argv[] )
{
  char * filename1 = argv[1];
  char * filename2 = argv[2];

  // Parse command line parameters
  typedef double PixelType;
  typedef otb::ImageViewer<PixelType>  ImageViewerType;
  typedef ImageViewerType::OffsetType OffsetType;
  typedef ImageViewerType::ImageType ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;

  // instantiation
  ImageViewerType::Pointer viewer1 = ImageViewerType::New();
  ImageViewerType::Pointer viewer2 = ImageViewerType::New();

  // check for input images
  ReaderType::Pointer reader1 = ReaderType::New();
  reader1->SetFileName(filename1);
  reader1->GenerateOutputInformation();
  viewer1->SetImage(reader1->GetOutput());

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(filename2);
  reader2->GenerateOutputInformation();
  viewer2->SetImage(reader2->GetOutput());

  // CASE 1: Simple link
  viewer1->Link(viewer2);
  viewer1->Show();
  viewer2->Show();
  Fl::check();

  // CASE 2: Unlink
  viewer1->Unlink(viewer2);
  viewer1->Show();
  viewer2->Show();
  Fl::check();

  // CASE 3: Link with Offset
  OffsetType offset;
  offset.Fill(50);
  viewer2->Link(viewer1,offset);
  viewer1->Show();
  viewer2->Show();
  Fl::check();

  return EXIT_SUCCESS;
}


