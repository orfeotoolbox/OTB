message(STATUS "Setup TinyXML ...")

set(proj TINYXML)

set(DEFAULT_USE_SYSTEM_TINYXML  OFF)

option(USE_SYSTEM_TINYXML "  Use a system build of TinyXML." ${DEFAULT_USE_SYSTEM_TINYXML})
mark_as_advanced(USE_SYSTEM_TINYXML)

if(USE_SYSTEM_TINYXML)
  message(STATUS "  Using TinyXML system version")
else()
  set(${proj}_DEPENDENCIES)
  set(TINYXML_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(TINYXML_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://sourceforge.net/projects/tinyxml/files/tinyxml/2.6.2/tinyxml_2_6_2.tar.gz/download"
    URL_MD5 c1b864c96804a10526540c664ade67f0
    BINARY_DIR ${TINYXML_SB_BUILD_DIR}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
    CMAKE_COMMAND
      # use 'env' because CTest launcher doesn't perform shell interpretation
      env LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib
      ${CMAKE_COMMAND}
    DEPENDS ${${proj}_DEPENDENCIES}
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy 
      ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt 
      ${TINYXML_SB_SRC}
    )
  
  message(STATUS "  Using TinyXML SuperBuild version")
endif()