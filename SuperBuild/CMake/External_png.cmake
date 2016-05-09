if(NOT __EXTERNAL_PNG__)
set(__EXTERNAL_PNG__ 1)

if(USE_SYSTEM_PNG)
  find_package ( PNG )
  message(STATUS "  Using libpng system version")
else()
  SETUP_SUPERBUILD(PROJECT PNG)
  message(STATUS "  Using libpng SuperBuild version")

  # declare dependencies
  ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(PNG ZLIB)

  ADD_SUPERBUILD_CMAKE_VAR(PNG ZLIB_INCLUDE_DIR)
  ADD_SUPERBUILD_CMAKE_VAR(PNG ZLIB_LIBRARY)

    ExternalProject_Add(PNG
      PREFIX PNG
      URL "http://sourceforge.net/projects/libpng/files/libpng16/older-releases/1.6.16/lpng1616.zip/download"
      URL_MD5 c90c9587c9a5c735327fb3f6900f6b03
      BINARY_DIR ${PNG_SB_BUILD_DIR}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
      DEPENDS ${PNG_DEPENDENCIES}
      CMAKE_CACHE_ARGS
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DCMAKE_INSTALL_PREFIX:PATH=${SB_INSTALL_PREFIX}
        -DSKIP_INSTALL_EXECUTABLES:BOOL=OFF
        -DPNG_STATIC:BOOL=OFF
        -DPNG_SHARED:BOOL=ON
        -DPNG_TESTS:BOOL=OFF
        -DCMAKE_PREFIX_PATH:STRING=${SB_INSTALL_PREFIX};${CMAKE_PREFIX_PATH}
        ${PNG_SB_CONFIG}
      CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    )
    #having -DPNGLIB_NAME:STRING=libpng in CMAKE_CACHE_ARGS doesnt work
    #libpng library built in the name libpng16.lib and other libs (Qt4)
    #reports libpng.lib is not found As Qt is strict on the name of libpng
    #below we copy the libpng16.lib to libpng.lib.
    #modifying CMakeLists.txt in libpng is another way but then whole
    #CMakeLists.txt must be kept in superbuild repository.
    if(MSVC)
      ExternalProject_Add_Step(PNG duplicate_pnglib
        COMMAND ${CMAKE_COMMAND} -E copy
        ${SB_INSTALL_PREFIX}/lib/libpng16.lib ${SB_INSTALL_PREFIX}/lib/libpng.lib
        DEPENDEES install
    )
    endif()

  set(_SB_PNG_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_PNG_LIBRARY ${SB_INSTALL_PREFIX}/lib/libpng.lib)
  elseif(UNIX)
    set(_SB_PNG_LIBRARY ${SB_INSTALL_PREFIX}/lib/libpng${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

endif()
endif()
