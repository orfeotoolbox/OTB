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

INCLUDE_ONCE_MACRO(GLEW)

SETUP_SUPERBUILD(GLEW)

ExternalProject_Add(GLEW
  PREFIX GLEW
  URL "https://downloads.sourceforge.net/project/glew/glew/1.13.0/glew-1.13.0.tgz"
  URL_MD5 7cbada3166d2aadfc4169c4283701066
  SOURCE_DIR ${GLEW_SB_SRC}
  BINARY_DIR ${GLEW_SB_BUILD_DIR}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  CONFIGURE_COMMAND ${SB_CMAKE_COMMAND} ${SB_CMAKE_ARGS} ${SB_CMAKE_CACHE_ARGS} -DBUILD_UTILS:BOOL=OFF ${GLEW_SB_SRC}/build/cmake/
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

SUPERBUILD_PATCH_SOURCE(GLEW)

set(_SB_GLEW_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
if(WIN32)
  set(_SB_GLEW_LIBRARY ${SB_INSTALL_PREFIX}/lib/glew32.lib)
elseif(UNIX)
  set(_SB_GLEW_LIBRARY ${SB_INSTALL_PREFIX}/lib/libGLEW${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()
