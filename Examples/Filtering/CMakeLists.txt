PROJECT(FilteringExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(CompositeFilterExample CompositeFilterExample.cxx )
TARGET_LINK_LIBRARIES(CompositeFilterExample OTBCommon OTBIO ITKBasicFilters)

ADD_EXECUTABLE(GradientMagnitudeImageFilter GradientMagnitudeImageFilter.cxx )
TARGET_LINK_LIBRARIES(GradientMagnitudeImageFilter OTBCommon OTBIO)

ADD_EXECUTABLE(GradientMagnitudeRecursiveGaussianImageFilter GradientMagnitudeRecursiveGaussianImageFilter.cxx )
TARGET_LINK_LIBRARIES(GradientMagnitudeRecursiveGaussianImageFilter
OTBCommon OTBIO)

ADD_EXECUTABLE(SecondDerivativeRecursiveGaussianImageFilter SecondDerivativeRecursiveGaussianImageFilter.cxx )
TARGET_LINK_LIBRARIES(SecondDerivativeRecursiveGaussianImageFilter
OTBCommon OTBIO)

ADD_EXECUTABLE(BinaryThresholdImageFilter BinaryThresholdImageFilter.cxx )
TARGET_LINK_LIBRARIES(BinaryThresholdImageFilter OTBCommon OTBIO)

ADD_EXECUTABLE(ThresholdImageFilter ThresholdImageFilter.cxx )
TARGET_LINK_LIBRARIES(ThresholdImageFilter OTBCommon OTBIO)

ADD_EXECUTABLE(CannyEdgeDetectionImageFilter CannyEdgeDetectionImageFilter.cxx )
TARGET_LINK_LIBRARIES(CannyEdgeDetectionImageFilter OTBCommon OTBIO)

ADD_EXECUTABLE(DerivativeImageFilter DerivativeImageFilter.cxx )
TARGET_LINK_LIBRARIES(DerivativeImageFilter OTBCommon OTBIO)

ADD_EXECUTABLE(MedianImageFilter MedianImageFilter.cxx )
TARGET_LINK_LIBRARIES(MedianImageFilter OTBCommon OTBIO)

ADD_EXECUTABLE(MeanImageFilter MeanImageFilter.cxx )
TARGET_LINK_LIBRARIES(MeanImageFilter OTBCommon OTBIO)

ADD_EXECUTABLE(MathematicalMorphologyBinaryFilters MathematicalMorphologyBinaryFilters.cxx )
TARGET_LINK_LIBRARIES(MathematicalMorphologyBinaryFilters OTBCommon
OTBIO)

ADD_EXECUTABLE(MathematicalMorphologyGrayscaleFilters MathematicalMorphologyGrayscaleFilters.cxx )
TARGET_LINK_LIBRARIES(MathematicalMorphologyGrayscaleFilters OTBCommon
OTBIO)

ADD_EXECUTABLE(DiscreteGaussianImageFilter DiscreteGaussianImageFilter.cxx )
TARGET_LINK_LIBRARIES(DiscreteGaussianImageFilter OTBCommon OTBIO ITKBasicFilters)

ADD_EXECUTABLE(LaplacianRecursiveGaussianImageFilter1 LaplacianRecursiveGaussianImageFilter1.cxx )
TARGET_LINK_LIBRARIES(LaplacianRecursiveGaussianImageFilter1 OTBCommon
OTBIO ITKBasicFilters)

ADD_EXECUTABLE(LaplacianRecursiveGaussianImageFilter2 LaplacianRecursiveGaussianImageFilter2.cxx )
TARGET_LINK_LIBRARIES(LaplacianRecursiveGaussianImageFilter2 OTBCommon
OTBIO ITKBasicFilters)

ADD_EXECUTABLE(GradientAnisotropicDiffusionImageFilter GradientAnisotropicDiffusionImageFilter.cxx )
TARGET_LINK_LIBRARIES(GradientAnisotropicDiffusionImageFilter
OTBCommon OTBIO ITKBasicFilters)

ADD_EXECUTABLE(DanielssonDistanceMapImageFilter DanielssonDistanceMapImageFilter.cxx )
TARGET_LINK_LIBRARIES(DanielssonDistanceMapImageFilter OTBCommon OTBIO
ITKCommon ITKBasicFilters)

ADD_EXECUTABLE(GeometriesFilter GeometriesFilter.cxx)
TARGET_LINK_LIBRARIES(GeometriesFilter OTBOGRAdapters)

ADD_EXECUTABLE(GeometriesChangeSpatialReference GeometriesChangeSpatialReference.cxx)
TARGET_LINK_LIBRARIES(GeometriesChangeSpatialReference OTBOGRAdapters)

IF(OTB_USE_MAPNIK)
ADD_EXECUTABLE(RasterizationExample RasterizationExample.cxx)
TARGET_LINK_LIBRARIES(RasterizationExample OTBCommon OTBIO OTBBasicFilters)
ENDIF(OTB_USE_MAPNIK)

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Filtering)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbFilteringExamplesTests)

