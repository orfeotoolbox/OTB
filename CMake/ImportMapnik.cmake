MESSAGE(STATUS "Importing Mapnik...")
OPTION(OTB_USE_MAPNIK "Use mapnik library." OFF)
MARK_AS_ADVANCED(OTB_USE_MAPNIK)

IF(OTB_USE_MAPNIK)
        IF(NOT OTB_USE_EXTERNAL_BOOST)
          MESSAGE(FATAL_ERROR "You need to set OTB_USE_EXTERNAL_BOOST to ON when using MAPNIK")
        ENDIF(NOT OTB_USE_EXTERNAL_BOOST)

        FIND_PATH(MAPNIK_INCLUDE_DIR mapnik/map.hpp PATHS)
        MARK_AS_ADVANCED(MAPNIK_INCLUDE_DIR)
        IF (NOT MAPNIK_INCLUDE_DIR)
                MESSAGE(FATAL_ERROR
                        "Cannot find MAPNIK include directory. Please set MAPNIK_INCLUDE_DIR or set OTB_USE_MAPNIK OFF.")
        ENDIF (NOT MAPNIK_INCLUDE_DIR)

        FIND_PATH(FREETYPE2_INCLUDE_DIR freetype/freetype.h PATHS /usr/include/freetype2)
        MARK_AS_ADVANCED(FREETYPE2_INCLUDE_DIR)
        IF (NOT FREETYPE2_INCLUDE_DIR)
                MESSAGE(FATAL_ERROR
                        "Cannot find FREETYPE2 include directory. Please set  or set OTB_USE_MAPNIK OFF.")
        ENDIF (NOT FREETYPE2_INCLUDE_DIR)

        FIND_LIBRARY(MAPNIK_LIBRARY mapnik )
        MARK_AS_ADVANCED(MAPNIK_LIBRARY)
        IF (NOT MAPNIK_LIBRARY)
                MESSAGE(FATAL_ERROR
                        "Cannot find MAPNIK library. Please set MAPNIK_LIBRARY or set OTB_USE_MAPNIK OFF.")
        ENDIF (NOT MAPNIK_LIBRARY)

        # libicuuc is a unicode library mapnik relies on.
        # since libicuuc version 1.42, we need to link against it when linking against mapnik
        # otherwise we get undefined symbol error
        FIND_LIBRARY(ICUUC_LIBRARY icuuc )
        MARK_AS_ADVANCED(ICUUC_LIBRARY)
        IF (NOT ICUUC_LIBRARY)
                MESSAGE(FATAL_ERROR
                        "Cannot find ICUUC library, needed by MAPNIK. Please set ICUUC_LIBRARY or set OTB_USE_MAPNIK OFF.")
        ENDIF (NOT ICUUC_LIBRARY)

        # Add compiler option
        ADD_DEFINITIONS(-DOTB_USE_MAPNIK)

        INCLUDE_DIRECTORIES(${MAPNIK_INCLUDE_DIR})
        
        MESSAGE(STATUS "  Enabling Mapnik support")
        MESSAGE(STATUS "  Mapnik includes : ${MAPNIK_INCLUDE_DIR}")
        MESSAGE(STATUS "  Mapnik library  : ${MAPNIK_LIBRARY}")
        
ELSE(OTB_USE_MAPNIK)

    MESSAGE(STATUS "  Disabling Mapnik support")
    
ENDIF(OTB_USE_MAPNIK)
