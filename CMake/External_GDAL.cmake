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

  if(USE_SYSTEM_PNG)
    set(GDAL_SB_PNG_CONFIG)
  else()
    set(GDAL_SB_PNG_CONFIG 
      --with-png=${CMAKE_INSTALL_PREFIX}
      )
    list(APPEND ${proj}_DEPENDENCIES PNG)
  endif()

  # if(USE_SYSTEM_JPEG)
    # set(GDAL_SB_JPEG_CONFIG)
  # else()
    # set(GDAL_SB_JPEG_CONFIG 
      # --with-jpeg=${CMAKE_INSTALL_PREFIX}
      # )
    # list(APPEND ${proj}_DEPENDENCIES JPEG)
  # endif()  
  
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
  
    if(USE_SYSTEM_CURL)
    set(GDAL_SB_CURL_CONFIG)
  else()
    set(GDAL_SB_CURL_CONFIG 
      --with-curl=${CMAKE_INSTALL_PREFIX}
      )
    list(APPEND ${proj}_DEPENDENCIES CURL)
  endif()
  
  if(UNIX)
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://download.osgeo.org/gdal/1.11.0/gdal-1.11.0.tar.gz"
      URL_MD5 9fdf0f2371a3e9863d83e69951c71ec4
      BINARY_DIR ${GDAL_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      DEPENDS ${${proj}_DEPENDENCIES}
      UPDATE_COMMAND  ${CMAKE_COMMAND} -E copy_directory ${GDAL_SB_SRC} ${GDAL_SB_BUILD_DIR}        
      PATCH_COMMAND ${CMAKE_COMMAND} -E touch ${GDAL_SB_SRC}/config.rpath      
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
        ${GDAL_SB_PNG_CONFIG}        
        ${GDAL_SB_OPENJPEG_CONFIG}
        ${GDAL_SB_SQLITE_CONFIG}
        ${GDAL_SB_GEOS_CONFIG}
        ${GDAL_SB_EXPAT_CONFIG}
        ${GDAL_SB_LIBKML_CONFIG}
      BUILD_COMMAND $(MAKE)
      INSTALL_COMMAND $(MAKE) install
    )

  else(MSVC)
  ##add libkml
    list(REMOVE_ITEM ${proj}_DEPENDENCIES LIBKML)
    
    STRING(REGEX REPLACE "/" "\\\\" CMAKE_WIN_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
    configure_file(${CMAKE_SOURCE_DIR}/patches/${proj}/nmake_gdal_extra.opt.in ${CMAKE_BINARY_DIR}/nmake_gdal_extra.opt)
      
    ExternalProject_Add(${proj}_build
       PREFIX ${proj}
       URL "http://download.osgeo.org/gdal/1.11.0/gdal-1.11.0.tar.gz"
       URL_MD5 9fdf0f2371a3e9863d83e69951c71ec4
       BINARY_DIR ${GDAL_SB_BUILD_DIR}
       INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
       DEPENDS ${${proj}_DEPENDENCIES}
       PATCH_COMMAND  ${CMAKE_COMMAND} -E copy_directory  ${GDAL_SB_SRC} ${GDAL_SB_BUILD_DIR}
       CONFIGURE_COMMAND ""
       BUILD_COMMAND ${SB_MAKE_CMD} /f ${GDAL_SB_BUILD_DIR}/Makefile.vc MSVC_VER=${SB_MSVC_VER} EXT_NMAKE_OPT=${CMAKE_BINARY_DIR}/nmake_gdal_extra.opt 
       INSTALL_COMMAND ${CMAKE_COMMAND} -E copy  ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt
      ${CMAKE_BINARY_DIR}/${proj}/_install
    )
    
    ExternalProject_Add(${proj}
      PREFIX ${proj}/_install
      DOWNLOAD_COMMAND ""
      SOURCE_DIR ${proj}/_install
      BINARY_DIR ${GDAL_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DGDAL_INCLUDE_DIR:STRING=${GDAL_SB_BUILD_DIR}
        -DGDAL_LIB_DIR:STRING=${GDAL_SB_BUILD_DIR}
      DEPENDS ${proj}_build
      CMAKE_COMMAND
    )    
    
  endif()
  
  message(STATUS "  Using GDAL SuperBuild version")
  
endif()
