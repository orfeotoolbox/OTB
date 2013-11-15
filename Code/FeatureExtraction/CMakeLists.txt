# Sources of non-templated classes.


file(GLOB OTBFeatureExtraction_SRCS "*.cxx" )

add_library(OTBFeatureExtraction ${OTBFeatureExtraction_SRCS})
target_link_libraries(OTBFeatureExtraction OTBCommon)
if(OTB_USE_SIFTFAST)
  target_link_libraries(OTBFeatureExtraction otbsiftfast)
endif(OTB_USE_SIFTFAST)

if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(OTBFeatureExtraction PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif(OTB_LIBRARY_PROPERTIES)

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS OTBFeatureExtraction
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif(NOT OTB_INSTALL_NO_LIBRARIES)

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  if( NOT OTB_USE_SIFTFAST )
    list(REMOVE_ITEM __files1 "${CMAKE_CURRENT_SOURCE_DIR}/otbSiftFastImageFilter.h" )
    list(REMOVE_ITEM __files2 "${CMAKE_CURRENT_SOURCE_DIR}/otbSiftFastImageFilter.txx" )
  endif( NOT OTB_USE_SIFTFAST )

  install(FILES ${__files1} ${__files2}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/FeatureExtraction
    COMPONENT Development)
endif(NOT OTB_INSTALL_NO_DEVELOPMENT)
