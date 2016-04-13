if(NOT __EXTERNAL_QT4__)
set(__EXTERNAL_QT4__ 1)

message(STATUS "Setup Qt4 ...")

if(USE_SYSTEM_QT4)
  find_package ( Qt4 )
  message(STATUS "  Using Qt4 system version")
else()
  SETUP_SUPERBUILD(PROJECT QT4)
  message(STATUS "  Using Qt4 SuperBuild version")
  if(UNIX AND NOT APPLE)
    message(STATUS "  SuperBuild may fail to compile Qt4. If so, you should install it via package manager.")
  endif()

  if(NOT DEFINED git_protocol)
    set(git_protocol "git")
  endif()

  # declare dependencies
  ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(QT4 ZLIB TIFF PNG SQLITE FREETYPE)

  #use system libs always for Qt4 as we build them from source or have already in system
  set(QT4_SB_CONFIG)

    STRING(REGEX REPLACE "/$" "" CMAKE_WIN_INSTALL_PREFIX ${SB_INSTALL_PREFIX})
    STRING(REGEX REPLACE "/" "\\\\" CMAKE_WIN_INSTALL_PREFIX ${CMAKE_WIN_INSTALL_PREFIX})

  if(MSVC)
    configure_file(${CMAKE_SOURCE_DIR}/patches/QT4/configure_qt4.bat.in
                  ${CMAKE_BINARY_DIR}/configure_qt4.bat
                  @ONLY)

    ExternalProject_Add(QT4
      PREFIX QT4
      URL "http://download.qt.io/official_releases/qt/4.8/4.8.7/qt-everywhere-opensource-src-4.8.7.tar.gz"
      URL_MD5 d990ee66bf7ab0c785589776f35ba6ad
      BINARY_DIR ${QT4_SB_SRC}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
      CONFIGURE_COMMAND
        configure_qt4.bat
      PATCH_COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/configure_qt4.bat
        ${QT4_SB_SRC}
        DEPENDS ${QT4_DEPENDENCIES} )

    ExternalProject_Add_Step(QT4 _jpeg_lib_name
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/patches/QT4/qjpeghandler.pri
      ${QT4_SB_SRC}/src/gui/image/
      DEPENDEES patch update
      DEPENDERS configure  )

  else()
    ExternalProject_Add(QT4
      PREFIX QT4
      URL "http://download.qt.io/official_releases/qt/4.8/4.8.7/qt-everywhere-opensource-src-4.8.7.tar.gz"
      URL_MD5 d990ee66bf7ab0c785589776f35ba6ad
      BINARY_DIR ${QT4_SB_SRC}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
      CONFIGURE_COMMAND
        ${QT4_SB_SRC}/configure -prefix ${SB_INSTALL_PREFIX} -opensource -confirm-license -release -shared -nomake demos -nomake examples -nomake tools -no-phonon-backend -no-phonon -no-script -no-scripttools -no-multimedia -no-webkit -qt-sql-sqlite -plugin-sql-sqlite -no-nis -no-qt3support  -system-zlib -system-libpng -system-libtiff -system-libjpeg -system-sqlite -L ${SB_INSTALL_PREFIX}/lib -I ${SB_INSTALL_PREFIX}/include -I ${SB_INSTALL_PREFIX}/include/freetype2 -no-xrender -no-fontconfig -no-sm
      DEPENDS ${QT4_DEPENDENCIES}
      )
  endif()

  set(_SB_QT_QMAKE_EXECUTABLE ${SB_INSTALL_PREFIX}/bin/qmake)

  #copying Qt4 to have out-of-source build takes too much disk space
  # ExternalProject_Add_Step(QT4 copy_source
      # COMMAND ${CMAKE_COMMAND} -E copy_directory
      # ${QT4_SB_SRC} ${QT4_SB_BUILD_DIR}
      # DEPENDEES patch update
      # DEPENDERS configure
  # )

endif()
endif()
