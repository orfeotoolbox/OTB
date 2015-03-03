otb_module_test()

set(OTBImageIOTests
otbImageIOTestDriver.cxx
otbImageFileWriterWithExtendedOptionBox.cxx
otbImageFileReaderONERAComplex.cxx
otbImageFileReaderRADComplexFloat.cxx
otbShortRGBImageIOTest.cxx
otbPipelineMetadataHandlingWithUFFilterTest.cxx
otbImageMetadataFileWriterTest.cxx
otbImageFileWriterTestWithoutInput.cxx
otbVectorImageStreamingFileWriterTestWithoutInput.cxx
otbReadingComplexDataIntoComplexImageTest.cxx
otbStreamingImageFileWriterTest.cxx
otbImageFileReaderRADInt.cxx
otbImageFileReaderRGBTest.cxx
otbImageMetadataStreamingFileWriterTest.cxx
otbFloatImageIOTest.cxx
otbImageFileReaderMSTAR.cxx
otbGDALDriverDoubleWritingTest.cxx
otbDoubleImageIOTest.cxx
otbStreamingImageFileWriterTestCalculateNumberOfDivisions.cxx
otbImageFileReaderRADComplexFloatExtract.cxx
otbImageFileReaderRADComplexInt.cxx
otbImageStreamingFileWriterTestWithoutInput.cxx
otbScalarBufferToImageFileWriterTest.cxx
otbImageFileWriterONERAComplex.cxx
otbPipelineMetadataHandlingTest.cxx
otbMultiResolutionReadingInfo.cxx
otbImageFileReaderTestFloat.cxx
otbComplexImageTests.cxx
otbImageFileReaderRADChar.cxx
otbScalarBufferToImageFileWriterNew.cxx
otbShortImageIOTest.cxx
otbImageSeriesFileReader.cxx
otbStreamingShortImageFileWriterTest.cxx
otbImageFileWriterStreamingONERAComplex.cxx
otbImageFileReaderWithComplexPixel.cxx
otbImageFileReaderRADFloat.cxx
otbVectorImageFileReaderWriterTest.cxx
otbIntImageIOTest.cxx
otbImageFileReaderONERA.cxx
otbImageFileReaderServerName.cxx
otbPNGIndexedNbBandsTest.cxx
otbImageFileReaderTest.cxx
otbVectorImageFileWriterTestWithoutInput.cxx
otbWritingComplexDataWithComplexImageTest.cxx
otbStreamingImageFileWriterWithFilterTest.cxx
otbImageFileReaderRADComplexDouble.cxx
otbPipeline.cxx
otbStreamingImageFilterTest.cxx
otbImageFileWriterRGBTest.cxx
otbComplexImageManipulationTest.cxx
otbImageFileWriterTest.cxx
otbImageIOFactoryNew.cxx
otbCompareWritingComplexImage.cxx
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

otb_add_test(NAME ioTvONERAImageFileReaderComplex COMMAND otbImageIOTestDriver
  --compare-n-images ${EPSILON_9} 2
  ${BASELINE}/ioImageFileReaderONERAComplexReal.hdr
  ${TEMP}/ioImageFileReaderONERAComplexReal.hdr
  ${BASELINE}/ioImageFileReaderONERAComplexImag.hdr
  ${TEMP}/ioImageFileReaderONERAComplexImag.hdr
  otbImageFileReaderONERAComplex
  LARGEINPUT{ONERA/spa3_0215_rad.ent}
  ${TEMP}/ioImageFileReaderONERAComplexReal.hdr
  ${TEMP}/ioImageFileReaderONERAComplexImag.hdr )

otb_add_test(NAME ioTvImageFileReaderRADCR42TIF COMMAND otbImageIOTestDriver
  otbImageFileReaderRADComplexFloat
  ${INPUTDATA}/RADCR4_image.rad
  ${TEMP}/ioImageFileReaderRAD2TIF.tif )

otb_add_test(NAME ioTvImageFileReaderCI42CR4 COMMAND otbImageIOTestDriver
  otbImageFileReaderRADComplexFloat
  ${INPUTDATA}/RADCI4_image.rad
  ${TEMP}/ioImageFileReaderCI42CR4.rad )

otb_add_test(NAME ioTvImageFileReaderShortRGB_BSQ2HDR COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I2.hd
  ${TEMP}/ioImageFileReaderI2_BSQ2HDR.hdr
  otbShortRGBImageIOTest
  ${INPUTDATA}/poupees_I2.hd
  ${TEMP}/ioImageFileReaderI2_BSQ2HDR.hdr )

otb_add_test(NAME ioTvImageFileReaderShortHDR2BSQ COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I2.hd
  ${TEMP}/ioImageFileReaderI2_HDR2BSQ.hd
  otbShortRGBImageIOTest
  ${TEMP}/ioImageFileReaderI2_BSQ2HDR.hdr
  ${TEMP}/ioImageFileReaderI2_HDR2BSQ.hd )
set_property(TEST ioTvImageFileReaderShortHDR2BSQ PROPERTY DEPENDS ioTvImageFileReaderShortRGB_BSQ2HDR)

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

otb_add_test(NAME ioTvImageFileReaderRGB_BSQ2PNG COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees.hdr
  ${TEMP}/ioImageFileReaderRGB_BSQ2PNG_poupees.png
  otbImageFileReaderRGBTest
  ${INPUTDATA}/poupees.hdr
  ${TEMP}/ioImageFileReaderRGB_BSQ2PNG_poupees.png )

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
  --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees
  ${TEMP}/ioImageReaderWriterRgbENVI2PNG_poupees.png
  otbImageFileReaderRGBTest
  ${INPUTDATA}/poupees
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

otb_add_test(NAME ioTvImageFileReaderRGB_BSQ2PNG_2 COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/couleurs_extrait.png
  ${TEMP}/ioImageFileReaderRGB_PNG2BSQ_2.png
  otbImageFileReaderRGBTest
  ${TEMP}/ioImageFileWriterRGB_PNG2BSQ.hdr
  ${TEMP}/ioImageFileReaderRGB_PNG2BSQ_2.png )
set_property(TEST ioTvImageFileReaderRGB_BSQ2PNG_2 PROPERTY DEPENDS ioTvImageFileWriterRGB_PNG2BSQ)

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

otb_add_test(NAME ioTvImageFileReaderRGB_BSQ2TIFF COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees.hdr
  ${TEMP}/ioImageFileReaderRGB_BSQ2TIFF_poupees.tiff
  otbImageFileReaderRGBTest
  ${INPUTDATA}/poupees.hdr
  ${TEMP}/ioImageFileReaderRGB_BSQ2TIFF_poupees.tiff )

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

otb_add_test(NAME ioTvStreamingIFWriterLUMWithoutStreaming COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}       ${TEMP}/ioImageFileReaderPNG2LUM.lum
  ${TEMP}/ioStreamingImageFileWriterLUM2LUMWithoutStreaming.lum
  otbStreamingImageFileWriterTest
  ${TEMP}/ioImageFileReaderPNG2LUM.lum
  ${TEMP}/ioStreamingImageFileWriterLUM2LUMWithoutStreaming.lum
  0 # StreamingImageFileWriter
  0 # NumberOfStreamDivisions
  )
