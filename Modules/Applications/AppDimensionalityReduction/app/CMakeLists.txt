set(OTBAppDimensionalityReduction_LINK_LIBS
  ${OTBImageManipulation_LIBRARIES}
  ${OTBStatistics_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBDimensionalityReduction_LIBRARIES}
)

otb_create_application(
  NAME           DimensionalityReduction
  SOURCES        otbDimensionalityReduction.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
