project(GeospatialAnalysisExamples)
include_regular_expression("^.*$")

add_executable(PostGISCreateTable PostGISCreateTable.cxx )
target_link_libraries(PostGISCreateTable OTBCommon OTBIO OTBGeospatialAnalysis)



if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/GeospatialAnalysis)

set(INPUTDATA ${OTB_DATA_ROOT}/Examples)
#Remote sensing images (large images )
if(OTB_DATA_USE_LARGEINPUT)
  set(INPUTLARGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
endif(OTB_DATA_USE_LARGEINPUT)

set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS ${CXX_TEST_PATH}/otbGeospatialAnalysisExamplesTests)

set(TOL 0.0)
set(EPS 0.001)

add_test( trTeGeospatialAnalysisPostGISCreateTableTest ${EXE_TESTS}

        GeospatialAnalysisPostGISCreateTableTest
          localhost
          orfeotoolbox_test
          orfeotoolbox_test_user
          Bidfeud0
        )



include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbGeospatialAnalysisExamplesTests otbGeospatialAnalysisExamplesTests.cxx)
target_link_libraries(otbGeospatialAnalysisExamplesTests  OTBCommon OTBIO OTBGeospatialAnalysis OTBTesting)

endif( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
