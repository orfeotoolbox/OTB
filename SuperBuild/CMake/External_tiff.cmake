set(proj TIFF)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup libtiff...")

if(USE_SYSTEM_TIFF)
  find_package ( TIFF )
  message(STATUS "  Using libtiff system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using libtiff SuperBuild version")

  # declare dependencies
  ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(${proj} ZLIB JPEG)

  INCLUDE_SUPERBUILD_DEPENDENCIES(${${proj}_DEPENDENCIES})
  # set proj back to its original value
  set(proj TIFF)

  set(TIFF_SB_CONFIG)
  if(NOT USE_SYSTEM_ZLIB)
    list(APPEND TIFF_SB_CONFIG "--with-zlib-include-dir=${SB_INSTALL_PREFIX}/include")
    list(APPEND TIFF_SB_CONFIG "--with-zlib-lib-dir=${SB_INSTALL_PREFIX}/lib")
  endif()
  if(NOT USE_SYSTEM_JPEG)
    list(APPEND TIFF_SB_CONFIG "--with-jpeg-include-dir=${SB_INSTALL_PREFIX}/include")
    list(APPEND TIFF_SB_CONFIG "--with-jpeg-lib-dir=${SB_INSTALL_PREFIX}/lib")
  endif()

  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://download.osgeo.org/libtiff/tiff-4.0.6.tar.gz"
    URL_MD5 d1d2e940dea0b5ad435f21f03d96dd72
    SOURCE_DIR ${TIFF_SB_SRC}
    BINARY_DIR ${TIFF_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    DEPENDS ${${proj}_DEPENDENCIES}
    CMAKE_CACHE_ARGS
    -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
    -DCMAKE_BUILD_TYPE:STRING=Release
    -DBUILD_SHARED_LIBS:BOOL=ON
    -DBUILD_TESTING:BOOL=OFF
    CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    )

  set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libtiff_i.lib)
  elseif(UNIX)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libtiff${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

endif()
endif()
