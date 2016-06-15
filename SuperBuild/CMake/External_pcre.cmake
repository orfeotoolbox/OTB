INCLUDE_ONCE_MACRO(PCRE)
SETUP_SUPERBUILD(PROJECT PCRE)
if(MSVC)
  # TODO ?
else()
  ExternalProject_Add(PCRE
    PREFIX PCRE
    URL "http://sourceforge.net/projects/pcre/files/pcre/8.36/pcre-8.36.tar.gz/download"
    URL_MD5 ff7b4bb14e355f04885cf18ff4125c98
    BINARY_DIR ${PCRE_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    CONFIGURE_COMMAND
    ${PCRE_SB_BUILD_DIR}/configure
    --prefix=${SB_INSTALL_PREFIX}
    BUILD_COMMAND $(MAKE)
    INSTALL_COMMAND $(MAKE) install
    DEPENDS ${PCRE_DEPENDENCIES}
    )

  ExternalProject_Add_Step(PCRE copy_source
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PCRE_SB_SRC} ${PCRE_SB_BUILD_DIR}
    DEPENDEES patch update
    DEPENDERS configure
    )
  
endif()
