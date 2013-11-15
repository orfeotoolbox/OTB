# Sources of non-templated classes.

file(GLOB OTBMultiTemporal_SRCS "*.cxx" )

add_library(OTBMultiTemporal ${OTBMultiTemporal_SRCS})
target_link_libraries(OTBMultiTemporal OTBCommon ${ITK_LIBRARIES})
if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(OTBMultiTemporal PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif(OTB_LIBRARY_PROPERTIES)

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS OTBMultiTemporal
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif(NOT OTB_INSTALL_NO_LIBRARIES)

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  install(FILES ${__files1} ${__files2}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/MultiTemporal
    COMPONENT Development)
endif(NOT OTB_INSTALL_NO_DEVELOPMENT)
