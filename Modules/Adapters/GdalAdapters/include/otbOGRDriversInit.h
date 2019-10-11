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

#ifndef otbOGRDriversInit_h
#define otbOGRDriversInit_h

#include <boost/noncopyable.hpp>

#include "OTBGdalAdaptersExport.h"

namespace otb
{
namespace ogr
{
/**\ingroup Geometry
 * Singleton-like class to provide lazy-registering of all \c OGRDriver's.
 *
 * Call
 * \code
 * otb::ogr::Drivers::Init();
 * \endcode
 * before using classes wrapping OGR.
 *
 * <p><b>Properties</b><br>
 * - Follows Meyer's Singleton design.
 * - Implicitly initialized with a default constructor
 * - MT-Safe in C++11, and some compilers like GCC (not on old releases of
 * VC++)
 * - Non-copyable
 */
class OTBGdalAdapters_EXPORT Drivers : private boost::noncopyable
{
  /** \name Singleton related functions */
  //@{
public:
  static Drivers& Init();

private:
  /** Constructor.
   * Calls \c OGRRegisterAll().
   */
  Drivers();
  /** Destructor.
   * Calls \c OGRCleanupAll().
   */
  ~Drivers();
  //@}
};
}
} // end namespace otb::ogr

#endif // otbOGRDriversInit_h
