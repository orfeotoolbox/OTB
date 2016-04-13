if(NOT __EXTERNAL_GEOS__)
set(__EXTERNAL_GEOS__ 1)

message(STATUS "Setup GEOS ...")

if(USE_SYSTEM_GEOS)
  # TODO : FindGEOS.cmake
  find_package ( GEOS )
  message(STATUS "  Using GEOS system version")
else()
  SETUP_SUPERBUILD(PROJECT GEOS)
  message(STATUS "  Using GEOS SuperBuild version")

  ExternalProject_Add(GEOS
    PREFIX GEOS
    URL "http://download.osgeo.org/geos/geos-3.4.2.tar.bz2"
    URL_MD5 fc5df2d926eb7e67f988a43a92683bae
    SOURCE_DIR ${GEOS_SB_SRC}
    BINARY_DIR ${GEOS_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DBUILD_SHARED_LIBS:BOOL=ON
      -DBUILD_TESTING:BOOL=OFF
      -DGEOS_ENABLE_TESTS:BOOL=OFF
      CMAKE_COMMAND ${SB_CMAKE_COMMAND} )

    ExternalProject_Add_Step(GEOS remove_static
      COMMAND ${CMAKE_COMMAND} -E remove
      ${SB_INSTALL_PREFIX}/lib/libgeos.a
      DEPENDEES install)

  set(_SB_GEOS_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_GEOS_LIBRARY ${SB_INSTALL_PREFIX}/lib/geos.lib)
  elseif(UNIX)
    set(_SB_GEOS_LIBRARY ${SB_INSTALL_PREFIX}/lib/libgeos${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

 endif()
endif()
