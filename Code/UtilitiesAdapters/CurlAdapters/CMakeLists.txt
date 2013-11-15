# Sources of non-templated classes.

file(GLOB OTBCurlAdapters_SRCS "*.cxx" )

add_library(OTBCurlAdapters ${OTBCurlAdapters_SRCS})
target_link_libraries(OTBCurlAdapters ${ITK_LIBRARIES})

if(OTB_USE_CURL)
  include_directories(${CURL_INCLUDE_DIR})
  target_link_libraries(OTBCurlAdapters ${CURL_LIBRARY})
endif(OTB_USE_CURL)

if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(OTBCurlAdapters PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif(OTB_LIBRARY_PROPERTIES)

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS OTBCurlAdapters
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif(NOT OTB_INSTALL_NO_LIBRARIES)

# Note: no txx allowed here
if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  install(FILES ${__files1}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/UtilitiesAdapters/CurlAdapters
    COMPONENT Development)
endif(NOT OTB_INSTALL_NO_DEVELOPMENT)
