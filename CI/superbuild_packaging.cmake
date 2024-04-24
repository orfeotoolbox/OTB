#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# We are included from main_superbuild.cmake
# This script is a prototype for the future CI, it may evolve rapidly in a near future

include( "${CMAKE_CURRENT_LIST_DIR}/macros.cmake" )

set (ENV{LANG} "C") # Only ascii output

get_filename_component( OTB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR} DIRECTORY )

if(WIN32)
  file(TO_NATIVE_PATH "${XDK_INSTALL_PATH}" XDK_INSTALL_PATH_NATIVE)
  file(TO_NATIVE_PATH "${CTEST_BINARY_DIRECTORY}/bin" OTB_BUILD_BIN_DIR_NATIVE)
  set(ENV{PATH} "$ENV{PATH};${OTB_BUILD_BIN_DIR_NATIVE}" )
  set(ENV{PATH} "${XDK_INSTALL_PATH_NATIVE}\\bin;$ENV{PATH}" )
  set(ENV{PATH} "$ENV{PATH};${XDK_INSTALL_PATH_NATIVE}\\lib" )
  set(ENV{GDAL_DATA} "${XDK_INSTALL_PATH_NATIVE}\\data" )
  set(ENV{PROJ_LIB} "${XDK_INSTALL_PATH_NATIVE}\\share\\proj" )
  set( CTEST_ENVIRONMENT
"PATH=$ENV{PATH}
GDAL_DATA=$ENV{GDAL_DATA}
PROJ_LIB=$ENV{PROJ_LIB}
")
else()
  set(ENV{PATH} "${XDK_INSTALL_PATH}/lib:${XDK_INSTALL_PATH}/bin:$ENV{PATH}" )
  set( GDAL_DATA "${XDK_INSTALL_PATH}/share/gdal" )
  set( PROJ_LIB "${XDK_INSTALL_PATH}/share" )
  set( CTEST_ENVIRONMENT
"PATH=$ENV{PATH}
")
endif()

set ( CTEST_BUILD_CONFIGURATION "Release" )
set ( CTEST_CMAKE_GENERATOR "Ninja" )
set ( CTEST_BUILD_FLAGS "-j1")
set ( CTEST_BUILD_NAME "Packages" )
# Detect site
if(NOT DEFINED IMAGE_NAME)
  if(DEFINED ENV{IMAGE_NAME})
    set(IMAGE_NAME $ENV{IMAGE_NAME})
  endif()
endif()
set ( CTEST_SITE "${IMAGE_NAME}" )

# Find the build name and CI profile
set_dash_build_name()

# Directory variable
set ( CTEST_SOURCE_DIRECTORY "${OTB_SOURCE_DIR}" )
set ( CTEST_BINARY_DIRECTORY "${OTB_SOURCE_DIR}/build" )
set ( CTEST_INSTALL_DIRECTORY "${OTB_SOURCE_DIR}/install_packages" )
set ( PROJECT_SOURCE_DIR "${CTEST_SOURCE_DIRECTORY}" )

# Copy back xdk for RUN_PATH reason
# We might want to change the name of artifact in main_superbuild
# file ( COPY "${OTB_SOURCE_DIR}/install/xdk" DESTINATION "${CI_ROOT_DIR}")
# Packages case:
# SUPERBUILD_BINARY_DIR this is needed for OTB_BINARY_DIR, not sure we need it
# SUPERBUILD_INSTALL_DIR do we need it? it seems so... We will set it to anything
# DOWNLOAD_LOCATION
# OTB_BINARY_DIR
# CMAKE_INSTALL_PREFIX
set ( CONFIGURE_OPTIONS
"-DCMAKE_INSTALL_PREFIX=${CTEST_INSTALL_DIRECTORY};\
-DOTB_BINARY_DIR=${OTB_SOURCE_DIR}/build;\
-DXDK_INSTALL_PATH=${OTB_SOURCE_DIR}/xdk;\
-DNAME_SUFFIX=${NAME_SUFFIX};\
-DCPACK_ARCHIVE_COMPONENT_INSTALL=${PACKAGE_BY_MODULE}")

# Look for a GIT command-line client.
find_program(CTEST_GIT_COMMAND NAMES git git.cmd)

# Sources are already checked out : do nothing for update
set(CTEST_GIT_UPDATE_CUSTOM "${CMAKE_COMMAND}" "-E" "echo" "No update")


ctest_start( Experimental TRACK CI_Package )

ctest_update( SOURCE "${OTB_SOURCE_DIR}" )


ctest_configure(
  BUILD "${CTEST_BINARY_DIRECTORY}"
  SOURCE "${CTEST_SOURCE_DIRECTORY}"
  OPTIONS "${CONFIGURE_OPTIONS}"
  RETURN_VALUE _configure_rv
  CAPTURE_CMAKE_ERROR _configure_error
  )

if( NOT _configure_rv EQUAL 0 )
  ctest_submit()
  message( SEND_ERROR "An error occurs during ctest_configure.")
  return()
endif()


ctest_build(
  BUILD "${CTEST_BINARY_DIRECTORY}"
  TARGET package
  RETURN_VALUE _build_rv
  CAPTURE_CMAKE_ERROR _build_error
  )

if( NOT _build_rv EQUAL 0 )
  message( SEND_ERROR "An error occurs during ctest_build.")
endif()

ctest_submit()
