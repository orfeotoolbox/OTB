
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


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
        1 2 3
         )

ADD_TEST(mtTvTimeSeriesLeastSquaresFittingFunctor2 ${MULTITEMPORAL_TESTS1}
        otbTimeSeriesLeastSquareFittingFunctorTest
        10 0.3 3.123
         )

ADD_TEST(mtTvTimeSeriesLeastSquaresFittingFunctorWeights ${MULTITEMPORAL_TESTS1}
        otbTimeSeriesLeastSquareFittingFunctorWeightsTest
        1 2 3
         )


ADD_TEST(mtTuSavitzkyGolayInterpolationFunctor ${MULTITEMPORAL_TESTS1}
        otbSavitzkyGolayInterpolationFunctorNew
         )

ADD_TEST(mtTvSavitzkyGolayInterpolationFunctorTest ${MULTITEMPORAL_TESTS1}
        otbSavitzkyGolayInterpolationFunctorTest
         )

ADD_TEST(mtTvEnvelopeSavitzkyGolayInterpolationFunctorTest ${MULTITEMPORAL_TESTS1}
        otbEnvelopeSavitzkyGolayInterpolationFunctorTest
         )

# -------  CXX Source Files -----------------------------------
SET(BasicMultiTemporal_SRCS1
otbPolynomialTimeSeriesTest.cxx
otbTimeSeriesLeastSquareFittingFunctorNew.cxx
otbTimeSeriesLeastSquareFittingFunctorTest.cxx
otbTimeSeriesLeastSquareFittingFunctorWeightsTest.cxx
otbSavitzkyGolayInterpolationFunctorNew.cxx
otbSavitzkyGolayInterpolationFunctorTest.cxx
otbEnvelopeSavitzkyGolayInterpolationFunctorTest.cxx
)

ADD_EXECUTABLE(otbMultiTemporalTests1 otbMultiTemporalTests1.cxx ${BasicMultiTemporal_SRCS1})
TARGET_LINK_LIBRARIES(otbMultiTemporalTests1 OTBIO OTBMultiTemporal  OTBTesting)


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
