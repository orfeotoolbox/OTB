#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
# Copyright (C) 2020 CS Systemes d'Information (CS SI)
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

set(OTBImageIOTests
otbImageIOTestDriver.cxx
otbImageFileWriterWithExtendedOptionBox.cxx
otbShortRGBImageIOTest.cxx
otbPipelineMetadataHandlingWithUFFilterTest.cxx
otbImageMetadataFileWriterTest.cxx
otbImageFileWriterTestWithoutInput.cxx
otbVectorImageStreamingFileWriterTestWithoutInput.cxx
otbReadingComplexDataIntoComplexImageTest.cxx
otbStreamingImageFileWriterTest.cxx
otbImageMetadataStreamingFileWriterTest.cxx
otbFloatImageIOTest.cxx
otbGDALDriverDoubleWritingTest.cxx
otbDoubleImageIOTest.cxx
otbStreamingImageFileWriterTestCalculateNumberOfDivisions.cxx
otbImageStreamingFileWriterTestWithoutInput.cxx
otbScalarBufferToImageFileWriterTest.cxx
otbPipelineMetadataHandlingTest.cxx
otbMultiResolutionReadingInfo.cxx
otbImageFileReaderTestFloat.cxx
otbComplexImageTests.cxx
otbShortImageIOTest.cxx
otbImageSeriesFileReader.cxx
otbStreamingShortImageFileWriterTest.cxx
otbImageFileReaderWithComplexPixel.cxx
otbVectorImageFileReaderWriterTest.cxx
otbIntImageIOTest.cxx
otbPNGIndexedNbBandsTest.cxx
otbImageFileReaderTest.cxx
otbVectorImageFileWriterTestWithoutInput.cxx
otbWritingComplexDataWithComplexImageTest.cxx
otbStreamingImageFileWriterWithFilterTest.cxx
otbPipeline.cxx
otbStreamingImageFilterTest.cxx
otbImageFileReaderRGBTest.cxx
otbImageFileWriterRGBTest.cxx
otbComplexImageManipulationTest.cxx
otbImageFileWriterTest.cxx
otbCompareWritingComplexImage.cxx
otbImageFileReaderOptBandTest.cxx
otbImageFileWriterOptBandTest.cxx
otbMultiImageFileWriterTest.cxx
)

add_executable(otbImageIOTestDriver ${OTBImageIOTests})
target_link_libraries(otbImageIOTestDriver ${OTBImageIO-Test_LIBRARIES})
otb_module_target_label(otbImageIOTestDriver)

# Tests Declaration

otb_add_test(NAME ioTvImageFileWriterExtendedFileName_Box COMMAND otbImageIOTestDriver
  otbImageFileWriterWithExtendedOptionBox
  ${INPUTDATA}/maur_rgb_24bpp.tif
  ${TEMP}/ioImageFileWriterExtendedFileName_box.tif
  2
  2
  200
  200
  10
  )

otb_add_test(NAME ioTvPipelineMetadataHandlingWithUFFilterTest COMMAND otbImageIOTestDriver
  --compare-metadata ${EPSILON_9}
  ${INPUTDATA}/HFAGeoreferenced.img
  ${TEMP}/ioTvPipelineMetadataHandlingWithUFFilterTest.img
  otbPipelineMetadataHandlingWithUFFilterTest
  ${INPUTDATA}/HFAGeoreferenced.img
  ${TEMP}/ioTvPipelineMetadataHandlingWithUFFilterTest.img
  )

otb_add_test(NAME ioTvImageMetadataWriterTest_HFA COMMAND otbImageIOTestDriver
  --compare-metadata ${EPSILON_9}
  ${INPUTDATA}/HFAGeoreferenced.img
  ${TEMP}/ioTvImageMetadataWriterHFA.img
  otbImageMetadataFileWriterTest
  ${INPUTDATA}/HFAGeoreferenced.img
  ${TEMP}/ioTvImageMetadataWriterHFA.img
  )

otb_add_test(NAME ioTvImageMetadataWriterTest_TIF COMMAND otbImageIOTestDriver
  --compare-metadata ${EPSILON_9}
  ${INPUTDATA}/spot5SubWithGcps.tif
  ${TEMP}/ioTvImageMetadataWriterTIF.tif
  otbImageMetadataFileWriterTest
  ${INPUTDATA}/spot5SubWithGcps.tif
  ${TEMP}/ioTvImageMetadataWriterTIF.tif
  )

otb_add_test(NAME ioTvImageFileReaderRGB_Gdal_SPOT5TIF2PNG COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesTIF.png
  ${TEMP}/ioImageFileReaderRGB_Gdal_SPOT5TIF2PNG_poupeesTIF.png
  otbImageFileReaderRGBTest
  ${INPUTDATA}/poupeesTIF/IMAGERY.TIF
  ${TEMP}/ioImageFileReaderRGB_Gdal_SPOT5TIF2PNG_poupeesTIF.png )

otb_add_test(NAME ioTvImageReaderWriterRgbPNG2ENVI COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees.png
  ${TEMP}/ioImageReaderWriterRgbPNG2ENVI_poupees
  otbImageFileReaderRGBTest
  ${INPUTDATA}/poupees.png
  ${TEMP}/ioImageReaderWriterRgbPNG2ENVI_poupees.hdr )

otb_add_test(NAME ioTvImageReaderWriterRgbPNG2PNG COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees.png
  ${TEMP}/ioImageReaderWriterRgbPNG2PNG_poupees.png
  otbImageFileReaderRGBTest
  ${INPUTDATA}/poupees.png
  ${TEMP}/ioImageReaderWriterRgbPNG2PNG_poupees.png )

otb_add_test(NAME ioTvImageReaderWriterRgbPNG2PNGBox COMMAND otbImageIOTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coTvExtractROI_QB.png
  ${TEMP}/coTvExtractROI_QB_Box.png
  otbImageFileReaderRGBTest
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/coTvExtractROI_QB_Box.png?&box=0:0:70:70
  )

