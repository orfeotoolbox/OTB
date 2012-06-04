IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

ADD_SUBDIRECTORY(OGRAdapters)

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
         ${LARGEINPUT}/TERRASARX/2008-03-10_GrandCanyon_SSC/TSX1_SAR__SSC______SM_S_SRA_20080310T133220_20080310T133228/TSX1_SAR__SSC______SM_S_SRA_20080310T133220_20080310T133228.xml
         ${TEMP}/uaTvPlatformPositionComputeBaselineTest.txt)
ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(uaTvGeometricSarSensorModelAdapterNew
         ${UtilitiesAdapters_TESTS1}
         otbGeometricSarSensorModelAdapterNewTest)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(uaTvGeometricSarSensorModelAdapterTest
         ${UtilitiesAdapters_TESTS1}
           --compare-ascii ${EPSILON_9}
           ${BASELINE_FILES}/uaTvGeometricSarSensorModelAdapterTest.txt
           ${TEMP}/uaTvGeometricSarSensorModelAdapterTest.txt
         otbGeometricSarSensorModelAdapterTest
         ${LARGEINPUT}/TERRASARX/2008-03-10_GrandCanyon_SSC/TSX1_SAR__SSC______SM_S_SRA_20080310T133220_20080310T133228/TSX1_SAR__SSC______SM_S_SRA_20080310T133220_20080310T133228.xml
         ${TEMP}/uaTvGeometricSarSensorModelAdapterTest.txt)
ENDIF(OTB_DATA_USE_LARGEINPUT)

SET(UtilitiesAdapters_SRCS1
otbUtilitiesAdaptersTests1.cxx
otbPlatformPositionAdapter.cxx
otbGeometricSarSensorModelAdapter.cxx
)


ADD_EXECUTABLE(otbUtilitiesAdaptersTests1 otbUtilitiesAdaptersTests1.cxx ${UtilitiesAdapters_SRCS1})
TARGET_LINK_LIBRARIES(otbUtilitiesAdaptersTests1 OTBOssimAdapters OTBTesting)


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
