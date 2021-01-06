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

INCLUDE_ONCE_MACRO(QWT)

SETUP_SUPERBUILD(QWT)

# declare dependencies
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(QWT QT5)

set(QWT_SB_MAKE_PROGRAM ${CMAKE_MAKE_PROGRAM})
if(MSVC)
  set(QWT_SB_MAKE_PROGRAM "nmake")
endif()

configure_file(${CMAKE_SOURCE_DIR}/patches/QWT/qwtconfig.pri
  ${CMAKE_BINARY_DIR}/qwtconfig.pri
  @ONLY)

if(_SB_QT_QMAKE_EXECUTABLE)
  set(QWT_SB_CONFIGURE_PROGRAM ${_SB_QT_QMAKE_EXECUTABLE})

elseif(QT_QMAKE_EXECUTABLE)
  set(QWT_SB_CONFIGURE_PROGRAM ${QT_QMAKE_EXECUTABLE})

else()
  find_program( QT_QMAKE_EXECUTABLE "qmake" )

  if( NOT QT_QMAKE_EXECUTABLE )
    set(QT_QMAKE_EXECUTABLE "" CACHE FILEPATH "Path to qmake executable")
    message(FATAL_ERROR "Please set the qmake executable to use (QT_QMAKE_EXECUTABLE)")

  else()
    set( QWT_SB_CONFIGURE_PROGRAM ${QT_QMAKE_EXECUTABLE} )

  endif()
endif()

ExternalProject_Add(QWT
  PREFIX QWT
  URL "http://downloads.sourceforge.net/project/qwt/qwt/6.1.5/qwt-6.1.5.zip"
  URL_MD5 61a8cae35ab6201d916304ec4a6f06b8
  SOURCE_DIR ${QWT_SB_SRC}
  BINARY_DIR ${QWT_SB_SRC}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  DEPENDS ${QWT_DEPENDENCIES}
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/qwtconfig.pri ${QWT_SB_SRC}
  CONFIGURE_COMMAND ${QWT_SB_CONFIGURE_PROGRAM} ${QWT_SB_SRC}/qwt.pro
  BUILD_COMMAND ${QWT_SB_MAKE_PROGRAM}
  INSTALL_COMMAND ${QWT_SB_MAKE_PROGRAM} install
  LOG_CONFIGURE 0
  LOG_BUILD 0
  LOG_INSTALL 0
  )

SUPERBUILD_PATCH_SOURCE(QWT)

set(_SB_QWT_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
if(WIN32)
  set(_SB_QWT_LIBRARY ${SB_INSTALL_PREFIX}/lib/qwt_i.lib)
elseif(UNIX)
  set(_SB_QWT_LIBRARY ${SB_INSTALL_PREFIX}/lib/libqwt${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()
