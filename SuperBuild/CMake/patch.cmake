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

find_program(PATCH_PROGRAM NAMES patch)
if(NOT PATCH_PROGRAM)
  message(FATAL_ERROR "patch program not found. PATCH_PROGRAM. search names :' patch'")
endif()

separate_arguments(DOT_DIFF_FILES)
list(SORT DOT_DIFF_FILES)

if(NOT PATCH_ARGS)
  set(PATCH_ARGS "-ut")
endif()

foreach(dot_diff_file ${DOT_DIFF_FILES})
  message("Input patch file: ${dot_diff_file}")
  execute_process(
    COMMAND
    ${CMAKE_COMMAND} -E chdir ${SOURCE_DIR}
    ${PATCH_PROGRAM} "${PATCH_ARGS}" "-p1" "-i" "${dot_diff_file}"
    RESULT_VARIABLE patch_rv
    OUTPUT_VARIABLE patch_ov
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_STRIP_TRAILING_WHITESPACE)

  set(PATCHING_FAILED FALSE)

  if(NOT "${patch_rv}" STREQUAL "0")
    if( NOT "${patch_ov}" MATCHES "previously applied")
      set(PATCHING_FAILED TRUE)
    endif()
  endif()

  if( "${patch_ov}" MATCHES "FAILED")
    set(PATCHING_FAILED TRUE)
  endif()

  if(PATCHING_FAILED)
    message(FATAL_ERROR
      "${PATCH_PROGRAM} ${PATCH_ARGS} -p1 -i ${dot_diff_file} failed\n error: ${patch_ov} \n")
  else()
    message("${patch_ov}")
  endif()

  unset(patch_rv)
  unset(patch_ov)
endforeach()
