set(OTBStatistics_SRC
  otbSamplerBase.cxx
  otbPeriodicSampler.cxx
  otbPatternSampler.cxx
  otbRandomSampler.cxx
  )

add_library(OTBStatistics ${OTBStatistics_SRC})
target_link_libraries(OTBStatistics
  ${OTBCommon_LIBRARIES}
  ${OTBITK_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
  ${OTBProjection_LIBRARIES}
  ${OTBStreaming_LIBRARIES}
  )

otb_module_target(OTBStatistics)
