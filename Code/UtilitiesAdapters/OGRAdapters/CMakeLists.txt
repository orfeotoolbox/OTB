# Sources of non-templated classes.
file(GLOB OTBOGRAdapters_SRCS "*.cxx" )

add_library(OTBOGRAdapters ${OTBOGRAdapters_SRCS})
target_link_libraries(OTBOGRAdapters OTBCommon ${OGR_LIBRARY})
if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(OTBOGRAdapters PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif(OTB_LIBRARY_PROPERTIES)

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS OTBOGRAdapters
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif(NOT OTB_INSTALL_NO_LIBRARIES)

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  file(GLOB __files3 "${CMAKE_CURRENT_BINARY_DIR}/*.h")
  # install(FILES ${__files1}
  # install(FILES ${__files1} ${__files2}
  install(FILES ${__files1} ${__files2} ${__files3}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/UtilitiesAdapters/OGRAdapters
    COMPONENT Development)
endif(NOT OTB_INSTALL_NO_DEVELOPMENT)
