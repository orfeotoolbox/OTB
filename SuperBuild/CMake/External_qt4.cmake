set(proj QT4)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup Qt4 ...")

if(USE_SYSTEM_QT4)
  find_package ( Qt4 )
  add_custom_target(${proj})
  message(STATUS "  Using Qt4 system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using Qt4 SuperBuild version")
  
  if(NOT DEFINED git_protocol)
    set(git_protocol "git")
  endif()
  
  # declare dependencies
  set(${proj}_DEPENDENCIES ZLIB TIFF PNG SQLITE)
  INCLUDE_SUPERBUILD_DEPENDENCIES(${${proj}_DEPENDENCIES})
  # set proj back to its original value
  set(proj QT4)
  
 
  #use system libs always for Qt4 as we build them from source or have already in system
  set(QT4_SB_CONFIG)

    STRING(REGEX REPLACE "/$" "" CMAKE_WIN_INSTALL_PREFIX ${SB_INSTALL_PREFIX})    
    STRING(REGEX REPLACE "/" "\\\\" CMAKE_WIN_INSTALL_PREFIX ${CMAKE_WIN_INSTALL_PREFIX})
    
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://download.qt-project.org/official_releases/qt/4.8/4.8.6/qt-everywhere-opensource-src-4.8.6.tar.gz"
    URL_MD5 2edbe4d6c2eff33ef91732602f3518eb
    BINARY_DIR ${QT4_SB_SRC}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    CONFIGURE_COMMAND
      configure
        -prefix ${CMAKE_WIN_INSTALL_PREFIX}
        -opensource
        -confirm-license
        -release
        -shared
        -nomake demos
        -nomake examples
        -nomake tools
        -no-phonon-backend
        -no-phonon
        -no-script
        -no-scripttools
        -no-multimedia
        -no-webkit
        -qt-sql-sqlite
        -plugin-sql-sqlite
        -no-nis
        -no-qt3support
        -system-zlib
        -system-libpng
        -system-libtiff
        -system-libjpeg
        #-system-sqlite
        -L ${CMAKE_WIN_INSTALL_PREFIX}\\lib
        -I ${CMAKE_WIN_INSTALL_PREFIX}\\include
    DEPENDS ${${proj}_DEPENDENCIES}
    )
  
  #copying Qt4 to have out-of-source build takes too much disk space
  # ExternalProject_Add_Step(${proj} copy_source
      # COMMAND ${CMAKE_COMMAND} -E copy_directory 
      # ${QT4_SB_SRC} ${QT4_SB_BUILD_DIR}
      # DEPENDEES patch update
      # DEPENDERS configure
  # )

endif()
endif()
