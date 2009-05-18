//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: TIFF Info object.
// 
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimTiffInfo_HEADER
#define ossimTiffInfo_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimInfoBase.h>
#include <ossim/base/ossimFilename.h>

class ossimEndian;

/**
 * @brief TIFF info class.
 *
 * Encapsulates the listgeo functionality.
 */
class OSSIM_DLL ossimTiffInfo : public ossimInfoBase
{
public:
   
   /** default constructor */
   ossimTiffInfo();

   /** virtual destructor */
   virtual ~ossimTiffInfo();

   enum
   {
      UNDEFINED                        = 0,
      PIXEL_IS_AREA                    = 1,
      PIXEL_IS_POINT                   = 2,
      TIFFTAG_SUBFILETYPE              = 254,
      TIFFTAG_IMAGEWIDTH               = 256,
      TIFFTAG_IMAGELENGTH              = 257,
      TIFFTAG_BITSPERSAMPLE            = 258,
      TIFFTAG_COMPRESSION              = 259,
      TIFFTAG_PHOTOMETRIC              = 262,
      TIFFTAG_IMAGEDESCRIPTION         = 270,
      TIFFTAG_MODEL                    = 272,
      TIFFTAG_STRIPOFFSETS             = 273,
      TIFFTAG_ORIENTATION	       = 274,
      TIFFTAG_SAMPLESPERPIXEL          = 277,
      TIFFTAG_ROWSPERSTRIP             = 278,
      TIFFTAG_STRIPBYTECOUNTS          = 279,
      TIFFTAG_MINSAMPLEVALUE           = 280,
      TIFFTAG_MAXSAMPLEVALUE           = 281,
      TIFFTAG_XRESOLUTION              = 282,
      TIFFTAG_YRESOLUTION              = 283,
      TIFFTAG_PLANARCONFIG             = 284,
      TIFFTAG_RESOLUTIONUNIT           = 296,
      TIFFTAG_DATETIME                 = 306,
      TIFFTAG_TILEWIDTH                = 322,
      TIFFTAG_TILELENGTH               = 323,
      TIFFTAG_TILEOFFSETS              = 324,
      TIFFTAG_TILEBYTECOUNTS           = 325,
      TIFFTAG_EXTRASAMPLES             = 338,
      TIFFTAG_SAMPLEFORMAT             = 339,
      TIFFTAG_SMINSAMPLEVALUE          = 340,
      TIFFTAG_SMAXSAMPLEVALUE          = 341,
      SAMPLEFORMAT_UINT                = 1,
      SAMPLEFORMAT_INT                 = 2,
      SAMPLEFORMAT_IEEEFP              = 3,
      SAMPLEFORMAT_VOID                = 4,
      SAMPLEFORMAT_COMPLEXINT          = 5,
      SAMPLEFORMAT_COMPLEXIEEEFP       = 6,
      TIFFTAG_SUBIFD                   = 330,
      GT_MODEL_TYPE_GEO_KEY            = 1024,
      GT_RASTER_TYPE_GEO_KEY           = 1025,
      GT_CITATION_GEO_KEY              = 1026,
      GEOGRAPHIC_TYPE_GEO_KEY          = 2048,
      GEOG_CITATION_GEO_KEY            = 2049,
      GEOG_GEODETIC_DATUM_GEO_KEY      = 2050,
      GEOG_LINEAR_UNITS_GEO_KEY        = 2052,
      GEOG_ANGULAR_UNITS_GEO_KEY       = 2054,
      GEOG_ELLIPSOID_GEO_KEY           = 2056,
      GEOG_SEMI_MAJOR_AXIS             = 2057,
      GEOG_SEMI_MINOR_AXIS             = 2058,
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
      TIFFTAG_COPYRIGHT                = 33432,
      MODEL_PIXEL_SCALE_TAG            = 33550,
      MODEL_TIE_POINT_TAG              = 33922,
      MODEL_TRANSFORM_TAG              = 34264,
      TIFFTAG_PHOTOSHOP                = 34377,
      GEO_KEY_DIRECTORY_TAG            = 34735,
      GEO_DOUBLE_PARAMS_TAG            = 34736,
      GEO_ASCII_PARAMS_TAG             = 34737
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

   enum WordType
   {
      TWO_OR_EIGHT  = 0,
      FOUR_OR_EIGHT = 1
   };

   enum
   {
      TIFF_NOTYPE = 0,      /* placeholder */
      TIFF_BYTE = 1,        /* 8-bit unsigned integer */
      TIFF_ASCII = 2,       /* 8-bit bytes w/ last byte null */
      TIFF_SHORT = 3,       /* 16-bit unsigned integer */
      TIFF_LONG = 4,        /* 32-bit unsigned integer */
      TIFF_RATIONAL = 5,    /* 64-bit unsigned fraction */
      TIFF_SBYTE = 6,       /* !8-bit signed integer */
      TIFF_UNDEFINED = 7,   /* !8-bit untyped data */
      TIFF_SSHORT = 8,      /* !16-bit signed integer */
      TIFF_SLONG = 9,       /* !32-bit signed integer */
      TIFF_SRATIONAL = 10,  /* !64-bit signed fraction */
      TIFF_FLOAT = 11,      /* !32-bit IEEE floating point */
      TIFF_DOUBLE = 12,     /* !64-bit IEEE floating point */
      TIFF_IFD = 13,        /* %32-bit unsigned integer (offset) */
      TIFF_LONG8 = 16,      /* BigTIFF 64-bit unsigned integer */
      TIFF_SLONG8 = 17,     /* BigTIFF 64-bit signed integer */
      TIFF_IFD8 = 18        /* BigTIFF 64-bit unsigned integer (offset) */
   };

