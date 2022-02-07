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

INCLUDE_ONCE_MACRO(PROJ)

SETUP_SUPERBUILD(PROJ)

# declare dependencies
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(PROJ SQLITE TIFF CURL)

if(WIN32)
  set(PROJ_URL "https://download.osgeo.org/proj/proj-6.2.1.tar.gz")
  set(PROJ_MD5 9f874e227d221daf95f7858dc55dfa3e)
  set(PROJ_FLAGS -DBUILD_LIBPROJ_SHARED:BOOL=TRUE -DVERSIONED_OUTPUT:BOOL=FALSE -DPROJ_TESTS:BOOL=OFF)
else()
  set(PROJ_URL "https://download.osgeo.org/proj/proj-8.1.1.tar.gz")
  set(PROJ_MD5 f017fd7d35311b0d65b2cf0503844690)
  set(PROJ_FLAGS -DBUILD_SHARED_LIBS:BOOL=TRUE -DBUILD_TESTING:BOOL=OFF -DENABLE_TIFF:BOOL=ON -DENABLE_CURL:BOOL=ON)
endif()

ExternalProject_Add(PROJ
  DEPENDS ${PROJ_DEPENDENCIES}
  PREFIX PROJ
  URL ${PROJ_URL}
  URL_MD5 ${PROJ_MD5}
  BINARY_DIR ${PROJ_SB_SRC}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
  -DBUILD_FRAMEWORKS_AND_BUNDLE:BOOL=FALSE
  -DPROJ_LIB_SUBDIR:STRING=lib
  -DPROJ_INCLUDE_SUBDIR:STRING=include
  ${PROJ_FLAGS}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

SUPERBUILD_PATCH_SOURCE(PROJ)

set(_SB_PROJ_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
if(WIN32)
  set(_SB_PROJ_LIBRARY ${SB_INSTALL_PREFIX}/lib/proj.lib)
elseif(UNIX)
  set(_SB_PROJ_LIBRARY ${SB_INSTALL_PREFIX}/lib/libproj${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()
