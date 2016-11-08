set(OTBSupervised_SRC
  otbMachineLearningModelFactoryBase.cxx
  )

if(OTB_USE_OPENCV)
list(APPEND OTBSupervised_SRC otbCvRTreesWrapper.cxx)
endif()

add_library(OTBSupervised ${OTBSupervised_SRC})
target_link_libraries(OTBSupervised
  ${OTBCommon_LIBRARIES}
  ${OTBSVMLearning_LIBRARIES}
  ${OTBOpenCV_LIBRARIES}
  ${OTBShark_LIBRARIES}
  )

otb_module_target(OTBSupervised)
