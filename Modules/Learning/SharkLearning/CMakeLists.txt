project(OTBSharkLearning)

set(OTBSharkLearning_LIBRARIES   
  ${OTBShark_LIBRARIES}
  ${OTBLearningBase_LIBRARIES}
  ${OTBCommon_LIBRARIES} )

otb_module_impl()
