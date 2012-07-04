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
#include "ogr_core.h"

#include "cpl_conv.h"
#include "boost/lexical_cast.hpp"


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

std::string GeoInformationConversion::ToWKT(const std::string& candidateSrid)
{
  std::string wkt(candidateSrid);
  try
    {
    int srid = boost::lexical_cast<int>(candidateSrid);
    return GeoInformationConversion::ToWKT(srid);
    }
  catch(boost::bad_lexical_cast &)
    {
    return wkt;
    }
}

bool GeoInformationConversion::IsESRIValidWKT(const std::string &wkt)
{
  OGRSpatialReference SRS(wkt.c_str());
  SRS.morphToESRI();
  return SRS.Validate()!=OGRERR_NONE;
}

} // End namespace otb
