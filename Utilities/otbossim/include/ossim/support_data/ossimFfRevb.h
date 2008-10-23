//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Ken Melero (kmelero@imagelinks.com)
//         Orginally written by Dave Burken (dburken@imagelinks.com)
// Description: This class parses an EOSAT Fast Format rev b header.
//
//********************************************************************
// $Id: ossimFfRevb.h 10384 2007-01-26 20:32:12Z gpotts $

#ifndef ossimFfRevb_HEADER
#define ossimFfRevb_HEADER
#include <ossim/base/ossimReferenced.h>

#include <iostream>
using namespace std;

//***************************************************************************
// CLASS: ossimFfRevb.h
//***************************************************************************
class ossimFfRevb : public ossimReferenced
{
public:
   ossimFfRevb  ();
   ossimFfRevb  (const char* headerFile);
   ~ossimFfRevb (){};

   enum ErrorStatus
   {
      OSSIM_OK    = 0,
      OSSIM_ERROR = 1
   };

   //***
   // Enumerations for parsing header.  Contains size in bytes
   // for all fields.
   // 
   // NOTE:  Sizes are the actual number of bytes for that field so add
   //        one for trailing '\0' to get the string length for the
   //        data member that are character arrays.
   //***
   enum
   {
      NUMBER_OF_PROJECTION_PARAMETERS = 15,
      NUMBER_OF_BANDS                 = 7
   };
   
   enum
   {
      PRODUCT_ORDER_NUMBER_DESC_SIZE      = 9,
      PRODUCT_ORDER_NUMBER_SIZE           = 11,
      WRS_DESC_SIZE                       = 6,
      PATH_ROW_NUMBER_SIZE                = 9,
      DATE_DESC_SIZE                      = 19,
      DATE_SIZE                           = 8,
      SAT_NUMBER_DESC_SIZE                = 12,
      SAT_NUMBER_SIZE                     = 2,
      INSTRUMENT_TYPE_DESC_SIZE           = 13,
      INSTRUMENT_TYPE_SIZE                = 4,
      PRODUCT_TYPE_DESC_SIZE              = 15,
      PRODUCT_TYPE_SIZE                   = 14,
      PRODUCT_SIZE_DESC_SIZE              = 15,
      PRODUCT_SIZE_SIZE                   = 10,
      MAP_SHEET_SIZE                      = 78,
      PROCESSING_TYPE_DESC_SIZE           = 30,
      PROCESSING_TYPE_SIZE                = 10,
      RESAMPLING_ALGO_DESC_SIZE           = 13,
      RESAMPLING_ALGO_SIZE                = 2,
      RADIANCE_DESC_SIZE                  = 20,
      RADIANCE_SIZE                       = 16,
      VOLUME_NUMBER_DESC_SIZE             = 20,
      VOLUME_NUMBER_SIZE                  = 3,
      FIRST_LINE_DESC_SIZE                = 14,
      FIRST_LINE_IN_VOLUME_SIZE           = 5,
      LINES_PER_VOLUME_DESC_SIZE          = 15,
      LINES_PER_VOLUME_SIZE               = 5,
      ORIENTATION_ANGLE_DESC_SIZE         = 14,
      ORIENTATION_ANGLE_SIZE              = 6,
      MAP_PROJ_NAME_DESC_SIZE             = 13,
      MAP_PROJ_NAME_SIZE                  = 4,
      USGS_PROJ_NUMBER_DESC_SIZE          = 20,
      USGS_PROJ_NUMBER_SIZE               = 6,
      USGS_MAP_ZONE_DESC_SIZE             = 16,
      USGS_MAP_ZONE_SIZE                  = 6,
      USGS_PROJ_PARAMS_DESC_SIZE          = 29,
      USGS_PROJ_PARAMS_SIZE               = 24,
      ELLIPSOID_DESC_SIZE                 = 18,
      ELLIPSOID_SIZE                      = 20,
      MAJOR_AXIS_DESC_SIZE                = 18,
      MAJOR_AXIS_SIZE                     = 11,
      MINOR_AXIS_DESC_SIZE                = 18,
      MINOR_AXIS_SIZE                     = 11,
      PIXEL_GSD_DESC_SIZE                 = 13,
      PIXEL_GSD_SIZE                      = 5,
      PIXELS_PER_LINE_DESC_SIZE           = 17,
      PIXELS_PER_LINE_SIZE                = 5,
      LINES_PER_IMAGE_DESC_SIZE           = 17,
      LINES_PER_IMAGE_SIZE                = 5,
      CORNER_DESC_SIZE                    = 4,
      LON_SIZE                            = 13,
      LAT_SIZE                            = 12,
      EASTING_SIZE                        = 13,
      NORTHING_SIZE                       = 13,
      BANDS_PRESENT_DESC_SIZE             = 16,
      BANDS_PRESENT_SIZE                  = 7,
      BLOCKING_FACTOR_DESC_SIZE           = 18,
      BLOCKING_FACTOR_SIZE                = 4,
      RECORD_LENGTH_DESC_SIZE             = 16,
      RECORD_LENGTH_SIZE                  = 5,
      SUN_ELEVATION_DESC_SIZE             = 16,
      SUN_ELEVATION_SIZE                  = 2,
      SUN_AZIMUTH_DESC_SIZE               = 14,
      SUN_AZIMUTH_SIZE                    = 3,
      CENTER_DESC_SIZE                    = 8,
      CENTER_SAMPLE_SIZE                  = 6,
      CENTER_LINE_SIZE                    = 6,
      OFFSET_DESC_SIZE                    = 8,
      OFFSET_SIZE                         = 4,
      REV_DESC_SIZE                       = 4,
      FORMAT_VERSION_SIZE                 = 1
   };
      
