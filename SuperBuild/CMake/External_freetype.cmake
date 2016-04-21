if(NOT __EXTERNAL_FREETYPE__)
set(__EXTERNAL_FREETYPE__ 1)

if(USE_SYSTEM_FREETYPE)
  find_package ( FreeType )
  message(STATUS "  Using FreeType system version")
else()
  SETUP_SUPERBUILD(PROJECT FREETYPE)
  message(STATUS "  Using FreeType SuperBuild version")

  ExternalProject_Add(FREETYPE
    PREFIX FREETYPE
    URL "http://download.savannah.gnu.org/releases/freetype/freetype-2.6.tar.gz"
    URL_MD5 1d733ea6c1b7b3df38169fbdbec47d2b
    BINARY_DIR ${FREETYPE_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DBUILD_SHARED_LIBS:BOOL=ON
    CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    )

  set(_SB_FREETYPE_INCLUDE_DIRS ${SB_INSTALL_PREFIX}/include/freetype2)
  if(WIN32)
    set(_SB_FREETYPE_LIBRARY ${SB_INSTALL_PREFIX}/lib/freetype.lib)
  elseif(UNIX)
    set(_SB_FREETYPE_LIBRARY ${SB_INSTALL_PREFIX}/lib/libfreetype${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

endif()
endif()
