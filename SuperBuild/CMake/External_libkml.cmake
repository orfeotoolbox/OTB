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

INCLUDE_ONCE_MACRO(LIBKML)

SETUP_SUPERBUILD(LIBKML)

# declare dependencies
##set(LIBKML_DEPENDENCIES EXPAT ZLIB BOOST)
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(LIBKML EXPAT ZLIB BOOST)

ADD_SUPERBUILD_CMAKE_VAR(LIBKML EXPAT_INCLUDE_DIR)
ADD_SUPERBUILD_CMAKE_VAR(LIBKML EXPAT_LIBRARY)
ADD_SUPERBUILD_CMAKE_VAR(LIBKML ZLIB_INCLUDE_DIR)
ADD_SUPERBUILD_CMAKE_VAR(LIBKML ZLIB_LIBRARY)
ADD_SUPERBUILD_CMAKE_VAR(LIBKML Boost_INCLUDE_DIR)

ExternalProject_Add(LIBKML
  PREFIX LIBKML
  URL "http://ftp.de.debian.org/debian/pool/main/libk/libkml/libkml_1.3.0~r864+dfsg.orig.tar.gz"
  URL_MD5 487e3c8dd1ad03551e361ab6a1b4e083
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

set(_SB_LIBKML_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
if(WIN32)
  list(APPEND _SB_LIBKML_BASE_LIBRARY ${SB_INSTALL_PREFIX}/lib/kmlbase.lib)
  if(USE_SYSTEM_EXPAT)
    list(APPEND _SB_LIBKML_BASE_LIBRARY ${EXPAT_LIBRARY})
  else()
    list(APPEND _SB_LIBKML_BASE_LIBRARY ${_SB_EXPAT_LIBRARY})
  endif()
  list(APPEND _SB_LIBKML_BASE_LIBRARY ${SB_INSTALL_PREFIX}/lib/uriparser.lib)

  set(_SB_LIBKML_CONVENIENCE_LIBRARY ${SB_INSTALL_PREFIX}/lib/kmlconvenience.lib)
  set(_SB_LIBKML_DOM_LIBRARY ${SB_INSTALL_PREFIX}/lib/kmldom.lib)
  set(_SB_LIBKML_ENGINE_LIBRARY ${SB_INSTALL_PREFIX}/lib/kmlengine.lib)
  set(_SB_LIBKML_REGIONATOR_LIBRARY ${SB_INSTALL_PREFIX}/lib/kmlregionator.lib)
  set(_SB_LIBKML_XSD_LIBRARY ${SB_INSTALL_PREFIX}/lib/kmlxsd.lib)
  set(_SB_LIBKML_MINIZIP_LIBRARY ${SB_INSTALL_PREFIX}/lib/minizip.lib)
elseif(UNIX)
  set(_SB_LIBKML_BASE_LIBRARY ${SB_INSTALL_PREFIX}/lib/libkmlbase${CMAKE_SHARED_LIBRARY_SUFFIX})
  set(_SB_LIBKML_CONVENIENCE_LIBRARY ${SB_INSTALL_PREFIX}/lib/libkmlconvenience${CMAKE_SHARED_LIBRARY_SUFFIX})
  set(_SB_LIBKML_DOM_LIBRARY ${SB_INSTALL_PREFIX}/lib/libkmldom${CMAKE_SHARED_LIBRARY_SUFFIX})
  set(_SB_LIBKML_ENGINE_LIBRARY ${SB_INSTALL_PREFIX}/lib/libkmlengine${CMAKE_SHARED_LIBRARY_SUFFIX})
  set(_SB_LIBKML_REGIONATOR_LIBRARY ${SB_INSTALL_PREFIX}/lib/libkmlregionator${CMAKE_SHARED_LIBRARY_SUFFIX})
  set(_SB_LIBKML_XSD_LIBRARY ${SB_INSTALL_PREFIX}/lib/libkmlxsd${CMAKE_SHARED_LIBRARY_SUFFIX})
  set(_SB_LIBKML_MINIZIP_LIBRARY ${SB_INSTALL_PREFIX}/lib/libminizip${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()
