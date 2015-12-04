PROJECT(ossimcore)
Message("################## Setting up OSSIM core library #########################")

IF(NOT OSSIM_BUILD_ADDITIONAL_OSSIM_DIRECTORIES)
   SET(OSSIM_BUILD_ADDITIONAL_OSSIM_DIRECTORIES "" CACHE PATH "Specify full paths separated by ; to additional OSSIM applications you wish to include into the ossim build framework.  These will have variables setup for the ossim includes" FORCE)
ENDIF(NOT OSSIM_BUILD_ADDITIONAL_OSSIM_DIRECTORIES)

cmake_minimum_required(VERSION 2.8)

SET(CMAKE_MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

INCLUDE(OssimVersion)
INCLUDE(OssimCommonVariables)

# Expose some build options
OPTION(BUILD_OSSIM_FREETYPE_SUPPORT "Set to ON to build OSSIM with freetype support.  Use OFF to turn off freetype support." ON)

OPTION(BUILD_OSSIM_MPI_SUPPORT "Set to ON to build OSSIM with MPI support.  Use OFF to turn off MPI support." ON)

OPTION(BUILD_OSSIM_ID_SUPPORT "Set to ON to build OSSIM SVN ID support into the library.  Use OFF to turn off ID support." ON)

OPTION(BUILD_OSSIM_APPS "Set to ON to build ossim apps.  Use OFF to disable." ON)
OPTION(BUILD_OSSIM_CURL_APPS "Set to ON to build ossim curl dependent apps.  Use ON to enable." OFF)
OPTION(BUILD_OSSIM_TEST_APPS "Set to ON to build ossim test apps.  Use OFF to disable." ON)

OPTION(BUILD_OSSIM_APPS "Set to ON to build ossim test apps.  Use OFF to disable." ON)


###################################################################################
# Include the Utilities in the root make
###################################################################################
INCLUDE(OssimUtilities)

INCLUDE(CheckIncludeFile)
CHECK_INCLUDE_FILE("dirent.h" CMAKE_HAVE_DIRENT_H)
CHECK_INCLUDE_FILE("unistd.h" CMAKE_HAVE_UNISTD_H)
CHECK_INCLUDE_FILE("getopt.h" CMAKE_HAVE_GETOPT_H)
CHECK_INCLUDE_FILE("fcntl.h" CMAKE_HAVE_FCNTL_H)
CHECK_INCLUDE_FILE("dbmalloc.h" CMAKE_HAVE_DBMALLOC_H)
CHECK_INCLUDE_FILE("malloc.h" CMAKE_HAVE_MALLOC_H)
CHECK_INCLUDE_FILE("dlfcn.h" CMAKE_HAVE_DLFCN_H)

# Set our include paths:
include_directories( ${PROJECT_SOURCE_DIR}/include )
include_directories( ${PROJECT_BINARY_DIR}/include )

# Stores list of libs to link with. Initialized throughout.
set( ossimDependentLibs )

#---
# Find required and optional packages and add their include paths and libraries:
#---

# GEOS - Currently optional until it is actually called.:
find_package( GEOS )
if( GEOS_FOUND )
   include_directories( ${GEOS_INCLUDE_DIR} )
   set( ossimDependentLibs ${ossimDependentLibs} ${GEOS_LIBRARY} )
else( GEOS_FOUND )
   message( FATAL_ERROR "Could not find geos package! Consider installing this as it will soon become a required package." )
endif( GEOS_FOUND )

# GEOFIFF - Required:
set( OSSIM_HAS_GEOTIFF 0 )
find_package( GEOTIFF )
if( GEOTIFF_FOUND )
   include_directories( ${GEOTIFF_INCLUDE_DIR} )
   set( ossimDependentLibs ${ossimDependentLibs} ${GEOTIFF_LIBRARY} )
   set( OSSIM_HAS_GEOTIFF 1 )
else( GEOTIFF_FOUND )
   message( FATAL_ERROR "Could not find required geotiff package!" )
endif( GEOTIFF_FOUND )

# GEOTRANS - Required: (GeoTrans 3.4.x port future...)
# find_package( Geotrans )
# if( GEOTRANS_FOUND )
#   include_directories( ${GEOTRANS_INCLUDE_DIR} )
#   set( ossimDependentLibs ${ossimDependentLibs} ${GEOTRANS_LIBRARY} )
# else( GEOTRANS_FOUND )
#   message( FATAL_ERROR "Could not find required geotrans package!" )
# endif( GEOTRANS_FOUND )

# JPEG - Required package:
find_package( JPEG )
if( JPEG_FOUND )
   include_directories( ${JPEG_INCLUDE_DIR} )
   set( ossimDependentLibs ${ossimDependentLibs} ${JPEG_LIBRARY} )
else( JPEG_FOUND )
   message( FATAL_ERROR "Could not find required jpeg package!" )
endif( JPEG_FOUND )

# OpenThreads - Required:
find_package( OpenThreads )
if( OPENTHREADS_FOUND )
   include_directories( ${OPENTHREADS_INCLUDE_DIR} )
   set( ossimDependentLibs ${ossimDependentLibs} ${OPENTHREADS_LIBRARY} )
else( OPENTHREADS_FOUND )
   message( FATAL_ERROR "Could not find required OpenThreads package!" )
endif( OPENTHREADS_FOUND )

# TIFF - Required:
find_package( TIFF )
if( TIFF_FOUND )
   include_directories( ${TIFF_INCLUDE_DIR} )
   set( ossimDependentLibs ${ossimDependentLibs} ${TIFF_LIBRARY} )
else( TIFF_FOUND )
   message( FATAL_ERROR "Could not find required tiff package!" )
endif( TIFF_FOUND )

# DL - Required on unix:
if( UNIX )
   find_library( DL_LIBRARY dl )
   if ( DL_LIBRARY )
      set( ossimDependentLibs ${ossimDependentLibs} ${DL_LIBRARY} )
   else( DL_LIBRARY )
      # TODO:  Is this required on unix?  If so move to the required section.
      message( FATAL_ERROR "Could not find dl library!" )
   endif( DL_LIBRARY )
endif( UNIX )

# FREETYPE - Optional:
set( OSSIM_HAS_FREETYPE 0 )
if( BUILD_OSSIM_FREETYPE_SUPPORT )
   find_package(Freetype)
   if( FREETYPE_FOUND )
      include_directories( ${FREETYPE_INCLUDE_DIRS} )
      set( ossimDependentLibs ${ossimDependentLibs} ${FREETYPE_LIBRARIES} )
      set( OSSIM_HAS_FREETYPE 1 )
   else( FREETYPE_FOUND )
      message( WARNING "Could not find optional freetype package!" )
   endif( FREETYPE_FOUND )
endif( BUILD_OSSIM_FREETYPE_SUPPORT )

# MPI - Optional:
set( OSSIM_HAS_MPI 0 )
if( BUILD_OSSIM_MPI_SUPPORT )
   find_package(MPI)
   if ( MPI_FOUND )
      include_directories( ${MPI_INCLUDE_DIR} )
      set( ossimDependentLibs ${ossimDependentLibs} ${MPI_LIBRARY} )
      set( OSSIM_HAS_MPI 1 )
   else ( MPI_FOUND )
      message( WARNING "Could not find optional MPI package!" )
   endif ( MPI_FOUND )
endif( BUILD_OSSIM_MPI_SUPPORT )

# ZLIB - Optional:
set( OSSIM_HAS_LIBZ 0 )
find_package( ZLIB )
if ( ZLIB_FOUND )
   include_directories( ${ZLIB_INCLUDE_DIR} )
   set( ossimDependentLibs ${ossimDependentLibs} ${ZLIB_LIBRARY} )
   set( OSSIM_HAS_LIBZ 1 )
else ( ZLIB_FOUND )
   message( WARNING "Could not find optional zlib package!" )
endif ( ZLIB_FOUND )

#---
# Call the OSSIM macros in OssimUtilities.cmake
#---
TODAYS_DATE(OSSIM_BUILD_DATE)
SET(OSSIM_SVN_REVISION_NUMBER "UNKNOWN")
GET_SVN_REVISION()
if ( NOT ${Project_WC_REVISION} EQUAL 0 )
   set( OSSIM_SVN_REVISION_NUMBER ${Project_WC_REVISION} )
endif()

#####################################################################################
# Call the configure files for ossimConfig and ossimVersion setup
#####################################################################################
set(OSSIM_VERSION_NUMBER "\"${OSSIM_VERSION}\"")
set(OSSIM_BUILD_DATE "\"${OSSIM_BUILD_DATE}\"")
set(OSSIM_REVISION_NUMBER "\"${OSSIM_SVN_REVISION_NUMBER}\"")

# Setting of OSSIM_ID to variable expanded $Id$ results.  Embedded troughout code.
set(OSSIM_ID_ENABLED   1)
IF(NOT BUILD_OSSIM_ID_SUPPORT)
   set(OSSIM_ID_ENABLED 0)
ENDIF(NOT BUILD_OSSIM_ID_SUPPORT)

SET(OSSIM_CONFIGURE_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/ossim/ossimConfig.h")
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/src/ossim/ossimConfig.h.in"
	       "${OSSIM_CONFIGURE_HEADER}")
SET(OSSIM_VERSION_HEADER_CONFIG "${CMAKE_CURRENT_SOURCE_DIR}/src/ossim/ossimVersion.h.in")
SET(OSSIM_VERSION_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/ossim/ossimVersion.h")
CONFIGURE_FILE("${OSSIM_VERSION_HEADER_CONFIG}"
               "${OSSIM_VERSION_HEADER}")


##################################################################################################
# Instead of creating another CMakeLists under src we will just add the subdirectories to build
# here
##################################################################################################
subdirs(src/ossim)

#---
# Building apps optional.
#---
if(BUILD_OSSIM_APPS)
  subdirs(src/apps)
endif()

#---
# Building curl dependant apps optional. default=OFF
#---
if(BUILD_OSSIM_CURL_APPS)
  subdirs(src/apps/curl_apps)
endif()

#---
# Building test apps optional.
#---
if(BUILD_OSSIM_TEST_APPS)
  subdirs(src/test)
endif()

set(TARGET_COMMON_LIBRARIES ossim ${OPENTHREADS_LIBRARY})

##################################################################################################
#
# Setup install destinations for the shared files:
# 1) Projection CSV files:
#
##################################################################################################
FILE(GLOB ossim_projection_codes_csv ${${PROJECT_NAME}_SOURCE_DIR}/share/ossim/projection/*.csv ${${PROJECT_NAME}_SOURCE_DIR}/share/ossim/projection/*.txt)
INSTALL(FILES ${ossim_projection_codes_csv} DESTINATION share/ossim/projection COMPONENT ossim)

# 2) Utility API JSON files
FILE(GLOB ossim_util_api_json ${${PROJECT_NAME}_SOURCE_DIR}/share/ossim/util/*.json)
INSTALL(FILES ${ossim_util_api_json} DESTINATION share/ossim/util COMPONENT ossim)

###################### OUTPUT GENERAL VARIABLE SETTINGS #######################

MESSAGE( STATUS "OSSIM_REVISION_NUMBER           = ${OSSIM_REVISION_NUMBER}" )
MESSAGE( STATUS "BUILD_LIBRARY_DIR               = ${BUILD_LIBRARY_DIR}" )
MESSAGE( STATUS "BUILD_RUNTIME_DIR               = ${BUILD_RUNTIME_DIR}" )
MESSAGE( STATUS "BUILD_OSSIM_FREETYPE_SUPPORT    = ${BUILD_OSSIM_FREETYPE_SUPPORT}" )
MESSAGE( STATUS "BUILD_OSSIM_APPS                = ${BUILD_OSSIM_APPS}" )
MESSAGE( STATUS "BUILD_OSSIM_CURL_APPS           = ${BUILD_OSSIM_CURL_APPS}" )
MESSAGE( STATUS "BUILD_OSSIM_TEST_APPS           = ${BUILD_OSSIM_TEST_APPS}" )
MESSAGE( STATUS "CMAKE_BUILD_TYPE                = ${CMAKE_BUILD_TYPE}" )
MESSAGE( STATUS "CMAKE_CXX_COMPILER              = ${CMAKE_CXX_COMPILER}" )
MESSAGE( STATUS "CMAKE_CXX_FLAGS                 = ${CMAKE_CXX_FLAGS}" )
MESSAGE( STATUS "CMAKE_CXX_FLAGS_DEBUG           = ${CMAKE_CXX_FLAGS_DEBUG}" )
MESSAGE( STATUS "CMAKE_CXX_FLAGS_RELEASE         = ${CMAKE_CXX_FLAGS_RELEASE}" )
MESSAGE( STATUS "CMAKE_INSTALL_PREFIX            = ${CMAKE_INSTALL_PREFIX}" )
MESSAGE( STATUS "FREETYPE_LIBRARY                = ${FREETYPE_LIBRARIES}" )
MESSAGE( STATUS "FREETYPE_INCLUDE                = ${FREETYPE_INCLUDE_DIRS}" )
MESSAGE( STATUS "GEOTIFF_LIBRARY                 = ${GEOTIFF_LIBRARIES}" )
MESSAGE( STATUS "GEOTIFF_INCLUDE                 = ${GEOTIFF_INCLUDE_DIR}" )
MESSAGE( STATUS "GEOTRANS_LIBRARY                = ${GEOTRANS_LIBRARY}" )
MESSAGE( STATUS "GEOTRANS_INCLUDE                = ${GEOTRANS_INCLUDE_DIR}" )
MESSAGE( STATUS "GEOS_LIBRARY                    = ${GEOS_LIBRARY}" )
MESSAGE( STATUS "GEOS_INCLUDE                    = ${GEOS_INCLUDE_DIR}" )
MESSAGE( STATUS "JPEG_LIBRARY                    = ${JPEG_LIBRARIES}" )
MESSAGE( STATUS "JPEG_INCLUDE                    = ${JPEG_INCLUDE_DIR}" )
MESSAGE( STATUS "MPI_LIBRARY                     = ${MPI_LIBRARIES}" )
MESSAGE( STATUS "MPI_INCLUDE                     = ${MPI_INCLUDE_PATH}" )
MESSAGE( STATUS "OPENTHREADS_LIBRARY             = ${OPENTHREADS_LIBRARY}" )
MESSAGE( STATUS "OPENTHREADS_INCLUDE             = ${OPENTHREADS_INCLUDE_DIR}" )
MESSAGE( STATUS "OSSIM_COMPILE_WITH_FULL_WARNING = ${OSSIM_COMPILE_WITH_FULL_WARNING}" )
MESSAGE( STATUS "OSSIM DEPENDENT LIBRARIES       = ${ossimDependentLibs}" )
MESSAGE( STATUS "TIFF_LIBRARY                    = ${TIFF_LIBRARIES}" )
MESSAGE( STATUS "TIFF_INCLUDE                    = ${TIFF_INCLUDE_DIR}" )
MESSAGE( STATUS "ZLIB_LIBRARY                    = ${ZLIB_LIBRARIES}" )
MESSAGE( STATUS "ZLIB_INCLUDE                    = ${ZLIB_INCLUDE_DIR}" )

#-----------------------------------------------------------------------------
# uninstall target
#-----------------------------------------------------------------------------
OSSIM_ADD_COMMON_MAKE_UNINSTALL()

message("################## end of OSSIM core library setup #########################")
