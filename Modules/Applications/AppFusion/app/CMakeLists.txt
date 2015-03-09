set(OTBAppFusion_LINK_LIBS
  ${OTBImageBase_LIBRARIES}
  ${OTBInterpolation_LIBRARIES}
  ${OTBPanSharpening_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBProjection_LIBRARIES}
)

otb_create_application(
  NAME           BundleToPerfectSensor
  SOURCES        otbBundleToPerfectSensor.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           Pansharpening
  SOURCES        otbPansharpening.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