otb_add_test(NAME ioTvImageReaderWriterRgbENVI2PNG COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees.raw
  ${TEMP}/ioImageReaderWriterRgbENVI2PNG_poupees.png
  otbImageFileReaderRGBTest
  ${INPUTDATA}/poupees.raw
  ${TEMP}/ioImageReaderWriterRgbENVI2PNG_poupees.png )

otb_add_test(NAME ioTvImageFileReaderRGB_Cai_SPOT5TIF2PNG COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesTIF.png
  ${TEMP}/ioImageFileReaderRGB_Cai_SPOT5TIF2PNG_poupeesTIF.png
  otbImageFileReaderRGBTest
  ${INPUTDATA}/poupeesTIF/
  ${TEMP}/ioImageFileReaderRGB_Cai_SPOT5TIF2PNG_poupeesTIF.png )

otb_add_test(NAME ioTvImageFileReaderRGB_PNG2PNG COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/couleurs_extrait.png
  ${TEMP}/ioImageFileReaderRGB_PNG2PNG.png
  otbImageFileReaderRGBTest
  ${INPUTDATA}/couleurs_extrait.png
  ${TEMP}/ioImageFileReaderRGB_PNG2PNG.png )

otb_add_test(NAME ioTvImageReaderWriterRgbJPEG2TIF COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/couleurs.jpg
  ${TEMP}/ioImageReaderWriterRgbJPEG2TIF_couleurs.tif
  otbImageFileReaderRGBTest
  ${INPUTDATA}/couleurs.jpg
  ${TEMP}/ioImageReaderWriterRgbJPEG2TIF_couleurs.tif )

otb_add_test(NAME ioTvImageReaderWriterRgbPNG2TIF COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees.png
  ${TEMP}/ioImageReaderWriterRgbPNG2TIF_poupees.tif
  otbImageFileReaderRGBTest
  ${INPUTDATA}/poupees.png
  ${TEMP}/ioImageReaderWriterRgbPNG2TIF_poupees.tif )

otb_add_test(NAME ioTvImageFileReaderRGB_PNG2TIFF COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileReaderRGB_PNG2TIFF.tiff
  ${TEMP}/ioImageFileReaderRGB_PNG2TIFF.tiff
  otbImageFileReaderRGBTest
  ${INPUTDATA}/couleurs_extrait.png
  ${TEMP}/ioImageFileReaderRGB_PNG2TIFF.tiff )

otb_add_test(NAME ioTvImageReaderWriterRgbJPEG2PNG COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/couleurs.jpg
  ${TEMP}/ioImageReaderWriterRgbJPEG2PNG_couleurs.png
  otbImageFileReaderRGBTest
  ${INPUTDATA}/couleurs.jpg
  ${TEMP}/ioImageReaderWriterRgbJPEG2PNG_couleurs.png )

otb_add_test(NAME ioTvImageReaderWriterRgbPNG2JPEG COMMAND otbImageIOTestDriver
  otbImageFileReaderRGBTest
  ${INPUTDATA}/poupees.png
  ${TEMP}/ioImageReaderWriterRgbPNG2JPEG_poupees.jpg )

otb_add_test(NAME ioTvImageReaderWriterRgbTIF2PNG COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees.tif
  ${TEMP}/ioImageReaderWriterRgbTIF2PNG_poupees.png
  otbImageFileReaderRGBTest
  ${INPUTDATA}/poupees.tif
  ${TEMP}/ioImageReaderWriterRgbTIF2PNG_poupees.png )


otb_add_test(NAME ioTvImageFileWriterRGB_PNG2PNG COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileWriterRGB_PNG2PNG.png
  ${TEMP}/ioImageFileWriterRGB_PNG2PNG.png
  otbImageFileWriterRGBTest
  ${INPUTDATA}/couleurs_extrait.png
  ${TEMP}/ioImageFileWriterRGB_PNG2PNG.png )

otb_add_test(NAME ioTvGDALImageFileWriterTIF2TIF COMMAND otbImageIOTestDriver
  otbImageFileWriterRGBTest
  ${INPUTDATA}/poupeesTIF/IMAGERY.TIF
  ${TEMP}/poupees.tiff )

otb_add_test(NAME ioTvImageMetadataStreamingWriterTest_HFA COMMAND otbImageIOTestDriver
  --compare-metadata ${EPSILON_9}
  ${INPUTDATA}/HFAGeoreferenced.img
  ${TEMP}/ioTvImageMetadataStreamingWriterHFA.img
  otbImageMetadataStreamingFileWriterTest
  ${INPUTDATA}/HFAGeoreferenced.img
  ${TEMP}/ioTvImageMetadataStreamingWriterHFA.img
  )

otb_add_test(NAME ioTvImageMetadataStreamingWriterTest_TIF COMMAND otbImageIOTestDriver
  --compare-metadata ${EPSILON_9}
  ${INPUTDATA}/spot5SubWithGcps.tif
  ${TEMP}/ioTvImageMetadataStreamingWriterTIF.tif
  otbImageMetadataStreamingFileWriterTest
  ${INPUTDATA}/spot5SubWithGcps.tif
  ${TEMP}/ioTvImageMetadataStreamingWriterTIF.tif
  )

otb_add_test(NAME ioTvGDALDriverDoubleWritingTest COMMAND otbImageIOTestDriver
  --compare-n-images ${EPSILON_9} 4
  ${INPUTDATA}/poupees.tif
  ${TEMP}/ioGdalDriverDoubleWritingTestImage1.tif
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ioGdalDriverDoubleWritingTestImage2.tif
  ${INPUTDATA}/poupees.tif
  ${TEMP}/ioGdalDriverDoubleWritingTestImage3.tif
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ioGdalDriverDoubleWritingTestImage4.tif
  otbGDALDriverDoubleWritingTest
  ${INPUTDATA}/poupees.tif
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ioGdalDriverDoubleWritingTestImage1.tif
  ${TEMP}/ioGdalDriverDoubleWritingTestImage2.tif
  ${TEMP}/ioGdalDriverDoubleWritingTestImage3.tif
  ${TEMP}/ioGdalDriverDoubleWritingTestImage4.tif
  )

