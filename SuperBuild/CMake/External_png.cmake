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

INCLUDE_ONCE_MACRO(PNG)

SETUP_SUPERBUILD(PNG)

# declare dependencies
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(PNG ZLIB)


ExternalProject_Add(PNG
  PREFIX PNG
  URL "http://downloads.sourceforge.net/project/libpng/libpng16/older-releases/1.6.16/lpng1616.zip"
  URL_MD5 c90c9587c9a5c735327fb3f6900f6b03
  BINARY_DIR ${PNG_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  DEPENDS ${PNG_DEPENDENCIES}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
  -DSKIP_INSTALL_EXECUTABLES:BOOL=OFF
  -DPNG_STATIC:BOOL=OFF
  -DPNG_SHARED:BOOL=ON
  -DPNG_TESTS:BOOL=OFF
  -DVERSIONED_OUTPUT_LIB:BOOL=OFF
  -DSKIP_INSTALL_FILES:BOOL=ON
  -DSKIP_INSTALL_EXPORT:BOOL=ON
  -DSKIP_INSTALL_EXECUTABLES:BOOL=OFF
  ${PNG_SB_CONFIG}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )
set(_SB_PNG_PNG_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)

SUPERBUILD_PATCH_SOURCE(PNG)

