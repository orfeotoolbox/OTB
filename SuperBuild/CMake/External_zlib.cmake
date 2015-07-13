set(proj ZLIB)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup Zlib ...")

if(USE_SYSTEM_ZLIB)
  find_package(ZLIB)
  add_custom_target(${proj})
  message(STATUS "  Using Zlib system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using zlib SuperBuild version")

  # set proj back to its original value
  set(proj ZLIB)
  
  # Try official release 1.2.8
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://sourceforge.net/projects/libpng/files/zlib/1.2.8/zlib-1.2.8.tar.gz/download"
    URL_MD5 44d667c142d7cda120332623eab69f40
    BINARY_DIR ${ZLIB_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:PATH=${SB_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
      #-DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
      #-DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      -DBUILD_SHARED_LIBS:BOOL=ON)

    if(UNIX)
      ExternalProject_Add_Step(${proj} remove_static
        COMMAND ${CMAKE_COMMAND} -E remove ${SB_INSTALL_PREFIX}/lib/libz.a
        DEPENDEES install)  
    endif()
  
    if(MSVC)
      ExternalProject_Add_Step(${proj} msvc_copy_hell
        COMMAND ${CMAKE_COMMAND} -E copy ${ZLIB_SB_BUILD_DIR}/zlib.lib ${SB_INSTALL_PREFIX}/lib/zdll.lib
        DEPENDEES install)  
    endif()
    
    set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
    if(WIN32)
      set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/zdll.lib)
    elseif(UNIX)
      set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libz${CMAKE_SHARED_LIBRARY_SUFFIX})
    endif()
    
endif()

endif()
