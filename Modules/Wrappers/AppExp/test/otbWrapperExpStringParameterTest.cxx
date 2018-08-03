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

#include "otbWrapperExpStringParameter.h"
using namespace otb::WrapperExp;

int StringParameterTest( int  , char **  )
{
  StringParameter::Pointer param (StringParameter::New());
  param->SetDefaultValue("toto");
  param->SetValue("titi");
  if (param->GetValue() != "titi")
    {
    return EXIT_FAILURE;
    }
  param->Reset();
  if (param->GetValue() != "toto")
    {
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}

