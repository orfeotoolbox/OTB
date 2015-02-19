project(OTBCurl)

try_compile(OTB_CURL_MULTI_AVAILABLE
              ${CMAKE_CURRENT_BINARY_DIR}/CMake
              ${CMAKE_CURRENT_SOURCE_DIR}/CMake/otbTestCurlMulti.cxx
              CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${CURL_INCLUDE_DIRS}" "-DLINK_LIBRARIES:STRING=${CURL_LIBRARIES}"
              OUTPUT_VARIABLE OUTPUT)

configure_file( src/otb_curl.h.in src/otb_curl.h )

set(OTBCurl_SYSTEM_INCLUDE_DIRS ${CURL_INCLUDE_DIRS})
set(OTBCurl_LIBRARIES "${CURL_LIBRARIES}")
set(OTBCurl_INCLUDE_DIRS
  ${OTBCurl_BINARY_DIR}/src
  )

otb_module_impl()

install(FILES ${OTBCurl_BINARY_DIR}/src/otb_curl.h
  DESTINATION ${OTBCurl_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
