message(STATUS "Setup Qt4 ...")

set(proj QT4)

set(DEFAULT_USE_SYSTEM_QT4  ON)

option(USE_SYSTEM_QT4 "  Use a system build of Qt4." ${DEFAULT_USE_SYSTEM_QT4})
mark_as_advanced(USE_SYSTEM_QT4)

if(USE_SYSTEM_QT4)
  message(STATUS "  Using Qt4 system version")
else()
  set(${proj}_DEPENDENCIES)
  set(QT4_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(QT4_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  if(NOT DEFINED git_protocol)
    set(git_protocol "git")
  endif()
  
  #activate only required components for Qt4
  set(QT4_SB_CONFIG )

  if(NOT USE_SYSTEM_ZLIB) 
    list(APPEND ${proj}_DEPENDENCIES ZLIB)
  endif()

  if(NOT USE_SYSTEM_TIFF) 
    list(APPEND ${proj}_DEPENDENCIES TIFF)
  endif()

  if(NOT USE_SYSTEM_PNG) 
    list(APPEND ${proj}_DEPENDENCIES PNG)
  endif()
  
  if(NOT USE_SYSTEM_SQLITE) 
    list(APPEND ${proj}_DEPENDENCIES SQLITE)
  endif()
 
 #use system libs always for Qt4 as we build them from source or have already in system
  set(QT4_SB_CONFIG)

    STRING(REGEX REPLACE "/$" "" CMAKE_WIN_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})    
    STRING(REGEX REPLACE "/" "\\\\" CMAKE_WIN_INSTALL_PREFIX ${CMAKE_WIN_INSTALL_PREFIX})
    
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://download.qt-project.org/official_releases/qt/4.8/4.8.6/qt-everywhere-opensource-src-4.8.6.tar.gz"
    URL_MD5 2edbe4d6c2eff33ef91732602f3518eb
    BINARY_DIR ${QT4_SB_SRC}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -E chdir ${QT4_SB_SRC} configure -prefix ${CMAKE_WIN_INSTALL_PREFIX} -opensource -confirm-license -release -shared -nomake demos -nomake examples -nomake tools -no-phonon-backend -no-phonon -no-script -no-scripttools -no-multimedia -no-webkit -qt-sql-sqlite -plugin-sql-sqlite -no-nis -no-qt3support -system-zlib -system-libpng -system-libtiff -system-libjpeg -system-sqlite
    DEPENDS ${${proj}_DEPENDENCIES}
    )
  
  #copying Qt4 to have out-of-source build takes too much disk space
  # ExternalProject_Add_Step(${proj} copy_source
      # COMMAND ${CMAKE_COMMAND} -E copy_directory 
      # ${QT4_SB_SRC} ${QT4_SB_BUILD_DIR}
      # DEPENDEES patch update
      # DEPENDERS configure
  # )

  message(STATUS "  Using Qt4 SuperBuild version")

endif()