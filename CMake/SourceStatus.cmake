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

function(repository_status root_repo_dir result_var1)
  if(NOT EXISTS "${root_repo_dir}/.git")
    return()
  endif()

  find_package(Git)
  if(NOT GIT_FOUND)
    message(STATUS "git not found. Make sure git can be found in your PATH. (source status will not be reported)")
    set(${result_var1})
    return()
  endif()

  execute_process(COMMAND ${GIT_EXECUTABLE} config --get remote.origin.url
    WORKING_DIRECTORY ${root_repo_dir}
    OUTPUT_VARIABLE git_remote_origin_url
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
  if(git_remote_origin_url)
    get_filename_component(dir_name ${git_remote_origin_url} NAME)
  else()
    get_filename_component(dir_name ${root_repo_dir} NAME)
  endif()

  execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%H --oneline
    WORKING_DIRECTORY ${root_repo_dir}
    OUTPUT_VARIABLE last_commit_id
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

  #git symbolic-ref --short -q HEAD does not work on older git version
  #rather than checking for git version. it is better to not use that
  #option and employ cmake stuff to do the work
  execute_process(COMMAND ${GIT_EXECUTABLE} symbolic-ref -q HEAD
    WORKING_DIRECTORY ${root_repo_dir}
    OUTPUT_VARIABLE git_symbolic_ref_output
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)



  execute_process(COMMAND " ${GIT_EXECUTABLE} for-each-ref --format='%(objectname) %(refname:short)' refs/heads | awk "/^$(${GIT_EXECUTABLE} rev-parse HEAD)/ {print \$2}" "
    WORKING_DIRECTORY ${root_repo_dir}
    OUTPUT_VARIABLE git_describe_contains
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

  message(STATUS "git for each : ${git_describe_contains}")
  message(STATUS "CI_COMMIT_REF_NAME : ${CI_COMMIT_REF_NAME}")
  message(STATUS "GIT_BRANCH : ${GIT_BRANCH}")



  set(branch_name)
  if(git_symbolic_ref_output)
    get_filename_component(branch_name ${git_symbolic_ref_output} NAME)
  endif()

  set(return_msg "${dir_name} repository info: \n")
  set(return_msg "${return_msg} git log -1 --oneline     = [${last_commit_id}] \n")
  set(return_msg "${return_msg} git symbolic-ref -q HEAD = ${branch_name}")
  message(STATUS "CURRENT BRANCH is : ${branch_name}")
  set(${result_var1} ${return_msg} PARENT_SCOPE)

  #print other information inside function call
  execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%H
    WORKING_DIRECTORY ${root_repo_dir}
    OUTPUT_VARIABLE OTB_WC_REVISION
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

  message(STATUS "Repository status (${dir_name}) :")
  message(STATUS "  Repository revision is ${OTB_WC_REVISION}")

  execute_process(COMMAND ${GIT_EXECUTABLE} status -s
    WORKING_DIRECTORY ${root_repo_dir}
    OUTPUT_VARIABLE OTB_WC_STATUS
    TIMEOUT 20
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

  if(OTB_WC_STATUS)
    message(STATUS "  Local file modifications (${dir_name}) :")
    string(REPLACE "\n" "\n--     " OTB_WC_STATUS_PRETTYPRINT "    ${OTB_WC_STATUS}")
    message(STATUS "${OTB_WC_STATUS_PRETTYPRINT}")
  else()
    message(STATUS "  No files modified locally (${dir_name})")
  endif()


endfunction()



function(package_name root_repo_dir out_commit_id out_modify_pkg_name)
  if(NOT EXISTS "${root_repo_dir}/.git")
    return()
  endif()

  find_package(Git)
  if(NOT GIT_FOUND)
    message(STATUS "git not found. Make sure git can be found in your PATH. (source status will not be reported)")
    set(${result_var1})
    return()
  endif()

  execute_process(
            COMMAND git describe
            WORKING_DIRECTORY ${root_repo_dir}
            OUTPUT_VARIABLE GIT_CURRENT_COMMIT_ID OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(${out_commit_id} ${GIT_CURRENT_COMMIT_ID} PARENT_SCOPE)

  #git symbolic-ref --short -q HEAD does not work on older git version
  #rather than checking for git version. it is better to not use that
  #option and employ cmake stuff to do the work
  execute_process(COMMAND ${GIT_EXECUTABLE} symbolic-ref -q HEAD
    WORKING_DIRECTORY ${root_repo_dir}
    OUTPUT_VARIABLE git_symbolic_ref_output
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

  set(branch_name)
  if(git_symbolic_ref_output)
    get_filename_component(branch_name ${git_symbolic_ref_output} NAME)
  endif()

  message(STATUS "current_branch_name : ${branch_name}")

  if("${branch_name}" STREQUAL "2076-develop-package-names")
    set(${out_modify_pkg_name} "ON" PARENT_SCOPE)
  elseif()
    set(${out_modify_pkg_name} "OFF" PARENT_SCOPE)
  endif()

endfunction()


