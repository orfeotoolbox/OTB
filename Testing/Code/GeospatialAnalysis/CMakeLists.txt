
if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


set(GEOSPATIALANALYSIS_TESTS ${CXX_TEST_PATH}/otbGeospatialAnalysisTests)

# -------------  Transactors ----------------------------

add_test(gfTuPostGISCreateTableTransactorNew ${GEOSPATIALANALYSIS_TESTS}
     otbPostGISCreateTableTransactorNew
        )

add_test(gfTuPostGISCreateTableTransactorAccessors ${GEOSPATIALANALYSIS_TESTS}
     otbPostGISCreateTableTransactorAccessors
        )

add_test(gfTuPostGISCreateTableTransactorCreate ${GEOSPATIALANALYSIS_TESTS}
  otbPostGISCreateTableTransactorCreate
  localhost
  orfeotoolbox_test
  orfeotoolbox_test_user
  Bidfeud0
        )

add_test(gfTuPostGISFromStringTransactorNew ${GEOSPATIALANALYSIS_TESTS}
     otbPostGISFromStringTransactorNew
        )


# -------       CXX source files -----------------------------------
set(GEOSPATIALANALYSIS_SRCS
otbGeospatialAnalysisTests.cxx
otbPostGISCreateTableTransactorNew.cxx
otbPostGISCreateTableTransactorAccessors.cxx
otbPostGISCreateTableTransactorCreate.cxx
otbPostGISFromStringTransactorNew.cxx
)

OTB_ADD_EXECUTABLE(otbGeospatialAnalysisTests "${GEOSPATIALANALYSIS_SRCS}" "OTBGeospatialAnalysis;OTBTesting;OTBCommon;pq;pqxx")

endif( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
