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

INCLUDE_ONCE_MACRO(TIFF)

SETUP_SUPERBUILD(TIFF)

# declare dependencies
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(TIFF ZLIB JPEG)

if(MSVC)
  set(TIFF_C_FLAGS "/D_CRT_SECURE_NO_WARNINGS /DWIN32")
endif()

ExternalProject_Add(TIFF
  PREFIX TIFF
  URL "http://download.osgeo.org/libtiff/tiff-4.0.8.tar.gz"
  URL_MD5 2a7d1c1318416ddf36d5f6fa4600069b
  SOURCE_DIR ${TIFF_SB_SRC}
  BINARY_DIR ${TIFF_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  DEPENDS ${TIFF_DEPENDENCIES}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
  -DCMAKE_C_FLAGS:STRING=${TIFF_C_FLAGS}
  -DCMAKE_INSTALL_LIBDIR:PATH=lib
  -DCMAKE_INSTALL_BINDIR:PATH=bin
  -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS:BOOL=TRUE
  -DBUILD_TESTING:BOOL=FALSE
  -Djpeg:BOOL=TRUE
  -Dlzma:BOOL=FALSE
  -Djbig:BOOL=FALSE
  -Dzlib:BOOL=TRUE
  -DWITH_OPENGL:BOOL=FALSE
  -Dpixarlog:BOOL=TRUE
  -Dcxx:BOOL=FALSE
  -Dwith_opengl:BOOL=FALSE
  -Dwith_tools:BOOL=FALSE
  -Dwith_test:BOOL=FALSE
  -Dwith_contrib:BOOL=FALSE
  -Dwith_docs:BOOL=FALSE
  ${TIFF_SB_CONFIG}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )
    
SUPERBUILD_PATCH_SOURCE(TIFF)

#do we really need these variables? 
set(_SB_TIFF_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
if(WIN32)
  set(_SB_TIFF_LIBRARY ${SB_INSTALL_PREFIX}/lib/tiff.lib)
elseif(UNIX)
  set(_SB_TIFF_LIBRARY ${SB_INSTALL_PREFIX}/lib/libtiff${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()
