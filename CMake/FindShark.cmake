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

# - Find Shark
# Find the Shark includes and library
#
#   SHARK_FOUND       - True if Shark found.
#   SHARK_INCLUDE_DIR - where to find Core/shark.h
#   SHARK_LIBRARIES   - List of libraries required for shark.
#

set(SHARK_SEARCH_PATH)

set(SHARK_CONFIG_FILE)

if(Shark_DIR)
  get_filename_component(SHARK_SEARCH_PATH "${Shark_DIR}" PATH)
  get_filename_component(SHARK_SEARCH_PATH "${SHARK_SEARCH_PATH}" PATH)
  get_filename_component(SHARK_SEARCH_PATH "${SHARK_SEARCH_PATH}" PATH)

  if(EXISTS "${Shark_DIR}/SharkConfig.cmake")
    set(SHARK_CONFIG_FILE ${Shark_DIR}/SharkConfig.cmake)
  endif()

endif() #if(Shark_DIR)

find_path( SHARK_INCLUDE_DIR
  NAMES shark/Core/Shark.h
  PATHS "${SHARK_SEARCH_PATH}"
  PATH_SUFFIXES include include/shark shark
  )

find_library( SHARK_LIBRARY
  NAMES shark shark_debug
  PATHS "${SHARK_SEARCH_PATH}"
  PATH_SUFFIXES lib
  )

mark_as_advanced( SHARK_INCLUDE_DIR  SHARK_LIBRARY )

find_package( Boost 1.48.0 REQUIRED QUIET
  COMPONENTS serialization
  )
if(NOT Boost_FOUND)
  message(FATAL_ERROR "Please make sure Boost 1.48.0 is installed on your system")
endif()

if(NOT SHARK_LIBRARY)
  message(FATAL_ERROR "Cannot find SHARK_LIBRARY. set it with cmake -DSHARK_LIBRARY=")
  return()
endif()

if(NOT SHARK_INCLUDE_DIR)
  message(FATAL_ERROR "Cannot find SHARK_INCLUDE_DIR. Set it with cmake -DSHARK_INCLUDE_DIR=")
  return()
endif()

get_filename_component(SHARK_INSTALLDIR ${SHARK_LIBRARY} PATH)
get_filename_component(SHARK_INSTALLDIR ${SHARK_INSTALLDIR} PATH)

if(NOT SHARK_CONFIG_FILE)
  find_file(SHARK_CONFIG_FILE SharkConfig.cmake
    PATHS ${SHARK_INSTALLDIR}
    PATH_SUFFIXES lib/cmake/Shark share/Shark cmake/Shark)
endif()

if(SHARK_CONFIG_FILE)
  file(STRINGS "${SHARK_CONFIG_FILE}" SHARK_CONFIG_FILE_CONTENTS)
  string(REGEX REPLACE
      ".*set.*\\(SHARK_VERSION_MAJOR..([0-9]+)\"\\).*" "\\1"
       SHARK_VERSION_MAJOR "${SHARK_CONFIG_FILE_CONTENTS}")
  string(REGEX REPLACE
      ".*set.*\\(SHARK_VERSION_MINOR..([0-9]+)\"\\).*" "\\1"
       SHARK_VERSION_MINOR "${SHARK_CONFIG_FILE_CONTENTS}")
  string(REGEX REPLACE
      ".*set.*\\(SHARK_VERSION_PATCH..([0-9]+)\"\\).*" "\\1"
       SHARK_VERSION_PATCH "${SHARK_CONFIG_FILE_CONTENTS}")       

  set(SHARK_VERSION_STRING 
  "${SHARK_VERSION_MAJOR}.${SHARK_VERSION_MINOR}.${SHARK_VERSION_PATCH}")
endif()

set(SHARK_USE_OPENMP_matched)
#define SHARK_USE_OPENMP
file(STRINGS "${SHARK_INCLUDE_DIR}/shark/Core/Shark.h" SHARK_H_CONTENTS)
string(REGEX MATCH
  "#define.SHARK_USE_OPENMP"
  SHARK_USE_OPENMP_matched "${SHARK_H_CONTENTS}")

#this variable is added in cache but not used now
# you can use it to see if shark installation has openMP
# later if needed in other places..
set(SHARK_USE_OPENMP FALSE CACHE BOOL "shark is built with OpenMP")
if(SHARK_USE_OPENMP_matched)
  set(SHARK_USE_OPENMP TRUE CACHE BOOL "shark is built with OpenMP" FORCE)
  message(STATUS "Shark is built with OpenMP: SHARK_USE_OPENMP = TRUE")
else()
  message(STATUS "Shark is built without OpenMP: SHARK_USE_OPENMP = FALSE")
endif()

INCLUDE(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Shark
  REQUIRED_VARS SHARK_LIBRARY SHARK_INCLUDE_DIR
  VERSION_VAR SHARK_VERSION_STRING)

if(SHARK_FOUND)
  set(SHARK_INCLUDE_DIRS ${SHARK_INCLUDE_DIR} ${Boost_INCLUDE_DIR} )
  set(SHARK_LIBRARIES ${SHARK_LIBRARY} ${Boost_LIBRARIES} )
endif()
