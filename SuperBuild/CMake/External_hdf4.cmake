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

INCLUDE_ONCE_MACRO(HDF4)

SETUP_SUPERBUILD(HDF4)

# declare dependencies
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(HDF4 ZLIB JPEG)

set(HDF4_SB_CONFIG)

set(HDF4_SB_DF_LIB dfalt)
set(HDF4_SB_MF_LIB mfhdfalt)

ExternalProject_Add(HDF4
  PREFIX HDF4
  URL "http://support.hdfgroup.org/ftp/HDF/HDF_Current/src/hdf-4.2.13.tar.gz"
  URL_MD5 a6aa950b3fce5162b96496d8ea0b82bf
  SOURCE_DIR ${HDF4_SB_SRC}
  BINARY_DIR ${HDF4_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  DEPENDS ${HDF4_DEPENDENCIES}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
    -DBUILD_TESTING:BOOL=OFF
    -DHDF4_ALLOW_EXTERNAL_SUPPORT:BOOL=OFF
    -DHDF4_BUILD_EXAMPLES:BOOL=OFF
    -DHDF4_BUILD_JAVA:BOOL=OFF
    -DHDF4_BUILD_TOOLS:BOOL=OFF
    -DHDF4_BUILD_UTILS:BOOL=OFF
    -DHDF4_PACK_EXAMPLES:BOOL=OFF
    -DHDF4_BUILD_FORTRAN:BOOL=OFF
    -DHDF4_BUILD_XDR_LIB:BOOL=OFF
    -DHDF4_DISABLE_COMPILER_WARNINGS:BOOL=OFF
    -DHDF4_ENABLE_COVERAGE:BOOL=OFF
    -DHDF4_ENABLE_DEPRECATED_SYMBOLS:BOOL=ON
    -DHDF4_ENABLE_JPEG_LIB_SUPPORT:BOOL=ON
    -DHDF4_ENABLE_NETCDF:BOOL=OFF
    -DHDF4_ENABLE_PARALLEL:BOOL=OFF
    -DHDF4_ENABLE_SZIP_SUPPORT:BOOL=OFF
    -DHDF4_ENABLE_Z_LIB_SUPPORT:BOOL=ON
    -DHDF4_NO_PACKAGES:BOOL=ON
    -DHDF4_PACKAGE_EXTLIBS:BOOL=OFF
    -DHDF_ENABLE_LARGE_FILE:BOOL=ON
    -DHDF4_SRC_LIB_CORENAME:STRING=${HDF4_SB_DF_LIB}
    -DHDF4_MF_LIB_CORENAME:STRING=${HDF4_SB_MF_LIB}
    ${HDF4_SB_CONFIG}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

SUPERBUILD_PATCH_SOURCE(HDF4)
