if( __EXTERNAL_FREETYPE__)
  return()
else()
  set(__EXTERNAL_FREETYPE__ 1)
endif()

if(USE_SYSTEM_FREETYPE)
  message(STATUS "  Using FreeType system version")
  return()
endif()


SETUP_SUPERBUILD(PROJECT FREETYPE)
message(STATUS "  Using FreeType SuperBuild version")

set(FREETYPE_SB_CONFIG)
if(MSVC AND BUILD_SHARED_LIBS)
  list(APPEND FREETYPE_SB_CONFIG
    -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS:BOOL=TRUE)
endif()

ExternalProject_Add(FREETYPE
  PREFIX FREETYPE
  URL "http://download.savannah.gnu.org/releases/freetype/freetype-2.6.tar.gz"
  URL_MD5 1d733ea6c1b7b3df38169fbdbec47d2b
  BINARY_DIR ${FREETYPE_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND} ${FREETYPE_SB_CONFIG}
  )

set(_SB_FREETYPE_INCLUDE_DIRS ${SB_INSTALL_PREFIX}/include/freetype2)
if(WIN32)
  set(_SB_FREETYPE_LIBRARY ${SB_INSTALL_PREFIX}/lib/freetype.lib)
elseif(UNIX)
  set(_SB_FREETYPE_LIBRARY ${SB_INSTALL_PREFIX}/lib/libfreetype${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()
