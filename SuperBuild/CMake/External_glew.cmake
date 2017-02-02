INCLUDE_ONCE_MACRO(GLEW)

SETUP_SUPERBUILD(GLEW)

ExternalProject_Add(GLEW
  PREFIX GLEW
  URL "https://sourceforge.net/projects/glew/files/glew/1.13.0/glew-1.13.0.tgz/download"
  URL_MD5 7cbada3166d2aadfc4169c4283701066
  SOURCE_DIR ${GLEW_SB_SRC}
  BINARY_DIR ${GLEW_SB_BUILD_DIR}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  CONFIGURE_COMMAND ${SB_CMAKE_COMMAND} ${SB_CMAKE_CACHE_ARGS} -DBUILD_UTILS:BOOL=OFF ${GLEW_SB_SRC}/build/cmake/
  )

SUPERBUILD_PATCH_SOURCE(GLEW)

set(_SB_GLEW_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
if(WIN32)
  set(_SB_GLEW_LIBRARY ${SB_INSTALL_PREFIX}/lib/glew32.lib)
elseif(UNIX)
  set(_SB_GLEW_LIBRARY ${SB_INSTALL_PREFIX}/lib/libGLEW${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()
