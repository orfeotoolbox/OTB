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
function(install_include_dirs)
  # #install ${SUPERBUILD_INSTALL_DIR}/include directory.
  file(GLOB ALL_IN_INCLUDE_DIR "${SUPERBUILD_INSTALL_DIR}/include/*")
  foreach(INCLUDE_DIR_ITEM ${ALL_IN_INCLUDE_DIR})
    get_filename_component(INCLUDE_DIR_ITEM_name ${INCLUDE_DIR_ITEM} NAME)
    if( IS_DIRECTORY ${INCLUDE_DIR_ITEM})
      install(CODE
        "message(STATUS \"Installing: ${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR}/include/${INCLUDE_DIR_ITEM_name}/\")" )
      install(DIRECTORY   "${INCLUDE_DIR_ITEM}"
        DESTINATION "${PKG_STAGE_DIR}/include/"
        MESSAGE_NEVER)
    else()
      install(FILES   "${INCLUDE_DIR_ITEM}"
        DESTINATION "${PKG_STAGE_DIR}/include/" )
    endif() #if( IS_DIRECTORY    
  endforeach()
endfunction()
