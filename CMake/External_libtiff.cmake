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
  
    set(TIFF_SB_ZLIB_CONFIG)
    set(ZLIB_DIR ${CMAKE_INSTALL_PREFIX})
    configure_file(${CMAKE_SOURCE_DIR}/patches/${proj}/nmake.opt ${TIFF_SB_BUILD_DIR}/nmake.opt)
    
     ExternalProject_Add(${proj}_build
      PREFIX ${proj}
      URL "http://download.osgeo.org/libtiff/tiff-4.0.3.tar.gz"
      URL_MD5 051c1068e6a0627f461948c365290410
      SOURCE_DIR ${TIFF_SB_SRC}
      BINARY_DIR ${TIFF_SB_SRC}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      DEPENDS ${${proj}_DEPENDENCIES}
      PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${TIFF_SB_BUILD_DIR}/nmake.opt ${TIFF_SB_SRC}
      CONFIGURE_COMMAND ""
      BUILD_COMMAND nmake /f ${TIFF_SB_SRC}/Makefile.vc
      INSTALL_COMMAND ${CMAKE_COMMAND} -E copy  ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt
      ${CMAKE_BINARY_DIR}/${proj}/_install
    )

    ExternalProject_Add(${proj}
      PREFIX ${proj}/_install
      DOWNLOAD_COMMAND ""
      SOURCE_DIR ${proj}/_install
      BINARY_DIR ${TIFF_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DTIFF_BUILD_DIR:STRING=${TIFF_SB_SRC}/libtiff
      DEPENDS ${proj}_build
      CMAKE_COMMAND
    )

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
