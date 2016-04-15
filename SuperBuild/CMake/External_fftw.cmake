if(NOT __EXTERNAL_FFTW__)
set(__EXTERNAL_FFTW__ 1)

if(USE_SYSTEM_FFTW)
  # TODO : use ITK's FindFFTW
  #find_package ( FFTW )
  message(STATUS "  Using FFTW system version")
else()
  SETUP_SUPERBUILD(PROJECT FFTW)
  set(projFloat FFTWF)
  set(projDouble FFTWD)

  set(FFTW_SB_BUILD_FLOAT_DIR ${CMAKE_BINARY_DIR}/FFTW/src/${projFloat})
  set(FFTW_SB_BUILD_DOUBLE_DIR ${CMAKE_BINARY_DIR}/FFTW/src/${projDouble})

  if(MSVC)
    # TODO : get precompiled binaries as not all MSVC versions can compile FFTW
      message(STATUS "  FFTW SuperBuild is not available yet...")
      add_custom_target(FFTW)
  else()
    message(STATUS "  Using FFTW SuperBuild version")
    # Compile the float version of FFTW
    ExternalProject_Add(${projFloat}
      PREFIX FFTW
      URL "http://www.fftw.org/fftw-3.3.4.tar.gz"
      URL_MD5 2edab8c06b24feeb3b82bbb3ebf3e7b3
      SOURCE_DIR ${FFTW_SB_BUILD_FLOAT_DIR}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
      CONFIGURE_COMMAND
      ${SB_ENV_CONFIGURE_CMD}
        ${FFTW_SB_BUILD_FLOAT_DIR}/configure
            --prefix=${SB_INSTALL_PREFIX}
            --enable-shared
            --disable-static
            --enable-float
            --enable-threads
            --disable-fortran
      DEPENDS ${FFTW_DEPENDENCIES}
      )

    #ExternalProject_Add_Step(${projFloat} copy_source
    #  COMMAND ${CMAKE_COMMAND} -E copy_directory
    #    ${FFTW_SB_SRC} ${FFTW_SB_BUILD_FLOAT_DIR}
    #  DEPENDEES patch update
    #  DEPENDERS configure
    #  )

    # Compile the double version of FFTW
    ExternalProject_Add(FFTW
      PREFIX FFTW
      URL "http://www.fftw.org/fftw-3.3.4.tar.gz"
      URL_MD5 2edab8c06b24feeb3b82bbb3ebf3e7b3
      SOURCE_DIR ${FFTW_SB_BUILD_DOUBLE_DIR}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
      CONFIGURE_COMMAND
      ${SB_ENV_CONFIGURE_CMD}
        ${FFTW_SB_BUILD_DOUBLE_DIR}/configure
            --prefix=${SB_INSTALL_PREFIX}
            --enable-shared
            --disable-static
            --disable-float
            --enable-threads
            --disable-fortran
      DEPENDS ${FFTW_DEPENDENCIES}
      )

    #ExternalProject_Add_Step(${projDouble} copy_source
    #  COMMAND ${CMAKE_COMMAND} -E copy_directory
    #    ${FFTW_SB_SRC} ${FFTW_SB_BUILD_DOUBLE_DIR}
    #  DEPENDEES patch update
    #  DEPENDERS configure
    #  )

    add_dependencies(FFTW ${projFloat})

    FIX_RPATH_FOR_AUTOCONF_BUILD(FFTW "libfftw3*dylib")

    set(_SB_FFTW_INCLUDE_PATH ${SB_INSTALL_PREFIX}/include)

  endif()



endif()
endif()
