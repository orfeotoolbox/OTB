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


function(func_install_xdk_files)

  func_patch_cmake_files_for(NAME "ITK"
    VERSION "${PKG_ITK_SB_VERSION}"
    MATCH_STRING  "${CMAKE_INSTALL_PREFIX}"
    REPLACE_VAR "ITK_INSTALL_PREFIX"
    )
  
  func_patch_cmake_files_for(NAME "OTB"
    VERSION "${PKG_OTB_VERSION_MAJOR}.${PKG_OTB_VERSION_MINOR}"
    MATCH_STRING  "${CMAKE_INSTALL_PREFIX}"
    REPLACE_VAR "OTB_INSTALL_PREFIX"
    )
  
  set(QT_REQ_DIRS)
  if(MSVC)
    file(GLOB ALL_LIB_FILES "${SUPERBUILD_INSTALL_DIR}/lib/*.lib")
  endif()
  
  foreach(LIB_FILE ${LIB_FILES})
    install_rule(${LIB_FILE})
  endforeach()
  
    
  list(APPEND QT_REQ_DIRS imports)

  
  list(APPEND QT_REQ_DIRS mkspecs)
  list(APPEND QT_REQ_DIRS plugins)
  list(APPEND QT_REQ_DIRS translations)
  foreach(QT_REQ_DIR ${QT_REQ_DIRS} )
    if(EXISTS "${SUPERBUILD_INSTALL_DIR}/${QT_REQ_DIR}")
      func_install_without_message("${SUPERBUILD_INSTALL_DIR}/${QT_REQ_DIR}" "")
    endif()
  endforeach()

endfunction() #func_install_xdk_files


function(func_install_monteverdi_support_files)

  # Just check if required variables are defined.
  foreach(req
      PACKAGE_OTB_SRC_DIR
      PACKAGE_SUPPORT_FILES_DIR
      QT_PLUGINS_DIR
      PKG_STAGE_BIN_DIR
      PKG_QTPLUGINS_DIR

      PKG_OTB_TRANSLATIONS_DIRNAME
      )
    if(NOT DEFINED ${req} OR "${${req}}" STREQUAL "")
      message(FATAL_ERROR "you must set ${req} before calling this method")
    endif()
    set(vars "${vars}  ${req}=[${${req}}]\n")
  endforeach(req)

  #install icon file for .app file. Monteverdi and Mapla has same icon!
  if(APPLE)
    install(FILES ${PACKAGE_SUPPORT_FILES_DIR}/Monteverdi.icns
      DESTINATION ${PKG_STAGE_DIR})
  endif()


  if(UNIX)
    file(GLOB DOT_A_FILES ${SUPERBUILD_INSTALL_DIR}/lib/lib*.a )
    foreach(DOT_A_FILE ${DOT_A_FILES})
      install(FILES "${DOT_A_FILE}" DESTINATION ${PKG_STAGE_DIR}/lib)
    endforeach()
  endif()



endfunction()

