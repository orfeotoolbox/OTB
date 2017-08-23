set(OTBAppSARUtils_LINK_LIBS
  ${OTBSARUtils_LIBRARIES}
  {OTBImageNoise_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBITK_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
)

otb_create_application(
  NAME           ComputeModulusAndPhase
  SOURCES        otbComputeModulusAndPhase.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           Despeckle
  SOURCES        otbDespeckle.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
