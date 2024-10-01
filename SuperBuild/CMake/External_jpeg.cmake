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

INCLUDE_ONCE_MACRO(JPEG)

SETUP_SUPERBUILD(JPEG)

# overwrite CMAKE_INSTALL_LIBDIR since when compiling with centos, it will be
# installed in lib64 if left unchanged
set(JPEG_CONFIGURE_COMMAND "${SB_CMAKE_COMMAND}"
  ${SB_CMAKE_ARGS}
  ${SB_CMAKE_CACHE_ARGS}
  -DCMAKE_INSTALL_LIBDIR:PATH=lib
  -DENABLE_SHARED=TRUE
  -DENABLE_STATIC=FALSE
  -DWITH_SIMD=FALSE
  -DWITH_TURBOJPEG=FALSE
  -DWITH_ARITH_DEC=TRUE
  -DWITH_JAVA=FALSE
  ${JPEG_SB_SRC} )

ExternalProject_Add(JPEG
  PREFIX JPEG
  URL "https://github.com/libjpeg-turbo/libjpeg-turbo/releases/download/3.0.3/libjpeg-turbo-3.0.3.tar.gz"
  URL_MD5 fdeae6d78ca577a37857f2d14e33adbe
  SOURCE_DIR ${JPEG_SB_SRC}
  BINARY_DIR ${JPEG_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CONFIGURE_COMMAND ${JPEG_CONFIGURE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

SUPERBUILD_PATCH_SOURCE(JPEG)

SUPERBUILD_UPDATE_CMAKE_VARIABLES(JPEG FALSE)

set(_SB_JPEGLIB_H_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
if(WIN32)
  set(_SB_JPEG_LIB ${SB_INSTALL_PREFIX}/lib/jpeg_i.lib)
elseif(UNIX)
  set(_SB_JPEG_LIB ${SB_INSTALL_PREFIX}/lib/libjpeg${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()
