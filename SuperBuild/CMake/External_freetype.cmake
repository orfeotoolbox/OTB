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

INCLUDE_ONCE_MACRO(FREETYPE)

SETUP_SUPERBUILD(FREETYPE)

set(FREETYPE_SB_CONFIG)
if(MSVC AND BUILD_SHARED_LIBS)
  list(APPEND FREETYPE_SB_CONFIG
    -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS:BOOL=TRUE)
endif()

ExternalProject_Add(FREETYPE
  PREFIX FREETYPE
  URL "http://download.savannah.gnu.org/releases/freetype/freetype-2.6.tar.gz"
  URL_MD5 1d733ea6c1b7b3df38169fbdbec47d2b
  BINARY_DIR ${FREETYPE_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
  ${FREETYPE_SB_CONFIG}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

set(_SB_FREETYPE_INCLUDE_DIRS ${SB_INSTALL_PREFIX}/include/freetype2)
if(WIN32)
  set(_SB_FREETYPE_LIBRARY ${SB_INSTALL_PREFIX}/lib/freetype.lib)
elseif(UNIX)
  set(_SB_FREETYPE_LIBRARY ${SB_INSTALL_PREFIX}/lib/libfreetype${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()
