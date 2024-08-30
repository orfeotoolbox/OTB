/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperParameterKey_h
#define otbWrapperParameterKey_h

#include <string>
#include <vector>
#include <ostream>
#include <itksys/RegularExpression.hxx>
#include "itkMacro.h"
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
class OTBApplicationEngine_EXPORT ParameterKey
{
public:
  typedef ParameterKey Self;

  ParameterKey();
  ParameterKey(const char* val);
  ParameterKey(const std::string& val);
  virtual ~ParameterKey();


  /** Class Methods used to interface with the registered factories */
  /*
last() : return "tata"
   ** parent() : return "toto.titi"
   ** root() : return "toto"
   ** split() : return vector<string> ("toto", "titi", "tata")
   ** append(std::string)
   ** append(const Key&)
   */

  /** Returns the string after the last point separator */
  std::string GetLastElement();

  /** Returns the string before the first point separator */
  std::string GetFirstElement();

  /** Returns the string before the last point separator */
  std::string GetRoot();

  /** Returns the vector of string that contains each element separated by a point. */
  std::vector<std::string> Split();

  /** Append a string at the end of the key. A point separator will be added before the string. */
  void Append(const std::string& val);

  /** Append a string at the end of the key. A point separator will be added before the string. */
  void Append(const ParameterKey& pKey);

  /** Get Key value */
  std::string GetKey()
  {
    return m_Key;
  }

  /** Get Key value */
  std::string GetKey() const
  {
    return m_Key;
  }

  /** Set Key value */
  void SetKey(const std::string& val)
  {
    // Check chain : lowercase, alphanumerical or "."
    itksys::RegularExpression reg;
    reg.compile("([^0-9a-z\\.])");

    if (!reg.find(val))
    {
      m_Key = val;
    }
    else
    {
      itkGenericExceptionMacro("Invalid key '" << val << "'. Must be in lowercase, containing alphanumerical characters or \".\"");
    }
  }

private:
  ParameterKey(const Self&) = delete;
  void operator=(const Self&) = delete;

  std::string m_Key;
};

} // end namespace Wrapper
} // end namespace otb

#endif // otbWrapperParameterKey_h_
