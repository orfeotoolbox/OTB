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
      -DTIFF_LIBRARY:STRING=${CMAKE_INSTALL_PREFIX}/lib/libtiff.so
      )
    list(APPEND ${proj}_DEPENDENCIES TIFF)
  endif()
  
  if(USE_SYSTEM_GEOTIFF)
    set(OSSIM_SB_GEOTIFF_CONFIG)
  else()
    set(OSSIM_SB_GEOTIFF_CONFIG 
      -DGEOTIFF_INCLUDE_DIR:STRING=${CMAKE_INSTALL_PREFIX}/include
      -DGEOTIFF_LIBRARY:STRING=${CMAKE_INSTALL_PREFIX}/lib/libgeotiff.so
      )
    list(APPEND ${proj}_DEPENDENCIES GEOTIFF)
  endif()
  
  if(USE_SYSTEM_GEOS)
    set(OSSIM_SB_GEOS_CONFIG)
  else()
    set(OSSIM_SB_GEOS_CONFIG
      -DGEOS_LIBRARY:STRING=${CMAKE_INSTALL_PREFIX}/lib/libgeos.so
      -DGEOS_INCLUDE_DIR:STRING=${CMAKE_INSTALL_PREFIX}/include/geos
      )
    list(APPEND ${proj}_DEPENDENCIES GEOS)
  endif()
  
  if(USE_SYSTEM_JPEG)
    set(OSSIM_SB_JPEG_CONFIG)
  else()
    set(OSSIM_SB_JPEG_CONFIG 
      -DJPEG_INCLUDE_DIR:STRING=${CMAKE_INSTALL_PREFIX}/include
      -DJPEG_LIBRARY:STRING=${CMAKE_INSTALL_PREFIX}/lib/libjpeg.so
      )
    list(APPEND ${proj}_DEPENDENCIES JPEG)
  endif()
  
  if(USE_SYSTEM_OPENTHREAD)
    set(OSSIM_SB_OPENTHREAD_CONFIG)
  else()
    set(OSSIM_SB_OPENTHREAD_CONFIG 
      -DOPENTHREADS_INCLUDE_DIR:STRING=${CMAKE_INSTALL_PREFIX}/include
      -DOPENTHREADS_LIBRARY:STRING=${CMAKE_INSTALL_PREFIX}/lib/libOpenThreads.so
      )
    list(APPEND ${proj}_DEPENDENCIES OPENTHREAD)
  endif()

    ExternalProject_Add(ossim_cmakemodules
    PREFIX ${proj}/_cmakemodules
    SVN_REPOSITORY "http://svn.osgeo.org/ossim/trunk/ossim_package_support/cmake/CMakeModules"
    SVN_REVISION -r 23087
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND "")
    
  set(OSSIM_CXX_FLAGS  -D__STDC_CONSTANT_MACROS)
  if(MSVC)
    #checkout cmakeModules 
    set(${proj}_DEPENDENCIES ossim_cmakemodules)
     list(APPEND ${proj}_DEPENDENCIES OPENTHREAD)
    #cmake flags for exception handling in c++    
    set(OSSIM_CXX_FLAGS /EHsc)
    #clear all dependent library settings except GEOTIFF as we link with gdal_i. 
    #This is because of gisinternals built with internal geotiff
    set(OSSIM_SB_TIFF_CONFIG)
    set(OSSIM_SB_GEOS_CONFIG)
    set(OSSIM_SB_JPEG_CONFIG)
    set(OSSIM_SB_OPENTHREAD_CONFIG)
    #build geotiff and use it
    set(OSSIM_SB_GEOTIFF_CONFIG -DGEOTIFF_LIBRARY:STRING=${CMAKE_INSTALL_PREFIX}/lib/gdal_i.lib)
  endif()
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    SVN_REPOSITORY "http://svn.osgeo.org/ossim/trunk/ossim/"
    SVN_REVISION -r 23087
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
      ${OSSIM_SB_OPENTHREAD_CONFIG}
      -DBUILD_OSSIM_MPI_SUPPORT:BOOL=OFF
    DEPENDS ${${proj}_DEPENDENCIES}
    CMAKE_COMMAND
      # use 'env' because CTest launcher doesn't perform shell interpretation
      env LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib
      ${CMAKE_COMMAND}
    )
    
    ExternalProject_Add_Step(${proj} copy_CMakeModules
      COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${CMAKE_BINARY_DIR}/${proj}/_cmakemodules/src/ossim_cmakemodules ${OSSIM_SB_SRC}/CMakeModules
      DEPENDEES patch update
      DEPENDERS configure
    )  
 
  message(STATUS "  Using OSSIM SuperBuild version")
endif()

