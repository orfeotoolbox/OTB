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
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://download.osgeo.org/ossim/source/latest/ossim-1.8.18-1.tar.gz"
    URL_MD5 c61e77f3fab08df0e486367eb4365c91
    BINARY_DIR ${OSSIM_SB_BUILD_DIR}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
      -DOSSIM_DEV_HOME:STRING=${OSSIM_SB_SRC}
      -DOSSIM_VERSION:STRING=1.8.18-1
      -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DBUILD_CSMAPI:BOOL=OFF
      -DBUILD_OSSIMCSM_PLUGIN:STRING=OFF
      -DCMAKE_CXX_FLAGS:STRING=-D__STDC_CONSTANT_MACROS
      -DWMS_INCLUDE_DIR:STRING=${OSSIM_SB_SRC}/libwms/include
      ${OSSIM_SB_TIFF_CONFIG}
      ${OSSIM_SB_GEOTIFF_CONFIG}
      ${OSSIM_SB_GEOS_CONFIG}
      ${OSSIM_SB_JPEG_CONFIG}
      -DBUILD_OSSIMPREDATOR:BOOL=OFF
      -DBUILD_OSSIMPLANET:BOOL=OFF
      -DBUILD_OSSIMPLANETQT:BOOL=OFF
      -DBUILD_OSSIM_MPI_SUPPORT:BOOL=OFF
      -DBUILD_OSSIMQT4:BOOL=OFF
      -DBUILD_OSSIMGUI:BOOL=OFF
      -DBUILD_OMS:BOOL=OFF
      -DBUILD_WMS:BOOL=OFF
      -DCMAKE_MODULE_PATH:PATH=${OSSIM_SB_SRC}/ossim_package_support/cmake/CMakeModules
    PATCH_COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/ossim_patch_1.cmake
    DEPENDS ${${proj}_DEPENDENCIES}
    CMAKE_COMMAND
      LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib
      ${CMAKE_COMMAND}
    )
  
  # Write patch file in binary dir
  file(WRITE ${CMAKE_BINARY_DIR}/ossim_patch_1.cmake 
  "file(WRITE ${OSSIM_SB_SRC}/CMakeLists.txt \"add_subdirectory(ossim_package_support/cmake)\")")
  
  message(STATUS "  Using OSSIM SuperBuild version")
endif()

