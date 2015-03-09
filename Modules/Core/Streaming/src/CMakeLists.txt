set(OTBStreaming_SRC
  otbPipelineMemoryPrintCalculator.cxx
  )

add_library(OTBStreaming ${OTBStreaming_SRC})
target_link_libraries(OTBStreaming 
  ${OTBImageBase_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}
  ${OTBCommon_LIBRARIES}

  )

otb_module_target(OTBStreaming)
