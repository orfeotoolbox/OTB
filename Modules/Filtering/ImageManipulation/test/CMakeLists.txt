otb_module_test()

set(OTBImageManipulationTests
otbImageManipulationTestDriver.cxx
otbBinaryImageDensityFunctionNew.cxx
otbPrintableImageFilter.cxx
otbConcatenateScalarValueImageFilterTest.cxx
otbShiftScaleImageAdaptor.cxx
otbImageToVectorImageCastFilter.cxx
otbFunctionWithNeighborhoodToImageFilter.cxx
otbSqrtSpectralAngleImageFilter.cxx
otbUnaryFunctorNeighborhoodImageFilterNew.cxx
otbStreamingShrinkImageFilter.cxx
otbUnaryFunctorWithIndexImageFilterNew.cxx
otbUnaryFunctorImageFilterNew.cxx
otbUnaryImageFunctorWithVectorImageFilter.cxx
otbImageToVectorImageCastFilterNew.cxx
otbPrintableImageFilterWithMask.cxx
otbStreamingResampleImageFilter.cxx
otbUnaryFunctorNeighborhoodWithOffsetImageFilterNew.cxx
otbBoxAndWhiskerImageFilterNew.cxx
otbVectorImageToAmplitudeImageFilter.cxx
otbUnaryFunctorNeighborhoodWithOffsetImageFilter.cxx
otbStreamingResampleImageFilterCompareWithITK.cxx
otbRegionProjectionResampler.cxx
otbVectorImageTo3DScalarImageFilterNew.cxx
otbUnaryFunctorWithIndexImageFilter.cxx
otbMeanFunctorImageTest.cxx
otbBinaryImageToDensityImageFilterNew.cxx
otbVectorImageTo3DScalarImageFilter.cxx
otbTileImageFilter.cxx
otbMatrixImageFilterTest.cxx
otbMatrixTransposeMatrixImageFilter.cxx
otbPerBandVectorImageFilterNew.cxx
otbUnaryFunctorNeighborhoodImageFilter.cxx
otbStreamingResampleImageFilterNew.cxx
otbStreamingInnerProductVectorImageFilter.cxx
otbPhaseFunctorTest.cxx
otbShiftScaleVectorImageFilterNew.cxx
otbChangeLabelImageFilter.cxx
otbClampVectorImageFilter.cxx
otbPrintableImageFilterNew.cxx
otbShiftScaleImageAdaptorNew.cxx
otbStreamingInnerProductVectorImageFilterNew.cxx
otbBoxAndWhiskerImageFilter.cxx
otbStreamingShrinkImageFilterNew.cxx
otbVectorRescaleIntensityImageFilter.cxx
otbLog10ThresholdedImageFilterTest.cxx
otbChangeLabelImageFilterNew.cxx
otbMatrixTransposeMatrixImageFilterNew.cxx
otbExtractROIResample.cxx
otbLocalGradientVectorImageFilter.cxx
otbBinaryImageMinimalBoundingRegionCalculator.cxx
otbPerBandVectorImageFilterWithSobelFilter.cxx
otbBinaryImageDensityFunction.cxx
otbVectorImageToAmplitudeImageFilterNew.cxx
otbThresholdVectorImageFilter.cxx
otbPerBandVectorImageFilterWithMeanFilter.cxx
otbConcatenateVectorImageFilterNew.cxx
otbAmplitudeFunctorTest.cxx
otbMultiplyByScalarImageTest.cxx
otbClampImageFilter.cxx
otbConcatenateVectorImageFilter.cxx
otbBinaryImageMinimalBoundingRegionCalculatorNew.cxx
otbVectorRescaleIntensityImageFilterNew.cxx
otbSpectralAngleDistanceImageFilterNew.cxx
otbUnaryImageFunctorWithVectorImageFilterNew.cxx
otbBinaryImageToDensityImageFilter.cxx
otbSpectralAngleDistanceImageFilter.cxx
otbFunctionWithNeighborhoodToImageFilterNew.cxx
otbEuclideanDistanceMetricWithMissingValue.cxx
otbEuclideanDistanceMetricWithMissingValueNew.cxx
)