set_property(TEST ioTvStreamingIFWriterLUMWithoutStreaming PROPERTY DEPENDS ioTvImageFileReaderPNG2LUM)

otb_add_test(NAME ioTvStreamingIFWriterBSQWithoutStreaming COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees_1canal.c1.hdr
  ${TEMP}/ioStreamingImageFileWriterBSQWithoutStreaming.hdr
  otbStreamingImageFileWriterTest
  ${INPUTDATA}/poupees_1canal.c1.hdr
  ${TEMP}/ioStreamingImageFileWriterBSQWithoutStreaming.hdr
  0 # Streaming
  0
  )
set_property(TEST ioTvStreamingIFWriterBSQWithoutStreaming PROPERTY DEPENDS ioTvImageFileReaderPNG2BSQ)

otb_add_test(NAME ioTvStreamingWithIFWriterLUMWithStreaming COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}       ${TEMP}/ioImageFileReaderPNG2LUM.lum
  ${TEMP}/ioStreamingWithImageFileWriterLUM2LUMWithStreaming_10.lum
  otbStreamingImageFileWriterTest
  ${TEMP}/ioImageFileReaderPNG2LUM.lum
  ${TEMP}/ioStreamingWithImageFileWriterLUM2LUMWithStreaming_10.lum
  0 # StreamingImageFileWriter
  10 # NumberOfStreamDivisions
  )
set_property(TEST ioTvStreamingWithIFWriterLUMWithStreaming PROPERTY DEPENDS ioTvImageFileReaderPNG2LUM)

