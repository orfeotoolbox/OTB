set(OTBLearningBase_SRC
  otbMachineLearningModelFactoryBase.cxx
  )

add_library(OTBLearningBase ${OTBLearningBase_SRC})
target_link_libraries(OTBLearningBase 
  ${OTBCommon_LIBRARIES}
  ${OTBSVMLearning_LIBRARIES}
  ${OTBOpenCVAdapters_LIBRARIES}
  )

otb_module_target(OTBLearningBase)
