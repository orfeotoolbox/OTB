set(proj MUPARSERX)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup muParserX ...")

if(USE_SYSTEM_MUPARSERX)
  find_package ( MUPARSERX )
  add_custom_target(${proj})
  message(STATUS "  Using muParserX system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using muParserX SuperBuild version")
  
  set(MUPARSERX_FLAGS)
  if(APPLE)
    set(MUPARSERX_FLAGS "-DCMAKE_CXX_FLAGS:STRING=-std=c++0x")
  endif()
  
  # svn checkout http://muparserx.googlecode.com/svn/trunk/ muparserx-read-only
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    #URL "http://muparserx.googlecode.com/svn/archives/muparserx_v3_0_2.zip"
    #URL_MD5 487a91084f1929d2fdc9bedee598a77a
    SVN_REPOSITORY "http://muparserx.googlecode.com/svn/trunk"
    SVN_REVISION -r 291
    BINARY_DIR ${MUPARSERX_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
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
    )
  
  set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/muparserx.lib)
  elseif(UNIX)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libmuparserx${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

endif()
endif()
