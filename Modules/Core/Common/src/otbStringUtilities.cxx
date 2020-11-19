/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include "otbStringUtilities.h"

namespace otb
{

string_view lstrip(string_view const& v, string_view const& c )
{
  string_view::const_iterator pos = std::find_if(
        v.cbegin(), v.cend(),
        [&v, &c](char in) -> bool {return std::find(c.cbegin(),c.cend(),in) == c.cend();});

  // Beware: returned string_view may be empty
  return string_view(pos, v.cend());
}

string_view rstrip(string_view const& v, string_view const& c )
{
  string_view::const_reverse_iterator pos = std::find_if(
        v.crbegin(), v.crend(),
        [&v, &c](char in) -> bool {return std::find(c.cbegin(),c.cend(),in) == c.cend();});

  // Beware: returned string_view may be empty
  return string_view(v.cbegin(), &*pos - &(*v.cbegin()) + 1);
}

} // end namespace otb
