message(STATUS "Setup Ossim...")

set(proj OSSIM)

set(DEFAULT_USE_SYSTEM_OSSIM  OFF)

# OSGeo4W provides an "ossim" package : use it otherwise if it is installed and not used by OTB
# we get conflicts (because OSGeo4W include dir is included for other dependencies
if(WIN32)
  set(DEFAULT_USE_SYSTEM_OSSIM  ON)
endif()

option(USE_SYSTEM_OSSIM "  Use a system build of Ossim." ${DEFAULT_USE_SYSTEM_OSSIM})
mark_as_advanced(USE_SYSTEM_OSSIM)

if(USE_SYSTEM_OSSIM)
  message(STATUS "  Using OSSIM system version")
else()
  set(${proj}_DEPENDENCIES)
  set(OSSIM_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(OSSIM_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})

  
  # set project dependencies (GEOS, GDAL, TIFF, JPEG, OPENTHREADS, )
  if(USE_SYSTEM_TIFF)
    set(OSSIM_SB_TIFF_CONFIG)
  else()
    set(OSSIM_SB_TIFF_CONFIG 
      -DTIFF_INCLUDE_DIR:STRING=${CMAKE_INSTALL_PREFIX}/include 
      -DTIFF_LIBRARY:STRING=${CMAKE_INSTALL_PREFIX}/lib/libtiff${CMAKE_SHARED_LIBRARY_SUFFIX}
      )
    list(APPEND ${proj}_DEPENDENCIES TIFF)
  endif()
  
  if(USE_SYSTEM_GEOTIFF)
    set(OSSIM_SB_GEOTIFF_CONFIG)
  else()
    set(OSSIM_SB_GEOTIFF_CONFIG 
      -DGEOTIFF_INCLUDE_DIR:STRING=${CMAKE_INSTALL_PREFIX}/include
      -DGEOTIFF_LIBRARY:STRING=${CMAKE_INSTALL_PREFIX}/lib/libgeotiff${CMAKE_SHARED_LIBRARY_SUFFIX}
      )
    list(APPEND ${proj}_DEPENDENCIES GEOTIFF)
  endif()
  
  if(USE_SYSTEM_GEOS)
    set(OSSIM_SB_GEOS_CONFIG)
  else()
    set(OSSIM_SB_GEOS_CONFIG
      -DGEOS_LIBRARY:STRING=${CMAKE_INSTALL_PREFIX}/lib/libgeos${CMAKE_SHARED_LIBRARY_SUFFIX}
      -DGEOS_INCLUDE_DIR:STRING=${CMAKE_INSTALL_PREFIX}/include/geos
      )
    list(APPEND ${proj}_DEPENDENCIES GEOS)
  endif()
  
  if(USE_SYSTEM_JPEG)
    set(OSSIM_SB_JPEG_CONFIG)
  else()
    set(OSSIM_SB_JPEG_CONFIG 
      -DJPEG_INCLUDE_DIR:STRING=${CMAKE_INSTALL_PREFIX}/include
      -DJPEG_LIBRARY:STRING=${CMAKE_INSTALL_PREFIX}/lib/libjpeg${CMAKE_SHARED_LIBRARY_SUFFIX}
      )
    list(APPEND ${proj}_DEPENDENCIES JPEG)
  endif()
  
  if(USE_SYSTEM_OPENTHREADS)
    set(OSSIM_SB_OPENTHREADS_CONFIG)
  else()
    set(OSSIM_SB_OPENTHREADS_CONFIG 
      -DOPENTHREADS_INCLUDE_DIR:STRING=${CMAKE_INSTALL_PREFIX}/include
      -DOPENTHREADS_LIBRARY:STRING=${CMAKE_INSTALL_PREFIX}/lib/libOpenThreads${CMAKE_SHARED_LIBRARY_SUFFIX}
      )
    list(APPEND ${proj}_DEPENDENCIES OPENTHREADS)
  endif()

  ExternalProject_Add(ossim_cmakemodules
    PREFIX ${proj}/_cmakemodules
    SVN_REPOSITORY "http://svn.osgeo.org/ossim/trunk/ossim_package_support/cmake/CMakeModules"
    SVN_REVISION -r 23087
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND "")
    
  list(APPEND ${proj}_DEPENDENCIES ossim_cmakemodules)

  set(OSSIM_CXX_FLAGS  -D__STDC_CONSTANT_MACROS)
  
  if(MSVC)
    set(OSSIM_CXX_FLAGS /EHsc)
    set(OSSIM_SB_TIFF_CONFIG)
    set(OSSIM_SB_GEOS_CONFIG)
    set(OSSIM_SB_JPEG_CONFIG)
    set(OSSIM_SB_OPENTHREADS_CONFIG)
    set(OSSIM_SB_GEOTIFF_CONFIG)
  endif()
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    SVN_REPOSITORY "http://svn.osgeo.org/ossim/trunk/ossim/"
    SVN_REVISION -r 23092
    BINARY_DIR ${OSSIM_SB_BUILD_DIR}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
      -DOSSIM_VERSION:STRING=1.8.18-1
      -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DCMAKE_CXX_FLAGS:STRING=${OSSIM_CXX_FLAGS}
      ${OSSIM_SB_TIFF_CONFIG}
      ${OSSIM_SB_GEOTIFF_CONFIG}
      ${OSSIM_SB_GEOS_CONFIG}
      ${OSSIM_SB_JPEG_CONFIG}
      ${OSSIM_SB_OPENTHREADS_CONFIG}
      -DBUILD_OSSIM_MPI_SUPPORT:BOOL=OFF
      -DBUILD_OSSIM_APPS:BOOL=OFF     
    DEPENDS ${${proj}_DEPENDENCIES}
    CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    )
    
    ExternalProject_Add_Step(${proj} copy_CMakeModules
      COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${CMAKE_BINARY_DIR}/${proj}/_cmakemodules/src/ossim_cmakemodules ${OSSIM_SB_SRC}/CMakeModules
      DEPENDEES patch update
      DEPENDERS configure
    )  
 
  message(STATUS "  Using OSSIM SuperBuild version")
endif()

