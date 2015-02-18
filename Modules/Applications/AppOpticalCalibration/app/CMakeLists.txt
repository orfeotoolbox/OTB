set(OTBAppOpticalCalibration_LINK_LIBS
  ${OTBOpticalCalibration_LIBRARIES}
  ${OTBProjection_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
)

otb_create_application(
  NAME           OpticalCalibration
  SOURCES        otbOpticalCalibration.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
