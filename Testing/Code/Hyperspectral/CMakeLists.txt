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
ADD_TEST(hyTuEigenvalueLikelihoodMaximizationNew
         ${Hyperspectral_TESTS1}
         otbEigenvalueLikelihoodMaximizationNewTest)         

ADD_TEST(hyTvEigenvalueLikelihoodMaximizationTest
         ${Hyperspectral_TESTS1}
           --compare-ascii ${NOTOL}
           ${BASELINE_FILES}/hyTvEigenvalueLikelihoodMaximizationTest.txt
           ${TEMP}/hyTvEigenvalueLikelihoodMaximizationTest.txt
         otbEigenvalueLikelihoodMaximizationTest
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${TEMP}/hyTvEigenvalueLikelihoodMaximizationTest.txt)

ADD_TEST(hyTuVCAImageFilterNew
         ${Hyperspectral_TESTS1}
         otbVCAImageFilterNew
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${TEMP}/synthetic_vca_5.hdr
         5 )

ADD_TEST(hyTvVCAImageFilterTest1
         ${Hyperspectral_TESTS1}
           --compare-image ${NOTOL}
           ${BASELINE_FILES}/hyTvVCAImageFilterTest.tif
           ${TEMP}/hyTvVCAImageFilterTest.tif
         otbVCAImageFilterTestHighSNR
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${TEMP}/hyTvVCAImageFilterTest.tif
         5 )

ADD_TEST(hyTuSparseUnmixingImageFilterNew
		${Hyperspectral_TESTS1}
		otbSparseUnmixingImageFilterNewTest)

# TODO
#ADD_TEST(hyTvSparseUnmixingImageFilterTest
#		${Hyperspectral_TESTS1}
#		otbSparseUnmixingImageFilterTest
#       )


ADD_TEST(hyTuUnConstrainedLeastSquareImageFilterNew
         ${Hyperspectral_TESTS1}
         otbUnConstrainedLeastSquareImageFilterNewTest)

ADD_TEST(hyTvUnConstrainedLeastSquareImageFilterTest
         ${Hyperspectral_TESTS1}
           --compare-image ${NOTOL}
           ${BASELINE_FILES}/hyTvUnConstrainedLeastSquareImageFilterTest.tif
           ${TEMP}/hyTvUnConstrainedLeastSquareImageFilterTest.tif
         otbUnConstrainedLeastSquareImageFilterTest
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
         ${TEMP}/hyTvUnConstrainedLeastSquareImageFilterTest.tif)



ADD_TEST(hyTuNCLSImageFilterNew
         ${Hyperspectral_TESTS1}
         otbNCLSImageFilterNewTest)

ADD_TEST(hyTvNCLSSquareImageFilterTest
         ${Hyperspectral_TESTS1}
           --compare-image ${NOTOL}
           ${BASELINE_FILES}/hyTvNCLSImageFilterTest.tif
           ${TEMP}/hyTvNCLSImageFilterTest.tif
         otbNCLSImageFilterTest
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
         ${TEMP}/hyTvNCLSImageFilterTest.tif)


ADD_TEST(hyTuISRAUnmixingImageFilterNew
         ${Hyperspectral_TESTS1}
         otbISRAUnmixingImageFilterNewTest)

ADD_TEST(hyTvISRAUnmixingImageFilterTest
         ${Hyperspectral_TESTS1}
           --compare-image ${NOTOL}
           ${BASELINE_FILES}/hyTvISRAUnmixingImageFilterTest.tif
           ${TEMP}/hyTvISRAUnmixingImageFilterTest.tif
         otbISRAUnmixingImageFilterTest
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
         ${TEMP}/hyTvISRAUnmixingImageFilterTest.tif)


ADD_TEST(hyTuFCLSUnmixingImageFilterNew
         ${Hyperspectral_TESTS1}
         otbFCLSUnmixingImageFilterNewTest)

ADD_TEST(hyTvFCLSUnmixingImageFilterTest
         ${Hyperspectral_TESTS1}
           --compare-image ${NOTOL}
           ${BASELINE_FILES}/hyTvFCLSUnmixingImageFilterTest.tif
           ${TEMP}/hyTvFCLSUnmixingImageFilterTest.tif
         otbFCLSUnmixingImageFilterTest
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
         ${TEMP}/hyTvFCLSUnmixingImageFilterTest.tif)


SET(Hyperspectral_SRCS1
otbHyperspectralTests1.cxx
otbEigenvalueLikelihoodMaximization.cxx
otbCLSPSTOUnmixingImageFilter.cxx
otbFCLSUnmixingImageFilter.cxx
otbFullyConstrainedLeastSquareImageFilter.cxx
otbISRAUnmixingImageFilter.cxx
otbNCLSUnmixingImageFilter.cxx
otbSparseUnmixingImageFilter.cxx
otbUnConstrainedLeastSquareImageFilter.cxx
otbVCAImageFilter.cxx
)

SET(Hyperspectral_SRCS2
otbHyperspectralTests2.cxx
otbLocalRxDetectorTest.cxx
otbLocalRxDetectorRoiTest.cxx
)

ADD_EXECUTABLE(otbHyperspectralTests1 otbHyperspectralTests1.cxx ${Hyperspectral_SRCS1})
TARGET_LINK_LIBRARIES(otbHyperspectralTests1 OTBIO OTBHyperspectral OTBMultiScale OTBTesting)

ADD_EXECUTABLE(otbHyperspectralTests2 otbHyperspectralTests2.cxx ${Hyperspectral_SRCS2})
TARGET_LINK_LIBRARIES(otbHyperspectralTests2 OTBIO OTBHyperspectral OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
