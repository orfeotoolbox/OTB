//*******************************************************************
// Copyright (C) 2001 ImageLinks Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Contains class declaration for GeoTiffDatumLut. Used to map a
// geotiff coordinate transformation code to an ossim projection.
//*******************************************************************
//  $Id: ossimGeoTiffDatumLut.h 9094 2006-06-13 19:12:40Z dburken $

#ifndef ossimGeoTiffDatumLut_HEADER
#define ossimGeoTiffDatumLut_HEADER

#include <ossim/base/ossimLookUpTable.h>
#include <ossim/base/ossimKeywordNames.h>

class OSSIMDLLEXPORT ossimGeoTiffDatumLut : public ossimLookUpTable
{
public:

   enum
   {
      //---
      // Datum codes cut from geotiff specification section 6.3.2.1.
      //---
      GCS_Adindan                     = 4201,
      GCS_Arc_1950                    = 4209,
      GCS_Arc_1960                    = 4210,
      GCS_ED50                        = 4230,
      GCS_NAD27                       = 4267,
      GCS_NAD83                       = 4269,
      GCS_OSGB_1936                   = 4277,
      GCS_WGS_72                      = 4322,
      GCS_WGS_84                      = 4326,
      GCS_Tokyo                       = 4301,
      GCS_NAD83_HARN                  = 4152,

      //---
      // Datum codes cut from geotiff specification section 6.3.2.1.
      // NOTE:  These codes imply an ellipsoid only!
      //---
      GCS_Clark_1866                  = 4008,
      
      //---
      // Datum codes cut from geotiff specification section 6.3.2.2.
      //---
      DatumE_WGS84                    = 6030,
      Datum_Adindan                   = 6201,
      Datum_Arc_1950                  = 6209,
      Datum_Arc_1960                  = 6210,
      Datum_European_Datum_1950       = 6230,
      Datum_North_American_Datum_1927 = 6267,
      Datum_North_American_Datum_1983 = 6269,
      Datum_OSGB_1936                 = 6277,
      Datum_Tokyo                     = 6301,
      Datum_WGS72                     = 6322,
      Datum_WGS84                     = 6326,
      Datum_NAD83_HARN                = 6152
   };
   
   ossimGeoTiffDatumLut();

   virtual ~ossimGeoTiffDatumLut();
   
   virtual ossimKeyword getKeyword() const;
};

#endif
