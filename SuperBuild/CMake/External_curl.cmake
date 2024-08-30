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

INCLUDE_ONCE_MACRO(CURL)

SETUP_SUPERBUILD(CURL)

# declare dependencies
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(CURL ZLIB)
if(NOT APPLE AND NOT WIN32)
  ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(CURL OPENSSL)
  ADD_SUPERBUILD_CMAKE_VAR(CURL OPENSSL_INCLUDE_DIR)
  ADD_SUPERBUILD_CMAKE_VAR(CURL OPENSSL_SSL_LIBRARY)
  ADD_SUPERBUILD_CMAKE_VAR(CURL OPENSSL_CRYPTO_LIBRARY)
  list(APPEND CURL_SB_CONFIG
    -DCMAKE_USE_OPENSSL:BOOL=ON
    -DCMAKE_C_FLAGS:STRING=-fPIC)
# Set CURL_CA_PATH to none because openssl in not enabled and CMake generation will fail if
# a CA path is found in auto mode.
elseif(WIN32)
  list(APPEND CURL_SB_CONFIG
    -DCMAKE_USE_WINSSL:BOOL=ON
    -DCURL_WINDOWS_SSPI:BOOL=ON
    -DHAVE_INET_PTON:STRING=0)
endif()

ExternalProject_Add(CURL
  PREFIX CURL
  URL "https://github.com/curl/curl/releases/download/curl-8_2_1/curl-8.2.1.tar.gz"
  URL_MD5 b25588a43556068be05e1624e0e74d41
  BINARY_DIR ${CURL_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
  -DBUILD_CURL_TESTS:BOOL=OFF
  -DBUILD_CURL_EXE:BOOL=ON
  -DBUILD_DASHBOARD_REPORTS:BOOL=OFF
  -DBUILD_RELEASE_DEBUG_DIRS:BOOL=OFF
  -DCMAKE_USE_GSSAPI:BOOL=OFF
  -DCMAKE_USE_LIBSSH2:BOOL=OFF
  -DCMAKE_USE_OPENLDAP:BOOL=OFF
  -DCMAKE_USE_MBEDTLS:BOOL=OFF
  -DCURL_DISABLE_COOKIES:BOOL=OFF
  -DCURL_DISABLE_CRYPTO_AUTH:BOOL=OFF
  -DCURL_DISABLE_DICT:BOOL=ON
  -DCURL_DISABLE_FILE:BOOL=OFF
  -DCURL_DISABLE_FTP:BOOL=OFF
  -DCURL_DISABLE_GOPHER:BOOL=ON
  -DCURL_DISABLE_HTTP:BOOL=OFF
  -DCURL_DISABLE_IMAP:BOOL=ON
  -DCURL_DISABLE_LDAP:BOOL=ON
  -DCURL_DISABLE_LDAPS:BOOL=ON
  -DCURL_DISABLE_POP3:BOOL=ON
  -DCURL_DISABLE_PROXY:BOOL=OFF
  -DCURL_DISABLE_RTSP:BOOL=ON
  -DCURL_DISABLE_SMTP:BOOL=ON
  -DCURL_DISABLE_TELNET:BOOL=ON
  -DCURL_DISABLE_TFTP:BOOL=ON
  -DCURL_DISABLE_VERBOSE_STRINGS:BOOL=OFF
  -DCURL_HIDDEN_SYMBOLS:BOOL=OFF
  -DCURL_STATICLIB:BOOL=OFF
  -DDISABLED_THREADSAFE:BOOL=OFF
  -DENABLE_ARES:BOOL=OFF
  -DENABLE_CURLDEBUG:BOOL=OFF
  -DENABLE_DEBUG:BOOL=OFF
  -DENABLE_IPV6:BOOL=OFF
  -DENABLE_UNIX_SOCKETS:BOOL=OFF
  -DENABLE_MANUAL:BOOL=OFF
  -DHTTP_ONLY:BOOL=OFF
  -DUSE_WIN32_LDAP:BOOL=OFF
  -DCMAKE_INSTALL_LIBDIR:STRING=lib
  ${CURL_SB_CONFIG}
  DEPENDS ${CURL_DEPENDENCIES}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

set(_SB_CURL_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
if(WIN32)
  set(_SB_CURL_LIBRARY "${SB_INSTALL_PREFIX}/lib/libcurl_imp.lib")
elseif(UNIX)
  set(_SB_CURL_LIBRARY ${SB_INSTALL_PREFIX}/lib/libcurl${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()