otb_add_test(NAME ioTvStreamingWithIFWriterBSQWithStreaming COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}       ${TEMP}/ioImageFileReaderPNG2BSQ.hd
  ${TEMP}/ioStreamingWithImageFileWriterBSQ2BSQWithStreaming_10.hd
  otbStreamingImageFileWriterTest
  ${TEMP}/ioImageFileReaderPNG2BSQ.hd
  ${TEMP}/ioStreamingWithImageFileWriterBSQ2BSQWithStreaming_10.hd
  0 # StreamingImageFileWriter
  10 # NumberOfStreamDivisions
  )
set_property(TEST ioTvStreamingWithIFWriterBSQWithStreaming PROPERTY DEPENDS ioTvImageFileReaderPNG2BSQ)

otb_add_test(NAME ioTvStreamingIFWriterBSQWithStreaming COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees_1canal.c1.hdr
  ${TEMP}/ioStreamingImageFileWriterBSQWithStreaming_100.hdr
  otbStreamingImageFileWriterTest
  ${INPUTDATA}/poupees_1canal.c1.hdr
  ${TEMP}/ioStreamingImageFileWriterBSQWithStreaming_100.hdr
  1 # Streaming
  100
  )
set_property(TEST ioTvStreamingIFWriterBSQWithStreaming PROPERTY DEPENDS ioTvImageFileReaderPNG2BSQ)

otb_add_test(NAME ioTvStreamingIFWriterLUMWithStreaming COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}       ${TEMP}/ioImageFileReaderPNG2LUM.lum
  ${TEMP}/ioStreamingImageFileWriterLUM2LUMWithStreaming_10.lum
  otbStreamingImageFileWriterTest
  ${TEMP}/ioImageFileReaderPNG2LUM.lum
  ${TEMP}/ioStreamingImageFileWriterLUM2LUMWithStreaming_10.lum
  1 # StreamingImageFileWriter
  10 # NumberOfStreamDivisions
  )
set_property(TEST ioTvStreamingIFWriterLUMWithStreaming PROPERTY DEPENDS ioTvImageFileReaderPNG2LUM)

otb_add_test(NAME ioTvReadingComplexDataIntoComplexImage COMMAND otbImageIOTestDriver
  otbReadingComplexDataIntoComplexImageTest
  LARGEINPUT{RADARSAT1/GOMA2/SCENE01/DAT_01.001}
  1000 1000
  )

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

otb_add_test(NAME ioTvImageFileWriterRGB_PNG2BSQ COMMAND otbImageIOTestDriver
  otbImageFileWriterRGBTest
  ${INPUTDATA}/couleurs_extrait.png
  ${TEMP}/ioImageFileWriterRGB_PNG2BSQ.hdr )

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

otb_add_test(NAME ioTvImageFileReaderMSTAR COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileReaderMSTAR_rescal_chanel1.png
  ${TEMP}/ioImageFileReaderMSTAR_rescal_chanel1.png
  otbImageFileReaderMSTAR
  ${INPUTDATA}/MSTAR/HB19896.016
  ${TEMP}/ioImageFileReaderMSTAR_rescal_chanel1.png )

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
  --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetTileDimensionTiledStreaming.tif
  otbImageFileWriterTestCalculateNumberOfDivisions
  ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetTileDimensionTiledStreaming.tif
  SetTileDimensionTiledStreaming
  32
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

otb_add_test(NAME ioTvImageFileReaderRADCI22ENVI COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE}/ioImageFileReaderRADCI22ENVI.hdr
  ${TEMP}/ioImageFileReaderRADCI22ENVI.hdr
  otbImageFileReaderRADComplexFloatExtract
  LARGEINPUT{RAD_CI2/3017_HH.rad}
  ${TEMP}/ioImageFileReaderRADCI22ENVI.hdr )

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

otb_add_test(NAME ioTvONERAImageFileWriterComplex COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE}/ioImageFileReaderONERAComplexWrite.ent
  ${TEMP}/ioImageFileReaderONERAComplexWrite.ent
  otbImageFileWriterONERAComplex
  LARGEINPUT{ONERA/spa3_0215_rad.ent}
  ${TEMP}/ioImageFileReaderONERAComplexWrite.ent )

otb_add_test(NAME ioTvImageFileReaderServerNameOTBTest COMMAND otbImageIOTestDriver
  otbImageFileReaderServerName
  ${OTB_DATA_ROOT}/Examples/osmfile.otb)

