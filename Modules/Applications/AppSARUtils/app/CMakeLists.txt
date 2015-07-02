set(OTBAppSARUtils_LINK_LIBS
  ${OTBSARUtils_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
)

otb_create_application(
  NAME           ComputeModulusAndPhase
  SOURCES        otbComputeModulusAndPhase.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
