if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


set(Hyperspectral_TESTS1 ${CXX_TEST_PATH}/otbHyperspectralTests1)
set(Hyperspectral_TESTS2 ${CXX_TEST_PATH}/otbHyperspectralTests2)
set(Hyperspectral_TESTS3 ${CXX_TEST_PATH}/otbHyperspectralTests3)

# HyperspectralTests1
add_test(hyTuEigenvalueLikelihoodMaximizationNew ${Hyperspectral_TESTS1}
         otbEigenvalueLikelihoodMaximizationNewTest)

add_test(hyTvEigenvalueLikelihoodMaximizationTest ${Hyperspectral_TESTS1}
           --compare-ascii ${EPSILON_9}
           ${BASELINE_FILES}/hyTvEigenvalueLikelihoodMaximizationTest.txt
           ${TEMP}/hyTvEigenvalueLikelihoodMaximizationTest.txt
         otbEigenvalueLikelihoodMaximizationTest
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${TEMP}/hyTvEigenvalueLikelihoodMaximizationTest.txt)

# HyperspectralTests1
add_test(hyTuVirtualDimensionalityNew ${Hyperspectral_TESTS1}
         otbVirtualDimensionalityNewTest)

add_test(hyTvVirtualDimensionalityTest ${Hyperspectral_TESTS1}
           --compare-ascii ${EPSILON_9}
           ${BASELINE_FILES}/hyTvVirtualDimensionalityTest.txt
           ${TEMP}/hyTvVirtualDimensionalityTest.txt
         otbVirtualDimensionalityTest
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${TEMP}/hyTvVirtualDimensionalityTest.txt
         1E-3)

add_test(hyTuVCAImageFilterNew ${Hyperspectral_TESTS1}
         otbVCAImageFilterNew
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${TEMP}/synthetic_vca_5.hdr
         5 )

add_test(hyTvVCAImageFilterTest1 ${Hyperspectral_TESTS1}
           --compare-image ${EPSILON_9}
           ${BASELINE}/TvHyVertexComponentAnalysisImage.tif
           ${TEMP}/hyTvVCAImageFilterTest.tif
         otbVCAImageFilterTestHighSNR
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${TEMP}/hyTvVCAImageFilterTest.tif
         5 )

add_test(hyTuSparseUnmixingImageFilterNew ${Hyperspectral_TESTS1}
          otbSparseUnmixingImageFilterNew)

# TODO
#add_test(hyTvSparseUnmixingImageFilterTest ${Hyperspectral_TESTS1}
#          otbSparseUnmixingImageFilterTest
#          )


add_test(hyTuUnConstrainedLeastSquareImageFilterNew ${Hyperspectral_TESTS1}
         otbUnConstrainedLeastSquareImageFilterNewTest)

add_test(hyTvUnConstrainedLeastSquareImageFilterTest ${Hyperspectral_TESTS1}
           --compare-image ${EPSILON_9}
           ${BASELINE_FILES}/hyTvUnConstrainedLeastSquareImageFilterTest.tif
           ${TEMP}/hyTvUnConstrainedLeastSquareImageFilterTest.tif
         otbUnConstrainedLeastSquareImageFilterTest
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
         ${TEMP}/hyTvUnConstrainedLeastSquareImageFilterTest.tif)



add_test(hyTuNCLSUnmixingImageFilterNew ${Hyperspectral_TESTS1}
         otbNCLSUnmixingImageFilterNewTest)

add_test(hyTvNCLSUnmixingImageFilterTest ${Hyperspectral_TESTS1}
           --compare-image ${EPSILON_9}
           ${BASELINE_FILES}/hyTvNCLSImageFilterTest.tif
           ${TEMP}/hyTvNCLSImageFilterTest.tif
         otbNCLSUnmixingImageFilterTest
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
         ${TEMP}/hyTvNCLSImageFilterTest.tif
         10)


add_test(hyTuISRAUnmixingImageFilterNew ${Hyperspectral_TESTS1}
         otbISRAUnmixingImageFilterNewTest)

add_test(hyTvISRAUnmixingImageFilterTest ${Hyperspectral_TESTS1}
           --compare-image ${EPSILON_9}
           ${BASELINE_FILES}/hyTvISRAUnmixingImageFilterTest.tif
           ${TEMP}/hyTvISRAUnmixingImageFilterTest.tif
         otbISRAUnmixingImageFilterTest
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
         ${TEMP}/hyTvISRAUnmixingImageFilterTest.tif
         10)

# HyperspectralTests3
add_test(hyTuMDMDNMFImageFilterNew ${Hyperspectral_TESTS3}
         otbMDMDNMFImageFilterNewTest)

add_test(hyTvMDMDNMFImageFilterTest ${Hyperspectral_TESTS3}
         --compare-image ${EPSILON_9}
         ${BASELINE}/hyTvMDMDNMFImageFilterTest.tif
         ${TEMP}/hyTvMDMDNMFImageFilterTest.tif
         otbMDMDNMFImageFilterTest
         ${INPUTDATA}/cupriteSubHsi.tif
         ${TEMP}/hyTvMDMDNMFImageFilterTest.tif
         100)

add_test(hyTvMDMDNMFImageFilterTest2 ${Hyperspectral_TESTS3}
         otbMDMDNMFImageFilterTest2
         ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
         ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
         ${TEMP}/hyTvMDMDNMFImageFilterTest2.tif
         100)


set(Hyperspectral_SRCS1
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

set(Hyperspectral_SRCS2
otbHyperspectralTests2.cxx
otbLocalRxDetectorTest.cxx
otbLocalRxDetectorRoiTest.cxx
)

set(Hyperspectral_SRCS3
otbHyperspectralTests3.cxx
otbMDMDNMFImageFilter.cxx
)

add_executable(otbHyperspectralTests1 otbHyperspectralTests1.cxx ${Hyperspectral_SRCS1})
target_link_libraries(otbHyperspectralTests1 OTBIO OTBHyperspectral OTBMultiScale OTBTesting)

add_executable(otbHyperspectralTests2 otbHyperspectralTests2.cxx ${Hyperspectral_SRCS2})
target_link_libraries(otbHyperspectralTests2 OTBIO OTBHyperspectral OTBTesting)

add_executable(otbHyperspectralTests3 otbHyperspectralTests3.cxx ${Hyperspectral_SRCS3})
target_link_libraries(otbHyperspectralTests3 OTBIO OTBHyperspectral OTBTesting)

endif()
