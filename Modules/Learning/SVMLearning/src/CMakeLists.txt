set(OTBSVMLearning_SRC
  otbExhaustiveExponentialOptimizer.cxx
  )

add_library(OTBSVMLearning ${OTBSVMLearning_SRC})
target_link_libraries(OTBSVMLearning 
  ${OTBVectorDataBase_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBLibSVM_LIBRARIES}
  ${OTBStreaming_LIBRARIES}
  ${OTBCommon_LIBRARIES}

  )

otb_module_target(OTBSVMLearning)
