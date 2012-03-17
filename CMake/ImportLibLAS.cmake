MESSAGE(STATUS "Importing LibLAS...")
OPTION(OTB_USE_LIBLAS "Use liblas library to support Lidar data format." ON)
MARK_AS_ADVANCED(OTB_USE_LIBLAS)

IF(OTB_USE_LIBLAS)
  FIND_PACKAGE(LibLAS)

  # Initialize option with what's found on the system
  IF(LIBLAS_FOUND)
    OPTION(OTB_USE_EXTERNAL_LIBLAS "Use external LIBLAS library." ON)
  ELSE(LIBLAS_FOUND)
    OPTION(OTB_USE_EXTERNAL_LIBLAS "Use external LIBLAS library." OFF)
  ENDIF(LIBLAS_FOUND)
  MARK_AS_ADVANCED(OTB_USE_EXTERNAL_LIBLAS)
  
  IF(OTB_USE_EXTERNAL_LIBLAS)
    IF(LIBLAS_FOUND)
      MESSAGE(STATUS "Using LibLAS external version")
    ELSE(LIBLAS_FOUND)
      # Generate an error if no external liblas is available
      MESSAGE(FATAL_ERROR "LibLAS required but not found. "
                          "Either turn OTB_USE_EXTERNAL_LIBLAS to OFF to use the internal version, "
                          "or OTB_USE_LIBLAS to OFF to disable LAS support")
    ENDIF(LIBLAS_FOUND)
  ELSE(OTB_USE_EXTERNAL_LIBLAS)
    MESSAGE(STATUS "Using LibLAS internal version")
    SET(LIBLAS_LIBRARIES otbliblas)
    ADD_DEFINITIONS(-DLAS_DISABLE_DLL)
  ENDIF(OTB_USE_EXTERNAL_LIBLAS)
  
ENDIF(OTB_USE_LIBLAS)

