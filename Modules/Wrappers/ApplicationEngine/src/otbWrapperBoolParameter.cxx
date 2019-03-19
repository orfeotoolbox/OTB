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

#include "otbWrapperBoolParameter.h"

namespace otb
{
namespace Wrapper
{

BoolParameter::BoolParameter()
  : m_Value(false)
{}

bool
BoolParameter::GetValue() const
{
  return m_Value;
}

std::string
BoolParameter::GetValueAsString() const
{
  return std::string(m_Value?"true":"false");
}

void
BoolParameter::SetValue(bool state)
{
  if (m_Value != state)
    {
    m_Value = state;
    this->Modified();
    }
  this->SetActive(true);
}

void
BoolParameter::SetValue(const std::string & str)
{
  std::string lowerStr;
  // only strings less than 8 characters expected
  lowerStr.reserve(8);
  for (unsigned int i=0 ; i < std::min(8U,(unsigned int) str.size()) ; i++ )
    {
    lowerStr.push_back(tolower(str[i]));
    }
  if (lowerStr == "1" || lowerStr == "on" || lowerStr == "true" || lowerStr == "yes")
    {
    this->SetValue(true);
    }
  else if (lowerStr == "0" || lowerStr == "off" || lowerStr == "false" || lowerStr == "no")
    {
    this->SetValue(false);
    }
  else
    {
    itkGenericExceptionMacro(<< "Wrong value for BoolParameter (" << str << "),"
      " accepts: 0, 1, on, off, true, false, yes, no");
    }
}

ParameterType BoolParameter::GetType() const
{
  return ParameterType_Bool;
}

int BoolParameter::ToInt() const
{
  return static_cast<int>(this->GetValue());
}

std::string BoolParameter::ToString() const
{
  return GetValueAsString();
}

void BoolParameter::FromString(const std::string& value)
{
  SetValue(value);
}

void BoolParameter::FromInt(int value)
{
  SetValue(static_cast<bool>(value));
}

} // end of namespace Wrapper
} // end of namespace otb
