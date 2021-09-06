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

INCLUDE_ONCE_MACRO(QT5)

SETUP_SUPERBUILD(QT5)

#NOTE: make sure your superbuild install directory does not contain any
#Qt files from previous install of superbuild QT.
# declare dependencies
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(QT5 ZLIB PNG JPEG FREETYPE)

#use system libs always for Qt5 as we build them from source or have already in system

if(SB_INSTALL_PREFIX)
  file(TO_NATIVE_PATH ${SB_INSTALL_PREFIX} QT5_INSTALL_PREFIX_NATIVE)
  file(TO_NATIVE_PATH ${SB_INSTALL_PREFIX}/include QT5_INCLUDE_PREFIX_NATIVE)
  file(TO_NATIVE_PATH ${SB_INSTALL_PREFIX}/lib QT5_LIB_PREFIX_NATIVE)
  file(TO_NATIVE_PATH ${SB_INSTALL_PREFIX}/include/freetype2 QT5_INCLUDE_FREETYPE_NATIVE)
endif()

file(TO_NATIVE_PATH ${QT5_SB_SRC} QT5_SB_SRC_NATIVE)

if(WIN32)
  set(QT5_SB_OPENGL_CONFIG "-opengl desktop")
else()
  set(QT5_SB_OPENGL_CONFIG "")
endif()

#Common options for all cases
# -skip qtbase
# -skip qttools  (need linguist)
# -skip qttranslations
set(QT5_SB_CONFIG
  "-prefix ${QT5_INSTALL_PREFIX_NATIVE} -L ${QT5_LIB_PREFIX_NATIVE} \
  -I ${QT5_INCLUDE_PREFIX_NATIVE} -I ${QT5_INCLUDE_FREETYPE_NATIVE} \
  -opensource -confirm-license -release -shared \
  -nomake examples -make tools -no-openssl \
  ${QT5_SB_OPENGL_CONFIG} \
  -skip qtgamepad  \
  -skip qt3d  \
  -skip qtactiveqt  \
  -skip qtandroidextras \
  -skip qtcanvas3d  \
  -skip qtcharts  \
  -skip qtconnectivity  \
  -skip qtdatavis3d  \
  -skip qtdeclarative  \
  -skip qtdoc  \
  -skip qtgamepad  \
  -skip qtgraphicaleffects  \
  -skip qtimageformats  \
  -skip qtlocation  \
  -skip qtmacextras  \
  -skip qtmultimedia  \
  -skip qtnetworkauth \
  -skip qtpurchasing  \
  -skip qtquickcontrols  \
  -skip qtquickcontrols2  \
  -skip qtremoteobjects  \
  -skip qtscript  \
  -skip qtsensors  \
  -skip qtserialbus  \
  -skip qtserialport  \
  -skip qtspeech \
  -skip qtsvg  \
  -skip qtvirtualkeyboard  \
  -skip qtwayland  \
  -skip qtwebchannel  \
  -skip qtwebengine  \
  -skip qtwebglplugin  \
  -skip qtwebsockets  \
  -skip qtwebview  \
  -skip qtwinextras  \
  -skip qtx11extras  \
  -skip qtxmlpatterns \
  -system-libpng -system-libjpeg -system-zlib -system-freetype -qt-harfbuzz")

if(UNIX)
  if(APPLE)
    set(QT5_SB_CONFIG "${QT5_SB_CONFIG} -no-framework")
  else()
      #Linux
      # -no-use-gold-linker: https://bugreports.qt.io/browse/QTBUG-66571
      # Without this option the gold linker is forced if it is available
      # In more recent versions the default linker is used instead, and this
      # option has been removed.
      set(QT5_SB_CONFIG "${QT5_SB_CONFIG} -no-glib -no-fontconfig -no-use-gold-linker")
  endif()
  #common for all unix
  set(QT5_SB_CONFIG "${QT5_SB_CONFIG} -no-dbus -no-icu -v")
elseif(MSVC)
  set(QT5_SB_CONFIG "${QT5_SB_CONFIG} -mp")
endif()

if(UNIX AND USE_LOW_KERNEL_VERSION)
  set(QT5_SB_CONFIG "${QT5_SB_CONFIG} -no-feature-getentropy -no-feature-renameat2")
endif()

if(WIN32)
  set(QT5_BIN_EXT ".exe")
  file(TO_NATIVE_PATH ${QT5_SB_SRC}/configure.bat QT5_CONFIGURE_SCRIPT)
  set(QT5_CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/configure_qt5.bat)
  set(QT5_CONFIGURE_COMMAND_IN ${CMAKE_SOURCE_DIR}/patches/QT5/configure_qt5.bat.in)
else()
  set(QT5_BIN_EXT "")
  file(TO_NATIVE_PATH ${QT5_SB_SRC}/configure QT5_CONFIGURE_SCRIPT)
  set(QT5_CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/configure_qt5.sh)
  set(QT5_CONFIGURE_COMMAND_IN ${CMAKE_SOURCE_DIR}/patches/QT5/configure_qt5.sh.in)
endif()

if(EXISTS "${QT5_CONFIGURE_COMMAND}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E remove -f "${QT5_CONFIGURE_COMMAND}")
endif()

configure_file( ${QT5_CONFIGURE_COMMAND_IN} ${QT5_CONFIGURE_COMMAND} @ONLY )

ExternalProject_Add(QT5
  PREFIX QT5
  URL "http://master.qt.io/archive/qt/5.15/5.15.2/single/qt-everywhere-src-5.15.2.tar.xz"
  URL_MD5 e1447db4f06c841d8947f0a6ce83a7b5
  BINARY_DIR ${QT5_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CONFIGURE_COMMAND ${QT5_CONFIGURE_COMMAND}
  DEPENDS ${QT5_DEPENDENCIES}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

SUPERBUILD_PATCH_SOURCE(QT5)

set(_SB_QT_QMAKE_EXECUTABLE ${SB_INSTALL_PREFIX}/bin/qmake)

if(UNIX AND NOT APPLE)
  ExternalProject_Add_Step(QT5 adding_font
  COMMAND ${CMAKE_COMMAND}
  -D BUILD_DIR=${QT5_SB_BUILD_DIR}
  -D INSTALL_DIR=${SB_INSTALL_PREFIX}
  -D DOWNLOAD_LOCATION=${DOWNLOAD_LOCATION}
  -P ${CMAKE_SOURCE_DIR}/CMake/External_font.cmake
  DEPENDEES install
  WORKING_DIRECTORY ${SB_INSTALL_PREFIX} )
endif()
