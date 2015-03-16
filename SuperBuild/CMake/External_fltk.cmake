message(STATUS "Setup FLTK...")

set(proj FLTK)

set(DEFAULT_USE_SYSTEM_FLTK  OFF)

option(USE_SYSTEM_FLTK "  Use FLTK installed on system." ${DEFAULT_USE_SYSTEM_FLTK})
mark_as_advanced(USE_SYSTEM_FLTK)

if(USE_SYSTEM_FLTK)
  message(STATUS "  Using FLTK system version")
else()
  set(${proj}_DEPENDENCIES)
  set(FLTK_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(FLTK_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  set(FLTK_SB_BUILD_SHARED ON)
  if(MSVC)
    set(FLTK_SB_BUILD_SHARED OFF)
  endif()
  
  if(NOT USE_SYSTEM_ZLIB)
    list(APPEND ${proj}_DEPENDENCIES ZLIB)
  endif()
  
  if(USE_SYSTEM_JPEG)
    set(FLTK_SB_JPEG_CONFIG)
  else()
    if(MSVC)
      set(FLTK_SB_JPEG_CONFIG
        -DJPEG_INCLUDE_DIR:PATH=${CMAKE_INSTALL_PREFIX}/include
        -DJPEG_LIBRARY:FILEPATH=${CMAKE_INSTALL_PREFIX}/lib/libjpeg.lib)
    else(UNIX)
      set(FLTK_SB_JPEG_CONFIG
        -DJPEG_INCLUDE_DIR:PATH=${CMAKE_INSTALL_PREFIX}/include
        -DJPEG_LIBRARY:FILEPATH=${CMAKE_INSTALL_PREFIX}/lib/libjpeg.so)
    endif()
    list(APPEND ${proj}_DEPENDENCIES JPEG)
  endif()
  
  if(USE_SYSTEM_PNG)
    list(APPEND ${proj}_DEPENDENCIES PNG) 
  endif()
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://fltk.org/pub/fltk/1.3.3/fltk-1.3.3-source.tar.gz"
    URL_MD5 9ccdb0d19dc104b87179bd9fd10822e3
    BINARY_DIR ${FLTK_SB_BUILD_DIR}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    DEPENDS ${${proj}_DEPENDENCIES}
    CMAKE_CACHE_ARGS
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
      -DOPTION_BUILD_EXAMPLES:BOOL=OFF
      -DOPTION_BUILD_SHARED_LIBS:BOOL=${FLTK_SB_BUILD_SHARED}
      -DOPTION_USE_SYSTEM_LIBPNG:BOOL=ON
      -DOPTION_USE_SYSTEM_LIBJPEG:BOOL=ON
      -DOPTION_USE_SYSTEM_ZLIB:BOOL=ON
      -DLIB_zlib:BOOL=ON
      -DLIB_jpeg:BOOL=ON
      -DLIB_png:BOOL=ON
      ${FLTK_SB_JPEG_CONFIG}
    CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  )

  message(STATUS "  Using FLTK SuperBuild version")
  
endif()
