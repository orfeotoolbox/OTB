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

#ifndef otbWrapperParameterKey_h
#define otbWrapperParameterKey_h

#include <algorithm>
#include <string>
#include <string_view>
#include <vector>
#include "OTBApplicationEngineExport.h"

namespace otb
{
namespace Wrapper
{

/** \class Application
 *  \brief This class is a the Parameter key representation.
 *
 *  The constructor accepts a string key constructed as follow : parent.me.child.
 *  It implements some key manipulation as described in the code.
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT ParameterKey final
{
public:
  using Self = ParameterKey;

  ParameterKey() = default;
  ParameterKey(std::string val);

  /** Class Methods used to interface with the registered factories */
  /*
     last() : return "tata"
   ** parent() : return "toto.titi"
   ** root() : return "toto" -- Actually GetRoot() returns the parent() part
   ** split() : return vector<string> ("toto", "titi", "tata")
   ** append(std::string)
   ** append(const Key&)
   */

  /** Returns the string after the last point separator */
  std::string_view GetLastElement() const;

  /** Returns the string before the first point separator */
  [[deprecated]] std::string_view GetFirstElement() const;

  /** Returns the string before the last point separator */
  std::string_view GetRoot() const;

  /** Returns the vector of string that contains each element separated by a point. */
  std::vector<std::string> Split() const;

  /** Append a string at the end of the key. A point separator will be added before the string. */
  void Append(const std::string& val);

  /** Append a string at the end of the key. A point separator will be added before the string. */
  void Append(const ParameterKey& pKey);

  /** Get Key value */
  std::string const& GetKey() const
  {
    return m_Key;
  }

  /** Set Key value */
  void SetKey(std::string val);

  // ----[ Split On First Dot
  [[nodiscard]]
  static constexpr
  std::pair<std::string_view, std::string_view>
  SplitKeyOnFirstDot(std::string_view key)
  {
    auto const dot = key.find_first_of('.');
    if (dot == std::string_view::npos)
    {
      return {{key}, {}};
    }
    else
    {
      return {
        {key.data(), dot},
        {key.begin()+dot+1, key.end()}
      };
    }
  }

  [[nodiscard]] constexpr auto SplitKeyOnFirstDot() const
  {
    return SplitKeyOnFirstDot(m_Key);
  }

  // ----[ Split On Last Dot
  [[nodiscard]]
  static constexpr
  std::pair<std::string_view, std::string_view>
  SplitKeyOnLastDot(std::string_view key)
  {
    auto const dot = key.find_last_of('.');
    if (dot == std::string_view::npos)
    {
      return {{}, {key}};
    }
    else
    {
      return {
        {key.data(), dot},
        {key.begin()+dot+1, key.end()}
      };
    }
  }

  [[nodiscard]] constexpr auto SplitKeyOnLastDot() const
  {
    return SplitKeyOnLastDot(m_Key);
  }

  [[nodiscard]] constexpr auto HasSeveralParts() const
  {
    return m_Key.find('.') != std::string::npos;
  }

private:
  ParameterKey(const Self&) = delete;
  void operator=(const Self&) = delete;

  std::string m_Key;
};

} // end namespace Wrapper
} // end namespace otb

#endif // otbWrapperParameterKey_h_
