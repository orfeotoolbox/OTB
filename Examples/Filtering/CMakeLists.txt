project(FilteringExamples)
include_regular_expression("^.*$")

add_executable(CompositeFilterExample CompositeFilterExample.cxx )
target_link_libraries(CompositeFilterExample OTBCommon OTBIO  )

add_executable(GradientMagnitudeImageFilter GradientMagnitudeImageFilter.cxx )
target_link_libraries(GradientMagnitudeImageFilter OTBCommon OTBIO)

add_executable(GradientMagnitudeRecursiveGaussianImageFilter GradientMagnitudeRecursiveGaussianImageFilter.cxx )
target_link_libraries(GradientMagnitudeRecursiveGaussianImageFilter
OTBCommon OTBIO)

add_executable(SecondDerivativeRecursiveGaussianImageFilter SecondDerivativeRecursiveGaussianImageFilter.cxx )
target_link_libraries(SecondDerivativeRecursiveGaussianImageFilter
OTBCommon OTBIO)

add_executable(BinaryThresholdImageFilter BinaryThresholdImageFilter.cxx )
target_link_libraries(BinaryThresholdImageFilter OTBCommon OTBIO)

add_executable(ThresholdImageFilter ThresholdImageFilter.cxx )
target_link_libraries(ThresholdImageFilter OTBCommon OTBIO)

add_executable(CannyEdgeDetectionImageFilter CannyEdgeDetectionImageFilter.cxx )
target_link_libraries(CannyEdgeDetectionImageFilter OTBCommon OTBIO)

add_executable(DerivativeImageFilter DerivativeImageFilter.cxx )
target_link_libraries(DerivativeImageFilter OTBCommon OTBIO)

add_executable(MedianImageFilter MedianImageFilter.cxx )
target_link_libraries(MedianImageFilter OTBCommon OTBIO)

add_executable(MeanImageFilter MeanImageFilter.cxx )
target_link_libraries(MeanImageFilter OTBCommon OTBIO)

add_executable(MathematicalMorphologyBinaryFilters MathematicalMorphologyBinaryFilters.cxx )
target_link_libraries(MathematicalMorphologyBinaryFilters OTBCommon
OTBIO)

add_executable(MathematicalMorphologyGrayscaleFilters MathematicalMorphologyGrayscaleFilters.cxx )
target_link_libraries(MathematicalMorphologyGrayscaleFilters OTBCommon
OTBIO)

add_executable(DiscreteGaussianImageFilter DiscreteGaussianImageFilter.cxx )
target_link_libraries(DiscreteGaussianImageFilter OTBCommon OTBIO  )

add_executable(LaplacianRecursiveGaussianImageFilter1 LaplacianRecursiveGaussianImageFilter1.cxx )
target_link_libraries(LaplacianRecursiveGaussianImageFilter1 OTBCommon
OTBIO  )

add_executable(LaplacianRecursiveGaussianImageFilter2 LaplacianRecursiveGaussianImageFilter2.cxx )
target_link_libraries(LaplacianRecursiveGaussianImageFilter2 OTBCommon
OTBIO  )

add_executable(GradientAnisotropicDiffusionImageFilter GradientAnisotropicDiffusionImageFilter.cxx )
target_link_libraries(GradientAnisotropicDiffusionImageFilter
OTBCommon OTBIO  )

add_executable(DanielssonDistanceMapImageFilter DanielssonDistanceMapImageFilter.cxx )
target_link_libraries(DanielssonDistanceMapImageFilter OTBCommon OTBIO
${ITK_LIBRARIES}  )

add_executable(GeometriesFilter GeometriesFilter.cxx)
target_link_libraries(GeometriesFilter OTBOGRAdapters)

add_executable(GeometriesChangeSpatialReference GeometriesChangeSpatialReference.cxx)
target_link_libraries(GeometriesChangeSpatialReference OTBOGRAdapters)

if(OTB_USE_MAPNIK)
add_executable(RasterizationExample RasterizationExample.cxx)
target_link_libraries(RasterizationExample OTBCommon OTBIO OTBBasicFilters)
endif(OTB_USE_MAPNIK)

if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Filtering)

set(INPUTDATA ${OTB_DATA_ROOT}/Examples)
set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS ${CXX_TEST_PATH}/otbFilteringExamplesTests)

set(TOL 0.0)
set(PNGTOL 2)

# ------- MathematicalMorphologyBinaryFiltersTest----------

