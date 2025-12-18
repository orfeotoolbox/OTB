#
# Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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
set(__LIBSVM_VERSION "336")
set(__LIBSVM_DL_NAME "libsvm-v${__LIBSVM_VERSION}.tar.gz")

ExternalProject_Add(LIBSVM
  PREFIX LIBSVM
  URL "https://github.com/cjlin1/libsvm/archive/refs/tags/v${__LIBSVM_VERSION}.tar.gz"
  URL_MD5 a93de5a77a453c1ea1b99269473eb71e
  # change download name as the file notation vx.x.x is used everywhere
  # and can be in conflict with other package
  DOWNLOAD_NAME ${__LIBSVM_DL_NAME}
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
