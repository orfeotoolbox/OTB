# Sources of non-templated classes.

file(GLOB OTBOssimAdapters_SRCS "*.cxx" )

if(OTB_USE_EXTERNAL_OSSIM)
  include_directories( ${OSSIM_INCLUDE_DIR} )
else()
  include_directories( ${OTB_SOURCE_DIR}/Utilities/otbossim/include
                       ${OTB_BINARY_DIR}/Utilities/otbossim/include )
endif()

add_library(OTBOssimAdapters ${OTBOssimAdapters_SRCS})
target_link_libraries(OTBOssimAdapters ${OSSIM_LIBRARIES} otbossimplugins ${ITK_LIBRARIES})
if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(OTBOssimAdapters PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif()

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS OTBOssimAdapters
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif()

# Note: no txx allowed here
if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  install(FILES ${__files1}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/UtilitiesAdapters/OssimAdapters
    COMPONENT Development)
endif()
