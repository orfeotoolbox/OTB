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

INCLUDE_ONCE_MACRO(QT5)

SETUP_SUPERBUILD(QT5)

#RK: are we ready for QT4 build on linux?.
#This comment here scares me away.
#Installing QT4 from packages also need sqlite otherwise there is issue

# if(UNIX AND NOT APPLE)
#   message(STATUS "  SuperBuild may fail to compile Qt4. If so, you should install it via package manager.")
# endif()


#set(QT4_SB_ENABLE_GTK OFF CACHE INTERNAL "Enable GTK+ style with qt using -gtkstlye. Default is OFF")

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

#Common options for all cases
# -skip qtbase
# -skip qttools  (need linguist)
# -skip qttranslations
set(QT5_SB_CONFIG
  "-prefix ${QT5_INSTALL_PREFIX_NATIVE} -L ${QT5_LIB_PREFIX_NATIVE} \
  -I ${QT5_INCLUDE_PREFIX_NATIVE} -I ${QT5_INCLUDE_FREETYPE_NATIVE} \
  -opensource -confirm-license -release -shared \
  -nomake examples -make tools -no-openssl \
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
  -system-libpng -system-libjpeg -system-zlib -system-freetype")
# "-prefix ${QT4_INSTALL_PREFIX_NATIVE} -L ${QT4_LIB_PREFIX_NATIVE} \
# -I ${QT4_INCLUDE_PREFIX_NATIVE} -I ${QT4_INCLUDE_FREETYPE_NATIVE} \
# -opensource -confirm-license -release -shared -nomake demos \
# -nomake examples -nomake tools -no-phonon-backend -no-phonon -no-script \
# -no-scripttools -no-multimedia -no-audio-backend -no-webkit -no-declarative \
# -no-accessibility -no-qt3support -no-xmlpatterns -no-sql-sqlite -no-openssl \
# -no-libtiff -no-libmng -system-libpng -system-libjpeg -system-zlib")

#RK: building faling on mac. png include is in a macframework
# if(USE_SYSTEM_PNG)
  # set(QT4_SB_CONFIG "${QT4_SB_CONFIG} -I ${PNG_PNG_INCLUDE_DIR}")
# endif()

if(UNIX)
  if(APPLE)
    set(QT5_SB_CONFIG "${QT5_SB_CONFIG} -no-framework")
  else() #Linux
#    if(QT5_SB_ENABLE_GTK)
#      message(WARNING "QT5_SB_ENABLE_GTK support is experimental")
#      set(QT5_SB_CONFIG "${QT5_SB_CONFIG} -sm -xrender -xrandr -gtkstyle")
#    else()
      set(QT5_SB_CONFIG "${QT5_SB_CONFIG} -no-glib -no-fontconfig")
#    endif()
  endif()
  #common for all unix
  set(QT5_SB_CONFIG "${QT5_SB_CONFIG} -no-dbus -no-icu -v")
elseif(MSVC)
  set(QT5_SB_CONFIG "${QT5_SB_CONFIG} -mp")
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

#Remove left over or previous installation from install prefix.
#Existing files in install prefix was disturbing a second installation.
#even after the QT4 directory is removed from build

# add_custom_target(QT4-uninstall
#   COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/include/QtCore"
#   COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/include/QtDBus"
#   COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/include/QtGui"
#   COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/include/QtNetwork"
#   COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/include/QtOpenGL"
#   COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/include/QtSql"
#   COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/include/QtSvg"
#   COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/include/QtTest"
#   COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/include/QtXml"
#   COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/include/Qt"
#   COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/mkspecs"
#   COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/plugins"
#   COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/translations"
#   COMMAND ${CMAKE_COMMAND} -E remove -f "${SB_INSTALL_PREFIX}/lib/libQt*"
#   COMMAND ${CMAKE_COMMAND} -E remove -f "${SB_INSTALL_PREFIX}/lib/pkgconfig/Qt*"
#   COMMAND ${CMAKE_COMMAND} -E remove -f "${SB_INSTALL_PREFIX}/bin/qmake${QT4_BIN_EXT}"
#   COMMAND ${CMAKE_COMMAND} -E remove -f "${SB_INSTALL_PREFIX}/bin/lrelease${QT4_BIN_EXT}"
#   COMMAND ${CMAKE_COMMAND} -E remove -f "${SB_INSTALL_PREFIX}/bin/moc${QT4_BIN_EXT}"
#   COMMAND ${CMAKE_COMMAND} -E remove -f "${SB_INSTALL_PREFIX}/bin/rcc${QT4_BIN_EXT}"
#   COMMAND ${CMAKE_COMMAND} -E remove -f "${SB_INSTALL_PREFIX}/bin/uic${QT4_BIN_EXT}"
#   COMMAND ${CMAKE_COMMAND} -E remove -f "${SB_INSTALL_PREFIX}/bin/libQt*"
#   WORKING_DIRECTORY "${SB_INSTALL_PREFIX}"
#   )

#adding it to dependencies will remove the files when configure QWT
#list(APPEND QT4_DEPENDENCIES QT4-uninstall)

  ExternalProject_Add(QT5
    PREFIX QT5
    URL "http://download.qt.io/official_releases/qt/5.10/5.10.1/single/qt-everywhere-src-5.10.1.tar.xz"
    URL_MD5 7e167b9617e7bd64012daaacb85477af
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
  -D BUILD_DIR=${QT5_SB_BUILD_DIR} -D INSTALL_DIR=${SB_INSTALL_PREFIX} -D DOWNLOAD_LOCATION=${DOWNLOAD_LOCATION} -P /home/antoine/dev/off_otbL/otb/SuperBuild/CMake/font_qt.cmake 
  DEPENDEES install
  WORKING_DIRECTORY ${SB_INSTALL_PREFIX} )
endif()
