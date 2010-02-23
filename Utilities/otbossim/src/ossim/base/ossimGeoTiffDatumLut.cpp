//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Contains class definition for ossimGeoTiffDatumLut.
//*******************************************************************
//  $Id: ossimGeoTiffDatumLut.cpp 16472 2010-02-01 19:52:27Z gpotts $

#include <ossim/base/ossimGeoTiffDatumLut.h>
#include <ossim/base/ossimString.h>

static const int TABLE_SIZE = 24;

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimGeoTiffDatumLut::ossimGeoTiffDatumLut()
   :
      ossimLookUpTable(TABLE_SIZE)
{
   //***
   // Complete initialization of data member "theTable".
   // Note:  Scalar types defined in constants.h file.
   //***
   theTable[0].theKey    = GCS_Adindan;
   theTable[0].theValue  = "ADI-M";
   
   theTable[1].theKey    = GCS_Arc_1950;
   theTable[1].theValue  = "ARF-M";
   
   theTable[2].theKey    = GCS_Arc_1960;
   theTable[2].theValue  = "ARS-M";
   
   theTable[3].theKey    = GCS_ED50;
   theTable[3].theValue  = "EUR-M";
   
   theTable[4].theKey    = GCS_NAD27;
   theTable[4].theValue  = "NAS-C";
   
   theTable[5].theKey    = GCS_NAD83;
   theTable[5].theValue  = "NAR-C";
   
   theTable[6].theKey    = GCS_OSGB_1936;
   theTable[6].theValue  = "OGB-M";
   
   theTable[7].theKey    = GCS_WGS_72;
   theTable[7].theValue  = "WGD";
   
   theTable[8].theKey    = GCS_WGS_84;
   theTable[8].theValue  = "WGE";
   
   theTable[9].theKey    = GCS_Tokyo;
   theTable[9].theValue  = "TOY-M";
   
   theTable[10].theKey   = DatumE_WGS84;
   theTable[10].theValue = "WGE";
   
   theTable[11].theKey   = Datum_Adindan;
   theTable[11].theValue = "ADI-M";
   
   theTable[12].theKey   = Datum_Arc_1950;
   theTable[12].theValue = "ARF-M";
   
   theTable[13].theKey   = Datum_Arc_1960;
   theTable[13].theValue = "ARS-M";
   
   theTable[14].theKey   = Datum_European_Datum_1950;
   theTable[14].theValue = "EUR-M";
   
   theTable[15].theKey   = Datum_North_American_Datum_1927;
   theTable[15].theValue = "NAS-C";
   
   theTable[16].theKey   = Datum_North_American_Datum_1983;
   theTable[16].theValue = "NAR-C";
   
   theTable[17].theKey   = Datum_OSGB_1936;
   theTable[17].theValue = "OGB-M";
   
   theTable[18].theKey   = Datum_Tokyo;
   theTable[18].theValue = "TOY-M";
   
   theTable[19].theKey   = Datum_WGS72;
   theTable[19].theValue = "WGD";
   
   theTable[20].theKey   = Datum_WGS84;
   theTable[20].theValue = "WGE";

   theTable[21].theKey   = Datum_NAD83_HARN;
   theTable[21].theValue = "NAR";

   theTable[22].theKey   = GCS_NAD83_HARN;
   theTable[22].theValue = "NAR";

   //---
   // Note:  This code implies a Clark 1866 ellipse with no datum.
   // NAS-C used as work around for poor tag setting out of Erdas Imagine.
   //---
   theTable[23].theKey   = GCS_Clark_1866;
   theTable[23].theValue = "NAS-C";
   theTable[23].theKey   = 3785;
   theTable[23].theValue = "6055";
   theTable[23].theKey   = 900913;
   theTable[23].theValue = "6055";
}

ossimGeoTiffDatumLut::~ossimGeoTiffDatumLut()
{
}

ossimKeyword ossimGeoTiffDatumLut::getKeyword() const
{
   return ossimKeyword("geotiff_datum_code", "");
}
