otb_module_test()
set(OTBUnsupervisedTests
  otbUnsupervisedTestDriver.cxx
  otbMachineLearningUnsupervisedModelCanRead.cxx
  otbTrainMachineLearningUnsupervisedModel.cxx
  otbContingencyTableCalculatorTest.cxx
  )

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
  set(OTBUnsupervisedTests ${OTBUnsupervisedTests} otbSharkUnsupervisedImageClassificationFilter.cxx)
  include(tests-shark.cmake)
endif()

add_executable(otbUnsupervisedTestDriver ${OTBUnsupervisedTests})
target_link_libraries(otbUnsupervisedTestDriver ${OTBUnsupervised-Test_LIBRARIES})
otb_module_target_label(otbUnsupervisedTestDriver)
