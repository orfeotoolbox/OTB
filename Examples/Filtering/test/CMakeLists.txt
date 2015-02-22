set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Filtering)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)

# ------- MathematicalMorphologyBinaryFiltersTest----------

otb_add_test(NAME fiTeMathematicalMorphologyBinaryFiltersTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/MathematicalMorphologyBinaryErosionOutput.png
    ${TEMP}/MathematicalMorphologyBinaryErosionOutput.png
    ${BASELINE}/MathematicalMorphologyBinaryDilationOutput.png
    ${TEMP}/MathematicalMorphologyBinaryDilationOutput.png
  Execute $<TARGET_FILE:MathematicalMorphologyBinaryFilters>
    ${INPUTDATA}/ROISpot5.png
    ${TEMP}/MathematicalMorphologyBinaryErosionOutput.png
    ${TEMP}/MathematicalMorphologyBinaryDilationOutput.png
    150 180
)

# ------- DanielssonDistanceMapImageFilterTest----------

otb_add_test(NAME fiTeDanielssonDistanceMapImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/DanielssonDistanceMapImageFilterOutput1.png
    ${TEMP}/DanielssonDistanceMapImageFilterOutput1.png
    ${BASELINE}/DanielssonDistanceMapImageFilterOutput2.png
    ${TEMP}/DanielssonDistanceMapImageFilterOutput2.png
  Execute $<TARGET_FILE:DanielssonDistanceMapImageFilter>
    ${INPUTDATA}/FivePoints.png
    ${TEMP}/DanielssonDistanceMapImageFilterOutput1.png
    ${TEMP}/DanielssonDistanceMapImageFilterOutput2.png
)

# ------- MeanImageFilterTest----------

otb_add_test(NAME fiTeMeanImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/MeanImageFilterOutput.png
    ${TEMP}/MeanImageFilterOutput.png
  Execute $<TARGET_FILE:MeanImageFilter>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/MeanImageFilterOutput.png
)

# ------- ThresholdImageFilterTest----------

otb_add_test(NAME fiTeThresholdImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 3
    ${BASELINE}/ThresholdImageFilterOutputBelow.png
    ${TEMP}/ThresholdImageFilterOutputBelow.png
    ${BASELINE}/ThresholdImageFilterOutputAbove.png
    ${TEMP}/ThresholdImageFilterOutputAbove.png
    ${BASELINE}/ThresholdImageFilterOutputOutside.png
    ${TEMP}/ThresholdImageFilterOutputOutside.png
  Execute $<TARGET_FILE:ThresholdImageFilter>
    ${INPUTDATA}/ROISpot5.png
    ${TEMP}/ThresholdImageFilterOutputBelow.png
    ${TEMP}/ThresholdImageFilterOutputAbove.png
    ${TEMP}/ThresholdImageFilterOutputOutside.png
)

# ------- GradientMagnitudeRecursiveGaussianImageFilterTest----------

otb_add_test(NAME fiTeGradientMagnitudeRecursiveGaussianImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/GradientMagnitudeRecursiveGaussianImageFilterOutput3.png
    ${TEMP}/GradientMagnitudeRecursiveGaussianImageFilterOutput3.png
  Execute $<TARGET_FILE:GradientMagnitudeRecursiveGaussianImageFilter>
    ${INPUTDATA}/ROISpot5.png
    ${TEMP}/GradientMagnitudeRecursiveGaussianImageFilterOutput3.png
    3
)

# ------- GradientAnisotropicDiffusionImageFilterTest----------

otb_add_test(NAME fiTeGradientAnisotropicDiffusionImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/GradientAnisotropicDiffusionImageFilterOutput.png
    ${TEMP}/GradientAnisotropicDiffusionImageFilterOutput.png
  Execute $<TARGET_FILE:GradientAnisotropicDiffusionImageFilter>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/GradientAnisotropicDiffusionImageFilterOutput.png
    5 0.125 3
)

# ------- GradientMagnitudeImageFilterTest----------

otb_add_test(NAME fiTeGradientMagnitudeImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/GradientMagnitudeImageFilterOutput.png
    ${TEMP}/GradientMagnitudeImageFilterOutput.png
  GradientMagnitudeImageFilterTest
    ${INPUTDATA}/ROISpot5.png
    ${TEMP}/GradientMagnitudeImageFilterOutput.png
)

# ------- MathematicalMorphologyGrayscaleFiltersTest----------