add_executable(otbImageManipulationTestDriver ${OTBImageManipulationTests})
target_link_libraries(otbImageManipulationTestDriver ${OTBImageManipulation-Test_LIBRARIES})
otb_module_target_label(otbImageManipulationTestDriver)

# Tests Declaration




otb_add_test(NAME bfTuBinaryImageDensityFunctionNew COMMAND  otbImageManipulationTestDriver
  otbBinaryImageDensityFunctionNew
  )

otb_add_test(NAME bfTvPrintableImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvPrintableImageFilter.png
  ${TEMP}/bfTvPrintableImageFilter.png
  otbPrintableImageFilter
  ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
  ${TEMP}/bfTvPrintableImageFilter.png
  )

otb_add_test(NAME bfTvConcatenateScalarValueImageFilterTest COMMAND otbImageManipulationTestDriver
  otbConcatenateScalarValueImageFilterTest )

otb_add_test(NAME bfTuConcatenateScalarValueImageFilterNew COMMAND otbImageManipulationTestDriver
  otbConcatenateScalarValueImageFilterNew )


otb_add_test(NAME coTvImageToVectorImageCastFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coImageToVectorImageCastFilterOutput.tif
  ${TEMP}/coImageToVectorImageCastFilterOutput.tif
  otbImageToVectorImageCastFilter
  ${INPUTDATA}/poupees.hdr
  ${TEMP}/coImageToVectorImageCastFilterOutput.tif
  )

otb_add_test(NAME bfTvFunctionWithNeighborhoodToImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvFunctionWithNeighborhoodToImageFilterTest.tif
  ${TEMP}/bfTvFunctionWithNeighborhoodToImageFilterTest.tif
  otbFunctionWithNeighborhoodToImageFilter
  ${INPUTDATA}/poupees_1canal.hd
  ${TEMP}/bfTvFunctionWithNeighborhoodToImageFilterTest.tif
  2 # radius
  )

otb_add_test(NAME feTvSqrtSpectralAngleImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_8}   ${BASELINE}/feSqrtSpectralAngle.tif
  ${TEMP}/feSqrtSpectralAngle.tif
  otbSqrtSpectralAngleImageFilter
  ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
  ${TEMP}/feSqrtSpectralAngle.tif
  185 270 150 619 #reference pixel
  )

otb_add_test(NAME coTuUnaryFunctorNeighborhoodImageFilterNew COMMAND otbImageManipulationTestDriver
  otbUnaryFunctorNeighborhoodImageFilterNew
  )

otb_add_test(NAME bfTvStreamingShrinkImageFilterQBPAN COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvStreamingShrinkImageFilterQBPANOutput.hdr
  ${TEMP}/bfTvStreamingShrinkImageFilterQBPANOutput.hdr
  otbStreamingShrinkImageFilter
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  ${TEMP}/bfTvStreamingShrinkImageFilterQBPANOutput.hdr
  50
  )

otb_add_test(NAME bfTvStreamingShrinkImageFilterQBMUL COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvStreamingShrinkImageFilterQBMULOutput.hdr
  ${TEMP}/bfTvStreamingShrinkImageFilterQBMULOutput.hdr
  otbStreamingShrinkImageFilter
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
  ${TEMP}/bfTvStreamingShrinkImageFilterQBMULOutput.hdr
  20
  )

otb_add_test(NAME coTuUnaryFunctorWithIndexImageFilterNew COMMAND otbImageManipulationTestDriver
  otbUnaryFunctorWithIndexImageFilterNew
  )


otb_add_test(NAME bfTuUnaryFunctorImageFilterNew COMMAND otbImageManipulationTestDriver
  otbUnaryFunctorImageFilterNew)




otb_add_test(NAME bfTvUnaryImageFunctorWithVectorImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvUnaryImageFunctorWithVectorImageFilter.tif
  ${TEMP}/bfTvUnaryImageFunctorWithVectorImageFilter.tif
  otbUnaryImageFunctorWithVectorImageFilter
  ${INPUTDATA}/poupees_sub.png
  ${TEMP}/bfTvUnaryImageFunctorWithVectorImageFilter.tif
  )

