if(NOT __EXTERNAL_GLFW__)
set(__EXTERNAL_GLFW__ 1)

if(USE_SYSTEM_GLFW)
  message(STATUS "  Using glfw system version")
else()
  set(GLFW_DEPENDENCIES)
  set(GLFW_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/GLFW/build)
  set(GLFW_SB_SRC ${CMAKE_BINARY_DIR}/GLFW/src/GLFW)

  ExternalProject_Add(GLFW
    PREFIX GLFW
    URL "https://github.com/glfw/glfw/releases/download/3.1.2/glfw-3.1.2.zip"
    URL_MD5 8023327bfe979b3fe735e449e2f54842
    BINARY_DIR ${GLFW_SB_BUILD_DIR}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    CMAKE_CACHE_ARGS
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DBUILD_SHARED_LIBS:BOOL=ON
      -DGLFW_BUILD_EXAMPLES:BOOL=OFF
      -DGLFW_BUILD_DOCS:BOOL=OFF
      -DGLFW_BUILD_TESTS:BOOL=OFF
      -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
  )

  set(_SB_GLFW_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_GLFW_LIBRARY ${SB_INSTALL_PREFIX}/lib/libglfw.lib)
  elseif(UNIX)
    set(_SB_GLFW_LIBRARY ${SB_INSTALL_PREFIX}/lib/libglfw${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

  message(STATUS "  Using glfw SuperBuild version")
endif()
endif()
