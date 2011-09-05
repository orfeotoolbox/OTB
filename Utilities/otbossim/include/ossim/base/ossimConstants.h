/********************************************************************
 *
 * License:  See top level LICENSE.txt file.
 *
 * Author: Ken Melero
 * 
 * Description: Common file for global constants.
 *
 **************************************************************************
 * $Id: ossimConstants.h 19793 2011-06-30 13:26:56Z gpotts $
 */
#ifndef ossimConstants_HEADER
#define ossimConstants_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus   
#include <cfloat>
#else
#include <float.h>
#endif
#include <ossim/ossimConfig.h>

#ifdef NULL
#undef NULL
#define NULL 0
#endif
/**
 * WARNINGS SECTION:
 */
#ifdef _MSC_VER /* Quiet a bunch of MSVC warnings... */
#  pragma warning(disable:4786) /* visual c6.0 compiler */
#  pragma warning(disable:4251)/* for std:: member variable to have dll interface */
#  pragma warning(disable:4275) /* for std:: base class to have dll interface */
#  pragma warning(disable:4800) /* int forcing value to bool */
#  pragma warning(disable:4244) /* conversion, possible loss of data */
#endif
#if defined(__GNUC__)
#   define OSSIM_DEPRECATE_METHOD(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#   define OSSIM_DEPRECATE_METHOD(func)  __declspec(deprecated) func 
#else
#   define OSSIM_DEPRECATE_METHOD(func)
#endif
   
/**
 * DLL IMPORT/EXORT SECTION
 */
#if defined(OSSIM_STATIC)
#  define OSSIMEXPORT
#  define OSSIMIMPORT
#  define OSSIMDLLEXPORT
#  define OSSIM_DLL
#  define OSSIMDLLEXPORT_DATA(type) type
#  define OSSIM_DLL_DATA(type) type
#  define OSSIMDLLEXPORT_CTORFN 
#elif defined(__MINGW32__) || defined(__CYGWIN__) || defined(_MSC_VER) || defined(__VISUALC__) || defined(__BORLANDC__) || defined(__WATCOMC__)
#  define OSSIMEXPORT __declspec(dllexport)
#  define OSSIMIMPORT __declspec(dllimport)
#  ifdef OSSIMMAKINGDLL
#    define OSSIMDLLEXPORT OSSIMEXPORT
#    define OSSIM_DLL       OSSIMEXPORT
#    define OSSIMDLLEXPORT_DATA(type) OSSIMEXPORT type
#    define OSSIM_DLL_DATA(type) OSSIMEXPORT type
#    define OSSIMDLLEXPORT_CTORFN
#  else
#    define OSSIMDLLEXPORT OSSIMIMPORT
#    define OSSIM_DLL      OSSIMIMPORT
#    define OSSIMDLLEXPORT_DATA(type) OSSIMIMPORT type
#    define OSSIM_DLL_DATA(type) OSSIMIMPORT type
#    define OSSIMDLLEXPORT_CTORFN
#  endif
#else /* not #if defined(_MSC_VER) */
#  define OSSIMEXPORT
#  define OSSIMIMPORT
#  define OSSIMDLLEXPORT
#  define OSSIM_DLL
#  define OSSIMDLLEXPORT_DATA(type) type
#  define OSSIM_DLL_DATA(type) type
#  define OSSIMDLLEXPORT_CTORFN
#endif /* #if defined(_MSC_VER) */
   
/**
 * Previous DLL import export section.  Commented out, but left just in case.
 */
#if 0 /* Comment out ALL this mess! */
#if defined(_MSC_VER) || defined(__VISUALC__) || defined(__BORLANDC__) || defined(__GNUC__) || defined(__WATCOMC__)
#  if (_MSC_VER >= 1300)	/* MSVC .NET 2003 version */
#    define OSSIMEXPORT __declspec(dllexport)
#    define OSSIMIMPORT __declspec(dllimport)
#  else
#    define OSSIMEXPORT __declspec(dllexport)
#    define OSSIMIMPORT __declspec(dllimport)
#  endif
#else /* compiler doesn't support __declspec() */
#   define OSSIMEXPORT
#   define OSSIMIMPORT
#endif