add_test(fiTeMathematicalMorphologyBinaryFiltersTest ${EXE_TESTS}
        --compare-n-images ${TOL} 2
        ${BASELINE}/MathematicalMorphologyBinaryErosionOutput.png
        ${TEMP}/MathematicalMorphologyBinaryErosionOutput.png
        ${BASELINE}/MathematicalMorphologyBinaryDilationOutput.png
        ${TEMP}/MathematicalMorphologyBinaryDilationOutput.png
        MathematicalMorphologyBinaryFiltersTest
        ${INPUTDATA}/ROISpot5.png
        ${TEMP}/MathematicalMorphologyBinaryErosionOutput.png
        ${TEMP}/MathematicalMorphologyBinaryDilationOutput.png
    150 180
)

# ------- DanielssonDistanceMapImageFilterTest----------

add_test(fiTeDanielssonDistanceMapImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 2
        ${BASELINE}/DanielssonDistanceMapImageFilterOutput1.png
        ${TEMP}/DanielssonDistanceMapImageFilterOutput1.png
        ${BASELINE}/DanielssonDistanceMapImageFilterOutput2.png
        ${TEMP}/DanielssonDistanceMapImageFilterOutput2.png
        DanielssonDistanceMapImageFilterTest
        ${INPUTDATA}/FivePoints.png
        ${TEMP}/DanielssonDistanceMapImageFilterOutput1.png
        ${TEMP}/DanielssonDistanceMapImageFilterOutput2.png
)

# ------- MeanImageFilterTest----------

add_test(fiTeMeanImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/MeanImageFilterOutput.png
        ${TEMP}/MeanImageFilterOutput.png
        MeanImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/MeanImageFilterOutput.png
)

# ------- ThresholdImageFilterTest----------

add_test(fiTeThresholdImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 3
        ${BASELINE}/ThresholdImageFilterOutputBelow.png
        ${TEMP}/ThresholdImageFilterOutputBelow.png
        ${BASELINE}/ThresholdImageFilterOutputAbove.png
        ${TEMP}/ThresholdImageFilterOutputAbove.png
        ${BASELINE}/ThresholdImageFilterOutputOutside.png
        ${TEMP}/ThresholdImageFilterOutputOutside.png
        ThresholdImageFilterTest
        ${INPUTDATA}/ROISpot5.png
        ${TEMP}/ThresholdImageFilterOutputBelow.png
        ${TEMP}/ThresholdImageFilterOutputAbove.png
        ${TEMP}/ThresholdImageFilterOutputOutside.png
)

# ------- GradientMagnitudeRecursiveGaussianImageFilterTest----------

add_test(fiTeGradientMagnitudeRecursiveGaussianImageFilterTest ${EXE_TESTS}
        --compare-n-images ${PNGTOL} 1
        ${BASELINE}/GradientMagnitudeRecursiveGaussianImageFilterOutput3.png
        ${TEMP}/GradientMagnitudeRecursiveGaussianImageFilterOutput3.png
        GradientMagnitudeRecursiveGaussianImageFilterTest
        ${INPUTDATA}/ROISpot5.png
        ${TEMP}/GradientMagnitudeRecursiveGaussianImageFilterOutput3.png
    3
)

# ------- GradientMagnitudeRecursiveGaussianImageFilterTest----------

add_test(fiTeGradientMagnitudeRecursiveGaussianImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/GradientMagnitudeRecursiveGaussianImageFilterOutput5.png
        ${TEMP}/GradientMagnitudeRecursiveGaussianImageFilterOutput5.png
        GradientMagnitudeRecursiveGaussianImageFilterTest
        ${INPUTDATA}/ROISpot5.png
        ${TEMP}/GradientMagnitudeRecursiveGaussianImageFilterOutput5.png
    5
)

# ------- GradientAnisotropicDiffusionImageFilterTest----------

add_test(fiTeGradientAnisotropicDiffusionImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/GradientAnisotropicDiffusionImageFilterOutput.png
        ${TEMP}/GradientAnisotropicDiffusionImageFilterOutput.png
        GradientAnisotropicDiffusionImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/GradientAnisotropicDiffusionImageFilterOutput.png
    5 0.125 3
)

# ------- GradientMagnitudeImageFilterTest----------

add_test(fiTeGradientMagnitudeImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/GradientMagnitudeImageFilterOutput.png
        ${TEMP}/GradientMagnitudeImageFilterOutput.png
        GradientMagnitudeImageFilterTest
        ${INPUTDATA}/ROISpot5.png
        ${TEMP}/GradientMagnitudeImageFilterOutput.png
)

# ------- MathematicalMorphologyGrayscaleFiltersTest----------

