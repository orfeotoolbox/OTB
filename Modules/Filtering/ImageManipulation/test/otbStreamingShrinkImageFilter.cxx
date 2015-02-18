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
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImage.h"
#include "otbStreamingShrinkImageFilter.h"

int otbStreamingShrinkImageFilter(int itkNotUsed(argc), char * argv[])
{
  char *             inputFilename = argv[1];
  char *             outputFilename = argv[2];
  unsigned int       shrinkFactor = atoi(argv[3]);
  const unsigned int Dimension = 2;

  typedef unsigned int                                          PixelType;
  typedef otb::VectorImage<PixelType, Dimension>                ImageType;
  typedef otb::ImageFileReader<ImageType>                       ReaderType;
  typedef otb::ImageFileWriter<ImageType>                       WriterType;
  typedef otb::StreamingShrinkImageFilter<ImageType, ImageType> ShrinkType;

  ReaderType::Pointer reader = ReaderType::New();
  ShrinkType::Pointer shrink = ShrinkType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);
  shrink->SetShrinkFactor(shrinkFactor);

  shrink->SetInput(reader->GetOutput());
  shrink->Update();

  writer->SetInput(shrink->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
