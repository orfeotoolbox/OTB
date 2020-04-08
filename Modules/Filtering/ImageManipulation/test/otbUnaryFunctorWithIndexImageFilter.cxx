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


#include "otbUnaryFunctorWithIndexImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

namespace Functor
{
template <class TInput, class TOutput>
class UnaryFunctorWithIndexImageFilterFunctorTest
{
public:
  UnaryFunctorWithIndexImageFilterFunctorTest()
  {
  }
  ~UnaryFunctorWithIndexImageFilterFunctorTest()
  {
  }

  typedef itk::Index<2> IndexType;

  inline TOutput operator()(const TInput& inPix, IndexType itkNotUsed(index))
  {
    return (static_cast<TOutput>(inPix));
  }
};
}

int otbUnaryFunctorWithIndexImageFilter(int itkNotUsed(argc), char* argv[])
{
  const char* inputFileName  = argv[1];
  const char* outputFileName = argv[2];

  typedef double InputPixelType;
  typedef otb::VectorImage<InputPixelType, 2> ImageType;
  typedef ImageType::PixelType            PixelType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  typedef Functor::UnaryFunctorWithIndexImageFilterFunctorTest<PixelType, PixelType> FunctorType;
  typedef otb::UnaryFunctorWithIndexImageFilter<ImageType, ImageType, FunctorType> UnaryFunctorWithIndexImageFilterType;

  // Instantiating object
  UnaryFunctorWithIndexImageFilterType::Pointer object = UnaryFunctorWithIndexImageFilterType::New();
  ReaderType::Pointer                           reader = ReaderType::New();
  WriterType::Pointer                           writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);

  object->SetInput(reader->GetOutput());
  writer->SetInput(object->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