otb_add_test(NAME coTuImageToVectorImageCastFilter COMMAND otbImageManipulationTestDriver
  otbImageToVectorImageCastFilterNew)

otb_add_test(NAME bfTvPrintableImageFilterWithMask COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvPrintableImageFilterWithMask.png
  ${TEMP}/bfTvPrintableImageFilterWithMask.png
  otbPrintableImageFilterWithMask
  ${INPUTDATA}/poupees.tif
  ${INPUTDATA}/carre.png
  ${TEMP}/bfTvPrintableImageFilterWithMask.png
  0 # background
  )

otb_add_test(NAME bfTvStreamingResampleImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvStreamingResamplePoupeesTest.tif
  ${TEMP}/bfTvStreamingResamplePoupeesTest.tif
  otbStreamingResampleImageFilter
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfTvStreamingResamplePoupeesTest.tif
  )

otb_add_test(NAME coTuUnaryFunctorNeighborhoodWithOffsetImageFilterNew COMMAND otbImageManipulationTestDriver
  otbUnaryFunctorNeighborhoodWithOffsetImageFilterNew
  )

otb_add_test(NAME bfTuBoxAndWhiskerImageFilterNew COMMAND otbImageManipulationTestDriver
  otbBoxAndWhiskerImageFilterNew )

otb_add_test(NAME bfTvVectorImageToAmplitudeImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvVectorImageToAmplitudeImageFilterOutput.tif
  ${TEMP}/bfTvVectorImageToAmplitudeImageFilterOutput.tif
  otbVectorImageToAmplitudeImageFilter
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfTvVectorImageToAmplitudeImageFilterOutput.tif
  )

otb_add_test(NAME coTvUnaryFunctorNeighborhoodWithOffsetImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${INPUTDATA}/poupees.tif
  ${TEMP}/coTvUnaryFunctorNeighborhoodWithOffsetImageFilter.tif
  otbUnaryFunctorNeighborhoodWithOffsetImageFilter
  ${INPUTDATA}/poupees.tif
  ${TEMP}/coTvUnaryFunctorNeighborhoodWithOffsetImageFilter.tif
  2 # radius
  2 # offset[0]
  2  #offset[1]
  )

otb_add_test(NAME bfTvStreamingResampleImageFilterCompareWithITK COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${TEMP}/bfTvStreamingResamplePoupeesTestITK.tif
  ${TEMP}/bfTvStreamingResamplePoupeesTestOTB.tif
  otbStreamingResampleImageFilterCompareWithITK
  ${INPUTDATA}/poupees.tif
  3000 3000
  ${TEMP}/bfTvStreamingResamplePoupeesTestITK.tif
  ${TEMP}/bfTvStreamingResamplePoupeesTestOTB.tif
  )

otb_add_test(NAME prTvRegionProjectionResamplerToulouse COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_4}  ${BASELINE}/prTvRegionProjectionResamplerToulouse.tif
  ${TEMP}/prTvRegionProjectionResamplerToulouse.tif
  otbRegionProjectionResampler
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  ${TEMP}/prTvRegionProjectionResamplerToulouse.tif
  1.44 43.6050 # Long/Lat of Toulouse, Capitole
  500
  500
  100
  0.00001
  -0.00001
  )

otb_add_test(NAME prTvRegionProjectionResamplerCevennes COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_4}  ${BASELINE}/prTvRegionProjectionResamplerCevennes.tif
  ${TEMP}/prTvRegionProjectionResamplerCevennes.tif
  otbRegionProjectionResampler
  LARGEINPUT{QUICKBIRD/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF}
  ${TEMP}/prTvRegionProjectionResamplerCevennes.tif
  3.7 44.08
  500
  500
  10
  0.00001
  -0.00001
  )

otb_add_test(NAME bfTuVectorImageTo3DScalarImageFilterNew COMMAND otbImageManipulationTestDriver
  otbVectorImageTo3DScalarImageFilterNew)

otb_add_test(NAME coTvUnaryFunctorWithIndexImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${INPUTDATA}/poupees.tif
  ${TEMP}/coTvUnaryFunctorWithIndexImageFilter.tif
  otbUnaryFunctorWithIndexImageFilter
  ${INPUTDATA}/poupees.tif
  ${TEMP}/coTvUnaryFunctorWithIndexImageFilter.tif
  )

