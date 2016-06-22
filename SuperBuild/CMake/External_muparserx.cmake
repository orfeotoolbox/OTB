if( __EXTERNAL_MUPARSERX__)
  return()
else()
  set(__EXTERNAL_MUPARSERX__ 1)
endif()

if(USE_SYSTEM_MUPARSERX)
  message(STATUS "  Using muParserX system version")
  return()
endif()

SETUP_SUPERBUILD(PROJECT MUPARSERX)
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

ExternalProject_Add(MUPARSERX
  PREFIX MUPARSERX
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
  DEPENDS ${MUPARSERX_DEPENDENCIES}
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/patches/MUPARSERX/CMakeLists.txt
  ${MUPARSERX_SB_SRC}
  UPDATE_COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/patches/MUPARSERX/mpParserMessageProvider.cpp
  ${MUPARSERX_SB_SRC}/parser/
  )

SUPERBUILD_UPDATE_CMAKE_VARIABLES(MUPARSERX FALSE)