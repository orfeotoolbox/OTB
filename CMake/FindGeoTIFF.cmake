#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

# - Find GEOTIFF library
# Find the native GEOTIFF includes and library
#
#   GEOTIFF_FOUND        - True if GEOTIFF found.
#   GEOTIFF_INCLUDE_DIRS - where to find geotiff.h
#   GEOTIFF_LIBRARIES    - List of libraries to link when using GEOTIFF.
#   GEOTIFF_VERSION        - version string
#   GEOTIFF_VERSION_MAJOR  - version major number
#   GEOTIFF_VERSION_MINOR  - version minor number
#   GEOTIFF_VERSION_PATCH  - version patch number
#   GEOTIFF_VERSION_NUMBER - version number ((major*100)+minor)*100+patch

#

find_path( GEOTIFF_INCLUDE_DIR geotiff.h
           PATH_SUFFIXES geotiff libgeotiff )

if(EXISTS "${GEOTIFF_INCLUDE_DIR}/geotiff.h")
  file(READ "${GEOTIFF_INCLUDE_DIR}/geotiff.h" _geotiff_h_CONTENTS)
  string(REGEX REPLACE ".*# *define +LIBGEOTIFF_VERSION +([0-9])([0-9])([0-9])([0-9]).*" "\\1" GEOTIFF_VERSION_MAJOR "${_geotiff_h_CONTENTS}")
  string(REGEX REPLACE ".*# *define +LIBGEOTIFF_VERSION +([0-9])([0-9])([0-9])([0-9]).*" "\\2" GEOTIFF_VERSION_MINOR "${_geotiff_h_CONTENTS}")
  string(REGEX REPLACE ".*# *define +LIBGEOTIFF_VERSION +([0-9])([0-9])([0-9])([0-9]).*" "\\3" GEOTIFF_VERSION_PATCH "${_geotiff_h_CONTENTS}")
  set(GEOTIFF_VERSION "${GEOTIFF_VERSION_MAJOR}.${GEOTIFF_VERSION_MINOR}.${GEOTIFF_VERSION_PATCH}")
  math(EXPR GEOTIFF_VERSION_NUMBER
    "((${GEOTIFF_VERSION_MAJOR})*100+${GEOTIFF_VERSION_MINOR})*100+${GEOTIFF_VERSION_PATCH}")
else()
  if(NOT GEOTIFF_FIND_QUIETLY)
  message(WARNING "geotiff.h not found !")
  endif()
endif()


set( GEOTIFF_NAMES ${GEOTIFF_NAMES} geotiff_i geotiff libgeotiff_i libgeotiff )
find_library( GEOTIFF_LIBRARY NAMES ${GEOTIFF_NAMES} )

mark_as_advanced( GEOTIFF_INCLUDE_DIR GEOTIFF_LIBRARY )

# handle the QUIETLY and REQUIRED arguments and set GEOTIFF_FOUND to TRUE if 
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args( GeoTIFF DEFAULT_MSG
  GEOTIFF_LIBRARY GEOTIFF_INCLUDE_DIR)

if(GEOTIFF_FOUND)
  set( GEOTIFF_INCLUDE_DIRS ${GEOTIFF_INCLUDE_DIR} )
  set( GEOTIFF_LIBRARIES ${GEOTIFF_LIBRARY} )
endif()
