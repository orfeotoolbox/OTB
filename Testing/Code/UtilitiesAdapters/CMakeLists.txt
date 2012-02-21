IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE       ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA      ${OTB_DATA_ROOT}/Input)
SET(TEMP           ${OTBTesting_BINARY_DIR}/Temporary)

IF(OTB_DATA_USE_LARGEINPUT)
  SET(LARGEINPUT ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

SET(NOTOL 0.0)
SET(EPSILON_9 0.000000001)
SET(EPSILON_3 0.001)

SET(UtilitiesAdapters_TESTS1 ${CXX_TEST_PATH}/otbUtilitiesAdaptersTests1)

ADD_TEST(uaTvPlatformPositionComputeBaselineNew
         ${UtilitiesAdapters_TESTS1}
         otbPlatformPositionComputeBaselineNewTest)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(uaTvPlatformPositionComputeBaselineTest
         ${UtilitiesAdapters_TESTS1}
           --compare-ascii ${EPSILON_9}
           ${BASELINE_FILES}/uaTvPlatformPositionComputeBaselineTest.txt
           ${TEMP}/uaTvPlatformPositionComputeBaselineTest.txt
         otbPlatformPositionComputeBaselineTest
         ${LARGEINPUT}/TERRASARX/ULURU/TSX1_SAR__SSC______HS_S_SRA_20090212T204239_20090212T204240/TSX1_SAR__SSC______HS_S_SRA_20090212T204239_20090212T204240.xml
         ${LARGEINPUT}/TERRASARX/ULURU/TSX1_SAR__SSC______HS_S_SRA_20090223T204240_20090223T204241/TSX1_SAR__SSC______HS_S_SRA_20090223T204240_20090223T204241.xml
         ${TEMP}/uaTvPlatformPositionComputeBaselineTest.txt)
ENDIF(OTB_DATA_USE_LARGEINPUT)

SET(UtilitiesAdapters_SRCS1
otbUtilitiesAdaptersTests1.cxx
otbPlatformPositionAdapter.cxx
)


ADD_EXECUTABLE(otbUtilitiesAdaptersTests1 otbUtilitiesAdaptersTests1.cxx ${UtilitiesAdapters_SRCS1})
TARGET_LINK_LIBRARIES(otbUtilitiesAdaptersTests1 OTBOssimAdapters OTBTesting)


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
