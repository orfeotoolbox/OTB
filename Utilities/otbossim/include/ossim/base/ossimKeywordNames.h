//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file
//
// Author: Ken Melero (kmelero@remotesensing.org)
//
// Description: Common file for global Keywors.
//
//*************************************************************************
// $Id: ossimKeywordNames.h 19180 2011-03-22 17:36:33Z oscarkramer $

#ifndef ossimKeywordNames_HEADER
#define ossimKeywordNames_HEADER

#include <ossim/base/ossimConstants.h>

class OSSIMDLLEXPORT ossimKeywordNames
{
public:
   //***
   // NOTE:  List keyword in alphabetical order.
   //***
   static const char* AZIMUTH_ANGLE_KW;
   static const char* BAND_KW;
   static const char* BANDS_KW;
   static const char* BORDER_SIZE_KW;
   static const char* BRUSH_COLOR_KW;
   static const char* BYTE_ORDER_KW;
   static const char* CE90_ABSOLUTE_KW;
   static const char* CE90_RELATIVE_KW;
   static const char* CENTER_PIXEL_X_KW;
   static const char* CENTER_PIXEL_Y_KW;
   static const char* CENTRAL_MERIDIAN_KW;
   static const char* CENTRAL_POINT1_LAT_KW;
   static const char* CENTRAL_POINT1_LON_KW;
   static const char* CENTRAL_POINT2_LAT_KW;
   static const char* CENTRAL_POINT2_LON_KW;
   static const char* COMPRESS_KW;
   static const char* COMPRESSION_QUALITY_KW;
   static const char* COMPRESSION_TYPE_KW;
   static const char* CONVERGENCE_THRESHOLD_KW;
   static const char* CREATE_EX_KW;
   static const char* CREATE_EXTERNAL_GEOMETRY_KW;
   static const char* CREATE_IMAGE_KW;
   static const char* CREATE_HISTOGRAM_KW;   
   static const char* CREATE_OVERVIEW_KW;
   static const char* DATA_FILE_KW;
   static const char* DATE_KW;
   static const char* DATE_YEAR_KW;
   static const char* DATE_MONTH_KW;
   static const char* DATE_DAY_KW;
   static const char* DATE_HOUR_KW;
   static const char* DATE_MIN_KW;
   static const char* DATE_SEC_KW;
   static const char* DATE_FRACT_SEC_KW;
   static const char* DATUM_KW;
   static const char* DECIMAL_DEGREES_PER_PIXEL_LAT;
   static const char* DECIMAL_DEGREES_PER_PIXEL_LON;
   static const char* DESCRIPTION_KW;
   static const char* ELEVATION_ANGLE_KW;
   static const char* ELEVATION_CELL_KW;
   static const char* ELEVATION_SOURCE_KW;
   static const char* ELEVATION_LOOKUP_FLAG_KW;
   static const char* ELLIPSE_CODE_KW;
   static const char* ELLIPSE_EPSG_CODE_KW;
   static const char* ELLIPSE_NAME_KW;
   static const char* ENABLED_KW;
   static const char* ENABLE_CACHE_KW;
   static const char* EXTRACT_VERTICES_KW;  
   static const char* ENTRY_KW;
   static const char* FALSE_EASTING_NORTHING_KW;
   static const char* FALSE_EASTING_NORTHING_UNITS_KW;
   static const char* FALSE_EASTING_KW;
   static const char* FALSE_NORTHING_KW;
   static const char* FEATURE_NAME_KW;
   static const char* FILENAME_KW;
   static const char* FILEPATH_KW;
   static const char* FILL_FLAG_KW;
   static const char* FRAME_INDEX_KW;       
   static const char* GCS_CODE_KW;
   static const char* GEOM_FILE_KW;
   static const char* HEMISPHERE_KW;
   static const char* HORIZONTAL_SIZE_KW;
   static const char* ID_KW;
   static const char* IMAGE_CE90_KW;
   static const char* IMAGE_FILE_KW;
   static const char* IMAGE_ID_KW;
   static const char* IMAGE_MODEL_TRANSFORM_MATRIX_KW;
   static const char* IMAGE_MODEL_TRANSFORM_UNIT_KW;
   static const char* IMAGE_PATH_KW;
   static const char* IMAGE_TYPE_KW;
   static const char* IMAGE_DATE_KW;
   static const char* IMAGE_FILE_SIZE_KW;
   static const char* INPUT_RR_LEVEL_KW;
   static const char* INTERLEAVE_TYPE_KW;
   static const char* JULIAN_DAY_KW;
   static const char* LAT_INCREMENT_KW;
   static const char* LL_LAT_KW;
   static const char* LL_LON_KW;
   static const char* LR_LAT_KW;
   static const char* LR_LON_KW;
   static const char* LL_X_KW;
   static const char* LL_Y_KW;
   static const char* LR_X_KW;
   static const char* LR_Y_KW;
   static const char* LON_INCREMENT_KW;
   static const char* MAJOR_AXIS_KW;
   static const char* MAX_VALUE_KW;
   static const char* MAX_ITERATIONS_KW;
   static const char* MAX_QUADTREE_LEVELS_KW;
   static const char* METADATA_TYPE_KW;
   static const char* METERS_PER_PIXEL_X_KW;
   static const char* METERS_PER_PIXEL_Y_KW;
   static const char* MINOR_AXIS_KW;
   static const char* MIN_VALUE_KW;
   static const char* NULL_VALUE_KW;
   static const char* NUMBER_BANDS_KW;
   static const char* NUMBER_ENTRIES_KW;
   static const char* NUMBER_INPUT_BANDS_KW;
   static const char* NUMBER_INPUTS_KW;
   static const char* NUMBER_OUTPUTS_KW;
   static const char* NUMBER_OUTPUT_BANDS_KW;
   static const char* NUMBER_LINES_KW;
   static const char* NUMBER_REDUCED_RES_SETS_KW;
   static const char* NUMBER_SAMPLES_KW;
   static const char* ORIGIN_LATITUDE_KW;
   static const char* ORIGIN_X_KW;
   static const char* ORIGIN_Y_KW;
   static const char* OUTPUT_FILE_KW;
   static const char* OUTPUT_FILE_PREFIX_KW;
   static const char* OUTPUT_FILE_EXTENSION_KW;
   static const char* OUTPUT_TILE_SIZE_KW;
   static const char* OVERVIEW_COMPRESSION_QUALITY_KW;
   static const char* OVERVIEW_COMPRESSION_TYPE_KW;
   static const char* OVERVIEW_FILE_KW;
   static const char* OVERVIEW_STOP_DIMENSION_KW;
   static const char* PCS_CODE_KW;
   static const char* PEN_COLOR_KW;
   static const char* PHOTOMETRIC_KW;
   static const char* PIXEL_SCALE_XY_KW;
   // static const char* PIXEL_SCALE_Y_KW;
   static const char* PIXEL_SCALE_UNITS_KW;
   static const char* PIXEL_TYPE_KW;
   static const char* PLANAR_CONFIG_KW;
   static const char* POINT_WIDTH_HEIGHT_KW;
   static const char* PROJECTION_KW;
   static const char* QUALITY_KW;
   static const char* QUERY_KW;
   static const char* RADIOMETRY_KW;
   static const char* REDUCED_RES_LEVEL_KW;
   static const char* REF_GPT_LAT_KW;
   static const char* REF_GPT_LON_KW;
   static const char* REF_GPT_HGT_KW;
   static const char* REF_IPT_LINE_KW;
   static const char* REF_IPT_SAMP_KW;
   static const char* ROTATION_KW;
   static const char* SCALAR_TYPE_KW;
   static const char* SCALE_PER_PIXEL_X_KW;
   static const char* SCALE_PER_PIXEL_Y_KW;
   static const char* SCALE_FACTOR_KW;
   static const char* SCALE_X_KW;
   static const char* SCALE_Y_KW;
   static const char* SCALE_FACTOR_X_KW;
   static const char* SCALE_FACTOR_Y_KW;
   static const char* SENSOR_ID_KW;
   static const char* SRS_NAME_KW;
   static const char* STD_PARALLEL_1_KW;
   static const char* STD_PARALLEL_2_KW;
   static const char* THICKNESS_KW;
   static const char* TIE_POINT_EASTING_KW;
   static const char* TIE_POINT_NORTHING_KW;
   static const char* TIE_POINT_XY_KW;
   // static const char* TIE_POINT_Y_KW;
   static const char* TIE_POINT_UNITS_KW;
   
   static const char* TIE_POINT_LAT_KW;
   static const char* TIE_POINT_LON_KW;
   static const char* TILE_SOURCE_KW;
   static const char* TILE_TYPE_KW;
   static const char* TILE_SIZE_X_KW;
   static const char* TILE_SIZE_Y_KW;
   static const char* TRANSLATION_X_KW;
   static const char* TRANSLATION_Y_KW;
   static const char* TYPE_KW;
   static const char* UL_LAT_KW;
   static const char* UL_LON_KW;
   static const char* UNITS_KW;
  
   static const char* UR_LAT_KW;
   static const char* UR_LON_KW;
   static const char* UL_X_KW;
   static const char* UL_Y_KW;
   static const char* UR_X_KW;
   static const char* UR_Y_KW;
   static const char* VALID_VERTICES_KW;
   static const char* VERTICAL_SIZE_KW;
   static const char* ZONE_KW;

protected:
   ossimKeywordNames();
   ossimKeywordNames(const ossimKeywordNames& rhs);
   const ossimKeywordNames& operator =(const ossimKeywordNames &rhs);
};

#endif
