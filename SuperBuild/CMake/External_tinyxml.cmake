if( __EXTERNAL_TINYXML__)
  return()
else()
  set(__EXTERNAL_TINYXML__ 1)
endif()

if(USE_SYSTEM_TINYXML)
  message(STATUS "  Using TinyXML system version")
  return()
endif()

  SETUP_SUPERBUILD(PROJECT TINYXML)
  message(STATUS "  Using TinyXML SuperBuild version")

  ExternalProject_Add(TINYXML
    PREFIX TINYXML
    URL "http://sourceforge.net/projects/tinyxml/files/tinyxml/2.6.2/tinyxml_2_6_2.tar.gz/download"
    URL_MD5 c1b864c96804a10526540c664ade67f0
    BINARY_DIR ${TINYXML_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
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
