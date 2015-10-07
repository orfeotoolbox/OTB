otb_module_test()
#----------- SarRadiometricCalibration TESTS ----------------
otb_test_application(NAME apTvRaSarRadiometricCalibration_SENTINEL1
  APP  SarRadiometricCalibration
  OPTIONS -in ${INPUTDATA}/SENTINEL1_SLC_S6_1S_extract_300_300.tif?&geom=${INPUTDATA}/SENTINEL1_SLC_S6_1S_extract_300_300.geom
  -out ${TEMP}/apTvRaSarRadiometricCalibration_SENTINEL1.tif
  )
