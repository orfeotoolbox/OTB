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

INCLUDE_ONCE_MACRO(GEOTIFF)

SETUP_SUPERBUILD(GEOTIFF)

# declare dependencies
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(GEOTIFF TIFF PROJ JPEG ZLIB)

ExternalProject_Add(GEOTIFF
  PREFIX GEOTIFF
  URL "http://download.osgeo.org/geotiff/libgeotiff/libgeotiff-1.4.2.tar.gz"
  URL_MD5 96ab80e0d4eff7820579957245d844f8
  SOURCE_DIR ${GEOTIFF_SB_SRC}
  BINARY_DIR ${GEOTIFF_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  DEPENDS ${GEOTIFF_DEPENDENCIES}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
  -DBUILD_TESTING:BOOL=OFF
  -DPROJ4_OSGEO4W_HOME:PATH=${SB_INSTALL_PREFIX}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

SUPERBUILD_PATCH_SOURCE(GEOTIFF)