add_test(fiTeMathematicalMorphologyGrayscaleFiltersTest ${EXE_TESTS}
        --compare-n-images ${TOL} 2
        ${BASELINE}/MathematicalMorphologyGrayscaleErosionOutput.png
        ${TEMP}/MathematicalMorphologyGrayscaleErosionOutput.png
        ${BASELINE}/MathematicalMorphologyGrayscaleDilationOutput.png
        ${TEMP}/MathematicalMorphologyGrayscaleDilationOutput.png
        MathematicalMorphologyGrayscaleFiltersTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/MathematicalMorphologyGrayscaleErosionOutput.png
        ${TEMP}/MathematicalMorphologyGrayscaleDilationOutput.png
    150 180
)

# ------- LaplacianRecursiveGaussianImageFilter1Test----------

add_test(fiTeLaplacianRecursiveGaussianImageFilter1Test ${EXE_TESTS}
        --compare-n-images ${TOL} 2
        ${BASELINE}/LaplacianRecursiveGaussianImageFilteroutput5.hdr
        ${TEMP}/LaplacianRecursiveGaussianImageFilteroutput5.hdr
        ${BASELINE}/LaplacianRecursiveGaussianImageFilterOutput5.png
        ${TEMP}/LaplacianRecursiveGaussianImageFilterOutput5.png
        LaplacianRecursiveGaussianImageFilter1Test
        ${INPUTDATA}/ROISpot5.png
        ${TEMP}/LaplacianRecursiveGaussianImageFilteroutput5.hdr
        5
        ${TEMP}/LaplacianRecursiveGaussianImageFilterOutput5.png
)


# ------- LaplacianRecursiveGaussianImageFilter2Test----------

add_test(fiTeLaplacianRecursiveGaussianImageFilter2Test ${EXE_TESTS}
        --compare-n-images ${TOL} 2
        ${BASELINE}/LaplacianRecursiveGaussianImageFilter2output5.hdr
        ${TEMP}/LaplacianRecursiveGaussianImageFilter2output5.hdr
        ${BASELINE}/LaplacianRecursiveGaussianImageFilter2Output5.png
        ${TEMP}/LaplacianRecursiveGaussianImageFilter2Output5.png
        LaplacianRecursiveGaussianImageFilter2Test
        ${INPUTDATA}/ROISpot5.png
        ${TEMP}/LaplacianRecursiveGaussianImageFilter2output5.hdr
        5
        ${TEMP}/LaplacianRecursiveGaussianImageFilter2Output5.png
)

# ------- CannyEdgeDetectionImageFilterTest----------

add_test(fiTeCannyEdgeDetectionImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/CannyEdgeDetectorImageFilterOutput.png
        ${TEMP}/CannyEdgeDetectorImageFilterOutput.png
        CannyEdgeDetectionImageFilterTest
        ${INPUTDATA}/ROISpot5.png
        ${TEMP}/CannyEdgeDetectorImageFilterOutput.png
 2
)

# ------- DiscreteGaussianImageFilterTest----------

add_test(fiTeDiscreteGaussianImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/DiscreteGaussianImageFilterOutput.png
        ${TEMP}/DiscreteGaussianImageFilterOutput.png
        DiscreteGaussianImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/DiscreteGaussianImageFilterOutput.png
    4 9
)

# ------- MedianImageFilterTest----------

add_test(fiTeMedianImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/MedianImageFilterOutput.png
        ${TEMP}/MedianImageFilterOutput.png
        MedianImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/MedianImageFilterOutput.png
)

# ------- BinaryThresholdImageFilterTest----------

add_test(fiTeBinaryThresholdImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/BinaryThresholdImageFilterOutput.png
        ${TEMP}/BinaryThresholdImageFilterOutput.png
        BinaryThresholdImageFilterTest
        ${INPUTDATA}/ROISpot5.png
        ${TEMP}/BinaryThresholdImageFilterOutput.png
 150 180 0 255
)

# ------- DerivativeImageFilterTest----------

add_test(fiTeDerivativeImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 2
        ${BASELINE}/DerivativeImageFilterFloatOutput.hdr
        ${TEMP}/DerivativeImageFilterFloatOutput.hdr
        ${BASELINE}/DerivativeImageFilterOutput.png
        ${TEMP}/DerivativeImageFilterOutput.png
        DerivativeImageFilterTest
        ${INPUTDATA}/ROISpot5.png
        ${TEMP}/DerivativeImageFilterFloatOutput.hdr
        ${TEMP}/DerivativeImageFilterOutput.png
    1 0
)

include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbFilteringExamplesTests otbFilteringExamplesTests.cxx)
target_link_libraries(otbFilteringExamplesTests   ${ITK_LIBRARIES} OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBTesting)

endif( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