#if defined(__WXPM__)
#  if defined (__WATCOMC__)
#    define OSSIMEXPORT __declspec(dllexport)
 /*
   __declspec(dllimport) prepends __imp to imported symbols. We do NOT
   want that!
 */
#    define OSSIMIMPORT
#  elif (!(defined(__VISAGECPP__) && (__IBMCPP__ < 400 || __IBMC__ < 400 )))
#    define OSSIMEXPORT _Export
#    define OSSIMIMPORT _Export
#  endif
#elif defined(__OSSIMMAC__)
#  ifdef __MWERKS__
#    define OSSIMEXPORT __declspec(export)
#    define OSSIMIMPORT __declspec(import)
#  endif
#endif

#if defined(_MSC_VER)
#  pragma warning(disable:4786) /* visual c6.0 compiler */
#  pragma warning(disable:4251) /* for std:: member variable to have dll interface */
#  pragma warning(disable:4275) /* for std:: base class to have dll interface */
#  pragma warning(disable:4800) /* int forcing value to bool */
#  pragma warning(disable:4244) /* conversion, possible loss of data */

#  ifdef OSSIMSINGLEDLL /* one gigantic dll, all declared export */
#    ifdef EXPORT_OSMMATRIX
#      define EXPORT_OSMMATRIX  OSSIMEXPORT
#    endif
#    ifndef OSSIMDLLEXPORT
#      define OSSIMDLLEXPORT    OSSIMEXPORT
#    endif
#    ifdef EXPORT_OSMELEV
#      define EXPORT_OSMELEV    OSSIMEXPORT
#    endif
#    ifdef EXPORT_OSMSPTDATA
#      define EXPORT_OSMSPTDATA OSSIMEXPORT
#    endif
#    ifdef EXPORT_ISO8211
#      define EXPORT_ISO8211    OSSIMEXPORT
#    endif
#    ifdef EXPORT_OSMPROJ
#      define EXPORT_OSMPROJ    OSSIMEXPORT
#    endif
#    ifndef EXPORT_OSMIMAGING
#      define EXPORT_OSMIMAGING OSSIMEXPORT
#    endif
#  else /* individual dlls, each with their own import/export symbols */
#    ifndef EXPORT_OSMMATRIX
#      ifdef OSMMATRIX_EXPORTS
#        define EXPORT_OSMMATRIX OSSIMEXPORT
#      else
#        define EXPORT_OSMMATRIX OSSIMIMPORT
#      endif
#    endif
#    ifndef OSSIMDLLEXPORT
#      ifdef OSMBASE_EXPORTS
#        define OSSIMDLLEXPORT OSSIMEXPORT
#      else
#        define OSSIMDLLEXPORT OSSIMIMPORT
#      endif
#    endif
#    ifndef EXPORT_OSMELEV
#      ifdef OSMELEV_EXPORTS
#        define EXPORT_OSMELEV OSSIMEXPORT
#      else
#        define EXPORT_OSMELEV OSSIMIMPORT
#      endif
#    endif
#    ifndef EXPORT_OSMSPTDATA
#      ifdef OSMSPTDATA_EXPORTS
#        define EXPORT_OSMSPTDATA OSSIMEXPORT
#      else
#        define EXPORT_OSMSPTDATA OSSIMIMPORT
#      endif
#    endif
#    ifndef EXPORT_OSMPROJ
#      ifdef OSMPROJ_EXPORTS
#        define EXPORT_OSMPROJ OSSIMEXPORT
#      else
#        define EXPORT_OSMPROJ OSSIMIMPORT
#      endif
#    endif
#    ifndef EXPORT_ISO8211
#      ifdef ISO8211_EXPORTS
#        define EXPORT_ISO8211 OSSIMEXPORT
#      else
#        define EXPORT_ISO8211 OSSIMIMPORT
#      endif
#    endif
#    ifndef EXPORT_OSMIMAGING
#      ifdef OSMIMAGING_EXPORTS
#        define EXPORT_OSMIMAGING OSSIMEXPORT
#      else
#        define EXPORT_OSMIMAGING OSSIMIMPORT
#      endif
#    endif
#  endif
#else /* defined(_MSC_VER) */
#  ifdef OSSIMMAKINGDLL
#    define OSSIMDLLEXPORT OSSIMEXPORT
#    define OSSIM_DLL       OSSIMEXPORT
#    define OSSIMDLLEXPORT_DATA(type) OSSIMEXPORT type
#    define OSSIM_DLL_DATA(type) OSSIMEXPORT type
#    define OSSIMDLLEXPORT_CTORFN
#  elif defined(OSSIMUSINGDLL)
#    define OSSIMDLLEXPORT OSSIMIMPORT
#    define OSSIM_DLL      OSSIMIMPORT
#    define OSSIMDLLEXPORT_DATA(type) OSSIMIMPORT type
#    define OSSIM_DLL_DATA(type) OSSIMIMPORT type
#    define OSSIMDLLEXPORT_CTORFN
#  else /* not making nor using DLL */
#    define OSSIMDLLEXPORT
#    define OSSIM_DLL
#    define OSSIM_DLL_DATA(type) type
#    define OSSIMDLLEXPORT_DATA(type) type
#    define OSSIMDLLEXPORT_CTORFN
#  endif
#  define EXPORT_OSMMATRIX  OSSIMDLLEXPORT
#  define EXPORT_OSMELEV    OSSIMDLLEXPORT
#  define EXPORT_OSMSPTDATA OSSIMDLLEXPORT
#  define EXPORT_OSMPROJ    OSSIMDLLEXPORT
#  define EXPORT_OSMIMAGING OSSIMDLLEXPORT
#endif
#endif /* End of commented out "#if 0" old DLL import/export section. */

