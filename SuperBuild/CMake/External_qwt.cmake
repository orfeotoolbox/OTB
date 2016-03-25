set(proj QWT)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup Qwt...")

if(USE_SYSTEM_QWT)
  message(STATUS "  Using ${proj} system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using Qwt SuperBuild version")

  # declare dependencies
  ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(${proj} QT4)

  INCLUDE_SUPERBUILD_DEPENDENCIES(${${proj}_DEPENDENCIES})
  # set proj back to its original value
  set(proj QWT)
  
  set(QWT_SB_MAKE_PROGRAM ${CMAKE_MAKE_PROGRAM})
  if(MSVC)
    set(QWT_SB_MAKE_PROGRAM "nmake")
  endif()
   
  configure_file(${CMAKE_SOURCE_DIR}/patches/${proj}/qwtconfig.pri 
                 ${CMAKE_BINARY_DIR}/qwtconfig.pri
                 @ONLY)
  # TODO : detect the qmake executable path
  set(QT_QMAKE_EXECUTABLE ${SB_INSTALL_PREFIX}/bin/qmake)
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://sourceforge.net/projects/qwt/files/qwt/5.2.3/qwt-5.2.3.zip/download"
    URL_MD5 310a1c8ab831f4b2219505dcb7691cf1
    SOURCE_DIR ${QWT_SB_SRC}
    BINARY_DIR ${QWT_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    DEPENDS ${${proj}_DEPENDENCIES}
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy_directory ${QWT_SB_SRC} ${QWT_SB_BUILD_DIR}
     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/qwtconfig.pri ${QWT_SB_BUILD_DIR}
    CONFIGURE_COMMAND ${QT_QMAKE_EXECUTABLE} ${QWT_SB_BUILD_DIR}/qwt.pro
    BUILD_COMMAND ${QWT_SB_MAKE_PROGRAM}
    INSTALL_COMMAND ${QWT_SB_MAKE_PROGRAM} install
  )
  
  set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libqwt.lib)
  elseif(UNIX)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libqwt${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

endif()
endif()
