if(NOT __EXTERNAL_GLUT__)
set(__EXTERNAL_GLUT__ 1)

if(USE_SYSTEM_GLUT)
  message(STATUS "  Using GLUT system version")
else()

  if(APPLE)
    message(FATAL_ERROR "Cannot activate GLUT on OSX. See manits issue #1194")
  endif()

  SETUP_SUPERBUILD(PROJECT GLUT)
  message(STATUS "  Using GLUT SuperBuild version")

  ExternalProject_Add(GLUT
    PREFIX GLUT
    URL "http://downloads.sourceforge.net/project/freeglut/freeglut/2.8.1/freeglut-2.8.1.tar.gz"
    URL_MD5 918ffbddcffbac83c218bc52355b6d5a
    BINARY_DIR ${GLUT_SB_BUILD_DIR}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/patches/GLUT/CMakeLists.txt
    ${GLUT_SB_SRC}
    CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  )

  set(_SB_GLUT_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_GLUT_LIBRARY ${SB_INSTALL_PREFIX}/lib/libfreeglut.lib)
  elseif(UNIX)
    set(_SB_GLUT_LIBRARY ${SB_INSTALL_PREFIX}/lib/libfreeglut${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

  message(STATUS "  Using glut SuperBuild version")
endif()
endif()
