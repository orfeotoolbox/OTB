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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbWrapperParameter.h"

int otbWrapperParameterNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef otb::Wrapper::Parameter ParameterBaseType;
  ParameterBaseType::Pointer parameter = ParameterBaseType::New();

  //std::cout << parameter << std::endl;

  return EXIT_SUCCESS;
}

int otbWrapperParameterTest1(int itkNotUsed(argc), char* argv[])
{
  typedef otb::Wrapper::Parameter ParameterBaseType;
  ParameterBaseType::Pointer parameter = ParameterBaseType::New();

  const std::string name = argv[1];

  parameter->SetName(name);

  if (name == parameter->GetName())
    {
    return EXIT_SUCCESS;
    }
  else
    {
    return EXIT_FAILURE;
    }
}
