otb_module_test()

#${otb-module} will be the name of this module and will not need to be
#changed when this module is renamed.

set(${otb-module}Tests
  otbMPIModuleTestDriver.cxx
  otbMPIHelloTest.cxx
)

add_executable(otbMPIModuleTestDriver ${${otb-module}Tests})
target_link_libraries(otbMPIModuleTestDriver ${${otb-module}-Test_LIBRARIES} ${OTBCommon_LIBRARIES} ${OTBMPI_LIBRARIES})
otb_module_target_label(otbMPIModuleTestDriver)

# Test MPIHello
otb_add_test(NAME otbMPIHelloTest
  COMMAND otbMPIModuleTestDriver otbMPIHelloTest)

#test application
otb_test_application(NAME otbMPIHelloAppTest
  APP  MPIHelloApp)
