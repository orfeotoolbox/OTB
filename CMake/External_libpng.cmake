message(STATUS "Setup libpng...")

set(proj PNG)

set(DEFAULT_USE_SYSTEM_PNG  OFF)

option(USE_SYSTEM_PNG "  Use a system build of libtiff." ${DEFAULT_USE_SYSTEM_TIFF})
mark_as_advanced(USE_SYSTEM_PNG)

if(USE_SYSTEM_PNG)
  message(STATUS "  Using libtiff system version")
else()
  set(${proj}_DEPENDENCIES)
  set(PNG_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(PNG_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  if(USE_SYSTEM_ZLIB)
    set(PNG_SB_ZLIB_CONFIG)
  else()
    if(MSVC)
        set(PNG_SB_ZLIB_CONFIG
            -DZLIB_INCLUDE_DIR:PATH=${CMAKE_INSTALL_PREFIX}/include
            -DZLIB_LIBRARY:FILEPATH=${CMAKE_INSTALL_PREFIX}/lib/zlib.lib)
        list(APPEND ${proj}_DEPENDENCIES ZLIB)
    endif()
  endif()
  
  if(MSVC)
  
     ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://sourceforge.net/projects/libpng/files/libpng16/1.6.16/lpng1616.zip/download"
      URL_MD5 c90c9587c9a5c735327fb3f6900f6b03
      BINARY_DIR ${PNG_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      DEPENDS ${${proj}_DEPENDENCIES}
      CMAKE_CACHE_ARGS
            -DCMAKE_BUILD_TYPE:STRING=Release
            -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
            ${PNG_SB_ZLIB_CONFIG}
      CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    )

  else(UNIX)
    #TODO
    
  endif()
  
  message(STATUS "  Using libpng SuperBuild version")
endif()
