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
# This script is for the superbuild build on the CI platform

include( "${CMAKE_CURRENT_LIST_DIR}/macros.cmake" )

set (ENV{LANG} "C") # Only ascii output
get_filename_component(OTB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)

set ( SUPERBUILD_SOURCE_DIR "${OTB_SOURCE_DIR}/SuperBuild" )

set ( CTEST_BUILD_CONFIGURATION "Release" )
set ( CTEST_CMAKE_GENERATOR "Unix Makefiles" )
set ( PROJECT_SOURCE_DIR "${SUPERBUILD_SOURCE_DIR}" )
set ( CTEST_SOURCE_DIRECTORY "${SUPERBUILD_SOURCE_DIR}" )
set ( CTEST_BINARY_DIRECTORY "${OTB_SOURCE_DIR}/build/" )
set ( CTEST_SITE "${IMAGE_NAME}" )


# Find the build name and CI profile
set_dash_build_name()

# We need a directory independent from user
# in CI the architecture is /builds/user/otb
# So we will go in /builds/
# This is platform dependent, and the next step (build) also
# depends on that, as some paths are hardcoded
# This can be fixed with a packaging of OTB_DEPENDS
set (CTEST_INSTALL_DIRECTORY "${OTB_SOURCE_DIR}/xdk/")

# HACK
# This is needed because when using return() function ctest is trying
# to run the CTEST_COMMAND. And we need it to not produce an error
set (CTEST_COMMAND "echo \"Exit\"") # HACK FIX ME
set (CMAKE_COMMAND "cmake")

########################################################################
########################################################################
# Build process
########################################################################
########################################################################

# Look for a GIT command-line client.
find_program(CTEST_GIT_COMMAND NAMES git git.cmd)

# FIXME: Replace ${GIT} variable with $[CTEST_GIT_COMMAND}"
set( GIT "${CTEST_GIT_COMMAND}" )

# Sources are already checked out : do nothing for update
set(CTEST_GIT_UPDATE_CUSTOM echo No update)

ctest_start (Experimental TRACK CI_Prepare)

ctest_update( SOURCE "${OTB_SOURCE_DIR}" )

set(CTEST_BUILD_FLAGS "-j16")

set ( SB_CONFIGURE_OPTIONS "")
include( "${CMAKE_CURRENT_LIST_DIR}/sb_configure_options.cmake" )

ctest_configure(BUILD "${CTEST_BINARY_DIRECTORY}"
    SOURCE "${SUPERBUILD_SOURCE_DIR}"
    OPTIONS "${SB_CONFIGURE_OPTIONS}"
    RETURN_VALUE _configure_rv
    CAPTURE_CMAKE_ERROR _configure_error
    )

if ( NOT _configure_rv EQUAL 0 )
  ctest_submit()
  message( SEND_ERROR "An error occurs during ctest_configure. Dependencies might be buggy.")
  return()
endif()

########################################################################
########################################################################
# Check process
########################################################################
########################################################################
# Once that we have configure our build we can check if it exists a
# corresponding SB on superbuild-artifact

# How to get md5sum:
# * concatenate all source files in one
# * add configure result : CMakeCache.txt
####################################
file( GLOB_RECURSE sb_file_list "${OTB_SOURCE_DIR}/SuperBuild/*")
set( SB_TXT "${OTB_SOURCE_DIR}/full_sb.txt")
foreach(sb_file  ${sb_file_list})
  file(READ ${sb_file} CONTENTS)
  file(APPEND ${SB_TXT} "${sb_file}${CONTENTS}")
endforeach(sb_file)
file(READ "${CTEST_BINARY_DIRECTORY}/CMakeCache.txt" CMAKE_ORIG)
file(APPEND ${SB_TXT} "CMakeCache.txt${CMAKE_ORIG}")
file ( MD5 "${SB_TXT}" SB_MD5)
message ( "SB_MD5 = ${SB_MD5}" )
file (REMOVE ${SB_TXT})

####################################

# checkout part
# we look for the right branch
# Branch name cannot have a ":"
# git ls-remote $REMOTE $BRANCH_NAME
####################################
file ( WRITE "${OTB_SOURCE_DIR}/sb_branch.txt" "${IMAGE_NAME}/${SB_MD5}")
message( "Checking out git for existence of archive")
set ( REMOTE "https://gitlab.orfeo-toolbox.org/gbonnefille/superbuild-artifact/")
set ( BRANCH_NAME "${IMAGE_NAME}/${SB_MD5}")

execute_process(
  COMMAND ${GIT} "ls-remote" "${REMOTE}" "${BRANCH_NAME}"
  OUTPUT_VARIABLE IS_SB_BUILD
  )
if ( IS_SB_BUILD )
  message( "Superbuild is already build for ${IMAGE_NAME} with sources as ${SB_MD5}")
  return()
else()
  message( "No build available, this job will build and push OTB_DEPENDS")
endif()
####################################
# Back to build

ctest_build(BUILD "${CTEST_BINARY_DIRECTORY}"
            TARGET "OTB_DEPENDS"
            RETURN_VALUE _build_rv
            NUMBER_ERRORS _build_nb_err
            CAPTURE_CMAKE_ERROR _build_error
            )

