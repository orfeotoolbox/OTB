message(STATUS "Setup muParserX ...")

set(proj MUPARSERX)

set(DEFAULT_USE_SYSTEM_MUPARSERX  OFF)

option(USE_SYSTEM_MUPARSERX "  Use a system build of muParserX." ${DEFAULT_USE_SYSTEM_MUPARSERX})
mark_as_advanced(USE_SYSTEM_MUPARSERX)

if(USE_SYSTEM_MUPARSERX)
  message(STATUS "  Using muParserX system version")
else()
  set(${proj}_DEPENDENCIES)
  set(MUPARSERX_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(MUPARSERX_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  # svn checkout http://muparserx.googlecode.com/svn/trunk/ muparserx-read-only
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    #URL "http://muparserx.googlecode.com/svn/archives/muparserx_v3_0_2.zip"
    #URL_MD5 487a91084f1929d2fdc9bedee598a77a
    SVN_REPOSITORY "http://muparserx.googlecode.com/svn/trunk"
    SVN_REVISION -r 291
    BINARY_DIR ${MUPARSERX_SB_BUILD_DIR}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DBUILD_SHARED_LIBS:BOOL=ON
    CMAKE_COMMAND
      # use 'env' because CTest launcher doesn't perform shell interpretation
      env LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib
      ${CMAKE_COMMAND}
    DEPENDS ${${proj}_DEPENDENCIES}
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy 
      ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt 
      ${MUPARSERX_SB_SRC}
    )
  
  message(STATUS "  Using muParserX SuperBuild version")

endif()