/*
  we know that if this is defined all other types are defined
  since its included from ossimConfig.h
*/
typedef char                   ossim_int8;
typedef unsigned char          ossim_uint8;
typedef signed char            ossim_sint8;

typedef short                  ossim_int16;
typedef unsigned short         ossim_uint16;
typedef signed short           ossim_sint16;

typedef int                    ossim_int32;
typedef unsigned int           ossim_uint32;
typedef signed int             ossim_sint32;

typedef float                  ossim_float32;
typedef double                 ossim_float64;


typedef long long              ossim_int64;
typedef unsigned long long     ossim_uint64;
typedef signed long long       ossim_sint64;
   
typedef ossim_int32            ossimErrorCode;

enum ossimVertexOrdering
{
   OSSIM_VERTEX_ORDER_UNKNOWN    = 0,
   OSSIM_CLOCKWISE_ORDER         = 1,
   OSSIM_COUNTERCLOCKWISE_ORDER  = 2
};

//---
// For histogram builders.  Note that FAST computation mode will not sample all tiles.
//---   
enum ossimHistogramMode
{
   OSSIM_HISTO_MODE_UNKNOWN = 0,
   OSSIM_HISTO_MODE_NORMAL  = 1,
   OSSIM_HISTO_MODE_FAST    = 2
};

/*
 Corner coordinates are relative to center (0) of pixel
 or relative to the upper left of pixel (1).
*/
enum ossimPixelType
{
   OSSIM_PIXEL_IS_POINT = 0,
   OSSIM_PIXEL_IS_AREA  = 1
};

/*
 Definitions for scalar type identification.
*/
enum ossimScalarType
{
   OSSIM_SCALAR_UNKNOWN    =  0, 
   OSSIM_UINT8             =  1, /**< 8 bit unsigned integer        */
   OSSIM_SINT8             =  2, /**< 8 bit signed integer          */
   OSSIM_UINT16            =  3, /**< 16 bit unsigned integer       */
   OSSIM_SINT16            =  4, /**< 16 bit signed integer         */
   OSSIM_UINT32            =  5, /**< 32 bit unsigned integer       */
   OSSIM_SINT32            =  6, /**< 32 bit signed integer         */
   OSSIM_FLOAT32           =  7, /**< 32 bit floating point         */
   OSSIM_FLOAT64           =  8, /**< 64 bit floating point         */
   OSSIM_CINT16            =  9, /**< 16 bit complex integer        */
   OSSIM_CINT32            = 10, /**< 32 bit complex integer        */
   OSSIM_CFLOAT32          = 11, /**< 32 bit complex floating point */
   OSSIM_CFLOAT64          = 12, /**< 64 bit complex floating point */

