IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_DIR}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_DIR}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_DIR}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_DIR}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

ADD_TEST(inTuInstallationTest ${CXX_TEST_PATH}/otbInstallationTest      
  ${INPUTDATA}/poupees.hd )

ADD_EXECUTABLE(otbInstallationTest otbInstallationTest.cxx )
TARGET_LINK_LIBRARIES(otbInstallationTest OTBCommon cai )

ENDIF( NOT OTB_DISABLE_CXX_TESTING )

