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

# reset required flags
set(SHARK_REQUIRED_C_FLAGS "")
set(SHARK_REQUIRED_CXX_FLAGS "")
set(SHARK_REQUIRED_LINK_FLAGS "")

# Check if Shark was built with OpenMP, CBLAS, DYNLIB, ...
file(STRINGS "${SHARK_INCLUDE_DIR}/shark/Core/Shark.h" SHARK_H_CONTENTS)

if(SHARK_H_CONTENTS MATCHES "#define.SHARK_USE_OPENMP")
  set(SHARK_USE_OPENMP 1)
  find_package(OpenMP QUIET)
  if(OPENMP_FOUND)
    set(SHARK_REQUIRED_C_FLAGS "${SHARK_REQUIRED_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(SHARK_REQUIRED_CXX_FLAGS "${SHARK_REQUIRED_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    if(OpenMP_EXE_LINKER_FLAGS)
      set(SHARK_REQUIRED_LINK_FLAGS "${SHARK_REQUIRED_LINK_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    endif()
  endif()
else()
  set(SHARK_USE_OPENMP 0)
endif()

if(SHARK_H_CONTENTS MATCHES "#define.SHARK_USE_CBLAS")
  set(SHARK_USE_CBLAS 1)
else()
  set(SHARK_USE_CBLAS 0)
endif()

if(SHARK_H_CONTENTS MATCHES "#define.SHARK_USE_DYNLIB")
  set(SHARK_USE_DYNLIB 1)
else()
  set(SHARK_USE_DYNLIB 0)
endif()

if(SHARK_USE_CBLAS AND SHARK_USE_DYNLIB)
  set(REQUIRED_CBLAS_LIB CBLAS_LIBRARY)
  find_library(CBLAS_LIBRARY NAMES cblas blas PATH_SUFFIXES blas)
else()
  set(REQUIRED_CBLAS_LIB)
endif()

INCLUDE(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
find_package_handle_standard_args(Shark
  REQUIRED_VARS SHARK_LIBRARY SHARK_INCLUDE_DIR ${REQUIRED_CBLAS_LIB}
  VERSION_VAR SHARK_VERSION_STRING)

if(SHARK_FOUND)
  set(SHARK_INCLUDE_DIRS ${SHARK_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
  if(Boost_MINOR_VERSION STREQUAL "72")
    get_target_property(Boost_current_LIB ${Boost_SERIALIZATION_LIBRARY} IMPORTED_LOCATION_RELEASE)
    set(SHARK_BOOST_LIBRARY "${Boost_current_LIB}")
  else()
    set(SHARK_BOOST_LIBRARY ${Boost_SERIALIZATION_LIBRARY})
  endif()
  set(SHARK_LIBRARIES ${SHARK_LIBRARY} ${SHARK_BOOST_LIBRARY})
  if(REQUIRED_CBLAS_LIB)
    set(SHARK_LIBRARIES ${SHARK_LIBRARIES} ${CBLAS_LIBRARY})
  endif()
endif()