   //***
   // Enumerations for byte offsets in header.  All offsets are relative to
   // the first byte of the first record which is byte 0.
   //***
   enum
   {
      // One record:  bytes 0 - 1535
      PRODUCT_ORDER_NUMBER_OFFSET           = 9,
      PATH_ROW_NUMBER_OFFSET                = 26,
      DATE_OFFSET                           = 54,
      SAT_NUMBER_OFFSET                     = 74,
      INSTRUMENT_TYPE_OFFSET                 = 89,
      PRODUCT_TYPE_OFFSET                   = 108,
      PRODUCT_SIZE_OFFSET                   = 137,
      MAP_SHEET_NAME_OFFSET                 = 147,
      PROCESSING_TYPE_OFFSET                = 255,
      RESAMPLING_ALGO_OFFSET                = 278,
      VOLUME_NUMBER_OFFSET                  = 438,
      FIRST_LINE_IN_VOLUME_OFFSET           = 455,
      LINES_PER_VOLUME_OFFSET               = 475,
      ORIENTATION_ANGLE_OFFSET              = 494,
      MAP_PROJ_NAME_OFFSET                  = 513,
      USGS_PROJ_NUMBER_OFFSET               = 537,
      USGS_MAP_ZONE_OFFSET                  = 559,
      ELLIPSOID_OFFSET                      = 972,
      MAJOR_AXIS_OFFSET                     = 1010,
      MINOR_AXIS_OFFSET                     = 1039,
      PIXEL_GSD_OFFSET                      = 1063,
      PIXELS_PER_LINE_OFFSET                = 1085,
      LINES_PER_IMAGE_OFFSET                = 1107, 
      UL_LON_OFFSET                         = 1116,
      UL_LAT_OFFSET                         = 1130,
      UL_EASTING_OFFSET                     = 1143,
      UL_NORTHING_OFFSET                    = 1157,
      UR_LON_OFFSET                         = 1174,
      UR_LAT_OFFSET                         = 1188,
      UR_EASTING_OFFSET                     = 1201,
      UR_NORTHING_OFFSET                    = 1215,
      LR_LON_OFFSET                         = 1232,
      LR_LAT_OFFSET                         = 1246,
      LR_EASTING_OFFSET                     = 1259,
      LR_NORTHING_OFFSET                    = 1273,
      LL_LON_OFFSET                         = 1290,
      LL_LAT_OFFSET                         = 1304,
      LL_EASTING_OFFSET                     = 1317,
      LL_NORTHING_OFFSET                    = 1331,
      BANDS_PRESENT_OFFSET                  = 1360,
      BLOCKING_FACTOR_OFFSET                = 1385,
      RECORD_LENGTH_OFFSET                  = 1405,
      SUN_ELEVATION_OFFSET                  = 1426, 
      SUN_AZIMUTH_OFFSET                    = 1442,
      CENTER_LON_OFFSET                     = 1453,
      CENTER_LAT_OFFSET                     = 1467,
      CENTER_EASTING_OFFSET                 = 1480,
      CENTER_NORTHING_OFFSET                = 1494,
      CENTER_SAMPLE_OFFSET                  = 1507,
      CENTER_LINE_OFFSET                    = 1513,
      OFFSET_OFFSET                         = 1527,
      FORMAT_VERSION_OFFSET                 = 1535 };

   int errorStatus() const { return theErrorStatus; }

   void print(ostream& os) const;

   void write(ostream& os) const;

