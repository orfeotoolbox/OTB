#
# Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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
get_filename_component(OTB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)
set (ENV{LANG} "C") # Only ascii output

# Build Configuration : Release, Debug..
if(ci_build_type)
  set (CTEST_BUILD_CONFIGURATION ${ci_build_type})
else()
  set (CTEST_BUILD_CONFIGURATION "Release")
endif()

set (CTEST_CMAKE_GENERATOR "Ninja")

# detect short sha
if(NOT DEFINED ENV{CI_COMMIT_SHORT_SHA})
  execute_process(COMMAND git log -1 --pretty=format:%h
                  WORKING_DIRECTORY ${OTB_SOURCE_DIR}
                  OUTPUT_VARIABLE ci_short_sha)
else()
  set(ci_short_sha "$ENV{CI_COMMIT_SHORT_SHA}")
endif()

# Find the build name and CI profile. Customize it with FFTW
set_dash_build_name()
set (CTEST_BUILD_NAME "FFTW Tests ${ci_ref_name} P-${ci_pipeline_id}")

# Detect site
if(NOT DEFINED IMAGE_NAME)
  if(DEFINED ENV{IMAGE_NAME})
    set(IMAGE_NAME $ENV{IMAGE_NAME})
  endif()
endif()
set (CTEST_SITE "${IMAGE_NAME}")

# Directory variable
set (CTEST_SOURCE_DIRECTORY "${OTB_SOURCE_DIR}")
if(BUILD_DIR)
  set (CTEST_BINARY_DIRECTORY "${BUILD_DIR}")
else()
  set (CTEST_BINARY_DIRECTORY "${OTB_SOURCE_DIR}/build/")
endif()
set (CTEST_INSTALL_DIRECTORY "${OTB_SOURCE_DIR}/install/")
set (PROJECT_SOURCE_DIR "${OTB_SOURCE_DIR}")

# Ctest command value
set (CMAKE_COMMAND "cmake")

message(STATUS "CI profile : ${ci_profile}")

# Sources are already checked out : do nothing for update
set(CTEST_GIT_UPDATE_CUSTOM "${CMAKE_COMMAND}" "-E" "echo" "No update")
# Look for a GIT command-line client.
find_program(CTEST_GIT_COMMAND NAMES git git.cmd)



ctest_start (Experimental TRACK CI_Build)

ctest_update()

# Run only onverlapSave tests
ctest_test(PARALLEL_LEVEL 8
    RETURN_VALUE _test_rv
    CAPTURE_CMAKE_ERROR _test_error
    INCLUDE "OverlapSave"
    )
  # Test log
file ( WRITE 
"${OTB_SOURCE_DIR}/log/test_return_value_log.txt" "${_test_rv}")
file ( WRITE 
"${OTB_SOURCE_DIR}/log/test_cmake_error_log.txt" "${_test_error}")

if ( NOT _test_rv EQUAL 0 )
  message( SEND_ERROR "An error occurs during ctest_test.")
endif()

ctest_submit()
