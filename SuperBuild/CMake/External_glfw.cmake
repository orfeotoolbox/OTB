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

INCLUDE_ONCE_MACRO(GLFW)

SETUP_SUPERBUILD(GLFW)

ExternalProject_Add(GLFW
  PREFIX GLFW
  URL "https://github.com/glfw/glfw/releases/download/3.1.2/glfw-3.1.2.zip"
  URL_MD5 8023327bfe979b3fe735e449e2f54842
  BINARY_DIR ${GLFW_SB_BUILD_DIR}
  INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
  -DGLFW_BUILD_EXAMPLES:BOOL=OFF
  -DGLFW_BUILD_DOCS:BOOL=OFF
  -DGLFW_BUILD_TESTS:BOOL=OFF
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

SUPERBUILD_PATCH_SOURCE(GLFW)
