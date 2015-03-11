set(proj GDAL)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup GDAL...")

if(USE_SYSTEM_GDAL)
  find_package ( GDAL REQUIRED )
  add_custom_target(${proj})
  message(STATUS "  Using GDAL system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  
  if(USE_SYSTEM_TIFF)
    if(NOT SYSTEM_TIFF_PREFIX STREQUAL "")
      list(APPEND GDAL_SB_CONFIG --with-libtiff=${SYSTEM_TIFF_PREFIX})
    endif()
  else()
    list(APPEND GDAL_SB_CONFIG --with-libtiff=${SB_INSTALL_PREFIX})
    list(APPEND ${proj}_DEPENDENCIES TIFF)
  endif()
  
  if(USE_SYSTEM_GEOTIFF)
    # TODO : handle specific prefix
  else()
    list(APPEND GDAL_SB_CONFIG --with-geotiff=${SB_INSTALL_PREFIX})
    list(APPEND ${proj}_DEPENDENCIES GEOTIFF)
  endif()

  if(USE_SYSTEM_PNG)
    # TODO : handle specific prefix
  else()
    list(APPEND GDAL_SB_CONFIG --with-png=${SB_INSTALL_PREFIX})
    list(APPEND ${proj}_DEPENDENCIES PNG)
  endif()

  #gdal on msvc link with internal jpeg .
  if(USE_SYSTEM_JPEG)
    # TODO : handle specific prefix
  else()
    list(APPEND GDAL_SB_CONFIG --with-jpeg=${SB_INSTALL_PREFIX})
    list(APPEND ${proj}_DEPENDENCIES JPEG)
  endif()  
  
  if(USE_SYSTEM_OPENJPEG)
    # TODO : handle specific prefix
  else()
    list(APPEND GDAL_SB_CONFIG --with-openjpeg=${SB_INSTALL_PREFIX})
    list(APPEND ${proj}_DEPENDENCIES OPENJPEG)
  endif()
  
  if(USE_SYSTEM_SQLITE)
    # TODO : handle specific prefix
  else()
    list(APPEND GDAL_SB_CONFIG --with-sqlite3=${SB_INSTALL_PREFIX})
    list(APPEND ${proj}_DEPENDENCIES SQLITE)
  endif()
  
  if(USE_SYSTEM_GEOS)
    # TODO : handle specific prefix
  else()
    list(APPEND GDAL_SB_CONFIG --with-geos=${SB_INSTALL_PREFIX}/bin/geos-config)
    list(APPEND ${proj}_DEPENDENCIES GEOS)
  endif()
  
  if(USE_SYSTEM_ZLIB)
    # TODO : handle specific prefix
  else()
    # Not shure this setting works with zlib
    list(APPEND GDAL_SB_CONFIG --with-libz=${SB_INSTALL_PREFIX})
    list(APPEND ${proj}_DEPENDENCIES ZLIB)
  endif()
  
  if(USE_SYSTEM_EXPAT)
    # TODO : handle specific prefix
  else()
    list(APPEND GDAL_SB_CONFIG --with-expat=${SB_INSTALL_PREFIX})
    list(APPEND ${proj}_DEPENDENCIES EXPAT)
  endif()
  
  if(USE_SYSTEM_LIBKML)
    # TODO : handle specific prefix
  else()
    list(APPEND GDAL_SB_CONFIG --with-libkml=${SB_INSTALL_PREFIX})
    list(APPEND ${proj}_DEPENDENCIES LIBKML)
  endif()
  
  if(USE_SYSTEM_CURL)
    # TODO : handle specific prefix
  else()
    list(APPEND GDAL_SB_CONFIG --with-curl=${SB_INSTALL_PREFIX})
    list(APPEND ${proj}_DEPENDENCIES CURL)
  endif()
  
  if(UNIX)
    set(GDAL_SB_EXTRA_OPTIONS "" CACHE STRING "Extra options to be passed to GDAL configure script")
    mark_as_advanced(GDAL_SB_EXTRA_OPTIONS)
    
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://download.osgeo.org/gdal/1.11.0/gdal-1.11.0.tar.gz"
      URL_MD5 9fdf0f2371a3e9863d83e69951c71ec4
      BINARY_DIR ${GDAL_SB_BUILD_DIR}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      DEPENDS ${${proj}_DEPENDENCIES}
      UPDATE_COMMAND  ${CMAKE_COMMAND} -E copy_directory ${GDAL_SB_SRC} ${GDAL_SB_BUILD_DIR}        
      PATCH_COMMAND ${CMAKE_COMMAND} -E touch ${GDAL_SB_SRC}/config.rpath      
      CONFIGURE_COMMAND 
        # use 'env' because CTest launcher doesn't perform shell interpretation
        env ${LDLIBVAR}=${SB_INSTALL_PREFIX}/lib 
        ${GDAL_SB_BUILD_DIR}/configure 
        --prefix=${SB_INSTALL_PREFIX}
        --enable-static=no
        --without-ogdi
        --without-jasper
        ${GDAL_SB_CONFIG}
        ${GDAL_SB_EXTRA_OPTIONS}
      BUILD_COMMAND $(MAKE)
      INSTALL_COMMAND $(MAKE) install
    )

  else(MSVC)
  ##add libkml
  ##https://trac.osgeo.org/gdal/ticket/5725     
  ##is needed for SQLITE driver 
    list(REMOVE_ITEM ${proj}_DEPENDENCIES LIBKML)

    STRING(REGEX REPLACE "/$" "" CMAKE_WIN_INSTALL_PREFIX ${SB_INSTALL_PREFIX})    
    STRING(REGEX REPLACE "/" "\\\\" CMAKE_WIN_INSTALL_PREFIX ${CMAKE_WIN_INSTALL_PREFIX})
    configure_file(${CMAKE_SOURCE_DIR}/patches/${proj}/nmake_gdal_extra.opt.in ${CMAKE_BINARY_DIR}/nmake_gdal_extra.opt)
      
    ExternalProject_Add(${proj}
       PREFIX ${proj}
       URL "http://download.osgeo.org/gdal/1.11.0/gdal-1.11.0.tar.gz"
       URL_MD5 9fdf0f2371a3e9863d83e69951c71ec4
       SOURCE_DIR ${GDAL_SB_SRC}
       BINARY_DIR ${GDAL_SB_BUILD_DIR}
       INSTALL_DIR ${SB_INSTALL_PREFIX}
       DEPENDS ${${proj}_DEPENDENCIES}
       PATCH_COMMAND ${CMAKE_COMMAND} -E copy_directory  ${GDAL_SB_SRC} ${GDAL_SB_BUILD_DIR}
       CONFIGURE_COMMAND  ${CMAKE_COMMAND} -E copy  ${CMAKE_SOURCE_DIR}/patches/${proj}/ogrsqlitevirtualogr.cpp
      ${GDAL_SB_BUILD_DIR}/ogr/ogrsf_frmts/sqlite/ogrsqlitevirtualogr.cpp
       BUILD_COMMAND nmake /f ${GDAL_SB_BUILD_DIR}/makefile.vc MSVC_VER=${MSVC_VERSION} EXT_NMAKE_OPT=${CMAKE_BINARY_DIR}/nmake_gdal_extra.opt
       INSTALL_COMMAND nmake /f ${GDAL_SB_BUILD_DIR}/makefile.vc devinstall MSVC_VER=${MSVC_VERSION} EXT_NMAKE_OPT=${CMAKE_BINARY_DIR}/nmake_gdal_extra.opt
    )
    
    
  endif()
  
  message(STATUS "  Using GDAL SuperBuild version")
  
endif()
endif()
