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

# - Find ICUUC
# Find the native ICUUC includes and library
#
#   ICUUC_FOUND        - True if ICUUC found.
#   ICUUC_INCLUDE_DIRS - where to find unicode/unistr.h, etc.
#   ICUUC_LIBRARIES    - List of libraries when using ICUUC.
#

if( ICUUC_INCLUDE_DIR )
    # Already in cache, be silent
    set( ICUUC_FIND_QUIETLY TRUE )
endif()

find_path( ICUUC_INCLUDE_DIR unicode/unistr.h )

find_library( ICUUC_LIBRARY
              NAMES icuuc )

# handle the QUIETLY and REQUIRED arguments and set ICUUC_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( ICUUC DEFAULT_MSG ICUUC_LIBRARY ICUUC_INCLUDE_DIR )

mark_as_advanced( ICUUC_INCLUDE_DIR ICUUC_LIBRARY )

if(ICUUC_FOUND)
  set(ICUUC_INCLUDE_DIRS ${ICUUC_INCLUDE_DIR})
  set(ICUUC_LIBRARIES ${ICUUC_LIBRARY})
else()
  set(ICUUC_INCLUDE_DIRS)
  set(ICUUC_LIBRARIES)
endif()

