message(STATUS "Setup muParser ...")

set(proj MUPARSER)

set(DEFAULT_USE_SYSTEM_MUPARSER  OFF)

option(USE_SYSTEM_MUPARSER "  Use a system build of muParser." ${DEFAULT_USE_SYSTEM_MUPARSER})
mark_as_advanced(USE_SYSTEM_MUPARSER)

if(USE_SYSTEM_MUPARSER)
  message(STATUS "  Using muParser system version")
else()
  set(${proj}_DEPENDENCIES)
  set(MUPARSER_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(MUPARSER_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://downloads.sourceforge.net/project/muparser/muparser/Version%202.2.3/muparser_v2_2_3.zip"
    URL_MD5 9de40ec1dab5bd2787ee344fce5846ad
    BINARY_DIR ${MUPARSER_SB_BUILD_DIR}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
    CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    DEPENDS ${${proj}_DEPENDENCIES}
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy 
        ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt 
        ${MUPARSER_SB_SRC}
    )
  
  message(STATUS "  Using muParser SuperBuild version")

endif()
