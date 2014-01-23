message(STATUS "Importing Mapnik...")
option(OTB_USE_MAPNIK "Use mapnik library." OFF)
mark_as_advanced(OTB_USE_MAPNIK)

if(OTB_USE_MAPNIK)
        if(NOT OTB_USE_EXTERNAL_BOOST)
          message(FATAL_ERROR "You need to set OTB_USE_EXTERNAL_BOOST to ON when using MAPNIK")
        endif()

        find_path(MAPNIK_INCLUDE_DIR mapnik/map.hpp PATHS)
        mark_as_advanced(MAPNIK_INCLUDE_DIR)

        find_library(MAPNIK_LIBRARY NAMES mapnik2 mapnik)
        mark_as_advanced(MAPNIK_LIBRARY)

        find_path(FREETYPE2_INCLUDE_DIR freetype/freetype.h PATHS /usr/include/freetype2)
        mark_as_advanced(FREETYPE2_INCLUDE_DIR)

        # libicuuc (http://site.icu-project.org/) is a unicode library mapnik relies on.
        # mapnik includes drag icu includes
        # when linking with mapnik, need to link with libicuuc (at least since 1.42 release)
        find_path(ICUUC_INCLUDE_DIR unicode/unistr.h)
        mark_as_advanced(ICUUC_INCLUDE_DIR)

        find_library(ICUUC_LIBRARY icuuc)
        mark_as_advanced(ICUUC_LIBRARY)

        # ltdl (libtool dl)
        find_path(LTDL_INCLUDE_DIR ltdl.h)
        mark_as_advanced(LTDL_INCLUDE_DIR)

        find_library(LTDL_LIBRARY ltdl)
        mark_as_advanced(LTDL_LIBRARY)

        if(NOT MAPNIK_INCLUDE_DIR)
                message(FATAL_ERROR
                        "Cannot find MAPNIK include directory. Please set MAPNIK_INCLUDE_DIR or set OTB_USE_MAPNIK OFF.")
        endif()
        if(NOT MAPNIK_LIBRARY)
                message(FATAL_ERROR
                        "Cannot find MAPNIK library. Please set MAPNIK_LIBRARY or set OTB_USE_MAPNIK OFF.")
        endif()

        if(NOT FREETYPE2_INCLUDE_DIR)
                message(FATAL_ERROR
                        "Cannot find FREETYPE2 include directory. Please set FREETYPE2_INCLUDE_DIR or set OTB_USE_MAPNIK OFF.")
        endif()

        if(NOT ICUUC_INCLUDE_DIR)
                message(FATAL_ERROR
                        "Cannot find ICUUC_INCLUDE_DIR include directory. Please set ICUUC_INCLUDE_DIR or set OTB_USE_MAPNIK OFF.")
        endif()
        if(NOT ICUUC_LIBRARY)
                message(FATAL_ERROR
                        "Cannot find ICUUC library, needed by MAPNIK. Please set ICUUC_LIBRARY or set OTB_USE_MAPNIK OFF.")
        endif()

        if(NOT LTDL_INCLUDE_DIR)
                message(FATAL_ERROR
                        "Cannot find LTDL_INCLUDE_DIR include directory. Please set LTDL_INCLUDE_DIR or set OTB_USE_MAPNIK OFF.")
        endif()
        if(NOT LTDL_LIBRARY)
                message(FATAL_ERROR
                        "Cannot find ICUUC library, needed by MAPNIK. Please set ICUUC_LIBRARY or set OTB_USE_MAPNIK OFF.")
        endif()


        file(READ "${MAPNIK_INCLUDE_DIR}/mapnik/version.hpp" _mapnik_version_hpp_CONTENTS)
        STRING(REGEX REPLACE ".*#define MAPNIK_MAJOR_VERSION ([0-9]+).*" "\\1" MAPNIK_MAJOR_VERSION  ${_mapnik_version_hpp_CONTENTS})
        STRING(REGEX REPLACE ".*#define MAPNIK_MINOR_VERSION ([0-9]+).*" "\\1" MAPNIK_MINOR_VERSION  ${_mapnik_version_hpp_CONTENTS})
        STRING(REGEX REPLACE ".*#define MAPNIK_PATCH_VERSION ([0-9]+).*" "\\1" MAPNIK_PATCH_VERSION  ${_mapnik_version_hpp_CONTENTS})
        #make version number as in version.hpp of mapnik
        #//#define MAPNIK_VERSION (MAPNIK_MAJOR_VERSION*100000) + (MAPNIK_MINOR_VERSION*100) + (MAPNIK_PATCH_VERSION)
        #if no MAPNIK_MAJOR_VERSION macro is defined then
        #we read only MAPNIK_VERSION macro which contains the version number. This is for version less than 2.1
        #as major, minor, patch are introduced in version 2.1
        #See mapnik commit - https://github.com/mapnik/mapnik/commit/2abe02bd96104988ea76869d1203aa270cf9a68c
        if(MAPNIK_MAJOR_VERSION)
          MATH(EXPR OTB_MAPNIK_VERSION "(${MAPNIK_MAJOR_VERSION}*100000) + (${MAPNIK_MINOR_VERSION}*100) + (${MAPNIK_PATCH_VERSION})" )
          #make MAPNIK_VERSION to have a standard version style eg: 2.2.0 instead of 020020
          SET(MAPNIK_VERSION ${MAPNIK_MAJOR_VERSION}.${MAPNIK_MINOR_VERSION}.${MAPNIK_PATCH_VERSION})
       else()
         STRING(REGEX REPLACE ".*#define MAPNIK_VERSION ([0-9]+).*" "\\1" MAPNIK_VERSION  ${_mapnik_version_hpp_CONTENTS})
       endif()
        #should we check for 2.0 here instead of 2.2 to include agg2
        if(${OTB_MAPNIK_VERSION} LESS "200200")
          message(STATUS "Does not support mapnik2 interface:  ${MAPNIK_VERSION}")
          add_definitions(-DOTB_MAPNIK_COMPATIBILITY_API07)
          # This should be dropped when we don't want to support this any more
          # Estimated date: 02/2013.
        else()
          find_path(AGG_INCLUDE_DIR agg_pixfmt_rgba.h PATHS /usr/include /usr/include/agg2)
          mark_as_advanced(AGG_INCLUDE_DIR)
          if(NOT AGG_INCLUDE_DIR)
            message(FATAL_ERROR
              "Cannot find AGG headers, needed by MAPNIK. Please set AGG_INCLUDE_DIR or set OTB_USE_MAPNIK OFF.")
          endif()
          include_directories(${AGG_INCLUDE_DIR})
          SET(OTB_MAPNIK_SUPPORTS_API20 TRUE)
        endif()

        # Add compiler option
        add_definitions(-DOTB_USE_MAPNIK)

        include_directories(${MAPNIK_INCLUDE_DIR} ${ICUUC_INCLUDE_DIR} ${LTDL_INCLUDE_DIR} ${FREETYPE2_INCLUDE_DIR})

        message(STATUS "  Enabling Mapnik support")
        message(STATUS "  Mapnik version: ${MAPNIK_VERSION}")
        message(STATUS "  Mapnik includes : ${MAPNIK_INCLUDE_DIR}")
        message(STATUS "  Mapnik library  : ${MAPNIK_LIBRARY}")
        message(STATUS "  ICU includes : ${ICUUC_INCLUDE_DIR}")
        message(STATUS "  ICU library  : ${ICUUC_LIBRARY}")
        message(STATUS "  LTDL includes : ${LTDL_INCLUDE_DIR}")
        message(STATUS "  LTDL library  : ${LTDL_LIBRARY}")

else()

    message(STATUS "  Disabling Mapnik support")

endif()
