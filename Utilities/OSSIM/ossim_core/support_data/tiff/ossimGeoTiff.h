//***************************************************************************
// FILE: ossimGeoTiff.h
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
// Class definition for ossimGeoTiff which is designed to read and hold tag
// information.
//
//***************************************************************************
// $Id: ossimGeoTiff.h,v 1.12 2005/09/01 14:34:02 gpotts Exp $

#ifndef ossimGeoTiff_HEADER
#define ossimGeoTiff_HEADER

#include <stdio.h>
#include <vector>
using namespace std;

#include "base/common/ossimErrorStatusInterface.h"
#include "base/misc/ossimEndian.h"
#include "base/data_types/ossimString.h"

class ossimFilename;
class ossimKeywordlist;


class ossimGeoTiff : public ossimErrorStatusInterface
{
public:
   ossimGeoTiff(const ossimFilename& file);
   ~ossimGeoTiff();
  
   enum
   {
      UNDEFINED                        = 0,
      PIXEL_IS_AREA                    = 1,
      PIXEL_IS_POINT                   = 2,
      GT_MODEL_TYPE_GEO_KEY            = 1024,
      GT_RASTER_TYPE_GEO_KEY           = 1025,
      GEOGRAPHIC_TYPE_GEO_KEY          = 2048,
      GEOG_GEODETIC_DATUM_GEO_KEY      = 2050,
      GEOG_ANGULAR_UNITS_GEO_KEY       = 2054,
      PROJECTED_CS_TYPE_GEO_KEY        = 3072,
      PCS_CITATION_GEO_KEY             = 3073,
      PROJECTION_GEO_KEY               = 3074,
      PROJ_COORD_TRANS_GEO_KEY         = 3075,
      LINEAR_UNITS_GEO_KEY             = 3076,
      PROJ_STD_PARALLEL1_GEO_KEY       = 3078,
      PROJ_STD_PARALLEL2_GEO_KEY       = 3079,
      PROJ_NAT_ORIGIN_LONG_GEO_KEY     = 3080,
      PROJ_NAT_ORIGIN_LAT_GEO_KEY      = 3081,
      PROJ_FALSE_EASTING_GEO_KEY       = 3082,
      PROJ_FALSE_NORTHING_GEO_KEY      = 3083,
      PROJ_CENTER_LONG_GEO_KEY         = 3088,
      PROJ_CENTER_LAT_GEO_KEY          = 3089,
      PROJ_SCALE_AT_NAT_ORIGIN_GEO_KEY = 3092,
      LINEAR_METER                     = 9001,
      LINEAR_FOOT                      = 9002,
      LINEAR_FOOT_US_SURVEY            = 9003,
      ANGULAR_DEGREE                   = 9102,
      ANGULAR_ARC_MINUTE               = 9103,
      ANGULAR_ARC_SECOND               = 9104,
      ANGULAR_GRAD                     = 9105,
      ANGULAR_GON                      = 9106,
      ANGULAR_DMS                      = 9107,
      ANGULAR_DMS_HEMISPHERE           = 9108,
      PCS_BRITISH_NATIONAL_GRID        = 27700,
      USER_DEFINED                     = 32767,
      MODEL_PIXEL_SCALE_TAG            = 33550,
      MODEL_TIE_POINT_TAG              = 33922,
      MODEL_TRANSFORM_TAG              = 34264,
      GEO_KEY_DIRECTORY_TAG            = 34735,
      GEO_DOUBLE_PARAMS_TAG            = 34736,
      GEO_ASCII_PARAMS_TAG             = 34737,
   };

   enum CompressType
   {
      NOT_COMPRESSED = 0,
      COMPRESSED     = 1
   };
 
   enum PhotoInterpretation
   {
      PHOTO_MINISWHITE  = 0,   // min value is white 
      PHOTO_MINISBLACK  = 1,   // min value is black 
      PHOTO_RGB         = 2,   // RGB color model 
      PHOTO_PALETTE     = 3,   // color map indexed 
      PHOTO_MASK        = 4,   // $holdout mask 
      PHOTO_SEPARATED   = 5,   // !color separations 
      PHOTO_YCBCR       = 6,   // !CCIR 601 
      PHOTO_CIELAB      = 8    // !1976 CIE L*a*b*
   };

   enum ModelType
   {
      UNKNOWN               = 0,
      MODEL_TYPE_PROJECTED  = 1,  // Projection Coordinate System
      MODEL_TYPE_GEOGRAPHIC = 2,  // Geographic latitude-longitude System 
      MODEL_TYPE_GEOCENTRIC = 3
   };


   /**
    *  Reads tags.
    *  Returns true on success, false on error.
    */
   bool readTags(const ossimFilename& file);

   /**
    *  Returns the map zone as an interger.
    */
   int mapZone() const;

   /**
    *  Add geometry info from tags to keword list.
    *  Returns true on success, false on error.
    */
   bool addImageGeometry(ossimKeywordlist& kwl,
                         const char* prefix=0) const;

