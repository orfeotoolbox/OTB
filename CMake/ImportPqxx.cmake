MESSAGE(STATUS "Importing Pqxx...")

OPTION(OTB_USE_PQXX "Use pqxx library (EXPERIMENTAL)." OFF)
MARK_AS_ADVANCED(OTB_USE_PQXX)

IF(OTB_USE_PQXX)
  FIND_PACKAGE(Pqxx)
  IF(PQXX_FOUND)
    MESSAGE(STATUS "Activating Pqxx support")
    ADD_DEFINITIONS(-DOTB_USE_PQXX)
  ELSE(PQXX_FOUND)
    # Generate an error if no external Pqxx is available
    MESSAGE(FATAL_ERROR "Pqxx required but not found. "
                        "Turn OTB_USE_PQXX to OFF")
  ENDIF(PQXX_FOUND)
ELSE(OTB_USE_PQXX)
  MESSAGE(STATUS "Disabling Pqxx support")
ENDIF(OTB_USE_PQXX)
