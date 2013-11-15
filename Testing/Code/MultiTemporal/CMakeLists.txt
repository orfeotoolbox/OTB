
if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


set(MULTITEMPORAL_TESTS1 ${CXX_TEST_PATH}/otbMultiTemporalTests1)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MULTITEMPORAL_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::testname   ------------------------------

add_test(mtTvPolynomialTimeSeriesTest1 ${MULTITEMPORAL_TESTS1}
        otbPolynomialTimeSeriesTest
         )

add_test(mtTuTimeSeriesLeastSquaresFittingFunctor ${MULTITEMPORAL_TESTS1}
        otbTimeSeriesLeastSquareFittingFunctorNew
         )

add_test(mtTvTimeSeriesLeastSquaresFittingFunctor ${MULTITEMPORAL_TESTS1}
        otbTimeSeriesLeastSquareFittingFunctorTest
        1 2 3
         )

add_test(mtTvTimeSeriesLeastSquaresFittingFunctor2 ${MULTITEMPORAL_TESTS1}
        otbTimeSeriesLeastSquareFittingFunctorTest
        10 0.3 3.123
         )

add_test(mtTvTimeSeriesLeastSquaresFittingFunctorWeights ${MULTITEMPORAL_TESTS1}
        otbTimeSeriesLeastSquareFittingFunctorWeightsTest
        1 2 3
         )


add_test(mtTuSavitzkyGolayInterpolationFunctor ${MULTITEMPORAL_TESTS1}
        otbSavitzkyGolayInterpolationFunctorNew
         )

add_test(mtTvSavitzkyGolayInterpolationFunctorTest ${MULTITEMPORAL_TESTS1}
        otbSavitzkyGolayInterpolationFunctorTest
         )

add_test(mtTvEnvelopeSavitzkyGolayInterpolationFunctorTest ${MULTITEMPORAL_TESTS1}
        otbEnvelopeSavitzkyGolayInterpolationFunctorTest
         )

# -------  CXX Source Files -----------------------------------
set(BasicMultiTemporal_SRCS1
otbPolynomialTimeSeriesTest.cxx
otbTimeSeriesLeastSquareFittingFunctorNew.cxx
otbTimeSeriesLeastSquareFittingFunctorTest.cxx
otbTimeSeriesLeastSquareFittingFunctorWeightsTest.cxx
otbSavitzkyGolayInterpolationFunctorNew.cxx
otbSavitzkyGolayInterpolationFunctorTest.cxx
otbEnvelopeSavitzkyGolayInterpolationFunctorTest.cxx
)

add_executable(otbMultiTemporalTests1 otbMultiTemporalTests1.cxx ${BasicMultiTemporal_SRCS1})
target_link_libraries(otbMultiTemporalTests1 OTBIO OTBMultiTemporal  OTBTesting)


endif( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
