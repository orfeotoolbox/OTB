set(proj QWT)

message(STATUS "Setup Qwt...")

set(DEFAULT_USE_SYSTEM_QWT  OFF)

option(USE_SYSTEM_QWT "  Use system installed GLFW." ${DEFAULT_USE_SYSTEM_QWT})
mark_as_advanced(USE_SYSTEM_QWT)

set(${proj}_DEPENDENCIES)

if(USE_SYSTEM_QWT)
  message(STATUS "  Using ${proj} system version")
else()
  if(USE_SYSTEM_QT4)
    message(STATUS "  Using Qt4 system version")
  else()
    set(MVD2_SB_QT4_CONFIG)
    if(MSVC)
      list(APPEND ${proj}_DEPENDENCIES QT4)
      set(MVD2_SB_QT4_CONFIG)
    else(UNIX)
      message(STATUS "  This platform does not provide Qt4 via SuperBuild")
    endif()
  endif()

  set(QWT_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(QWT_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  set(QWT_SB_MAKE_PROGRAM ${CMAKE_MAKE_PROGRAM})
  if(MSVC)
    set(QWT_SB_MAKE_PROGRAM "nmake")
  endif()
   
  configure_file(${CMAKE_SOURCE_DIR}/patches/${proj}/qwtconfig.pri 
                 ${CMAKE_BINARY_DIR}/qwtconfig.pri
                 @ONLY)
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://sourceforge.net/projects/qwt/files/qwt/5.2.3/qwt-5.2.3.zip/download"
    URL_MD5 310a1c8ab831f4b2219505dcb7691cf1
    BINARY_DIR ${QWT_SB_SRC}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    DEPENDS ${${proj}_DEPENDENCIES}
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/qwtconfig.pri ${QWT_SB_SRC}
    CONFIGURE_COMMAND qmake ${QWT_SB_SRC}/qwt.pro
    BUILD_COMMAND ${QWT_SB_MAKE_PROGRAM}
    INSTALL_COMMAND ${QWT_SB_MAKE_PROGRAM} install
  )
  
  message(STATUS "  Using Qwt SuperBuild version")
endif()
