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

INCLUDE_ONCE_MACRO(OPENSSL)
SETUP_SUPERBUILD(OPENSSL)

# declare dependencies
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(OPENSSL ZLIB)

if(WIN32)
  find_program(PERL_COMMAND NAMES perl perl.exe)
  if(NOT PERL_COMMAND)
    message(FATAL_ERROR "PERL_COMMAND not set. it is needed to configure openssl!")
    return()
  endif()

  set(OPENSSL_BUILD_ARCH "linux-x32")
  set(OPENSSL_BUILD_ARCH "VC-WIN32")
  set(OPENSSL_CONFIGURE_CMD "${OPENSSL_SB_SRC}/ms/do_ms.bat")
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(OPENSSL_BUILD_ARCH "linux-x86_64")
    set(OPENSSL_BUILD_ARCH "VC-WIN64A")
    set(OPENSSL_CONFIGURE_CMD "${OPENSSL_SB_SRC}/ms/do_win64a.bat")
  endif()
endif()

if(MSVC)
  ExternalProject_Add(OPENSSL
    PREFIX OPENSSL
    URL "https://github.com/openssl/openssl/archive/OpenSSL_1_0_1p.tar.gz"
    URL_MD5 6bc1f9a9d9d474aceceb377e758e48ec
    DEPENDS ${OPENSSL_DEPENDENCIES}
    BINARY_DIR ${OPENSSL_SB_SRC}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    PATCH_COMMAND ${PERL_COMMAND} ${OPENSSL_SB_SRC}/Configure
    ${OPENSSL_BUILD_ARCH}
    no-asm
    "--prefix=${SB_INSTALL_PREFIX_NATIVE}"
    "--openssldir=${SB_INSTALL_PREFIX_NATIVE}"
    CONFIGURE_COMMAND ${OPENSSL_CONFIGURE_CMD}
    BUILD_COMMAND nmake -f "${OPENSSL_SB_SRC}/ms/ntdll.mak"
    INSTALL_COMMAND nmake -f "${OPENSSL_SB_SRC}/ms/ntdll.mak" install
    LOG_DOWNLOAD 1
    LOG_CONFIGURE 1
    LOG_BUILD 1
    LOG_INSTALL 1
    )

else(UNIX)
  ExternalProject_Add(OPENSSL
    PREFIX OPENSSL
    DEPENDS ${OPENSSL_DEPENDENCIES}
    URL "https://github.com/openssl/openssl/archive/OpenSSL_1_0_1p.tar.gz"
    URL_MD5 6bc1f9a9d9d474aceceb377e758e48ec
    BINARY_DIR ${OPENSSL_SB_SRC}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    CONFIGURE_COMMAND
    ${SB_ENV_CONFIGURE_CMD}
    ${OPENSSL_SB_SRC}/config ${OPENSSL_BUILD_ARCH}
    "--prefix=${SB_INSTALL_PREFIX}"
    "--libdir=lib"
    shared
    zlib
    zlib-dynamic
    "-I${SB_INSTALL_PREFIX}/include"
    "-L${SB_INSTALL_PREFIX}/lib"
    BUILD_COMMAND $(MAKE)
    INSTALL_COMMAND $(MAKE) install
    LOG_DOWNLOAD 1
    LOG_CONFIGURE 1
    LOG_BUILD 1
    LOG_INSTALL 1
    )

  ExternalProject_Add_Step(OPENSSL remove_static
    COMMAND ${CMAKE_COMMAND} -E remove
    ${SB_INSTALL_PREFIX}/lib/libssl.a
    ${SB_INSTALL_PREFIX}/lib/libcrypto.a
    ${SB_INSTALL_PREFIX}/bin/openssl
    ${SB_INSTALL_PREFIX}/bin/c_rehash
    DEPENDEES install)

endif()

set(_SB_OPENSSL_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
if(WIN32)
  set(_SB_OPENSSL_LIBRARY "${SB_INSTALL_PREFIX}/lib/ssleay32.lib;${SB_INSTALL_PREFIX}/lib/libeay32.lib")
elseif(UNIX)
  set(_SB_OPENSSL_SSL_LIBRARY ${SB_INSTALL_PREFIX}/lib/libssl${CMAKE_SHARED_LIBRARY_SUFFIX})
  set(_SB_OPENSSL_CRYPTO_LIBRARY ${SB_INSTALL_PREFIX}/lib/libcrypto${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()
