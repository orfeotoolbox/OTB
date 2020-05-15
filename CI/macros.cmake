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
# This script is for the superbuild build on the CI platform

# Find the build name and CI profile
macro( set_dash_build_name )
  set(ci_profile wip)

  set(ci_mr_source "$ENV{CI_MERGE_REQUEST_SOURCE_BRANCH_NAME}")
  set(ci_mr_target "$ENV{CI_MERGE_REQUEST_TARGET_BRANCH_NAME}")
  set(ci_mr_iid "$ENV{CI_MERGE_REQUEST_IID}")

  set(ci_pipeline_id "$ENV{CI_PIPELINE_ID}")
  set(ci_job_id "$ENV{CI_JOB_ID}")

  set(ci_ref_name "$ENV{CI_COMMIT_REF_NAME}")

  set (CTEST_BUILD_NAME ${ci_short_sha})

  if(ci_mr_source AND ci_mr_target AND ci_mr_iid)
    set (CTEST_BUILD_NAME "${ci_mr_source} (MR ${ci_mr_iid}) P-${ci_pipeline_id}")
    set(ci_profile mr)

  elseif(ci_ref_name)
    set (CTEST_BUILD_NAME "${ci_ref_name} P-${ci_pipeline_id}")

    if("${ci_ref_name}" STREQUAL "develop")
      set(ci_profile develop)

    elseif("${ci_ref_name}" MATCHES "^release-[0-9]+\\.[0-9]+\$")
      set(ci_profile release)
    endif()

  endif()

endmacro()

# Download and extract an XDK (needs OTB_SOURCE_DIR, and the file sb_branch.txt in it)
macro(get_xdk)
  # How to get md5sum:
  # * concatenate all source files in one
  # * add configure result from previous job ${OTB_SOURCE_DIR}/build/CMakeCache.txt
  file(STRINGS "${OTB_SOURCE_DIR}/sb_branch.txt" BRANCH_NAME LIMIT_COUNT 1 REGEX "^([A-Za-z0-9]|/|-|_|\\.)+" )

  # Look for a GIT command-line client.
  find_program(CTEST_GIT_COMMAND NAMES git git.cmd)

  # Download xdk :
  #   git clone $REPOSITORY_URL --branch $BRANCH_NAME --depth 1 superbuild-artifact
  set ( REMOTE "https://gitlab.orfeo-toolbox.org/orfeotoolbox/superbuild-artifact.git")
  execute_process(
    COMMAND ${CTEST_GIT_COMMAND} "clone" "${REMOTE}" "--branch" "${BRANCH_NAME}"
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
    message( "Command : COMMAND ${CTEST_GIT_COMMAND} clone ${REMOTE} --branch ${BRANCH_NAME} \
      --depth 1 superbuild-artifact")
    message( "Result :")
    message( "clone_res = ${clone_res}" )
    message( "clone_out = ${clone_out}" )
    message( "clone_err = ${clone_err}" )
    message( FATAL_ERROR "Problem in retreiving the archive")
  endif()

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
endmacro()
