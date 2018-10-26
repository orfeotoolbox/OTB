#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

otb_module_test()

set(OTBImageManipulationTests
otbImageManipulationTestDriver.cxx
otbPrintableImageFilter.cxx
otbConcatenateScalarValueImageFilterTest.cxx
otbShiftScaleImageAdaptor.cxx
otbImageToVectorImageCastFilter.cxx
otbFunctionWithNeighborhoodToImageFilter.cxx
otbSqrtSpectralAngleImageFilter.cxx
otbStreamingShrinkImageFilter.cxx
otbUnaryImageFunctorWithVectorImageFilter.cxx
otbPrintableImageFilterWithMask.cxx
otbStreamingResampleImageFilter.cxx
otbVectorImageToAmplitudeImageFilter.cxx
otbUnaryFunctorNeighborhoodWithOffsetImageFilter.cxx
otbStreamingResampleImageFilterCompareWithITK.cxx
otbRegionProjectionResampler.cxx
otbUnaryFunctorWithIndexImageFilter.cxx
otbMeanFunctorImageTest.cxx
otbVectorImageTo3DScalarImageFilter.cxx
otbTileImageFilter.cxx
otbMatrixImageFilterTest.cxx
otbMatrixTransposeMatrixImageFilter.cxx
otbUnaryFunctorNeighborhoodImageFilter.cxx
otbStreamingInnerProductVectorImageFilter.cxx
otbPhaseFunctorTest.cxx
otbChangeLabelImageFilter.cxx
otbBoxAndWhiskerImageFilter.cxx
otbVectorRescaleIntensityImageFilter.cxx
otbLog10ThresholdedImageFilterTest.cxx
otbExtractROIResample.cxx
otbLocalGradientVectorImageFilter.cxx
otbBinaryImageMinimalBoundingRegionCalculator.cxx
otbPerBandVectorImageFilterWithSobelFilter.cxx
otbBinaryImageDensityFunction.cxx
otbThresholdVectorImageFilter.cxx
otbPerBandVectorImageFilterWithMeanFilter.cxx
otbAmplitudeFunctorTest.cxx
otbMultiplyByScalarImageTest.cxx
otbClampImageFilter.cxx
otbConcatenateVectorImageFilter.cxx
otbBinaryImageToDensityImageFilter.cxx
otbSpectralAngleDistanceImageFilter.cxx
otbEuclideanDistanceMetricWithMissingValue.cxx
otbNRIBandImagesToOneNComplexBandsImage.cxx
otbOneRIBandImageToOneComplexBandImage.cxx
otbTwoNRIBandsImageToNComplexBandsImage.cxx
otbChangeNoDataValueFilter.cxx
otbImageToNoDataMaskFilter.cxx
otbChangeInformationImageFilter.cxx
otbGridResampleImageFilter.cxx
otbMaskedIteratorDecorator.cxx
)

add_executable(otbImageManipulationTestDriver ${OTBImageManipulationTests})
target_link_libraries(otbImageManipulationTestDriver ${OTBImageManipulation-Test_LIBRARIES})
otb_module_target_label(otbImageManipulationTestDriver)

# Tests Declaration





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


otb_add_test(NAME bfTvStreamingShrinkImageFilterQBPAN COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvStreamingShrinkImageFilterQBPANOutput.tif
  ${TEMP}/bfTvStreamingShrinkImageFilterQBPANOutput.tif
  otbStreamingShrinkImageFilter
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  ${TEMP}/bfTvStreamingShrinkImageFilterQBPANOutput.tif
  50
  )

otb_add_test(NAME bfTvStreamingShrinkImageFilterQBMUL COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvStreamingShrinkImageFilterQBMULOutput.tif
  ${TEMP}/bfTvStreamingShrinkImageFilterQBMULOutput.tif
  otbStreamingShrinkImageFilter
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
  ${TEMP}/bfTvStreamingShrinkImageFilterQBMULOutput.tif
  20
  )






otb_add_test(NAME bfTvUnaryImageFunctorWithVectorImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvUnaryImageFunctorWithVectorImageFilter.tif
  ${TEMP}/bfTvUnaryImageFunctorWithVectorImageFilter.tif
  otbUnaryImageFunctorWithVectorImageFilter
  ${INPUTDATA}/poupees_sub.png
  ${TEMP}/bfTvUnaryImageFunctorWithVectorImageFilter.tif
  )

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


