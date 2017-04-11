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

INCLUDE_ONCE_MACRO(GEOS)

SETUP_SUPERBUILD(GEOS)

if(MSVC)
  set(GEOS_PATCH_COMMAND ${CMAKE_COMMAND} -E copy
	  ${CMAKE_SOURCE_DIR}/patches/GEOS/CMakeLists.txt
	  ${CMAKE_SOURCE_DIR}/patches/GEOS/nmake.opt
	  ${GEOS_SB_SRC})
  set(GEOS_CMAKE_COMMAND ${SB_CMAKE_COMMAND} -DGEOS_SB_SRC=${GEOS_SB_SRC})

else()
  set(GEOS_PATCH_COMMAND)
  set(GEOS_CMAKE_COMMAND ${SB_CMAKE_COMMAND})

endif()

ExternalProject_Add(GEOS
   PREFIX GEOS
   URL "http://download.osgeo.org/geos/geos-3.5.0.tar.bz2"
   URL_MD5 136842690be7f504fba46b3c539438dd
   SOURCE_DIR ${GEOS_SB_SRC}
   BINARY_DIR ${GEOS_SB_SRC}
   INSTALL_DIR ${SB_INSTALL_PREFIX}
   DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
   CMAKE_CACHE_ARGS
   ${SB_CMAKE_CACHE_ARGS}
   -DGEOS_ENABLE_TESTS:BOOL=OFF
   PATCH_COMMAND ${GEOS_PATCH_COMMAND}
   CMAKE_COMMAND ${GEOS_CMAKE_COMMAND}
   )

if(NOT MSVC)
  SUPERBUILD_PATCH_SOURCE(GEOS)
endif()

set(_SB_GEOS_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
if(WIN32)
  set(_SB_GEOS_LIBRARY ${SB_INSTALL_PREFIX}/lib/geos_i.lib )
elseif(UNIX)
  set(_SB_GEOS_LIBRARY ${SB_INSTALL_PREFIX}/lib/libgeos${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()