# $Id$

PROJECT(OTBExamples)

IF(OTB_BINARY_DIR)
  # We are building inside the tree.

SUBDIRS( 
  BasicFilters
  FeatureExtraction
  DataRepresentation
  IO
  Filtering
  ChangeDetection
  Learning
  Classification
  Segmentation
  Iterators
  MultiScale
  DisparityMap
  Projections
  Registration
  Tutorials
)

IF(OTB_USE_VISU)
        SUBDIRS(Visu)
ENDIF(OTB_USE_VISU)

IF(OTB_USE_PATENTED)
  SUBDIRS( Patented )
ENDIF(OTB_USE_PATENTED)

#Recopie du fichier README.txt dans l'arborescence BINARY
IF( EXISTS ${OTB_BINARY_DIR}/Examples/README.txt )
ELSE( EXISTS ${OTB_BINARY_DIR}/Examples/README.txt )
  CONFIGURE_FILE(${OTB_SOURCE_DIR}/Examples/README.txt
    ${OTB_BINARY_DIR}/Examples/README.txt @ONLY IMMEDIATE)
ENDIF( EXISTS ${OTB_BINARY_DIR}/Examples/README.txt )


ELSE(OTB_BINARY_DIR)
  # We are building as a stand-alone project.
  SET(LIBRARY_OUTPUT_PATH ${OTBExamples_BINARY_DIR}/bin CACHE PATH "Single output directory for building all libraries.")
  SET(EXECUTABLE_OUTPUT_PATH ${OTBExamples_BINARY_DIR}/bin CACHE PATH "Single output directory for building all executables.")
  MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)
  SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

  # Configure Dart testing support.
  SET(PROJECT_SOURCE_DIR "${OTBExamples_SOURCE_DIR}/..")
  INCLUDE(${CMAKE_ROOT}/Modules/Dart.cmake)
  SET(PROJECT_SOURCE_DIR "${OTBExamples_SOURCE_DIR}")
  MARK_AS_ADVANCED(TCL_TCLSH)
  IF(BUILD_TESTING)
    ENABLE_TESTING()
  ENDIF(BUILD_TESTING)

  # Find OTB and load its settings.
  FIND_PACKAGE(OTB)
  IF(OTB_FOUND)
    INCLUDE(${OTB_USE_FILE})
    SUBDIRS( 
        BasicFilters
        FeatureExtraction
        DataRepresentation
        IO
        Filtering
        ChangeDetection
        Learning
        Classification
        Segmentation
        Iterators
        MultiScale
        DisparityMap
        Registration
        Tutorials
        )

    IF(OTB_USE_VISU)
        SUBDIRS(Visu)
    ENDIF(OTB_USE_VISU)

    IF(OTB_USE_PATENTED)
        SUBDIRS( Patented )
    ENDIF(OTB_USE_PATENTED)
  ELSE(OTB_FOUND)
    MESSAGE("OTB not found.  Please set OTB_DIR")
  ENDIF(OTB_FOUND)
ENDIF(OTB_BINARY_DIR)