otb_add_test(NAME coTvUnaryFunctorNeighborhoodImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coTvUnaryFunctorNeighborhoodImageFilter.png
  ${TEMP}/coTvUnaryFunctorNeighborhoodImageFilter.png
  otbUnaryFunctorNeighborhoodImageFilter
  ${INPUTDATA}/amst.png
  ${TEMP}/coTvUnaryFunctorNeighborhoodImageFilter.png
  3
  )

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


otb_add_test(NAME coTvShiftScaleImageAdaptor COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE}/coTvShiftScaleImageAdaptor.tif    # copie the input image
  ${TEMP}/coTvShiftScaleImageAdaptor.tif
  otbShiftScaleImageAdaptor
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/coTvShiftScaleImageAdaptor.tif
  )













otb_add_test(NAME bfTvBoxAndWhiskerImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfBoxAndWhiskerImageFilter_05_05.tif
  ${TEMP}/bfBoxAndWhiskerImageFilter_05_05.tif
  otbBoxAndWhiskerImageFilter
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfBoxAndWhiskerImageFilter_05_05.tif
  05 05)

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
  ${BASELINE}/bfTvPerBandVectorImageFilterWithSobelFilterOutput.tif
  ${TEMP}/bfTvPerBandVectorImageFilterWithSobelFilterOutput.tif
  otbPerBandVectorImageFilterWithSobelFilter
  ${INPUTDATA}/poupees.png
  ${TEMP}/bfTvPerBandVectorImageFilterWithSobelFilterOutput.tif
  )


otb_add_test(NAME bfTvBinaryImageDensityFunction COMMAND otbImageManipulationTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvBinaryImageDensityFunctionOutputAscii.txt
  ${TEMP}/bfTvBinaryImageDensityFunctionOutputAscii.txt
  otbBinaryImageDensityFunction
  ${INPUTDATA}/scene.png
  ${TEMP}/bfTvBinaryImageDensityFunctionOutputAscii.txt
  )

otb_add_test(NAME bfTvThresholdVectorImageFilterTest COMMAND otbImageManipulationTestDriver
  --compare-image 0.0001
  ${BASELINE}/bfTvThresholdVectorImageFilterTest_Output.tif
  ${TEMP}/bfTvThresholdVectorImageFilterTest_Output.tif
  otbThresholdVectorImageFilterTest
  ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
  ${TEMP}/bfTvThresholdVectorImageFilterTest_Output.tif
  )


otb_add_test(NAME bfTvPerBandVectorImageFilterWithMeanFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvPerBandVectorImageFilterWithMeanFilterOutput.tif
  ${TEMP}/bfTvPerBandVectorImageFilterWithMeanFilterOutput.tif
  otbPerBandVectorImageFilterWithMeanFilter
  ${INPUTDATA}/poupees.png
  ${TEMP}/bfTvPerBandVectorImageFilterWithMeanFilterOutput.tif
  )

otb_add_test(NAME bfTvAmplitudeFunctorTest COMMAND otbImageManipulationTestDriver
  otbAmplitudeFunctorTest
  )

otb_add_test(NAME feTvMultiplyByScalarImage COMMAND otbImageManipulationTestDriver
  otbMultiplyByScalarImageFilterTest)


otb_add_test(NAME bfTvClampImageFilterTest COMMAND otbImageManipulationTestDriver
  --compare-image 0.0001
  ${BASELINE}/bfTvClampImageFilterTest_Output.tif
  ${TEMP}/bfTvClampImageFilterTest_Output.tif
  otbClampImageFilterTest
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/bfTvClampImageFilterTest_Output.tif
  )

otb_add_test(NAME bfTvClampImageFilterConversionTest COMMAND otbImageManipulationTestDriver
  otbClampImageFilterConversionTest
  ${INPUTDATA}/veryverySmallFSATSW.tif
  )

