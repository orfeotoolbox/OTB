#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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
function(patch_cmake_files)
  cmake_parse_arguments(PATCH  "" "NAME;VERSION;MATCH_STRING;REPLACE_VAR" "" ${ARGN} )

  set(PATCH_DIR_NAME ${PATCH_NAME}-${PATCH_VERSION})
  set(PATCH_DIR "${CMAKE_CURRENT_BINARY_DIR}/lib/cmake/${PATCH_DIR_NAME}")
  set(PATCH_STAGE_DIR ${CMAKE_CURRENT_BINARY_DIR}/patched/${PATCH_DIR_NAME})

  ##message("COPY ${PATCH_DIR} to ${PATCH_STAGE_DIR} for patching")

  execute_process(
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PATCH_STAGE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PATCH_DIR}" "${PATCH_STAGE_DIR}"
    )

  set(DIR_LIST "${PATCH_STAGE_DIR}|${PATCH_STAGE_DIR}/Modules")

  execute_process(COMMAND ${CMAKE_COMMAND}
    -DP_DIRS=${DIR_LIST}
    -DP_MATCH=${PATCH_MATCH_STRING}
    -DP_REPLACE=${PATCH_REPLACE_VAR}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/CMake/post_install.cmake
    RESULT_VARIABLE patch_${PATCH_NAME}_cmake_rv
    )

  if(patch_${PATCH_NAME}_cmake_rv)
    message(FATAL_ERROR "    execute_process() failed.")
  endif()

  install_without_message("${PATCH_STAGE_DIR}" "${XDK_INSTALL_PATH}/lib/cmake")

endfunction()
