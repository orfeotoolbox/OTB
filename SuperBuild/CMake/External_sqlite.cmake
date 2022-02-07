#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

INCLUDE_ONCE_MACRO(SQLITE)

SETUP_SUPERBUILD(SQLITE)

if(WIN32)
  set(SQLITE_URL "https://www.sqlite.org/2015/sqlite-amalgamation-3080801.zip")
  set(SQLITE_MD5 b1cbcbd710bdfd762dc169f1676053b5)
else()
  set(SQLITE_URL "https://www.sqlite.org/2021/sqlite-amalgamation-3360000.zip")
  set(SQLITE_MD5 c5d360c74111bafae1b704721ff18fe6)
endif()

ExternalProject_Add(SQLITE
  PREFIX SQLITE
  URL ${SQLITE_URL}
  URL_MD5 ${SQLITE_MD5}
  SOURCE_DIR ${SQLITE_SB_SRC}
  BINARY_DIR ${SQLITE_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/patches/SQLITE ${SQLITE_SB_SRC}
  CMAKE_CACHE_ARGS ${SB_CMAKE_CACHE_ARGS}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

set(_SB_SQLITE_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
if(WIN32)
  set(_SB_SQLITE_LIBRARY ${SB_INSTALL_PREFIX}/lib/sqlite3.lib)
elseif(UNIX)
  set(_SB_SQLITE_LIBRARY ${SB_INSTALL_PREFIX}/lib/libsqlite3${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()