otb_add_test(NAME ioTvStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfDivisionsStrippedStreaming COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfDivisionsStrippedStreaming.tif
  otbImageFileWriterTestCalculateNumberOfDivisions
  ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfDivisionsStrippedStreaming.tif
  SetNumberOfDivisionsStrippedStreaming
  10
  )

otb_add_test(NAME ioTvStreamingImageFileWriterCalculateNumberOfDivisions_SetAutomaticTiledStreaming COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetAutomaticTiledStreaming.tif
  otbImageFileWriterTestCalculateNumberOfDivisions
  ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetAutomaticTiledStreaming.tif
  SetAutomaticTiledStreaming
  10
  )

otb_add_test(NAME ioTvStreamingImageFileWriterCalculateNumberOfDivisions_DEFAULT COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_DEFAULT.tif
  otbImageFileWriterTestCalculateNumberOfDivisions
  ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_DEFAULT.tif
  DEFAULT
  )

otb_add_test(NAME ioTvStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfLinesStrippedStreaming COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfLinesStrippedStreaming.tif
  otbImageFileWriterTestCalculateNumberOfDivisions
  ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfLinesStrippedStreaming.tif
  SetNumberOfLinesStrippedStreaming
  5
  )

otb_add_test(NAME ioTvStreamingImageFileWriterCalculateNumberOfDivisions_SetTileDimensionTiledStreaming COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/QB_Toulouse_ortho_labelImage.tif
  ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetTileDimensionTiledStreaming.tif
  otbImageFileWriterTestCalculateNumberOfDivisions
  ${INPUTDATA}/QB_Toulouse_ortho_labelImage.tif
  ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetTileDimensionTiledStreaming.tif
  SetTileDimensionTiledStreaming
  256
  )

otb_add_test(NAME ioTvStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfDivisionsTiledStreamingS COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfDivisionsTiledStreaming.tif
  otbImageFileWriterTestCalculateNumberOfDivisions
  ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfDivisionsTiledStreaming.tif
  SetNumberOfDivisionsTiledStreaming
  10
  )

otb_add_test(NAME ioTvStreamingImageFileWriterCalculateNumberOfDivisions_SetAutomaticStrippedStreaming COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetAutomaticStrippedStreaming.tif
  otbImageFileWriterTestCalculateNumberOfDivisions
  ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetAutomaticStrippedStreaming.tif
  SetAutomaticStrippedStreaming
  10
  )

otb_add_test(NAME ioTvImageFileReaderRADCI22TIF COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE}/ioImageFileReaderRADCI22TIF.tif
  ${TEMP}/ioImageFileReaderRADCI22TIF.tif
  otbImageFileReaderRADComplexFloatExtract
  LARGEINPUT{RAD_CI2/3017_HH.rad}
  ${TEMP}/ioImageFileReaderRADCI22TIF.tif )

otb_add_test(NAME ioTvImageFileReaderTIF2RADCI4 COMMAND otbImageIOTestDriver
  otbImageFileReaderRADComplexInt
  ${INPUTDATA}/RSAT_imagery_VH.tif
  ${TEMP}/ioImageFileReaderTIF2RADCI4.rad )

otb_add_test(NAME ioTvScalarBufferToImageFileWriterTest COMMAND otbImageIOTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/ioTvScalarBufferToImageFileWriterTest.tif
  ${TEMP}/ioTvScalarBufferToImageFileWriterTest.tif
  otbScalarBufferToImageFileWriterTest
  4
  50
  75
  ${TEMP}/ioTvScalarBufferToImageFileWriterTest.tif
  )

otb_add_test(NAME ioTvPipelineMetadataHandlingTest COMMAND otbImageIOTestDriver
  --compare-metadata ${EPSILON_9}
  ${INPUTDATA}/HFAGeoreferenced.img
  ${TEMP}/ioTvPipelineMetadataHandling.img
  otbPipelineMetadataHandlingTest
  ${INPUTDATA}/HFAGeoreferenced.img
  ${TEMP}/ioTvPipelineMetadataHandling.img
  )

otb_add_test(NAME ioTvMultiResolutionReadingInfo_TIFF COMMAND otbImageIOTestDriver
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiResolutionReadingInfoOut_tiff.txt
  ${TEMP}/ioTvMultiResolutionReadingInfoOut_tiff.txt
  otbMultiResolutionReadingInfo
  ${INPUTDATA}/maur_rgb.tif
  ${TEMP}/ioTvMultiResolutionReadingInfoOut_tiff.txt
  )

otb_add_test(NAME ioTvMultiResolutionReadingInfo_TIFF2 COMMAND otbImageIOTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvMultiResolutionReadingInfoOut_tiff2.txt
  ${TEMP}/ioTvMultiResolutionReadingInfoOut_tiff2.txt
  otbMultiResolutionReadingInfo
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
  ${TEMP}/ioTvMultiResolutionReadingInfoOut_tiff2.txt
  )

otb_add_test(NAME ioTvMultiResolutionReadingInfo COMMAND otbImageIOTestDriver
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiResolutionReadingInfoOut.txt
  ${TEMP}/ioTvMultiResolutionReadingInfoOut.txt
  otbMultiResolutionReadingInfo
  ${INPUTDATA}/bretagne.j2k
  ${TEMP}/ioTvMultiResolutionReadingInfoOut.txt
  )

