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

#ifndef __otbGeoInformationConversion_h
#define __otbGeoInformationConversion_h

#include "ogr_spatialref.h"

namespace otb
{

/** \class GeoInformationConversion
 *  \brief Helper to do the geo informations conversions used
 *         frequently. 
 *  
 *  This class can help to build a WKT from a valid EPSG number using
 *  the method ToWKT(int srid).
 *           
 * \sa 
 */

class GeoInformationConversion
{
public:
  /** method used to create a wkt from an epsg number  */
  static const char * ToWKT(int srid)
  {
    const char * wkt = NULL;
    
    // Build the srs from the epsg number
    OGRSpatialReferenceH hSRS = NULL;
    hSRS = OSRNewSpatialReference(NULL);
    
    if (OSRImportFromEPSG(hSRS,srid ) == OGRERR_NONE)
      {
      // Get the wkt 
      if ( OSRExportToWkt(hSRS,(char**) &wkt) == OGRERR_NONE)
        {
        OSRDestroySpatialReference(hSRS);
        }
      }
    return  wkt;
  }

protected:
  /** Constructors */
  GeoInformationConversion();

  /** Destructor */
  virtual ~GeoInformationConversion();
};


} // End namespace otb

#endif
