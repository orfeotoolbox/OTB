#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

macro(cleanup_package)

if(EXISTS "${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR}")

message(STATUS "Deleting package staging directory: ${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR}")
execute_process(COMMAND 
${CMAKE_COMMAND} -E remove_directory
  "${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR}"
  RESULT_VARIABLE cleanup_stagedir_rv
  OUTPUT_VARIABLE cleanup_stagedir_ov
  ERROR_VARIABLE cleanup_stagedir_ov
  )
  
  if(cleanup_stagedir_rv)
  message(FATAL_ERROR  "Delete ${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR} - FAILED\ncleanup_stagedir_ov=${cleanup_stagedir_ov}")
  else()
  message(STATUS "Delete ${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR} - OK")  
  endif()
endif()

if(EXISTS "${PACKAGE_OUTPUT_FILE}")
message(STATUS "Remove PACKAGE_OUTPUT_FILE: ${PACKAGE_OUTPUT_FILE}")
execute_process(COMMAND 
${CMAKE_COMMAND} -E remove -f "${PACKAGE_OUTPUT_FILE}"
  RESULT_VARIABLE cleanup_pkgfile_rv
  OUTPUT_VARIABLE cleanup_pkgfile_ov
  ERROR_VARIABLE cleanup_pkgfile_ov
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
  
    if(cleanup_pkgfile_rv)
  message(FATAL_ERROR  "Delete ${PACKAGE_OUTPUT_FILE} - FAILED\ncleanup_pkgfile_ov=${cleanup_pkgfile_ov}")
  else()
  message(STATUS "Delete ${PACKAGE_OUTPUT_FILE} - OK")  
  endif()
  
endif()
endmacro()
