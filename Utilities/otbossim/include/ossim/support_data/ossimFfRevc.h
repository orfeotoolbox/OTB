//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Ken Melero (kmelero@imagelinks.com)
//         Orginally written by Dave Burken (dburken@imagelinks.com)
// Description: This class parses an EOSAT Fast Format rev c header.
//
//********************************************************************
// $Id: ossimFfRevc.h 9967 2006-11-29 02:01:23Z gpotts $

#ifndef ossimFfRevc_HEADER
#define ossimFfRevc_HEADER
#include <iostream>
using namespace std;

// Forward class declarations.
class ossimString;
      
//***************************************************************************
// CLASS: ossimFfRevc.h
//***************************************************************************

class ossimFfRevc
{
public:
   ossimFfRevc  ();
   ossimFfRevc  (const char* headerFile);
   ~ossimFfRevc (){};

   enum ErrorStatus
   {
      OSSIM_OK    = 0,
      OSSIM_ERROR = 1
   };

   //>
   // Enumerations for parsing header.  Contains size in bytes
   // for all fields.
   // 
   // NOTE:  Sizes are the actual number of bytes for that field so add
   //        one for trailing '\0' to get the string length for the
   //        data member that are character arrays.
   //<

   enum
   {
      NUMBER_OF_BANDS                 = 8,
      NUMBER_OF_SCENES                = 4,
      NUMBER_OF_PROJECTION_PARAMETERS = 15
   };
   
   enum
   {
      // Adminstrative record:
      PRODUCT_ORDER_NUMBER_DESC_SIZE      = 12,
      PRODUCT_ORDER_NUMBER_SIZE           = 11,
      LOCATION_DESC_SIZE                  = 11,
      PATH_ROW_NUMBER_SIZE                = 17,
      DATE_DESC_SIZE                      = 19,
      DATE_SIZE                           = 8,
      SAT_NAME_DESC_SIZE                  = 11,
      SAT_NAME_SIZE                       = 10,
      SENSOR_NAME_DESC_SIZE               = 9,
      SENSOR_NAME_SIZE                    = 10,
      SENSOR_MODE_DESC_SIZE               = 14,
      SENSOR_MODE_SIZE                    = 6,
      LOOK_ANGLE_DESC_SIZE                = 13,
      OFF_NADIR_ANGLE_SIZE                = 6,
      PRODUCT_TYPE_DESC_SIZE              = 14,
      PRODUCT_TYPE_SIZE                   = 18,
      PRODUCT_SIZE_DESC_SIZE              = 15,
      PRODUCT_SIZE_SIZE                   = 10,
      PROCESSING_TYPE_DESC_SIZE           = 20,
      PROCESSING_TYPE_SIZE                = 11,
      RESAMPLING_ALGO_DESC_SIZE           = 13,
      RESAMPLING_ALGO_SIZE                = 2,
      TAPE_VOLUME_NUMBER_DESC_SIZE        = 19,
      TAPE_VOLUME_NUMBER_SIZE             = 2,
      VOLUMES_PER_TAPE_SIZE               = 2,
      PIXELS_PER_LINE_DESC_SIZE           = 18,
      PIXELS_PER_LINE_SIZE                = 5,
      LINES_PER_IMAGE_DESC_SIZE           = 17,
      LINES_PER_IMAGE_SIZE                = 5,
      FIRST_LINE_DESC_SIZE                = 14,
      FIRST_LINE_IN_VOLUME_SIZE           = 5,
      BLOCKING_FACTOR_DESC_SIZE           = 18,
      BLOCKING_FACTOR_SIZE                = 2,
      RECORD_LENGTH_DESC_SIZE             = 16,
      RECORD_LENGTH_SIZE                  = 5,
      PIXEL_GSD_DESC_SIZE                 = 13,
      PIXEL_GSD_SIZE                      = 6,
      BITS_PER_PIXEL_DESC_SIZE            = 23,
      BITS_PER_PIXEL_SIZE                 = 2,  // Output and Acquired
      AQUIRED_BITS_PER_PIXEL_DESC_SIZE    = 26,
      BANDS_PRESENT_DESC_SIZE             = 15,
      BANDS_PRESENT_SIZE                  = 33,
      REV_DESC_SIZE                       = 15,
      FORMAT_VERSION_SIZE                 = 1,
      // Radiometric record:
      BIAS_GAIN_DESC_SIZE                 = 50, 
      BIAS_SIZE                           = 24,
      GAIN_SIZE                           = 24,
      // Geometric record:
      GEO_DESC_SIZE                       = 14,
      MAP_PROJECTION_NAME_DESC_SIZE       = 17,
      MAP_PROJECTION_NAME_SIZE            = 4,
      ELLIPSOID_DESC_SIZE                 = 12,
      ELLIPSOID_SIZE                      = 18,
      DATUM_DESC_SIZE                     = 8,
      DATUM_SIZE                          = 6,
      PROJECTION_PARAMETER_DESC_SIZE      = 28,
      PROJECTION_PARAMETER_SIZE           = 24,
      CORNER_DESC_SIZE                    = 4,
      CENTER_DESC_SIZE                    = 8,
      LON_SIZE                            = 13,
      LAT_SIZE                            = 12,
      EASTING_SIZE                        = 13,
      NORTHING_SIZE                       = 13,
      CENTER_SAMPLE_SIZE                  = 5,
      CENTER_LINE_SIZE                    = 5,
      HORIZONTAL_OFFSET_DESC_SIZE         = 8,
      HORIZONTAL_OFFSET_SIZE              = 6,
      ORIENTATION_ANGLE_DESC_SIZE         = 20,
      ORIENTATION_ANGLE_SIZE              = 6,
      SUN_ELEVATION_DESC_SIZE             = 21,
      SUN_ELEVATION_SIZE                  = 4,
      SUN_AZIMUTH_DESC_SIZE               = 20,
      SUN_AZIMUTH_SIZE                    = 5
   };

