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

INCLUDE_ONCE_MACRO(ZLIB)

SETUP_SUPERBUILD(ZLIB)


# Try official release 1.2.8
ExternalProject_Add(ZLIB
  PREFIX ZLIB
  URL "http://downloads.sourceforge.net/project/libpng/zlib/1.2.8/zlib-1.2.8.tar.gz"
  URL_MD5 44d667c142d7cda120332623eab69f40
  BINARY_DIR ${ZLIB_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CMAKE_CACHE_ARGS ${SB_CMAKE_CACHE_ARGS}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

#patch zlib cmake to disable static build on request
if(UNIX)
  ExternalProject_Add_Step(ZLIB remove_static
    COMMAND ${CMAKE_COMMAND} -E remove ${SB_INSTALL_PREFIX}/lib/libz.a
    DEPENDEES install)
endif()

#check who uses zdll.lib and remove this hack
if(MSVC)
  ExternalProject_Add_Step(ZLIB msvc_copy_hell
    COMMAND ${CMAKE_COMMAND} -E copy ${SB_INSTALL_PREFIX}/lib/zlib.lib ${SB_INSTALL_PREFIX}/lib/zdll.lib
    DEPENDEES install)
endif()

SUPERBUILD_PATCH_SOURCE(ZLIB)

set(_SB_ZLIB_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
if(WIN32)
  set(_SB_ZLIB_LIBRARY ${SB_INSTALL_PREFIX}/lib/zdll.lib)
elseif(UNIX)
  set(_SB_ZLIB_LIBRARY ${SB_INSTALL_PREFIX}/lib/libz${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()
