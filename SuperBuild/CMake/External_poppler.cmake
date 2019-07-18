INCLUDE_ONCE_MACRO(POPPLER)
SETUP_SUPERBUILD(POPPLER)
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(POPPLER ZLIB ICONV FREETYPE LCMS)

ExternalProject_Add(POPPLER
  PREFIX POPPLER
  URL "http://poppler.freedesktop.org/poppler-0.31.0.tar.xz"
  URL_MD5 b631498668562ba47732785e54808468
  SOURCE_DIR ${POPPLER_SB_SRC}
  BINARY_DIR ${POPPLER_SB_BUILD_DIR}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  CMAKE_CACHE_ARGS 
  ${SB_CMAKE_CACHE_ARGS}
  -DBUILD_CPP_TESTS:BOOL=OFF
  -DBUILD_GTK_TESTS:BOOL=OFF
  -DBUILD_QT5_TESTS:BOOL=OFF
  -DENABLE_QT5:BOOL=OFF
  -DENABLE_SPLASH:BOOL=ON
  -DWINDOWS_EXPORT_ALL_SYMBOLS:BOOL=ON
  -DENABLE_GLIB:BOOL=OFF
  -DENABLE_UTILS:BOOL=OFF
  -DENABLE_XPDF_HEADERS:BOOL=ON
  -DENABLE_RELOCATABLE:BOOL=OFF
  DEPENDS ${POPPLER_DEPENDENCIES}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 0
  LOG_BUILD 0
  LOG_INSTALL 1
  )

SUPERBUILD_PATCH_SOURCE(POPPLER)