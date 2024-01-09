#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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
INCLUDE_ONCE_MACRO(PATCHELF)

SETUP_SUPERBUILD(PATCHELF)

ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(PATCHELF ZLIB)

ExternalProject_Add(PATCHELF
  PREFIX PATCHELF
  URL                 "https://github.com/NixOS/patchelf/releases/download/0.18.0/patchelf-0.18.0.tar.gz"
  URL_MD5             b02099b0d63f06b3fe370f4edfc0c085
  DOWNLOAD_DIR        ${DOWNLOAD_LOCATION}
  BINARY_DIR          ${PATCHELF_SB_BUILD_DIR}
  INSTALL_DIR         ${SB_INSTALL_PREFIX}
  DEPENDS             ${PATCHELF_DEPENDENCIES}
  CONFIGURE_COMMAND
  ${PATCHELF_SB_SRC}/configure --prefix=${SB_INSTALL_PREFIX}
  BUILD_COMMAND $(MAKE)
  INSTALL_COMMAND $(MAKE) install
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )