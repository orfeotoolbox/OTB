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

#include "otbWrapperBoolParameter.h"

namespace otb
{
namespace Wrapper
{

bool
BoolParameter::GetValue() const
{
  return m_Value;
}

void
BoolParameter::SetValue(bool state)
{
  if (m_Value != state)
    {
    m_Value = state;
    this->Modified();
    }
}

void
BoolParameter::SetValue(const std::string & str)
{
  std::string lowerStr;
  // only strings less than 10 characters expected
  lowerStr.reserve(10);
  for (unsigned int i=0 ; i < std::min(10U,(unsigned int) str.size()) ; i++ )
    {
    lowerStr.push_back(tolower(str[i]));
    }
  if (lowerStr == "1" || lowerStr == "on" || lowerStr == "true")
    {
    this->SetValue(true);
    }
  else if (lowerStr == "0" || lowerStr == "off" || lowerStr == "false")
    {
    this->SetValue(false);
    }
  else
    {
    itkGenericExceptionMacro(<< "Wrong value for BoolParameter (" << str << "), accepts: 0, 1, on, off, true, false");
    }
}

} // end of namespace Wrapper
} // end of namespace otb
