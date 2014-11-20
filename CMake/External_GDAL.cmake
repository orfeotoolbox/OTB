message(STATUS "Setup GDAL...")

set(proj GDAL)

set(DEFAULT_USE_SYSTEM_GDAL  OFF)

option(USE_SYSTEM_GDAL "  Use a system build of GDAL." ${DEFAULT_USE_SYSTEM_GDAL})
mark_as_advanced(USE_SYSTEM_GDAL)

if(USE_SYSTEM_GDAL)
  message(STATUS "  Using GDAL system version")
else()
  
  set(GDAL_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(GDAL_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  if(WIN32)
    # TODO : use nmake
  else()
    
    ExternalProject_Add(${proj}
      DEPENDS OPENJPEG TIFF
      PREFIX ${proj}
      URL "http://download.osgeo.org/gdal/1.11.0/gdal-1.11.0.tar.gz"
      URL_MD5 9fdf0f2371a3e9863d83e69951c71ec4
      BINARY_DIR ${GDAL_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      CONFIGURE_COMMAND ${GDAL_SB_BUILD_DIR}/configure 
        --prefix=${CMAKE_INSTALL_PREFIX}
        --with-libtiff=${CMAKE_INSTALL_PREFIX}
        --with-geotiff=${CMAKE_INSTALL_PREFIX}
        #--with-rename-internal-libtiff-symbols=yes
        #--with-rename-internal-libgeotiff-symbols=yes
        --with-openjpeg=${CMAKE_INSTALL_PREFIX}
        --without-ogdi
        --without-jasper
      BUILD_COMMAND make
      INSTALL_COMMAND make install
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
