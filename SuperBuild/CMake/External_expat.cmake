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

INCLUDE_ONCE_MACRO(EXPAT)

SETUP_SUPERBUILD(EXPAT)

ExternalProject_Add(EXPAT
  PREFIX EXPAT
  URL "http://downloads.sourceforge.net/project/expat/expat/2.1.0/expat-2.1.0.tar.gz"
  URL_MD5 dd7dab7a5fea97d2a6a43f511449b7cd
  BINARY_DIR ${EXPAT_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
  -DBUILD_examples:BOOL=OFF
  -DBUILD_tests:BOOL=OFF
  -DBUILD_tools:BOOL=OFF
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

SUPERBUILD_UPDATE_CMAKE_VARIABLES(EXPAT FALSE)
