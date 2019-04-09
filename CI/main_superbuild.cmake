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
###########################################################################
###########################################################################
# Download xkd
###########################################################################
###########################################################################

# Get md5sum of the sources
###########################
file( GLOB_RECURSE sb_file_list "${OTB_SOURCE_DIR}/SuperBuild/*")
set( SB_TXT "${OTB_SOURCE_DIR}/full_sb.txt")
foreach(sb_file  ${sb_file_list})
  file(READ ${sb_file} CONTENTS)
  file(APPEND ${SB_TXT} "${sb_file}${CONTENTS}")
endforeach(sb_file)
file ( MD5 "${SB_TXT}" SB_MD5)
message ( "SB_MD5 = ${SB_MD5}" )
file (REMOVE ${SB_TXT})

####################################


# git clone $REPOSITORY_URL --branch $BRANCH_NAME --depth 1 superbuild-artifact
###############################################################################
set ( REMOTE "https://gitlab.orfeo-toolbox.org/gbonnefille/superbuild-artifact.git")
set ( BRANCH_NAME "${IMAGE_NAME}/${SB_MD5}")
set( GIT "git" )

execute_process(
  COMMAND ${GIT} "clone" "${REMOTE}" "--branch" "${BRANCH_NAME}"
  "--depth" "1" "superbuild-artifact"
  WORKING_DIRECTORY ${CI_PROJ_DIR}
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
  return()
endif()

set (CMAKE_COMMAND "cmake")
execute_process(
  COMMAND ${CMAKE_COMMAND} "-E" "tar" "xf" 
  "${CI_PROJ_DIR}/superbuild-artifact/SuperBuild_Install.tar"
  WORKING_DIRECTORY ${CI_ROOT_DIR}
  )

set( XDK_PATH "${CI_ROOT_DIR}/xdk")

if ( DEBUG )
  if ( EXISTS "${XDK_PATH}")
    message("Tar file exists at ${XDK_PATH}")
  else()
    message("Something went wrong no tar file in ${XDK_PATH}")
  endif()
endif()

###########################################################################
###########################################################################
# Building OTB
###########################################################################
###########################################################################

set ( CTEST_BUILD_CONFIGURATION "Release" )
set ( CTEST_CMAKE_GENERATOR "Unix Makefiles" )
set ( CTEST_BUILD_FLAGS "-j16" )
set ( CTEST_BUILD_NAME "Superbuild_Build_Otb" )
set ( CTEST_SITE "${IMAGE_NAME}" )

# Directory variable
set ( CTEST_SOURCE_DIRECTORY "${OTB_SOURCE_DIR}" )
set ( CTEST_BINARY_DIRECTORY "${OTB_SOURCE_DIR}/build/" )
# install in same directory as xdk to be able to produce
# package afterwards
set ( CTEST_INSTALL_DIRECTORY "${XDK_PATH}" )
set ( PROJECT_SOURCE_DIR "${OTB_SOURCE_DIR}" )

# SuperBuild case : one configure option
set ( CONFIGURE_OPTIONS  "-DCMAKE_PREFIX_PATH=${XDK_PATH}" )

# FIX ME this part might platform dependent
set( GDAL_DATA "${XDK_PATH}/share/gdal" )
set( GEOTIFF_CSV "${XDK_PATH}/share/epsg_csv" )
set( PROJ_LIB "${XDK_PATH}/share" )
set( CTEST_ENVIRONMENT 
"PATH=${XDK_PATH}/lib:${XDK_PATH}/bin:$ENV{PATH}
GDAL_DATA= GDAL_DATA
GEOTIFF_CSV= GEOTIFF_CSV
PROJ_LIB= PROJ_LIB
")

ctest_start (Experimental TRACK Experimental)

ctest_configure(BUILD "${CTEST_BINARY_DIRECTORY}"
    SOURCE "${OTB_SOURCE_DIR}"
    OPTIONS "${CONFIGURE_OPTIONS}"
    RETURN_VALUE _configure_rv
    CAPTURE_CMAKE_ERROR _configure_error
    )

if ( NOT _configure_rv EQUAL 0 )
  ctest_submit()
  message( SEND_ERROR "An error occurs during ctest_configure.")
endif()

ctest_build(BUILD "${CTEST_BINARY_DIRECTORY}"
            RETURN_VALUE _build_rv
            CAPTURE_CMAKE_ERROR _build_error
            )

if ( NOT _build_rv EQUAL 0 )
  ctest_submit()
  message( SEND_ERROR "An error occurs during ctest_build.")
endif()

# Uncomment when ready for test
# ctest_test(PARALLEL_LEVEL 8
#            RETURN_VALUE _test_rv
#            CAPTURE_CMAKE_ERROR _test_error
#            )

# if ( NOT _test_rv EQUAL 0 )
#   ctest_submit()
#   message( SEND_ERROR "An error occurs during ctest_test.")
# endif()

ctest_submit()

# We need to install OTB for package purposes
set ( MAKE_COMMAND "make")
execute_process(
  COMMAND ${MAKE_COMMAND} "install"
  WORKING_DIRECTORY ${CTEST_BINARY_DIRECTORY}
  RESULT_VARIABLE install_res
  OUTPUT_VARIABLE install_out
  ERROR_VARIABLE install_err
  )

if ( DEBUG )
  message( "Install output")
  message( "install_res = ${install_res}" )
  message( "install_out = ${install_out}" )
  message( "install_err = ${install_err}" )
endif()

# Artifacts can only be in project dir...
file ( COPY "${XDK_PATH}" DESTINATION "${OTB_SOURCE_DIR}/install")

