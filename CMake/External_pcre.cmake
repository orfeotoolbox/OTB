message(STATUS "Setup pcre...")

set(proj PCRE)

set(DEFAULT_USE_SYSTEM_PCRE  OFF)

option(USE_SYSTEM_PCRE "  Use a system build of pcre." ${DEFAULT_USE_SYSTEM_PCRE})
mark_as_advanced(USE_SYSTEM_PCRE)

if(USE_SYSTEM_PCRE)
  message(STATUS "  Using pcre system version")
else()
  set(${proj}_DEPENDENCIES)
  set(PCRE_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(PCRE_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  if(MSVC)
    # TODO ?
  else()
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://sourceforge.net/projects/pcre/files/pcre/8.36/pcre-8.36.tar.gz/download"
      URL_MD5 ff7b4bb14e355f04885cf18ff4125c98
      BINARY_DIR ${PCRE_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      CONFIGURE_COMMAND
        ${PCRE_SB_BUILD_DIR}/configure
        --prefix=${CMAKE_INSTALL_PREFIX}
      BUILD_COMMAND $(MAKE)
      INSTALL_COMMAND $(MAKE) install
      DEPENDS ${${proj}_DEPENDENCIES}
      )
    
    ExternalProject_Add_Step(${proj} copy_source
      COMMAND ${CMAKE_COMMAND} -E copy_directory 
        ${PCRE_SB_SRC} ${PCRE_SB_BUILD_DIR}
      DEPENDEES patch update
      DEPENDERS configure
      )
    
  endif()
  
  message(STATUS "  Using pcre SuperBuild version")
endif()