otb_add_test(NAME ioTvImageFileReaderPCI2TIF COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/ioImageFileReaderPCI2TIF.tif
  otbImageFileReaderTestFloat
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.pix
  ${TEMP}/ioImageFileReaderPCI2TIF.tif )

otb_add_test(NAME ioTvImageFileReaderTIF2PCI COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/ioImageFileReaderTIF2PCI.pix
  otbImageFileReaderTestFloat
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/ioImageFileReaderTIF2PCI.pix )

otb_add_test(NAME ioTvVectorImageComplexIntoRealDoubleTest COMMAND otbImageIOTestDriver
  otbVectorImageComplexIntoRealDoubleTest
  ${INPUTDATA}/multibandComplexDouble_2bands.raw.hdr
  )

otb_add_test(NAME ioTvVectorImageComplexDoubleTest COMMAND otbImageIOTestDriver
  otbVectorImageComplexDoubleTest
  ${INPUTDATA}/multibandComplexDouble_2bands.raw.hdr
  )

otb_add_test(NAME ioTvVectorImageComplexFloatTest COMMAND otbImageIOTestDriver
  otbVectorImageComplexFloatTest
  ${INPUTDATA}/multibandComplexFloat_2bands.raw.hdr
  )

otb_add_test(NAME ioTvVectorImageComplexIntoRealFloatTest COMMAND otbImageIOTestDriver
  otbVectorImageComplexIntoRealFloatTest
  ${INPUTDATA}/multibandComplexFloat_2bands.raw.hdr
  )

otb_add_test(NAME ioTvImageComplexFloatTest COMMAND otbImageIOTestDriver
  otbImageComplexFloatTest
  ${INPUTDATA}/multibandComplexFloat_2bands.raw.hdr
  )

otb_add_test(NAME ioTvImageComplexDoubleTest COMMAND otbImageIOTestDriver
  otbImageComplexDoubleTest
  ${INPUTDATA}/multibandComplexDouble_2bands.raw.hdr
  )

otb_add_test(NAME ioTvImageSeriesFileReader COMMAND otbImageIOTestDriver
  --compare-n-images ${EPSILON_9} 2
  ${BASELINE}/ioTvImageSeries-0.png
  ${TEMP}/ioTvImageSeries-0.png
  ${BASELINE}/ioTvImageSeries-1.png
  ${TEMP}/ioTvImageSeries-1.png
  otbImageSeriesFileReader
  ${INPUTDATA}/GomaAvantApres
  ${TEMP}/ioTvImageSeries.png
  )

otb_add_test(NAME ioTvImageFileReaderWithComplexPixel_PALSAR COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE}/ioImageFileReaderWithComplexPixel_PALSAR_l1.1.tif
  ${TEMP}/ioImageFileReaderWithComplexPixel_PALSAR_l1.1.tif
  otbImageFileReaderWithComplexPixelTest
  LARGEINPUT{PALSAR/200801280007/l1data/VOL-ALPSRP037120700-H1.1__A}
  ${TEMP}/ioImageFileReaderWithComplexPixel_PALSAR_l1.1.tif
  0 0 100 100)

otb_add_test(NAME ioTvImageFileReaderWithComplexPixel_ERS COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE}/ioImageFileReaderWithComplexPixel_ERS_SLCI.tif
  ${TEMP}/ioImageFileReaderWithComplexPixel_ERS_SLCI.tif
  otbImageFileReaderWithComplexPixelTest
  LARGEINPUT{SAR_ERS2_SLCI_SCENE1/}
  ${TEMP}/ioImageFileReaderWithComplexPixel_ERS_SLCI.tif
  0 0 100 100)

otb_add_test(NAME ioTvImageFileReaderWithComplexPixel_RADARSAT COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE}/ioImageFileReaderWithComplexPixel_RADARSAT.tif
  ${TEMP}/ioImageFileReaderWithComplexPixel_RADARSAT.tif
  otbImageFileReaderWithComplexPixelTest
  LARGEINPUT{RADARSAT1/GOMA/SCENE01/}
  ${TEMP}/ioImageFileReaderWithComplexPixel_RADARSAT.tif
  0 0 100 100)

otb_add_test(NAME ioTvVectorImageFileReaderWithComplexPixel_RADARSAT2 COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE}/ioImageFileReaderWithComplexPixel_RADARSAT2.tif
  ${TEMP}/ioImageFileReaderWithComplexPixel_RADARSAT2.tif
  otbVectorImageFileReaderWithComplexPixelTest
  LARGEINPUT{RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/product.xml}
  ${TEMP}/ioImageFileReaderWithComplexPixel_RADARSAT2.tif
  0 0 100 100)

otb_add_test(NAME ioTvVImageFileReader_RESOLUTION_0 COMMAND otbImageIOTestDriver
  otbVectorImageFileReaderWriterTest
  ${INPUTDATA}/maur_rgb.tif?&resol=0
  ${TEMP}/ioImageFileReader_RESOLUTION_0.tif )

otb_add_test(NAME ioTvVImageFileReader_RESOLUTION_3 COMMAND otbImageIOTestDriver
  otbVectorImageFileReaderWriterTest
  ${INPUTDATA}/maur_rgb.tif?&resol=3
  ${TEMP}/ioImageFileReader_RESOLUTION_3.tif )

otb_add_test(NAME ioTvVectorImageFileReaderWriterTest COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}
  ${INPUTDATA}/qb_RoadExtract.img.hdr
  ${TEMP}/ioTvVectorImageFileReaderWriterTest.hdr
  otbVectorImageFileReaderWriterTest
  ${INPUTDATA}/qb_RoadExtract.img.hdr
  ${TEMP}/ioTvVectorImageFileReaderWriterTest.hdr
  )

otb_add_test(NAME ioTvVImageFileReader_RESOLUTION_4 COMMAND otbImageIOTestDriver
  otbVectorImageFileReaderWriterTest
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF?&resol=4}
  ${TEMP}/ioImageFileReader_RESOLUTION_4.tif )

