message(STATUS "Setup libtiff...")

set(proj TIFF)

set(DEFAULT_USE_SYSTEM_TIFF  OFF)

option(USE_SYSTEM_TIFF "  Use a system build of libtiff." ${DEFAULT_USE_SYSTEM_TIFF})
mark_as_advanced(USE_SYSTEM_TIFF)

if(USE_SYSTEM_TIFF)
  message(STATUS "  Using libtiff system version")
else()
  set(${proj}_DEPENDENCIES)
  set(TIFF_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(TIFF_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  if(USE_SYSTEM_ZLIB)
    set(TIFF_SB_ZLIB_CONFIG)
  else()
    set(TIFF_SB_ZLIB_CONFIG 
      --with-zlib-include-dir=${CMAKE_INSTALL_PREFIX}/include
      )
    list(APPEND ${proj}_DEPENDENCIES ZLIB)
  endif()
  
  if(MSVC)
    # TODO : use nmake
    #using tiff from gis internals
  else()
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://download.osgeo.org/libtiff/tiff-4.0.3.tar.gz"
      URL_MD5 051c1068e6a0627f461948c365290410
      BINARY_DIR ${TIFF_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      CONFIGURE_COMMAND
        # use 'env' because CTest launcher doesn't perform shell interpretation
        env LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib 
        ${TIFF_SB_BUILD_DIR}/configure
        --prefix=${CMAKE_INSTALL_PREFIX}
        --enable-static=no
        ${TIFF_SB_ZLIB_CONFIG}
      BUILD_COMMAND $(MAKE)
      INSTALL_COMMAND $(MAKE) install
      DEPENDS ${${proj}_DEPENDENCIES}
      )
    
    ExternalProject_Add_Step(${proj} copy_source
      COMMAND ${CMAKE_COMMAND} -E copy_directory 
        ${TIFF_SB_SRC} ${TIFF_SB_BUILD_DIR}
      DEPENDEES patch update
      DEPENDERS configure
    )
    
  endif()
  
  message(STATUS "  Using libtiff SuperBuild version")
endif()
