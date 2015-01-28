message(STATUS "Setup FFTW ...")

set(proj FFTW)

set(DEFAULT_USE_SYSTEM_FFTW  OFF)

option(USE_SYSTEM_FFTW "  Use a system build of FFTW." ${DEFAULT_USE_SYSTEM_FFTW})
mark_as_advanced(USE_SYSTEM_FFTW)

if(USE_SYSTEM_FFTW)
  message(STATUS "  Using FFTW system version")
else()
  set(${proj}_DEPENDENCIES)
  set(projFloat ${proj}F)
  set(projDouble ${proj}D)
  
  set(FFTW_SB_BUILD_FLOAT_DIR ${CMAKE_BINARY_DIR}/${proj}/src/${projFloat})
  set(FFTW_SB_BUILD_DOUBLE_DIR ${CMAKE_BINARY_DIR}/${proj}/src/${projDouble})
  
  if(MSVC)
    # TODO : get precompiled binaries as not all MSVC versions can compile FFTW
      message(STATUS "  FFTW SuperBuild is not available yet...")
  else()
    # Compile the float version of FFTW
    ExternalProject_Add(${projFloat}
      PREFIX ${proj}
      URL "http://www.fftw.org/fftw-3.3.4.tar.gz"
      URL_MD5 2edab8c06b24feeb3b82bbb3ebf3e7b3
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      CONFIGURE_COMMAND
        ${FFTW_SB_BUILD_FLOAT_DIR}/configure
            --prefix=${CMAKE_INSTALL_PREFIX}
            --enable-shared
            --disable-static
            --enable-float
            --enable-threads
            --disable-fortran
      DEPENDS ${${proj}_DEPENDENCIES}
      )
      
    #ExternalProject_Add_Step(${projFloat} copy_source
    #  COMMAND ${CMAKE_COMMAND} -E copy_directory 
    #    ${FFTW_SB_SRC} ${FFTW_SB_BUILD_FLOAT_DIR}
    #  DEPENDEES patch update
    #  DEPENDERS configure
    #  )
    
    # Compile the double version of FFTW
    ExternalProject_Add(${projDouble}
      PREFIX ${proj}
      URL "http://www.fftw.org/fftw-3.3.4.tar.gz"
      URL_MD5 2edab8c06b24feeb3b82bbb3ebf3e7b3
      INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
      CONFIGURE_COMMAND
        ${FFTW_SB_BUILD_DOUBLE_DIR}/configure
            --prefix=${CMAKE_INSTALL_PREFIX}
            --enable-shared
            --disable-static
            --disable-float
            --enable-threads
            --disable-fortran
      DEPENDS ${${proj}_DEPENDENCIES}
      )
      
    #ExternalProject_Add_Step(${projDouble} copy_source
    #  COMMAND ${CMAKE_COMMAND} -E copy_directory 
    #    ${FFTW_SB_SRC} ${FFTW_SB_BUILD_DOUBLE_DIR}
    #  DEPENDEES patch update
    #  DEPENDERS configure
    #  )

          message(STATUS "  Using FFTW SuperBuild version")
  endif()
  


endif()