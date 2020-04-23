/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <complex>
#include <iostream>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkMacro.h"
#include "itkImageRegionIterator.h"


/***********
 * 1.
 * Write Image<scalar> with an ImageFileWriter<scalar>
 ***********/
template <class InternalType>
int otbImageScalarFileWriterTestWithoutInputGeneric(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char* outputFilename = argv[1];

  typedef InternalType PixelType;
  typedef otb::Image<PixelType, 2> ImageType;
  typedef typename ImageType::RegionType      RegionType;
  typedef typename ImageType::SizeType        SizeType;
  typedef typename ImageType::IndexType       IndexType;
  typedef itk::ImageRegionIterator<ImageType> IteratorType;
  typedef otb::ImageFileWriter<ImageType>     WriterType;

  typename ImageType::Pointer  image  = ImageType::New();
  typename WriterType::Pointer writer = WriterType::New();

  SizeType size;
  size.Fill(atoi(argv[2]));
  IndexType orig;
  orig.Fill(0);
  RegionType region;
  region.SetSize(size);
  region.SetIndex(orig);

  typedef typename ImageType::PointType   PointType;
  typedef typename ImageType::SpacingType SpacingType;
  PointType                               origin;
  origin.Fill(0.5);
  SpacingType spacing;
  spacing.Fill(1.0);
  image->SetOrigin(origin);
  image->SetSignedSpacing(spacing);

  image->SetRegions(region);
  image->Allocate();

  // Fill to zero values
  image->FillBuffer(static_cast<PixelType>(0));

  IteratorType it(image, image->GetLargestPossibleRegion());
  it.GoToBegin();

  while (!it.IsAtEnd())
  {
    it.Set(static_cast<PixelType>(size[0] * it.GetIndex()[1] + it.GetIndex()[0]));
    ++it;
  }

  writer->SetFileName(outputFilename);
  writer->SetInput(image);
  writer->Update();


  return EXIT_SUCCESS;
}

int otbImageScalarFileWriterTestWithoutInputShort(int argc, char* argv[])
{
  return otbImageScalarFileWriterTestWithoutInputGeneric<short>(argc, argv);
}
int otbImageScalarFileWriterTestWithoutInputInt(int argc, char* argv[])
{
  return otbImageScalarFileWriterTestWithoutInputGeneric<int>(argc, argv);
}
int otbImageScalarFileWriterTestWithoutInputFloat(int argc, char* argv[])
{
  return otbImageScalarFileWriterTestWithoutInputGeneric<float>(argc, argv);
}
int otbImageScalarFileWriterTestWithoutInputDouble(int argc, char* argv[])
{
  return otbImageScalarFileWriterTestWithoutInputGeneric<double>(argc, argv);
}

/***********
 * 1.
 * Write Image<scalar> with an ImageFileWriter<scalar>
 ***********/
template <class InternalType>
int otbImageComplexFileWriterTestWithoutInputGeneric(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char* outputFilename = argv[1];

  typedef std::complex<InternalType> PixelType;
  typedef otb::Image<PixelType, 2> ImageType;
  typedef typename ImageType::RegionType      RegionType;
  typedef typename ImageType::SizeType        SizeType;
  typedef typename ImageType::IndexType       IndexType;
  typedef itk::ImageRegionIterator<ImageType> IteratorType;
  typedef otb::ImageFileWriter<ImageType>     WriterType;

  typename ImageType::Pointer  image  = ImageType::New();
  typename WriterType::Pointer writer = WriterType::New();

  SizeType size;
  size.Fill(atoi(argv[2]));
  IndexType orig;
  orig.Fill(0);
  RegionType region;
  region.SetSize(size);
  region.SetIndex(orig);

  typedef typename ImageType::PointType   PointType;
  typedef typename ImageType::SpacingType SpacingType;
  PointType                               origin;
  origin.Fill(0.5);
  SpacingType spacing;
  spacing.Fill(1.0);
  image->SetOrigin(origin);
  image->SetSignedSpacing(spacing);

  image->SetRegions(region);
  image->Allocate();

  // Fill to zero values
  image->FillBuffer(static_cast<PixelType>(0));

  IteratorType it(image, image->GetLargestPossibleRegion());
  it.GoToBegin();

  typedef typename itk::NumericTraits<PixelType>::ScalarRealType ScalarRealType;
  while (!it.IsAtEnd())
  {
    ScalarRealType realPart = 2 * (size[0] * it.GetIndex()[1] + it.GetIndex()[0]);
    ScalarRealType imagPart = 2 * (size[0] * it.GetIndex()[1] + it.GetIndex()[0]) + 1;

    PixelType pixelVal(realPart, imagPart);

    // std::cout << "pixelVal = " << pixelVal <<std::endl;
    it.Set(pixelVal);
    // std::cout << "pixelVal = " <<it.Get() <<std::endl;
    ++it;
  }

  writer->SetFileName(outputFilename);
  writer->SetInput(image);
  writer->Update();


  return EXIT_SUCCESS;
}


int otbImageComplexFileWriterTestWithoutInputShort(int argc, char* argv[])
{
  return otbImageComplexFileWriterTestWithoutInputGeneric<short>(argc, argv);
}
int otbImageComplexFileWriterTestWithoutInputInt(int argc, char* argv[])
{
  return otbImageComplexFileWriterTestWithoutInputGeneric<int>(argc, argv);
}
int otbImageComplexFileWriterTestWithoutInputFloat(int argc, char* argv[])
{
  return otbImageComplexFileWriterTestWithoutInputGeneric<float>(argc, argv);
}
int otbImageComplexFileWriterTestWithoutInputDouble(int argc, char* argv[])
{
  return otbImageComplexFileWriterTestWithoutInputGeneric<double>(argc, argv);
}
