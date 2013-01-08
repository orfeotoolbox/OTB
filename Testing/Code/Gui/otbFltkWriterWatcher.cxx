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
#include "otbFltkWriterWatcher.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "otbImageFileWriter.h"
#include "otbImageFileWriter.h"

int otbFltkWriterWatcher(int argc, char * argv[])
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

  otb::FltkWriterWatcher watcher1(writer1, 0, 0, 200, 40, "Gradient (streaming)");
  writer1->Update();

  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetInput(gradient->GetOutput());
  writer2->SetFileName(outfname);

  otb::FltkWriterWatcher watcher2(writer2, 0, 0, 200, 40, "Gradient (non streaming)");
  writer2->Update();

  return EXIT_SUCCESS;
}
