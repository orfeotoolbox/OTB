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

INCLUDE_ONCE_MACRO(LERC)

SETUP_SUPERBUILD(LERC)

ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(LERC CURL)

set(__LERC_VERSION "4.0.0")
set(__LERC_URL "https://github.com/Esri/lerc/archive/refs/tags/v${__LERC_VERSION}.tar.gz")
set(__LERC_MD5 f5b00e53bf507aba13ca3de02726e2ba)

set(LERC_CONFIG_LIBS ""
)

set(__LERC_DL_NAME "lerc-${__LERC_VERSION}.tar.gz")

ExternalProject_Add(LERC
   PREFIX LERC
   DEPENDS ${LERC_DEPENDENCIES}
   URL ${__LERC_URL}
   URL_MD5 ${__LERC_MD5}
   # change download name as the file notation vx.x.x is used everywhere
   # and can be in conflict with other package
   DOWNLOAD_NAME ${__LERC_DL_NAME}
   SOURCE_DIR ${LERC_SB_SRC}
   BINARY_DIR ${LERC_SB_BUILD_DIR}
   INSTALL_DIR ${SB_INSTALL_PREFIX}
   DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
   CMAKE_CACHE_ARGS
   ${SB_CMAKE_CACHE_ARGS}
   ${LERC_CONFIG_LIBS}
   -DCMAKE_INSTALL_LIBDIR:STRING=lib
   CMAKE_COMMAND ${SB_CMAKE_COMMAND}
   LOG_DOWNLOAD 1
   LOG_CONFIGURE 1
   LOG_BUILD 1
   LOG_INSTALL 1
   )

