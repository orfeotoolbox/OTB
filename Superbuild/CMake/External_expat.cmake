set(proj EXPAT)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup expat ...")

if(USE_SYSTEM_EXPAT)
  find_package ( EXPAT REQUIRED )
  add_custom_target(${proj})
  message(STATUS "  Using expat system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://sourceforge.net/projects/expat/files/expat/2.1.0/expat-2.1.0.tar.gz/download"
    URL_MD5 dd7dab7a5fea97d2a6a43f511449b7cd
    BINARY_DIR ${EXPAT_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DBUILD_SHARED_LIBS:BOOL=ON
        -DBUILD_examples:BOOL=OFF
        -DBUILD_tests:BOOL=OFF
        -DBUILD_tools:BOOL=OFF
    CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    )
    
  message(STATUS "  Using Expat SuperBuild version")

endif()
endif()
