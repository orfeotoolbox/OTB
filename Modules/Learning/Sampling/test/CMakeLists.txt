otb_module_test()

set(OTBSamplingTests
otbSamplingTestDriver.cxx
otbOGRDataToClassStatisticsFilterTest.cxx
)

add_executable(otbSamplingTestDriver ${OTBSamplingTests})
target_link_libraries(otbSamplingTestDriver ${OTBSampling-Test_LIBRARIES})
otb_module_target_label(otbSamplingTestDriver)

# Tests Declaration

otb_add_test(NAME leTuOGRDataToClassStatisticsFilterNew COMMAND otbSamplingTestDriver
  otbOGRDataToClassStatisticsFilterNew )
