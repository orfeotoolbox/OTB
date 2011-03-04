
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(NOTOL 0.0)
SET(EPSILON_10 0.0000000001)

SET(MULTITEMPORAL_TESTS1 ${CXX_TEST_PATH}/otbMultiTemporalTests1)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MULTITEMPORAL_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::testname   ------------------------------

ADD_TEST(mtTvPolynomialTimeSeriesTest1 ${MULTITEMPORAL_TESTS1}
        otbPolynomialTimeSeriesTest
	 )

ADD_TEST(mtTuTimeSeriesLeastSquaresFittingFunctor ${MULTITEMPORAL_TESTS1}
        otbTimeSeriesLeastSquareFittingFunctorNew
	 )

ADD_TEST(mtTvTimeSeriesLeastSquaresFittingFunctor ${MULTITEMPORAL_TESTS1}
        otbTimeSeriesLeastSquareFittingFunctorTest
	 )


# -------  CXX Source Files -----------------------------------
SET(BasicMultiTemporal_SRCS1 
otbPolynomialTimeSeriesTest.cxx
otbTimeSeriesLeastSquareFittingFunctorNew.cxx
otbTimeSeriesLeastSquareFittingFunctorTest.cxx
)

ADD_EXECUTABLE(otbMultiTemporalTests1 otbMultiTemporalTests1.cxx ${BasicMultiTemporal_SRCS1})
TARGET_LINK_LIBRARIES(otbMultiTemporalTests1 OTBIO OTBMultiTemporal  OTBTesting)


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
