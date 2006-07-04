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

#ADD_EXECUTABLE(GradientMagnitudeImageFilter GradientMagnitudeImageFilter.cxx )
#TARGET_LINK_LIBRARIES(GradientMagnitudeImageFilter ITKCommon ITKIO)

#ADD_EXECUTABLE(GradientMagnitudeRecursiveGaussianImageFilter GradientMagnitudeRecursiveGaussianImageFilter.cxx )
#TARGET_LINK_LIBRARIES(GradientMagnitudeRecursiveGaussianImageFilter ITKCommon ITKIO)

#ADD_EXECUTABLE(GradientRecursiveGaussianImageFilter GradientRecursiveGaussianImageFilter.cxx )
#TARGET_LINK_LIBRARIES(GradientRecursiveGaussianImageFilter ITKCommon ITKIO)

#ADD_EXECUTABLE(SecondDerivativeRecursiveGaussianImageFilter SecondDerivativeRecursiveGaussianImageFilter.cxx )
#TARGET_LINK_LIBRARIES(SecondDerivativeRecursiveGaussianImageFilter ITKCommon ITKIO)

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

#ADD_EXECUTABLE(DerivativeImageFilter DerivativeImageFilter.cxx )
#TARGET_LINK_LIBRARIES(DerivativeImageFilter ITKCommon ITKIO)

#ADD_EXECUTABLE(SigmoidImageFilter SigmoidImageFilter.cxx )
#TARGET_LINK_LIBRARIES(SigmoidImageFilter ITKCommon ITKIO)

#ADD_EXECUTABLE(MedianImageFilter MedianImageFilter.cxx )
#TARGET_LINK_LIBRARIES(MedianImageFilter ITKCommon ITKIO)

#ADD_EXECUTABLE(MeanImageFilter MeanImageFilter.cxx )
#TARGET_LINK_LIBRARIES(MeanImageFilter ITKCommon ITKIO)

#ADD_EXECUTABLE(MathematicalMorphologyBinaryFilters MathematicalMorphologyBinaryFilters.cxx )
#TARGET_LINK_LIBRARIES(MathematicalMorphologyBinaryFilters ITKCommon ITKIO)

#ADD_EXECUTABLE(MathematicalMorphologyGrayscaleFilters MathematicalMorphologyGrayscaleFilters.cxx )
#TARGET_LINK_LIBRARIES(MathematicalMorphologyGrayscaleFilters ITKCommon ITKIO)

#ADD_EXECUTABLE(MorphologicalImageEnhancement MorphologicalImageEnhancement.cxx )
#TARGET_LINK_LIBRARIES(MorphologicalImageEnhancement ITKCommon ITKIO)

#ADD_EXECUTABLE(CurvatureFlowImageFilter CurvatureFlowImageFilter.cxx )
#TARGET_LINK_LIBRARIES(CurvatureFlowImageFilter ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(MinMaxCurvatureFlowImageFilter MinMaxCurvatureFlowImageFilter.cxx )
#TARGET_LINK_LIBRARIES(MinMaxCurvatureFlowImageFilter ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(BinaryMinMaxCurvatureFlowImageFilter BinaryMinMaxCurvatureFlowImageFilter.cxx )
#TARGET_LINK_LIBRARIES(BinaryMinMaxCurvatureFlowImageFilter ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(DiscreteGaussianImageFilter DiscreteGaussianImageFilter.cxx )
#TARGET_LINK_LIBRARIES(DiscreteGaussianImageFilter ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(GaussianBlurImageFunction GaussianBlurImageFunction.cxx )
#TARGET_LINK_LIBRARIES(GaussianBlurImageFunction ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(SmoothingRecursiveGaussianImageFilter SmoothingRecursiveGaussianImageFilter.cxx )
#TARGET_LINK_LIBRARIES(SmoothingRecursiveGaussianImageFilter ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(SmoothingRecursiveGaussianImageFilter2 SmoothingRecursiveGaussianImageFilter2.cxx )
#TARGET_LINK_LIBRARIES(SmoothingRecursiveGaussianImageFilter2 ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(LaplacianRecursiveGaussianImageFilter1 LaplacianRecursiveGaussianImageFilter1.cxx )
#TARGET_LINK_LIBRARIES(LaplacianRecursiveGaussianImageFilter1 ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(LaplacianRecursiveGaussianImageFilter2 LaplacianRecursiveGaussianImageFilter2.cxx )
#TARGET_LINK_LIBRARIES(LaplacianRecursiveGaussianImageFilter2 ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(ScaleSpaceGenerator2D ScaleSpaceGenerator2D.cxx )
#TARGET_LINK_LIBRARIES(ScaleSpaceGenerator2D ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(BinomialBlurImageFilter BinomialBlurImageFilter.cxx )
#TARGET_LINK_LIBRARIES(BinomialBlurImageFilter ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(CurvatureAnisotropicDiffusionImageFilter CurvatureAnisotropicDiffusionImageFilter.cxx )
#TARGET_LINK_LIBRARIES(CurvatureAnisotropicDiffusionImageFilter ITKCommon ITKBasicFilters ITKIO)

#ADD_EXECUTABLE(GradientAnisotropicDiffusionImageFilter GradientAnisotropicDiffusionImageFilter.cxx )
#TARGET_LINK_LIBRARIES(GradientAnisotropicDiffusionImageFilter ITKCommon ITKBasicFilters ITKIO)

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

#ADD_EXECUTABLE(DanielssonDistanceMapImageFilter DanielssonDistanceMapImageFilter.cxx )
#TARGET_LINK_LIBRARIES(DanielssonDistanceMapImageFilter ITKCommon ITKBasicFilters ITKIO)

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

ENDIF( NOT ITK_DISABLE_CXX_TESTING )

