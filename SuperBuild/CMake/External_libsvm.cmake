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

INCLUDE_ONCE_MACRO(LIBSVM)

SETUP_SUPERBUILD(LIBSVM)

ExternalProject_Add(LIBSVM
  PREFIX LIBSVM
  URL "https://github.com/cjlin1/libsvm/archive/v322.tar.gz"
  URL_MD5 d9617d29efad013573f63ca9a517f490
  BINARY_DIR ${LIBSVM_SB_BUILD_DIR}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  CMAKE_CACHE_ARGS ${SB_CMAKE_CACHE_ARGS}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  DEPENDS ${LIBSVM_DEPENDENCIES}
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/patches/LIBSVM/CMakeLists.txt
  ${LIBSVM_SB_SRC}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

set(_SB_LIBSVM_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
if(WIN32)
  set(_SB_LIBSVM_LIBRARY ${SB_INSTALL_PREFIX}/lib/svm.lib)
elseif(UNIX)
  set(_SB_LIBSVM_LIBRARY ${SB_INSTALL_PREFIX}/lib/libsvm${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()
