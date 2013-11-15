
# Sources of non-templated classes.
file(GLOB OTBApplicationEngine_SRCS "*.cxx" )

if(WIN32)
  set(CMAKE_C_FLAGS   "/bigobj ${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "/bigobj ${CMAKE_CXX_FLAGS}")
endif()

add_library(OTBApplicationEngine ${OTBApplicationEngine_SRCS})
target_link_libraries(OTBApplicationEngine OTBCommon OTBIO)

if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(OTBApplicationEngine PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif()

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS OTBApplicationEngine
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif()

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  install(FILES ${__files1} ${__files2}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/ApplicationEngine
    COMPONENT Development)
endif()

