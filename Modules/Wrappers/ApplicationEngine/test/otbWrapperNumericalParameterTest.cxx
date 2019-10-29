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
#pragma warning(disable : 4786)
#endif

#include "otbWrapperNumericalParameter.h"

using namespace otb::Wrapper;

template <typename T>
void assert_equal(const T& a, const T& b)
{
  if (a != b)
  {
    itkGenericExceptionMacro("assert_equal failed");
  }
}

int otbWrapperFloatParameterTest(int, char* [])
{
  auto param = FloatParameter::New();

  param->SetKey("mykey");
  param->SetDescription("My description.");

  assert_equal(param->GetType(), ParameterType_Float);

  { // SetValue
    const double val = 42.005;
    param->SetValue(val);
    assert_equal(param->GetValue(), val);
    assert_equal(param->ToInt(), int(val));
    assert_equal(param->ToFloat(), val);
    assert_equal(param->ToString(), std::string("42.005"));
  }

  { // FromFloat
    const double val = -6.5;
    param->FromFloat(val);
    assert_equal(param->GetValue(), val);
    assert_equal(param->ToInt(), int(val));
    assert_equal(param->ToFloat(), val);
    assert_equal(param->ToString(), std::string("-6.5"));
  }

  { // FromString
    const std::string str = "-100.01";
    const double       val = -100.01;
    param->FromString(str);
    assert_equal(param->GetValue(), val);
    assert_equal(param->ToInt(), int(val));
    assert_equal(param->ToFloat(), val);
    assert_equal(param->ToString(), std::string("-100.01"));
  }

  return EXIT_SUCCESS;
}

int otbWrapperIntParameterTest(int, char* [])
{
  auto param = IntParameter::New();

  param->SetKey("mykey");
  param->SetDescription("My description.");

  assert_equal(param->GetType(), ParameterType_Int);

  { // SetValue
    const int val = 42;
    param->SetValue(val);
    assert_equal(param->GetValue(), val);
    assert_equal(param->ToInt(), val);
    assert_equal(param->ToFloat(), double(val));
    assert_equal(param->ToString(), std::string("42"));
  }

  { // FromString
    const std::string str = "-100";
    const int         val = -100;
    param->FromString(str);
    assert_equal(param->GetValue(), val);
    assert_equal(param->ToInt(), val);
    assert_equal(param->ToFloat(), double(val));
    assert_equal(param->ToString(), std::string("-100"));
  }

  return EXIT_SUCCESS;
}

int otbWrapperRAMParameterTest(int, char* [])
{
  typedef RAMParameter      RAMParameterType;
  RAMParameterType::Pointer parameter = RAMParameterType::New();

  // Test Set/Get Value
  parameter->SetValue(256);
  parameter->SetValue(2560);
  parameter->SetValue(128);
  std::cout << "Last RAMParameter Value set : " << parameter->GetValue() << std::endl;

  // Test Set/Get Default, extremum value
  RAMParameterType::ScalarType min = 0;
  RAMParameterType::ScalarType max = 1024;
  RAMParameterType::ScalarType def = 256;

  parameter->SetDefaultValue(def);
  parameter->SetMinimumValue(min);
  parameter->SetMaximumValue(max);

  if (parameter->GetMinimumValue() != min)
  {
    std::cout << "Minimum Value : expexted " << min << " --> got " << parameter->GetMinimumValue() << std::endl;
    return EXIT_FAILURE;
  }

  if (parameter->GetMaximumValue() != max)
  {
    std::cout << "Maximum Value : expexted " << max << " --> got " << parameter->GetMaximumValue() << std::endl;
    return EXIT_FAILURE;
  }

  if (parameter->GetDefaultValue() != def)
  {
    std::cout << "Default Value : expexted " << def << " --> got " << parameter->GetDefaultValue() << std::endl;
    return EXIT_FAILURE;
  }

  // Reset Value
  parameter->Reset();
  std::cout << "Last RAMParameter Value set : " << parameter->GetValue() << std::endl;


  return EXIT_SUCCESS;
}
