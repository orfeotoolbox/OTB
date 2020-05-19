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

INCLUDE_ONCE_MACRO(JPEG)

SETUP_SUPERBUILD(JPEG)

if(WIN32)
  set(JPEG_CONFIGURE_COMMAND "${SB_CMAKE_COMMAND}"
    ${SB_CMAKE_ARGS}
    ${SB_CMAKE_CACHE_ARGS}
    -DENABLE_SHARED=TRUE
    -DENABLE_STATIC=FALSE
    -DWITH_SIMD=FALSE
    -DWITH_TURBOJPEG=FALSE
    -DWITH_ARITH_DEC=TRUE
    -DWITH_JAVA=FALSE
    ${JPEG_SB_SRC} )

else()
  set(JPEG_CONFIGURE_COMMAND "${SB_ENV_CONFIGURE_CMD};${JPEG_SB_SRC}/configure"
    ${SB_CONFIGURE_ARGS}
    --with-arith-dec
    --without-simd
    --without-java
     )

endif()

ExternalProject_Add(JPEG
  PREFIX JPEG
  URL "http://downloads.sourceforge.net/project/libjpeg-turbo/1.4.1/libjpeg-turbo-1.4.1.tar.gz"
  URL_MD5 b1f6b84859a16b8ebdcda951fa07c3f2
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
