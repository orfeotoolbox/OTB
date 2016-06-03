set(OTBSampling_SRC
  otbSamplingRateCalculator.cxx
)

add_library(OTBSampling ${OTBSampling_SRC})
target_link_libraries(OTBSampling
  ${OTBCommon_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBStatistics_LIBRARIES}
  ${OTBIOGDAL_LBRARIES}
  )

otb_module_target(OTBSampling)
