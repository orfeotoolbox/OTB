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

INCLUDE_ONCE_MACRO(OPENTHREADS)

SETUP_SUPERBUILD(OPENTHREADS)

ExternalProject_Add(OPENTHREADS
  PREFIX OPENTHREADS
  URL "http://www.openscenegraph.org/downloads/stable_releases/OpenSceneGraph-3.4.0/source/OpenSceneGraph-3.4.0.zip"
  URL_MD5 a5e762c64373a46932e444f6f7332496
  SOURCE_DIR ${OPENTHREADS_SB_SRC}
  BINARY_DIR ${OPENTHREADS_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  DEPENDS ${OPENTHREADS_DEPENDENCIES}
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/patches/OPENTHREADS/CMakeLists.txt
  ${OPENTHREADS_SB_SRC}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

set(_SB_OPENTHREADS_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
if(WIN32)
  set(_SB_OPENTHREADS_LIBRARY ${SB_INSTALL_PREFIX}/lib/OpenThreads.lib)
elseif(UNIX)
  set(_SB_OPENTHREADS_LIBRARY ${SB_INSTALL_PREFIX}/lib/libOpenThreads${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()
