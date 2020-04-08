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

# Deploy a qt.conf file in the specified
# target build directory, as a pre-build step
macro( deploy_qt_conf TARGET_NAME )

  add_custom_command( 
      TARGET ${TARGET_NAME}
      PRE_BUILD 
      COMMAND ${CMAKE_COMMAND}
      ARGS -D QT_TRANSLATIONS_DIR:PATH=${QT_TRANSLATIONS_DIR}
           -D QT_PLUGINS_DIR:PATH=${QT_PLUGINS_DIR}
           -D Monteverdi_SOURCE_DIR:PATH=${Monteverdi_SOURCE_DIR}
           -D TARGET_DIR:PATH=$<TARGET_FILE_DIR:${TARGET_NAME}>
           -P ${Monteverdi_SOURCE_DIR}/CMake/DeployQtConfScript.cmake
      COMMENT "Deploying qt.conf in ${TARGET_NAME} build dir"
      VERBATIM
  )

endmacro()
