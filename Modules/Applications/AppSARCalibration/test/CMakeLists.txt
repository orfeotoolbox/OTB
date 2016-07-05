otb_module_test()
#----------- SarRadiometricCalibration TESTS ----------------
otb_test_application(NAME apTvRaSarRadiometricCalibration_SENTINEL1
  APP  SARCalibration
  OPTIONS -in ${INPUTDATA}/SENTINEL1_SLC_S6_1S_extract_300_300.tif?&geom=${INPUTDATA}/SENTINEL1_SLC_S6_1S_extract_300_300.geom
  -out ${TEMP}/apTvRaSarRadiometricCalibration_SENTINEL1.tif
  VALID   --compare-image ${NOTOL}
  ${BASELINE}/raTvSarRadiometricCalibration_SENTINEL1.tif
  ${TEMP}/apTvRaSarRadiometricCalibration_SENTINEL1.tif )

otb_test_application(NAME apTvRaSarRadiometricCalibration_RADARSAT2
  APP  SARCalibration
  OPTIONS -in ${INPUTDATA}/RADARSAT2_ALTONA_300_300_VV.tif?&geom=${INPUTDATA}/RADARSAT2_ALTONA_300_300_VV.geom
  -out ${TEMP}/apTvRaSarRadiometricCalibration_RADARSAT2.tif
  VALID   --compare-image ${EPSILON_5}
  ${BASELINE}/raTvSarRadiometricCalibration_RADARSAT2.tif
  ${TEMP}/apTvRaSarRadiometricCalibration_RADARSAT2.tif )
