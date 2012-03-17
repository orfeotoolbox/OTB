MESSAGE(STATUS "Importing EXPAT...")

FIND_PACKAGE(EXPAT)

# Initialize option with what's found on the system
IF(EXPAT_FOUND)
  OPTION(OTB_USE_EXTERNAL_EXPAT "Use an outside build of EXPAT." ON)
ELSE(EXPAT_FOUND)
  OPTION(OTB_USE_EXTERNAL_EXPAT "Use an outside build of EXPAT." OFF)
ENDIF(EXPAT_FOUND)
MARK_AS_ADVANCED(OTB_USE_EXTERNAL_EXPAT)

# Setup
IF(OTB_USE_EXTERNAL_EXPAT)
    IF(EXPAT_FOUND)
      MESSAGE(STATUS "Using EXPAT external version")
    ELSE(EXPAT_FOUND)
      # Generate an error if no external EXPAT is available
      MESSAGE(FATAL_ERROR "EXPAT required but not found. "
                          "Turn OTB_USE_EXTERNAL_EXPAT to OFF to use the internal version.")
    ENDIF(EXPAT_FOUND)
ELSE(OTB_USE_EXTERNAL_EXPAT)
    MESSAGE(STATUS "Using EXPAT internal version")
    
    SET(EXPAT_LIBRARIES otbexpat)
    ADD_DEFINITIONS(-DOTB_USE_INTERNAL_EXPAT)
    IF(NOT BUILD_SHARED_LIBS)
      ADD_DEFINITIONS(-DXML_STATIC)
    ENDIF(NOT BUILD_SHARED_LIBS)
    INCLUDE_DIRECTORIES(BEFORE "${OTB_SOURCE_DIR}/Utilities/otbexpat")
    INCLUDE_DIRECTORIES(BEFORE "${OTB_BINARY_DIR}/Utilities/otbexpat")
ENDIF(OTB_USE_EXTERNAL_EXPAT)