   /**
    * @brief open method.
    *
    * @param file File name to open.
    *
    * @return true on success false on error.
    */
   virtual bool open(const ossimFilename& file);
   
   /**
    * Print method.
    *
    * @param out Stream to print to.
    * 
    * @return std::ostream&
    */
   virtual std::ostream& print(std::ostream& out) const;

private:
   
   /** Initializes s reference.  Does byte swapping as needed. */
   void readShort(ossim_uint16& s, std::ifstream& str) const;
   
   /** Initializes l reference.  Does byte swapping as needed. */
   void readLong(ossim_uint32& l, std::ifstream& str) const;
   
   /** Initializes l reference.  Does byte swapping as needed. */
   void readLongLong(ossim_uint64& l, std::ifstream& str) const;
   
   /** Attempts to parse the pcs code (3072). */
   void parsePcsCode(int code);
   
   /** Attempts to parse the projection geo key (3074). */
   void parseProjGeoCode(int code);
   
   /**
    *  Converts double passed in to meters if needed.  The conversion is
    *  base on "theLiniarUnitsCode".
    */
   double convert2meters(double d) const;
   
   /**
    * This will read either 4 or 8 bytes depending on the version and
    * initialize offset.
    *
    * @return true if stream is good, false if not.
    */
   bool getOffset(std::streamoff& offset, std::ifstream& str) const;
   
   /**
    * This will read either 2 , 4or 8 bytes depending on the version and
    * WORD_TYPE.
    *
    * @return true if stream is good, false if not.
    */
   bool getValue(ossim_uint64& val, std::ifstream& str, WordType type) const;
   
   ossim_uint64 getArraySizeInBytes(ossim_uint64 length,
                                    ossim_uint16 type) const;
   
   /**
    * @return The byte size of each tiff type or 0 if unhandled.
    */
   ossim_uint16 getTypeByteSize(ossim_uint16 type) const;
   
   /**
    * Eats the value field.  Either 4 or 8 bytes depending on the version.
    */
   void eatValue(std::ifstream& str) const;
   
   void swapBytes(ossim_uint8* v, ossim_uint16 type, ossim_uint64 count) const;
   
   /** Initializes v. */
   template <class T> void getArrayValue(T& v,
                                         ossim_uint8* array,
                                         ossim_uint64 position) const;
   
   std::ostream& print(std::ostream& out,
                       const std::string&  prefix,
                       ossim_uint64   tagIdx,
                       ossim_uint16   tag,
                       ossim_uint16   type,
                       ossim_uint64   count,
                       ossim_uint64   arraySizeInBytes,
                       ossim_uint8*   valueArray) const;
   
   std::ostream& printValue(std::ostream& out,
                            ossim_uint16 type,
                            ossim_uint8* valueArray) const;
   
   std::ostream& printArray(std::ostream& out,
                            ossim_uint16 type,
                            ossim_uint64 count,
                            ossim_uint8* valueArray) const;
   
   /** @brief Prints geo keys. */
   std::ostream& printGeoKeys(std::ostream& out,
                              const std::string& prefix,
                              ossim_uint64   geoKeyLength,
                              ossim_uint16*  geoKeyBlock,
                              ossim_uint64   geoDoubleLength,
                              ossim_float64* geoDoubleBlock,
                              ossim_uint64   geoAsciiLength,
                              ossim_int8*    geoAsciiBlock) const;
   

   /** @brief Prints key 1024 */
   std::ostream& printModelType(std::ostream& out,
                                const std::string& prefix,
                                ossim_uint16 code) const;

   /** @brief Prints key 1025 */
   std::ostream& printRasterType(std::ostream& out,
                                 const std::string& prefix,
                                 ossim_uint16 code) const;

   /** @brief Prints key 2054 */
   std::ostream& printAngularUnits(std::ostream& out,
                                   const std::string& prefix,
                                   ossim_uint16 code) const;

   /** @brief Prints key 3075 */
   std::ostream& printCoordTrans(std::ostream& out,
                                  const std::string& prefix,
                                  ossim_uint16 code) const;

   /** @brief Prints key 3076 */
   std::ostream& printLinearUnits(std::ostream& out,
                                  const std::string& prefix,
                                  ossim_uint16 code) const;

   /** @brief Prints tag 274 */
   std::ostream& printOrientation(std::ostream& out,
                                  const std::string& prefix,
                                  ossim_uint16 code) const;
  
   /** @brief adds imageN. to prefix where N is zero base directory index. */
   void getDirPrefix(ossim_int32 dirIndex, std::string& prefix) const;

   
   ossimFilename  theFile;
   ossimEndian*   theEndian;
   ossim_uint16   theVersion;
};

#endif /* End of "#ifndef ossimTiffInfo_HEADER" */
