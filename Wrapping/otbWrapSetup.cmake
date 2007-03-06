#-----------------------------------------------------------------------------
# wrapper config
OPTION(OTB_CSWIG_TCL "Build cswig Tcl wrapper support (requires CableSwig)." OFF)
OPTION(OTB_CSWIG_PYTHON "Build cswig Python wrapper support (requires CableSwig)." OFF)
OPTION(OTB_CSWIG_JAVA "Build cswig Java wrapper support " OFF)

# perl support does not work, contact bill hoffman at kitware
# if you are interested in perl wrapping.  It is close, but
# not there yet.
#OPTION(OTB_CSWIG_PERL "Build cswig Perl wrapper support " OFF)

#-----------------------------------------------------------------------------
# Do we need CableSwig?
SET(OTB_NEED_CableSwig 0)

IF(USE_WRAP_OTB)
  SET(OTB_NEED_CableSwig 1)
ENDIF(USE_WRAP_OTB)

IF(OTB_CSWIG_TCL)
  SET(OTB_NEED_CableSwig 1)
ENDIF(OTB_CSWIG_TCL)

IF(OTB_CSWIG_PYTHON)
SET(OTB_NEED_CableSwig 1)
ENDIF(OTB_CSWIG_PYTHON)

IF(OTB_CSWIG_JAVA)
  SET(OTB_NEED_CableSwig 1)
ENDIF(OTB_CSWIG_JAVA)

IF(OTB_CSWIG_PERL)
  SET(OTB_NEED_CableSwig 1)
ENDIF(OTB_CSWIG_PERL)

IF(OTB_NEED_CableSwig)

  IF(NOT BUILD_SHARED_LIBS)
    MESSAGE(FATAL_ERROR "Wrapping requires a shared build, change BUILD_SHARED_LIBS to ON")
  ENDIF(NOT BUILD_SHARED_LIBS)

  # Search first if CableSwig is in the OTB source tree
  IF(EXISTS ${OTB_SOURCE_DIR}/Utilities/CableSwig)
    SET(CMAKE_MODULE_PATH ${OTB_SOURCE_DIR}/Utilities/CableSwig/SWIG/CMake)

    # CableSwig is included in the source distribution.
    SET(OTB_BUILD_CABLESWIG 1)
    SET(CableSwig_DIR ${OTB_BINARY_DIR}/Utilities/CableSwig CACHE PATH "CableSwig_DIR: The directory containing CableSwigConfig.cmake.")
    SET(CableSwig_FOUND 1)
    SET(CableSwig_INSTALL_ROOT ${OTB_INSTALL_LIB_DIR}/CSwig)
    INCLUDE(${CableSwig_DIR}/CableSwigConfig.cmake OPTIONAL) 
    SUBDIRS(Utilities/CableSwig)
  ELSE(EXISTS ${OTB_SOURCE_DIR}/Utilities/CableSwig)
    # If CableSwig is not in the source tree, 
    # then try to find a binary build of CableSwig
    FIND_PACKAGE(CableSwig)
    SET(CMAKE_MODULE_PATH ${CableSwig_DIR}/SWIG/CMake)
  ENDIF(EXISTS ${OTB_SOURCE_DIR}/Utilities/CableSwig)
 
  IF(NOT CableSwig_FOUND)
    # We have not found CableSwig.  Complain.
    MESSAGE(FATAL_ERROR "CableSwig is required for CSwig Wrapping.")
  ENDIF(NOT CableSwig_FOUND)

ENDIF(OTB_NEED_CableSwig)


