if( __EXTERNAL_EXPAT__)
  return()
else()
  set(__EXTERNAL_EXPAT__ 1)
endif()

if(USE_SYSTEM_EXPAT)
  message(STATUS "  Using Expat system version")
  return()
endif()

SETUP_SUPERBUILD(PROJECT EXPAT)
message(STATUS "  Using Expat SuperBuild version")

ExternalProject_Add(EXPAT
  PREFIX EXPAT
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

SUPERBUILD_UPDATE_CMAKE_VARIABLES(EXPAT FALSE)
