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

ADD_TEST(uaTvRPCSolverAdapterValidationTest
         ${UtilitiesAdapters_TESTS1}
         otbRPCSolverAdapterTest
         ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
         10 0.25 0.35
         ${INPUTDATA}/DEM/srtm_directory/
         ${INPUTDATA}/DEM/egm96.grd
)

ADD_TEST(uaTvRPCSolverAdapterNoDEMValidationTest
         ${UtilitiesAdapters_TESTS1}
         otbRPCSolverAdapterTest
         ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
         10 0.25 0.35
         no
         no
)

ADD_TEST(uaTvRPCSolverAdapterOutGeomTest
         ${UtilitiesAdapters_TESTS1}
  --compare-ascii ${EPSILON_9}
         ${BASELINE_FILES}/uaTvRPCSolverAdapterOutGeomTest.geom
         ${TEMP}/uaTvRPCSolverAdapterOutGeomTest.geom
         otbRPCSolverAdapterTest
         ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
         10 0.25 0.35
         ${INPUTDATA}/DEM/srtm_directory/
         ${INPUTDATA}/DEM/egm96.grd
         ${TEMP}/uaTvRPCSolverAdapterOutGeomTest.geom
)

ADD_TEST(uaTvRPCSolverAdapterNotEnoughPointsForElevationTest
         ${UtilitiesAdapters_TESTS1}
         otbRPCSolverAdapterTest
         ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
         5 0.25 0.35
         no
         no
         )

ADD_TEST(uaTvRPCSolverAdapterNotEnoughPointsTest
         ${UtilitiesAdapters_TESTS1}
         otbRPCSolverAdapterTest
         ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
         4 0.25 0.35
         ${INPUTDATA}/DEM/srtm_directory/
         ${INPUTDATA}/DEM/egm96.grd
         )
SET_TESTS_PROPERTIES(uaTvRPCSolverAdapterNotEnoughPointsTest PROPERTIES WILL_FAIL TRUE)

ENDIF(OTB_DATA_USE_LARGEINPUT)

# Exhaustive DEM handler testing

ADD_TEST(uaTvDEMHandler_AboveEllipsoid_SRTM_Geoid
  ${UtilitiesAdapters_TESTS1}
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.434583
  44.647083
  0
  383.580313671
  0.001
)

ADD_TEST(uaTvDEMHandler_AboveMSL_SRTM_Geoid
  ${UtilitiesAdapters_TESTS1}
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.434583
  44.647083
  1
  339.513
  0.001
)

ADD_TEST(uaTvDEMHandler_AboveEllipsoid_SRTM_NoGeoid
  ${UtilitiesAdapters_TESTS1}
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  no
  40
  8.434583
  44.647083
  0
  339.513
  0.001
)

ADD_TEST(uaTvDEMHandler_AboveMSL_SRTM_NoGeoid
  ${UtilitiesAdapters_TESTS1}
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  no
  40
  8.434583
  44.647083
  1
  339.513
  0.001
)

ADD_TEST(uaTvDEMHandler_AboveEllipsoid_NoSRTM_Geoid
  ${UtilitiesAdapters_TESTS1}
  otbDEMHandlerTest
  no
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.434583
  44.647083
  0
  44.067113
  0.001
)

ADD_TEST(uaTvDEMHandler_AboveMSL_NoSRTM_Geoid
  ${UtilitiesAdapters_TESTS1}
  otbDEMHandlerTest
  no
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.434583
  44.647083
  1
  0
  0.001
)


ADD_TEST(uaTvDEMHandler_AboveEllipsoid_NoSRTM_NoGeoid
  ${UtilitiesAdapters_TESTS1}
  otbDEMHandlerTest
  no
  no
  40
  8.434583
  44.647083
  0
  40
  0.001
)

ADD_TEST(uaTvDEMHandler_AboveMSL_NoSRTM_NoGeoid
  ${UtilitiesAdapters_TESTS1}
  otbDEMHandlerTest
  no
  no
  40
  8.434583
  44.647083
  1
  0
  0.001
)


