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
  
  set(SB_QT4_CONFIG)
  set(QT_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
  if(MSVC)
  #--prefix for configure requires path in windows style
   STRING(REGEX REPLACE "/" "\\\\" QT_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
   set(SB_QT4_CONFIG -no-webkit -no-script -system-libpng -system-libtiff -system-libjpeg -system-zlib -system-sqlite -no-freetype -no-phonon -no-phonon-backend -no-script -no-scripttools -no-multimedia -nomake demos -platform win32-msvc2010 -nomake examples -nomake tools)
  endif()

  if(MSVC)
    message(STATUS "  Qt4 is not added to SuperBuild yet..")
  else(UNIX)
  
   ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://download.qt-project.org/official_releases/qt/4.8/4.8.6/qt-everywhere-opensource-src-4.8.6.tar.gz"
    URL_MD5 2edbe4d6c2eff33ef91732602f3518eb
    #GIT_REPOSITORY "${git_protocol}://gitorious.org/qt/qt.git"
    #GIT_TAG dde0c2f4f6d72bcc9db732eb51399395ffc37778
    BINARY_DIR ${QT4_SB_SRC}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CONFIGURE_COMMAND  ${QT4_SB_BUILD_DIR}/configure
       -prefix ${QT_INSTALL_PREFIX}
       -opensource
       -confirm-license
       -release
       -shared
       ${SB_QT4_CONFIG}
    BUILD_COMMAND ${SB_MAKE_CMD}            
    DEPENDS ${${proj}_DEPENDENCIES}
  )
  
    ExternalProject_Add_Step(${proj} copy_source
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
        ${QT4_SB_SRC} ${QT4_SB_BUILD_DIR}
        DEPENDEES patch update
        DEPENDERS configure
  )
  endif()

  
  message(STATUS "  Using Qt4 SuperBuild version")

endif()