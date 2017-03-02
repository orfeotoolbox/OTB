otb_module_test()
set(OTBUnsupervisedTests
  otbUnsupervisedTestDriver.cxx
  otbMachineLearningClusteringModelCanRead.cxx
  otbTrainMachineLearningClusteringModel.cxx
  otbContingencyTableCalculatorTest.cxx
  )


add_executable(otbUnsupervisedTestDriver ${OTBUnsupervisedTests})
target_link_libraries(otbUnsupervisedTestDriver ${OTBUnsupervised-Test_LIBRARIES})
otb_module_target_label(otbUnsupervisedTestDriver)

# Tests Declaration

otb_add_test(NAME leTuContingencyTableCalculatorNew COMMAND otbUnsupervisedTestDriver
  otbContingencyTableCalculatorNew)

otb_add_test(NAME leTvContingencyTableCalculatorSetListSamples COMMAND otbUnsupervisedTestDriver
  otbContingencyTableCalculatorSetListSamples 1000 4)

otb_add_test(NAME leTvContingencyTableCalculatorUpdate COMMAND otbUnsupervisedTestDriver
  otbContingencyTableCalculatorCompute 4 3)

otb_add_test(NAME leTvContingencyTableCalculatorUpdateWithBaseline COMMAND otbUnsupervisedTestDriver
  otbContingencyTableCalculatorComputeWithBaseline)


if(OTB_USE_SHARK)
  include(tests-shark.cmake)
endif()
