IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

INCLUDE(TestingHelper)

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

ADD_TEST(uaTvGeometricSarSenorModelAdapterNew
         ${UtilitiesAdapters_TESTS1}
         otbGeometricSarSenorModelAdapterNewTest)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(uaTvGeometricSarSenorModelAdapterTest
         ${UtilitiesAdapters_TESTS1}
           --compare-ascii ${EPSILON_9}
           ${BASELINE_FILES}/uaTvGeometricSarSenorModelAdapterTest.txt
           ${TEMP}/uaTvGeometricSarSenorModelAdapterTest.txt
         otbGeometricSarSenorModelAdapterTest
         ${LARGEINPUT}/TERRASARX/ULURU/TSX1_SAR__SSC______HS_S_SRA_20090212T204239_20090212T204240/TSX1_SAR__SSC______HS_S_SRA_20090212T204239_20090212T204240.xml
         ${LARGEINPUT}/TERRASARX/ULURU/TSX1_SAR__SSC______HS_S_SRA_20090223T204240_20090223T204241/TSX1_SAR__SSC______HS_S_SRA_20090223T204240_20090223T204241.xml
         ${TEMP}/uaTvGeometricSarSenorModelAdapter.txt)
ENDIF(OTB_DATA_USE_LARGEINPUT)

SET(UtilitiesAdapters_SRCS1
otbUtilitiesAdaptersTests1.cxx
otbPlatformPositionAdapter.cxx
otbGeometricSarSensorModelAdapter.cxx
)


ADD_EXECUTABLE(otbUtilitiesAdaptersTests1 otbUtilitiesAdaptersTests1.cxx ${UtilitiesAdapters_SRCS1})
TARGET_LINK_LIBRARIES(otbUtilitiesAdaptersTests1 OTBOssimAdapters OTBTesting)


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
