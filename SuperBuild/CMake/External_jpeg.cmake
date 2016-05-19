if( __EXTERNAL_JPEG__)
  return()
else()
  set(__EXTERNAL_JPEG__ 1)
endif()

if(USE_SYSTEM_JPEG)
  message(STATUS "  Using libjpeg system version")
  return()
endif()

SETUP_SUPERBUILD(PROJECT JPEG)
message(STATUS "  Using libjpeg SuperBuild version")

if(WIN32)
  set(JPEG_PATCH_COMMAND ${CMAKE_COMMAND}
    -E copy
    ${CMAKE_SOURCE_DIR}/patches/JPEG/jconfigint.h.in
    ${JPEG_SB_SRC}/win/)

else()
  set(JPEG_PATCH_COMMAND)
endif()

ExternalProject_Add(JPEG
  PREFIX JPEG
  URL "http://sourceforge.net/projects/libjpeg-turbo/files/1.4.1/libjpeg-turbo-1.4.1.tar.gz"
  URL_MD5 b1f6b84859a16b8ebdcda951fa07c3f2
  SOURCE_DIR ${JPEG_SB_SRC}
  BINARY_DIR ${JPEG_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CONFIGURE_COMMAND ${SB_CMAKE_COMMAND}
    -DCMAKE_BUILD_TYPE=Release
    -DENABLE_SHARED=TRUE
    -DENABLE_STATIC=FALSE
    -DWITH_SIMD=FALSE
    -DWITH_TURBOJPEG=FALSE
    -DWITH_ARITH_DEC=TRUE
    -DWITH_JAVA=FALSE
    -DCMAKE_INSTALL_PREFIX=${SB_INSTALL_PREFIX}
    ${JPEG_SB_SRC}
    PATCH_COMMAND ${JPEG_PATCH_COMMAND}
  )

SUPERBUILD_PATCH_SOURCE(JPEG "patch-for-unix")

set(_SB_JPEG_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
if(WIN32)
  set(_SB_JPEG_LIBRARY ${SB_INSTALL_PREFIX}/lib/jpeg.lib)
elseif(UNIX)
  set(_SB_JPEG_LIBRARY ${SB_INSTALL_PREFIX}/lib/libjpeg${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()
