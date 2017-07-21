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

INCLUDE_ONCE_MACRO(CURL)

SETUP_SUPERBUILD(CURL)

# declare dependencies
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(CURL ZLIB)
if(NOT APPLE)
  ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(CURL OPENSSL)
endif()

ExternalProject_Add(CURL
  PREFIX CURL
  URL "http://curl.haxx.se/download/curl-7.40.0.tar.gz"
  URL_MD5 58943642ea0ed050ab0431ea1caf3a6f
  BINARY_DIR ${CURL_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
  -DBUILD_CURL_TESTS:BOOL=OFF
  -DBUILD_CURL_EXE:BOOL=ON
  -DCMAKE_USE_OPENSSL:BOOL=OFF
  -DCMAKE_USE_LIBSSH2:BOOL=OFF
  -DCURL_DISABLE_LDAP:BOOL=ON
  -DCMAKE_USE_OPENLDAP:BOOL=OFF
  -DENABLE_MANUAL:BOOL=OFF
  -DENABLE_IPV6:BOOL=OFF
  ${CURL_SB_CONFIG}
  DEPENDS ${CURL_DEPENDENCIES}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )
