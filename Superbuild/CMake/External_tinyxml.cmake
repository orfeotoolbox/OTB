set(proj TINYXML)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup TinyXML ...")

if(USE_SYSTEM_TINYXML)
  find_package ( TinyXML REQUIRED )
  message(STATUS "  Using TinyXML system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://sourceforge.net/projects/tinyxml/files/tinyxml/2.6.2/tinyxml_2_6_2.tar.gz/download"
    URL_MD5 c1b864c96804a10526540c664ade67f0
    BINARY_DIR ${TINYXML_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
    CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    DEPENDS ${${proj}_DEPENDENCIES}
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy 
      ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt 
      ${TINYXML_SB_SRC}
    )
  
  message(STATUS "  Using TinyXML SuperBuild version")
endif()
endif()
