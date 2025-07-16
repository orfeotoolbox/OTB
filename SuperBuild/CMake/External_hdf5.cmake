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

INCLUDE_ONCE_MACRO(HDF5)

SETUP_SUPERBUILD(HDF5)

# declare dependencies
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(HDF5 ZLIB)

set(HDF5_SB_CONFIG)

# When building HDF4 in debug, the lib names are suffixed with "_debug.so"
# and with this name, NETCDF can not find it. Thus compil this lib
# in release
set(__custom_sb_cmake_args ${SB_CMAKE_CACHE_ARGS})
list(REMOVE_ITEM __custom_sb_cmake_args "-DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}")
list(APPEND __custom_sb_cmake_args "-DCMAKE_BUILD_TYPE:STRING=Release")

ExternalProject_Add(HDF5
  PREFIX HDF5
  URL "https://github.com/HDFGroup/hdf5/releases/download/hdf5_1.14.6/hdf5-1.14.6.tar.gz"
  URL_MD5 63426c8e24086634eaf9179a8c5fe9e5
  SOURCE_DIR ${HDF5_SB_SRC}
  BINARY_DIR ${HDF5_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  DEPENDS ${HDF5_DEPENDENCIES}
  CMAKE_CACHE_ARGS
  ${__custom_sb_cmake_args}
    -DBUILD_TESTING:BOOL=OFF
    -DHDF5_BUILD_CPP_LIB:BOOL=ON
    -DHDF5_BUILD_EXAMPLES:BOOL=OFF
    -DHDF5_BUILD_FORTRAN:BOOL=OFF
    -DHDF5_BUILD_HL_LIB:BOOL=ON
    -DHDF5_BUILD_JAVA:BOOL=OFF
    -DHDF5_BUILD_TOOLS:BOOL=OFF
    -DHDF5_ENABLE_SZIP_SUPPORT:BOOL=OFF
    -DHDF5_ENABLE_Z_LIB_SUPPORT:BOOL=ON
    –DH5_USE_110_API:BOOL=ON
    -DDEFAULT_API_VERSION:STRING=v110
    ${HDF5_SB_CONFIG}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

# this should not be needed but see Mantis-1457
if(UNIX)
  set(_SB_HDF5_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  set(_SB_HDF5_C_LIBRARY ${SB_INSTALL_PREFIX}/lib/libhdf5${CMAKE_SHARED_LIBRARY_SUFFIX})
  set(_SB_HDF5_HL_LIBRARY ${SB_INSTALL_PREFIX}/lib/libhdf5_hl${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()
