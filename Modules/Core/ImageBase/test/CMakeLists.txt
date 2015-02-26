otb_module_test()

set(OTBImageBaseTests
otbImageBaseTestDriver.cxx
otbFlexibleDistanceWithMissingValue.cxx
otbImageAndVectorImageOperationFilterNew.cxx
otbImagePCAShapeModelEstimatorTest.cxx
otbFunctionToImageFilter.cxx
otbOssimElevManagerTest.cxx
otbExtractROI2.cxx
otbComplexToVectorImageCastFilter.cxx
otbMultiChannelExtractROI.cxx
otbVectorImageTest.cxx
otbExtractROITestMetaData.cxx
otbTestMultiExtractMultiUpdate.cxx
otbExtractROI.cxx
otbFunctionToImageFilterNew.cxx
otbVectorImageToASImageAdaptorNew.cxx
otbImageAndVectorImageOperationFilterTest.cxx
otbImageOfVectorsToMonoChannelExtractROI.cxx
otbImageOfVectorsToMonoChannelExtractROINew.cxx
otbExtractROINew.cxx
otbExtractROI_RGB.cxx
otbComplexToIntensityFilterTest.cxx
otbMultiToMonoChannelExtractROINew.cxx
otbMultiToMonoChannelExtractROI.cxx
otbImageTest.cxx
otbFlexibleDistanceWithMissingValueNew.cxx
otbImageFunctionAdaptor.cxx
otbMultiChannelExtractROINew.cxx
)

add_executable(otbImageBaseTestDriver ${OTBImageBaseTests})
target_link_libraries(otbImageBaseTestDriver ${OTBImageBase-Test_LIBRARIES})
otb_module_target_label(otbImageBaseTestDriver)

# Tests Declaration


otb_add_test(NAME bfTvFlexibleDistanceWithMissingValue COMMAND otbImageBaseTestDriver
  otbFlexibleDistanceWithMissingValue
  0.000001
  )

otb_add_test(NAME bfTuImageAndVectorImageOperationFilterNew COMMAND otbImageBaseTestDriver
  otbImageAndVectorImageOperationFilterNew)

otb_add_test(NAME bfTvItkImagePCAShapeModelEstimatorTest COMMAND otbImageBaseTestDriver
  --compare-n-images ${EPSILON_7} 4
  ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_eigen_vectors1.tif
  ${TEMP}/bfInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator_pc1.tif
  ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_eigen_vectors2.tif
  ${TEMP}/bfInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator_pc2.tif
  ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_eigen_vectors3.tif
  ${TEMP}/bfInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator_pc3.tif
  ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_mean.tif
  ${TEMP}/bfInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator_MEAN.tif
  otbImagePCAShapeModelEstimatorTest
  ${INPUTDATA}/poupees_sub_c1.png
  ${INPUTDATA}/poupees_sub_c2.png
  ${INPUTDATA}/poupees_sub_c3.png
  ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_mean.tif
  ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_eigen_vectors1.tif
  ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_eigen_vectors2.tif
  ${TEMP}/bfITKimagePCAShapeModelEstimatorTest_eigen_vectors3.tif
  )
set_property(TEST bfTvItkImagePCAShapeModelEstimatorTest PROPERTY DEPENDS bfTvInnerProductPCAImageFilterAllsOutputsWithoutUnbiasedEstimator)





otb_add_test(NAME bfTvFunctionToImageFilterTest COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvFunctionToImageFilterTest.tif
  ${TEMP}/bfTvFunctionToImageFilterTest.tif
  otbFunctionToImageFilter
  ${INPUTDATA}/poupees_1canal.hd
  ${TEMP}/bfTvFunctionToImageFilterTest.tif
  2 # radius
  )


otb_add_test(NAME prTvossimElevManagerTest3 COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}  ${BASELINE}/prTvossimElevManagerTest3.tif
  ${TEMP}/prTvossimElevManagerTest3.tif
  otbOssimElevManagerTest
  ${INPUTDATA}/DEM/srtm_directory
  ${TEMP}/prTvossimElevManagerTest3.tif
  -1.8 52   0.02 -0.018 232 422
  )

otb_add_test(NAME prTvossimElevManagerTest COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}  ${BASELINE}/prTvossimElevManagerTest.tif
  ${TEMP}/prTvossimElevManagerTest.tif
  otbOssimElevManagerTest
  ${INPUTDATA}/DEM/srtm_directory
  ${TEMP}/prTvossimElevManagerTest.tif
  6.5
  45.5
  0.002
  -0.002
  500
  500
  )

