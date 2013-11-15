###############################################################################
#
# CMake module to search for GeoTIFF library
#
# On success, the macro sets the following variables:
# GEOTIFF_FOUND       = if the library found
# GEOTIFF_LIBRARIES   = full path to the library
# GEOTIFF_INCLUDE_DIR = where to find the library headers also defined,
#                       but not for general use are
# GEOTIFF_LIBRARY     = where to find the PROJ.4 library.
# GEOTIFF_VERSION     = version of library which was found, e.g. "1.2.5"
#
# Copyright (c) 2009 Mateusz Loskot <mateusz@loskot.net>
#
# Module source: http://github.com/mloskot/workshop/tree/master/cmake/
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
###############################################################################
message(STATUS "Searching for GeoTIFF ${GeoTIFF_FIND_VERSION}+ library")

if(GEOTIFF_INCLUDE_DIR)
  # Already in cache, be silent
  set(GEOTIFF_FIND_QUIETLY TRUE)
endif()

if(WIN32)
  set(OSGEO4W_IMPORT_LIBRARY geotiff_i)
  if(DEFINED ENV{OSGEO4W_ROOT})
    set(OSGEO4W_ROOT_DIR $ENV{OSGEO4W_ROOT})
    message(STATUS "Trying OSGeo4W using environment variable OSGEO4W_ROOT=$ENV{OSGEO4W_ROOT}")
  else()
    set(OSGEO4W_ROOT_DIR c:/OSGeo4W)
    message(STATUS "Trying OSGeo4W using default location OSGEO4W_ROOT=${OSGEO4W_ROOT_DIR}")
  endif()
endif()

find_path(GEOTIFF_INCLUDE_DIR
  geotiff.h
  PATH_PREFIXES geotiff
  PATHS
  ${OSGEO4W_ROOT_DIR}/include)

set(GEOTIFF_NAMES ${OSGEO4W_IMPORT_LIBRARY} geotiff)

find_library(GEOTIFF_LIBRARY
  NAMES ${GEOTIFF_NAMES}
  PATHS
  ${OSGEO4W_ROOT_DIR}/lib)

if(GEOTIFF_FOUND)
  set(GEOTIFF_LIBRARIES ${GEOTIFF_LIBRARY})
endif()

if(GEOTIFF_INCLUDE_DIR)
  set(GEOTIFF_VERSION 0)

  set(GEOTIFF_VERSION_H "${GEOTIFF_INCLUDE_DIR}/geotiff.h")
  file(READ ${GEOTIFF_VERSION_H} GEOTIFF_VERSION_H_CONTENTS)

  if(DEFINED GEOTIFF_VERSION_H_CONTENTS)
    string(REGEX REPLACE ".*#define[ \t]LIBGEOTIFF_VERSION[ \t]+([0-9]+).*" "\\1" GEOTIFF_VERSION_NUM "${GEOTIFF_VERSION_H_CONTENTS}")

    if(NOT ${GEOTIFF_VERSION_NULL} MATCHES "[0-9]+")
      message(FATAL_ERROR "GeoTIFF version parsing failed!")
    endif()

    if(GEOTIFF_VERSION_NUM AND NOT "${GEOTIFF_VERSION_NUM}" STREQUAL "0")
      math(EXPR GTIFF_VERSION_MAJOR "${GEOTIFF_VERSION_NUM} / 1000")
      math(EXPR GTIFF_VERSION_MINOR "${GEOTIFF_VERSION_NUM} % 1000 / 100")
      math(EXPR GTIFF_VERSION_PATCH "${GEOTIFF_VERSION_NUM} % 100 / 10")
    endif()

    set(GEOTIFF_VERSION "${GTIFF_VERSION_MAJOR}.${GTIFF_VERSION_MINOR}.${GTIFF_VERSION_PATCH}"
      CACHE INTERNAL "The version string for GeoTIFF library")

    if(GEOTIFF_VERSION VERSION_EQUAL GeoTIFF_FIND_VERSION OR
        GEOTIFF_VERSION VERSION_GREATER GeoTIFF_FIND_VERSION)
      message(STATUS "Found GeoTIFF version: ${GEOTIFF_VERSION}")
    else()
      message(FATAL_ERROR "GeoTIFF version check failed. Version ${GEOTIFF_VERSION} was found, at least version ${GeoTIFF_FIND_VERSION} is required")
    endif()
  else()
    message(FATAL_ERROR "Failed to open ${GEOTIFF_VERSION_H} file")
  endif()

endif()

# Handle the QUIETLY and REQUIRED arguments and set GEOTIFF_FOUND to TRUE
# if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GeoTIFF DEFAULT_MSG GEOTIFF_LIBRARY GEOTIFF_INCLUDE_DIR)
