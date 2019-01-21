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

INCLUDE_ONCE_MACRO(FFTW)

SETUP_SUPERBUILD(FFTW)

set(FFTWF_SB_SRC ${CMAKE_BINARY_DIR}/FFTW/src/FFTWF)
set(FFTWD_SB_SRC ${CMAKE_BINARY_DIR}/FFTW/src/FFTWD)
set(FFTWF_SB_BUILD ${CMAKE_BINARY_DIR}/FFTW/build/FFTWF)
set(FFTWD_SB_BUILD ${CMAKE_BINARY_DIR}/FFTW/build/FFTWD)

if(WIN32)
  set(FFTW_SB_CONFIG -DWITH_COMBINED_THREADS:BOOL=ON)
else()
  set(FFTW_SB_CONFIG -DWITH_COMBINED_THREADS:BOOL=OFF)
endif()

# Compile the float version of FFTW
ExternalProject_Add(FFTWF
  PREFIX FFTW/FFTWF
  URL "http://www.fftw.org/fftw-3.3.8.tar.gz"
  URL_MD5 8aac833c943d8e90d51b697b27d4384d
  SOURCE_DIR ${FFTWF_SB_SRC}
  BINARY_DIR ${FFTWF_SB_BUILD}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
    -DCMAKE_INSTALL_LIBDIR:PATH=lib
    -DBUILD_TESTS:BOOL=OFF
    -DDISABLE_FORTRAN:BOOL=ON
    -DENABLE_THREADS:BOOL=ON
    -DENABLE_SSE2:BOOL=ON
    -DENABLE_FLOAT:BOOL=ON
    ${FFTW_SB_CONFIG}
  DEPENDS ${FFTW_DEPENDENCIES}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

# Compile the double version of FFTW
ExternalProject_Add(FFTWD
  PREFIX FFTW/FFTWD
  URL "http://www.fftw.org/fftw-3.3.8.tar.gz"
  URL_MD5 8aac833c943d8e90d51b697b27d4384d
  SOURCE_DIR ${FFTWD_SB_SRC}
  BINARY_DIR ${FFTWD_SB_BUILD}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
    -DCMAKE_INSTALL_LIBDIR:PATH=lib
    -DBUILD_TESTS:BOOL=OFF
    -DDISABLE_FORTRAN:BOOL=ON
    -DENABLE_THREADS:BOOL=ON
    -DENABLE_SSE2:BOOL=ON
    -DENABLE_FLOAT:BOOL=OFF
    ${FFTW_SB_CONFIG}
  DEPENDS FFTWF
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

ExternalProject_Add(FFTW
  PREFIX FFTW
  DOWNLOAD_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  DEPENDS FFTWD
  )

set(_SB_FFTW_INCLUDE_PATH ${SB_INSTALL_PREFIX}/include)
