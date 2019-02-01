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

#ifndef otbOGR_h
#define otbOGR_h

namespace otb {
  /**\defgroup gGeometry Geometry
   * \ingroup Thematic
   * \brief Classes and functions aimed toward the manipulation of set of geometric objects.
   * This module mainly provides an encapsulation of <a
   * href="http://www.gdal.org/ogr/">OGR API</a>.
   * @{
   */
  namespace ogr { }
  //@}
} // namespace otb

#include "otbOGRDriversInit.h"
#include "otbOGRDataSourceWrapper.h"
// #include "otbOGRLayerWrapper.h" // implicit
// #include "otbOGRFeatureWrapper.h" // implicit
// #include "otbOGRFieldWrapper.h" // implicit
// #include "otbOGRGeometryWrapper.h" // implicit

/**\ingroup gGeometry
 * \file    otbOGR.h
 * \brief How to include all include files dedicated to OGR encapsulation in OTB...
 */


#endif // otbOGR_h
