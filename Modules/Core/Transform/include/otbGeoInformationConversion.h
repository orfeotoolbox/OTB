/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

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
 *  the method ToWKT(int srid).
 */
namespace GeoInformationConversion
{
  /** method used to create a wkt from an epsg number  */
  OTBTransform_EXPORT std::string ToWKT(int srid);
  OTBTransform_EXPORT std::string ToWKT(const std::string& candidateSrid);

  /** this method try to morph a wkt to ESRI WKT format and returns the error code**/
  OTBTransform_EXPORT bool IsESRIValidWKT(const std::string &Wkt);
}

} // End namespace otb

#endif
