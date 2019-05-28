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

INCLUDE_ONCE_MACRO(GEOS)

SETUP_SUPERBUILD(GEOS)

ExternalProject_Add(GEOS
   PREFIX GEOS
   URL "http://download.osgeo.org/geos/geos-3.6.1.tar.bz2"
   URL_MD5 c97e338b3bc81f9848656e9d693ca6cc
   SOURCE_DIR ${GEOS_SB_SRC}
   BINARY_DIR ${GEOS_SB_SRC}
   INSTALL_DIR ${SB_INSTALL_PREFIX}
   DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
   CMAKE_CACHE_ARGS
   ${SB_CMAKE_CACHE_ARGS}
   -DGEOS_ENABLE_TESTS:BOOL=OFF
   -DGEOS_ENABLE_MACOSX_FRAMEWORK:BOOL=OFF
   -DGEOS_BUILD_STATIC:BOOL=${BUILD_STATIC_LIBS}
   -DGEOS_BUILD_SHARED:BOOL=${BUILD_SHARED_LIBS}
   CMAKE_COMMAND ${SB_CMAKE_COMMAND}
   LOG_DOWNLOAD 1
   LOG_CONFIGURE 1
   LOG_BUILD 1
   LOG_INSTALL 1
   )

# Patch still needed with version 3.6.1 to avoid error during CMake configuration
# See https://trac.osgeo.org/geos/ticket/753
SUPERBUILD_PATCH_SOURCE(GEOS)


