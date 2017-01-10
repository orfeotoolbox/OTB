otb_module_test()
set(OTBSupervisedTests
otbSupervisedTestDriver.cxx
otbConfusionMatrixCalculatorTest.cxx
otbConfusionMatrixMeasurementsTest.cxx
otbMachineLearningModelCanRead.cxx
otbTrainMachineLearningModel.cxx
otbImageClassificationFilter.cxx
otbMachineLearningRegressionTests.cxx
)

if(OTB_USE_SHARK)
set(OTBSupervisedTests ${OTBSupervisedTests} otbSharkImageClassificationFilter.cxx)
endif()

add_executable(otbSupervisedTestDriver ${OTBSupervisedTests})
target_link_libraries(otbSupervisedTestDriver ${OTBSupervised-Test_LIBRARIES})
otb_module_target_label(otbSupervisedTestDriver)

# Tests Declaration

otb_add_test(NAME leTvConfusionMatrixCalculatorWrongSize COMMAND otbSupervisedTestDriver
  otbConfusionMatrixCalculatorWrongSize 1000 4)
set_property(TEST leTvConfusionMatrixCalculatorWrongSize PROPERTY WILL_FAIL true)

otb_add_test(NAME leTvConfusionMatrixCalculatorUpdate COMMAND otbSupervisedTestDriver
  otbConfusionMatrixCalculatorCompute 4 3)

otb_add_test(NAME leTvConfusionMatrixCalculatorUpdateWithBaseline COMMAND otbSupervisedTestDriver
  otbConfusionMatrixCalculatorComputeWithBaseline)

otb_add_test(NAME leTvConfusionMatrixCalculatorSetListSamples COMMAND otbSupervisedTestDriver
  otbConfusionMatrixCalculatorSetListSamples 1000 4)

otb_add_test(NAME leTuConfusionMatrixCalculatorNew COMMAND otbSupervisedTestDriver
  otbConfusionMatrixCalculatorNew)

otb_add_test(NAME leTvConfusionMatrixMeasurementsTest COMMAND otbSupervisedTestDriver
  otbConfusionMatrixMeasurementsTest)

otb_add_test(NAME leTuConfusionMatrixMeasurementsNew COMMAND otbSupervisedTestDriver
  otbConfusionMatrixMeasurementsNew)

otb_add_test(NAME leTvConfusionMatrixConcatenateTest COMMAND otbSupervisedTestDriver
  otbConfusionMatrixConcatenateTest
  ${INPUTDATA}/Classification/QB_1_ortho_C1.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C2.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C3.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C4.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C5.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C6.csv)

if(OTB_USE_LIBSVM)
  include(tests-libsvm.cmake)
endif()

if(OTB_USE_OPENCV)
  include(tests-opencv.cmake)
endif()

if(OTB_USE_SHARK)
  include(tests-shark.cmake)
endif()
