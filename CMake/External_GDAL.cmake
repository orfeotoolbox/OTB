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
  
  if(USE_SYSTEM_EXPAT)
    set(GDAL_SB_EXPAT_CONFIG)
  else()
    set(GDAL_SB_EXPAT_CONFIG 
      --with-expat=${CMAKE_INSTALL_PREFIX}
      )
    list(APPEND ${proj}_DEPENDENCIES EXPAT)
  endif()
  
  if(USE_SYSTEM_LIBKML)
    set(GDAL_SB_LIBKML_CONFIG)
  else()
    set(GDAL_SB_LIBKML_CONFIG 
      --with-libkml=${CMAKE_INSTALL_PREFIX}
      )
    list(APPEND ${proj}_DEPENDENCIES LIBKML)
  endif()
  
  if(UNIX)
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://download.osgeo.org/gdal/1.11.0/gdal-1.11.0.tar.gz"
      URL_MD5 9fdf0f2371a3e9863d83e69951c71ec4
      BINARY_DIR ${GDAL_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      CONFIGURE_COMMAND 
        # use 'env' because CTest launcher doesn't perform shell interpretation
        env LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib 
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
        ${GDAL_SB_EXPAT_CONFIG}
        ${GDAL_SB_LIBKML_CONFIG}
        BUILD_COMMAND $(MAKE)
        INSTALL_COMMAND $(MAKE) install
        DEPENDS ${${proj}_DEPENDENCIES}
        PATCH_COMMAND ${CMAKE_COMMAND} -E touch ${GDAL_SB_SRC}/config.rpath
    )
    
    ExternalProject_Add_Step(${proj} copy_source
        COMMAND ${CMAKE_COMMAND} -E copy_directory 
        ${GDAL_SB_SRC} ${GDAL_SB_BUILD_DIR}
        DEPENDEES patch update
        DEPENDERS configure
    )
    
  else(WIN32)
    if(MSVC10)
        set(URL_PREFIX "release-1600")
        set(GDAL_LIBS_URL_HASH 2a3e4db48c68d490e3101b5ca98a4bce)
        set(GDAL_DLL_URL_HASH cc5f45081ec0d0a7604816fabf5f2588)
    else(MSVC80)
         set(URL_PREFIX "release-1500")    
    else(CMAKE_COMPILER_2005)
        set(URL_PREFIX "release-1400")
    endif()
        
    set(GDAL_LIBS_URL "http://download.gisinternals.com/sdk/downloads/${URL_PREFIX}-gdal-1-11-1-mapserver-6-4-1-libs.zip")
    set(GDAL_DLL_URL "http://download.gisinternals.com/sdk/downloads/${URL_PREFIX}-gdal-1-11-1-mapserver-6-4-1.zip")
        
    #TODO: find hash for archives properly
    set(GDAL_LIBS_URL_HASH 2a3e4db48c68d490e3101b5ca98a4bce)
    set(GDAL_DLL_URL_HASH cc5f45081ec0d0a7604816fabf5f2588)

    ExternalProject_Add(${proj}_BIN
        PREFIX ${proj}/_BIN
        URL ${GDAL_DLL_URL}
        URL_MD5 ${GDAL_DLL_URL_HASH}
        SOURCE_DIR ${GDAL_SB_SRC}
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory ${GDAL_SB_SRC} ${CMAKE_INSTALL_PREFIX}
     )
     
    ExternalProject_Add(${proj}
        PREFIX ${proj}
        URL ${GDAL_LIBS_URL}
        URL_MD5 ${GDAL_LIBS_URL_HASH} 
        SOURCE_DIR ${GDAL_SB_SRC}
        DEPENDS ${proj}_BIN
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory  ${GDAL_SB_SRC} ${CMAKE_INSTALL_PREFIX}
    )
        
    ExternalProject_Add_Step(${proj} remove_libkml_headers
        COMMAND ${CMAKE_COMMAND} -E remove_directory
        ${CMAKE_INSTALL_PREFIX}/include/kml
        DEPENDEES install
    )
    
    foreach(KMLLIB "base" "convenience" "regionator" "dom" "engine" "xsd")
        ExternalProject_Add_Step(${proj} remove_libkml_${KMLLIB}
        COMMAND ${CMAKE_COMMAND} -E remove 
        ${CMAKE_INSTALL_PREFIX}/lib/libkml${KMLLIB}.lib
        DEPENDEES remove_libkml_headers
    )
    endforeach()


  endif()
  
  list(APPEND OTB_DEPS ${proj})
  message(STATUS "  Using GDAL SuperBuild version")
  
  
endif()
