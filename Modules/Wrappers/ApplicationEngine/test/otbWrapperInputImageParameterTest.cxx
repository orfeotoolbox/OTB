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

#include "otbWrapperInputImageParameter.h"


int otbWrapperInputImageParameterTest1(int itkNotUsed(argc), char* argv[])
{
  typedef otb::Wrapper::InputImageParameter InputImageParameterType;
  InputImageParameterType::Pointer param = InputImageParameterType::New();

  param->SetFromFileName(argv[1]);
  param->SetKey(argv[2]);
  param->SetDescription(argv[3]);

  return EXIT_SUCCESS;
}
