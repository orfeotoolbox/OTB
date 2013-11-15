message(STATUS "Importing Mapnik...")
option(OTB_USE_MAPNIK "Use mapnik library." OFF)
mark_as_advanced(OTB_USE_MAPNIK)

if(OTB_USE_MAPNIK)
        if(NOT OTB_USE_EXTERNAL_BOOST)
          message(FATAL_ERROR "You need to set OTB_USE_EXTERNAL_BOOST to ON when using MAPNIK")
        endif()

        find_path(MAPNIK_INCLUDE_DIR mapnik/map.hpp PATHS)
        mark_as_advanced(MAPNIK_INCLUDE_DIR)

        find_library(MAPNIK_LIBRARY NAMES mapnik2 mapnik)
        mark_as_advanced(MAPNIK_LIBRARY)

        find_path(FREETYPE2_INCLUDE_DIR freetype/freetype.h PATHS /usr/include/freetype2)
        mark_as_advanced(FREETYPE2_INCLUDE_DIR)

        # libicuuc (http://site.icu-project.org/) is a unicode library mapnik relies on.
        # mapnik includes drag icu includes
        # when linking with mapnik, need to link with libicuuc (at least since 1.42 release)
        find_path(ICUUC_INCLUDE_DIR unicode/unistr.h)
        mark_as_advanced(ICUUC_INCLUDE_DIR)

        find_library(ICUUC_LIBRARY icuuc)
        mark_as_advanced(ICUUC_LIBRARY)

        # ltdl (libtool dl)
        find_path(LTDL_INCLUDE_DIR ltdl.h)
        mark_as_advanced(LTDL_INCLUDE_DIR)

        find_library(LTDL_LIBRARY ltdl)
        mark_as_advanced(LTDL_LIBRARY)

        if(NOT MAPNIK_INCLUDE_DIR)
                message(FATAL_ERROR
                        "Cannot find MAPNIK include directory. Please set MAPNIK_INCLUDE_DIR or set OTB_USE_MAPNIK OFF.")
        endif()
        if(NOT MAPNIK_LIBRARY)
                message(FATAL_ERROR
                        "Cannot find MAPNIK library. Please set MAPNIK_LIBRARY or set OTB_USE_MAPNIK OFF.")
        endif()

        if(NOT FREETYPE2_INCLUDE_DIR)
                message(FATAL_ERROR
                        "Cannot find FREETYPE2 include directory. Please set FREETYPE2_INCLUDE_DIR or set OTB_USE_MAPNIK OFF.")
        endif()

        if(NOT ICUUC_INCLUDE_DIR)
                message(FATAL_ERROR
                        "Cannot find ICUUC_INCLUDE_DIR include directory. Please set ICUUC_INCLUDE_DIR or set OTB_USE_MAPNIK OFF.")
        endif()
        if(NOT ICUUC_LIBRARY)
                message(FATAL_ERROR
                        "Cannot find ICUUC library, needed by MAPNIK. Please set ICUUC_LIBRARY or set OTB_USE_MAPNIK OFF.")
        endif()

        if(NOT LTDL_INCLUDE_DIR)
                message(FATAL_ERROR
                        "Cannot find LTDL_INCLUDE_DIR include directory. Please set LTDL_INCLUDE_DIR or set OTB_USE_MAPNIK OFF.")
        endif()
        if(NOT LTDL_LIBRARY)
                message(FATAL_ERROR
                        "Cannot find ICUUC library, needed by MAPNIK. Please set ICUUC_LIBRARY or set OTB_USE_MAPNIK OFF.")
        endif()


        # Validating the config
        try_compile(OTB_MAPNIK_SUPPORTS_API20
          ${CMAKE_CURRENT_BINARY_DIR}/CMake
          ${CMAKE_CURRENT_SOURCE_DIR}/CMake/otbTestCompileMapnik2.cxx
          CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${MAPNIK_INCLUDE_DIR};${ICUUC_INCLUDE_DIR};${LTDL_INCLUDE_DIR};${FREETYPE2_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${MAPNIK_LIBRARY};${ICUUC_LIBRARY}"
          OUTPUT_VARIABLE OUTPUT)
        if(OTB_MAPNIK_SUPPORTS_API20)
          message(STATUS "  Testing if Mapnik2     -- yes")
        else()
          message(STATUS "  Testing if Mapnik2     -- no")
          #message(STATUS "Does not support mapnik2 interface: ${OUTPUT}")
          message(STATUS "  Assuming mapnik 0.7")
          # This should be dropped when we don't want to support this any more
          # Estimated date: 02/2013.
          add_definitions(-DOTB_MAPNIK_COMPATIBILITY_API07)
        endif()

        # Add compiler option
        add_definitions(-DOTB_USE_MAPNIK)

        include_directories(${MAPNIK_INCLUDE_DIR} ${ICUUC_INCLUDE_DIR} ${LTDL_INCLUDE_DIR} ${FREETYPE2_INCLUDE_DIR})

        message(STATUS "  Enabling Mapnik support")
        message(STATUS "  Mapnik includes : ${MAPNIK_INCLUDE_DIR}")
        message(STATUS "  Mapnik library  : ${MAPNIK_LIBRARY}")
        message(STATUS "  ICU includes : ${ICUUC_INCLUDE_DIR}")
        message(STATUS "  ICU library  : ${ICUUC_LIBRARY}")
        message(STATUS "  LTDL includes : ${LTDL_INCLUDE_DIR}")
        message(STATUS "  LTDL library  : ${LTDL_LIBRARY}")

else()

    message(STATUS "  Disabling Mapnik support")

endif()
