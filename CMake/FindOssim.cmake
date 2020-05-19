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

# - Find Ossim
# Find the native Ossim includes and library
#
#   OSSIM_FOUND        - True if Ossim found.
#   OSSIM_INCLUDE_DIRS - where to find ossim/init/ossimInit.h, etc.
#   OSSIM_LIBRARIES    - List of libraries when using Ossim.
#

if( OSSIM_INCLUDE_DIR )
    # Already in cache, be silent
    set( Ossim_FIND_QUIETLY TRUE )
endif()

find_path( OSSIM_INCLUDE_DIR
           NAMES ossim/init/ossimInit.h )

# Version checking
set(OSSIM_VERSION)
if(EXISTS "${OSSIM_INCLUDE_DIR}/ossim/ossimVersion.h")
  file(READ "${OSSIM_INCLUDE_DIR}/ossim/ossimVersion.h" _ossim_version_h_CONTENTS)
  string(REGEX REPLACE ".*# *define OSSIM_VERSION *\"([0-9.]+)\".*" "\\1" OSSIM_VERSION "${_ossim_version_h_CONTENTS}")
  if("${OSSIM_VERSION}" VERSION_LESS "1.8.20")
    message(WARNING "The OSSIM include directory detected by OTB is: '${OSSIM_INCLUDE_DIR}'."
      "This version (${OSSIM_VERSION}) is not fully compatible with OTB."
      "Please consider updating your Ossim to version 1.8.20-3 or above")
  else()
    if("${OSSIM_VERSION}" VERSION_EQUAL "1.8.20")
      # special case to detect versions 1.8.20-1 and 1.8.20-2
      if(EXISTS "${OSSIM_INCLUDE_DIR}/ossim/projection/ossimRpcProjection.h")
        file(STRINGS "${OSSIM_INCLUDE_DIR}/ossim/projection/ossimRpcProjection.h" _ossim_rpc_proj_h_CONTENTS REGEX "^// \\$Id: ossimRpcProjection\\.h [0-9.]+ ")
        string(REGEX REPLACE ".*ossimRpcProjection\\.h ([0-9.]+) .*" "\\1" _ossim_svn_revision_hint "${_ossim_rpc_proj_h_CONTENTS}")
        if("${_ossim_svn_revision_hint}" LESS "23666")
          message(WARNING "The OSSIM include directory detected by OTB is: '${OSSIM_INCLUDE_DIR}'."
            "This version (${OSSIM_VERSION}, rev < r23666) is not fully compatible with OTB."
            "Please consider updating your Ossim to version 1.8.20-3 or above")
        endif()
      endif()
    endif()
  endif()
else()
  if(NOT Ossim_FIND_QUIETLY)
    message(WARNING "ossimVersion.h not found !")
  endif()
endif()

# Hack to detect version 2.2.0, without ossimVersion.h
if(EXISTS "${OSSIM_INCLUDE_DIR}/ossim/projection/ossimRpcSolver.h")
  file(STRINGS "${OSSIM_INCLUDE_DIR}/ossim/projection/ossimRpcSolver.h" _ossim_rpc_solv_content)
  if(_ossim_rpc_solv_content MATCHES "const +ossimRefPtr<ossimRpcModel> +getRpcModel\\(\\)")
    if((NOT OSSIM_VERSION) OR (OSSIM_VERSION VERSION_LESS "2.2.0"))
      set(OSSIM_VERSION "2.2.0")
    endif()
  endif()
endif()

if(OSSIM_VERSION)
  string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1" OSSIM_MAJOR_VERSION_NUMBER "${OSSIM_VERSION}")
  string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\2" OSSIM_MINOR_VERSION_NUMBER "${OSSIM_VERSION}")
  string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\3" OSSIM_PATCH_VERSION_NUMBER "${OSSIM_VERSION}")
  math(EXPR OSSIM_VERSION_NUMBER
    "((${OSSIM_MAJOR_VERSION_NUMBER})*100+${OSSIM_MINOR_VERSION_NUMBER})*100+${OSSIM_PATCH_VERSION_NUMBER}")
else()
  # Unknown version : default to 0
  set(OSSIM_VERSION_NUMBER 0)
endif()

# Look for the library
find_library(OSSIM_LIBRARY
             NAMES ossim)

# handle the QUIETLY and REQUIRED arguments and set OSSIM_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Ossim DEFAULT_MSG OSSIM_LIBRARY OSSIM_INCLUDE_DIR )

mark_as_advanced( OSSIM_INCLUDE_DIR OSSIM_LIBRARY )

set(OSSIM_LIBRARIES "${OSSIM_LIBRARY};${OPENTHREADS_LIBRARY}")
set(OSSIM_INCLUDE_DIRS ${OSSIM_INCLUDE_DIR})
