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

#include "otbWrapperStringParameter.h"


int otbWrapperStringParameterTest1(int itkNotUsed(argc), char* argv[])
{
  typedef otb::Wrapper::StringParameter StringParameterType;
  StringParameterType::Pointer          numParam = StringParameterType::New();

  const std::string value = argv[1];
  const std::string key   = argv[2];
  const std::string desc  = argv[3];

  numParam->SetValue(value);
  numParam->SetKey(key);
  numParam->SetDescription(desc);

  return EXIT_SUCCESS;
}