otb_add_test(NAME ioTvVectorImageFileReaderWriterJP22TIF COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}  ${BASELINE}/ioTvVectorImageFileReaderWriterJP22TIFOutput.tif
  ${TEMP}/ioTvVectorImageFileReaderWriterJP22TIFOutput.tif
  otbVectorImageFileReaderWriterTest
  LARGEINPUT{JPEG2000/QB_multicomp_small.jpx}
  ${TEMP}/ioTvVectorImageFileReaderWriterJP22TIFOutput.tif
  )

otb_add_test(NAME ioTvCheckNbBandsPNGIndexee COMMAND otbImageIOTestDriver
  otbPNGIndexedNbBandsTest
  ${INPUTDATA}/sbuv_indexee.png
  1 )

otb_add_test(NAME ioTvImageFileReaderPNG2PNG COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileReaderPNG2PNG_cthead1.png
  ${TEMP}/ioImageFileReaderPNG2PNG_cthead1.png
  otbImageFileReaderTest
  ${INPUTDATA}/cthead1.png
  ${TEMP}/ioImageFileReaderPNG2PNG_cthead1.png )

otb_add_test(NAME ioTvImageFileReaderPDS2TIFF COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/pdsImage.img
  ${TEMP}/ioTvImageFileReaderPDS2TIFF.tif
  otbImageFileReaderTest
  ${INPUTDATA}/pdsImage.img
  ${TEMP}/ioTvImageFileReaderPDS2TIFF.tif)

otb_add_test(NAME ioTvWritingComplexDataWithComplexImage COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE}/ioWritingComplexDataWithComplexImageOutput.tif
  ${TEMP}/ioWritingComplexDataWithComplexImageOutput.tif
  otbWritingComplexDataWithComplexImageTest
  LARGEINPUT{RADARSAT1/GOMA2/SCENE01/DAT_01.001}
  ${TEMP}/ioWritingComplexDataWithComplexImageOutput.tif
  1000 1000
  500  500
  )

otb_add_test(NAME ioTvImageFileReaderCR42CR8 COMMAND otbImageIOTestDriver
  otbImageFileReaderRADComplexDouble
  ${INPUTDATA}/RADCR4_image.rad
  ${TEMP}/ioImageFileReaderCR42CR8.rad )

otb_add_test(NAME ioTvStreamingPipeline_TIF2PNG COMMAND otbImageIOTestDriver
  otbPipeline
  ${INPUTDATA}/cthead1.png #ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ioPipeline.png
  )

otb_add_test(NAME ioTvStreamingImageFilterGDAL_TIFF COMMAND otbImageIOTestDriver
  otbStreamingImageFilterTest
  ${INPUTDATA}/poupeesTIF/IMAGERY.TIF
  ${TEMP}/ioStreamingImageFilterTestGDAL_PoupeesTif.png
  )

otb_add_test(NAME ioTvStreamingImageFilterGDAL_ENVI COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9} ${INPUTDATA}/poupees_1canal.c1.hdr
  ${TEMP}/ioStreamingImageFilterTestCDAL_ENVI.png
  otbStreamingImageFilterTest
  ${INPUTDATA}/poupees_1canal.c1.hdr
  ${TEMP}/ioStreamingImageFilterTestCDAL_ENVI.png
  )

otb_add_test(NAME ioTvStreamingShortImageFileWriterStreamedTest COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}
  ${INPUTDATA}/poupees_sub.png
  ${TEMP}/ioTvStreamingShortImageFileWriterStreamedTest.png
  otbStreamingShortImageFileWriterTest
  ${INPUTDATA}/poupees_sub.png
  ${TEMP}/ioTvStreamingShortImageFileWriterStreamedTest.png
  1
  4
  )

otb_add_test(NAME ioTvStreamingShortImageFileWriterTest COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}
  ${INPUTDATA}/poupees_sub.png
  ${TEMP}/ioTvStreamingShortImageFileWriterTest.png
  otbStreamingShortImageFileWriterTest
  ${INPUTDATA}/poupees_sub.png
  ${TEMP}/ioTvStreamingShortImageFileWriterTest.png
  0
  )

otb_add_test(NAME ioTvImageFileWriterPNG2PNG COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/cthead1.png
  ${TEMP}/ioImageFileWriterPNG2PNG_cthead1.png
  otbImageFileWriterTest
  ${INPUTDATA}/cthead1.png
  ${TEMP}/ioImageFileWriterPNG2PNG_cthead1.png )


otb_add_test(NAME ioTvImageFileWriterTIF2TIF COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioTvImageFileWriterTIF2TIFOutput.tif
  ${TEMP}/ioImageFileWriterTIF2TIF_QB_Toulouse_Ortho_PAN.tif
  otbImageFileWriterTest
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/ioImageFileWriterTIF2TIF_QB_Toulouse_Ortho_PAN.tif?&epsg=4326 )

otb_add_test(NAME ioTvCompareWritingComplexImage_TIF COMMAND otbImageIOTestDriver
  otbCompareWritingComplexImageTest
  LARGEINPUT{PALSAR/200801280007/l1data/VOL-ALPSRP037120700-H1.1__A}
  ${TEMP}/ioTvImageFileReaderCompareComplexPixelWithFloatPixel_PALSAR.tif
  0 0 100 100)

otb_add_test(NAME ioTvCompareWritingComplexImage_HDR COMMAND otbImageIOTestDriver
  otbCompareWritingComplexImageTest
  LARGEINPUT{PALSAR/200801280007/l1data/VOL-ALPSRP037120700-H1.1__A}
  ${TEMP}/ioTvImageFileReaderCompareComplexPixelWithFloatPixel_PALSAR.hdr
  0 0 100 100)


