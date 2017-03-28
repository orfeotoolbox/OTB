otb_module_test()
#----------- SarRadiometricCalibration TESTS ----------------
otb_test_application(NAME apTvRaSarRadiometricCalibration_SENTINEL1
  APP  SARCalibration
  OPTIONS -in ${INPUTDATA}/SENTINEL1_SLC_S6_1S_extract_1200_1100_300_300.tiff?&geom=${INPUTDATA}/SENTINEL1_SLC_S6_1S_extract_1200_1100_300_300.geom
  -out ${TEMP}/apTvRaSarRadiometricCalibration_SENTINEL1.tif
  VALID   --compare-image ${NOTOL}
  ${BASELINE}/raTvSarRadiometricCalibration_SENTINEL1.tif
  ${TEMP}/apTvRaSarRadiometricCalibration_SENTINEL1.tif )

if(OTB_DATA_USE_LARGEINPUT)
  otb_test_application(NAME apTvRaSarRadiometricCalibration_SENTINEL1_PRODUCT_INPUT
    APP  SARCalibration
    OPTIONS -in  LARGEINPUT{SENTINEL1/S1A_S6_SLC__1SSV_20150619T195043/measurement/s1a-s6-slc-vv-20150619t195043-20150619t195101-006447-00887d-001.tiff}
    -out "${TEMP}/apTvRaSarRadiometricCalibration_SENTINEL1_PRODUCT_INPUT.tif?box=1200:1100:256:256"
    VALID   --compare-image ${NOTOL}
    ${BASELINE}/raTvSarRadiometricCalibration_SENTINEL1_PRODUCT_INPUT.tif
    ${TEMP}/apTvRaSarRadiometricCalibration_SENTINEL1_PRODUCT_INPUT.tif )
endif()

otb_test_application(NAME apTvRaSarRadiometricCalibration_RADARSAT2
  APP  SARCalibration
  OPTIONS -in ${INPUTDATA}/RADARSAT2_ALTONA_300_300_VV.tif?&geom=${INPUTDATA}/RADARSAT2_ALTONA_300_300_VV.geom
  -out ${TEMP}/apTvRaSarRadiometricCalibration_RADARSAT2.tif
  VALID   --compare-image ${EPSILON_5}
  ${BASELINE}/raTvSarRadiometricCalibration_RADARSAT2.tif
  ${TEMP}/apTvRaSarRadiometricCalibration_RADARSAT2.tif )

otb_test_application(NAME apTvRaSarDeburst_SENTINEL1
  APP  SARDeburst
  OPTIONS -in ${INPUTDATA}/s1a-iw1-slc-vh-amp_xt.tif
  -out ${TEMP}/apTvRaSarDeburst_SENTINEL1_output.tif uint16
  VALID --compare-image ${NOTOL}
  # Same baseline as filter test
  ${BASELINE}/saTvSarDeburstImageFilterTestOutput.tif
  ${TEMP}/apTvRaSarDeburst_SENTINEL1_output.tif)
