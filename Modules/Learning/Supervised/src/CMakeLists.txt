set(OTBSupervised_SRC
  otbMachineLearningModelFactoryBase.cxx
  otbMachineLearningUtils.cxx
  )

add_library(OTBSupervised ${OTBSupervised_SRC})
target_link_libraries(OTBSupervised 
  ${OTBCommon_LIBRARIES}
  ${OTBSVMLearning_LIBRARIES}
  ${OTBOpenCV_LIBRARIES}
  )

otb_module_target(OTBSupervised)
