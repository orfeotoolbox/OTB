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

# - Find TinyXML
# Find the native TinyXML includes and library
#
#   TINYXML_FOUND        - True if TinyXML found.
#   TINYXML_INCLUDE_DIRS - where to find tinyxml.h, etc.
#   TINYXML_LIBRARIES    - List of libraries when using TinyXML.
#   TINYXML_VERSION        - version string
#   TINYXML_VERSION_MAJOR  - version major number
#   TINYXML_VERSION_MINOR  - version minor number
#   TINYXML_VERSION_PATCH  - version patch number
#   TINYXML_VERSION_NUMBER - version number ((major*100)+minor)*100+patch
#

if( TINYXML_INCLUDE_DIR )
    # Already in cache, be silent
    set( TinyXML_FIND_QUIETLY TRUE )
endif()

find_path( TINYXML_INCLUDE_DIR tinyxml.h
           PATH_SUFFIXES tinyxml )

if(EXISTS "${TINYXML_INCLUDE_DIR}/tinyxml.h")
  file(READ "${TINYXML_INCLUDE_DIR}/tinyxml.h" _tinyxml_h_CONTENTS)
  string(REGEX REPLACE ".*const +int +TIXML_MAJOR_VERSION += +([0-9]+);.*" "\\1" TINYXML_VERSION_MAJOR "${_tinyxml_h_CONTENTS}")
  string(REGEX REPLACE ".*const +int +TIXML_MINOR_VERSION += +([0-9]+);.*" "\\1" TINYXML_VERSION_MINOR "${_tinyxml_h_CONTENTS}")
  string(REGEX REPLACE ".*const +int +TIXML_PATCH_VERSION += +([0-9]+);.*" "\\1" TINYXML_VERSION_PATCH "${_tinyxml_h_CONTENTS}")
  set(TINYXML_VERSION "${TINYXML_VERSION_MAJOR}.${TINYXML_VERSION_MINOR}.${TINYXML_VERSION_PATCH}")
  math(EXPR TINYXML_VERSION_NUMBER
    "((${TINYXML_VERSION_MAJOR})*100+${TINYXML_VERSION_MINOR})*100+${TINYXML_VERSION_PATCH}")
else()
  if(NOT TINYXML_FIND_QUIETLY)
  message(WARNING "tinyxml.h not found !")
  endif()
endif()


find_library( TINYXML_LIBRARY
              NAMES tinyxml
              PATH_SUFFIXES tinyxml )

# handle the QUIETLY and REQUIRED arguments and set TINYXML_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( TinyXML DEFAULT_MSG TINYXML_LIBRARY TINYXML_INCLUDE_DIR )

mark_as_advanced( TINYXML_INCLUDE_DIR TINYXML_LIBRARY )

if(TINYXML_FOUND)

  set(TINYXML_INCLUDE_DIRS ${TINYXML_INCLUDE_DIR})
  set(TINYXML_LIBRARIES    ${TINYXML_LIBRARY})

else()

  set(TINYXML_INCLUDE_DIRS)
  set(TINYXML_LIBRARIES)

endif()