otb_add_test(NAME coTvExtractROI2 COMMAND otbImageBaseTestDriver
  --compare-image ${NOTOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
  ${TEMP}/coExtractROI2_cthead1_26_97_209_100.png
  otbExtractROI2
  ${INPUTDATA}/cthead1.png
  ${TEMP}/coExtractROI2_cthead1_26_97_209_100.png
  26 97 209 100 )


otb_add_test(NAME bfTuComplexToVectorImageCastFilterTest COMMAND otbImageBaseTestDriver
  --compare-n-images ${NOTOL} 2
  ${BASELINE}/bfTvComplexToVectorImageCastFilterTest.tif
  ${TEMP}/bfTvComplexToVectorImageCastFilterTest.tif
  ${BASELINE}/bfTvComplexToVectorImageCastFilterTest.tif
  ${TEMP}/bfTvVectorComplexToVectorImageCastFilterTest.tif
  otbComplexToVectorImageCastFilterTest
  ${INPUTDATA}/monobandComplexDouble
  ${TEMP}/bfTvComplexToVectorImageCastFilterTest.tif
  ${TEMP}/bfTvVectorComplexToVectorImageCastFilterTest.tif
  )

otb_add_test(NAME bfTuComplexToVectorImageCastFilterNew COMMAND otbImageBaseTestDriver
  otbComplexToVectorImageCastFilterNew
  )

otb_add_test(NAME coTvMultiChannelROI_RGB2RGB_PNG3 COMMAND otbImageBaseTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_321.png
  ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_321.png
  otbMultiChannelExtractROI
  ${INPUTDATA}/couleurs_extrait.png
  ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_321.png
  -startX 300 -startY 10 -sizeX 250 -sizeY 50  -channels 3 2 1  )

otb_add_test(NAME coTvMultiChannelROI_RGB2RGB_PNG2 COMMAND otbImageBaseTestDriver
  --compare-image ${NOTOL}   ${BASELINE}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_123.png
  ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_123.png
  otbMultiChannelExtractROI
  -uchar ${INPUTDATA}/couleurs_extrait.png
  -uchar ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_123.png
  -startX 300 -startY 10 -sizeX 250 -sizeY 50 -channels 1 2 3  )

otb_add_test(NAME coTvMultiChannelROI_RGB2RGB_PNG COMMAND otbImageBaseTestDriver
  --compare-image ${NOTOL}   ${INPUTDATA}/couleurs_extrait.png
  ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG.png
  otbMultiChannelExtractROI
  ${INPUTDATA}/couleurs_extrait.png
  ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG.png
  -channels 1 2 3 )

otb_add_test(NAME coTvMultiChannelROI_5c_BSQ COMMAND otbImageBaseTestDriver
  --compare-image ${NOTOL} ${BASELINE}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_12321.hdr
  ${TEMP}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_12321.hdr
  otbMultiChannelExtractROI
  ${INPUTDATA}/poupees.hdr
  ${TEMP}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_12321.hdr
  -startX 303 -startY 2 -sizeX 134  -sizeY 330 -channels 1 2 3 2 1 )

otb_add_test(NAME coTvMultiChannelROI_RGB2NG_PNG1 COMMAND otbImageBaseTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
  ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
  otbMultiChannelExtractROI
  ${INPUTDATA}/couleurs_extrait.png
  ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
  -startX 300 -startY 10 -sizeX 250 -sizeY 50 -channels 1 )

otb_add_test(NAME coTvMultiChannelROI_RGB2NG_PNG2 COMMAND otbImageBaseTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_2.png
  ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_2.png
  otbMultiChannelExtractROI
  ${INPUTDATA}/couleurs_extrait.png
  ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_2.png
  -startX 300 -startY 10 -sizeX 250 -sizeY 50  -channels 2 )

otb_add_test(NAME coTvMultiChannelROI_RGB2NG_PNG3 COMMAND otbImageBaseTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_3.png
  ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_3.png
  otbMultiChannelExtractROI
  ${INPUTDATA}/couleurs_extrait.png
  ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_3.png
  -startX 300 -startY 10 -sizeX 250 -sizeY 50  -channels 3 )

otb_add_test(NAME coTvMultiChannelROI_3c_BSQ COMMAND otbImageBaseTestDriver
  --compare-image  ${NOTOL}  ${BASELINE}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_345.hdr
  ${TEMP}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_345.hdr
  otbMultiChannelExtractROI
  ${TEMP}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_12321.hdr
  ${TEMP}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_345.hdr
  -startX 0 -startY 0 -sizeX 134  -sizeY 330 -channels 3 4 5 )
set_property(TEST coTvMultiChannelROI_3c_BSQ PROPERTY DEPENDS coTvMultiChannelROI_5c_BSQ)


otb_add_test(NAME ioTvMultiDatasetReading2 COMMAND otbImageBaseTestDriver
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiDatasetReading2.txt
  ${TEMP}/ioTvMultiDatasetReading2.txt
  otbVectorImageTest
  ${INPUTDATA}/MOD09Q1G_EVI.A2006233.h07v03.005.2008338190308.hdf?&sdataidx=5
  ${TEMP}/ioTvMultiDatasetReading2.txt
  )

otb_add_test(NAME ioTvMultiDatasetReading1 COMMAND otbImageBaseTestDriver
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiDatasetReading1.txt
  ${TEMP}/ioTvMultiDatasetReading1.txt
  otbVectorImageTest
  ${INPUTDATA}/MOD09Q1G_EVI.A2006233.h07v03.005.2008338190308.hdf?&sdataidx=0
  ${TEMP}/ioTvMultiDatasetReading1.txt
  )

otb_add_test(NAME coTvExtractROITestMetaData_TIFF COMMAND otbImageBaseTestDriver
  --compare-ascii ${NOTOL}
  ${TEMP}/coTvExtractROITestMetaData1.txt
  ${TEMP}/coTvExtractROITestMetaData2.txt
  --ignore-lines-with 1 Origin:
  otbExtractROITestMetaData
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  ${TEMP}/coTvExtractROITestMetaData1.tif
  ${TEMP}/coTvExtractROITestMetaData2.tif
  ${TEMP}/coTvExtractROITestMetaData1.txt
  ${TEMP}/coTvExtractROITestMetaData2.txt
  )

otb_add_test(NAME coTvExtractROITestMetaData_HDR COMMAND otbImageBaseTestDriver
  --compare-ascii ${NOTOL}
  ${TEMP}/coTvExtractROITestMetaData1_hdr.txt
  ${TEMP}/coTvExtractROITestMetaData2_hdr.txt
  --ignore-lines-with 1 Origin:
  otbExtractROITestMetaData
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  ${TEMP}/coTvExtractROITestMetaData1.hdr
  ${TEMP}/coTvExtractROITestMetaData2.hdr
  ${TEMP}/coTvExtractROITestMetaData1_hdr.txt
  ${TEMP}/coTvExtractROITestMetaData2_hdr.txt
  )

otb_add_test(NAME coTvMultiChannelExtractROI_Romania COMMAND otbImageBaseTestDriver
  --compare-image  ${NOTOL}
  ${BASELINE}/coTvMultiChannelExtractROI_Romania.tif
  ${TEMP}/coTvMultiChannelExtractROI_Romania.tif
  otbTestMultiExtractMultiUpdate
  ${INPUTDATA}/Romania_Extract.tif
  ${TEMP}/coTvMultiChannelExtractROI_Romania.tif
  0 # startX
  0 # startY
  100 #sizeX
  100 #sizeY
  2 # first channel
  3 # last channel
  )

otb_add_test(NAME coTvTestMultiExtractMultiUpdate COMMAND otbImageBaseTestDriver
  --compare-image  ${NOTOL}
  ${BASELINE}/coTvMultiExtractMultiUpdate.tif
  ${TEMP}/coTvMultiExtractMultiUpdate.tif
  otbTestMultiExtractMultiUpdate
  ${INPUTDATA}/poupees.tif
  ${TEMP}/coTvMultiExtractMultiUpdate.tif
  70 # startX
  60 # startY
  50 #sizeX
  50 #sizeY
  1 # first channel
  3 # last channel
  )

otb_add_test(NAME coTvExtractROI COMMAND otbImageBaseTestDriver
  --compare-image ${NOTOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
  ${TEMP}/coExtractROI_cthead1_26_97_209_100.png
  otbExtractROI
  ${INPUTDATA}/cthead1.png
  ${TEMP}/coExtractROI_cthead1_26_97_209_100.png
  26 97 209 100 )

otb_add_test(NAME coTvExtractROIQB COMMAND otbImageBaseTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coTvExtractROIQB.png
  ${TEMP}/coTvExtractROIQB.png
  otbExtractROI
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/coTvExtractROIQB.png
  0 0 70 70
  )

otb_add_test(NAME bfTuFunctionToImageFilterNew COMMAND otbImageBaseTestDriver
  otbFunctionToImageFilterNew
  )



otb_add_test(NAME coTuVectorImageToASImageAdaptorNew COMMAND otbImageBaseTestDriver
  otbVectorImageToASImageAdaptorNew
  )


otb_add_test(NAME bfTvImageAndVectorImageOperationFilterTest COMMAND otbImageBaseTestDriver
  --compare-n-images ${NOTOL} 4
  ${BASELINE}/bfTvImageAndVectorImageOperationFilterTestAdd.tif
  ${TEMP}/bfTvImageAndVectorImageOperationFilterTestAdd.tif
  ${BASELINE}/bfTvImageAndVectorImageOperationFilterTestSub.tif
  ${TEMP}/bfTvImageAndVectorImageOperationFilterTestSub.tif
  ${BASELINE}/bfTvImageAndVectorImageOperationFilterTestMul.tif
  ${TEMP}/bfTvImageAndVectorImageOperationFilterTestMul.tif
  ${BASELINE}/bfTvImageAndVectorImageOperationFilterTestDiv.tif
  ${TEMP}/bfTvImageAndVectorImageOperationFilterTestDiv.tif
  otbImageAndVectorImageOperationFilterTest
  ${INPUTDATA}/checkerboard_128_128.png
  ${INPUTDATA}/poupees_sub.png
  ${TEMP}/bfTvImageAndVectorImageOperationFilterTestAdd.tif
  ${TEMP}/bfTvImageAndVectorImageOperationFilterTestSub.tif
  ${TEMP}/bfTvImageAndVectorImageOperationFilterTestMul.tif
  ${TEMP}/bfTvImageAndVectorImageOperationFilterTestDiv.tif
  )

otb_add_test(NAME coTvImageOfVectorsToMonoChannelExtractROI COMMAND otbImageBaseTestDriver
  --compare-image ${NOTOL}   ${BASELINE}/coImageOfVectorsToMonoChannelExtractROI.png
  ${TEMP}/coImageOfVectorsToMonoChannelExtractROI.png
  otbImageOfVectorsToMonoChannelExtractROI
  ${INPUTDATA}/couleurs_extrait.png
  300 10  # Start X Y
  250 50  # Size X Y
  1       # Channel
  ${TEMP}/coImageOfVectorsToMonoChannelExtractROI.png
  )


otb_add_test(NAME coTuImageOfVectorsToMonoChannelExtractROINew COMMAND otbImageBaseTestDriver
  otbImageOfVectorsToMonoChannelExtractROINew
  )



otb_add_test(NAME coTuExtractROINew COMMAND otbImageBaseTestDriver
  otbExtractROINew)


otb_add_test(NAME coTvExtractROI_RGB COMMAND otbImageBaseTestDriver
  --compare-image ${NOTOL}   ${BASELINE}/coExtractROI_RGB_300_10_200_50.png
  ${TEMP}/coExtractROI_RGB_300_10_200_50.png
  otbExtractROI_RGB
  ${INPUTDATA}/couleurs_extrait.png
  ${TEMP}/coExtractROI_RGB_300_10_200_50.png
  300 10 200 50 )

otb_add_test(NAME coTvExtractROI_QB COMMAND otbImageBaseTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coTvExtractROI_QB.png
  ${TEMP}/coTvExtractROI_QB.png
  otbExtractROI_RGB
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/coTvExtractROI_QB.png
  0 0 70 70
  )

otb_add_test(NAME bfTvComplexToIntensityFilterTest COMMAND otbImageBaseTestDriver
  otbComplexToIntensityFilterTest)


otb_add_test(NAME coTuMultiToMonoROINew COMMAND otbImageBaseTestDriver
  otbMultiToMonoChannelExtractROINew)

otb_add_test(NAME coTvMultiToMonoROI_RGB2NG_PNG COMMAND otbImageBaseTestDriver
  --compare-image ${NOTOL}   ${BASELINE}/coMultiToMonoChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
  ${TEMP}/coMultiToMonoChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
  otbMultiToMonoChannelExtractROI
  ${INPUTDATA}/couleurs_extrait.png
  ${TEMP}/coMultiToMonoChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
  -startX 300 -startY 10 -sizeX 250 -sizeY 50 -channel 1 )

otb_add_test(NAME ioTvOtbImageTestSAR_ERS COMMAND otbImageBaseTestDriver
  --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/ioOtbImageTestSAR_ERS.txt
  ${TEMP}/ioOtbImageTestSAR_ERS.txt
  otbImageTest
  LARGEINPUT{SAR_ERS2_SLCI_SCENE1/}
  ${TEMP}/ioOtbImageTestSAR_ERS.txt)

otb_add_test(NAME ioTvOtbImageTestRadarsat COMMAND otbImageBaseTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/ioOtbImageTestRadarsat.txt
  ${TEMP}/ioOtbImageTestRadarsat.txt
  otbImageTest
  LARGEINPUT{RADARSAT1/GOMA/SCENE01/}
  ${TEMP}/ioOtbImageTestRadarsat.txt)

otb_add_test(NAME bfTuFlexibleDistanceWithMissingValueNew COMMAND otbImageBaseTestDriver
  otbFlexibleDistanceWithMissingValueNew)

otb_add_test(NAME feTvImageFunctionAdaptor COMMAND otbImageBaseTestDriver
  otbImageFunctionAdaptor
  ${INPUTDATA}/poupees.png
  )

otb_add_test(NAME feTuImageFunctionAdaptorNew COMMAND otbImageBaseTestDriver
  otbImageFunctionAdaptorNew
  )


otb_add_test(NAME coTuMultiChannelROINew COMMAND otbImageBaseTestDriver
  otbMultiChannelExtractROINew)

