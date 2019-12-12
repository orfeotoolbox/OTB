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


#include "otbStreamingTraits.h"

#include "otbImage.h"

#include <iostream>

const unsigned int Dimension = 2;
typedef double     PixelType;

template <class TInterpolatorType>
void TestInstantiate()
{
  typename TInterpolatorType::Pointer instance = TInterpolatorType::New();
  std::cout << instance << std::endl;
}

int otbStreamingTraitsImage(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  // Images definition
  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::StreamingTraits<ImageType> StreamingTraits;

  // Check that all internal types can be instantiated
  TestInstantiate<StreamingTraits::NearestNeighborInterpolationType>();
  TestInstantiate<StreamingTraits::LinearInterpolationType>();
  TestInstantiate<StreamingTraits::BSplineInterpolationType>();
  TestInstantiate<StreamingTraits::GaussianInterpolationType>();
  TestInstantiate<StreamingTraits::CosineInterpolationType>();
  TestInstantiate<StreamingTraits::HammingInterpolationType>();
  TestInstantiate<StreamingTraits::WelchInterpolationType>();
  TestInstantiate<StreamingTraits::LanczosInterpolationType>();
  TestInstantiate<StreamingTraits::BlackmanInterpolationType>();
  TestInstantiate<StreamingTraits::ProlateInterpolationType>();
  TestInstantiate<StreamingTraits::BCOInterpolationType>();

  return EXIT_SUCCESS;
}

int otbStreamingTraitsVectorImage(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  // Images definition
  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  typedef otb::StreamingTraits<ImageType> StreamingTraits;

  // Check that all internal types can be instantiated
  TestInstantiate<StreamingTraits::NearestNeighborInterpolationType>();
  TestInstantiate<StreamingTraits::LinearInterpolationType>();
  TestInstantiate<StreamingTraits::GaussianInterpolationType>();
  TestInstantiate<StreamingTraits::BCOInterpolationType>();

  return EXIT_SUCCESS;
}