   //---
   // Below for backward compatibility only.  Please use above enums in
   // conjunction with null, min, max settings to determine bit depth.
   //---
   OSSIM_UCHAR             = 1,  /**< 8 bit unsigned iteger */
   OSSIM_USHORT16          = 3,  /**< 16 bit unsigned iteger */
   OSSIM_SSHORT16          = 4,  /**< 16 bit signed integer */
   OSSIM_USHORT11          = 13, /**< 16 bit unsigned integer (11 bits used) */
   OSSIM_FLOAT             = 7,  /**< 32 bit floating point */
   OSSIM_NORMALIZED_FLOAT  = 18, /**< normalized floating point  32 bit */
   OSSIM_DOUBLE            = 8,  /**< double 64 bit    */
   OSSIM_NORMALIZED_DOUBLE = 20  /**< Normalized double 64 bit */
};

/*
 Definitions for interleave type identification.
*/
enum ossimInterleaveType
{
   OSSIM_INTERLEAVE_UNKNOWN = 0,
   OSSIM_BIL                = 1,  /* band interleaved by line */
   OSSIM_BIP                = 2,  /* band interleaved by pixel */
   OSSIM_BSQ                = 3,  /* band sequential */
   OSSIM_BSQ_MULTI_FILE     = 4   /* band sequential in separate files */
};

/*
 Definitions for origin location.
*/
enum ossimOriginLocationType
{
   OSSIM_ORIGIN_LOCATION_UNKNOWN = 0,
   OSSIM_CENTER                  = 1,
   OSSIM_UPPER_LEFT              = 2,
   OSSIM_UPPER_LEFT_CENTER       = 3,
   OSSIM_LOWER_LEFT              = 4,
   OSSIM_LOWER_LEFT_CENTER       = 5
};

/*
 Definitions for coordinate system type.
*/
enum ossimCoordinateSystemType
{
   OSSIM_COORDINATE_SYSTEM_UNKNOWN = 0,
   OSSIM_IMAGE_SPACE               = 1,
   OSSIM_GEOGRAPHIC_SPACE          = 2,
   OSSIM_PROJECTED_SPACE           = 3,
   OSSIM_GEOCENTRIC_SPACE          = 4
};

/*
 Definitions for coordinate system orientation mode .
*/
enum ossimCoordSysOrientMode
{
   OSSIM_LEFT_HANDED  = 0,
   OSSIM_RIGHT_HANDED = 1
};

/*
 Definitions for unit type.
*/
enum ossimUnitType
{
   OSSIM_UNIT_UNKNOWN    = 0,
   OSSIM_METERS          = 1,
   OSSIM_FEET            = 2,
   OSSIM_US_SURVEY_FEET  = 3,
   OSSIM_DEGREES         = 4,
   OSSIM_RADIANS         = 5,
   OSSIM_NAUTICAL_MILES  = 6,
   OSSIM_SECONDS         = 7,
   OSSIM_MINUTES         = 8,
   OSSIM_PIXEL           = 9,
   OSSIM_MILES           = 10,
   OSSIM_MILLIMETERS     = 11,
   OSSIM_MICRONS         = 12,
   OSSIM_CENTIMETERS     = 13,
   OSSIM_YARDS           = 14,
   OSSIM_INCHES          = 15,
   OSSIM_KILOMETERS      = 16
};

