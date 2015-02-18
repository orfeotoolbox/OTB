set(OTBAppChangeDetection_LINK_LIBS
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBChangeDetection_LIBRARIES}
)

otb_create_application(
  NAME           MultivariateAlterationDetector
  SOURCES        otbMultivariateAlterationDetector.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
