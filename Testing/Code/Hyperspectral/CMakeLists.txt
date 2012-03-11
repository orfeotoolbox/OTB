IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


SET(Hyperspectral_TESTS1 ${CXX_TEST_PATH}/otbHyperspectralTests1)
SET(Hyperspectral_TESTS2 ${CXX_TEST_PATH}/otbHyperspectralTests2)
SET(Hyperspectral_TESTS3 ${CXX_TEST_PATH}/otbHyperspectralTests3)

# HyperspectralTests1
ADD_TEST(hyTuEigenvalueLikelihoodMaximizationNew
         ${Hyperspectral_TESTS1}
         otbEigenvalueLikelihoodMaximizationNewTest)

ADD_TEST(hyTvEigenvalueLikelihoodMaximizationTest
         ${Hyperspectral_TESTS1}
           --compare-ascii ${EPSILON_9}
           ${BASELINE_FILES}/hyTvEigenvalueLikelihoodMaximizationTest.txt
           ${TEMP}/hyTvEigenvalueLikelihoodMaximizationTest.txt
         otbEigenvalueLikelihoodMaximizationTest
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${TEMP}/hyTvEigenvalueLikelihoodMaximizationTest.txt)

# HyperspectralTests1
ADD_TEST(hyTuVirtualDimensionalityNew
         ${Hyperspectral_TESTS1}
         otbVirtualDimensionalityNewTest)

ADD_TEST(hyTvVirtualDimensionalityTest
         ${Hyperspectral_TESTS1}
           --compare-ascii ${EPSILON_9}
           ${BASELINE_FILES}/hyTvVirtualDimensionalityTest.txt
           ${TEMP}/hyTvVirtualDimensionalityTest.txt
         otbVirtualDimensionalityTest
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${TEMP}/hyTvVirtualDimensionalityTest.txt
         1E-3)

ADD_TEST(hyTuVCAImageFilterNew
         ${Hyperspectral_TESTS1}
         otbVCAImageFilterNew
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${TEMP}/synthetic_vca_5.hdr
         5 )

ADD_TEST(hyTvVCAImageFilterTest1
         ${Hyperspectral_TESTS1}
           --compare-image ${EPSILON_9}
           ${BASELINE}/TvHyVertexComponentAnalysisImage.tif
           ${TEMP}/hyTvVCAImageFilterTest.tif
         otbVCAImageFilterTestHighSNR
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${TEMP}/hyTvVCAImageFilterTest.tif
         5 )

ADD_TEST(hyTuSparseUnmixingImageFilterNew
		${Hyperspectral_TESTS1}
		otbSparseUnmixingImageFilterNew)

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
           --compare-image ${EPSILON_9}
           ${BASELINE_FILES}/hyTvUnConstrainedLeastSquareImageFilterTest.tif
           ${TEMP}/hyTvUnConstrainedLeastSquareImageFilterTest.tif
         otbUnConstrainedLeastSquareImageFilterTest
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
         ${TEMP}/hyTvUnConstrainedLeastSquareImageFilterTest.tif)



ADD_TEST(hyTuNCLSUnmixingImageFilterNew
         ${Hyperspectral_TESTS1}
         otbNCLSUnmixingImageFilterNewTest)

ADD_TEST(hyTvNCLSUnmixingImageFilterTest
         ${Hyperspectral_TESTS1}
           --compare-image ${EPSILON_9}
           ${BASELINE_FILES}/hyTvNCLSImageFilterTest.tif
           ${TEMP}/hyTvNCLSImageFilterTest.tif
         otbNCLSUnmixingImageFilterTest
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
         ${TEMP}/hyTvNCLSImageFilterTest.tif
         10)


ADD_TEST(hyTuISRAUnmixingImageFilterNew
         ${Hyperspectral_TESTS1}
         otbISRAUnmixingImageFilterNewTest)

ADD_TEST(hyTvISRAUnmixingImageFilterTest
         ${Hyperspectral_TESTS1}
           --compare-image ${EPSILON_9}
           ${BASELINE_FILES}/hyTvISRAUnmixingImageFilterTest.tif
           ${TEMP}/hyTvISRAUnmixingImageFilterTest.tif
         otbISRAUnmixingImageFilterTest
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
         ${TEMP}/hyTvISRAUnmixingImageFilterTest.tif
         10)

# HyperspectralTests3
ADD_TEST(hyTuMDMDNMFImageFilterNew
         ${Hyperspectral_TESTS3}
         otbMDMDNMFImageFilterNewTest)

ADD_TEST(hyTvMDMDNMFImageFilterTest
         ${Hyperspectral_TESTS3}
         --compare-image ${EPSILON_9}
         ${BASELINE}/hyTvMDMDNMFImageFilterTest.tif
         ${TEMP}/hyTvMDMDNMFImageFilterTest.tif
         otbMDMDNMFImageFilterTest
         ${INPUTDATA}/cupriteSubHsi.tif
         ${TEMP}/hyTvMDMDNMFImageFilterTest.tif
         100)

ADD_TEST(hyTvMDMDNMFImageFilterTest2
         ${Hyperspectral_TESTS3}
         otbMDMDNMFImageFilterTest2
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
         ${TEMP}/hyTvMDMDNMFImageFilterTest2.tif
         100)


SET(Hyperspectral_SRCS1
otbHyperspectralTests1.cxx
otbEigenvalueLikelihoodMaximization.cxx
otbUnConstrainedLeastSquareImageFilter.cxx
otbISRAUnmixingImageFilter.cxx
otbNCLSUnmixingImageFilter.cxx
otbSparseUnmixingImageFilterNew.cxx
otbSparseUnmixingImageFilter.cxx
otbVCAImageFilter.cxx
otbVirtualDimensionality.cxx
)

SET(Hyperspectral_SRCS2
otbHyperspectralTests2.cxx
otbLocalRxDetectorTest.cxx
otbLocalRxDetectorRoiTest.cxx
)

SET(Hyperspectral_SRCS3
otbHyperspectralTests3.cxx
otbMDMDNMFImageFilter.cxx
)

ADD_EXECUTABLE(otbHyperspectralTests1 otbHyperspectralTests1.cxx ${Hyperspectral_SRCS1})
TARGET_LINK_LIBRARIES(otbHyperspectralTests1 OTBIO OTBHyperspectral OTBMultiScale OTBTesting)

ADD_EXECUTABLE(otbHyperspectralTests2 otbHyperspectralTests2.cxx ${Hyperspectral_SRCS2})
TARGET_LINK_LIBRARIES(otbHyperspectralTests2 OTBIO OTBHyperspectral OTBTesting)

ADD_EXECUTABLE(otbHyperspectralTests3 otbHyperspectralTests3.cxx ${Hyperspectral_SRCS3})
TARGET_LINK_LIBRARIES(otbHyperspectralTests3 OTBIO OTBHyperspectral OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