/*
 Definitions for data object status.
*/
enum ossimDataObjectStatus
{
   OSSIM_STATUS_UNKNOWN = 0,
   OSSIM_NULL           = 1, // not initialized
   OSSIM_EMPTY          = 2, // initialized but blank or empty
   OSSIM_PARTIAL        = 3, // contains some null/invalid values
   OSSIM_FULL           = 4  // all valid data
};

/** Definitions for image type identification. */
enum ossimImageType
{
   OSSIM_IMAGE_TYPE_UNKNOWN       = 0,
   OSSIM_TIFF_STRIP               = 1,
   OSSIM_TIFF_STRIP_BAND_SEPARATE = 2,
   OSSIM_TIFF_TILED               = 3,
   OSSIM_TIFF_TILED_BAND_SEPARATE = 4,
   OSSIM_GENERAL_RASTER_BIP       = 5,
   OSSIM_GENERAL_RASTER_BIL       = 6,
   OSSIM_GENERAL_RASTER_BSQ       = 7,
   OSSIM_JPEG                     = 8
};

/** Definitions for metadata type identification. */
enum ossimMetadataType
{
   OSSIM_META_UNKNOWN   = 0,
   OSSIM_META_TFW       = 1,
   OSSIM_META_JGW       = 2,
   OSSIM_META_GEOM      = 3,
   OSSIM_META_README    = 4,
   OSSIM_META_FGDC      = 5,
   OSSIM_META_ENVI      = 6
};

enum ossimByteOrder
{
   OSSIM_LITTLE_ENDIAN = 0,
   OSSIM_BIG_ENDIAN    = 1
};

enum ossimCompareType
{
   OSSIM_COMPARE_FULL=0,      // compares full traversal if supported.  Not just instance pointers
   OSSIM_COMPARE_IMMEDIATE=1  // Only immediate attributes are compared
};
   
#ifndef M_PI
#  define M_PI             ((ossim_float64)3.141592653589793238462643)
#endif
#ifndef TWO_PI
#  define TWO_PI           ((ossim_float64)(2.0 * M_PI))
#endif
#ifndef DEG_PER_RAD
#  define DEG_PER_RAD      ((ossim_float64)(180.0/M_PI))
#endif
#ifndef SEC_PER_RAD
#  define SEC_PER_RAD      ((ossim_float64)206264.8062471)
#endif
#ifndef RAD_PER_DEG
#  define RAD_PER_DEG      ((ossim_float64)(M_PI/180.0))
#endif
#ifndef MTRS_PER_FT
#  define MTRS_PER_FT      ((ossim_float64)0.3048)
#endif
#ifndef FT_PER_MTRS
#  define FT_PER_MTRS      ((ossim_float64)3.280839895013122)
#endif
#ifndef FT_PER_MILE
#  define FT_PER_MILE      ((ossim_float64)5280.0)
#endif
#ifndef SQRT_TWO_PI
#  define SQRT_TWO_PI      ((ossim_float64)2.50662827463100050242)
#endif
#ifndef SQRT_TWO_PI_DIV2
#  define SQRT_TWO_PI_DIV2 ((ossim_float64)7.07106781186547524401E-1)
#endif

#define TIFFTAG_OSSIM_METADATA 50955
#define OSSIM_DEFAULT_TILE_HEIGHT  ((ossim_int32)64)
#define OSSIM_DEFAULT_TILE_WIDTH   ((ossim_int32)64)

/*
 NOTE Reference for U.S. feet value:
 U.S.G.S. "Map Projections - A Working Manual"
 USGS Professional paper 1395 page 51
*/
#ifndef US_METERS_PER_FT
#  define  US_METERS_PER_FT ((ossim_float64)0.3048006096)
#endif

//---
// Integer nan kept for ossimIpt.
// This should be the most negative int: -2147483648
//---
#define OSSIM_INT_NAN ((ossim_sint32)0x80000000)

#define OSSIM_DEFAULT_NULL_PIX_UCHAR ((ossim_uint8)0)
#define OSSIM_DEFAULT_MIN_PIX_UCHAR ((ossim_uint8)1)
#define OSSIM_DEFAULT_MAX_PIX_UCHAR ((ossim_uint8)255)