   //---
   //>
   // Enumerations for byte offsets in header.  All offsets are relative to
   // the first byte of the first record which is byte 0.
   //<
   
   enum
   {
      // Administrative record:  bytes 0 - 1535
      PRODUCT_ORDER_NUMBER_OFFSET           = 12,
      PRODUCT_TYPE_OFFSET                   = 654,
      PRODUCT_SIZE_OFFSET                   = 687,
      PROCESSING_TYPE_OFFSET                = 740,
      RESAMPLING_ALGO_OFFSET                = 764,
      TAPE_VOLUME_NUMBER_OFFSET             = 819,
      VOLUMES_PER_TAPE_OFFSET               = 822,
      PIXELS_PER_LINE_OFFSET                = 842,
      LINES_PER_IMAGE_OFFSET                = 864,
      FIRST_LINE_IN_VOLUME_OFFSET           = 894,
      BLOCKING_FACTOR_OFFSET                = 917,
      RECORD_LENGTH_OFFSET                  = 935,
      PIXEL_GSD_OFFSET                      = 954,
      BITS_PER_PIXEL_OFFSET                 = 983,
      ACQUIRED_BITS_PER_PIXEL_OFFSET        = 1011,
      BANDS_PRESENT_OFFSET                  = 1055,
      FORMAT_VERSION_OFFSET                 = 1535,
      // Radiometric record:  bytes 1536 - 3071
      // See file scope constants for bias and gain OFFSET positions.
      // Geometric record:  bytes 3072 - 4607
      MAP_PROJECTION_NAME_OFFSET    = 3103,
      ELLIPSOID_OFFSET              = 3119,  
      DATUM_OFFSET                  = 3145,
      UL_LON_OFFSET                 = 3637,
      UL_LAT_OFFSET                 = 3651,
      UL_EASTING_OFFSET             = 3664,
      UL_NORTHING_OFFSET            = 3678,
      UR_LON_OFFSET                 = 3717,
      UR_LAT_OFFSET                 = 3731,
      UR_EASTING_OFFSET             = 3744,
      UR_NORTHING_OFFSET            = 3758,
      LR_LON_OFFSET                 = 3797,
      LR_LAT_OFFSET                 = 3811,
      LR_EASTING_OFFSET             = 3824,
      LR_NORTHING_OFFSET            = 3838,
      LL_LON_OFFSET                 = 3877,
      LL_LAT_OFFSET                 = 3891,
      LL_EASTING_OFFSET             = 3904,
      LL_NORTHING_OFFSET            = 3918,
      CENTER_LON_OFFSET             = 3961,
      CENTER_LAT_OFFSET             = 3975,
      CENTER_EASTING_OFFSET         = 3988,
      CENTER_NORTHING_OFFSET        = 4002,
      CENTER_SAMPLE_OFFSET          = 4016,
      CENTER_LINE_OFFSET            = 4022,
      HORIZONTAL_OFFSET_OFFSET      = 4040,
      ORIENTATION_ANGLE_OFFSET      = 4066, 
      SUN_ELEVATION_OFFSET          = 4133, 
      SUN_AZIMUTH_OFFSET            = 4157
   };

   //---
   //>
   // An EOSAT Fast Format Rev C header consists of three
   // records:  administrative, radiometric and geometric
   // each record has a class defined for it to hold the pertinent
   // data.
   //<
   
   class AdminRecord
   {
   public:
      AdminRecord();
      ~AdminRecord(){}

      //  Data Members for record.
      
      char   theProductOrderNumber[PRODUCT_ORDER_NUMBER_SIZE + 1];

