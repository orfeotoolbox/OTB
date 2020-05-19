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

INCLUDE_ONCE_MACRO(MUPARSERX)

SETUP_SUPERBUILD(MUPARSERX)

set(MUPARSERX_SB_CONFIG)
if(MSVC AND BUILD_SHARED_LIBS)
  list(APPEND
    MUPARSERX_SB_CONFIG
    -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS:BOOL=TRUE)
endif()

ExternalProject_Add(MUPARSERX
  PREFIX MUPARSERX
  URL "https://github.com/beltoforion/muparserx/archive/v4.0.7.zip"
  URL_MD5 67819fc478436ea8f647c804e6f2e1a2
  BINARY_DIR ${MUPARSERX_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
  -DBUILD_EXAMPLES:BOOL=OFF
  ${MUPARSERX_SB_CONFIG}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  DEPENDS ${MUPARSERX_DEPENDENCIES}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

#Patch mpParserMessageProvider.cpp. This patch is integrated upstream but not yet released (last release is 4.0.7)
SUPERBUILD_PATCH_SOURCE(MUPARSERX)

SUPERBUILD_UPDATE_CMAKE_VARIABLES(MUPARSERX FALSE muparserx muparserx)