#define OSSIM_DEFAULT_NULL_PIX_UINT8 ((ossim_uint8)0)
#define OSSIM_DEFAULT_MIN_PIX_UINT8 ((ossim_uint8)1)
#define OSSIM_DEFAULT_MAX_PIX_UINT8 ((ossim_uint8)255)

#define OSSIM_DEFAULT_NULL_PIX_SINT8 ((ossim_sint8)0x80)
#define OSSIM_DEFAULT_MIN_PIX_SINT8 ((ossim_sint8)0x81)
#define OSSIM_DEFAULT_MAX_PIX_SINT8 ((ossim_sint8)0x7F)

#define OSSIM_DEFAULT_NULL_PIX_SINT16 ((ossim_sint16)0x8000)
#define OSSIM_DEFAULT_MIN_PIX_SINT16 ((ossim_sint16)0x8001)
#define OSSIM_DEFAULT_MAX_PIX_SINT16 ((ossim_sint16)0x7FFF)

#define OSSIM_DEFAULT_NULL_PIX_UINT16 ((ossim_uint16)0)
#define OSSIM_DEFAULT_MIN_PIX_UINT16 ((ossim_uint16)1)
#define OSSIM_DEFAULT_MAX_PIX_UINT16 ((ossim_uint16)0xFFFF)

#define OSSIM_DEFAULT_NULL_PIX_SINT32 ((ossim_sint32)0x80000000)
#define OSSIM_DEFAULT_MIN_PIX_SINT32 ((ossim_sint32)0x80000001)
#define OSSIM_DEFAULT_MAX_PIX_SINT32 ((ossim_sint32)0x7FFFFFFF)

#define OSSIM_DEFAULT_NULL_PIX_UINT32 ((ossim_uint32)0)
#define OSSIM_DEFAULT_MIN_PIX_UINT32 ((ossim_uint32)1)
#define OSSIM_DEFAULT_MAX_PIX_UINT32 ((ossim_uint32)0xFFFFFFFF)

#define OSSIM_DEFAULT_NULL_PIX_UINT11 ((ossim_uint16)0)
#define OSSIM_DEFAULT_MIN_PIX_UINT11 ((ossim_uint16)1)
#define OSSIM_DEFAULT_MAX_PIX_UINT11 ((ossim_uint16)0x07FF)

#define OSSIM_DEFAULT_NULL_PIX_FLOAT    ((ossim_float32)-1.0/FLT_EPSILON)
#define OSSIM_DEFAULT_MIN_PIX_FLOAT ((ossim_float32)((-1.0/FLT_EPSILON) + 1))
#define OSSIM_DEFAULT_MAX_PIX_FLOAT  ((ossim_float32)((1.0/FLT_EPSILON)))

#define OSSIM_DEFAULT_NULL_PIX_NORM_FLOAT ((ossim_float32)0)
#define OSSIM_DEFAULT_MIN_PIX_NORM_FLOAT ((ossim_float32)((2*FLT_EPSILON)))
#define OSSIM_DEFAULT_MAX_PIX_NORM_FLOAT ((ossim_float32)1.0)

#define OSSIM_DEFAULT_NULL_PIX_DOUBLE ((ossim_float64)-1.0/DBL_EPSILON)
#define OSSIM_DEFAULT_MIN_PIX_DOUBLE ((ossim_float64)((-1.0/DBL_EPSILON) + 1))
#define OSSIM_DEFAULT_MAX_PIX_DOUBLE ((ossim_float64)((1.0/DBL_EPSILON)))

#define OSSIM_DEFAULT_NULL_PIX_NORM_DOUBLE ((ossim_float64)0)
#define OSSIM_DEFAULT_MIN_PIX_NORM_DOUBLE ((ossim_float64)((2*DBL_EPSILON)))
#define OSSIM_DEFAULT_MAX_PIX_NORM_DOUBLE ((ossim_float64)(1.0))

#define OSSIM_DEFAULT_MEAN_SEA_LEVEL ((ossim_float64)0.0)

#ifdef __cplusplus
}
#endif

#endif
