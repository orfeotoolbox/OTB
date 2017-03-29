#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

# - Find Mapnik
# Find the native Mapnik includes and library
#
#   MAPNIK_FOUND        - True if MAPNIK found.
#   MAPNIK_INCLUDE_DIRS - where to find mapnik/map.hpp, etc.
#   MAPNIK_LIBRARIES    - List of libraries when using MAPNIK
#

find_path( MAPNIK_INCLUDE_DIR mapnik/map.hpp )

find_library( MAPNIK_LIBRARY
              NAMES mapnik2 mapnik )

# handle the QUIETLY and REQUIRED arguments and set MAPNIK_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( Mapnik DEFAULT_MSG MAPNIK_LIBRARY MAPNIK_INCLUDE_DIR )

mark_as_advanced( MAPNIK_INCLUDE_DIR MAPNIK_LIBRARY )

if(MAPNIK_FOUND)
  set(MAPNIK_INCLUDE_DIRS ${MAPNIK_INCLUDE_DIR})
  set(MAPNIK_LIBRARIES ${MAPNIK_LIBRARY})
else()
  set(MAPNIK_INCLUDE_DIRS)
  set(MAPNIK_LIBRARIES)
endif()

