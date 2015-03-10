message(STATUS "Setup GEOS ...")

set(proj GEOS)

set(DEFAULT_USE_SYSTEM_GEOS  OFF)

option(USE_SYSTEM_GEOS "  Use a system build of GEOS." ${DEFAULT_USE_SYSTEM_GEOS})
mark_as_advanced(USE_SYSTEM_GEOS)

if(USE_SYSTEM_GEOS)
  message(STATUS "  Using GEOS system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})

  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://download.osgeo.org/geos/geos-3.4.2.tar.bz2"
    URL_MD5 fc5df2d926eb7e67f988a43a92683bae
    SOURCE_DIR ${GEOS_SB_SRC}
    BINARY_DIR ${GEOS_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DBUILD_SHARED_LIBS:BOOL=ON
      -DBUILD_TESTING:BOOL=OFF
      -DGEOS_ENABLE_TESTS:BOOL=OFF
      CMAKE_COMMAND ${SB_CMAKE_COMMAND}  
  )
  
  message(STATUS "  Using GEOS SuperBuild version")
  
 endif()