otb_add_test(NAME bfTvMeanFunctorImageTest COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/bfMeanFunctorImage.tif
  ${TEMP}/bfMeanFunctorImage.tif
  otbMeanFunctorImageTest
  ${INPUTDATA}/couleurs_extrait.png
  ${TEMP}/bfMeanFunctorImage.tif
  )



otb_add_test(NAME bfTuBinaryImageToImageDensityFilterNew COMMAND  otbImageManipulationTestDriver
  otbBinaryImageToDensityImageFilterNew
  )

otb_add_test(NAME bfTvVectorImageTo3DScalarImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvVectorImageTo3DScalarImageFilterOutput.png
  ${TEMP}/bfTvVectorImageTo3DScalarImageFilterOutput.png
  otbVectorImageTo3DScalarImageFilter
  ${INPUTDATA}/poupees.png
  ${TEMP}/bfTvVectorImageTo3DScalarImageFilterOutput.png
  )

otb_add_test(NAME bfTvTileImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvTileImageFilterOutput.tif
  ${TEMP}/bfTvTileImageFilterOutput.tif
  otbTileImageFilter
  2 2
  ${TEMP}/bfTvTileImageFilterOutput.tif
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  )

otb_add_test(NAME bfTuTileImageFilterNew COMMAND otbImageManipulationTestDriver
  otbTileImageFilterNew
  )

otb_add_test(NAME bfTuMatrixImageFilterNew COMMAND otbImageManipulationTestDriver
  otbMatrixImageFilterNew
  )

otb_add_test(NAME bfTvMatrixImageFilterTest_1 COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvMatrixImageFilterTest_1.tif
  ${TEMP}/bfTvMatrixImageFilterTest_1.tif
  otbMatrixImageFilterTest
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfTvMatrixImageFilterTest_1.tif
  1
  )

otb_add_test(NAME bfTvMatrixImageFilterTest_2 COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvMatrixImageFilterTest_2.tif
  ${TEMP}/bfTvMatrixImageFilterTest_2.tif
  otbMatrixImageFilterTest
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfTvMatrixImageFilterTest_2.tif
  2
  )

otb_add_test(NAME bfTvMatrixImageFilterTest_3 COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvMatrixImageFilterTest_3.tif
  ${TEMP}/bfTvMatrixImageFilterTest_3.tif
  otbMatrixImageFilterTest
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfTvMatrixImageFilterTest_3.tif
  3
  )

otb_add_test(NAME bfTvMatrixTransposeMatrixImageFilter COMMAND otbImageManipulationTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvMatrixTransposeMatrixImageFilterResults.txt
  ${TEMP}/bfTvMatrixTransposeMatrixImageFilterResults.txt
  otbMatrixTransposeMatrixImageFilter
  ${INPUTDATA}/multiSpect.tif
  ${INPUTDATA}/multiSpectInterp.tif
  ${TEMP}/bfTvMatrixTransposeMatrixImageFilterResults.txt
  )

otb_add_test(NAME bfTuPerBandVectorImageFilterNew COMMAND otbImageManipulationTestDriver
  otbPerBandVectorImageFilterNew)


otb_add_test(NAME coTvUnaryFunctorNeighborhoodImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coTvUnaryFunctorNeighborhoodImageFilter.png
  ${TEMP}/coTvUnaryFunctorNeighborhoodImageFilter.png
  otbUnaryFunctorNeighborhoodImageFilter
  ${INPUTDATA}/amst.png
  ${TEMP}/coTvUnaryFunctorNeighborhoodImageFilter.png
  3
  )

otb_add_test(NAME bfTuStreamingResampleImageFilterNew COMMAND otbImageManipulationTestDriver
  otbStreamingResampleImageFilterNew)

