# Sources of non-templated classes.
file(GLOB OTBOpenCVAdapters_SRCS "foo.cxx" )

add_library(OTBOpenCVAdapters ${OTBOpenCVAdapters_SRCS})
target_link_libraries(OTBOpenCVAdapters ${OpenCV_LIBS})
if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(OTBOpenCVAdapters PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif()

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS OTBOpenCVAdapters
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif()

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  install(FILES ${__files1} ${__files2}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/UtilitiesAdapters/OpenCVAdapters
    COMPONENT Development)
endif()