if ( DEBUG )
  message( "Status for build:" )
  message("_build_rv=${_build_rv}")
  message("_build_nb_err=${_build_nb_err}")
  message("_build_error=${_build_error}")
endif()

if ( ( NOT ${_build_nb_err} EQUAL 0 ) OR ( ${_build_error} EQUAL -1 ))
  message( FATAL_ERROR "An error occurs during ctest_build.")
endif()

ctest_submit()

########################################################################
########################################################################
# Git process
########################################################################
########################################################################

# WE PUSH ONLY IF BUILD SUCCEED
# The image used will be passed to this script.
# TODO verify that images does not have forbidden char in there name
# TODO right now we rely on ctest_build to know whether there has been an error
# in build, whereas SuperBuild does not necessarily return an error if something
# goes wrong

# REPOSITORY_GIT_URL and REMOTE whould be the same. Right now there are
# different because one is https and one is ssh. Both should be ssh.
set( REPOSITORY_GIT_URL "git@gitlab.orfeo-toolbox.org:gbonnefille/superbuild-artifact.git")
# We clone master to have a basic configuration, mainly a correct .gitattribute
# git clone $REMOTE --branch master --depth 1 superbuild-artifact
execute_process(
  COMMAND ${GIT} "clone" "${REPOSITORY_GIT_URL}"
  "--branch" "master" "--depth" "1" "superbuild-artifact"
  WORKING_DIRECTORY "${OTB_SOURCE_DIR}"
  )
set ( SB_ARTIFACT_GIT "${OTB_SOURCE_DIR}/superbuild-artifact" )

# create a branche
execute_process(
  COMMAND ${GIT} "checkout" "-b" "${BRANCH_NAME}"
  WORKING_DIRECTORY ${SB_ARTIFACT_GIT}
  RESULT_VARIABLE co_res
  OUTPUT_VARIABLE co_out
  ERROR_VARIABLE co_err
  )

if ( DEBUG )
  message( "Step 4: check-o")
  message( "co_res = ${co_res}" )
  message( "co_out = ${co_out}" )
  message( "co_err = ${co_err}" )
endif()

set ( SB_TAR_NAME "SuperBuild_Install.tar" )

# Creating the tar
# May be for easier maintainability the tar name should be the same as the
# file inside.
execute_process(
  COMMAND ${CMAKE_COMMAND} "-E" "tar" "cf" "${SB_ARTIFACT_GIT}/${SB_TAR_NAME}"
  -- "${CTEST_INSTALL_DIRECTORY}"
  WORKING_DIRECTORY ${OTB_SOURCE_DIR}
  )

if ( DEBUG )
  if (EXISTS "${SB_ARTIFACT_GIT}/${SB_TAR_NAME}")
    message("Tar file exists in superbuild_artefact at: ${SB_ARTIFACT_GIT}/${SB_TAR_NAME}")
  else()
    message("Tar file does not exist")
  endif()
endif()

# add the file
execute_process(
  COMMAND ${GIT} "add" "${SB_TAR_NAME}"
  WORKING_DIRECTORY ${SB_ARTIFACT_GIT}
  RESULT_VARIABLE add_res
  OUTPUT_VARIABLE add_out
  ERROR_VARIABLE add_err
  )

if ( DEBUG )
  message( "Step 5: add")
  message( "add_res = ${add_res}" )
  message( "add_out = ${add_out}" )
  message( "add_err = ${add_err}" )
endif()


# commit
# We need the author because otherwise the mail is wrong
# In our case if toto is deploying a key in superbuild-artifact repo
# the the mail will be toto's
execute_process(
  COMMAND ${GIT} "commit" "--author=\"otbbot <otbbot@orfeo-toolbox.org>\""
  "-m" "\"New Superbuild for ${SB_MD5} on ${IMAGE_NAME}\""
  WORKING_DIRECTORY ${SB_ARTIFACT_GIT}
  RESULT_VARIABLE com_res
  OUTPUT_VARIABLE com_out
  ERROR_VARIABLE com_err
  )

if ( DEBUG )
  message( "Step 6: com")
  message( "com_res = ${com_res}" )
  message( "com_out = ${com_out}" )
  message( "com_err = ${com_err}" )
endif()


# This part is just for debug
if ( DEBUG )
  execute_process(
    COMMAND ${GIT} "log" "-1"
    WORKING_DIRECTORY ${SB_ARTIFACT_GIT}
    RESULT_VARIABLE log_res
    OUTPUT_VARIABLE log_out
    ERROR_VARIABLE log_err
    )

  message( "Step 6bis: log")
  message( "log_res = ${log_res}" )
  message( "log_out = ${log_out}" )
  message( "log_err = ${log_err}" )
endif()

# push
# we should be able to do a simple : git push origin $BRANCH_NAME
execute_process(
  COMMAND ${GIT} "push" "${REPOSITORY_GIT_URL}" "${BRANCH_NAME}"
  WORKING_DIRECTORY ${SB_ARTIFACT_GIT}
  RESULT_VARIABLE push_res
  OUTPUT_VARIABLE push_out
  ERROR_VARIABLE push_err
  )

if ( DEBUG )
  message( "Step 7: push")
  message( "push_res = ${push_res}" )
  message( "push_out = ${push_out}" )
  message( "push_err = ${push_err}" )
endif()