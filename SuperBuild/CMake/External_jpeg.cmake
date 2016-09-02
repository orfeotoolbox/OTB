INCLUDE_ONCE_MACRO(JPEG)

SETUP_SUPERBUILD(JPEG)

#CMake build is offically unsupported by JPEG turbo developers.

if(WIN32)
  set(JPEG_CONFIGURE_COMMAND
    ${SB_CMAKE_COMMAND}
    -DENABLE_SHARED=TRUE
    -DENABLE_STATIC=FALSE
    -DWITH_SIMD=FALSE
    -DWITH_TURBOJPEG=FALSE
    -DWITH_ARITH_DEC=TRUE
    -DWITH_JAVA=FALSE
    ${SB_CMAKE_CACHE_ARGS}
    ${JPEG_SB_SRC}
    )

else()
  set(JPEG_CONFIGURE_COMMAND  "${SB_ENV_CONFIGURE_CMD};LDFLAGS='-Wl,--rpath -Wl,${SB_INSTALL_PREFIX}/lib';${JPEG_SB_SRC}/configure"
    --prefix=${SB_INSTALL_PREFIX}
    --enable-shared=yes
    --enable-static=false
    --without-simd
    --without-turbojpeg
    --with-arith-dec
    --without-java
    )
endif()

ExternalProject_Add(JPEG
  PREFIX JPEG
  URL "http://sourceforge.net/projects/libjpeg-turbo/files/1.4.1/libjpeg-turbo-1.4.1.tar.gz"
  URL_MD5 b1f6b84859a16b8ebdcda951fa07c3f2
  SOURCE_DIR ${JPEG_SB_SRC}
  BINARY_DIR ${JPEG_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CONFIGURE_COMMAND ${JPEG_CONFIGURE_COMMAND}
  BUILD_COMMAND
  )

SUPERBUILD_PATCH_SOURCE(JPEG)

SUPERBUILD_UPDATE_CMAKE_VARIABLES(JPEG FALSE)