   int    path() const;
      //> Returns the path as an int.
      //<
   int    row() const;
      //> Returns the row as an int from the pathRowString.
      //<
   int    fraction() const;
      //> Returns the fraction as an int from the pathRowString.
      //<
   double projParam(int paramNumber) const;
      //>  Converts the USGS Projection Parameter string, which has a
      //   "D" to denote the exponent, to a double.
      //<

   friend ostream& operator<<(ostream& os, const ossimFfRevb& head);
     
   char   theProductOrderNumber[PRODUCT_ORDER_NUMBER_SIZE + 1];
   char   thePathRowNumber[PATH_ROW_NUMBER_SIZE + 1];   
   char   theAcquisitionDate[DATE_SIZE + 1];
      //> yyyyddmm
      //<
   char   theSatNumber[SAT_NUMBER_SIZE + 1];
      //> L4, L5, 1B, 1C
      //<
   char   theInstrumentType[INSTRUMENT_TYPE_SIZE + 1];
      //> TM, LISS1 LISS2, LISS3, PAN, WIFS
      //<
   char   theProductType[PRODUCT_TYPE_SIZE + 1]; // MAP or ORBIT oriented
   char   theProductSize[PRODUCT_SIZE_SIZE + 1]; // FULL, SUB, MAP
      //> SYSTEMATIC, PRECISION, TERRAIN RADIOMETRIC
      //<
   char   theMapSheetName[MAP_SHEET_SIZE + 1];
   char   theProcessingType[PROCESSING_TYPE_SIZE + 1];
      //> SYSTEMATIC, PRECISION, TERRAIN RADIOMETRIC
      //<
   char   theResampAlgorithm[RESAMPLING_ALGO_SIZE + 1]; // CC, NN or BL
   char   theBandRadiance[NUMBER_OF_BANDS][RADIANCE_SIZE + 1];
   char   theVolumeNumber[VOLUME_NUMBER_SIZE + 1];
   int    the1stLineInVolume;
   int    theLinesPerVolume;
   double theOrientationAngle;   // degrees
   char   theMapProjName[MAP_PROJ_NAME_SIZE + 1];
   int    theUsgsProjNumber;
   int    theUsgsMapZone;
   char   theUsgsProjParam[NUMBER_OF_PROJECTION_PARAMETERS]
                          [USGS_PROJ_PARAMS_SIZE + 1];
      //>  USGS Projection Parameters.  These are actually a string
      //   representing doubles; however, EOSAT uses a "D" for the
      //   exponent notation, so they are read in as a char*.
      //   To convert to double use the method "projParam".
      //<
   char   theEllipsoid[ELLIPSOID_SIZE + 1];
   double theSemiMajorAxis;
   double theSemiMinorAxis;
   double theGsd;             // pixel size in meters
   int    thePixelsPerLine;   // samples
   int    theLinesPerImage;   // lines

   char    theUlLon[LON_SIZE + 1]; // deg, min, sec
   char    theUlLat[LAT_SIZE + 1]; // deg, min, sec
   double  theUlEasting;           // meters
   double  theUlNorthing;          // meters
   
   char    theUrLon[LON_SIZE + 1]; // deg, min, sec
   char    theUrLat[LAT_SIZE + 1]; // deg, min, sec
   double  theUrEasting;           // meters
   double  theUrNorthing;          // meters
   
   char    theLrLon[LON_SIZE + 1]; // deg, min, sec
   char    theLrLat[LAT_SIZE + 1]; // deg, min, sec
   double  theLrEasting;           // meters
   double  theLrNorthing;          // meters
   
   char    theLlLon[LON_SIZE + 1]; // deg, min, sec
   char    theLlLat[LAT_SIZE + 1]; // deg, min, sec
   double  theLlEasting;           // meters
   double  theLlNorthing;          // meters

   char    theBandsPresentString[BANDS_PRESENT_SIZE + 1];
   int     theBlockingFactor;
   int     theRecordSize;
   int     theSunElevation;        // degrees
   int     theSunAzimuth;          // degrees

   char    theCenterLon[LON_SIZE + 1]; // deg, min, sec
   char    theCenterLat[LAT_SIZE + 1]; // deg, min, sec
   double  theCenterEasting;           // meters
   double  theCenterNorthing;          // meters
   int     theCenterSample;       // Nearest whole pixel.
   int     theCenterLine;         // Nearest whole pixel.
   int     theOffset;   // pixels
   char    theFormatVersion[FORMAT_VERSION_SIZE + 1];

private:
   void loadFromStream(istream& is);

   int checkStream(istream& is);

   ErrorStatus theErrorStatus;
};
   
#endif
