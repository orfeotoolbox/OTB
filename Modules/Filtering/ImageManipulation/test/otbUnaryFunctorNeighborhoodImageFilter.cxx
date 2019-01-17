/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "otbUnaryFunctorNeighborhoodImageFilter.h"
#include "otbImage.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

namespace Functor
{
template <class TIter, class TOutput>
class UnaryFunctorNeighborhoodImageFilterTest
{
public:
  UnaryFunctorNeighborhoodImageFilterTest() {}
  ~UnaryFunctorNeighborhoodImageFilterTest() {}

  inline TOutput operator ()(const TIter& it)
  {
    return (static_cast<TOutput>(it.GetCenterPixel()));

  }
};
}

int otbUnaryFunctorNeighborhoodImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];

  typedef unsigned char InputPixelType;
  const int Dimension = 2;
  typedef otb::Image<InputPixelType, Dimension> ImageType;
  typedef ImageType::PixelType                  PixelType;
  typedef otb::ImageFileReader<ImageType>       ReaderType;
  typedef otb::ImageFileWriter<ImageType>       WriterType;

  typedef itk::ConstNeighborhoodIterator<ImageType>                             IterType;
  typedef Functor::UnaryFunctorNeighborhoodImageFilterTest<IterType, PixelType> FunctorType;
  typedef otb::UnaryFunctorNeighborhoodImageFilter<ImageType, ImageType,
      FunctorType> UnaryFunctorNeighborhoodImageFilterType;

  // Instantiating object
  UnaryFunctorNeighborhoodImageFilterType::Pointer object = UnaryFunctorNeighborhoodImageFilterType::New();
  ReaderType::Pointer                              reader  = ReaderType::New();
  WriterType::Pointer                              writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);

  object->SetInput(reader->GetOutput());
  object->SetRadius(atoi(argv[3]));
  writer->SetInput(object->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
