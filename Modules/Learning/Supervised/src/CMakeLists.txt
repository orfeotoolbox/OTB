set(OTBSupervised_SRC
  otbCvRTreesWrapper.cxx
  otbMachineLearningModelFactoryBase.cxx
  )

add_library(OTBSupervised ${OTBSupervised_SRC})
target_link_libraries(OTBSupervised
  ${OTBCommon_LIBRARIES}
  ${OTBSVMLearning_LIBRARIES}
  ${OTBOpenCV_LIBRARIES}
  )

otb_module_target(OTBSupervised)
