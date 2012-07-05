PROJECT(GeospatialAnalysisExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(PostGISCreateTable PostGISCreateTable.cxx )
TARGET_LINK_LIBRARIES(PostGISCreateTable OTBCommon OTBIO OTBGeospatialAnalysis)



IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/GeospatialAnalysis)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
#Remote sensing images (large images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(INPUTLARGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbGeospatialAnalysisExamplesTests)

SET(TOL 0.0)
SET(EPS 0.001)

ADD_TEST( trTeGeospatialAnalysisPostGISCreateTableTest ${EXE_TESTS}
      
        GeospatialAnalysisPostGISCreateTableTest
          localhost
          orfeotoolbox_test
          orfeotoolbox_test_user
          Bidfeud0
        )


  
INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbGeospatialAnalysisExamplesTests otbGeospatialAnalysisExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbGeospatialAnalysisExamplesTests  OTBCommon OTBIO OTBGeospatialAnalysis OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
