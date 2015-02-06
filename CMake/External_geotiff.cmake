message(STATUS "Setup libgeotiff...")

set(proj GEOTIFF)

set(DEFAULT_USE_SYSTEM_GEOTIFF  OFF)

option(USE_SYSTEM_GEOTIFF "  Use a system build of libgeotiff." ${DEFAULT_USE_SYSTEM_GEOTIFF})
mark_as_advanced(USE_SYSTEM_GEOTIFF)

if(USE_SYSTEM_GEOTIFF)
  message(STATUS "  Using libgeotiff system version")
else()
  set(${proj}_DEPENDENCIES)
  set(GEOTIFF_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(GEOTIFF_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  # handle dependencies : TIFF Proj4 Zlib Jpeg
  if(USE_SYSTEM_TIFF)
    set(GEOTIFF_SB_TIFF_CONFIG)
  else()
    set(GEOTIFF_SB_TIFF_CONFIG --with-libtiff=${CMAKE_INSTALL_PREFIX})
    
    if(MSVC)
        set(GEOTIFF_SB_TIFF_CONFIG 
        -DTIFF_INCLUDE_DIR:STRING=${CMAKE_INSTALL_PREFIX}/include 
        -DTIFF_LIBRARY:STRING=${CMAKE_INSTALL_PREFIX}/lib/libtiff_i.lib
        )
    endif()
    
    list(APPEND ${proj}_DEPENDENCIES TIFF)
  endif()
  
  if(USE_SYSTEM_PROJ)
    set(GEOTIFF_SB_PROJ_CONFIG)
  else()
    set(GEOTIFF_SB_PROJ_CONFIG --with-proj=${CMAKE_INSTALL_PREFIX})
        if(MSVC)
        set(GEOTIFF_SB_PROJ_CONFIG 
        -DPROJ4_INCLUDE_DIR:STRING=${CMAKE_INSTALL_PREFIX}/include 
        -DPROJ4_LIBRARY:STRING=${CMAKE_INSTALL_PREFIX}/lib/proj_i.lib
        )
    endif()
    list(APPEND ${proj}_DEPENDENCIES PROJ)
  endif()
  
  if(USE_SYSTEM_ZLIB)
    set(GEOTIFF_SB_ZLIB_CONFIG)
  else()
    # geotiff configure script doesn't use the given path for zlib
    # so this feature is not enabled
    set(GEOTIFF_SB_ZLIB_CONFIG)
    if(MSVC)
        set(GEOTIFF_SB_ZLIB_CONFIG 
        -DZLIB_INCLUDE_DIR:STRING=${CMAKE_INSTALL_PREFIX}/include 
        -DZLIB_LIBRARY:STRING=${CMAKE_INSTALL_PREFIX}/lib/zlib.lib
        )
    list(APPEND ${proj}_DEPENDENCIES ZLIB)
    endif()
    #set(GEOTIFF_SB_ZLIB_CONFIG --with-zlib=${CMAKE_INSTALL_PREFIX})
    #list(APPEND ${proj}_DEPENDENCIES ZLIB)
  endif()
  
  if(USE_SYSTEM_JPEG)
    set(GEOTIFF_SB_JPEG_CONFIG)
  else()
    set(GEOTIFF_SB_JPEG_CONFIG --with-jpeg=${CMAKE_INSTALL_PREFIX})
    if(MSVC)
      set(GEOTIFF_SB_JPEG_CONFIG 
        -DJPEG_INCLUDE_DIR:STRING=${CMAKE_INSTALL_PREFIX}/include 
        -DJPEG_LIBRARY:STRING=${CMAKE_INSTALL_PREFIX}/lib/libjpeg.lib
        )
      list(APPEND ${proj}_DEPENDENCIES JPEG)
    endif()    
  endif()
  
  if(MSVC)

    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://download.osgeo.org/geotiff/libgeotiff/libgeotiff-1.4.0.tar.gz"
      URL_MD5 efa7b418bc00228fcda4da63557e40c2
      BINARY_DIR ${GEOTIFF_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      DEPENDS ${${proj}_DEPENDENCIES}
      PATCH_COMMAND ${CMAKE_COMMAND} -E copy 
        ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt
        ${GEOTIFF_SB_SRC}  
      
      CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DWITH_TIFF:BOOL=ON
        -DWITH_PROJ4:BOOL=ON
        -DWITH_JPEG:BOOL=OFF
        -DWITH_ZLIB:BOOL=ON
        -DWITH_UTILITIES:BOOL=ON
        ${GEOTIFF_SB_TIFF_CONFIG}
        ${GEOTIFF_SB_JPEG_CONFIG}
        ${GEOTIFF_SB_PROJ_CONFIG}
        ${GEOTIFF_SB_ZLIB_CONFIG}
      CMAKE_COMMAND
      )
  else()
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://download.osgeo.org/geotiff/libgeotiff/libgeotiff-1.4.0.tar.gz"
      URL_MD5 efa7b418bc00228fcda4da63557e40c2
      BINARY_DIR ${GEOTIFF_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      CONFIGURE_COMMAND
        # use 'env' because CTest launcher doesn't perform shell interpretation
        env ${LDLIBVAR}=${CMAKE_INSTALL_PREFIX}/lib
        ${GEOTIFF_SB_BUILD_DIR}/configure
        --prefix=${CMAKE_INSTALL_PREFIX}
        --enable-static=no
        ${GEOTIFF_SB_JPEG_CONFIG}
        ${GEOTIFF_SB_TIFF_CONFIG}
        ${GEOTIFF_SB_PROJ_CONFIG}
        ${GEOTIFF_SB_ZLIB_CONFIG}
        
      BUILD_COMMAND $(MAKE)
      INSTALL_COMMAND $(MAKE) install
      DEPENDS ${${proj}_DEPENDENCIES}
      PATCH_COMMAND ${CMAKE_COMMAND} -E copy 
        ${CMAKE_SOURCE_DIR}/patches/${proj}/configure
        ${GEOTIFF_SB_SRC}
      )
    
    ExternalProject_Add_Step(${proj} copy_source
      COMMAND ${CMAKE_COMMAND} -E copy_directory 
        ${GEOTIFF_SB_SRC} ${GEOTIFF_SB_BUILD_DIR}
      DEPENDEES patch update
      DEPENDERS configure
      )
    
  endif()
  
  message(STATUS "  Using GeoTIFF SuperBuild version")
endif()
