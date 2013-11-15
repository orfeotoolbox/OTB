message(STATUS "Importing Curl...")
find_package(CURL)
if(CURL_FOUND)
  option(OTB_USE_CURL "Use curl library." ON)
else(CURL_FOUND)
  option(OTB_USE_CURL "Use curl library." OFF)
endif(CURL_FOUND)

mark_as_advanced(OTB_USE_CURL)
if(OTB_USE_CURL)
    if(CURL_FOUND)
      message(STATUS "Using CURL external version")
      try_compile(OTB_CURL_MULTI_AVAILABLE
                    ${CMAKE_CURRENT_BINARY_DIR}/CMake
                    ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestCurlMulti.cxx
                    CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${CURL_INCLUDE_DIRS}" "-DLINK_LIBRARIES:STRING=${CURL_LIBRARIES}"
                    OUTPUT_VARIABLE OUTPUT)
    else(CURL_FOUND)
      message(FATAL_ERROR "CURL required but not found. "
                          "Turn OTB_USE_CURL to OFF to disable CURL support")
    endif(CURL_FOUND)
else(OTB_USE_CURL)
        message(STATUS "  Disabling Curl support")
endif(OTB_USE_CURL)
