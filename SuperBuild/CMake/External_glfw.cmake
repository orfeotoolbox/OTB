if(NOT __EXTERNAL_GLFW__w)
  set(__EXTERNAL_GLFW__ 1)

  if(USE_SYSTEM_GLFW)
    message(STATUS "  Using GLFW system version")
  else()
    SETUP_SUPERBUILD(PROJECT GLFW)
    message(STATUS "  Using GLFW SuperBuild version")

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
      CMAKE_COMMAND ${SB_CMAKE_COMMAND}
      )

    set(_SB_GLFW_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
    if(WIN32)
      set(_SB_GLFW_LIBRARY ${SB_INSTALL_PREFIX}/lib/libglfw.lib)
    elseif(UNIX)
      set(_SB_GLFW_LIBRARY ${SB_INSTALL_PREFIX}/lib/libglfw${CMAKE_SHARED_LIBRARY_SUFFIX})
    endif()

  endif()
endif()
