MESSAGE(STATUS "Importing Curl...")
FIND_PACKAGE(CURL)
IF(CURL_FOUND)
  OPTION(OTB_USE_CURL "Use curl library." ON)
ELSE(CURL_FOUND)
  OPTION(OTB_USE_CURL "Use curl library." OFF)
ENDIF(CURL_FOUND)

MARK_AS_ADVANCED(OTB_USE_CURL)
IF(OTB_USE_CURL)

        FIND_PATH(CURL_INCLUDE_DIR curl/curl.h PATHS)
        MARK_AS_ADVANCED(CURL_INCLUDE_DIR)
        IF (NOT CURL_INCLUDE_DIR)
                MESSAGE(FATAL_ERROR
                        "Cannot find CURL include directory. Please set CURL_INCLUDE_DIR or SET OTB_USE_CURL OFF.")
        ENDIF (NOT CURL_INCLUDE_DIR)

        FIND_LIBRARY(CURL_LIBRARY curl )
        MARK_AS_ADVANCED(CURL_LIBRARY)
        IF (NOT CURL_LIBRARY)
                MESSAGE(FATAL_ERROR
                        "Cannot find CURL library. Please set CURL_LIBRARY or SET OTB_USE_CURL OFF.")
        ENDIF (NOT CURL_LIBRARY)

        TRY_COMPILE(OTB_CURL_MULTI_AVAILABLE
                    ${CMAKE_CURRENT_BINARY_DIR}/CMake
                    ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestCurlMulti.cxx
                    CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${CURL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${CURL_LIBRARY}"
                    OUTPUT_VARIABLE OUTPUT)
         
        MESSAGE(STATUS "  Enabling Curl support")
        MESSAGE(STATUS "  Curl includes : ${CURL_INCLUDE_DIR}")
        MESSAGE(STATUS "  Curl library  : ${CURL_LIBRARY}")

ELSE(OTB_USE_CURL)    
        MESSAGE(STATUS "  Disabling Curl support")
ENDIF(OTB_USE_CURL)
