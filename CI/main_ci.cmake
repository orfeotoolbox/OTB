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

# This script is a prototype for the future CI, it may evolve rapidly in a near future
get_filename_component(OTB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)
set (ENV{LANG} "C") # Only ascii output

# Build Configuration : Release, Debug..
set (CTEST_BUILD_CONFIGURATION "Release")
set (CTEST_CMAKE_GENERATOR "Ninja")

set (CTEST_BUILD_NAME "$ENV{CI_COMMIT_SHORT_SHA}")
if("$ENV{CI_MERGE_REQUEST_SOURCE_BRANCH_NAME}" AND "$ENV{CI_MERGE_REQUEST_TARGET_BRANCH_NAME}")
  set (CTEST_BUILD_NAME "${CTEST_BUILD_NAME} ($ENV{CI_MERGE_REQUEST_SOURCE_BRANCH_NAME} to $ENV{CI_MERGE_REQUEST_TARGET_BRANCH_NAME})")
elseif("$ENV{CI_COMMIT_REF_NAME}")
  set (CTEST_BUILD_NAME "${CTEST_BUILD_NAME} ($ENV{CI_COMMIT_REF_NAME})")
endif()
set (CTEST_SITE "${IMAGE_NAME}")

# Directory variable
set (CTEST_SOURCE_DIRECTORY "${OTB_SOURCE_DIR}")
set (CTEST_BINARY_DIRECTORY "${OTB_SOURCE_DIR}/build/")
set (CTEST_INSTALL_DIRECTORY "${OTB_SOURCE_DIR}/install/")
set (PROJECT_SOURCE_DIR "${OTB_SOURCE_DIR}")

# Ctest command value
set (CMAKE_COMMAND "cmake")

# Data directory setting
set (OTB_DATA_ROOT "${OTB_SOURCE_DIR}/otb-data/") # todo
set (OTB_LARGEINPUT_ROOT "") # todo

message(STATUS "CI profile : $ENV{OTB_CI_PROFILE}")
message(STATUS "CI_COMMIT_REF_NAME : $ENV{CI_COMMIT_REF_NAME}")
message(STATUS "CTEST_BUILD_NAME : ${CTEST_BUILD_NAME}")

#The following file set the CONFIGURE_OPTIONS variable
set (CONFIGURE_OPTIONS  "")
include ( "${CMAKE_CURRENT_LIST_DIR}/configure_option.cmake" )

# End of configuration


ctest_start (Experimental TRACK Experimental)

ctest_configure(BUILD "${CTEST_BINARY_DIRECTORY}"
    SOURCE "${OTB_SOURCE_DIR}"
    OPTIONS "${CONFIGURE_OPTIONS}"
    RETURN_VALUE _configure_rv
    CAPTURE_CMAKE_ERROR _configure_error
    )

if ( NOT _configure_rv EQUAL 0 )
  ctest_submit()
  message( FATAL_ERROR "An error occurs during ctest_configure.")
endif()

ctest_build(BUILD "${CTEST_BINARY_DIRECTORY}"
            RETURN_VALUE _build_rv
            CAPTURE_CMAKE_ERROR _build_error
            )

if ( NOT _build_rv EQUAL 0 )
  ctest_submit()
  message( SEND_ERROR "An error occurs during ctest_build.")
endif()

# ctest_test(PARALLEL_LEVEL 8)

ctest_submit()
