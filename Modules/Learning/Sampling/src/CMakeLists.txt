set(OTBSampling_SRC
  otbPolygonClassStatisticsAccumulator.cxx
  )

add_library(OTBSampling ${OTBSampling_SRC})
target_link_libraries(OTBSampling
  ${OTBCommon_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBStatistics_LIBRARIES}
  )

otb_module_target(OTBSampling)
