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
#include "otbRCC8Edge.h"
#include <cstdlib>

int otbRCC8Edge(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::RCC8Edge               RCC8EdgeType;
  typedef RCC8EdgeType::RCC8ValueType RCC8ValueType;
  RCC8ValueType value = otb::OTB_RCC8_DC;

  // Instantiation
  RCC8EdgeType::Pointer edge = RCC8EdgeType::New();
  edge->SetValue(value);

  if (edge->GetValue() != value)
    {
    std::cout << "Test failed: edge->GetValue()!=value" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
