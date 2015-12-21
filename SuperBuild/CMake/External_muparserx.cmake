set(proj MUPARSERX)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup muParserX ...")

if(USE_SYSTEM_MUPARSERX)
  find_package ( MuParserX )
  message(STATUS "  Using muParserX system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using muParserX SuperBuild version")
  
  set(MUPARSERX_FLAGS)
  if(APPLE)
    set(MUPARSERX_FLAGS "-DCMAKE_CXX_FLAGS:STRING=-std=c++0x")
  endif()
  
  # We provide a zip archive of last muparserx release (3.0.5)
  # Archive was generated using commit sha on muparserx github page
  # Commands to create source archive:
  # wget https://github.com/beltoforion/muparserx/archive/2ace83b5411f1ab9940653c2bab0efa5140efb71.zip
  # mv 2ace83b5411f1ab9940653c2bab0efa5140efb71.zip muparserx_v3_0_5.zip

  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "https://www.orfeo-toolbox.org/packages/muparserx_v3_0_5.zip"
    URL_MD5 ad86b88c159ab68f4bfc99d71166e3c5
    BINARY_DIR ${MUPARSERX_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DBUILD_SHARED_LIBS:BOOL=ON
      ${MUPARSERX_FLAGS}
    CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    DEPENDS ${${proj}_DEPENDENCIES}
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy 
      ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt 
      ${MUPARSERX_SB_SRC}
    UPDATE_COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/patches/${proj}/mpParserMessageProvider.cpp
    ${MUPARSERX_SB_SRC}/parser/
    )
  
  set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/muparserx.lib)
  elseif(UNIX)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libmuparserx${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()
endif()
endif()