otb_add_test(NAME ioTvImageFileReaderServerNameHTTPTest COMMAND otbImageIOTestDriver
  otbImageFileReaderServerName
  http://tile.openstreetmap.org/)

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

otb_add_test(NAME ioTvImageFileReaderMW2TIFMW COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}        ${BASELINE}/QB_Toulouse_Ortho_PAN.mw
  ${TEMP}/ioImageFileReaderMW2TIFMW.tif
  otbImageFileReaderTestFloat
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.mw
  ${TEMP}/ioImageFileReaderMW2TIFMW.tif )

otb_add_test(NAME ioTvImageFileReaderPCI2TIF COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/ioImageFileReaderPCI2TIF.tif
  otbImageFileReaderTestFloat
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.pix
  ${TEMP}/ioImageFileReaderPCI2TIF.tif )

otb_add_test(NAME ioTvImageFileReaderTIF2MW COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.mw
  ${TEMP}/ioImageFileReaderTIF2MW.mw
  otbImageFileReaderTestFloat
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/ioImageFileReaderTIF2MW.mw )

otb_add_test(NAME ioTvImageFileReaderMW2TIFIMG COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}        ${INPUTDATA}/QB_Toulouse_Ortho_PAN_nogeoinfo.tif
  ${TEMP}/ioImageFileReaderMW2TIFIMG.tif
  otbImageFileReaderTestFloat
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.mw
  ${TEMP}/ioImageFileReaderMW2TIFIMG.tif )

otb_add_test(NAME ioTvImageFileReaderTIF2PCI COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/ioImageFileReaderTIF2PCI.pix
  otbImageFileReaderTestFloat
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/ioImageFileReaderTIF2PCI.pix )

otb_add_test(NAME ioTvVectorImageComplexIntoRealDoubleTest COMMAND otbImageIOTestDriver
  otbVectorImageComplexIntoRealDoubleTest
  ${INPUTDATA}/multibandComplexDouble_2bands.hdr
  )

otb_add_test(NAME ioTvVectorImageComplexDoubleTest COMMAND otbImageIOTestDriver
  otbVectorImageComplexDoubleTest
  ${INPUTDATA}/multibandComplexDouble_2bands.hdr
  )

otb_add_test(NAME ioTvVectorImageComplexFloatTest COMMAND otbImageIOTestDriver
  otbVectorImageComplexFloatTest
  ${INPUTDATA}/multibandComplexFloat_2bands.hdr
  )

otb_add_test(NAME ioTvVectorImageComplexIntoRealFloatTest COMMAND otbImageIOTestDriver
  otbVectorImageComplexIntoRealFloatTest
  ${INPUTDATA}/multibandComplexFloat_2bands.hdr
  )

otb_add_test(NAME ioTvImageComplexFloatTest COMMAND otbImageIOTestDriver
  otbImageComplexFloatTest
  ${INPUTDATA}/multibandComplexFloat_2bands.hdr
  )

otb_add_test(NAME ioTuVectorImageComplexNew COMMAND otbImageIOTestDriver
  otbVectorImageComplexNew)

otb_add_test(NAME ioTvImageComplexDoubleTest COMMAND otbImageIOTestDriver
  otbImageComplexDoubleTest
  ${INPUTDATA}/multibandComplexDouble_2bands.hdr
  )

otb_add_test(NAME ioTuScalarBufferToImageFileWriter COMMAND otbImageIOTestDriver
  otbScalarBufferToImageFileWriterNew )

otb_add_test(NAME ioTvImageFileReaderShortLUM2LUM COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I2.lum
  ${TEMP}/ioImageFileReaderI2_LUM2LUM.lum
  otbShortImageIOTest
  ${INPUTDATA}/poupees_I2.lum
  ${TEMP}/ioImageFileReaderI2_LUM2LUM.lum )

otb_add_test(NAME ioTvImageFileReaderShortLUM2HDR COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I2.lum
  ${TEMP}/ioImageFileReaderI2_LUM2HDR.hdr
  otbShortImageIOTest
  ${INPUTDATA}/poupees_I2.lum
  ${TEMP}/ioImageFileReaderI2_LUM2HDR.hdr )

otb_add_test(NAME ioTvImageFileReaderShortHDR2LUM COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I2.lum
  ${TEMP}/ioImageFileReaderI2_HDR2LUM.lum
  otbShortImageIOTest
  ${TEMP}/ioImageFileReaderI2_LUM2HDR.hdr
  ${TEMP}/ioImageFileReaderI2_HDR2LUM.lum )
set_property(TEST ioTvImageFileReaderShortHDR2LUM PROPERTY DEPENDS ioTvImageFileReaderShortLUM2HDR)

