# $Id$

PROJECT(OTBTesting)

# The Testing directory is setup so it can build either as part of the
# normal OTB build or as a stand-alone project.  This design is useful
# for testing the installation tree of OTB.

MAKE_DIRECTORY(${OTBTesting_BINARY_DIR}/Temporary)

IF(OTB_BINARY_DIR)
  # We are building inside the tree.
  SUBDIRS(Code Fa Utilities)

ELSE(OTB_BINARY_DIR)
  # We are building as a stand-alone project.
  SET(LIBRARY_OUTPUT_PATH ${OTBTesting_BINARY_DIR}/bin CACHE PATH "Single output directory for building all libraries.")
  SET(EXECUTABLE_OUTPUT_PATH ${OTBTesting_BINARY_DIR}/bin CACHE PATH "Single output directory for building all executables.")
  MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)
  SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

  # Configure Dart testing support.
  SET(PROJECT_SOURCE_DIR "${OTBTesting_SOURCE_DIR}/..")
  INCLUDE(${CMAKE_ROOT}/Modules/Dart.cmake)
  SET(PROJECT_SOURCE_DIR "${OTBTesting_SOURCE_DIR}")
  MARK_AS_ADVANCED(TCL_TCLSH)
  IF(BUILD_TESTING)
    ENABLE_TESTING()
  ENDIF(BUILD_TESTING)

  # Find the data directory.
  FIND_PATH(OTB_DATA_ROOT OTBData.readme ${OTBTesting_SOURCE_DIR}/Data $ENV{OTB_DATA_ROOT})
  MARK_AS_ADVANCED(OTB_DATA_ROOT)

  # Find OTB and load its settings.
  FIND_PACKAGE(OTB)
  IF(OTB_FOUND)
    INCLUDE(${OTB_USE_FILE})
    SUBDIRS(Code Fa)
  ELSE(OTB_FOUND)
    MESSAGE("OTB not found.  Please set OTB_DIR")
  ENDIF(OTB_FOUND)
ENDIF(OTB_BINARY_DIR)
