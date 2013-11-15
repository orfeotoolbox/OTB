file(GLOB OTBProjections_SRCS "*.cxx" )

if( NOT OTB_USE_CURL )
    list(REMOVE_ITEM OTBProjections_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/otbPlaceNameToLonLat.cxx" )
    list(REMOVE_ITEM OTBProjections_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/otbCoordinateToName.cxx" )
endif( NOT OTB_USE_CURL )

add_library(OTBProjections ${OTBProjections_SRCS})
target_link_libraries(OTBProjections OTBIO OTBOGRAdapters)

if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(OTBProjections PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif(OTB_LIBRARY_PROPERTIES)

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS OTBProjections
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif(NOT OTB_INSTALL_NO_LIBRARIES)

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  if( NOT OTB_USE_CURL )
    list(REMOVE_ITEM __files1 "${CMAKE_CURRENT_SOURCE_DIR}/otbPlaceNameToLonLat.h" )
    list(REMOVE_ITEM __files1 "${CMAKE_CURRENT_SOURCE_DIR}/otbCoordinateToName.h" )
  endif( NOT OTB_USE_CURL )
  install(FILES ${__files1} ${__files2}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Projections
    COMPONENT Development)
endif(NOT OTB_INSTALL_NO_DEVELOPMENT)
