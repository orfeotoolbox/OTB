otb_module_test()

set(OTBAppTestTests
otbWrapperApplicationDocTests.cxx
otbAppTestTestDriver.cxx
)

add_executable(otbAppTestTestDriver ${OTBAppTestTests})
target_link_libraries(otbAppTestTestDriver ${OTBAppTest-Test_LIBRARIES})
otb_module_target_label(otbAppTestTestDriver)


otb_add_test(NAME apCheckDocumentation COMMAND otbAppTestTestDriver
  otbWrapperApplicationDocTest ${OTB_BINARY_DIR}/lib/otb/applications)
