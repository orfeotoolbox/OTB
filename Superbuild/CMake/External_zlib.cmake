message(STATUS "Setup Zlib ...")

set(proj ZLIB)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

ADD_SYSTEM_PREFIX(PROJECT ${proj} CMAKE_ALIAS ZLIB_ROOT)

if(USE_SYSTEM_ZLIB)
  find_package(ZLIB REQUIRED)
  message(STATUS "  Using Zlib system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  
  # declare dependencies
  
  INCLUDE_SUPERBUILD_DEPENDENCIES(${${proj}_DEPENDENCIES})
  # set proj back to its original value
  set(proj ZLIB)
  
  # Try official release 1.2.8
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://sourceforge.net/projects/libpng/files/zlib/1.2.8/zlib-1.2.8.tar.gz/download"
    URL_MD5 44d667c142d7cda120332623eab69f40
    BINARY_DIR ${ZLIB_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:PATH=${SB_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
      #-DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
      #-DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      -DBUILD_SHARED_LIBS:BOOL=ON
  )
  
    if(MSVC)
      ExternalProject_Add_Step(${proj} msvc_copy_hell
        COMMAND ${CMAKE_COMMAND} -E copy ${ZLIB_SB_BUILD_DIR}/zlib.lib ${SB_INSTALL_PREFIX}/lib/zdll.lib
        DEPENDEES install
      )  
    endif()
    
    set(ZLIB_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
    if(MSVC)
      set(ZLIB_LIBRARY ${SB_INSTALL_PREFIX}/lib/zdll.lib)
    elseif(APPLE)
      set(ZLIB_LIBRARY ${SB_INSTALL_PREFIX}/lib/libz.dylib)
    else()
      set(ZLIB_LIBRARY ${SB_INSTALL_PREFIX}/lib/libz.so)
    endif()

     message(STATUS "  Using zlib SuperBuild version")
endif()

endif()
