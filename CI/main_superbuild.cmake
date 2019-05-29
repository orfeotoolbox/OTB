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

include( "${CMAKE_CURRENT_LIST_DIR}/macros.cmake" )

set (ENV{LANG} "C") # Only ascii output
get_filename_component(OTB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)

set ( DEBUG "1" )
###########################################################################
###########################################################################
# Download xkd
###########################################################################
###########################################################################

# How to get md5sum:
# * concatenate all source files in one
# * add configure result from previous job ${OTB_SOURCE_DIR}/build/CMakeCache.txt
###########################

file(READ "${OTB_SOURCE_DIR}/sb_branch.txt" BRANCH_NAME)

####################################


# git clone $REPOSITORY_URL --branch $BRANCH_NAME --depth 1 superbuild-artifact
###############################################################################
set ( REMOTE "https://gitlab.orfeo-toolbox.org/gbonnefille/superbuild-artifact.git")
# set ( BRANCH_NAME "${IMAGE_NAME}/${SB_MD5}")

# Look for a GIT command-line client.
find_program(CTEST_GIT_COMMAND NAMES git git.cmd)

# FIXME: Replace ${GIT} variable with $[CTEST_GIT_COMMAND}"
set( GIT "${CTEST_GIT_COMMAND}" )

execute_process(
  COMMAND ${GIT} "clone" "${REMOTE}" "--branch" "${BRANCH_NAME}"
  "--depth" "1" "superbuild-artifact"
  WORKING_DIRECTORY ${OTB_SOURCE_DIR}
  RESULT_VARIABLE clone_res
  OUTPUT_VARIABLE clone_out
  ERROR_VARIABLE clone_err
  )

if ( DEBUG )
  message( "Clone")
  message( "clone_res = ${clone_res}" )
  message( "clone_out = ${clone_out}" )
  message( "clone_err = ${clone_err}" )
endif()

if (clone_res)
  message( SEND_ERROR "Problem in retreiving the archive")
  message( "Command : COMMAND ${GIT} clone ${REMOTE} --branch ${BRANCH_NAME} \
    --depth 1 superbuild-artifact")
  message( "Result :")
  message( "clone_res = ${clone_res}" )
  message( "clone_out = ${clone_out}" )
  message( "clone_err = ${clone_err}" )
  return()
endif()

set (CMAKE_COMMAND "cmake")
execute_process(
  COMMAND ${CMAKE_COMMAND} "-E" "tar" "xf"
  "${OTB_SOURCE_DIR}/superbuild-artifact/SuperBuild_Install.tar"
  RESULT_VARIABLE tar_res
  OUTPUT_VARIABLE tar_out
  ERROR_VARIABLE tar_err
  WORKING_DIRECTORY ${OTB_SOURCE_DIR}
  )

if ( DEBUG )
  message( "${CMAKE_COMMAND} -E tar xf ${OTB_SOURCE_DIR}/superbuild-artifact/SuperBuild_Install.tar")
  message( "tar_res: '${clone_res}'" )
  message( "tar_out: '${tar_out}'" )
  message( "tar_err: '${tar_err}'" )
endif()

set( XDK_PATH "${OTB_SOURCE_DIR}/xdk")

if ( DEBUG )
  if ( EXISTS "${XDK_PATH}")
    message("Xdk folder exists at ${XDK_PATH}")
  else()
    message("Something went wrong no folder in ${XDK_PATH}")
    message("The branch was ${BRANCH_NAME}")
  endif()
endif()

###########################################################################
###########################################################################
# Building OTB
###########################################################################
###########################################################################

set ( CTEST_BUILD_CONFIGURATION "Release" )
set ( CTEST_CMAKE_GENERATOR "Unix Makefiles" )
set ( CTEST_BUILD_FLAGS "-j8" )
set ( CTEST_SITE "${IMAGE_NAME}" )

# Find the build name and CI profile
set_dash_build_name()

# Directory variable
set ( CTEST_SOURCE_DIRECTORY "${OTB_SOURCE_DIR}" )
set ( CTEST_BINARY_DIRECTORY "${OTB_SOURCE_DIR}/build/" )
# install in same directory as xdk to be able to produce
# package afterwards
set ( CTEST_INSTALL_DIRECTORY "${XDK_PATH}" )
set ( PROJECT_SOURCE_DIR "${OTB_SOURCE_DIR}" )

set (CONFIGURE_OPTIONS  "")
include ( "${CMAKE_CURRENT_LIST_DIR}/configure_options.cmake" )

# For superbuild we need remote module 
foreach(remote_module DiapOTBModule OTBTemporalGapFilling)#otbGRM #Mosaic # #SertitObject
    set ( CONFIGURE_OPTIONS 
      "${CONFIGURE_OPTIONS}-DModule_${remote_module}:BOOL=ON;")
endforeach()

# SuperBuild case : one more configure option
set ( CONFIGURE_OPTIONS
  "${CONFIGURE_OPTIONS}-DCMAKE_PREFIX_PATH=${XDK_PATH};")

# FIX ME this part might platform dependent
set( GDAL_DATA "${XDK_PATH}/share/gdal" )
set( GEOTIFF_CSV "${XDK_PATH}/share/epsg_csv" )
set( PROJ_LIB "${XDK_PATH}/share" )
set( CTEST_ENVIRONMENT
"PATH=${XDK_PATH}/lib:${XDK_PATH}/bin:$ENV{PATH}
")
# It seems that we do not need that
# GDAL_DATA= GDAL_DATA
# GEOTIFF_CSV= GEOTIFF_CSV
# PROJ_LIB= PROJ_LIB

# Sources are already checked out : do nothing for update
set(CTEST_GIT_UPDATE_CUSTOM echo No update)

ctest_start (Experimental TRACK CI_Build)

ctest_update()

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
  ctest_submit()
  message( FATAL_ERROR "An error occurs during ctest_configure.")
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

# SEND_ERROR if build error
# FATAL_ERROR if build error?
if ( NOT _build_rv EQUAL 0 )
  message( SEND_ERROR "An error occurs during ctest_build.")
endif()

# Uncomment when ready for test
ctest_test(PARALLEL_LEVEL 8
           RETURN_VALUE _test_rv
           CAPTURE_CMAKE_ERROR _test_error
           )

# Test log
file ( WRITE 
  "${OTB_SOURCE_DIR}/log/test_return_value_log.txt" "${_test_rv}")
file ( WRITE 
  "${OTB_SOURCE_DIR}/log/test_cmake_error_log.txt" "${_test_error}")

if ( NOT _test_rv EQUAL 0 )
  message( WARNING "Some tests have failed.")
endif()

ctest_submit()

# We need to install OTB for package purposes
set ( MAKE_COMMAND "make")
execute_process(
  COMMAND ${MAKE_COMMAND} "install"
  WORKING_DIRECTORY ${CTEST_BINARY_DIRECTORY}
  RESULT_VARIABLE install_rv
  OUTPUT_VARIABLE install_out
  ERROR_VARIABLE install_err
  )

if ( DEBUG )
  message( "Install output")
  message( "install_res = ${install_res}" )
  message( "install_out = ${install_out}" )
  message( "install_err = ${install_err}" )
endif()

# Install log
file ( WRITE 
  "${OTB_SOURCE_DIR}/log/install_out_log.txt" "${install_out}")
file ( WRITE 
  "${OTB_SOURCE_DIR}/log/install_error_log.txt" "${install_err}")

if ( NOT install_rv EQUAL 0 )
  message( SEND_ERROR "Install have failed.")
endif()