if(OTB_DATA_USE_LARGEINPUT)
  set( GenericTestPHR_TESTNB 0)

foreach( current_file ${GenericTestPHR_DATA_INPUTS} )
  list(GET GenericTestPHR_DATA_TYPES   ${GenericTestPHR_TESTNB} current_type   )
  math(EXPR GenericTestPHR_TESTNB "${GenericTestPHR_TESTNB} + 1")
  string(REGEX MATCH "TIFF" PHR_TIFF_FILE ${current_type})



if(NOT PHR_TIFF_FILE)
    # Extract resolution of input file
    otb_add_test(NAME ioTvMultiResolutionReadingInfo_${current_type} COMMAND otbImageIOTestDriver
             --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioTvMultiResolutionReadingInfo_${current_type}.txt
                                       ${TEMP}/ioTvMultiResolutionReadingInfo_${current_type}_OUT.txt
             --ignore-lines-with 1 SubDatasetIndex
             otbMultiResolutionReadingInfo
                ${current_file}
                ${TEMP}/ioTvMultiResolutionReadingInfo_${current_type}_OUT.txt
            )

endif()
endforeach()
endif()


# Tests about complex reading

set(INPUTFILE_PIXELTYPES_LIST "Int" "Short" "Float" "Double")
set(READING_PIXELTYPES_LIST "Int" "Short" "Float" "Double")
set(NBBANDS_LIST "2" "3" "4")

foreach(INPUTFILE_PIXELTYPE ${INPUTFILE_PIXELTYPES_LIST})

  foreach(READING_PIXELTYPE ${READING_PIXELTYPES_LIST})

    otb_add_test(NAME ioTvMonobandScalar${INPUTFILE_PIXELTYPE}ToImageComplex${READING_PIXELTYPE}
      COMMAND otbImageIOTestDriver
      otbMonobandScalarToImageComplex${READING_PIXELTYPE}
      ${INPUTDATA}/monoband${INPUTFILE_PIXELTYPE}.tif)

    otb_add_test(NAME ioTvMonobandScalar${INPUTFILE_PIXELTYPE}ToVectorImageComplex${READING_PIXELTYPE}
      COMMAND otbImageIOTestDriver
      otbMonobandScalarToVectorImageComplex${READING_PIXELTYPE}
      ${INPUTDATA}/monoband${INPUTFILE_PIXELTYPE}.tif)

    otb_add_test(NAME ioTvMonobandComplex${INPUTFILE_PIXELTYPE}ToImageScalar${READING_PIXELTYPE}
      COMMAND otbImageIOTestDriver
      otbMonobandComplexToImageScalar${READING_PIXELTYPE}
      ${INPUTDATA}/monobandComplex${INPUTFILE_PIXELTYPE}.tif) #hdr)

    otb_add_test(NAME ioTvMonobandComplex${INPUTFILE_PIXELTYPE}ToImageComplex${READING_PIXELTYPE}
      COMMAND otbImageIOTestDriver
      otbMonobandComplexToImageComplex${READING_PIXELTYPE}
      ${INPUTDATA}/monobandComplex${INPUTFILE_PIXELTYPE}.tif) #hdr)

    otb_add_test(NAME ioTvMonobandComplex${INPUTFILE_PIXELTYPE}ToVectorImageScalar${READING_PIXELTYPE}
      COMMAND otbImageIOTestDriver
      otbMonobandComplexToVectorImageScalar${READING_PIXELTYPE}
      ${INPUTDATA}/monobandComplex${INPUTFILE_PIXELTYPE}.tif) #hdr)

    otb_add_test(NAME ioTvMonobandComplex${INPUTFILE_PIXELTYPE}ToVectorImageComplex${READING_PIXELTYPE}
      COMMAND otbImageIOTestDriver
      otbMonobandComplexToVectorImageComplex${READING_PIXELTYPE}
      ${INPUTDATA}/monobandComplex${INPUTFILE_PIXELTYPE}.tif) #hdr)

    # Weird case : only the first 2 bands used
    otb_add_test(NAME ioTvMultibandScalar${INPUTFILE_PIXELTYPE}ToImageComplex${READING_PIXELTYPE}
      COMMAND otbImageIOTestDriver
      otbMultibandScalarToImageComplex${READING_PIXELTYPE}
      ${INPUTDATA}/multiband${INPUTFILE_PIXELTYPE}.tif)

    # Weird case : one scalar band -> one complex band with imag=0
    # but interleaving real and imaginary parts can only work for an even nb of bands
    otb_add_test(NAME ioTvMultibandScalar${INPUTFILE_PIXELTYPE}ToVectorImageComplex${READING_PIXELTYPE}
      COMMAND otbImageIOTestDriver
      otbMultibandScalarToVectorImageComplex${READING_PIXELTYPE}
      ${INPUTDATA}/multiband${INPUTFILE_PIXELTYPE}.tif)

    foreach(NBBANDS ${NBBANDS_LIST})

      # Weird case : need specifications to write the test
      # otb_add_test(NAME ioTvMultibandComplex${INPUTFILE_PIXELTYPE}${NBBANDS}BandsToImageScalar${READING_PIXELTYPE}
      #          COMMAND otbImageIOTestDriver
      #          otbMultibandComplexToImageScalar${READING_PIXELTYPE}
      #          ${INPUTDATA}/multibandComplex${INPUTFILE_PIXELTYPE}_${NBBANDS}bands.tif) #hdr)

      # Use Size 50x50 because 100x100 is to large to store pixel values coded as short.

      otb_add_test(NAME ioTvMultibandComplex${INPUTFILE_PIXELTYPE}${NBBANDS}BandsToVectorImageScalar${READING_PIXELTYPE}
        COMMAND otbImageIOTestDriver
        otbMultibandComplexToVectorImageScalar${READING_PIXELTYPE}
        ${INPUTDATA}/multibandComplex${INPUTFILE_PIXELTYPE}_${NBBANDS}bands_50x50.tif) #hdr)

      # Weird case : need specifications to write the test
      # [missing impl]
      # otb_add_test(NAME ioTvMultibandComplex${INPUTFILE_PIXELTYPE}${NBBANDS}BandsToImageComplex${READING_PIXELTYPE})

      # Use Size 50x50 because 100x100 is to large to store pixel values coded as short.

      otb_add_test(NAME ioTvMultibandComplex${INPUTFILE_PIXELTYPE}${NBBANDS}BandsToVectorImageComplex${READING_PIXELTYPE}
        COMMAND otbImageIOTestDriver
        otbMultibandComplexToVectorImageComplex${READING_PIXELTYPE}
        ${INPUTDATA}/multibandComplex${INPUTFILE_PIXELTYPE}_${NBBANDS}bands_50x50.tif) #hdr)

    endforeach()

  endforeach()

