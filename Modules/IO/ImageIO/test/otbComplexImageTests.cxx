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


#include "itkMacro.h"
#include <iostream>
#include <fstream>
#include <string>

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"


template <class InternalType>
int otbVectorImageComplexGenericTest(int itkNotUsed(argc), char* argv[])
{
  typedef std::complex<InternalType> PixelType;
  typedef otb::VectorImage<PixelType, 2> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typename ReaderType::Pointer            reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();
  std::cout << reader->GetOutput()->GetNumberOfComponentsPerPixel() << std::endl;
  otb::ImageIOBase::Pointer io = reader->GetImageIO();
  std::cout << io << std::endl;
  reader->Update();
  typename ImageType::IndexType index;
  index[0] = 0;
  index[1] = 0;

  typename ImageType::PixelType pixel = reader->GetOutput()->GetPixel(index);
  std::cout << pixel << std::endl;

  // Test value
  if ((pixel[0] != PixelType(0, 1)) || (pixel[1] != PixelType(20000, 20001)))
  {
    std::cout << "Found " << pixel[0] << " should be " << PixelType(0, 1) << std::endl;
    std::cout << "Found " << pixel[1] << " should be " << PixelType(20000, 20001) << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int otbVectorImageComplexFloatTest(int argc, char* argv[])
{
  return otbVectorImageComplexGenericTest<float>(argc, argv);
}

int otbVectorImageComplexDoubleTest(int argc, char* argv[])
{
  return otbVectorImageComplexGenericTest<double>(argc, argv);
}

template <class InternalType>
int otbImageComplexGenericTest(int itkNotUsed(argc), char* argv[])
{
  typedef std::complex<InternalType> PixelType;
  typedef otb::Image<PixelType, 2> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typename ReaderType::Pointer            reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();
  std::cout << reader->GetOutput()->GetNumberOfComponentsPerPixel() << std::endl;
  otb::ImageIOBase::Pointer io = reader->GetImageIO();
  std::cout << io << std::endl;
  reader->Update();
  typename ImageType::IndexType index;
  index[0] = 0;
  index[1] = 0;

  typename ImageType::PixelType pixel = reader->GetOutput()->GetPixel(index);
  std::cout << pixel << std::endl;

  // Test value
  if (pixel != PixelType(0, 1))
  {
    std::cout << "Found " << pixel << " should be " << PixelType(0, 1) << std::endl;
    return EXIT_FAILURE;
  }

  // Test other pixel
  index[0] = 10;
  index[1] = 10;

  pixel = reader->GetOutput()->GetPixel(index);
  std::cout << pixel << std::endl;

  if (pixel != PixelType(1010, 1011))
  {
    std::cout << "Found " << pixel << " should be " << PixelType(1010, 1011) << std::endl;
    return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;
}

int otbImageComplexFloatTest(int argc, char* argv[])
{
  return otbImageComplexGenericTest<float>(argc, argv);
}

int otbImageComplexDoubleTest(int argc, char* argv[])
{
  return otbImageComplexGenericTest<double>(argc, argv);
}


template <class InternalType>
int otbVectorImageComplexIntoRealGenericTest(int itkNotUsed(argc), char* argv[])
{
  typedef InternalType PixelType;
  typedef otb::VectorImage<PixelType, 2> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typename ReaderType::Pointer            reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();
  std::cout << reader->GetOutput()->GetNumberOfComponentsPerPixel() << std::endl;
  otb::ImageIOBase::Pointer io = reader->GetImageIO();
  std::cout << io << std::endl;
  reader->Update();
  typename ImageType::IndexType index;
  index[0] = 0;
  index[1] = 0;

  typename ImageType::PixelType pixel = reader->GetOutput()->GetPixel(index);
  std::cout << pixel << std::endl;

  // Test value
  if (reader->GetOutput()->GetNumberOfComponentsPerPixel() != 4)
  {
    std::cout << reader->GetOutput()->GetNumberOfComponentsPerPixel() << " bands instead of 4" << std::endl;
    return EXIT_FAILURE;
  }
  if ((pixel[0] != 0) || (pixel[1] != 1) || (pixel[2] != 20000) || (pixel[3] != 20001))
  {
    std::cout << "Found " << pixel[0] << " should be " << 0 << std::endl;
    std::cout << "Found " << pixel[1] << " should be " << 1 << std::endl;
    std::cout << "Found " << pixel[2] << " should be " << 20000 << std::endl;
    std::cout << "Found " << pixel[3] << " should be " << 20001 << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int otbVectorImageComplexIntoRealFloatTest(int argc, char* argv[])
{
  return otbVectorImageComplexIntoRealGenericTest<float>(argc, argv);
}

int otbVectorImageComplexIntoRealDoubleTest(int argc, char* argv[])
{
  return otbVectorImageComplexIntoRealGenericTest<double>(argc, argv);
}
