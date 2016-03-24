set(proj GLUT)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup glut...")

if(USE_SYSTEM_GLUT)
  message(STATUS "  Using glut system version")
else()
  set(${proj}_DEPENDENCIES)
  set(GLUT_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(GLUT_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://downloads.sourceforge.net/project/freeglut/freeglut/2.8.1/freeglut-2.8.1.tar.gz"
    URL_MD5 918ffbddcffbac83c218bc52355b6d5a
    BINARY_DIR ${GLUT_SB_BUILD_DIR}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy  ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt ${GLUT_SB_SRC}
    CMAKE_CACHE_ARGS
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DBUILD_SHARED_LIBS:BOOL=ON
      -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
  )
  
  set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libfreeglut.lib)
  elseif(UNIX)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libfreeglut${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

  message(STATUS "  Using glut SuperBuild version")
endif()
endif()