otb_add_test(NAME ioTvImageSeriesFileReader COMMAND otbImageIOTestDriver
  --compare-n-images ${EPSILON_9} 2
  ${BASELINE}/ioTvImageSeries-0.png
  ${TEMP}/ioTvImageSeries-0.png
  ${BASELINE}/ioTvImageSeries-1.png
  ${TEMP}/ioTvImageSeries-1.png
  otbImageSeriesFileReader
  ${EXAMPLEDATA}/GomaAvantApres
  ${TEMP}/ioTvImageSeries.png
  )

if(OTB_DATA_USE_LARGEINPUT)
# This test depends on the output of another test, which depends on large inputs. This situation should not happen.
otb_add_test(NAME ioTvONERAImageStreamWriter COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}
  ${TEMP}/ioImageFileReaderONERAComplexWrite.ent
  ${TEMP}/ioImageFileReaderONERAStreamWrite.ent
  otbImageFileWriterStreamingONERAComplex
  ${TEMP}/ioImageFileReaderONERAComplexWrite.ent
  ${TEMP}/ioImageFileReaderONERAStreamWrite.ent )
set_property(TEST ioTvONERAImageStreamWriter PROPERTY DEPENDS ioTvONERAImageFileWriterComplex)
endif()

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
  --compare-image ${EPSILON_9}  ${BASELINE}/ioTvVectorImageFileReaderWriterJP22TIFOutput
  ${TEMP}/ioTvVectorImageFileReaderWriterJP22TIFOutput.tif
  otbVectorImageFileReaderWriterTest
  LARGEINPUT{JPEG2000/QB_multicomp_small.jpx}
  ${TEMP}/ioTvVectorImageFileReaderWriterJP22TIFOutput.tif
  )

otb_add_test(NAME ioTvImageFileReaderIntBSQ2HDR COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I4.hd
  ${TEMP}/ioImageFileReaderI4_BSQ2HDR.hdr
  otbIntImageIOTest
  ${INPUTDATA}/poupees_I4.hd
  ${TEMP}/ioImageFileReaderI4_BSQ2HDR.hdr )

otb_add_test(NAME ioTvImageFileReaderIntHDR2BSQ COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I4.hd
  ${TEMP}/ioImageFileReaderI4_HDR2BSQ.hd
  otbIntImageIOTest
  ${TEMP}/ioImageFileReaderI4_BSQ2HDR.hdr
  ${TEMP}/ioImageFileReaderI4_HDR2BSQ.hd )
set_property(TEST ioTvImageFileReaderIntHDR2BSQ PROPERTY DEPENDS ioTvImageFileReaderIntBSQ2HDR)

otb_add_test(NAME ioTvONERAImageFileReader COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE}/ioImageFileReaderONERA.hdr
  ${TEMP}/ioImageFileReaderONERA.hdr
  otbImageFileReaderONERATest
  LARGEINPUT{ONERA/spa3_0215_rad.ent}
  ${TEMP}/ioImageFileReaderONERA.hdr
  10 10 100 100 )

otb_add_test(NAME ioTvImageFileReaderFloatHDR2BSQ COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R4.hd
  ${TEMP}/ioImageFileReaderR4_HDR2BSQ.hd
  otbFloatImageIOTest
  ${TEMP}/ioImageFileReaderR4_BSQ2HDR.hdr
  ${TEMP}/ioImageFileReaderR4_HDR2BSQ.hd )
set_property(TEST ioTvImageFileReaderFloatHDR2BSQ PROPERTY DEPENDS ioTvImageFileReaderFloatBSQ2HDR)

otb_add_test(NAME ioTvImageFileReaderFloatLUM2HDR COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R4.lum
  ${TEMP}/ioImageFileReaderR4_LUM2HDR.hdr
  otbFloatImageIOTest
  ${INPUTDATA}/poupees_R4.lum
  ${TEMP}/ioImageFileReaderR4_LUM2HDR.hdr )

otb_add_test(NAME ioTvImageFileReaderFloatBSQ2HDR COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R4.hd
  ${TEMP}/ioImageFileReaderR4_BSQ2HDR.hdr
  otbFloatImageIOTest
  ${INPUTDATA}/poupees_R4.hd
  ${TEMP}/ioImageFileReaderR4_BSQ2HDR.hdr )