SET(TOL 0.0)
SET(PNGTOL 2)

# ------- MathematicalMorphologyBinaryFiltersTest----------

ADD_TEST(fiTeMathematicalMorphologyBinaryFiltersTest ${EXE_TESTS}
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

ADD_TEST(fiTeDanielssonDistanceMapImageFilterTest ${EXE_TESTS}
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

ADD_TEST(fiTeMeanImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/MeanImageFilterOutput.png
        ${TEMP}/MeanImageFilterOutput.png
        MeanImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/MeanImageFilterOutput.png
)

# ------- ThresholdImageFilterTest----------

ADD_TEST(fiTeThresholdImageFilterTest ${EXE_TESTS}
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

ADD_TEST(fiTeGradientMagnitudeRecursiveGaussianImageFilterTest ${EXE_TESTS}
        --compare-n-images ${PNGTOL} 1
        ${BASELINE}/GradientMagnitudeRecursiveGaussianImageFilterOutput3.png
        ${TEMP}/GradientMagnitudeRecursiveGaussianImageFilterOutput3.png
        GradientMagnitudeRecursiveGaussianImageFilterTest
        ${INPUTDATA}/ROISpot5.png
        ${TEMP}/GradientMagnitudeRecursiveGaussianImageFilterOutput3.png
    3
)

# ------- GradientMagnitudeRecursiveGaussianImageFilterTest----------

ADD_TEST(fiTeGradientMagnitudeRecursiveGaussianImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/GradientMagnitudeRecursiveGaussianImageFilterOutput5.png
        ${TEMP}/GradientMagnitudeRecursiveGaussianImageFilterOutput5.png
        GradientMagnitudeRecursiveGaussianImageFilterTest
        ${INPUTDATA}/ROISpot5.png
        ${TEMP}/GradientMagnitudeRecursiveGaussianImageFilterOutput5.png
    5
)

# ------- GradientAnisotropicDiffusionImageFilterTest----------

ADD_TEST(fiTeGradientAnisotropicDiffusionImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/GradientAnisotropicDiffusionImageFilterOutput.png
        ${TEMP}/GradientAnisotropicDiffusionImageFilterOutput.png
        GradientAnisotropicDiffusionImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/GradientAnisotropicDiffusionImageFilterOutput.png
    5 0.125 3
)

# ------- GradientMagnitudeImageFilterTest----------

ADD_TEST(fiTeGradientMagnitudeImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/GradientMagnitudeImageFilterOutput.png
        ${TEMP}/GradientMagnitudeImageFilterOutput.png
        GradientMagnitudeImageFilterTest
        ${INPUTDATA}/ROISpot5.png
        ${TEMP}/GradientMagnitudeImageFilterOutput.png
)

# ------- MathematicalMorphologyGrayscaleFiltersTest----------

ADD_TEST(fiTeMathematicalMorphologyGrayscaleFiltersTest ${EXE_TESTS}
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

ADD_TEST(fiTeLaplacianRecursiveGaussianImageFilter1Test ${EXE_TESTS}
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

ADD_TEST(fiTeLaplacianRecursiveGaussianImageFilter2Test ${EXE_TESTS}
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

ADD_TEST(fiTeCannyEdgeDetectionImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/CannyEdgeDetectorImageFilterOutput.png
        ${TEMP}/CannyEdgeDetectorImageFilterOutput.png
        CannyEdgeDetectionImageFilterTest
        ${INPUTDATA}/ROISpot5.png
        ${TEMP}/CannyEdgeDetectorImageFilterOutput.png
 2
)

# ------- DiscreteGaussianImageFilterTest----------

ADD_TEST(fiTeDiscreteGaussianImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/DiscreteGaussianImageFilterOutput.png
        ${TEMP}/DiscreteGaussianImageFilterOutput.png
        DiscreteGaussianImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/DiscreteGaussianImageFilterOutput.png
    4 9
)

# ------- MedianImageFilterTest----------

ADD_TEST(fiTeMedianImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/MedianImageFilterOutput.png
        ${TEMP}/MedianImageFilterOutput.png
        MedianImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/MedianImageFilterOutput.png
)

# ------- BinaryThresholdImageFilterTest----------

ADD_TEST(fiTeBinaryThresholdImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/BinaryThresholdImageFilterOutput.png
        ${TEMP}/BinaryThresholdImageFilterOutput.png
        BinaryThresholdImageFilterTest
        ${INPUTDATA}/ROISpot5.png
        ${TEMP}/BinaryThresholdImageFilterOutput.png
 150 180 0 255
)

# ------- DerivativeImageFilterTest----------

ADD_TEST(fiTeDerivativeImageFilterTest ${EXE_TESTS}
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

INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbFilteringExamplesTests otbFilteringExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbFilteringExamplesTests  ITKAlgorithms ITKStatistics OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
