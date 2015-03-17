set(proj GEOTIFF)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup libgeotiff...")

if(USE_SYSTEM_GEOTIFF)
  find_package ( GeoTIFF )
  add_custom_target(${proj})
  message(STATUS "  Using libgeotiff system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using GeoTIFF SuperBuild version")
  
  # declare dependencies
  set(${proj}_DEPENDENCIES TIFF PROJ JPEG ZLIB)
  INCLUDE_SUPERBUILD_DEPENDENCIES(${${proj}_DEPENDENCIES})
  # set proj back to its original value
  set(proj GEOTIFF)
  
  if(MSVC)
    ADD_SUPERBUILD_CMAKE_VAR(TIFF_INCLUDE_DIR)
    ADD_SUPERBUILD_CMAKE_VAR(TIFF_LIBRARY)
    ADD_SUPERBUILD_CMAKE_VAR(PROJ4_INCLUDE_DIR)
    ADD_SUPERBUILD_CMAKE_VAR(PROJ4_LIBRARY)
    ADD_SUPERBUILD_CMAKE_VAR(ZLIB_INCLUDE_DIR)
    ADD_SUPERBUILD_CMAKE_VAR(ZLIB_LIBRARY)
    ADD_SUPERBUILD_CMAKE_VAR(JPEG_INCLUDE_DIR)
    ADD_SUPERBUILD_CMAKE_VAR(JPEG_LIBRARY)
  else()
    ADD_SUPERBUILD_CONFIGURE_VAR(TIFF_SB_ROOT --with-libtiff)
    ADD_SUPERBUILD_CONFIGURE_VAR(PROJ_SB_ROOT --with-proj)
    ADD_SUPERBUILD_CONFIGURE_VAR(ZLIB_SB_ROOT --with-zlib)
    ADD_SUPERBUILD_CONFIGURE_VAR(JPEG_SB_ROOT --with-jpeg)
  endif()
  
    #if(MSVC)
    #    set(GEOTIFF_SB_ZLIB_CONFIG 
    #    -DZLIB_INCLUDE_DIR:STRING=${SB_INSTALL_PREFIX}/include 
    #    -DZLIB_LIBRARY:STRING=${SB_INSTALL_PREFIX}/lib/zlib.lib
    #    )
  
  if(MSVC)

    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://download.osgeo.org/geotiff/libgeotiff/libgeotiff-1.4.0.tar.gz"
      URL_MD5 efa7b418bc00228fcda4da63557e40c2
      BINARY_DIR ${GEOTIFF_SB_BUILD_DIR}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      DEPENDS ${${proj}_DEPENDENCIES}
      PATCH_COMMAND ${CMAKE_COMMAND} -E copy 
        ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt
        ${GEOTIFF_SB_SRC}  
      
      CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DWITH_TIFF:BOOL=ON
        -DWITH_PROJ4:BOOL=ON
        -DWITH_JPEG:BOOL=OFF
        -DWITH_ZLIB:BOOL=ON
        -DWITH_UTILITIES:BOOL=ON
        -DCMAKE_PREFIX_PATH:STRING=${SB_INSTALL_PREFIX};${CMAKE_PREFIX_PATH}
        ${GEOTIFF_SB_CONFIG}
      CMAKE_COMMAND
      )
  else()
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://download.osgeo.org/geotiff/libgeotiff/libgeotiff-1.4.0.tar.gz"
      URL_MD5 efa7b418bc00228fcda4da63557e40c2
      BINARY_DIR ${GEOTIFF_SB_BUILD_DIR}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      CONFIGURE_COMMAND
        # use 'env' because CTest launcher doesn't perform shell interpretation
        ${SB_ENV_CONFIGURE_CMD}
        ${GEOTIFF_SB_BUILD_DIR}/configure
        --prefix=${SB_INSTALL_PREFIX}
        --enable-static=no
        ${GEOTIFF_SB_CONFIG}
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
  
  set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/geotiff_i.lib)
  elseif(UNIX)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libgeotiff${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()
  
endif()
endif()
