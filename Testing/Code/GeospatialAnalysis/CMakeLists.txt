
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(EXAMPLESDATA ${OTB_DATA_ROOT}/Examples)
IF(OTB_DATA_USE_LARGEINPUT)
  SET(LARGEINPUTDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)

#Tolerance sur la difference des valeurs numeriques dans le --compare-ascii
SET(EPS 0.001)

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
