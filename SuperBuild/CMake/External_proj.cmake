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

# declare dependencies
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(PROJ SQLITE)

ExternalProject_Add(PROJ
  DEPENDS ${PROJ_DEPENDENCIES}
  PREFIX PROJ
  URL "http://download.osgeo.org/proj/proj-6.2.1.tar.gz"
  URL_MD5 9f874e227d221daf95f7858dc55dfa3e
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

set(_SB_PROJ_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
if(WIN32)
  set(_SB_PROJ_LIBRARY ${SB_INSTALL_PREFIX}/lib/proj.lib)
elseif(UNIX)
  set(_SB_PROJ_LIBRARY ${SB_INSTALL_PREFIX}/lib/libproj${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()
