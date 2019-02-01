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

#ifndef otbUtils_h
#define otbUtils_h

#include <sstream>
#include <string>

#include "itkMacro.h"
#include "OTBCommonExport.h"

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
namespace Utils
{
  /** Check that the values for lon, lat are reasonable */
  bool OTBCommon_EXPORT IsLonLatValid(double lon, double lat);

  template <class T>
    inline std::string ConvertToString (const T& v)
  {
    std::stringstream s;
    s << v;
    return s.str();
  }

  /** Function that prints nothing (useful to disable libsvm logs)*/
  void OTBCommon_EXPORT PrintNothing(const char *s);

  extern OTBCommon_EXPORT bool const TrueConstant;

  extern OTBCommon_EXPORT bool const FalseConstant;
}

} // namespace otb

#endif
