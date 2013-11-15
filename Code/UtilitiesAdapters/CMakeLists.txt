add_subdirectory(CurlAdapters)
add_subdirectory(OssimAdapters)
add_subdirectory(TinyXMLAdapters)
add_subdirectory(OGRAdapters)
add_subdirectory(boost)
add_subdirectory(ITKPendingPatches)
if(OTB_USE_OPENCV)
  add_subdirectory(OpenCVAdapters)
endif()

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  install(FILES ${__files1}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/UtilitiesAdapters
    COMPONENT Development)
endif(NOT OTB_INSTALL_NO_DEVELOPMENT)
