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

# Find the AntiGrain Geometry graphical toolkit header files
#
#   AGG_FOUND        - True if AGG found.
#   AGG_INCLUDE_DIRS - where to find agg_pixfmt_rgba.h, etc.
#

if( AGG_INCLUDE_DIR )
    # Already in cache, be silent
    set( AGG_FIND_QUIETLY TRUE )
endif()

find_path( AGG_INCLUDE_DIR agg_pixfmt_rgba.h
  PATH_SUFFIXES agg2
  )

# handle the QUIETLY and REQUIRED arguments and set AGG_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( Agg DEFAULT_MSG AGG_INCLUDE_DIR )

mark_as_advanced( AGG_INCLUDE_DIR )

if(AGG_FOUND)
  set(AGG_INCLUDE_DIRS ${AGG_INCLUDE_DIR})
else()
  set(AGG_INCLUDE_DIRS)
endif()