ADD_TEST(uaTvDEMHandler_AboveEllipsoid_SRTM_Geoid_NoData
  ${UtilitiesAdapters_TESTS1}
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.687917
  44.237917
  0
  45.7464
  0.001
)

ADD_TEST(uaTvDEMHandler_AboveMSL_SRTM_Geoid_NoData
  ${UtilitiesAdapters_TESTS1}
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.687917
  44.237917
  1
  0
  0.001
)

ADD_TEST(uaTvDEMHandler_AboveEllipsoid_SRTM_NoGeoid_NoData
  ${UtilitiesAdapters_TESTS1}
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  no
  40
  8.687917
  44.237917
  0
  40
  0.001
)

ADD_TEST(uaTvDEMHandler_AboveMSL_SRTM_NoGeoid_NoData
  ${UtilitiesAdapters_TESTS1}
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  no
  40
  8.687917
  44.237917
  1
  0
  0.001
)

ADD_TEST(uaTvDEMHandler_AboveEllipsoid_NoSRTM_NoGeoid_NoData
  ${UtilitiesAdapters_TESTS1}
  otbDEMHandlerTest
  no
  no
  40
  8.687917
  44.237917
  0
  40
  0.001
)

ADD_TEST(uaTvDEMHandler_AboveMSL_NoSRTM_NoGeoid_NoData
  ${UtilitiesAdapters_TESTS1}
  otbDEMHandlerTest
  no
  no
  40
  8.687917
  44.237917
  1
  0
  0.001
)


ADD_TEST(uaTvDEMHandler_AboveEllipsoid_SRTM_Geoid_NoSRTMCoverage
  ${UtilitiesAdapters_TESTS1}
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  20
  10. # Point not covered
  10. # Point not covered
  0
  21.569 # Geoid offset
  0.001
)

ADD_TEST(uaTvDEMHandler_AboveMSL_SRTM_Geoid_NoSRTMCoverage
  ${UtilitiesAdapters_TESTS1}
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/DEM/egm96.grd
  20
  10. # Point not covered
  10. # Point not covered
  1
  0
  0.001
)

ADD_TEST(uaTvDEMHandler_AboveEllipsoid_SRTM_NoGeoid_NoSRTMCoverage
  ${UtilitiesAdapters_TESTS1}
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  no
  20
  10. # Point not covered
  10. # Point not covered
  0
  20
  0.001
)

ADD_TEST(uaTvDEMHandler_AboveMSL_SRTM_NoGeoid_NoSRTMCoverage
  ${UtilitiesAdapters_TESTS1}
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  no
  20
  10. # Point not covered
  10. # Point not covered
  1
  0
  0.001
)

ADD_TEST(uaTvDEMHandler_AboveEllipsoid_BadSRTM_Geoid
  ${UtilitiesAdapters_TESTS1}
  otbDEMHandlerTest
  ${INPUTDATA}/poupeesTIF/
  ${INPUTDATA}/DEM/egm96.grd
  40
  8.434583
  44.647083
  0
  383.580313671
  0.001
)

SET_TESTS_PROPERTIES(uaTvDEMHandler_AboveEllipsoid_BadSRTM_Geoid PROPERTIES WILL_FAIL 1)

ADD_TEST(uaTvDEMHandler_AboveEllipsoid_SRTM_BadGeoid
  ${UtilitiesAdapters_TESTS1}
  otbDEMHandlerTest
  ${INPUTDATA}/DEM/srtm_directory/
  ${INPUTDATA}/poupees.jpg
  40
  8.434583
  44.647083
  0
  339.513
  0.001
)


SET(UtilitiesAdapters_SRCS1
otbUtilitiesAdaptersTests1.cxx
otbPlatformPositionAdapter.cxx
otbGeometricSarSensorModelAdapter.cxx
otbRPCSolverAdapterTest.cxx
otbDEMHandlerTest.cxx
)


ADD_EXECUTABLE(otbUtilitiesAdaptersTests1 otbUtilitiesAdaptersTests1.cxx ${UtilitiesAdapters_SRCS1})
TARGET_LINK_LIBRARIES(otbUtilitiesAdaptersTests1 OTBOssimAdapters OTBTesting)


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