endforeach()


# Tests about writing Scalar / Complex data

set(DATA_PIXELTYPES_LIST "Int" "Short" "Float" "Double")
set(WRITING_PIXELTYPES_LIST "Int" "Short" "Float" "Double")

foreach(DATA_PIXELTYPE ${DATA_PIXELTYPES_LIST})

  otb_add_test(NAME ioTvWriteImageScalar${DATA_PIXELTYPE}
    COMMAND otbImageIOTestDriver
    --compare-image ${NOTOL}
    ${INPUTDATA}/monoband${DATA_PIXELTYPE}.tif
    ${TEMP}/monobandOut${DATA_PIXELTYPE}.tif
    otbImageScalarFileWriterTestWithoutInput${DATA_PIXELTYPE}
    ${TEMP}/monobandOut${DATA_PIXELTYPE}.tif
    100
    )

  otb_add_test(NAME ioTvWriteImageComplex${DATA_PIXELTYPE}
    COMMAND otbImageIOTestDriver
    --compare-image ${NOTOL}
    ${INPUTDATA}/monobandComplex${DATA_PIXELTYPE}.tif
    ${TEMP}/monobandComplexOut${DATA_PIXELTYPE}.tif
    otbImageComplexFileWriterTestWithoutInput${DATA_PIXELTYPE}
    ${TEMP}/monobandComplexOut${DATA_PIXELTYPE}.tif
    100
    )

  foreach(NBBANDS ${NBBANDS_LIST})

    otb_add_test(NAME ioTvWriteVectorImageScalar${DATA_PIXELTYPE}${NBBANDS}Bands
      COMMAND otbImageIOTestDriver
      --compare-image ${NOTOL}
      ${INPUTDATA}/multiband${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
      ${TEMP}/multibandOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
      otbVectorImageFileWriterScalarTestWithoutInput${DATA_PIXELTYPE}
      ${TEMP}/multibandOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
      50
      ${NBBANDS}
      )

    otb_add_test(NAME ioTvWriteVectorImageComplex${DATA_PIXELTYPE}${NBBANDS}Bands
      COMMAND otbImageIOTestDriver
      --compare-image ${NOTOL}
      ${INPUTDATA}/multibandComplex${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
      ${TEMP}/multibandComplexOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
      otbVectorImageFileWriterComplexTestWithoutInput${DATA_PIXELTYPE}
      ${TEMP}/multibandComplexOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
      50
      ${NBBANDS}
      )

  endforeach()

  ############
  # Tests about Streaming writing Scalar / Complex data
  otb_add_test(NAME ioTvStreamingWriteImageScalar${DATA_PIXELTYPE}
    COMMAND otbImageIOTestDriver
    --compare-image ${NOTOL}
    ${INPUTDATA}/monoband${DATA_PIXELTYPE}.tif
    ${TEMP}/monobandStrOut${DATA_PIXELTYPE}.tif
    otbImageScalarStreamingFileWriterTestWithoutInput${DATA_PIXELTYPE}
    ${TEMP}/monobandStrOut${DATA_PIXELTYPE}.tif
    100
    )

  otb_add_test(NAME ioTvStreamingWriteImageComplex${DATA_PIXELTYPE}
    COMMAND otbImageIOTestDriver
    --compare-image ${NOTOL}
    ${INPUTDATA}/monobandComplex${DATA_PIXELTYPE}.tif
    ${TEMP}/monobandStrComplexOut${DATA_PIXELTYPE}.tif
    otbImageComplexStreamingFileWriterTestWithoutInput${DATA_PIXELTYPE}
    ${TEMP}/monobandStrComplexOut${DATA_PIXELTYPE}.tif
    100
    )

  foreach(NBBANDS ${NBBANDS_LIST})

    otb_add_test(NAME ioTvStreamingWriteVectorImageScalar${DATA_PIXELTYPE}${NBBANDS}Bands
      COMMAND otbImageIOTestDriver
      --compare-image ${NOTOL}
      ${INPUTDATA}/multiband${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
      ${TEMP}/multibandStrOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
      otbVectorImageStreamingFileWriterScalarTestWithoutInput${DATA_PIXELTYPE}
      ${TEMP}/multibandStrOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
      50
      ${NBBANDS}
      )

    otb_add_test(NAME ioTvStreamingWriteVectorImageComplex${DATA_PIXELTYPE}${NBBANDS}Bands
      COMMAND otbImageIOTestDriver
      --compare-image ${NOTOL}
      ${INPUTDATA}/multibandComplex${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
      ${TEMP}/multibandStrComplexOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
      otbVectorImageStreamingFileWriterComplexTestWithoutInput${DATA_PIXELTYPE}
      ${TEMP}/multibandStrComplexOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
      50
      ${NBBANDS}
      )

  endforeach()

endforeach()

otb_add_test(NAME  ioTvImageFileReaderRGB_PNG2ENVI COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}        ${INPUTDATA}/couleurs_extrait.png
                                ${TEMP}/ioImageFileReaderRGB_PNG2ENVI.hdr
        otbImageFileReaderRGBTest
        ${INPUTDATA}/couleurs_extrait.png
        ${TEMP}/ioImageFileReaderRGB_PNG2ENVI.hdr )

otb_add_test(NAME  ioTvImageFileReaderPNG2ENVI COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}        ${BASELINE}/ioImageFileReaderPNG2ENVI.tif
                                ${TEMP}/ioImageFileReaderPNG2ENVI.hdr
        otbImageFileReaderTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileReaderPNG2ENVI.hdr )

otb_add_test(NAME ioTvImageIOToReaderOptions_OptBandTest COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9} ${BASELINE}/QB_Toulouse_Ortho_XS_OptBand2to4.tif
                               ${TEMP}/QB_Toulouse_Ortho_XS_OptBand2to4.tif
  otbImageFileReaderOptBandTest
  ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif?&bands=2:4
  ${TEMP}/QB_Toulouse_Ortho_XS_OptBand2to4.tif
  4
  )

otb_add_test(NAME ioTvImageIOToReaderOptions_OptBandReorgTest COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9} ${BASELINE}/QB_Toulouse_Ortho_XS_OptBandReorg.tif
                               ${TEMP}/QB_Toulouse_Ortho_XS_OptBandReorg.tif
  otbImageFileReaderOptBandTest
  ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif?&bands=2,:,-3,2:-1
  ${TEMP}/QB_Toulouse_Ortho_XS_OptBandReorg.tif
  4
  )

