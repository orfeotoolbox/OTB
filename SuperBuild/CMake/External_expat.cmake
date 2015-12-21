set(proj EXPAT)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup expat ...")

if(USE_SYSTEM_EXPAT)
  find_package ( EXPAT )
  message(STATUS "  Using expat system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using Expat SuperBuild version")
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://sourceforge.net/projects/expat/files/expat/2.1.0/expat-2.1.0.tar.gz/download"
    URL_MD5 dd7dab7a5fea97d2a6a43f511449b7cd
    BINARY_DIR ${EXPAT_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DBUILD_SHARED_LIBS:BOOL=ON
        -DBUILD_examples:BOOL=OFF
        -DBUILD_tests:BOOL=OFF
        -DBUILD_tools:BOOL=OFF
    CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    )
  
  set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/expat.lib)
  elseif(UNIX)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libexpat${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

endif()
endif()
