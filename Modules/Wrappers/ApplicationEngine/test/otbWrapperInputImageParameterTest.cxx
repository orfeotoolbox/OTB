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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbImage.h"
#include "otbWrapperInputImageParameter.h"


int otbWrapperInputImageParameterTest1(int, char* argv[])
{
  auto param = otb::Wrapper::InputImageParameter::New();
  param->SetFromFileName(argv[1]);
  param->SetKey(argv[2]);
  param->SetDescription(argv[3]);

  /*
  Originally reported on gitlab as bug #1899:
  https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/issues/1899
  */
  using ImageType = otb::Image<float, 2>;
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
