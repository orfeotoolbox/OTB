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

include( "${CMAKE_CURRENT_LIST_DIR}/macros.cmake" )

# This script is a prototype for the future CI, it may evolve rapidly in a near future
get_filename_component(OTB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)
set (ENV{LANG} "C") # Only ascii output

# Build Configuration : Release, Debug..
set (CTEST_BUILD_CONFIGURATION "Release")
set (CTEST_CMAKE_GENERATOR "Ninja")

# detect short sha
if(NOT DEFINED ENV{CI_COMMIT_SHORT_SHA})
  execute_process(COMMAND git log -1 --pretty=format:%h
                  WORKING_DIRECTORY ${OTB_SOURCE_DIR}
                  OUTPUT_VARIABLE ci_short_sha)
else()
  set(ci_short_sha "$ENV{CI_COMMIT_SHORT_SHA}")
endif()

# Find the build name and CI profile
set_dash_build_name()

# set pipelines to enable documentation
set(ci_cookbook_profiles mr develop release)
set(ci_doxygen_profiles mr develop release)
list(FIND ci_cookbook_profiles ${ci_profile} ci_do_cookbook)
list(FIND ci_doxygen_profiles ${ci_profile} ci_do_doxygen)

# Detect site
if(NOT DEFINED IMAGE_NAME)
  if(DEFINED ENV{IMAGE_NAME})
    set(IMAGE_NAME $ENV{IMAGE_NAME})
  endif()
endif()
set (CTEST_SITE "${IMAGE_NAME}")

# Detect "skip testing"
if(DEFINED ENV{CI_SKIP_TESTING})
  set(ci_skip_testing 1)
endif()

# Directory variable
set (CTEST_SOURCE_DIRECTORY "${OTB_SOURCE_DIR}")
if(BUILD_DIR)
  set (CTEST_BINARY_DIRECTORY "${BUILD_DIR}")
else()
  set (CTEST_BINARY_DIRECTORY "${OTB_SOURCE_DIR}/build/")
endif()
if(INSTALL_DIR)
  set (CTEST_INSTALL_DIRECTORY "${INSTALL_DIR}")
else()
  set (CTEST_INSTALL_DIRECTORY "${OTB_SOURCE_DIR}/install/")
endif()
set (PROJECT_SOURCE_DIR "${OTB_SOURCE_DIR}")

# Ctest command value
set (CMAKE_COMMAND "cmake")

message(STATUS "CI profile : ${ci_profile}")

#The following file set the CONFIGURE_OPTIONS variable
set (ENABLE_DOXYGEN OFF)
set (CONFIGURE_OPTIONS  "")
include ( "${CMAKE_CURRENT_LIST_DIR}/configure_options.cmake" )

# Sources are already checked out : do nothing for update
set(CTEST_GIT_UPDATE_CUSTOM "${CMAKE_COMMAND}" "-E" "echo" "No update")

# Look for a GIT command-line client.
find_program(CTEST_GIT_COMMAND NAMES git git.cmd)

# End of configuration


ctest_start (Experimental TRACK CI_Build)

ctest_update()

# --------------------------- Configure ----------------------------------------
ctest_configure(BUILD "${CTEST_BINARY_DIRECTORY}"
    SOURCE "${OTB_SOURCE_DIR}"
    OPTIONS "${CONFIGURE_OPTIONS}"
    RETURN_VALUE _configure_rv
    CAPTURE_CMAKE_ERROR _configure_error
    )
# Configure log
file ( WRITE 
  "${OTB_SOURCE_DIR}/log/configure_return_value_log.txt" "${_configure_rv}")
file ( WRITE 
  "${OTB_SOURCE_DIR}/log/configure_cmake_error_log.txt" "${_configure_error}")

if ( NOT _configure_rv EQUAL 0 )
  # stop processing here
  ctest_submit()
  message( FATAL_ERROR "An error occurs during ctest_configure.")
endif()

# ------------------------------ Build -----------------------------------------
if(ci_skip_install)
  message(STATUS "Skip install")
  set(CTEST_BUILD_TARGET)
else()
  set(CTEST_BUILD_TARGET install)
endif()

ctest_build(BUILD "${CTEST_BINARY_DIRECTORY}"
            RETURN_VALUE _build_rv
            CAPTURE_CMAKE_ERROR _build_error
            )

# Build log
file ( WRITE 
  "${OTB_SOURCE_DIR}/log/build_return_value_log.txt" "${_build_rv}")
file ( WRITE 
  "${OTB_SOURCE_DIR}/log/build_cmake_error_log.txt" "${_build_error}")

if ( NOT _build_rv EQUAL 0 )
  message( SEND_ERROR "An error occurs during ctest_build.")
endif()

# ----------------------------- Test -------------------------------------------
if(ci_skip_testing)
  message(STATUS "Skip testing")
  set(_test_rv 0)
else()
  ctest_test(PARALLEL_LEVEL 8
             RETURN_VALUE _test_rv
             CAPTURE_CMAKE_ERROR _test_error
             )
  # Test log
  file ( WRITE 
    "${OTB_SOURCE_DIR}/log/test_return_value_log.txt" "${_test_rv}")
  file ( WRITE 
    "${OTB_SOURCE_DIR}/log/test_cmake_error_log.txt" "${_test_error}")
endif()

if ( NOT _test_rv EQUAL 0 )
  message( SEND_ERROR "An error occurs during ctest_test.")
endif()

# ----------------------------- Submit -----------------------------------------
ctest_submit()

# ---------------------------- Doxygen -----------------------------------------
if(ENABLE_DOXYGEN)
  ctest_build(BUILD "${CTEST_BINARY_DIRECTORY}"
              TARGET Documentation
              RETURN_VALUE _doxy_rv
              CAPTURE_CMAKE_ERROR _doxy_error
              )
endif()
