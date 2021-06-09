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

INCLUDE_ONCE_MACRO(GLUT)

if(APPLE)
  message(FATAL_ERROR "Cannot activate GLUT on OSX. See manits issue #1194")
  return()
endif()

SETUP_SUPERBUILD(GLUT)

if(UNIX)

  set(GLUT_PATCH_COMMAND)

  set(GLUT_CONFIGURE_COMMAND  "${SB_ENV_CONFIGURE_CMD};${GLUT_SB_SRC}/configure" ${SB_CONFIGURE_ARGS})

else(MSVC)

  set(GLUT_PATCH_COMMAND ${CMAKE_COMMAND}
  -E copy
  ${CMAKE_SOURCE_DIR}/patches/GLUT/CMakeLists.txt
  ${GLUT_SB_SRC}
  )

  set(GLUT_CONFIGURE_COMMAND ${SB_CMAKE_COMMAND} ${SB_CMAKE_ARGS} ${SB_CMAKE_CACHE_ARGS} ${GLUT_SB_SRC} )
endif()

ExternalProject_Add(GLUT
  PREFIX GLUT
  URL "https://downloads.sourceforge.net/project/freeglut/freeglut/2.8.1/freeglut-2.8.1.tar.gz"
  URL_MD5 918ffbddcffbac83c218bc52355b6d5a
  BINARY_DIR ${GLUT_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  PATCH_COMMAND ${GLUT_PATCH_COMMAND}
  CONFIGURE_COMMAND ${GLUT_CONFIGURE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

set(_SB_GLUT_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
if(WIN32)
  set(_SB_GLUT_LIBRARY ${SB_INSTALL_PREFIX}/lib/libfreeglut.lib)
elseif(UNIX)
  set(_SB_GLUT_LIBRARY ${SB_INSTALL_PREFIX}/lib/libfreeglut${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()
