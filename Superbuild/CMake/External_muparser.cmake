set(proj MUPARSER)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup muParser ...")

if(USE_SYSTEM_MUPARSER)
  find_package ( MUPARSER REQUIRED )
  add_custom_target(${proj})
  message(STATUS "  Using muParser system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using muParser SuperBuild version")
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://downloads.sourceforge.net/project/muparser/muparser/Version%202.2.3/muparser_v2_2_3.zip"
    URL_MD5 9de40ec1dab5bd2787ee344fce5846ad
    BINARY_DIR ${MUPARSER_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
    CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    DEPENDS ${${proj}_DEPENDENCIES}
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy 
        ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt 
        ${MUPARSER_SB_SRC}
    )
  
  set(${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/muparser.lib)
  elseif(UNIX)
    set(${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libmuparser${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

endif()
endif()
