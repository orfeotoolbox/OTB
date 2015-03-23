set(proj SWIG)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup swig...")

if(USE_SYSTEM_SWIG)
  find_package ( SWIG )
  add_custom_target(${proj})
  message(STATUS "  Using swig system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using swig SuperBuild version")
  
  set(PythonInterp_FIND_VERSION 2.7)
  find_package(PythonInterp)
  if(PYTHONINTERP_FOUND)
    set(SWIG_SB_PYTHON_CONFIG "--with-python=${PYTHON_EXECUTABLE}")
  else()
    set(SWIG_SB_PYTHON_CONFIG)
    message(WARNING "  No suitable python interpreter was found !")
  endif()
  
  find_package(Java)
  if(JAVA_FOUND)
    set(SWIG_SB_JAVA_CONFIG "--with-java=${Java_JAVA_EXECUTABLE}")
  else()
    set(SWIG_SB_JAVA_CONFIG)
    message(WARNING "  No Java executable was found !")
  endif()
  
  if(MSVC)
    # Use pre-built swig executable (no linking is required, no install done)
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://sourceforge.net/projects/swig/files/swigwin/swigwin-3.0.5/swigwin-3.0.5.zip/download"
      URL_MD5 fd2e050f29e2a00b2348f5f7d3476490
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
      DEPENDS ${${proj}_DEPENDENCIES}
      )
    
  else()
    # declare dependencies
    set(${proj}_DEPENDENCIES PCRE BOOST)
    INCLUDE_SUPERBUILD_DEPENDENCIES(${${proj}_DEPENDENCIES})
    # set proj back to its original value
    set(proj SWIG)
    
    ADD_SUPERBUILD_CONFIGURE_VAR(PCRE_ROOT --with-pcre-prefix)
    ADD_SUPERBUILD_CONFIGURE_VAR(BOOST_ROOT --with-boost)
    
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://sourceforge.net/projects/swig/files/swig/swig-3.0.5/swig-3.0.5.tar.gz/download"
      URL_MD5 dcb9638324461b9baba8e044fe59031d
      BINARY_DIR ${SWIG_SB_BUILD_DIR}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
      CONFIGURE_COMMAND
        ${SWIG_SB_BUILD_DIR}/configure
        --prefix=${SB_INSTALL_PREFIX}
        ${SWIG_SB_PYTHON_CONFIG}
        ${SWIG_SB_JAVA_CONFIG}
        ${SWIG_SB_CONFIG}
      BUILD_COMMAND $(MAKE)
      INSTALL_COMMAND $(MAKE) install
      DEPENDS ${${proj}_DEPENDENCIES}
      )
    
    ExternalProject_Add_Step(${proj} copy_source
      COMMAND ${CMAKE_COMMAND} -E copy_directory 
        ${SWIG_SB_SRC} ${SWIG_SB_BUILD_DIR}
      DEPENDEES patch update
      DEPENDERS configure
      )
    
  endif()
  
  if(WIN32)
    set(_SB_SWIG_EXECUTABLE ${CMAKE_BINARY_DIR}/SWIG/src/SWIG/swig.exe)
  elseif(UNIX)
    set(_SB_SWIG_EXECUTABLE ${SB_INSTALL_PREFIX}/bin/swig)
  endif()
  
endif()
endif()