   /**
    *  Returns an ossimString representing the ossim projection name.
    *  Returns "unknown" if it can't find a match.
    */
   ossimString getOssimProjectionName() const;

   /**
    *  Attempts to set the ossim projection name from keys read.
    */
   void setOssimProjectionName();

   /**
    *  Returns an ossimString representing the ossim datum name code.
    *  Returns "unknown" if it can't find a match.
    */
   ossimString getOssimDatumName() const;

   /**
    *  Attempts to set the ossim datum code.
    */
   void setOssimDatumName();

   /**
    * Returns enumerated byte order of the tiff file.
    * Either OSSIM_LITTLE_ENDIAN or OSSIM_BIG_ENDIAN.
    * See ossimConstants.h
    */
   ossimByteOrder getByteOrder() const;

   void getScale(vector<double>& scale) const;
   void getTiePoint(vector<double>& tie_point) const;
   void getModelTransformation(vector<double>& transform) const;

   const vector<double>& getTiePoint() const;
   const vector<double>& getModelTransformation() const;
   const vector<double>& getScale() const;

   int getWidth() const;
   int getLength() const;
   int getSamplesPerPixel() const;
   
private:

   // Disallow ...
   ossimGeoTiff(const ossimGeoTiff* rhs) {}
   ossimGeoTiff& operator=(const ossimGeoTiff& rhs) { return *this; }
   
   /**
    *  Reads geo keys.
    *  Returns true on success, false on error.
    */
   bool readGeoKeys(ifstream& str, std::streampos offset, int length);

   /**
    *  Initializes s reference.  Does byte swapping as needed.
    */
   void readShort(ossim_uint16& s, ifstream& str);
   
   /**
    *  Initializes l reference.  Does byte swapping as needed.
    */
   void readLong(ossim_int32& l, ifstream& str);   
   

   /**
    *  Initializes l reference.  Does byte swapping as needed.
    */
   void readLong(ossim_uint32& l, ifstream& str);
      
   /**
    *  Initializes l reference.  Does byte swapping as needed.
    */
   void readDouble(ossim_float64& d, ifstream& str);   

   /**
    *  Attempts to parse the pcs code (3072).
    */
   void parsePcsCode(int code);

   /**
    *  Attempts to parse the projection geo key (3074).
    */
   void parseProjGeoCode(int code);

   /**
    *  Converts double passed in to meters if needed.  The conversion is
    *  base on "theLiniarUnitsCode".
    */
   double convert2meters(double d) const;
      
   ossim_uint32          theGeoKeyOffset;
   int                   theGeoKeyLength;
   ossimByteOrder        theTiffByteOrder;
   ossimEndian           theEndian;
   bool                  theGeoKeysPresentFlag;
   int                   theZone;
   ossimString           theHemisphere;
   int                   theDoubleParamLength;
   int                   theAsciiParamLength;
   ossimString           theProjectionName;
   ossimString           theDatumName;
   
   vector<double>        theScale;                         // tag 33550
   vector<double>        theTiePoint;                      // tag 33922
   vector<double>        theModelTransformation;           // tag 34264
   double*               theDoubleParam;                   // tag 34736
   char*                 theAsciiParam;                    // tag 34737
   
   int                   theWidth;                         // tag 256
   int                   theLength;                        // tag 257
   vector<ossim_uint16>  theBitsPerSample;                 // tag 258
   CompressType          theCompresionType;                // tag 259
   PhotoInterpretation   thePhotoInterpretation;           // tag 262
   ossimString           theImageDescriptionString;        // tag 270
   int                   theSamplesPerPixel;               // tag 277
   int                   thePlanarConfig;                  // tag 284
   ossimString           theSoftwareDescriptionString;     // tag 305
   ossimString           theDateDescriptionString;         // tag 306
   int                   theTileWidth;                     // tag 322
   int                   theTileLength;                    // tag 323

   ModelType             theModelType;                     // key 1024
   int                   theRasterType;                    // key 1025
   int                   theGcsCode;                       // key 2048
   int                   theDatumCode;                     // key 2050
   int                   theAngularUnits;                  // key 2054
   int                   thePcsCode;                       // key 3072
   bool                  theSavePcsCodeFlag;
   ossimString           thePcsCitation;                   // key 3073
   int                   theProjGeoCode;                   // key 3074
   int                   theCoorTransGeoCode;              // key 3075
   int                   theLinearUnitsCode;               // key 3076
   double                theStdPar1;                       // key 3078 
   double                theStdPar2;                       // key 3079
   mutable double        theOriginLon;                     // key 3080
   mutable double        theOriginLat;                     // key 3081
   double                theFalseEasting;                  // key 3082
   double                theFalseNorthing;                 // key 3083
   mutable double        theCenterLon;                     // key 3088
   mutable double        theCenterLat;                     // key 3099
   double                theScaleFactor;                   // key 3092
};

#endif
