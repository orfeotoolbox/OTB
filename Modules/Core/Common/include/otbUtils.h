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

#ifndef otbUtils_h
#define otbUtils_h

#include <sstream>            // for stringstream
#include <string>             // for string
#include "OTBCommonExport.h"  // for OTBCommon_EXPORT

namespace otb
{

/** \class Utils
 * \brief Utils operations.
 *
 * Some useful utilities in use across the library
 *
 *
 * \ingroup OTBCommon
 */
namespace Utils OTBCommon_EXPORT
{
  /** Check that the values for lon, lat are reasonable */
  bool IsLonLatValid(double lon, double lat);

  template <class T>
    inline std::string ConvertToString (const T& v)
  {
    std::stringstream s;
    s << v;
    return s.str();
  }

  /** Function that prints nothing (useful to disable libsvm logs)*/
  void PrintNothing(const char *s);

  extern bool const TrueConstant;

  extern bool const FalseConstant;
}

} // namespace otb

#endif
