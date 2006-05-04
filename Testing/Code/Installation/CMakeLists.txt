IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Images)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

ADD_TEST(inTuInstallationTest ${CXX_TEST_PATH}/otbInstallationTest      
  ${INPUTDATA}/poupees.hd )

ADD_EXECUTABLE(otbInstallationTest otbInstallationTest.cxx )
TARGET_LINK_LIBRARIES(otbInstallationTest OTBCommon cai )

ENDIF( NOT OTB_DISABLE_CXX_TESTING )