otb_add_test(NAME bfTvStreamingInnerProductVectorImageFilterDisableCenterData COMMAND otbImageManipulationTestDriver
  --compare-ascii 0.000001
  ${BASELINE_FILES}/bfStreamingInnerProductVectorImageFilterResultsDisableCenterData.txt
  ${TEMP}/bfStreamingInnerProductVectorImageFilterResultsDisableCenterData.txt
  otbStreamingInnerProductVectorImageFilter
  ${INPUTDATA}/poupees_sub_3c.png
  ${TEMP}/bfStreamingInnerProductVectorImageFilterResultsDisableCenterData.txt
  0 #center data
  )

otb_add_test(NAME bfTvStreamingInnerProductVectorImageFilter COMMAND otbImageManipulationTestDriver
  --compare-ascii 0.000001
  ${BASELINE_FILES}/bfStreamingInnerProductVectorImageFilterResults.txt
  ${TEMP}/bfStreamingInnerProductVectorImageFilterResults.txt
  otbStreamingInnerProductVectorImageFilter
  ${INPUTDATA}/poupees_sub_3c.png
  ${TEMP}/bfStreamingInnerProductVectorImageFilterResults.txt
  1 #center data
  )

otb_add_test(NAME bfTvPhaseFunctorTest COMMAND otbImageManipulationTestDriver
  otbPhaseFunctorTest
  )

otb_add_test(NAME bfTuShiftScaleVectorImageFilterNew COMMAND otbImageManipulationTestDriver
  otbShiftScaleVectorImageFilterNew)

otb_add_test(NAME bfTvChangeLabelImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfChangeLabelImageFilter.tif
  ${TEMP}/bfChangeLabelImageFilter.tif
  otbChangeLabelImageFilter
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/bfChangeLabelImageFilter.tif
  150   # lower threshold
  255   # upper  threshold
  )


otb_add_test(NAME bfTvClampVectorImageFilterTest COMMAND otbImageManipulationTestDriver
  --compare-image 0.0001
  ${BASELINE}/bfTvClampVectorImageFilterTest_Output.tif
  ${TEMP}/bfTvClampVectorImageFilterTest_Output.tif
  otbClampVectorImageFilterTest
  ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
  ${TEMP}/bfTvClampVectorImageFilterTest_Output.tif
  )

otb_add_test(NAME bfTuClampVectorImageFilterNew COMMAND otbImageManipulationTestDriver
  otbClampVectorImageFilterNew
  )

otb_add_test(NAME bfTuPrintableImageFilterNew COMMAND otbImageManipulationTestDriver
  otbPrintableImageFilterNew)

otb_add_test(NAME coTuShiftScaleImageAdaptorNew COMMAND otbImageManipulationTestDriver
  otbShiftScaleImageAdaptorNew
  )

otb_add_test(NAME bfTuStreamingInnerProductVectorImageFilterNew COMMAND otbImageManipulationTestDriver
  otbStreamingInnerProductVectorImageFilterNew
  )












otb_add_test(NAME bfTvBoxAndWhiskerImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfBoxAndWhiskerImageFilter_05_05.tif
  ${TEMP}/bfBoxAndWhiskerImageFilter_05_05.tif
  otbBoxAndWhiskerImageFilter
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfBoxAndWhiskerImageFilter_05_05.tif
  05 05)

otb_add_test(NAME bfTuStreamingShrinkImageFilterNew COMMAND otbImageManipulationTestDriver
  otbStreamingShrinkImageFilterNew)

otb_add_test(NAME bfTvVectorRescaleIntensityImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvVectorRescaleIntensityOutput.png
  ${TEMP}/bfTvVectorRescaleIntensityOutput.png
  otbVectorRescaleIntensityImageFilter
  ${INPUTDATA}/qb_RoadExtract.img.hdr
  ${TEMP}/bfTvVectorRescaleIntensityOutput.png
  0 255
  )

otb_add_test(NAME bfTvotbLog10ThresholdedImageFilterTest COMMAND otbImageManipulationTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvLog10ThresholdedImageFilter.txt
  ${TEMP}/bfTvLog10ThresholdedImageFilterTest.txt
  otbLog10ThresholdedImageFilterTest
  ${TEMP}/bfTvLog10ThresholdedImageFilterTest.txt
  )

otb_add_test(NAME bfTuChangeLabelImageFilterNew COMMAND otbImageManipulationTestDriver
  otbChangeLabelImageFilterNew
  )




