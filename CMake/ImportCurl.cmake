MESSAGE(STATUS "Importing Curl...")
FIND_PACKAGE(CURL)
IF(CURL_FOUND)
  OPTION(OTB_USE_CURL "Use curl library." ON)
ELSE(CURL_FOUND)
  OPTION(OTB_USE_CURL "Use curl library." OFF)
ENDIF(CURL_FOUND)

MARK_AS_ADVANCED(OTB_USE_CURL)
IF(OTB_USE_CURL)
    IF(CURL_FOUND)
      MESSAGE(STATUS "Using CURL external version")
      TRY_COMPILE(OTB_CURL_MULTI_AVAILABLE
                    ${CMAKE_CURRENT_BINARY_DIR}/CMake
                    ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestCurlMulti.cxx
                    CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${CURL_INCLUDE_DIRS}" "-DLINK_LIBRARIES:STRING=${CURL_LIBRARIES}"
                    OUTPUT_VARIABLE OUTPUT)
    ELSE(CURL_FOUND)
      MESSAGE(FATAL_ERROR "CURL required but not found. "
                          "Turn OTB_USE_CURL to OFF to disable CURL support")
    ENDIF(CURL_FOUND)
ELSE(OTB_USE_CURL)
        MESSAGE(STATUS "  Disabling Curl support")
ENDIF(OTB_USE_CURL)
