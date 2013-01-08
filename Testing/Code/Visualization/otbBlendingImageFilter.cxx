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
#include "otbBlendingImageFilter.h"
#include "otbImage.h"
#include "itkRGBPixel.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbBlendingImageFilter(int argc, char * argv[])
{
  typedef unsigned char                            ValueType;
  typedef itk::RGBPixel<ValueType>                 PixelType;
  typedef otb::Image<PixelType, 2>                 ImageType;
  typedef otb::BlendingImageFilter<ImageType>      BlendingFilterType;
  typedef otb::ImageFileReader<ImageType>          ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  // Instantiation
  BlendingFilterType::Pointer rendering = BlendingFilterType::New();
  ReaderType::Pointer         reader1   = ReaderType::New();
  ReaderType::Pointer         reader2   = ReaderType::New();
  WriterType::Pointer         writer    = WriterType::New();

  reader1->SetFileName(argv[1]);
  reader2->SetFileName(argv[2]);
  rendering->SetInput1(reader1->GetOutput());
  rendering->SetInput2(reader2->GetOutput());
  writer->SetInput(rendering->GetOutput());
  writer->SetFileName(argv[3]);
  writer->Update();

  return EXIT_SUCCESS;
}
