set(OTBAppFiltering_LINK_LIBS
  ${OTBImageNoise_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
)

otb_create_application(
  NAME           Despeckle
  SOURCES        otbDespeckle.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           Smoothing
  SOURCES        otbSmoothing.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
