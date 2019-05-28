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

INCLUDE_ONCE_MACRO(LIBKML)

SETUP_SUPERBUILD(LIBKML)

# declare dependencies
##set(LIBKML_DEPENDENCIES EXPAT ZLIB BOOST)
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(LIBKML EXPAT ZLIB BOOST)

ExternalProject_Add(LIBKML
  PREFIX LIBKML
  URL "http://ftp.de.debian.org/debian/pool/main/libk/libkml/libkml_1.3.0~r863.orig.tar.gz"
  URL_MD5 211ed5fdf2dd45aeb9c0abc8e1fe42be
  BINARY_DIR ${LIBKML_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  DEPENDS ${LIBKML_DEPENDENCIES}
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy_directory  ${CMAKE_SOURCE_DIR}/patches/LIBKML ${LIBKML_SB_SRC}
  CMAKE_CACHE_ARGS ${SB_CMAKE_CACHE_ARGS}
  ${LIBKML_SB_CONFIG}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