otb_add_test(NAME bfTuMatrixTransposeMatrixImageFilterNew COMMAND otbImageManipulationTestDriver
  otbMatrixTransposeMatrixImageFilterNew)

otb_add_test(NAME bfTvExtractROIResample2 COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvExtractROIResample2.tif
  ${TEMP}/bfTvExtractROIResample2.tif
  otbExtractROIResample
  ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/bfTvExtractROIResample2.tif
  1
  )

otb_add_test(NAME bfTvExtractROIResample COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvExtractROIResample.tif
  ${TEMP}/bfTvExtractROIResample.tif
  otbExtractROIResample
  ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/bfTvExtractROIResample.tif
  0
  )

otb_add_test(NAME bfTvLocalGradientVectorImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvLocalGradientVectorImageFilter.tif
  ${TEMP}/bfTvLocalGradientVectorImageFilter.tif
  otbLocalGradientVectorImageFilterTest
  -in ${INPUTDATA}/cupriteSubHsi.tif
  -out ${TEMP}/bfTvLocalGradientVectorImageFilter.tif)

otb_add_test(NAME bfTuLocalGradientVectorImageFilterNew COMMAND otbImageManipulationTestDriver
  otbLocalGradientVectorImageFilterNewTest)

otb_add_test(NAME bfTvBoundingRegionCalculator COMMAND otbImageManipulationTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfBoundingRegionCalculatorOutput.txt
  ${TEMP}/bfBoundingRegionCalculatorOutput.txt
  otbBinaryImageMinimalBoundingRegionCalculator
  4
  ${TEMP}/bfBoundingRegionCalculatorOutput.txt
  ${INPUTDATA}/rcc8_mire1.png
  ${INPUTDATA}/rcc8_mire2.png
  ${INPUTDATA}/rcc8_mire3.png
  ${INPUTDATA}/rcc8_mire4.png
  )

otb_add_test(NAME bfTvPerBandVectorImageFilterWithSobelFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvPerBandVectorImageFilterWithSobelFilterOutput.hdr
  ${TEMP}/bfTvPerBandVectorImageFilterWithSobelFilterOutput.hdr
  otbPerBandVectorImageFilterWithSobelFilter
  ${INPUTDATA}/poupees.png
  ${TEMP}/bfTvPerBandVectorImageFilterWithSobelFilterOutput.hdr
  )


otb_add_test(NAME bfTvBinaryImageDensityFunction COMMAND otbImageManipulationTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvBinaryImageDensityFunctionOutputAscii.txt
  ${TEMP}/bfTvBinaryImageDensityFunctionOutputAscii.txt
  otbBinaryImageDensityFunction
  ${INPUTDATA}/scene.png
  ${TEMP}/bfTvBinaryImageDensityFunctionOutputAscii.txt
  )

otb_add_test(NAME bfTuVectorImageToAmplitudeImageFilterNew COMMAND otbImageManipulationTestDriver
  otbVectorImageToAmplitudeImageFilterNew)

otb_add_test(NAME bfTvThresholdVectorImageFilterTest COMMAND otbImageManipulationTestDriver
  --compare-image 0.0001
  ${BASELINE}/bfTvThresholdVectorImageFilterTest_Output.tif
  ${TEMP}/bfTvThresholdVectorImageFilterTest_Output.tif
  otbThresholdVectorImageFilterTest
  ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
  ${TEMP}/bfTvThresholdVectorImageFilterTest_Output.tif
  )

otb_add_test(NAME bfTuThresholdVectorImageFilterNew COMMAND otbImageManipulationTestDriver
  otbThresholdVectorImageFilterNew
  )

otb_add_test(NAME bfTvPerBandVectorImageFilterWithMeanFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvPerBandVectorImageFilterWithMeanFilterOutput.hdr
  ${TEMP}/bfTvPerBandVectorImageFilterWithMeanFilterOutput.hdr
  otbPerBandVectorImageFilterWithMeanFilter
  ${INPUTDATA}/poupees.png
  ${TEMP}/bfTvPerBandVectorImageFilterWithMeanFilterOutput.hdr
  )

