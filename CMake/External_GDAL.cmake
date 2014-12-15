message(STATUS "Setup GDAL...")

set(proj GDAL)

set(DEFAULT_USE_SYSTEM_GDAL  OFF)

option(USE_SYSTEM_GDAL "  Use a system build of GDAL." ${DEFAULT_USE_SYSTEM_GDAL})
mark_as_advanced(USE_SYSTEM_GDAL)

if(USE_SYSTEM_GDAL)
  message(STATUS "  Using GDAL system version")
else()
  set(${proj}_DEPENDENCIES)
  set(GDAL_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(GDAL_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  if(USE_SYSTEM_TIFF)
    set(GDAL_SB_TIFF_CONFIG)
  else()
    set(GDAL_SB_TIFF_CONFIG 
        --with-libtiff=${CMAKE_INSTALL_PREFIX}
      )
    list(APPEND ${proj}_DEPENDENCIES TIFF)
  endif()
  
  if(USE_SYSTEM_GEOTIFF)
    set(GDAL_SB_GEOTIFF_CONFIG)
  else()
    set(GDAL_SB_GEOTIFF_CONFIG 
      --with-geotiff=${CMAKE_INSTALL_PREFIX}
      )
    list(APPEND ${proj}_DEPENDENCIES GEOTIFF)
  endif()
  
  if(USE_SYSTEM_OPENJPEG)
    set(GDAL_SB_OPENJPEG_CONFIG)
  else()
    set(GDAL_SB_OPENJPEG_CONFIG 
      --with-openjpeg=${CMAKE_INSTALL_PREFIX}
      )
    list(APPEND ${proj}_DEPENDENCIES OPENJPEG)
  endif()
  
  if(USE_SYSTEM_SQLITE)
    set(GDAL_SB_SQLITE_CONFIG)
  else()
    set(GDAL_SB_SQLITE_CONFIG 
      --with-sqlite3=${CMAKE_INSTALL_PREFIX}
      )
    list(APPEND ${proj}_DEPENDENCIES SQLITE)
  endif()
  
  if(USE_SYSTEM_GEOS)
    set(GDAL_SB_GEOS_CONFIG)
  else()
    set(GDAL_SB_GEOS_CONFIG 
      --with-geos=${CMAKE_INSTALL_PREFIX}/bin/geos-config
      )
    list(APPEND ${proj}_DEPENDENCIES GEOS)
  endif()
  
  if(USE_SYSTEM_ZLIB)
    set(GDAL_SB_ZLIB_CONFIG)
  else()
    # Not shure this setting works with zlib
    set(GDAL_SB_ZLIB_CONFIG 
      --with-libz=${CMAKE_INSTALL_PREFIX}
      )
    list(APPEND ${proj}_DEPENDENCIES ZLIB)
  endif()
  
  if(WIN32)
    # TODO : use nmake
  else()
    
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://download.osgeo.org/gdal/1.11.0/gdal-1.11.0.tar.gz"
      URL_MD5 9fdf0f2371a3e9863d83e69951c71ec4
      BINARY_DIR ${GDAL_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      CONFIGURE_COMMAND 
        LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib 
        ${GDAL_SB_BUILD_DIR}/configure 
        --prefix=${CMAKE_INSTALL_PREFIX}
        --enable-static=no
        --without-ogdi
        --without-jasper
        ${GDAL_SB_ZLIB_CONFIG}
        ${GDAL_SB_TIFF_CONFIG}
        ${GDAL_SB_GEOTIFF_CONFIG}
        ${GDAL_SB_OPENJPEG_CONFIG}
        ${GDAL_SB_SQLITE_CONFIG}
        ${GDAL_SB_GEOS_CONFIG}
      BUILD_COMMAND make
      INSTALL_COMMAND make install
      DEPENDS ${${proj}_DEPENDENCIES}
      PATCH_COMMAND ${CMAKE_COMMAND} -E touch ${GDAL_SB_SRC}/config.rpath
      )
    
    ExternalProject_Add_Step(${proj} copy_source
      COMMAND ${CMAKE_COMMAND} -E copy_directory 
        ${GDAL_SB_SRC} ${GDAL_SB_BUILD_DIR}
      DEPENDEES patch update
      DEPENDERS configure
      )
    
  endif()
  
  
  message(STATUS "  Using GDAL SuperBuild version")
endif()
