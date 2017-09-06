#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

if(MSVC)
  # TODO : get precompiled binaries as not all MSVC versions can compile FFTW
  message(STATUS "  FFTW SuperBuild is not available yet...")
  add_custom_target(FFTW)
  return()
endif()



# Compile the float version of FFTW
ExternalProject_Add(FFTWF
  PREFIX FFTW/FFTWF
  URL "http://www.fftw.org/fftw-3.3.4.tar.gz"
  URL_MD5 2edab8c06b24feeb3b82bbb3ebf3e7b3
  SOURCE_DIR ${FFTWF_SB_SRC}
  BINARY_DIR ${FFTWF_SB_SRC}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CONFIGURE_COMMAND
  ${SB_ENV_CONFIGURE_CMD}
  ${FFTWF_SB_SRC}/configure
  --prefix=${SB_INSTALL_PREFIX}
  --enable-shared
  --disable-static
  --enable-float
  --enable-threads
  --disable-fortran
  --disable-dependency-tracking
  DEPENDS ${FFTW_DEPENDENCIES}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

SUPERBUILD_PATCH_SOURCE(FFTWF)

# Compile the double version of FFTW
ExternalProject_Add(FFTWD
  PREFIX FFTW/FFTWD
  URL "http://www.fftw.org/fftw-3.3.4.tar.gz"
  URL_MD5 2edab8c06b24feeb3b82bbb3ebf3e7b3
  SOURCE_DIR ${FFTWD_SB_SRC}
  BINARY_DIR ${FFTWD_SB_SRC}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CONFIGURE_COMMAND
  ${SB_ENV_CONFIGURE_CMD}
  ${FFTWD_SB_SRC}/configure
  --prefix=${SB_INSTALL_PREFIX}
  --enable-shared
  --disable-static
  --disable-float
  --enable-threads
  --disable-fortran
  --disable-dependency-tracking
  DEPENDS FFTWF
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )


SUPERBUILD_PATCH_SOURCE(FFTWD)

ExternalProject_Add(FFTW
  PREFIX FFTW
  DOWNLOAD_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  DEPENDS FFTWD
  )

set(_SB_FFTW_INCLUDE_PATH ${SB_INSTALL_PREFIX}/include)
