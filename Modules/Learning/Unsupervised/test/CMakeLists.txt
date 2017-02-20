otb_module_test()
set(OTBUnsupervisedTests
  otbUnsupervisedTestDriver.cxx
  otbMachineLearningClusteringModelCanRead.cxx
  otbTrainMachineLearningClusteringModel.cxx
  )


add_executable(otbUnsupervisedTestDriver ${OTBUnsupervisedTests})
target_link_libraries(otbUnsupervisedTestDriver ${OTBUnsupervised-Test_LIBRARIES})
otb_module_target_label(otbUnsupervisedTestDriver)

# Tests Declaration

if(OTB_USE_SHARK)
  include(tests-shark.cmake)
endif()