otb_add_test(NAME ioTvImageIOToWriterOptions_OptBandTest COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9} ${BASELINE}/QB_Toulouse_Ortho_XS_OptBand2to4.tif
                               ${TEMP}/QB_Toulouse_Ortho_XS_WriterOptBand2to4.tif
  otbImageFileWriterOptBandTest
  ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
  ${TEMP}/QB_Toulouse_Ortho_XS_WriterOptBand2to4.tif?&bands=2:4
  4
  )

otb_add_test(NAME ioTvImageIOToWriterOptions_OptBandReorgTest COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9} ${BASELINE}/QB_Toulouse_Ortho_XS_OptBandReorg.tif
                               ${TEMP}/QB_Toulouse_Ortho_XS_WriterOptBandReorg.tif
  otbImageFileWriterOptBandTest
  ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
  ${TEMP}/QB_Toulouse_Ortho_XS_WriterOptBandReorg.tif?&bands=2,:,-3,2:-1
  4
  )

otb_add_test(NAME ioTvMultiImageFileWriter_SameSize
  COMMAND otbImageIOTestDriver
  --compare-n-images ${EPSILON_9} 2
  ${INPUTDATA}/GomaAvant.png
  ${TEMP}/ioTvMultiImageFileWriter_SameSize1.tif
  ${INPUTDATA}/GomaApres.png
  ${TEMP}/ioTvMultiImageFileWriter_SameSize2.tif
  otbMultiImageFileWriterTest
  ${INPUTDATA}/GomaAvant.png
  ${INPUTDATA}/GomaApres.png
  ${TEMP}/ioTvMultiImageFileWriter_SameSize1.tif
  ${TEMP}/ioTvMultiImageFileWriter_SameSize2.tif
  50)

otb_add_test(NAME ioTvMultiImageFileWriter_DiffSize
  COMMAND otbImageIOTestDriver
  --compare-n-images ${EPSILON_9} 2
  ${INPUTDATA}/GomaAvant.png
  ${TEMP}/ioTvMultiImageFileWriter_DiffSize1.tif
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/ioTvMultiImageFileWriter_DiffSize2.tif
  otbMultiImageFileWriterTest
  ${INPUTDATA}/GomaAvant.png
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/ioTvMultiImageFileWriter_DiffSize1.tif
  ${TEMP}/ioTvMultiImageFileWriter_DiffSize2.tif
  25)

otb_add_test(NAME ioTvMultiImageFileWriter_BoxExtendedFilename
  COMMAND otbImageIOTestDriver
  --compare-n-images ${EPSILON_9} 2
  ${BASELINE}/ioTvMultiImageFileWriter_ExtendedFilename1.tif
  ${TEMP}/ioTvMultiImageFileWriter_ExtendedFilename1.tif
  ${BASELINE}/ioTvMultiImageFileWriter_ExtendedFilename2.tif
  ${TEMP}/ioTvMultiImageFileWriter_ExtendedFilename2.tif
  otbMultiImageFileWriterTest
  ${INPUTDATA}/GomaAvant.png
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/ioTvMultiImageFileWriter_ExtendedFilename1.tif?&box=40:30:5:10
  ${TEMP}/ioTvMultiImageFileWriter_ExtendedFilename2.tif?&box=10:10:15:15
  50)




otb_add_test(NAME ioTvCompareMetadataTest COMMAND otbImageIOTestDriver
  --compare-metadata ${EPSILON_9}
  LARGEINPUT{SENTINEL1/S1A_S6_SLC__1SSV_20150619T195043}
  LARGEINPUT{SENTINEL1/S1A_S6_SLC__1SSV_20150619T195043}
  otbImageFileReaderRGBTest
  ${INPUTDATA}/couleurs_extrait.png
  ${TEMP}/ioImageFileReaderRGB_PNG2PNG.png )

# Imports metadata from a tif file containing OTB type metadata and export it in another tif
otb_add_test(NAME ioTvImportExportMetadataTest COMMAND otbImageIOTestDriver
  --compare-metadata ${EPSILON_9}
  ${INPUTDATA}/metadataIOexample.tif
  ${TEMP}/ioTvImportExportMetadataTest.tif
  otbImageFileReaderTest
  ${INPUTDATA}/metadataIOexample.tif # contains OTB metadata
  ${TEMP}/ioTvImportExportMetadataTest.tif )
