/*
 * Copyright (C) 2005-2026 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbWrapperParameterKey.h"
#include "itkMacro.h"
#include <itksys/RegularExpression.hxx>

#include "otb_boost_string_header.h"

namespace otb
{
namespace Wrapper
{

ParameterKey::ParameterKey(std::string val)
{
  this->SetKey(std::move(val));
}

std::string_view ParameterKey::GetLastElement() const
{
  auto const dot = m_Key.find_last_of('.');
  return {m_Key.begin()+dot+1, m_Key.end()};
}

std::string_view ParameterKey::GetFirstElement() const
{
  return {m_Key.data(), m_Key.find_first_of('.')};
}

std::string_view ParameterKey::GetRoot() const
{
  auto const dot = m_Key.find_last_of('.');
  return {m_Key.data(), dot};
}

std::vector<std::string> ParameterKey::Split() const
{
  std::vector<std::string> res;
  boost::split(res, m_Key, boost::is_any_of("."), boost::token_compress_on);

  return res;
}

static_assert(Wrapper::ParameterKey::SplitKeyOnFirstDot("toto").first == "toto");
static_assert(Wrapper::ParameterKey::SplitKeyOnFirstDot("toto").second == "");

static_assert(Wrapper::ParameterKey::SplitKeyOnFirstDot("toto.titi").first == "toto");
static_assert(Wrapper::ParameterKey::SplitKeyOnFirstDot("toto.titi").second == "titi");

static_assert(Wrapper::ParameterKey::SplitKeyOnFirstDot("toto.titi.tata").first == "toto");
static_assert(Wrapper::ParameterKey::SplitKeyOnFirstDot("toto.titi.tata").second == "titi.tata");


static_assert(Wrapper::ParameterKey::SplitKeyOnLastDot("toto").first == "");
static_assert(Wrapper::ParameterKey::SplitKeyOnLastDot("toto").second == "toto");

static_assert(Wrapper::ParameterKey::SplitKeyOnLastDot("toto.titi").first == "toto");
static_assert(Wrapper::ParameterKey::SplitKeyOnLastDot("toto.titi").second == "titi");

static_assert(Wrapper::ParameterKey::SplitKeyOnLastDot("toto.titi.tata").first == "toto.titi");
static_assert(Wrapper::ParameterKey::SplitKeyOnLastDot("toto.titi.tata").second == "tata");


void ParameterKey::Append(const std::string& val)
{
  m_Key.append(".");
  m_Key.append(val);
}


void ParameterKey::Append(const ParameterKey& pKey)
{
  this->Append(pKey.GetKey());
}

void ParameterKey::SetKey(std::string val)
{
  // Check chain : lowercase, alphanumerical or "."
  itksys::RegularExpression reg;
  reg.compile("([^0-9a-z\\.])");

  if (!reg.find(val))
  {
    m_Key = std::move(val);
  }
  else
  {
    itkGenericExceptionMacro("Invalid key '" << val << "'. Must be in lowercase, containing alphanumerical characters or \".\"");
  }
}

} // end namespace Wrapper
} // end namespace otb