otb_add_test(NAME coTvConcatenateVectorImageFilter COMMAND otbImageManipulationTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coConcatenateVectorImageFilterOutput1.tif
  ${TEMP}/coConcatenateVectorImageFilterOutput1.tif
  otbConcatenateVectorImageFilter
  ${INPUTDATA}/poupees.png
  ${INPUTDATA}/poupees.png
  ${TEMP}/coConcatenateVectorImageFilterOutput1.tif
  )

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
  ${BASELINE}/bfTvSpectralAngleDistanceOutput.tif
  ${TEMP}/bfTvSpectralAngleDistanceOutput.tif
  otbSpectralAngleDistanceImageFilter
  ${INPUTDATA}/qb_RoadExtract.img.hdr
  ${TEMP}/bfTvSpectralAngleDistanceOutput.tif
  337 557 432 859
  )

otb_add_test(NAME bfTvSpectralAngleDistanceImageFilterOneChannel COMMAND otbImageManipulationTestDriver
  otbSpectralAngleDistanceImageFilter
  ${INPUTDATA}/DeuxCercles.hdr
  ${TEMP}/bfTvSpectralAngleDistanceOneChannelOutput.tif
  100
  )
set_property(TEST bfTvSpectralAngleDistanceImageFilterOneChannel PROPERTY WILL_FAIL true)


otb_add_test(NAME bfTvEuclideanDistanceMetricWithMissingValue COMMAND otbImageManipulationTestDriver
  otbEuclideanDistanceMetricWithMissingValue)
otb_add_test(NAME bfTvTwoNRIBandsImageToNComplexBandsImage COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvNRIBandImagesToOneNComplexBandsImage.tif
  ${TEMP}/bfTvTwoNRIBandsImageToNComplexBandsImage.tif
  otbTwoNRIBandsImageToNComplexBandsImage
  ${INPUTDATA}/RSAT_imagery_HH_HV_VV.tif
  ${TEMP}/bfTvTwoNRIBandsImageToNComplexBandsImage.tif
  )


otb_add_test(NAME bfTvNRIBandImagesToOneNComplexBandsImage COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvNRIBandImagesToOneNComplexBandsImage.tif
  ${TEMP}/bfTvNRIBandImagesToOneNComplexBandsImage.tif
  otbNRIBandImagesToOneNComplexBandsImage
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${INPUTDATA}/RSAT_imagery_HV.tif
  ${INPUTDATA}/RSAT_imagery_VV.tif
  ${TEMP}/bfTvNRIBandImagesToOneNComplexBandsImage.tif
  )

otb_add_test(NAME bfTvOneRIBandImageToOneComplexBandImage COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_3}
  ${INPUTDATA}/RSAT_imageryC_HH.tif
  ${TEMP}/bfTvOneRIBandImageToOneComplexBandImage.tif
  otbOneRIBandImageToOneComplexBandImage
  ${INPUTDATA}/RSAT_imagery_HH.tif
  ${TEMP}/bfTvOneRIBandImageToOneComplexBandImage.tif
  )

otb_add_test(NAME filteringImageManipulationChangeNoDataValueFilter COMMAND otbImageManipulationTestDriver
  otbChangeNoDataValueFilter)

otb_add_test(NAME filteringImageManipulationImageToNoDataMaskFilter COMMAND otbImageManipulationTestDriver
  otbImageToNoDataMaskFilter)

otb_add_test(NAME bfTvChangeInformationImageFilter COMMAND  otbImageManipulationTestDriver
  otbChangeInformationImageFilter
  ${INPUTDATA}/WV2_PAN_ROI_1000_100.tif
  )

otb_add_test(NAME    otbGridResampleImageFilter
             COMMAND otbImageManipulationTestDriver otbGridResampleImageFilter)

otb_add_test(NAME bfTvMaskedIteratorDecoratorNominal COMMAND otbImageManipulationTestDriver
  otbMaskedIteratorDecoratorNominal
)

otb_add_test(NAME bfTvMaskedIteratorDecoratorDegenerate COMMAND otbImageManipulationTestDriver
  otbMaskedIteratorDecoratorDegenerate
)

otb_add_test(NAME bfTvMaskedIteratorDecoratorExtended COMMAND otbImageManipulationTestDriver
  otbMaskedIteratorDecoratorExtended
)
