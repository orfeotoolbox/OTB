INCLUDE_ONCE_MACRO(QT4)

SETUP_SUPERBUILD(QT4)

#RK: are we ready for QT4 build on linux?.
#This comment here scares me away.
#Installing QT4 from packages also need sqlite otherwise there is issue

# if(UNIX AND NOT APPLE)
#   message(STATUS "  SuperBuild may fail to compile Qt4. If so, you should install it via package manager.")
# endif()

if(NOT DEFINED git_protocol)
  set(git_protocol "git")
endif()

#NOTE: make sure your superbuild install directory does not contain any
#Qt files from previous install of superbuild QT.
# declare dependencies
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(QT4 ZLIB PNG JPEG SQLITE FREETYPE)

#use system libs always for Qt4 as we build them from source or have already in system
set(QT4_SB_CONFIG)
#RK: building faling on mac. png include is in a macframework
if(USE_SYSTEM_PNG)
  set(QT4_SB_CONFIG "-I ${PNG_PNG_INCLUDE_DIR}")
endif()

if(UNIX)
  if(APPLE)
    set(QT4_SB_CONFIG "${QT4_SB_CONFIG} -no-framework")
  else() #Linux
    set(QT4_SB_CONFIG "${QT4_SB_CONFIG} -sm -xrender -xrandr -gtkstyle")
  endif()
  set(QT4_SB_CONFIG "${QT4_SB_CONFIG} -no-nis -no-javascript-jit -v")
elseif(MSVC)
  set(QT4_SB_CONFIG "${QT4_SB_CONFIG} -mp")
endif()

if(USE_SYSTEM_FREETYPE)
  set(FT_INCLUDE ${FREETYPE_INCLUDE_DIRS})
else()
  set(FT_INCLUDE ${_SB_FREETYPE_INCLUDE_DIRS})
endif()

if(FT_INCLUDE)
  file(TO_NATIVE_PATH ${FT_INCLUDE} FT_INCLUDE_NATIVE)
endif()

if(SB_INSTALL_PREFIX)
  file(TO_NATIVE_PATH ${SB_INSTALL_PREFIX} QT4_INSTALL_PREFIX_NATIVE)
  file(TO_NATIVE_PATH ${SB_INSTALL_PREFIX}/include QT4_INCLUDE_PREFIX_NATIVE)
  file(TO_NATIVE_PATH ${SB_INSTALL_PREFIX}/lib QT4_LIB_PREFIX_NATIVE)
endif()

if(WIN32)
  set(QT4_BIN_EXT ".exe")
  file(TO_NATIVE_PATH ${QT4_SB_SRC}/configure.exe QT4_CONFIGURE_SCRIPT)
  set(QT4_CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/configure_qt4.bat)
  set(QT4_CONFIGURE_COMMAND_IN ${CMAKE_SOURCE_DIR}/patches/QT4/configure_qt4.bat.in)
else()
  set(QT4_BIN_EXT "")
  file(TO_NATIVE_PATH ${QT4_SB_SRC}/configure QT4_CONFIGURE_SCRIPT)
  set(QT4_CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/configure_qt4.sh)
  set(QT4_CONFIGURE_COMMAND_IN ${CMAKE_SOURCE_DIR}/patches/QT4/configure_qt4.sh.in)
endif()

configure_file(${QT4_CONFIGURE_COMMAND_IN} ${QT4_CONFIGURE_COMMAND} @ONLY )

#Remove left over or previous installation from install prefix.
#Existing files in install prefix was disturbing a second installation.
#even afer the QT4 directory is removed from build

add_custom_target(QT4-uninstall
  COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/include/QtCore"
  COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/include/QtDBus"
  COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/include/QtGui"
  COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/include/QtNetwork"
  COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/include/QtOpenGL"
  COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/include/QtSql"
  COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/include/QtSvg"
  COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/include/QtTest"
  COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/include/QtXml"
  COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/mkspecs"
  COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/plugins"
  COMMAND ${CMAKE_COMMAND} -E remove_directory "${SB_INSTALL_PREFIX}/translations"
  COMMAND ${CMAKE_COMMAND} -E remove -f "${SB_INSTALL_PREFIX}/lib/libQt*"
  COMMAND ${CMAKE_COMMAND} -E remove -f "${SB_INSTALL_PREFIX}/bin/qmake${QT4_BIN_EXT}"
  COMMAND ${CMAKE_COMMAND} -E remove -f "${SB_INSTALL_PREFIX}/bin/lrelease${QT4_BIN_EXT}"
  COMMAND ${CMAKE_COMMAND} -E remove -f "${SB_INSTALL_PREFIX}/bin/moc${QT4_BIN_EXT}"
  COMMAND ${CMAKE_COMMAND} -E remove -f "${SB_INSTALL_PREFIX}/bin/rcc${QT4_BIN_EXT}"
  COMMAND ${CMAKE_COMMAND} -E remove -f "${SB_INSTALL_PREFIX}/bin/uic${QT4_BIN_EXT}"
  COMMAND ${CMAKE_COMMAND} -E remove -f "${SB_INSTALL_PREFIX}/bin/libQt*"
  WORKING_DIRECTORY "${SB_INSTALL_PREFIX}"
  )

#adding it to dependencies will remove the files when configure QWT
#list(APPEND QT4_DEPENDENCIES QT4-uninstall)

  ExternalProject_Add(QT4
    PREFIX QT4
    URL "http://download.qt.io/official_releases/qt/4.8/4.8.7/qt-everywhere-opensource-src-4.8.7.tar.gz"
    URL_MD5 d990ee66bf7ab0c785589776f35ba6ad
    BINARY_DIR ${QT4_SB_SRC}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    CONFIGURE_COMMAND ${QT4_CONFIGURE_COMMAND}
    DEPENDS ${QT4_DEPENDENCIES}
    )


 if(MSVC)
   #Q: why this copy here?.
   #RK: Because QT4 sucks with qmake -query.
   ExternalProject_Add_Step(QT4 patches
     COMMAND ${CMAKE_COMMAND} -E copy_directory
     ${QT4_SB_SRC}/mkspecs ${SB_INSTALL_PREFIX}/mkspecs
     COMMAND ${CMAKE_COMMAND} -E copy
     ${CMAKE_SOURCE_DIR}/patches/QT4/qjpeghandler.pri
     ${QT4_SB_SRC}/src/gui/image/
     DEPENDEES patch update
     DEPENDERS configure )
endif()

SUPERBUILD_PATCH_SOURCE(QT4)

set(_SB_QT_QMAKE_EXECUTABLE ${SB_INSTALL_PREFIX}/bin/qmake)
