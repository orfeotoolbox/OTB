MESSAGE(STATUS "Importing Pqxx...")
OPTION(OTB_USE_PQXX "Use pqxx library (EXPERIMENTAL)." OFF)
MARK_AS_ADVANCED(OTB_USE_PQXX)
IF(OTB_USE_PQXX)

        FIND_PATH(PQXX_INCLUDE_DIR pqxx/pqxx PATHS)
        MARK_AS_ADVANCED(PQXX_INCLUDE_DIR)
        IF (NOT PQXX_INCLUDE_DIR)
                MESSAGE(FATAL_ERROR
                        "Cannot find PQXX include directory. Please set PQXX_INCLUDE_DIR or SET OTB_USE_PQXX OFF.")
        ENDIF (NOT PQXX_INCLUDE_DIR)


        FIND_LIBRARY(PQXX_LIBRARY pqxx )
        MARK_AS_ADVANCED(PQXX_LIBRARY)
        IF (NOT PQXX_LIBRARY)
                MESSAGE(FATAL_ERROR
                        "Cannot find PQXX library. Please set PQXX_LIBRARY or SET OTB_USE_PQXX OFF.")
        ENDIF (NOT PQXX_LIBRARY)

        FIND_LIBRARY(PQ_LIBRARY pq )
        MARK_AS_ADVANCED(PQ_LIBRARY)
        IF (NOT PQ_LIBRARY)
                MESSAGE(FATAL_ERROR
                        "Cannot find PQ library. Please set PQ_LIBRARY or SET OTB_USE_PQXX OFF.")
        ENDIF (NOT PQ_LIBRARY)


        # Add compiler option
        ADD_DEFINITIONS(-DOTB_USE_PQXX)

        INCLUDE_DIRECTORIES(${PQXX_INCLUDE_DIR})
        
        MESSAGE(STATUS "  Enabling Pqxx support")
        MESSAGE(STATUS "  Pqxx includes : ${PQXX_INCLUDE_DIR}")
        MESSAGE(STATUS "  Pqxx library  : ${PQXX_LIBRARY}")
        
ELSE(OTB_USE_PQXX)

    MESSAGE(STATUS "  Disabling Pqxx support")

ENDIF(OTB_USE_PQXX)
