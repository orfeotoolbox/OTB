
# Sources of non-templated classes.
file(GLOB OTBTesting_SRCS "*.cxx" )

# Remove the otbTestDriver cause only an executable is nedded
list(REMOVE_ITEM OTBTesting_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/otbTestDriver.cxx" )


add_library(OTBTesting ${OTBTesting_SRCS})
#set_target_properties(OTBTesting
#    PROPERTIES
#    LINK_INTERFACE_LIBRARIES ""
#)
target_link_libraries(OTBTesting OTBBasicFilters OTBIO OTBCommon OTBOGRAdapters)
if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(OTBTesting PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif(OTB_LIBRARY_PROPERTIES)

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS OTBTesting
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif(NOT OTB_INSTALL_NO_LIBRARIES)

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  install(FILES ${__files}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Testing
    COMPONENT Development)
endif(NOT OTB_INSTALL_NO_DEVELOPMENT)



# Compile otbTestDriver
# Needed in the OTB-Wrapping project.
# Has to be compiled even if the BUILD_TEST  are set to OFF
if(CMAKE_COMPILER_IS_GNUCXX)
  set_source_files_properties(otbTestDriver.cxx PROPERTIES COMPILE_FLAGS -w)
endif(CMAKE_COMPILER_IS_GNUCXX)

add_executable(otbTestDriver otbTestDriver.cxx)
target_link_libraries(otbTestDriver OTBIO OTBTesting)
set(ITK_TEST_DRIVER "${EXECUTABLE_OUTPUT_PATH}/otbTestDriver"
    CACHE INTERNAL "otbTestDriver path to be used by subprojects")

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  install(FILES ${__files1} ${__files2}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/IO
    COMPONENT Development)
  install(TARGETS otbTestDriver RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT Development)
endif(NOT OTB_INSTALL_NO_DEVELOPMENT)


