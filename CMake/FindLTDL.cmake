#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

# - Find LTDL
# Find the native LTDL includes and library
#
#   LTDL_FOUND        - True if LTDL found.
#   LTDL_INCLUDE_DIRS - where to find ltdl.h, etc.
#   LTDL_LIBRARIES    - List of libraries when using LTDL.
#

if( LTDL_INCLUDE_DIR )
    # Already in cache, be silent
    set( LTDL_FIND_QUIETLY TRUE )
endif()

find_path( LTDL_INCLUDE_DIR ltdl.h )

find_library( LTDL_LIBRARY
              NAMES ltdl )

# handle the QUIETLY and REQUIRED arguments and set LTDL_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( LTDL DEFAULT_MSG LTDL_LIBRARY LTDL_INCLUDE_DIR )

mark_as_advanced( LTDL_INCLUDE_DIR LTDL_LIBRARY )

if(LTDL_FOUND)
  set(LTDL_INCLUDE_DIRS ${LTDL_INCLUDE_DIR})
  set(LTDL_LIBRARIES ${LTDL_LIBRARY})
else()
  set(LTDL_INCLUDE_DIRS)
  set(LTDL_LIBRARIES)
endif()

