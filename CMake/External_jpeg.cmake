message(STATUS "Setup libjpeg...")

set(proj JPEG)

set(DEFAULT_USE_SYSTEM_JPEG  OFF)

option(USE_SYSTEM_JPEG "  Use a system build of libjpeg." ${DEFAULT_USE_SYSTEM_JPEG})
mark_as_advanced(USE_SYSTEM_JPEG)

if(USE_SYSTEM_JPEG)
  message(STATUS "  Using libjpeg system version")
else()
  set(${proj}_DEPENDENCIES)
  set(JPEG_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(JPEG_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  if(WIN32)

   
   ExternalProject_Add(${proj}_build
      PREFIX ${proj}
      URL "http://www.ijg.org/files/jpegsr8c.zip"
      URL_MD5 2af71a567af8f60a649b2fcc87c88192
      SOURCE_DIR ${JPEG_SB_SRC}
      BINARY_DIR ${JPEG_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      PATCH_COMMAND ${CMAKE_COMMAND} -E copy_directory ${JPEG_SB_SRC} ${JPEG_SB_BUILD_DIR}
      CONFIGURE_COMMAND ""
      BUILD_COMMAND nmake /f ${JPEG_SB_BUILD_DIR}/makefile.vc nodebug=1
      INSTALL_COMMAND ${CMAKE_COMMAND} -E copy  ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt
      ${CMAKE_BINARY_DIR}/${proj}/_install
    )

    ExternalProject_Add_Step(${proj}_build copy_jconfig
      COMMAND ${CMAKE_COMMAND} -E copy 
        ${JPEG_SB_BUILD_DIR}/jconfig.vc ${JPEG_SB_BUILD_DIR}/jconfig.h
      DEPENDEES configure
      DEPENDERS build
    )
      
      
    ExternalProject_Add(${proj}
      PREFIX ${proj}/_install
      DOWNLOAD_COMMAND ""
      SOURCE_DIR ${proj}/_install
      BINARY_DIR ${JPEG_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DJPEG_BUILD_DIR:STRING=${JPEG_SB_BUILD_DIR}
      DEPENDS ${proj}_build
      CMAKE_COMMAND
    )
      

  else()
    
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://sourceforge.net/projects/libjpeg/files/libjpeg/6b/jpegsrc.v6b.tar.gz/download"
      URL_MD5 dbd5f3b47ed13132f04c685d608a7547
      BINARY_DIR ${JPEG_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      PATCH_COMMAND
        ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/patches/JPEG/configure
        ${JPEG_SB_SRC}/
      CONFIGURE_COMMAND
        # use 'env' because CTest launcher doesn't perform shell interpretation
        env LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib 
        ${JPEG_SB_BUILD_DIR}/configure
        --build=${CMAKE_SYSTEM_PROCESSOR}-linux-gnu
        --host=${CMAKE_SYSTEM_PROCESSOR}-linux-gnu
        --prefix=${CMAKE_INSTALL_PREFIX}
        --mandir=${CMAKE_INSTALL_PREFIX}/share/man
        --enable-shared=yes
      BUILD_COMMAND $(MAKE)
      INSTALL_COMMAND $(MAKE) install-lib
      DEPENDS ${${proj}_DEPENDENCIES}
      )
    
    ExternalProject_Add_Step(${proj} copy_source
      COMMAND ${CMAKE_COMMAND} -E copy_directory 
        ${JPEG_SB_SRC} ${JPEG_SB_BUILD_DIR}
      DEPENDEES patch update
      DEPENDERS configure
      )
    
  endif()
  
  message(STATUS "  Using libjpeg SuperBuild version")
endif()
