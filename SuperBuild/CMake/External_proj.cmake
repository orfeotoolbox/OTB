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

INCLUDE_ONCE_MACRO(PROJ)

SETUP_SUPERBUILD(PROJ)

ExternalProject_Add(PROJ
  PREFIX PROJ
  URL "http://download.osgeo.org/proj/proj-4.9.3.tar.gz"
  URL_MD5 d598336ca834742735137c5674b214a1
  BINARY_DIR ${PROJ_SB_SRC}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
  -DBUILD_LIBPROJ_SHARED:BOOL=TRUE
  -DVERSIONED_OUTPUT:BOOL=FALSE
  -DBUILD_FRAMEWORKS_AND_BUNDLE:BOOL=FALSE
  -DPROJ_LIB_SUBDIR:STRING=lib
  -DPROJ_INCLUDE_SUBDIR:STRING=include
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

SUPERBUILD_PATCH_SOURCE(PROJ)
