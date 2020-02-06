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

//~ #if defined(HAVE_STD_SNPRINTF)
//~ #  include <cstdio>
//~ #elif defined(HAVE_MSC_SNPRINTF)
//~ #  include <stdio.h>
//~ #else
//~ #   error "Don't know how to perform snprintf"
//~ #endif
//~ 
//~ int otb::vs_printf(char * str, std::size_t size, const char * format, std::va_list ap)
//~ {
//~ #if defined(HAVE_STD_SNPRINTF)
    //~ const int res = std::vsnprintf(str, size, format, ap);
    //~ return res;
//~ #elif defined(HAVE_MSC_SNPRINTF)
    //~ const int res = _vsnprintf(str, size, format, ap);
    //~ // force 0x0 on the last possible character at worst
    //~ str[size-1] = '\0';
    //~ return res;
//~ #else
//~ #   error "Don't know how to perform snprintf"
//~ #endif
//~ }
//~ 
//~ int otb::s_printf(char *str, std::size_t size, const char *format, ...)
//~ {
    //~ std::va_list ap;
    //~ va_start(ap, format);
    //~ const int res = ossimplugins::vs_printf(str, size, format, ap);
    //~ va_end(ap);
    //~ return res;
//~ }

namespace otb
{

string_view lstrip(string_view const& v, string_view const& c )
{
  string_view::const_iterator pos = std::find_first_of(
    v.cbegin(), v.cend(), c.cbegin(), c.cend(),
    [](char c1, char c2) -> bool { return bool(c1 != c2) ;});
  // Beware: returned string_view may be empty
  return string_view(pos, v.cend());
}

string_view rstrip(string_view const& v, string_view const& c )
{
  string_view::const_reverse_iterator pos = std::find_first_of(
    v.crbegin(), v.crend(), c.cbegin(), c.cend(),
    [](char c1, char c2) -> bool { return bool(c1 != c2) ;});
  // Beware: returned string_view may be empty
  return string_view(v.cbegin(),&*pos - &(*v.cbegin()));
}

} // end namespace otb
