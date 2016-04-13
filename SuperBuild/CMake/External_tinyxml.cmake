if(NOT __EXTERNAL_TINYXML__)
set(__EXTERNAL_TINYXML__ 1)

message(STATUS "Setup TinyXML ...")

if(USE_SYSTEM_TINYXML)
  find_package ( TinyXML )
  message(STATUS "  Using TinyXML system version")
else()
  SETUP_SUPERBUILD(PROJECT TINYXML)
  message(STATUS "  Using TinyXML SuperBuild version")

  ExternalProject_Add(TINYXML
    PREFIX TINYXML
    URL "http://sourceforge.net/projects/tinyxml/files/tinyxml/2.6.2/tinyxml_2_6_2.tar.gz/download"
    URL_MD5 c1b864c96804a10526540c664ade67f0
    BINARY_DIR ${TINYXML_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
    CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    DEPENDS ${TINYXML_DEPENDENCIES}
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_SOURCE_DIR}/patches/TINYXML/CMakeLists.txt
      ${TINYXML_SB_SRC}
    )

  set(_SB_TINYXML_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_TINYXML_LIBRARY ${SB_INSTALL_PREFIX}/lib/tinyXML.lib)
  elseif(UNIX)
    set(_SB_TINYXML_LIBRARY ${SB_INSTALL_PREFIX}/lib/libtinyxml${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

endif()
endif()
