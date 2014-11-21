message(STATUS "Setup libtiff...")

set(proj TIFF)

set(DEFAULT_USE_SYSTEM_LIBTIFF  OFF)

option(USE_SYSTEM_LIBTIFF "  Use a system build of libtiff." ${DEFAULT_USE_SYSTEM_LIBTIFF})
mark_as_advanced(USE_SYSTEM_LIBTIFF)

if(USE_SYSTEM_LIBTIFF)
  message(STATUS "  Using libtiff system version")
else()
  set(${proj}_DEPENDENCIES)
  set(LIBTIFF_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(LIBTIFF_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  if(USE_SYSTEM_ZLIB)
    set(LIBTIFF_SB_ZLIB_CONFIG)
  else()
    set(LIBTIFF_SB_ZLIB_CONFIG 
      --with-zlib-include-dir=${CMAKE_INSTALL_PREFIX}/include
      )
    list(APPEND ${proj}_DEPENDENCIES ZLIB)
  endif()
  
  if(WIN32)
    # TODO : use nmake
  else()
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "ftp://ftp.remotesensing.org/pub/libtiff/tiff-4.0.3.tar.gz"
      URL_MD5 051c1068e6a0627f461948c365290410
      BINARY_DIR ${LIBTIFF_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      CONFIGURE_COMMAND
        LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib 
        ${LIBTIFF_SB_BUILD_DIR}/configure
        --prefix=${CMAKE_INSTALL_PREFIX}
        --enable-static=no
        ${LIBTIFF_SB_ZLIB_CONFIG}
      BUILD_COMMAND make
      INSTALL_COMMAND make install
      DEPENDS ${${proj}_DEPENDENCIES}
      )
    
    ExternalProject_Add_Step(${proj} copy_source
      COMMAND ${CMAKE_COMMAND} -E copy_directory 
        ${LIBTIFF_SB_SRC} ${LIBTIFF_SB_BUILD_DIR}
      DEPENDEES patch update
      DEPENDERS configure
      )
    
  endif()
  
  message(STATUS "  Using libtiff SuperBuild version")
endif()
