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

#include "otbPositionHelpers.h"
#include <ogr_spatialref.h>
#include <cassert>

namespace otb
{

void ConvertPhysicalPointCoordinatesToWGS84(
    lonlat_view_t lonlats, OGRCoordinateTransformation & ct)
{
  assert(lonlats.extent(0) == 2);
  auto const line_size = lonlats.extent(1);
#if GDAL_VERSION_NUM >= 3030000
  // #   pragma message "GDAL >= 3.3 => Using TransformWithErrorCodes"
  // Directly call TransformWithErrorCodes() instead of Transform() that
  // would allocate every time with malloc, and with a vector as well in
  // order to store results... that we'll ignore, for now...
  ct.TransformWithErrorCodes(
      line_size,
      &lonlats(0, 0), &lonlats(1, 0), nullptr, nullptr,
      nullptr /* or buffer_error_code.data() */);
  // success == all_of(buffer_error_code == 0);
  // otbLogMacro(Info, << "Converting from " << demLonLatPoint << " to " << demGeoPoint);
#else
#  pragma message "GDAL < 3.3 => Using Transform"
  ct.Transform(
      line_size,
      &lonlats(0, 0), &lonlats(1, 0), nullptr, nullptr,
      nullptr);
#endif
}

}  // otb namespace
