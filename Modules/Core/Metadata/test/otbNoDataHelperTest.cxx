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


#include "otbNoDataHelper.h"
#include "itkMetaDataDictionary.h"
#include "otbMacro.h"

int otbNoDataHelperTest(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  std::vector<bool>   b1(1, true);
  std::vector<double> v1(1, 0);

  itk::MetaDataDictionary dict;

  otb::WriteNoDataFlags(b1, v1, dict);
  otb::ReadNoDataFlags(dict, b1, v1);

  otbControlConditionTestMacro(otb::IsNoData(10, b1, v1), " wrong output of IsNoData function");
  otbControlConditionTestMacro(!otb::IsNoData(0, b1, v1), " wrong output of IsNoData function");
  b1[0] = false;
  otbControlConditionTestMacro(otb::IsNoData(10, b1, v1), " wrong output of IsNoData function");
  otbControlConditionTestMacro(otb::IsNoData(0, b1, v1), " wrong output of IsNoData function");


  std::vector<bool>   b2(4, true);
  std::vector<double> v2(4, 1);

  itk::VariableLengthVector<unsigned short int> v(4);
  v.Fill(10);

  otbControlConditionTestMacro(otb::IsNoData(v, b2, v2), " wrong output of IsNoData function");
  v[1] = 1;
  otbControlConditionTestMacro(!otb::IsNoData(v, b2, v2), " wrong output of IsNoData function");
  b2[1] = false;
  otbControlConditionTestMacro(otb::IsNoData(v, b2, v2), " wrong output of IsNoData function");

  return EXIT_SUCCESS;
}
