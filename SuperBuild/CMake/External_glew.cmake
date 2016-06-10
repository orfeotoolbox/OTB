if(NOT __EXTERNAL_GLEW__)
set(__EXTERNAL_GLEW__ 1)

if(USE_SYSTEM_GLEW)
  message(STATUS "  Using GLEW system version")
else()
  SETUP_SUPERBUILD(PROJECT GLEW)
  message(STATUS "  Using GLEW SuperBuild version")

  if(UNIX)
    ExternalProject_Add(GLEW
      PREFIX GLEW
      URL "https://sourceforge.net/projects/glew/files/glew/1.13.0/glew-1.13.0.tgz/download"
      URL_MD5 7cbada3166d2aadfc4169c4283701066
      BINARY_DIR ${GLEW_SB_BUILD_DIR}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DBUILD_SHARED_LIBS:BOOL=ON
      CMAKE_COMMAND ${SB_CMAKE_COMMAND}
      DEPENDS ${GLEW_DEPENDENCIES}
      PATCH_COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_SOURCE_DIR}/patches/GLEW/CMakeLists.txt
      ${GLEW_SB_SRC}
      )

  elseif(MSVC)
    set(SB_GLEW_DIR "Release/Win32")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(SB_GLEW_DIR "Release/x64")
    endif()
    ExternalProject_Add(GLEW
      PREFIX GLEW
      URL "https://sourceforge.net/projects/glew/files/glew/1.13.0/glew-1.13.0-win32.zip/download"
      URL_MD5 a2c0e4d75bdb84217550e51c0f1e7090
      BINARY_DIR ${GLEW_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
      PATCH_COMMAND ${CMAKE_COMMAND} -E copy_directory ${GLEW_SB_SRC}/include ${CMAKE_INSTALL_PREFIX}/include
      CONFIGURE_COMMAND ${CMAKE_COMMAND} -E copy ${GLEW_SB_SRC}/lib/${SB_GLEW_DIR}/glew32s.lib ${CMAKE_INSTALL_PREFIX}/lib
      BUILD_COMMAND  ${CMAKE_COMMAND} -E copy ${GLEW_SB_SRC}/lib/${SB_GLEW_DIR}/glew32.lib ${CMAKE_INSTALL_PREFIX}/lib
      INSTALL_COMMAND  ${CMAKE_COMMAND} -E copy ${GLEW_SB_SRC}/bin/${SB_GLEW_DIR}/glew32.dll ${CMAKE_INSTALL_PREFIX}/bin/
      )

    message(STATUS "  Pre-built binaries of GLEW are used for MSVC")
  endif()

 set(_SB_GLEW_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_GLEW_LIBRARY ${SB_INSTALL_PREFIX}/lib/libGLEW.lib)
  elseif(UNIX)
    set(_SB_GLEW_LIBRARY ${SB_INSTALL_PREFIX}/lib/libGLEW${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

endif()
endif()
