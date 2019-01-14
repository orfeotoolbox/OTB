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


#ifndef otbGeoInformationConversion_h
#define otbGeoInformationConversion_h

#include <string>
#include "OTBTransformExport.h"

namespace otb
{

/**
 *  \namespace GeoInformationConversion
 *  \brief Helper functions to do the geo information conversions used frequently.
 *
 *  This namespace provides helpers functions to build a WKT from a valid EPSG number using
 *  the method ToWKT(int srid), and also to retrieve an EPSG number from a WKT
 *  using the method ToEPSG(const std::string &)
 *
 */
namespace GeoInformationConversion
{
  /** method used to create a wkt from an epsg number  */
  OTBTransform_EXPORT std::string ToWKT(int srid);
  OTBTransform_EXPORT std::string ToWKT(const std::string& candidateSrid);

  /** this method try to morph a wkt to ESRI WKT format and returns the error code**/
  OTBTransform_EXPORT bool IsESRIValidWKT(const std::string &Wkt);

  /** Function used to get an epsg number from a wkt string.
   *  Returns -1 if the wkt is neither a PROJCS nor a GEOGCS.
   *  Returns 0 if no authority code is found but wkt has a PROJCS or a GEOGCS.
   *  Otherwise the EPSG authority code is returned
   */
  OTBTransform_EXPORT int ToEPSG(const std::string &wkt);
}

} // End namespace otb

#endif
