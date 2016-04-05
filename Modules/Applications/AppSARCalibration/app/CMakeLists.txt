set(OTBAppSARCalibration_LINK_LIBS
  ${OTBSARCalibration_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
)

otb_create_application(
  NAME           SarRadiometricCalibration
  SOURCES        otbSarRadiometricCalibration.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           SARRadiometricCalibration
  SOURCES        otbSARRadiometricCalibration.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
