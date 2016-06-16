INCLUDE_ONCE_MACRO(QT4)

SETUP_SUPERBUILD(PROJECT QT4)

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
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(QT4 ZLIB SQLITE FREETYPE)

#use system libs always for Qt4 as we build them from source or have already in system
set(QT4_SB_CONFIG)
#RK: building faling on mac. png include is in a macframework
if(USE_SYSTEM_PNG)
  set(QT4_SB_CONFIG "-I ${PNG_PNG_INCLUDE_DIR}")
endif()

if(UNIX)
  set(QT4_SB_CONFIG "${QT4_SB_CONFIG} -no-openssl")
  if(APPLE)
    set(QT4_SB_CONFIG "${QT4_SB_CONFIG} -no-framework")
  else() #Linux
    set(QT4_SB_CONFIG "${QT4_SB_CONFIG} -no-sm -no-xrender -no-fontconfig -no-gtkstyle -no-glib")
  endif()
endif()
STRING(REGEX REPLACE "/$" "" CMAKE_WIN_INSTALL_PREFIX ${SB_INSTALL_PREFIX})
STRING(REGEX REPLACE "/" "\\\\" CMAKE_WIN_INSTALL_PREFIX ${CMAKE_WIN_INSTALL_PREFIX})

if(MSVC)
  configure_file(
    ${CMAKE_SOURCE_DIR}/patches/QT4/configure_qt4.bat.in
    ${CMAKE_BINARY_DIR}/configure_qt4.bat
    @ONLY)
   
  set(QT4_CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/configure_qt4.bat)
  
else()
  configure_file(
    ${QT4_SB_SRC}/patches/QT4/configure_qt4.sh.in
    ${CMAKE_BINARY_DIR}/configure_qt4.sh
    @ONLY)
    
  set(QT4_CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/configure_qt4.sh)
  
endif()

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
  ExternalProject_Add_Step(QT4 copy_specs
    COMMAND
    ${CMAKE_COMMAND} -E copy_directory 
    ${QT4_SB_SRC}/mkspecs ${SB_INSTALL_PREFIX}/mkspecs
    DEPENDEES patch update
    DEPENDERS configure )
    
  ExternalProject_Add_Step(QT4 _jpeg_lib_name
    COMMAND
    ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/patches/QT4/qjpeghandler.pri
    ${QT4_SB_SRC}/src/gui/image/
    DEPENDEES patch update
    DEPENDERS configure )
  
endif()



  SUPERBUILD_PATCH_SOURCE(QT4)


set(_SB_QT_QMAKE_EXECUTABLE ${SB_INSTALL_PREFIX}/bin/qmake)
