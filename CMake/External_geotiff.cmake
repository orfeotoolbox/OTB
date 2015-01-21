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
  if(USE_SYSTEM_LIBTIFF)
    set(GEOTIFF_SB_TIFF_CONFIG)
  else()
    set(GEOTIFF_SB_TIFF_CONFIG --with-libtiff=${CMAKE_INSTALL_PREFIX})
    list(APPEND ${proj}_DEPENDENCIES TIFF)
  endif()
  
  if(USE_SYSTEM_PROJ)
    set(GEOTIFF_SB_PROJ_CONFIG)
  else()
    set(GEOTIFF_SB_PROJ_CONFIG --with-proj=${CMAKE_INSTALL_PREFIX})
    list(APPEND ${proj}_DEPENDENCIES PROJ)
  endif()
  
  if(USE_SYSTEM_ZLIB)
    set(GEOTIFF_SB_ZLIB_CONFIG)
  else()
    # geotiff configure script doesn't use the given path for zlib
    # so this feature is not enabled
    set(GEOTIFF_SB_ZLIB_CONFIG)
    #set(GEOTIFF_SB_ZLIB_CONFIG --with-zlib=${CMAKE_INSTALL_PREFIX})
    #list(APPEND ${proj}_DEPENDENCIES ZLIB)
  endif()
  
  if(USE_SYSTEM_JPEG)
    set(GEOTIFF_SB_JPEG_CONFIG)
  else()
    set(GEOTIFF_SB_JPEG_CONFIG --with-jpeg=${CMAKE_INSTALL_PREFIX})
    list(APPEND ${proj}_DEPENDENCIES JPEG)
  endif()
  
  if(WIN32)
    # TODO : use nmake
  else()
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://download.osgeo.org/geotiff/libgeotiff/libgeotiff-1.4.0.tar.gz"
      URL_MD5 efa7b418bc00228fcda4da63557e40c2
      BINARY_DIR ${GEOTIFF_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      CONFIGURE_COMMAND
        # use 'env' because CTest launcher doesn't perform shell interpretation
        env LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib
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
      )
    
    ExternalProject_Add_Step(${proj} copy_source
      COMMAND ${CMAKE_COMMAND} -E copy_directory 
        ${GEOTIFF_SB_SRC} ${GEOTIFF_SB_BUILD_DIR}
      DEPENDEES patch update
      DEPENDERS configure
      )
    
  endif()
  
  message(STATUS "  Using libgeotiff SuperBuild version")
endif()
