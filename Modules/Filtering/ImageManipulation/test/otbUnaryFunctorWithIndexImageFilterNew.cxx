/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
#include "otbImage.h"

namespace Functor
{
template <class TInput, class TOutput>
class UnaryFunctorWithIndexImageFilterFunctorNewTest
{
public:
  UnaryFunctorWithIndexImageFilterFunctorNewTest() {}
  ~UnaryFunctorWithIndexImageFilterFunctorNewTest() {}

  typedef itk::Index<2> IndexType;

  inline TOutput operator ()(const TInput& inPix, IndexType itkNotUsed(index))
  {
    return (static_cast<TOutput>(inPix));
  }
};
}

int otbUnaryFunctorWithIndexImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef double                                                                        InputPixelType;
  typedef otb::Image<InputPixelType, 2>                                                 ImageType;
  typedef ImageType::PixelType                                                          PixelType;
  typedef Functor::UnaryFunctorWithIndexImageFilterFunctorNewTest<PixelType, PixelType> FunctorType;
  typedef otb::UnaryFunctorWithIndexImageFilter<ImageType, ImageType,
      FunctorType>      UnaryFunctorWithIndexImageFilterType;

  // Instantiating object
  UnaryFunctorWithIndexImageFilterType::Pointer filter = UnaryFunctorWithIndexImageFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
