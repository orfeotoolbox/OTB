set(proj FREETYPE)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup freetype ...")

if(USE_SYSTEM_FREETYPE)
  find_package ( FreeType )
  message(STATUS "  Using FreeType system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using FreeType SuperBuild version")
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://download.savannah.gnu.org/releases/freetype/freetype-2.6.1.tar.gz"
    URL_MD5 348e667d728c597360e4a87c16556597
    BINARY_DIR ${FREETYPE_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DBUILD_SHARED_LIBS:BOOL=ON
    CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    )
  
  set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/freetype.lib)
  elseif(UNIX)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libfreetype${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

endif()
endif()
