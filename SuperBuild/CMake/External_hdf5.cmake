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

ExternalProject_Add(HDF5
  PREFIX HDF5
  URL "https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.12/hdf5-1.12.3/src/hdf5-1.12.3.tar.gz"
  URL_MD5 4da24fcd281b9eeb05dae9b258f72a72
  SOURCE_DIR ${HDF5_SB_SRC}
  BINARY_DIR ${HDF5_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  DEPENDS ${HDF5_DEPENDENCIES}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
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
