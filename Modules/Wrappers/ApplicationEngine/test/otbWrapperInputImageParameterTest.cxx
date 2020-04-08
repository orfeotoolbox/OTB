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

#if defined(_MSC_VER)
#pragma warning(disable : 4786)
#endif

#include "otbImage.h"
#include "otbWrapperInputImageParameter.h"

// Test image case, expect the same pointer for two calls with the same type
int otbWrapperInputImageParameterTest1(int, char* argv[])
{
  auto param = otb::Wrapper::InputImageParameter::New();
  param->SetKey(argv[2]);
  param->SetDescription(argv[3]);

  /*
  Originally reported on gitlab as bug #1899:
  https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/issues/1899
  */
  using ImageType       = otb::Image<float, 2>;
  using ImageTypeOutput = otb::Image<unsigned char, 2>;

  auto image = ImageType::New();
  param->SetImage(image.GetPointer());

  auto output1 = param->GetImage<ImageTypeOutput>();
  auto output2 = param->GetImage<ImageTypeOutput>();

  if (output1 != output2)
  {
    std::cerr << "InputImageParameter calls to ->GetImage<> are not consistent.\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

// Test image case, expect an exception for two calls with different types
int otbWrapperInputImageParameterTest2(int, char* [])
{
  using ImageType  = otb::Image<float, 2>;
  using ImageTypeA = otb::Image<unsigned char, 2>;
  using ImageTypeB = otb::Image<double, 2>;

  auto image = ImageType::New();
  auto param = otb::Wrapper::InputImageParameter::New();
  param->SetImage(image.GetPointer());

  try
  {
    param->GetImage<ImageTypeA>();
    param->GetImage<ImageTypeB>();
    param->GetImage<ImageTypeA>();
  }
  catch (itk::ExceptionObject&)
  {
    std::cerr << "Expected exception when calling GetImage() with different types.\n";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

// Filename case, expect the same pointer for two calls with the same type
int otbWrapperInputImageParameterTest3(int, char* argv[])
{
  auto param = otb::Wrapper::InputImageParameter::New();
  param->SetFromFileName(argv[1]);
  param->SetKey(argv[2]);
  param->SetDescription(argv[3]);

  using ImageTypeOutput = otb::Image<unsigned char, 2>;

  auto output1 = param->GetImage<ImageTypeOutput>();
  auto output2 = param->GetImage<ImageTypeOutput>();

  if (output1 != output2)
  {
    std::cerr << "InputImageParameter calls to ->GetImage<> are not consistent.\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

// Filename case, expect an exception for two calls with different types
int otbWrapperInputImageParameterTest4(int, char* argv[])
{
  using ImageTypeA = otb::Image<unsigned char, 2>;
  using ImageTypeB = otb::Image<double, 2>;

  auto param = otb::Wrapper::InputImageParameter::New();
  param->SetFromFileName(argv[1]);
  param->SetKey(argv[2]);
  param->SetDescription(argv[3]);

  try
  {
    param->GetImage<ImageTypeA>();
    param->GetImage<ImageTypeB>();
    param->GetImage<ImageTypeA>();
  }
  catch (itk::ExceptionObject&)
  {
    std::cerr << "Expected exception when calling GetImage() with different types.\n";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int otbWrapperInputImageParameterTest(int argc, char* argv[])
{
  return otbWrapperInputImageParameterTest1(argc, argv) && otbWrapperInputImageParameterTest2(argc, argv) && otbWrapperInputImageParameterTest3(argc, argv) &&
         otbWrapperInputImageParameterTest4(argc, argv);
}
