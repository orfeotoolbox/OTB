otb_module_test()
set(OTBSharkLearningTests
  otbSharkLearningTestDriver.cxx
  otbSharkMachineLearningModelCanRead.cxx
  otbSharkTrainMachineLearningModel.cxx
  otbSharkImageClassificationFilter.cxx
  )

add_executable(otbSharkLearningTestDriver ${OTBSharkLearningTests})
target_link_libraries(otbSharkLearningTestDriver 
  ${OTBSharkLearning_LIBRARIES} ${OTBSharkLearning-Test_LIBRARIES}  )
otb_module_target_label(otbSharkLearningTestDriver)

# Tests Declaration

if(OTB_USE_SHARK)
  include(tests-shark.cmake)
endif()
