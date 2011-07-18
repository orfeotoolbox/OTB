IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE       ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA      ${OTB_DATA_ROOT}/Input)
SET(TEMP           ${OTBTesting_BINARY_DIR}/Temporary)

IF(OTB_DATA_USE_LARGEINPUT)
  SET(LARGEINPUT ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

SET(NOTOL 0.0)
SET(EPSILON_9 0.000000001)
SET(EPSILON_3 0.001)

SET(Hyperspectral_TESTS1 ${CXX_TEST_PATH}/otbHyperspectralTests1)
SET(Hyperspectral_TESTS2 ${CXX_TEST_PATH}/otbHyperspectralTests2)

# HyperspectralTests1
ADD_TEST(hyTuHyperspectralVariableNew ${Hyperspectral_TESTS1}
        otbHyperspectralVariableNew)


SET(Hyperspectral_SRCS1
otbHyperspectralTests1.cxx
otbEigenvalueLikelihoodMaximization.cxx
otbCLSPSTOUnmixingImageFilter.cxx
otbFCLSUnmixingImageFilter.cxx
otbFullyConstrainedLeastSquareImageFilter.cxx
otbISRAUnmixingImageFilter.cxx
otbNCLSUnmixingImageFilter.cxx
#otbSparseUnmixingImageFilter.cxx
otbUnConstrainedLeastSquareImageFilter.cxx
otbVCAImageFilter.cxx
)

SET(Hyperspectral_SRCS2
otbHyperspectralTests2.cxx
otbLocalRxDetectorTest.cxx
otbLocalRxDetectorRoiTest.cxx
)

ADD_EXECUTABLE(otbHyperspectralTests1 otbHyperspectralTests1.cxx ${Hyperspectral_SRCS1})
TARGET_LINK_LIBRARIES(otbHyperspectralTests1 OTBIO OTBHyperspectral  OTBTesting)

ADD_EXECUTABLE(otbHyperspectralTests2 otbHyperspectralTests2.cxx ${Hyperspectral_SRCS2})
TARGET_LINK_LIBRARIES(otbHyperspectralTests2 OTBIO OTBHyperspectral OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
