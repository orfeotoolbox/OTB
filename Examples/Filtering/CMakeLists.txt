PROJECT(FilteringExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

#ADD_EXECUTABLE(AntiAliasBinaryImageFilter AntiAliasBinaryImageFilter.cxx )
#TARGET_LINK_LIBRARIES(AntiAliasBinaryImageFilter ITKCommon ITKIO)

ADD_EXECUTABLE(CompositeFilterExample CompositeFilterExample.cxx )
TARGET_LINK_LIBRARIES(CompositeFilterExample OTBCommon OTBIO ITKCommon ITKIO ITKBasicFilters)

#ADD_EXECUTABLE(FlipImageFilter FlipImageFilter.cxx )
#TARGET_LINK_LIBRARIES(FlipImageFilter ITKCommon ITKIO)

#ADD_EXECUTABLE(FFTImageFilter FFTImageFilter.cxx )
#TARGET_LINK_LIBRARIES(FFTImageFilter ITKCommon ITKIO)

IF( USE_FFTWF )
  #ADD_EXECUTABLE(FFTDirectInverse2 FFTDirectInverse2.cxx )
  #TARGET_LINK_LIBRARIES(FFTDirectInverse2 ITKCommon ITKIO ITKAlgorithms )
ENDIF( USE_FFTWF )

ADD_EXECUTABLE(GradientMagnitudeImageFilter GradientMagnitudeImageFilter.cxx )
TARGET_LINK_LIBRARIES(GradientMagnitudeImageFilter OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(GradientMagnitudeRecursiveGaussianImageFilter GradientMagnitudeRecursiveGaussianImageFilter.cxx )
TARGET_LINK_LIBRARIES(GradientMagnitudeRecursiveGaussianImageFilter
OTBCommon OTBIO ITKCommon ITKIO)

#ADD_EXECUTABLE(GradientRecursiveGaussianImageFilter GradientRecursiveGaussianImageFilter.cxx )
#TARGET_LINK_LIBRARIES(GradientRecursiveGaussianImageFilter ITKCommon ITKIO)

ADD_EXECUTABLE(SecondDerivativeRecursiveGaussianImageFilter SecondDerivativeRecursiveGaussianImageFilter.cxx )
TARGET_LINK_LIBRARIES(SecondDerivativeRecursiveGaussianImageFilter
OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(BinaryThresholdImageFilter BinaryThresholdImageFilter.cxx )
TARGET_LINK_LIBRARIES(BinaryThresholdImageFilter OTBCommon OTBIO ITKCommon ITKIO)

#ADD_EXECUTABLE(BinaryMedianImageFilter BinaryMedianImageFilter.cxx )
#TARGET_LINK_LIBRARIES(BinaryMedianImageFilter ITKCommon ITKIO)

#ADD_EXECUTABLE(VotingBinaryHoleFillingImageFilter VotingBinaryHoleFillingImageFilter.cxx )
#TARGET_LINK_LIBRARIES(VotingBinaryHoleFillingImageFilter ITKCommon ITKIO)

#ADD_EXECUTABLE(VotingBinaryIterativeHoleFillingImageFilter VotingBinaryIterativeHoleFillingImageFilter.cxx )
#TARGET_LINK_LIBRARIES(VotingBinaryIterativeHoleFillingImageFilter ITKCommon ITKIO)

#ADD_EXECUTABLE(OtsuThresholdImageFilter OtsuThresholdImageFilter.cxx )
#TARGET_LINK_LIBRARIES(OtsuThresholdImageFilter ITKCommon ITKIO)

#ADD_EXECUTABLE(OtsuMultipleThresholdImageFilter OtsuMultipleThresholdImageFilter.cxx )
#TARGET_LINK_LIBRARIES(OtsuMultipleThresholdImageFilter                      ITKCommon ITKIO ITKStatistics)

ADD_EXECUTABLE(ThresholdImageFilter ThresholdImageFilter.cxx )
TARGET_LINK_LIBRARIES(ThresholdImageFilter OTBCommon OTBIO ITKCommon ITKIO)

#ADD_EXECUTABLE(CastingImageFilters CastingImageFilters.cxx )
#TARGET_LINK_LIBRARIES(CastingImageFilters ITKCommon ITKIO)

ADD_EXECUTABLE(CannyEdgeDetectionImageFilter CannyEdgeDetectionImageFilter.cxx )
TARGET_LINK_LIBRARIES(CannyEdgeDetectionImageFilter OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(DerivativeImageFilter DerivativeImageFilter.cxx )
TARGET_LINK_LIBRARIES(DerivativeImageFilter OTBCommon OTBIO ITKCommon ITKIO)

#ADD_EXECUTABLE(SigmoidImageFilter SigmoidImageFilter.cxx )
#TARGET_LINK_LIBRARIES(SigmoidImageFilter ITKCommon ITKIO)

ADD_EXECUTABLE(MedianImageFilter MedianImageFilter.cxx )
TARGET_LINK_LIBRARIES(MedianImageFilter OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(MeanImageFilter MeanImageFilter.cxx )
TARGET_LINK_LIBRARIES(MeanImageFilter OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(MathematicalMorphologyBinaryFilters MathematicalMorphologyBinaryFilters.cxx )
TARGET_LINK_LIBRARIES(MathematicalMorphologyBinaryFilters OTBCommon
OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(MathematicalMorphologyGrayscaleFilters MathematicalMorphologyGrayscaleFilters.cxx )
TARGET_LINK_LIBRARIES(MathematicalMorphologyGrayscaleFilters OTBCommon
OTBIO ITKCommon ITKIO)

#ADD_EXECUTABLE(MorphologicalImageEnhancement MorphologicalImageEnhancement.cxx )
#TARGET_LINK_LIBRARIES(MorphologicalImageEnhancement ITKCommon ITKIO)

#ADD_EXECUTABLE(CurvatureFlowImageFilter CurvatureFlowImageFilter.cxx )
#TARGET_LINK_LIBRARIES(CurvatureFlowImageFilter ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(MinMaxCurvatureFlowImageFilter MinMaxCurvatureFlowImageFilter.cxx )
#TARGET_LINK_LIBRARIES(MinMaxCurvatureFlowImageFilter ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(BinaryMinMaxCurvatureFlowImageFilter BinaryMinMaxCurvatureFlowImageFilter.cxx )
#TARGET_LINK_LIBRARIES(BinaryMinMaxCurvatureFlowImageFilter ITKCommon ITKBasicFilters ITKIO)

ADD_EXECUTABLE(DiscreteGaussianImageFilter DiscreteGaussianImageFilter.cxx )
TARGET_LINK_LIBRARIES(DiscreteGaussianImageFilter OTBCommon OTBIO ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(GaussianBlurImageFunction GaussianBlurImageFunction.cxx )
#TARGET_LINK_LIBRARIES(GaussianBlurImageFunction ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(SmoothingRecursiveGaussianImageFilter SmoothingRecursiveGaussianImageFilter.cxx )
#TARGET_LINK_LIBRARIES(SmoothingRecursiveGaussianImageFilter ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(SmoothingRecursiveGaussianImageFilter2 SmoothingRecursiveGaussianImageFilter2.cxx )
#TARGET_LINK_LIBRARIES(SmoothingRecursiveGaussianImageFilter2 ITKCommon ITKBasicFilters ITKIO)

ADD_EXECUTABLE(LaplacianRecursiveGaussianImageFilter1 LaplacianRecursiveGaussianImageFilter1.cxx )
TARGET_LINK_LIBRARIES(LaplacianRecursiveGaussianImageFilter1 OTBCommon
OTBIO ITKCommon ITKBasicFilters ITKIO)

ADD_EXECUTABLE(LaplacianRecursiveGaussianImageFilter2 LaplacianRecursiveGaussianImageFilter2.cxx )
TARGET_LINK_LIBRARIES(LaplacianRecursiveGaussianImageFilter2 OTBCommon
OTBIO ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(ScaleSpaceGenerator2D ScaleSpaceGenerator2D.cxx )
#TARGET_LINK_LIBRARIES(ScaleSpaceGenerator2D ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(BinomialBlurImageFilter BinomialBlurImageFilter.cxx )
#TARGET_LINK_LIBRARIES(BinomialBlurImageFilter ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(CurvatureAnisotropicDiffusionImageFilter CurvatureAnisotropicDiffusionImageFilter.cxx )
#TARGET_LINK_LIBRARIES(CurvatureAnisotropicDiffusionImageFilter ITKCommon ITKBasicFilters ITKIO)

ADD_EXECUTABLE(GradientAnisotropicDiffusionImageFilter GradientAnisotropicDiffusionImageFilter.cxx )
TARGET_LINK_LIBRARIES(GradientAnisotropicDiffusionImageFilter
OTBCommon OTBIO ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(VectorGradientAnisotropicDiffusionImageFilter VectorGradientAnisotropicDiffusionImageFilter.cxx )
#TARGET_LINK_LIBRARIES(VectorGradientAnisotropicDiffusionImageFilter ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(VectorCurvatureAnisotropicDiffusionImageFilter VectorCurvatureAnisotropicDiffusionImageFilter.cxx )
#TARGET_LINK_LIBRARIES(VectorCurvatureAnisotropicDiffusionImageFilter ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(RGBCurvatureAnisotropicDiffusionImageFilter RGBCurvatureAnisotropicDiffusionImageFilter.cxx )
#TARGET_LINK_LIBRARIES(RGBCurvatureAnisotropicDiffusionImageFilter ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(RGBGradientAnisotropicDiffusionImageFilter RGBGradientAnisotropicDiffusionImageFilter.cxx )
#TARGET_LINK_LIBRARIES(RGBGradientAnisotropicDiffusionImageFilter ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(RGBToGrayscale RGBToGrayscale.cxx )
#TARGET_LINK_LIBRARIES(RGBToGrayscale ITKCommon ITKBasicFilters ITKIO)

ADD_EXECUTABLE(DanielssonDistanceMapImageFilter DanielssonDistanceMapImageFilter.cxx )
TARGET_LINK_LIBRARIES(DanielssonDistanceMapImageFilter OTBCommon OTBIO
ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(SignedDanielssonDistanceMapImageFilter SignedDanielssonDistanceMapImageFilter.cxx )
#TARGET_LINK_LIBRARIES(SignedDanielssonDistanceMapImageFilter ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(BilateralImageFilter BilateralImageFilter.cxx )
#TARGET_LINK_LIBRARIES(BilateralImageFilter ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(ResampleImageFilter ResampleImageFilter.cxx )
#TARGET_LINK_LIBRARIES(ResampleImageFilter ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(ResampleOrientedImageFilter ResampleOrientedImageFilter.cxx )
#TARGET_LINK_LIBRARIES(ResampleOrientedImageFilter ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(ResampleImageFilter2 ResampleImageFilter2.cxx )
#TARGET_LINK_LIBRARIES(ResampleImageFilter2 ITKCommon ITKBasicFilters ITKIO)

IF(NOT BORLAND)
#ADD_EXECUTABLE(FFTImageFilterFourierDomainFiltering FFTImageFilterFourierDomainFiltering.cxx )
#TARGET_LINK_LIBRARIES(FFTImageFilterFourierDomainFiltering ITKCommon ITKIO)

#ADD_EXECUTABLE(LaplacianImageFilter LaplacianImageFilter.cxx )
#TARGET_LINK_LIBRARIES(LaplacianImageFilter ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(ResampleImageFilter3 ResampleImageFilter3.cxx )
#TARGET_LINK_LIBRARIES(ResampleImageFilter3 ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(ResampleImageFilter4 ResampleImageFilter4.cxx )
#TARGET_LINK_LIBRARIES(ResampleImageFilter4 ITKCommon ITKBasicFilters ITKIO)
ENDIF(NOT BORLAND)

#ADD_EXECUTABLE(ResampleImageFilter5 ResampleImageFilter5.cxx )
#TARGET_LINK_LIBRARIES(ResampleImageFilter5 ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(ResampleImageFilter6 ResampleImageFilter6.cxx )
#TARGET_LINK_LIBRARIES(ResampleImageFilter6 ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(ResampleImageFilter8 ResampleImageFilter8.cxx )
#TARGET_LINK_LIBRARIES(ResampleImageFilter8 ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(ResampleImageFilter9 ResampleImageFilter9.cxx )
#TARGET_LINK_LIBRARIES(ResampleImageFilter9 ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(ResampleVolumesToBeIsotropic ResampleVolumesToBeIsotropic.cxx )
#TARGET_LINK_LIBRARIES(ResampleVolumesToBeIsotropic ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(WarpImageFilter1 WarpImageFilter1.cxx )
#TARGET_LINK_LIBRARIES(WarpImageFilter1 ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(SubsampleVolume SubsampleVolume.cxx )
#TARGET_LINK_LIBRARIES(SubsampleVolume ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(LaplacianSharpeningImageFilter LaplacianSharpeningImageFilter.cxx )
#TARGET_LINK_LIBRARIES(LaplacianSharpeningImageFilter ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(SurfaceExtraction SurfaceExtraction.cxx )
#TARGET_LINK_LIBRARIES(SurfaceExtraction ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(VectorIndexSelection VectorIndexSelection.cxx )
#TARGET_LINK_LIBRARIES(VectorIndexSelection ITKCommon ITKBasicFilters ITKIO)


#ADD_EXECUTABLE(FilteringExamples FilteringExamples.cxx)
#TARGET_LINK_LIBRARIES(FilteringExamples ITKIO ITKBasicFilters ITKCommon)

#ADD_EXECUTABLE(FilteringExamples2 FilteringExamples2.cxx)
#TARGET_LINK_LIBRARIES(FilteringExamples2 ITKIO ITKBasicFilters ITKCommon)

#ADD_EXECUTABLE(FilteringExamples3 FilteringExamples3.cxx)
#TARGET_LINK_LIBRARIES(FilteringExamples3 ITKIO ITKBasicFilters ITKCommon)

#ADD_EXECUTABLE(FilteringExamples4 FilteringExamples4.cxx)
#TARGET_LINK_LIBRARIES(FilteringExamples4 ITKIO ITKBasicFilters ITKCommon)

#ADD_EXECUTABLE(FilteringExamples5 FilteringExamples5.cxx)
#TARGET_LINK_LIBRARIES(FilteringExamples5 ITKIO ITKBasicFilters ITKCommon                      ITKStatistics)

#ADD_EXECUTABLE(FilteringExamples7 FilteringExamples7.cxx)
#TARGET_LINK_LIBRARIES(FilteringExamples7 ITKIO ITKBasicFilters ITKCommon)

#ADD_EXECUTABLE(FilteringExamples8 FilteringExamples8.cxx)
#TARGET_LINK_LIBRARIES(FilteringExamples8 ITKIO ITKBasicFilters ITKCommon)

#ADD_EXECUTABLE(FilteringExamples6 FilteringExamples6.cxx)
IF( USE_FFTWF )
  #TARGET_LINK_LIBRARIES(FilteringExamples6 ITKIO ITKBasicFilters ITKCommon ITKAlgorithms)
ELSE( USE_FFTWF )
  #TARGET_LINK_LIBRARIES(FilteringExamples6 ITKIO ITKBasicFilters ITKCommon)
ENDIF( USE_FFTWF )

#ADD_EXECUTABLE(DiffusionTensor3DReconstructionImageFilter DiffusionTensor3DReconstructionImageFilter.cxx)
#TARGET_LINK_LIBRARIES(DiffusionTensor3DReconstructionImageFilter ITKIO ITKBasicFilters ITKCommon)


  #ADD_EXECUTABLE(FFTDirectInverse FFTDirectInverse.cxx )
  #TARGET_LINK_LIBRARIES(FFTDirectInverse ITKCommon ITKIO)

IF(NOT BORLAND)
  #ADD_EXECUTABLE(DigitallyReconstructedRadiograph1 DigitallyReconstructedRadiograph1.cxx )
  #TARGET_LINK_LIBRARIES(DigitallyReconstructedRadiograph1 ITKCommon ITKIO)

  #ADD_EXECUTABLE(ResampleImageFilter7 ResampleImageFilter7.cxx )
  #TARGET_LINK_LIBRARIES(ResampleImageFilter7 ITKCommon ITKBasicFilters ITKIO)
ENDIF(NOT BORLAND)


IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_EXAMPLES_TESTING )


SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Filtering)

SET(INPUTDATA ${OTB_SOURCE_DIR}/Examples/Data)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbFilteringExamplesTests)

SET(TOL 0.0)
SET(PNGTOL 2)

# ------- MathematicalMorphologyBinaryFiltersTest----------

ADD_TEST(MathematicalMorphologyBinaryFiltersTest ${EXE_TESTS}
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

ADD_TEST(DanielssonDistanceMapImageFilterTest ${EXE_TESTS}
	--compare-n-images ${TOL} 3
	${BASELINE}/DanielssonDistanceMapImageFilterOutput1.png
	${TEMP}/DanielssonDistanceMapImageFilterOutput1.png
	${BASELINE}/DanielssonDistanceMapImageFilterOutput2.png
	${TEMP}/DanielssonDistanceMapImageFilterOutput2.png
	${BASELINE}/DanielssonDistanceMapImageFilterOutput3.mhd
	${TEMP}/DanielssonDistanceMapImageFilterOutput3.mhd
	DanielssonDistanceMapImageFilterTest
	${INPUTDATA}/FivePoints.png
	${TEMP}/DanielssonDistanceMapImageFilterOutput1.png
	${TEMP}/DanielssonDistanceMapImageFilterOutput2.png
	${TEMP}/DanielssonDistanceMapImageFilterOutput3.mhd
)

# ------- MeanImageFilterTest----------

ADD_TEST(MeanImageFilterTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/MeanImageFilterOutput.png
	${TEMP}/MeanImageFilterOutput.png
	MeanImageFilterTest
	${INPUTDATA}/QB_Suburb.png
	${TEMP}/MeanImageFilterOutput.png
)

# ------- ThresholdImageFilterTest----------

ADD_TEST(ThresholdImageFilterTest ${EXE_TESTS}
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

ADD_TEST(GradientMagnitudeRecursiveGaussianImageFilterTest ${EXE_TESTS}
	--compare-n-images ${PNGTOL} 1
	${BASELINE}/GradientMagnitudeRecursiveGaussianImageFilterOutput3.png
	${TEMP}/GradientMagnitudeRecursiveGaussianImageFilterOutput3.png
	GradientMagnitudeRecursiveGaussianImageFilterTest
	${INPUTDATA}/ROISpot5.png
	${TEMP}/GradientMagnitudeRecursiveGaussianImageFilterOutput3.png
    3
)

# ------- GradientMagnitudeRecursiveGaussianImageFilterTest----------

ADD_TEST(GradientMagnitudeRecursiveGaussianImageFilterTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/GradientMagnitudeRecursiveGaussianImageFilterOutput5.png
	${TEMP}/GradientMagnitudeRecursiveGaussianImageFilterOutput5.png
	GradientMagnitudeRecursiveGaussianImageFilterTest
	${INPUTDATA}/ROISpot5.png
	${TEMP}/GradientMagnitudeRecursiveGaussianImageFilterOutput5.png
    5
)

# ------- GradientAnisotropicDiffusionImageFilterTest----------

ADD_TEST(GradientAnisotropicDiffusionImageFilterTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/GradientAnisotropicDiffusionImageFilterOutput.png
	${TEMP}/GradientAnisotropicDiffusionImageFilterOutput.png
	GradientAnisotropicDiffusionImageFilterTest
	${INPUTDATA}/QB_Suburb.png
	${TEMP}/GradientAnisotropicDiffusionImageFilterOutput.png
    5 0.125 3
)

# ------- GradientMagnitudeImageFilterTest----------

ADD_TEST(GradientMagnitudeImageFilterTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/GradientMagnitudeImageFilterOutput.png
	${TEMP}/GradientMagnitudeImageFilterOutput.png
	GradientMagnitudeImageFilterTest
	${INPUTDATA}/ROISpot5.png
	${TEMP}/GradientMagnitudeImageFilterOutput.png
)

# ------- MathematicalMorphologyGrayscaleFiltersTest----------

ADD_TEST(MathematicalMorphologyGrayscaleFiltersTest ${EXE_TESTS}
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

ADD_TEST(LaplacianRecursiveGaussianImageFilter1Test ${EXE_TESTS}
	--compare-n-images ${TOL} 2
	${BASELINE}/LaplacianRecursiveGaussianImageFilteroutput3.mha
	${TEMP}/LaplacianRecursiveGaussianImageFilteroutput3.mha
        ${BASELINE}/LaplacianRecursiveGaussianImageFilterOutput3.png
	${TEMP}/LaplacianRecursiveGaussianImageFilterOutput3.png
	LaplacianRecursiveGaussianImageFilter1Test
	${INPUTDATA}/ROISpot5.png
	${TEMP}/LaplacianRecursiveGaussianImageFilteroutput3.mha
        3
        ${TEMP}/LaplacianRecursiveGaussianImageFilterOutput3.png
)

# ------- LaplacianRecursiveGaussianImageFilter1Test----------

ADD_TEST(LaplacianRecursiveGaussianImageFilter1Test ${EXE_TESTS}
	--compare-n-images ${TOL} 2
	${BASELINE}/LaplacianRecursiveGaussianImageFilteroutput5.mha
	${TEMP}/LaplacianRecursiveGaussianImageFilteroutput5.mha
        ${BASELINE}/LaplacianRecursiveGaussianImageFilterOutput5.png
	${TEMP}/LaplacianRecursiveGaussianImageFilterOutput5.png
	LaplacianRecursiveGaussianImageFilter1Test
	${INPUTDATA}/ROISpot5.png
	${TEMP}/LaplacianRecursiveGaussianImageFilteroutput5.mha
        5
	${TEMP}/LaplacianRecursiveGaussianImageFilterOutput5.png	
)

# ------- LaplacianRecursiveGaussianImageFilter2Test----------

ADD_TEST(LaplacianRecursiveGaussianImageFilter2Test ${EXE_TESTS}
	--compare-n-images ${TOL} 2
	${BASELINE}/LaplacianRecursiveGaussianImageFilter2output3.mha
	${TEMP}/LaplacianRecursiveGaussianImageFilter2output3.mha
	${BASELINE}/LaplacianRecursiveGaussianImageFilter2Output3.png
	${TEMP}/LaplacianRecursiveGaussianImageFilter2Output3.png
	LaplacianRecursiveGaussianImageFilter2Test
	${INPUTDATA}/ROISpot5.png
	${TEMP}/LaplacianRecursiveGaussianImageFilter2output3.mha
        3
	${TEMP}/LaplacianRecursiveGaussianImageFilter2Output3.png
)

# ------- LaplacianRecursiveGaussianImageFilter2Test----------

ADD_TEST(LaplacianRecursiveGaussianImageFilter2Test ${EXE_TESTS}
	--compare-n-images ${TOL} 2
	${BASELINE}/LaplacianRecursiveGaussianImageFilter2output5.mha
	${TEMP}/LaplacianRecursiveGaussianImageFilter2output5.mha
	${BASELINE}/LaplacianRecursiveGaussianImageFilter2Output5.png
	${TEMP}/LaplacianRecursiveGaussianImageFilter2Output5.png
	LaplacianRecursiveGaussianImageFilter2Test
	${INPUTDATA}/ROISpot5.png
	${TEMP}/LaplacianRecursiveGaussianImageFilter2output5.mha
        5
	${TEMP}/LaplacianRecursiveGaussianImageFilter2Output5.png
)

# ------- CannyEdgeDetectionImageFilterTest----------

ADD_TEST(CannyEdgeDetectionImageFilterTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/CannyEdgeDetectorImageFilterOutput.png
	${TEMP}/CannyEdgeDetectorImageFilterOutput.png
	CannyEdgeDetectionImageFilterTest
	${INPUTDATA}/ROISpot5.png
	${TEMP}/CannyEdgeDetectorImageFilterOutput.png
 2
)

# ------- DiscreteGaussianImageFilterTest----------

ADD_TEST(DiscreteGaussianImageFilterTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/DiscreteGaussianImageFilterOutput.png
	${TEMP}/DiscreteGaussianImageFilterOutput.png
	DiscreteGaussianImageFilterTest
	${INPUTDATA}/QB_Suburb.png
	${TEMP}/DiscreteGaussianImageFilterOutput.png
    4 9
)

# ------- MedianImageFilterTest----------

ADD_TEST(MedianImageFilterTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/MedianImageFilterOutput.png
	${TEMP}/MedianImageFilterOutput.png
	MedianImageFilterTest
	${INPUTDATA}/QB_Suburb.png
	${TEMP}/MedianImageFilterOutput.png
)

# ------- BinaryThresholdImageFilterTest----------

ADD_TEST(BinaryThresholdImageFilterTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/BinaryThresholdImageFilterOutput.png
	${TEMP}/BinaryThresholdImageFilterOutput.png
	BinaryThresholdImageFilterTest
	${INPUTDATA}/ROISpot5.png
	${TEMP}/BinaryThresholdImageFilterOutput.png
 150 180 0 255
)

# ------- DerivativeImageFilterTest----------

ADD_TEST(DerivativeImageFilterTest ${EXE_TESTS}
	--compare-n-images ${TOL} 2
	${BASELINE}/DerivativeImageFilterFloatOutput.mhd
	${TEMP}/DerivativeImageFilterFloatOutput.mhd
	${BASELINE}/DerivativeImageFilterOutput.png
	${TEMP}/DerivativeImageFilterOutput.png
	DerivativeImageFilterTest
	${INPUTDATA}/ROISpot5.png
	${TEMP}/DerivativeImageFilterFloatOutput.mhd
	${TEMP}/DerivativeImageFilterOutput.png
    1 0
)

# ------- RationalQuotientResampleImageFilterTest----------

ADD_TEST(RationalQuotientResampleImageFilterTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/RationalQuotientResampleImageFilterOutput.tif
	${TEMP}/RationalQuotientResampleImageFilterOutput.tif
	RationalQuotientResampleImageFilterTest
	${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	${TEMP}/RationalQuotientResampleImageFilterOutput.tif
	2 3
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbFilteringExamplesTests otbFilteringExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbFilteringExamplesTests gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_EXAMPLES_TESTING )
