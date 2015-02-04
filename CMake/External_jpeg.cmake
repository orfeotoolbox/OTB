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
      
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://www.ijg.org/files/jpegsr8c.zip"
      URL_MD5 2af71a567af8f60a649b2fcc87c88192
      SOURCE_DIR ${JPEG_SB_SRC}
      BINARY_DIR ${JPEG_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      PATCH_COMMAND ${CMAKE_COMMAND} -E copy  ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt ${JPEG_SB_SRC}
      CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF
    )

    ExternalProject_Add_Step(${proj} copy_jconfig
      COMMAND ${CMAKE_COMMAND} -E copy 
        ${JPEG_SB_SRC}/jconfig.vc ${JPEG_SB_SRC}/jconfig.h
      DEPENDEES patch
      DEPENDERS build
    )        
      

  else()
    
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://www.ijg.org/files/jpegsrc.v9a.tar.gz"
      URL_MD5 3353992aecaee1805ef4109aadd433e7
      BINARY_DIR ${JPEG_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      CONFIGURE_COMMAND
        ${JPEG_SB_BUILD_DIR}/configure
        --prefix=${CMAKE_INSTALL_PREFIX}
        --enable-shared=yes
      BUILD_COMMAND $(MAKE)
      INSTALL_COMMAND $(MAKE) install
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
