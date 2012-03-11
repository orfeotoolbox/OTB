
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


SET(GEOSPATIALANALYSIS_TESTS ${CXX_TEST_PATH}/otbGeospatialAnalysisTests)

# -------------  Transactors ----------------------------

ADD_TEST(gfTuPostGISCreateTableTransactorNew ${GEOSPATIALANALYSIS_TESTS}
     otbPostGISCreateTableTransactorNew
        )

ADD_TEST(gfTuPostGISCreateTableTransactorAccessors ${GEOSPATIALANALYSIS_TESTS}
     otbPostGISCreateTableTransactorAccessors
        )

ADD_TEST(gfTuPostGISCreateTableTransactorCreate ${GEOSPATIALANALYSIS_TESTS}
  otbPostGISCreateTableTransactorCreate
  localhost
  orfeotoolbox_test
  orfeotoolbox_test_user
  Bidfeud0
        )

ADD_TEST(gfTuPostGISFromStringTransactorNew ${GEOSPATIALANALYSIS_TESTS}
     otbPostGISFromStringTransactorNew
        )


# -------       CXX source files -----------------------------------
SET(GEOSPATIALANALYSIS_SRCS
otbGeospatialAnalysisTests.cxx
otbPostGISCreateTableTransactorNew.cxx
otbPostGISCreateTableTransactorAccessors.cxx
otbPostGISCreateTableTransactorCreate.cxx
otbPostGISFromStringTransactorNew.cxx
)

OTB_ADD_EXECUTABLE(otbGeospatialAnalysisTests "${GEOSPATIALANALYSIS_SRCS}" "OTBGeospatialAnalysis;OTBTesting;OTBCommon;pq;pqxx")

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