      char   thePathRowNumber[NUMBER_OF_SCENES][PATH_ROW_NUMBER_SIZE + 1];   
      char   theAcquisitionDate[NUMBER_OF_SCENES][DATE_SIZE + 1];
         //> yyyyddmm
         //<
      char   theSatName[NUMBER_OF_SCENES][SAT_NAME_SIZE + 1];
         //> L4, L5, 1B, 1C
         //<
      char   theSensorName[NUMBER_OF_SCENES][SENSOR_NAME_SIZE + 1];
         //> TM, LISS1 LISS2, LISS3, PAN, WIFS
         //<
      char   theSensorMode[NUMBER_OF_SCENES][SENSOR_MODE_SIZE + 1];
      double theOffNadirAngle[NUMBER_OF_SCENES];  // degrees

      char   theProductType[PRODUCT_TYPE_SIZE + 1]; // MAP or ORBIT oriented
      char   theProductSize[PRODUCT_SIZE_SIZE + 1]; // FULL, SUB, MAP
         //> SYSTEMATIC, PRECISION, TERRAIN RADIOMETRIC
         //<
      char   theProcessingType[PROCESSING_TYPE_SIZE + 1];
      char   theResampAlgorithm[RESAMPLING_ALGO_SIZE + 1]; // CC or NN
      int    theTapeVolumeNumber;
      int    theNumberVolumesPerTape;
      int    thePixelsPerLine;
      int    theLinesPerImage;
      int    the1stLineInVolume;
      int    theTapeBlockingFactor;
      int    theRecordSize;
      double theGsd; // pixel size in meters
      int    theOutputBitsPerPixel;
      int    theAcquiredBitsPerPixel;
      char   theBandsPresentString[BANDS_PRESENT_SIZE + 1];
      char   theFormatVersion[FORMAT_VERSION_SIZE + 1];
   };

   class RadiomRecord
   {
   public:
      RadiomRecord();
      ~RadiomRecord(){}
      

      double theBias[8];
      double theGain[8];
   };

   class GeoRecord
   {
   public:
      GeoRecord();
      ~GeoRecord(){}
      
      char    theMapProjectionName[MAP_PROJECTION_NAME_SIZE + 1];
      char    theEllipsoid[ELLIPSOID_SIZE + 1];
      char    theDatum[DATUM_SIZE + 1];
      double  theProjectionParams[15];
   
      char    theUlLon[LON_SIZE + 1]; // deg, min, sec
      char    theUlLat[LAT_SIZE + 1]; // deg, min, sec
      double  theUlEasting;
      double  theUlNorthing;
   
      char    theUrLon[LON_SIZE + 1]; // deg, min, sec
      char    theUrLat[LAT_SIZE + 1]; // deg, min, sec
      double  theUrEasting;
      double  theUrNorthing;
   
      char    theLrLon[LON_SIZE + 1]; // deg, min, sec
      char    theLrLat[LAT_SIZE + 1]; // deg, min, sec
      double  theLrEasting;
      double  theLrNorthing;
   
      char    theLlLon[LON_SIZE + 1]; // deg, min, sec
      char    theLlLat[LAT_SIZE + 1]; // deg, min, sec
      double  theLlEasting;
      double  theLlNorthing;

      char    theCenterLon[LON_SIZE + 1]; // deg, min, sec
      char    theCenterLat[LAT_SIZE + 1]; // deg, min, sec
      double  theCenterEasting;
      double  theCenterNorthing;
      int     theCenterSample;       // Nearest whole pixel.
      int     theCenterLine;         // Nearest whole pixel.

      int     theHorizontalOffset;   // pixels
      double  theOrientationAngle;   // degrees
      double  theSunElevationAngle;  // degrees
      double  theSunAzimuth;         // degrees
   };

   AdminRecord  theAdminRecord;
   RadiomRecord theRadiomRecord;
   GeoRecord    theGeoRecord;
   
   int errorStatus() const { return theErrorStatus; }

   void print(ostream& os) const;

   void write(ostream& os) const;

   friend ostream& operator<<(ostream& os, const ossimFfRevc& head);


   //>
   // Convenience methods.
   //<
   
   int    path(int sceneNbr=0) const;
      //> Returns the path as an int.
      //  Currently uses the1stPathRowNumber string.
      //<
   int    row(int sceneNbr=0) const;
      //> Returns the row as an int from the pathRowString.
      //  Currently uses the1stPathRowNumber string.
      //<
   int    fraction(int sceneNbr=0) const;
      //> Returns the fraction as an int from the pathRowString.
      //  Currently uses the1stPathRowNumber string.
      //<
   ossimString subScene(int sceneNbr=0) const;
      //> Returns the sub scene as an string from the pathRowString.
      //  Currently uses the1stPathRowNumber string.
      //<

   ossimString pathRow(int sceneNbr=0) const;
      //> Returns a string represnting the path and row as a name.
      //  i.e.: 287/05100D0 will be returned as p287r05100D0
      //  This method will remove any spaces.
      //<

private:
   void loadFromStream(istream& is);

   int checkStream(istream& is);

   ErrorStatus theErrorStatus;
};

   
#endif
