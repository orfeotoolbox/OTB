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

#include "otbWrapperParameterKey.h"

#include "otb_boost_string_header.h"

namespace otb
{
namespace Wrapper
{

ParameterKey::ParameterKey() : m_Key("")
{
}

ParameterKey::ParameterKey(const char* val)
{
  this->SetKey(std::string(val));
}

ParameterKey::ParameterKey(const std::string& val)
{
  this->SetKey(val);
}

ParameterKey::~ParameterKey()
{
}

std::string ParameterKey::GetLastElement()
{
  return m_Key.substr(m_Key.find_last_of(".") + 1, m_Key.size());
}

std::string ParameterKey::GetFirstElement()
{
  return m_Key.substr(0, m_Key.find_first_of("."));
}

std::string ParameterKey::GetRoot()
{
  return m_Key.substr(0, m_Key.find_last_of("."));
}


std::vector<std::string> ParameterKey::Split()
{
  std::vector<std::string> res;
  boost::split(res, m_Key, boost::is_any_of("."), boost::token_compress_on);

  return res;
}

void ParameterKey::Append(const std::string& val)
{
  m_Key.append(".");
  m_Key.append(val);
}


void ParameterKey::Append(const ParameterKey& pKey)
{
  this->Append(pKey.GetKey());
}

} // end namespace Wrapper
} // end namespace otb
