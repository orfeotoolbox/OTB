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

INCLUDE_ONCE_MACRO(MUPARSER)

SETUP_SUPERBUILD(MUPARSER)

ExternalProject_Add(MUPARSER
  PREFIX MUPARSER
  URL "https://github.com/beltoforion/muparser/archive/v2.3.2.tar.gz"
  URL_MD5 cbc1b284e03abc7081b3c30997959893
  BINARY_DIR ${MUPARSER_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CMAKE_CACHE_ARGS 
  ${SB_CMAKE_CACHE_ARGS}
  -DENABLE_OPENMP:BOOL=FALSE
  -DENABLE_SAMPLES:BOOL=FALSE
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  DEPENDS ${MUPARSER_DEPENDENCIES}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

SUPERBUILD_PATCH_SOURCE(MUPARSER)
