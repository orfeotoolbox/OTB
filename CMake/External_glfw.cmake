message(STATUS "Setup glfw...")

set(proj GLFW)

set(DEFAULT_USE_SYSTEM_GLFW  ON)

option(USE_SYSTEM_GLFW "  Use system installed GLFW." ${DEFAULT_USE_SYSTEM_GLFW})
mark_as_advanced(USE_SYSTEM_GLFW)

if(USE_SYSTEM_GLFW)
  message(STATUS "  Using glfw system version")
else()
  set(${proj}_DEPENDENCIES)
  set(GLFW_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(GLFW_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://sourceforge.net/projects/glfw/files/glfw/3.1/glfw-3.1.zip/download"
    URL_MD5 c4f6901c88ad0795f7c5da6b5599202f
    BINARY_DIR ${GLFW_SB_BUILD_DIR}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DBUILD_SHARED_LIBS:BOOL=ON
      -DGLFW_BUILD_EXAMPLES:BOOL=OFF
      -DGLFW_BUILD_DOCS:BOOL=OFF
      -DGLFW_BUILD_TESTS:BOOL=OFF
      -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
  )

  message(STATUS "  Using glfw SuperBuild version")
endif()
