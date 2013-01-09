MESSAGE(STATUS "Importing Mapnik...")
OPTION(OTB_USE_MAPNIK "Use mapnik library." OFF)
MARK_AS_ADVANCED(OTB_USE_MAPNIK)

IF(OTB_USE_MAPNIK)
        IF(NOT OTB_USE_EXTERNAL_BOOST)
          MESSAGE(FATAL_ERROR "You need to set OTB_USE_EXTERNAL_BOOST to ON when using MAPNIK")
        ENDIF(NOT OTB_USE_EXTERNAL_BOOST)

        FIND_PATH(MAPNIK_INCLUDE_DIR mapnik/map.hpp PATHS)
        MARK_AS_ADVANCED(MAPNIK_INCLUDE_DIR)

        FIND_LIBRARY(MAPNIK_LIBRARY NAMES mapnik2 mapnik)
        MARK_AS_ADVANCED(MAPNIK_LIBRARY)

        FIND_PATH(FREETYPE2_INCLUDE_DIR freetype/freetype.h PATHS /usr/include/freetype2)
        MARK_AS_ADVANCED(FREETYPE2_INCLUDE_DIR)

        # libicuuc (http://site.icu-project.org/) is a unicode library mapnik relies on.
        # mapnik includes drag icu includes
        # when linking with mapnik, need to link with libicuuc (at least since 1.42 release)
        FIND_PATH(ICUUC_INCLUDE_DIR unicode/unistr.h)
        MARK_AS_ADVANCED(ICUUC_INCLUDE_DIR)

        FIND_LIBRARY(ICUUC_LIBRARY icuuc)
        MARK_AS_ADVANCED(ICUUC_LIBRARY)

        # ltdl (libtool dl)
        FIND_PATH(LTDL_INCLUDE_DIR ltdl.h)
        MARK_AS_ADVANCED(LTDL_INCLUDE_DIR)

        FIND_LIBRARY(LTDL_LIBRARY ltdl)
        MARK_AS_ADVANCED(LTDL_LIBRARY)

        IF (NOT MAPNIK_INCLUDE_DIR)
                MESSAGE(FATAL_ERROR
                        "Cannot find MAPNIK include directory. Please set MAPNIK_INCLUDE_DIR or set OTB_USE_MAPNIK OFF.")
        ENDIF (NOT MAPNIK_INCLUDE_DIR)
        IF (NOT MAPNIK_LIBRARY)
                MESSAGE(FATAL_ERROR
                        "Cannot find MAPNIK library. Please set MAPNIK_LIBRARY or set OTB_USE_MAPNIK OFF.")
        ENDIF (NOT MAPNIK_LIBRARY)

        IF (NOT FREETYPE2_INCLUDE_DIR)
                MESSAGE(FATAL_ERROR
                        "Cannot find FREETYPE2 include directory. Please set FREETYPE2_INCLUDE_DIR or set OTB_USE_MAPNIK OFF.")
        ENDIF (NOT FREETYPE2_INCLUDE_DIR)

        IF (NOT ICUUC_INCLUDE_DIR)
                MESSAGE(FATAL_ERROR
                        "Cannot find ICUUC_INCLUDE_DIR include directory. Please set ICUUC_INCLUDE_DIR or set OTB_USE_MAPNIK OFF.")
        ENDIF (NOT ICUUC_INCLUDE_DIR)
        IF (NOT ICUUC_LIBRARY)
                MESSAGE(FATAL_ERROR
                        "Cannot find ICUUC library, needed by MAPNIK. Please set ICUUC_LIBRARY or set OTB_USE_MAPNIK OFF.")
        ENDIF (NOT ICUUC_LIBRARY)

        IF (NOT LTDL_INCLUDE_DIR)
                MESSAGE(FATAL_ERROR
                        "Cannot find LTDL_INCLUDE_DIR include directory. Please set LTDL_INCLUDE_DIR or set OTB_USE_MAPNIK OFF.")
        ENDIF (NOT LTDL_INCLUDE_DIR)
        IF (NOT LTDL_LIBRARY)
                MESSAGE(FATAL_ERROR
                        "Cannot find ICUUC library, needed by MAPNIK. Please set ICUUC_LIBRARY or set OTB_USE_MAPNIK OFF.")
        ENDIF (NOT LTDL_LIBRARY)


        # Validating the config
        TRY_COMPILE(OTB_MAPNIK_SUPPORTS_API20
          ${CMAKE_CURRENT_BINARY_DIR}/CMake
          ${CMAKE_CURRENT_SOURCE_DIR}/CMake/otbTestCompileMapnik2.cxx
          CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${MAPNIK_INCLUDE_DIR};${ICUUC_INCLUDE_DIR};${LTDL_INCLUDE_DIR};${FREETYPE2_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${MAPNIK_LIBRARY};${ICUUC_LIBRARY}"
          OUTPUT_VARIABLE OUTPUT)
        IF(OTB_MAPNIK_SUPPORTS_API20)
          MESSAGE(STATUS "  Testing if Mapnik2     -- yes")
        ELSE(OTB_MAPNIK_SUPPORTS_API20)
          MESSAGE(STATUS "  Testing if Mapnik2     -- no")
          #MESSAGE(STATUS "Does not support mapnik2 interface: ${OUTPUT}")
          MESSAGE(STATUS "  Assuming mapnik 0.7")
          # This should be dropped when we don't want to support this any more
          # Estimated date: 02/2013.
          ADD_DEFINITIONS(-DOTB_MAPNIK_COMPATIBILITY_API07)
        ENDIF(OTB_MAPNIK_SUPPORTS_API20)

        # Add compiler option
        ADD_DEFINITIONS(-DOTB_USE_MAPNIK)

        INCLUDE_DIRECTORIES(${MAPNIK_INCLUDE_DIR} ${ICUUC_INCLUDE_DIR} ${LTDL_INCLUDE_DIR} ${FREETYPE2_INCLUDE_DIR})

        MESSAGE(STATUS "  Enabling Mapnik support")
        MESSAGE(STATUS "  Mapnik includes : ${MAPNIK_INCLUDE_DIR}")
        MESSAGE(STATUS "  Mapnik library  : ${MAPNIK_LIBRARY}")
        MESSAGE(STATUS "  ICU includes : ${ICUUC_INCLUDE_DIR}")
        MESSAGE(STATUS "  ICU library  : ${ICUUC_LIBRARY}")
        MESSAGE(STATUS "  LTDL includes : ${LTDL_INCLUDE_DIR}")
        MESSAGE(STATUS "  LTDL library  : ${LTDL_LIBRARY}")

ELSE(OTB_USE_MAPNIK)

    MESSAGE(STATUS "  Disabling Mapnik support")

ENDIF(OTB_USE_MAPNIK)
