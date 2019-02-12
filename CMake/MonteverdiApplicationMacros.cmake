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

macro(create_monteverdi_application)
  cmake_parse_arguments(APPLICATION  "" "NAME;OUTPUT_NAME;COMPONENT_NAME" "SOURCES;LINK_LIBRARIES" ${ARGN} )

  if(WIN32)
    add_executable(${APPLICATION_NAME}
      WIN32
      ${APPLICATION_SOURCES})
  else() #Unix
    add_executable(${APPLICATION_NAME}
      ${APPLICATION_SOURCES})
  endif()

  if (APPLICATION_OUTPUT_NAME)
    set_target_properties(${APPLICATION_NAME} PROPERTIES OUTPUT_NAME ${APPLICATION_OUTPUT_NAME})
  endif()

  target_link_libraries(${APPLICATION_NAME} ${APPLICATION_LINK_LIBRARIES})

install(TARGETS ${APPLICATION_NAME}
  RUNTIME DESTINATION ${Monteverdi_INSTALL_BIN_DIR} COMPONENT ${APPLICATION_COMPONENT_NAME}
  LIBRARY DESTINATION ${Monteverdi_INSTALL_LIB_DIR} COMPONENT ${APPLICATION_COMPONENT_NAME}
  ARCHIVE DESTINATION ${Monteverdi_INSTALL_LIB_DIR} COMPONENT ${APPLICATION_COMPONENT_NAME}
  )

endmacro(create_monteverdi_application)