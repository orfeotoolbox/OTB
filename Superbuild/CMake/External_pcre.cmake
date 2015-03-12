set(proj PCRE)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup pcre...")

if(USE_SYSTEM_PCRE)
  # TODO : FindPCRE.cmake
  # find_package ( PCRE REQUIRED )
  add_custom_target(${proj})
  message(STATUS "  Using pcre system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using pcre SuperBuild version")
  
  if(MSVC)
    # TODO ?
  else()
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://sourceforge.net/projects/pcre/files/pcre/8.36/pcre-8.36.tar.gz/download"
      URL_MD5 ff7b4bb14e355f04885cf18ff4125c98
      BINARY_DIR ${PCRE_SB_BUILD_DIR}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      CONFIGURE_COMMAND
        ${PCRE_SB_BUILD_DIR}/configure
        --prefix=${SB_INSTALL_PREFIX}
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
  
endif()
endif()
