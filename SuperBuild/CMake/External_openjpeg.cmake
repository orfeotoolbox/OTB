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

INCLUDE_ONCE_MACRO(OPENJPEG)

SETUP_SUPERBUILD(OPENJPEG)

# declare dependencies
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(OPENJPEG ZLIB TIFF PNG)

#GIT_REPOSITORY "https://github.com/uclouvain/openjpeg.git"
#GIT_TAG d0babeb6f6cdd1887308137df37bb2b4724a6592

ExternalProject_Add(OPENJPEG
  PREFIX OPENJPEG
  URL "https://github.com/uclouvain/openjpeg/archive/v2.2.0.tar.gz"
  URL_MD5 269bb0b175476f3addcc0d03bd9a97b6
  BINARY_DIR ${OPENJPEG_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
  -DBUILD_CODEC:BOOL=ON
  -DBUILD_DOC:BOOL=OFF
  -DBUILD_JPIP:BOOL=OFF
  -DBUILD_JPWL:BOOL=OFF
  -DBUILD_MJ2:BOOL=OFF
  -DBUILD_PKGCONFIG_FILES:BOOL=ON
  -DBUILD_THIRDPARTY:BOOL=OFF
  -DBUILD_THIRDPARTY_LCMS:BOOL=ON
  ${OPENJPEG_SB_CONFIG}
  DEPENDS ${OPENJPEG_DEPENDENCIES}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

#Apply patches to openjpeg (for now Even Rouault optimizations)
SUPERBUILD_PATCH_SOURCE(OPENJPEG "-ut")
