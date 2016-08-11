INCLUDE_ONCE_MACRO(SWIG)

SETUP_SUPERBUILD(SWIG)

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
  ExternalProject_Add(SWIG
    PREFIX SWIG
    URL "http://sourceforge.net/projects/swig/files/swigwin/swigwin-3.0.7/swigwin-3.0.7.zip/download"
    URL_MD5 d8b5a9ce49c819cc1bfc1e797b85ba7a
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    DEPENDS ${SWIG_DEPENDENCIES}
    )

else()
  # declare dependencies
  ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(SWIG PCRE BOOST)

  ADD_SUPERBUILD_CONFIGURE_VAR(PCRE_ROOT --with-pcre-prefix)
  ADD_SUPERBUILD_CONFIGURE_VAR(BOOST_ROOT --with-boost)

  ExternalProject_Add(SWIG
    PREFIX SWIG
    URL "http://sourceforge.net/projects/swig/files/swig/swig-3.0.7/swig-3.0.7.tar.gz/download"
    URL_MD5 7fff46c84b8c630ede5b0f0827e3d90a
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
    DEPENDS ${SWIG_DEPENDENCIES}
    )

  ExternalProject_Add_Step(SWIG copy_source
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