otb_add_test(NAME fiTeMathematicalMorphologyGrayscaleFiltersTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/MathematicalMorphologyGrayscaleErosionOutput.png
    ${TEMP}/MathematicalMorphologyGrayscaleErosionOutput.png
    ${BASELINE}/MathematicalMorphologyGrayscaleDilationOutput.png
    ${TEMP}/MathematicalMorphologyGrayscaleDilationOutput.png
  Execute $<TARGET_FILE:MathematicalMorphologyGrayscaleFilters>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/MathematicalMorphologyGrayscaleErosionOutput.png
    ${TEMP}/MathematicalMorphologyGrayscaleDilationOutput.png
    150 180
)

# ------- LaplacianRecursiveGaussianImageFilter1Test----------

otb_add_test(NAME fiTeLaplacianRecursiveGaussianImageFilter1Test COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/LaplacianRecursiveGaussianImageFilteroutput5.hdr
    ${TEMP}/LaplacianRecursiveGaussianImageFilteroutput5.hdr
    ${BASELINE}/LaplacianRecursiveGaussianImageFilterOutput5.png
    ${TEMP}/LaplacianRecursiveGaussianImageFilterOutput5.png
  Execute $<TARGET_FILE:LaplacianRecursiveGaussianImageFilter1>
    ${INPUTDATA}/ROISpot5.png
    ${TEMP}/LaplacianRecursiveGaussianImageFilteroutput5.hdr
    5
    ${TEMP}/LaplacianRecursiveGaussianImageFilterOutput5.png
)


# ------- LaplacianRecursiveGaussianImageFilter2Test----------

otb_add_test(NAME fiTeLaplacianRecursiveGaussianImageFilter2Test COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/LaplacianRecursiveGaussianImageFilter2output5.hdr
    ${TEMP}/LaplacianRecursiveGaussianImageFilter2output5.hdr
    ${BASELINE}/LaplacianRecursiveGaussianImageFilter2Output5.png
    ${TEMP}/LaplacianRecursiveGaussianImageFilter2Output5.png
  Execute $<TARGET_FILE:LaplacianRecursiveGaussianImageFilter2>
    ${INPUTDATA}/ROISpot5.png
    ${TEMP}/LaplacianRecursiveGaussianImageFilter2output5.hdr
    5
    ${TEMP}/LaplacianRecursiveGaussianImageFilter2Output5.png
)

# ------- CannyEdgeDetectionImageFilterTest----------

otb_add_test(NAME fiTeCannyEdgeDetectionImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/CannyEdgeDetectorImageFilterOutput.png
    ${TEMP}/CannyEdgeDetectorImageFilterOutput.png
  Execute $<TARGET_FILE:CannyEdgeDetectionImageFilter>
    ${INPUTDATA}/ROISpot5.png
    ${TEMP}/CannyEdgeDetectorImageFilterOutput.png
    2
)

# ------- DiscreteGaussianImageFilterTest----------

otb_add_test(NAME fiTeDiscreteGaussianImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/DiscreteGaussianImageFilterOutput.png
    ${TEMP}/DiscreteGaussianImageFilterOutput.png
  Execute $<TARGET_FILE:DiscreteGaussianImageFilter>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/DiscreteGaussianImageFilterOutput.png
    4 9
)

# ------- MedianImageFilterTest----------

otb_add_test(NAME fiTeMedianImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/MedianImageFilterOutput.png
    ${TEMP}/MedianImageFilterOutput.png
  Execute $<TARGET_FILE:MedianImageFilter>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/MedianImageFilterOutput.png
)

# ------- BinaryThresholdImageFilterTest----------

otb_add_test(NAME fiTeBinaryThresholdImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/BinaryThresholdImageFilterOutput.png
    ${TEMP}/BinaryThresholdImageFilterOutput.png
  Execute $<TARGET_FILE:BinaryThresholdImageFilter>
    ${INPUTDATA}/ROISpot5.png
    ${TEMP}/BinaryThresholdImageFilterOutput.png
    150 180 0 255
)

# ------- DerivativeImageFilterTest----------

otb_add_test(NAME fiTeDerivativeImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/DerivativeImageFilterFloatOutput.hdr
    ${TEMP}/DerivativeImageFilterFloatOutput.hdr
    ${BASELINE}/DerivativeImageFilterOutput.png
    ${TEMP}/DerivativeImageFilterOutput.png
  Execute $<TARGET_FILE:DerivativeImageFilter>
    ${INPUTDATA}/ROISpot5.png
    ${TEMP}/DerivativeImageFilterFloatOutput.hdr
    ${TEMP}/DerivativeImageFilterOutput.png
    1 0
)
