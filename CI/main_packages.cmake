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
set (ENV{LANG} "C") # Only ascii output
get_filename_component(OTB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)
get_filename_component(CI_PROJ_DIR ${OTB_SOURCE_DIR} DIRECTORY)
get_filename_component(CI_ROOT_DIR ${CI_PROJ_DIR} DIRECTORY)

set ( DEBUG "1" )

set ( CTEST_BUILD_CONFIGURATION "Release" )
set ( CTEST_CMAKE_GENERATOR "Unix Makefiles" )
set ( CTEST_BUILD_FLAGS "-j1" )
set ( CTEST_BUILD_NAME "Packages" )
set ( CTEST_SITE "${IMAGE_NAME}" )

# Directory variable
set ( CTEST_SOURCE_DIRECTORY "${OTB_SOURCE_DIR}/Packaging" )
set ( CTEST_BINARY_DIRECTORY "${OTB_SOURCE_DIR}/build_packages/" )
set ( CTEST_INSTALL_DIRECTORY "${OTB_SOURCE_DIR}/install_packages" )
set ( PROJECT_SOURCE_DIR "${CTEST_SOURCE_DIRECTORY}" )

# Copy back xdk for RUN_PATH reason
file ( COPY "${OTB_SOURCE_DIR}/install" DESTINATION "${CI_ROOT_DIR}/xdk")
# Packages case: 
# SUPERBUILD_BINARY_DIR this is needed for OTB_BINARY_DIR, not sure we need it
# SUPERBUILD_INSTALL_DIR do we need it? it seems so... We will set it to anything
# DOWNLOAD_LOCATION
# OTB_BINARY_DIR
# CMAKE_INSTALL_PREFIX
set ( CONFIGURE_OPTIONS  
  "-DCMAKE_PREFIX_PATH=${CTEST_INSTALL_DIRECTORY};
-DOTB_BINARY_DIR=${OTB_SOURCE_DIR}/build;
-DSUPERBUILD_INSTALL_DIR=${CI_ROOT_DIR}/xdk;
-DSUPERBUILD_BINARY_DIR=${OTB_SOURCE_DIR};
  " )

ctest_start (Experimental TRACK Experimental)

ctest_configure(BUILD "${CTEST_BINARY_DIRECTORY}"
    SOURCE "${CTEST_SOURCE_DIRECTORY}"
    OPTIONS "${CONFIGURE_OPTIONS}"
    RETURN_VALUE _configure_rv
    CAPTURE_CMAKE_ERROR _configure_error
    )

# if ( NOT _configure_rv EQUAL 0 )
#   ctest_submit()
#   message( SEND_ERROR "An error occurs during ctest_configure.")
# endif()

# ctest_build(BUILD "${CTEST_BINARY_DIRECTORY}"
#             RETURN_VALUE _build_rv
#             CAPTURE_CMAKE_ERROR _build_error
#             )

# if ( NOT _build_rv EQUAL 0 )
#   ctest_submit()
#   message( SEND_ERROR "An error occurs during ctest_build.")
# endif()

# Uncomment when ready for test
# ctest_test(PARALLEL_LEVEL 8
#            RETURN_VALUE _test_rv
#            CAPTURE_CMAKE_ERROR _test_error
#            )

# if ( NOT _test_rv EQUAL 0 )
#   ctest_submit()
#   message( SEND_ERROR "An error occurs during ctest_test.")
# endif()

# ctest_submit()
