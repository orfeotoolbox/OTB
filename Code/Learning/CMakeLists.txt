# Sources of non-templated classes.


file(GLOB OTBLearning_SRCS "*.cxx" )

add_library(OTBLearning ${OTBLearning_SRCS})
target_link_libraries(OTBLearning OTBCommon ${ITK_LIBRARIES} otbsvm)

if(OTB_USE_OPENCV)
  target_link_libraries(OTBLearning OTBOpenCVAdapters)
endif()

if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(OTBLearning PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif()

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS OTBLearning
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif()

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  install(FILES ${__files1} ${__files2}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Learning
    COMPONENT Development)
endif()
