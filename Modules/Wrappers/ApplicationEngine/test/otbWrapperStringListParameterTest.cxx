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

#include "otbWrapperStringListParameter.h"



int otbWrapperStringListParameterTest1(int itkNotUsed(argc), char* argv[])
{
  typedef otb::Wrapper::StringListParameter StringListParameterType;
  StringListParameterType::Pointer numParam = StringListParameterType::New();

  const std::string value1 = argv[1];
  const std::string value2 = argv[2];
  const std::string value3 = argv[3];
  const std::string key    = argv[4];
  const std::string desc   = argv[5];

  numParam->AddString(value1);
  StringListParameterType::StringListType sList;
  sList.push_back(value2);
  sList.push_back(value3);
  numParam->SetValue( sList );
  numParam->SetKey(key);
  numParam->SetDescription(desc);

  if( numParam->GetValue()[0] != value2 )
    {
      return EXIT_FAILURE;
    }

  if( numParam->GetNthElement(1) != value3 )
    {
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
