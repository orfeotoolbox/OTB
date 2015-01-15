message(STATUS "Setup Proj.4 ...")

set(proj PROJ)

set(DEFAULT_USE_SYSTEM_PROJ  OFF)

option(USE_SYSTEM_PROJ "  Use a system build of Proj.4." ${DEFAULT_USE_SYSTEM_PROJ})
mark_as_advanced(USE_SYSTEM_PROJ)

if(USE_SYSTEM_PROJ)
  message(STATUS "  Using Proj4 system version")
else()
  
  set(PROJ_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(PROJ_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  if(WIN32)
    # TODO : use nmake
  else()
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://download.osgeo.org/proj/proj-4.8.0.tar.gz"
      URL_MD5 d815838c92a29179298c126effbb1537
      BINARY_DIR ${PROJ_SB_BUILD_DIR}
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      CONFIGURE_COMMAND
        # use 'env' because CTest launcher doesn't perform shell interpretation
        env LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib
        ${PROJ_SB_BUILD_DIR}/configure 
        --prefix=${CMAKE_INSTALL_PREFIX}
        --enable-static=no
      BUILD_COMMAND $(MAKE)
      INSTALL_COMMAND $(MAKE) install
      )
    
    ExternalProject_Add_Step(${proj} copy_source
      COMMAND ${CMAKE_COMMAND} -E copy_directory 
        ${PROJ_SB_SRC} ${PROJ_SB_BUILD_DIR}
      DEPENDEES patch update
      DEPENDERS configure
      )
    
  endif()
  
endif()