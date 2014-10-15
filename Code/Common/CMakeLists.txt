# Sources of non-templated classes.

file(GLOB OTBCommon_SRCS "*.cxx" )

if( NOT OTB_USE_MAPNIK )
    list(REMOVE_ITEM OTBCommon_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/otbVectorDataStyle.cxx" )
endif()

add_library(OTBCommon ${OTBCommon_SRCS})
#Apparently not supported by opensolaris at least
#set_target_properties(OTBCommon
#    PROPERTIES
#    LINK_INTERFACE_LIBRARIES ""
#)
target_link_libraries(OTBCommon ${ITK_LIBRARIES} otbconfigfile ${MUPARSER_LIBRARIES} ${MUPARSERX_LIBRARIES} ${OGR_LIBRARY} OTBOssimAdapters)
if(OTB_USE_MAPNIK)
    target_link_libraries(OTBCommon ${MAPNIK_LIBRARY} ${ICUUC_LIBRARY})
endif()

if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(OTBCommon PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif()

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS OTBCommon
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif()

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  file(GLOB __files3 "${CMAKE_CURRENT_BINARY_DIR}/*.h")
  install(FILES ${__files1} ${__files2} ${__files3}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Common
    COMPONENT Development)
endif()
