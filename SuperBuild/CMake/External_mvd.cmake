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

if( __EXTERNAL_MVD__)
  return()
else()
  set(__EXTERNAL_MVD__ 1)
endif()

SETUP_SUPERBUILD(MVD)

set(MONTEVERDI_GIT_TAG "develop" CACHE STRING "branch name of monteverdi to build. Default is 'develop'")

# declare dependencies
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(MVD OTB QWT QT4)

set(MVD_SB_CONFIG)
ADD_SUPERBUILD_CMAKE_VAR(MVD OTB_DIR)
ADD_SUPERBUILD_CMAKE_VAR(MVD QWT_INCLUDE_DIR)
ADD_SUPERBUILD_CMAKE_VAR(MVD QWT_LIBRARY)
ADD_SUPERBUILD_CMAKE_VAR(MVD QT_QMAKE_EXECUTABLE)

#TODO: control build testing via cmake variable properly

ExternalProject_Add(MVD
  PREFIX MVD
  GIT_REPOSITORY "https://git@git.orfeo-toolbox.org/git/monteverdi2.git"
  GIT_TAG "${MONTEVERDI_GIT_TAG}"
  SOURCE_DIR ${MVD_SB_SRC}
  BINARY_DIR ${MVD_SB_BUILD_DIR}
  INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  DEPENDS ${MVD_DEPENDENCIES}
  CMAKE_CACHE_ARGS ${SB_CMAKE_CACHE_ARGS}
  -DMonteverdi_INSTALL_LIB_DIR:STRING=lib
  -DOTB_DATA_ROOT:STRING=${OTB_DATA_ROOT}
  ${MVD_SB_CONFIG}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
)
