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
function(install_qtdev_files)
  if(NOT HAVE_QT)
    return()
  endif()
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
      install_without_message("${SUPERBUILD_INSTALL_DIR}/${QT_REQ_DIR}" "")
    endif()
  endforeach()

  install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Files/qt.conf
    DESTINATION ${PKG_STAGE_DIR}/bin
    )

  if(IS_DIRECTORY "${SUPERBUILD_INSTALL_DIR}/lib/fonts")
    install_without_message("${SUPERBUILD_INSTALL_DIR}/lib/fonts" "lib")
  endif()
endfunction()
