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
#include "otbVcaImageFilter.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

const unsigned int Dimension = 2;
typedef double PixelType;
typedef double PrecisionType;

typedef otb::Image<PixelType, Dimension> ImageType;
typedef otb::VectorImage<PixelType, Dimension> VectorImageType;
typedef otb::VCAImageFilter<VectorImageType> VCAFilterType;

typedef otb::ImageFileReader<VectorImageType> ReaderType;
typedef otb::ImageFileWriter<VectorImageType> WriterType;

int otbVCAImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  VCAFilterType::Pointer vca = VCAFilterType::New();
  std::cout << vca << std::endl;
  return EXIT_SUCCESS;
}

int otbVCAImageFilterTestHighSNR(int itkNotUsed(argc), char * argv[])
{
  const char * inputImage = argv[1];
  const char * outputImage = argv[2];
  const unsigned int nbEndmembers = atoi(argv[3]);

  ReaderType::Pointer readerImage = ReaderType::New();
  readerImage->SetFileName(inputImage);

  VCAFilterType::Pointer vca = VCAFilterType::New();
  vca->SetNumberOfEndmembers(nbEndmembers);
  vca->SetInput(readerImage->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImage);
  writer->SetInput(vca->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
