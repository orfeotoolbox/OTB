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

#include "otbStringToHTML.h"
#include <ostream>

namespace otb
{

StringToHTML::StringToHTML(const std::string& str) : m_Str(str)
{
}

void StringToHTML::Print(std::ostream& os) const
{
  // TODO : add a Tex to HTML translator (maybe try TtH)
  bool formulaMode = false;
  for (auto&& c : m_Str)
  {
    if (c == '`')
    {
      formulaMode = !formulaMode;
    }
    switch (c)
    {
    case '<':
      os.write("&lt;", 4);
      break;
    case '>':
      os.write("&gt;", 4);
      break;
    case '&':
      os.write("&amp;", 5);
      break;
    case '\n':
      os.write("<br/>", 5);
      break;
    default:
      os.put(c);
    }
  }
}

std::ostream& operator<<(std::ostream& os, const StringToHTML& str)
{
  str.Print(os);
  return os;
}

} // end of namespace otb
