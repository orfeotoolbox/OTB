
# Sources of non-templated classes.
file(GLOB OTBRadiometry_SRCS "*.cxx" )

add_library(OTBRadiometry ${OTBRadiometry_SRCS})

#Note: depend on OTBIO instead of otbossim to keep the correct order between gdal and ossim
target_link_libraries(OTBRadiometry OTBCommon otb6S OTBIO)
if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(OTBRadiometry PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif(OTB_LIBRARY_PROPERTIES)

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS OTBRadiometry
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif(NOT OTB_INSTALL_NO_LIBRARIES)

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  install(FILES ${__files1} ${__files2}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Radiometry
    COMPONENT Development)
endif(NOT OTB_INSTALL_NO_DEVELOPMENT)
