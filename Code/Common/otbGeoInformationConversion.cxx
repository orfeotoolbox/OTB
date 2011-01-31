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
#include "otbGeoInformationConversion.h"

#include "ogr_spatialref.h"
#include "cpl_conv.h"

namespace otb
{

std::string GeoInformationConversion::ToWKT(int srid)
{
  char * wkt = NULL;
  std::string ret;

  // Build the srs from the epsg number
  OGRSpatialReferenceH hSRS = NULL;
  hSRS = OSRNewSpatialReference(NULL);

  if (OSRImportFromEPSG(hSRS, srid ) == OGRERR_NONE)
    {
    // Get the wkt
    if ( OSRExportToWkt(hSRS, &wkt) == OGRERR_NONE)
      {
      ret = wkt;
      CPLFree(wkt);
      }
    }
  OSRRelease(hSRS);
  return  ret;
}

} // End namespace otb
