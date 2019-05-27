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

INCLUDE_ONCE_MACRO(QWT)

SETUP_SUPERBUILD(QWT)

# declare dependencies
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(QWT QT5)

configure_file(${CMAKE_SOURCE_DIR}/patches/QWT/qwtconfig.pri
  ${CMAKE_BINARY_DIR}/QWT/tmp/qwtconfig.pri
  @ONLY)
  
 if(CMAKE_BUILD_TYPE MATCHES "Debug")
  set(OTB_QWT_BUILD_TYPE "debug")
else()
  set(OTB_QWT_BUILD_TYPE "release")
endif()
configure_file(${CMAKE_SOURCE_DIR}/patches/QWT/qwtbuild.pri
  ${CMAKE_BINARY_DIR}/QWT/tmp/qwtbuild.pri
  @ONLY)
  
set( OTB_QWT_BUILD_TYPE "debug")
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
  ## MUST change SuperBuild/patches/QWT/qwtbuild.pri and qwtconfig.pri when qwt version is upgraded
  URL "https://sourceforge.net/projects/qwt/files/qwt/6.1.3/qwt-6.1.3.zip/download"
  URL_MD5 558911df37aee4c0c3049860e967401c 
  SOURCE_DIR ${QWT_SB_SRC}
  BINARY_DIR ${QWT_SB_SRC}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  DEPENDS ${QWT_DEPENDENCIES}
  PATCH_COMMAND 
	${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/QWT/tmp/qwtbuild.pri ${CMAKE_BINARY_DIR}/QWT/tmp/qwtconfig.pri ${QWT_SB_SRC}
  CONFIGURE_COMMAND ${QWT_SB_CONFIGURE_PROGRAM} ${QWT_SB_SRC}/qwt.pro
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

SUPERBUILD_PATCH_SOURCE(QWT)
