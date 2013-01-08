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
#include "itkMacro.h"

#include "otbImageFileReader.h"
#include "otbImage.h"
#include "otbStandardWriterWatcher.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "otbImageFileWriter.h"
#include "otbImageFileWriter.h"

int otbStandardWriterWatcher(int argc, char * argv[])
{
  const char *       infname = argv[1];
  const char *       outfname = argv[2];
  const unsigned int nbsd = atoi(argv[3]);

  const unsigned int Dimension = 2;
  typedef unsigned char                                           PixelType;
  typedef otb::Image<PixelType, Dimension>                        ImageType;
  typedef otb::ImageFileReader<ImageType>                         ReaderType;
  typedef itk::GradientMagnitudeImageFilter<ImageType, ImageType> FilterType;
  typedef otb::ImageFileWriter<ImageType>                StreamingWriterType;
  typedef otb::ImageFileWriter<ImageType>                         WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  FilterType::Pointer gradient = FilterType::New();

  gradient->SetInput(reader->GetOutput());

  StreamingWriterType::Pointer writer1 = StreamingWriterType::New();
  writer1->SetNumberOfDivisionsStrippedStreaming(nbsd);
  writer1->SetInput(gradient->GetOutput());
  writer1->SetFileName(outfname);

  typedef otb::StandardWriterWatcher WatcherType;
  WatcherType watcher1(writer1, "Gradient (streaming)");

//  otb::StandardWriterWatcher watcher3(writer1, gradient, "Gradient");
//
//  otb::StandardWriterWatcher watcher4(watcher1);
//
//  otb::StandardWriterWatcher watcher5 = watcher1;

  writer1->Update();

  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetInput(gradient->GetOutput());
  writer2->SetFileName(outfname);

  otb::StandardWriterWatcher watcher2(writer2, "Gradient (non streaming)");
  writer2->Update();

  // Test copy constructor.
  WatcherType watcher3( watcher1 );
  if ( watcher1.GetNameOfClass() != watcher3.GetNameOfClass()
       || watcher1.GetProcess() != watcher3.GetProcess()
       || watcher1.GetComment() != watcher3.GetComment() )
    {
    std::cout << "Copy constructor failed." << std::endl;
    return EXIT_FAILURE;
    }

  // Test default constructor.
  WatcherType watcher4;

  // Test assignment operator.
  watcher4 = watcher3;
  if ( watcher4.GetNameOfClass() != watcher3.GetNameOfClass()
       || watcher4.GetProcess() != watcher3.GetProcess()
       || watcher4.GetComment() != watcher3.GetComment() )
    {
    std::cout << "Operator= failed." << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