otb_add_test(NAME coTuConcatenateVectorImageFilterNew COMMAND otbImageManipulationTestDriver
  otbConcatenateVectorImageFilterNew)

otb_add_test(NAME bfTvAmplitudeFunctorTest COMMAND otbImageManipulationTestDriver
  otbAmplitudeFunctorTest
  )

otb_add_test(NAME feTvMultiplyByScalarImage COMMAND otbImageManipulationTestDriver
  otbMultiplyByScalarImageFilterTest)

otb_add_test(NAME bfTuClampImageFilterNew COMMAND otbImageManipulationTestDriver
  otbClampImageFilterNew
  )

otb_add_test(NAME bfTvClampImageFilterTest COMMAND otbImageManipulationTestDriver
  --compare-image 0.0001
  ${BASELINE}/bfTvClampImageFilterTest_Output.tif
  ${TEMP}/bfTvClampImageFilterTest_Output.tif
  otbClampImageFilterTest
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/bfTvClampImageFilterTest_Output.tif
  )

otb_add_test(NAME coTvConcatenateVectorImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coConcatenateVectorImageFilterOutput1.hdr
  ${TEMP}/coConcatenateVectorImageFilterOutput1.hdr
  otbConcatenateVectorImageFilter
  ${INPUTDATA}/poupees.png
  ${INPUTDATA}/poupees.png
  ${TEMP}/coConcatenateVectorImageFilterOutput1.hdr
  )

otb_add_test(NAME bfTuBoundingRegionCalculatorNew COMMAND otbImageManipulationTestDriver
  otbBinaryImageMinimalBoundingRegionCalculatorNew)

otb_add_test(NAME bfTuVectorRescaleIntensityImageFilterNew COMMAND otbImageManipulationTestDriver
  otbVectorRescaleIntensityImageFilterNew)

otb_add_test(NAME bfTuSpectralAngleDistanceImageFilterNew COMMAND otbImageManipulationTestDriver
  otbSpectralAngleDistanceImageFilterNew)

otb_add_test(NAME bfTuUnaryImageFunctorWithVectorImageFilterNew COMMAND otbImageManipulationTestDriver
  otbUnaryImageFunctorWithVectorImageFilterNew)

otb_add_test(NAME bfTvBinaryImageToDensityImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvBinaryImageToDensityImageFilterOutputImage.tif
  ${TEMP}/bfTvBinaryImageToDensityImageFilterOutputImage.tif
  otbBinaryImageToDensityImageFilter
  ${INPUTDATA}/scene.png
  ${TEMP}/bfTvBinaryImageToDensityImageFilterOutputImage.tif
  2 # radius
  )


otb_add_test(NAME bfTvSpectralAngleDistanceImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvSpectralAngleDistanceOutput.hdr
  ${TEMP}/bfTvSpectralAngleDistanceOutput.hdr
  otbSpectralAngleDistanceImageFilter
  ${INPUTDATA}/qb_RoadExtract.img.hdr
  ${TEMP}/bfTvSpectralAngleDistanceOutput.hdr
  337 557 432 859
  )

otb_add_test(NAME bfTvSpectralAngleDistanceImageFilterOneChannel COMMAND otbImageManipulationTestDriver
  otbSpectralAngleDistanceImageFilter
  ${INPUTDATA}/DeuxCercles.hdr
  ${TEMP}/bfTvSpectralAngleDistanceOneChannelOutput.hdr
  100
  )
set_property(TEST bfTvSpectralAngleDistanceImageFilterOneChannel PROPERTY WILL_FAIL true)

otb_add_test(NAME bfTvFunctionWithNeighborhoodToImageFilterNew COMMAND otbImageManipulationTestDriver
  otbFunctionWithNeighborhoodToImageFilterNew
  )

otb_add_test(NAME bfTvEuclideanDistanceMetricWithMissingValue COMMAND otbImageBaseTestDriver
  otbEuclideanDistanceMetricWithMissingValue)
otb_add_test(NAME bfTuEuclideanDistanceMetricWithMissingValueNew COMMAND otbImageBaseTestDriver
  otbEuclideanDistanceMetricWithMissingValueNew)