otb_add_test(NAME ioTvImageFileReaderFloatPNG2LUM COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}        ${INPUTDATA}/cthead1.png
  ${TEMP}/ioImageFileReaderFloatPNG2LUM.lum
  otbFloatImageIOTest
  ${INPUTDATA}/cthead1.png
  ${TEMP}/ioImageFileReaderFloatPNG2LUM.lum )

otb_add_test(NAME ioTvImageFileReaderFloatHDR2LUM COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R4.lum
  ${TEMP}/ioImageFileReaderR4_HDR2LUM.lum
  otbFloatImageIOTest
  ${TEMP}/ioImageFileReaderR4_LUM2HDR.hdr
  ${TEMP}/ioImageFileReaderR4_HDR2LUM.lum )
set_property(TEST ioTvImageFileReaderFloatHDR2LUM PROPERTY DEPENDS ioTvImageFileReaderFloatLUM2HDR)

otb_add_test(NAME ioTvCheckNbBandsPNGIndexee COMMAND otbImageIOTestDriver
  otbPNGIndexedNbBandsTest
  ${INPUTDATA}/sbuv_indexee.png
  4 )

otb_add_test(NAME ioTvImageFileReaderENVI2PNG COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/cthead1.png
  ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1_2.png
  otbImageFileReaderTest
  ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1.hdr
  ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1_2.png )
set_property(TEST ioTvImageFileReaderENVI2PNG PROPERTY DEPENDS ioTvImageFileWriterPNG2BSQ)

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

otb_add_test(NAME ioTvImageFileReaderDoubleBSQ2HDR COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R8.hd
  ${TEMP}/ioImageFileReaderR8_BSQ2HDR.hdr
  otbDoubleImageIOTest
  ${INPUTDATA}/poupees_R8.hd
  ${TEMP}/ioImageFileReaderR8_BSQ2HDR.hdr )

otb_add_test(NAME ioTvImageFileReaderDoubleHDR2BSQ COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R8.hd
  ${TEMP}/ioImageFileReaderR8_HDR2BSQ.hd
  otbDoubleImageIOTest
  ${TEMP}/ioImageFileReaderR8_BSQ2HDR.hdr
  ${TEMP}/ioImageFileReaderR8_HDR2BSQ.hd )
set_property(TEST ioTvImageFileReaderDoubleHDR2BSQ PROPERTY DEPENDS ioTvImageFileReaderDoubleBSQ2HDR)

otb_add_test(NAME ioTvImageFileReaderDoubleLUM2HDR COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R8.lum
  ${TEMP}/ioImageFileReaderR8_LUM2HDR.hdr
  otbDoubleImageIOTest
  ${INPUTDATA}/poupees_R8.lum
  ${TEMP}/ioImageFileReaderR8_LUM2HDR.hdr )

otb_add_test(NAME ioTvImageFileReaderWriterJP22TIF COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}  ${BASELINE}/ioTvImageFileReaderWriterJP22TIFOutput.tif
  ${TEMP}/ioTvImageFileReaderWriterJP22TIFOutput.tif
  otbDoubleImageIOTest
  LARGEINPUT{JPEG2000/QB_multicomp_small.jpx}
  ${TEMP}/ioTvImageFileReaderWriterJP22TIFOutput.tif
  )

otb_add_test(NAME ioTvImageFileReaderDoubleHDR2LUM COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R8.lum
  ${TEMP}/ioImageFileReaderR8_HDR2LUM.lum
  otbDoubleImageIOTest
  ${TEMP}/ioImageFileReaderR8_LUM2HDR.hdr
  ${TEMP}/ioImageFileReaderR8_HDR2LUM.lum )
set_property(TEST ioTvImageFileReaderDoubleHDR2LUM PROPERTY DEPENDS ioTvImageFileReaderDoubleLUM2HDR)

otb_add_test(NAME ioTvImageFileWriterPNG2PNG COMMAND otbImageIOTestDriver
  --compare-image ${EPSILON_9}   ${INPUTDATA}/cthead1.png
  ${TEMP}/ioImageFileWriterPNG2PNG_cthead1.png
  otbImageFileWriterTest
  ${INPUTDATA}/cthead1.png
  ${TEMP}/ioImageFileWriterPNG2PNG_cthead1.png )

otb_add_test(NAME ioTvImageFileWriterPNG2BSQ COMMAND otbImageIOTestDriver
  otbImageFileWriterTest
  ${INPUTDATA}/cthead1.png
  ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1.hdr )

otb_add_test(NAME ioTuImageIOFactoryNew COMMAND otbImageIOTestDriver
  otbImageIOFactoryNew )

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

