otb_module_test()

set(OTBImageBaseTests
  otbImageBaseTestDriver.cxx
  otbFlexibleDistanceWithMissingValue.cxx
  otbImageAndVectorImageOperationFilterNew.cxx
  otbImagePCAShapeModelEstimatorTest.cxx
  otbFunctionToImageFilter.cxx
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
  otbMetaImageFunction.cxx

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

otb_add_test(NAME coTvExtractROI2 COMMAND otbImageBaseTestDriver
  --compare-image ${NOTOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
  ${TEMP}/coExtractROI2_cthead1_26_97_209_100.png
  otbExtractROI2
  ${INPUTDATA}/cthead1.png
  ${TEMP}/coExtractROI2_cthead1_26_97_209_100.png
  26 97 209 100 )


otb_add_test(NAME bfTvComplexToVectorImageCastFilterTest COMMAND otbImageBaseTestDriver
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

if(GDAL_HAS_HDF4)
  otb_add_test(NAME ioTvMultiDatasetReading2 COMMAND otbImageBaseTestDriver
    --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiDatasetReading2.txt
    ${TEMP}/ioTvMultiDatasetReading2.txt
    --ignore-lines-with 1 Pointer:
    otbVectorImageTest
    ${INPUTDATA}/MOD09Q1G_EVI.A2006233.h07v03.005.2008338190308.hdf?&sdataidx=5
    ${TEMP}/ioTvMultiDatasetReading2.txt
    )

  otb_add_test(NAME ioTvMultiDatasetReading1 COMMAND otbImageBaseTestDriver
    --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiDatasetReading1.txt
    ${TEMP}/ioTvMultiDatasetReading1.txt
    --ignore-lines-with 1 Pointer:
    otbVectorImageTest
    ${INPUTDATA}/MOD09Q1G_EVI.A2006233.h07v03.005.2008338190308.hdf?&sdataidx=0
    ${TEMP}/ioTvMultiDatasetReading1.txt
    )
endif()

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

otb_add_test(NAME feTvMetaImageFunction COMMAND otbImageBaseTestDriver
  --compare-ascii ${EPSILON_8}
  ${BASELINE_FILES}/feTvMetaImageFunction.txt
  ${TEMP}/feTvMetaImageFunction.txt
  otbMetaImageFunction
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/feTvMetaImageFunction.txt
  451846.014047961 5412466.57452216
  )
otb_add_test(NAME feTuMetaImageFunctionNew COMMAND otbImageBaseTestDriver
  otbMetaImageFunctionNew
  )

if(OTB_DATA_USE_LARGEINPUT)
  set( GenericTestPHR_TESTNB 0)

  foreach( current_file ${GenericTestPHR_DATA_INPUTS} )
    list(GET GenericTestPHR_DATA_TYPES   ${GenericTestPHR_TESTNB} current_type   )
    math(EXPR GenericTestPHR_TESTNB "${GenericTestPHR_TESTNB} + 1")
    string(REGEX MATCH "TIFF" PHR_TIFF_FILE ${current_type})

    # Read info from the input file
    #otb_add_test(NAME ioTvVectorImageReadingInfo_${current_type} COMMAND otbImageBaseTestDriver
    #--ignore-order --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioTvMultiResolutionReading_${current_type}.txt
    #${TEMP}/ioTvMultiResolutionReading_${current_type}_OUT.txt
    #--ignore-lines-with 4 CacheSizeInBytes SubDatasetIndex Driver Corner 
    #otbVectorImageTest
    #${current_file}?&resol=0
    #${TEMP}/ioTvMultiResolutionReading_${current_type}_OUT.txt
    #)

    if(NOT PHR_TIFF_FILE)
      # Read info from the input file (res=5)
      #otb_add_test(NAME ioTvVectorImageReadingInfo_${current_type}_res5 COMMAND otbImageBaseTestDriver
      #--ignore-order --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioTvMultiResolutionReading_${current_type}_res5.txt
      #${TEMP}/ioTvMultiResolutionReading_${current_type}_res5_OUT.txt
      #--ignore-lines-with 4 CacheSizeInBytes Driver Corner SubDatasetIndex
      #otbVectorImageTest
      #${current_file}?&resol=5
      #${TEMP}/ioTvMultiResolutionReading_${current_type}_res5_OUT.txt
      #)
      # Decode an area inside one tile
      otb_add_test(NAME ioTvReaderWriterJP22TIF_${current_type}_1 COMMAND otbImageBaseTestDriver
	--compare-image ${EPSILON_9}  ${BASELINE}/ioTvJP22TIF_Extract_${current_type}_1.tif
        ${TEMP}/ioTvJP22TIF_Extract_${current_type}_1_OUT.tif
        otbMultiChannelExtractROI
        -ushort ${current_file}
        -ushort ${TEMP}/ioTvJP22TIF_Extract_${current_type}_1_OUT.tif
        -startX 256 -startY 512 -sizeX 256 -sizeY 128
        -time
        )

      # Decode an area which need 4 tiles
      otb_add_test(NAME ioTvReaderWriterJP22TIF_${current_type}_2 COMMAND otbImageBaseTestDriver
        --compare-image ${EPSILON_9}  ${BASELINE}/ioTvJP22TIF_Extract_${current_type}_2.tif
        ${TEMP}/ioTvJP22TIF_Extract_${current_type}_2_OUT.tif
        otbMultiChannelExtractROI
        -ushort ${current_file}
        -ushort ${TEMP}/ioTvJP22TIF_Extract_${current_type}_2_OUT.tif
        -startX 2000 -startY 2000 -sizeX 256 -sizeY 128
        -time
        )

      # Decode the whole image with the lowest resolution (res=5)
      # (take only one channel to put the file in baseline repository)
      #write only a subpart of the output image using the extended parameter "box"
      otb_add_test(NAME ioTvReaderWriterJP22TIF_${current_type}_res5 COMMAND otbImageBaseTestDriver
        --compare-image ${EPSILON_9}  ${BASELINE}/ioTvJP22TIF_Extract_${current_type}_res5.tif
        ${TEMP}/ioTvJP22TIF_Extract_${current_type}_res5_OUT.tif
        otbMultiChannelExtractROI
        -ushort ${current_file}?resol=5
        -ushort ${TEMP}/ioTvJP22TIF_Extract_${current_type}_res5_OUT.tif?&box=0:0:128:128
        -channels 1
        -time
        )

    endif()

  endforeach()

endif()

# ---  SPOT4 (CAI)  ---
otb_add_test(NAME  ioTvMultiChannelROI_SPOT42PNG COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
  ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
  otbMultiChannelExtractROI
  LARGEINPUT{SPOT4/RIO_DE_JANEIRO/}
  ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
  -startX 700 -startY 60 -sizeX 77 -sizeY 489
  -channels 1 2 3 )

otb_add_test(NAME  ioTvMultiChannelROI_SPOT42PNG_2 COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
  ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
  otbMultiChannelExtractROI
  LARGEINPUT{SPOT4/RIO_DE_JANEIRO/}
  ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
  -startX 700 -startY 60 -sizeX 77 -sizeY 489
  -channels 3 2 1 )

# ---  ENVI  ---
otb_add_test(NAME  ioTvMultiChannelROI_ENVI2PNG_2 COMMAND otbImageBaseTestDriver
  # THOMAS (provisoire) : Images OK mais erreur au DIFF ITK !!!
  #   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_ENVI2PNG_poupees_100_100_77_300_channels_1_2_3.png
  #                     ${TEMP}/ioExtractROI_ENVI2PNG_poupees_100_100_77_300_channels_1_2_3.png
  otbMultiChannelExtractROI
  -uchar ${INPUTDATA}/poupees.hdr
  -uchar ${TEMP}/ioExtractROI_ENVI2PNG_poupees_100_100_77_300_channels_1_2_3.png
  -startX 100 -startY 100 -sizeX 77 -sizeY 300
  -channels 1 2 3 )

# ---  SPOT5 BIL (CAI)  ---


# add_test(ioTvMultiChannelROI_SPOT5BIL2PNG_2COMMAND otbImageBaseTestDriver
#   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT5BIL2PNG_poupees_100_100_77_300_channels_1_2_3.png
#                     ${TEMP}/ioExtractROI_SPOT5BIL2PNG_poupees_100_100_77_300_channels_1_2_3.png
#         otbMultiChannelExtractROI
#         ${INPUTDATA}/poupeesBIL
#         ${TEMP}/ioExtractROI_SPOT5BIL2PNG_poupees_100_100_77_300_channels_1_2_3.png
#         -startX 100 -startY 100 -sizeX 77 -sizeY 300
#         -channels 1 2 3 )

# ---  SPOT5 (CAI)  ---
# ---  Test sur l'image SPOT 5 4 canaux  ---

otb_add_test(NAME   ioTvMultiChannelROI_SPOT52PNG COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channels_1_2_3.png
  ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channels_1_2_3.png
  otbMultiChannelExtractROI
  LARGEINPUT{SPOT5/TEHERAN/}
  ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channels_1_2_3.png
  -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
  -channels 1 2 3 )

otb_add_test(NAME   ioTvMultiChannelROI_SPOT52PNG_2 COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
  ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
  otbMultiChannelExtractROI
  LARGEINPUT{SPOT5/TEHERAN/}
  ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
  -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
  -channels 1 )

otb_add_test(NAME   ioTvMultiChannelROI_SPOT52PNG_3 COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
  ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
  otbMultiChannelExtractROI
  LARGEINPUT{SPOT5/TEHERAN/}
  ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
  -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
  -channels 2 )

otb_add_test(NAME   ioTvMultiChannelROI_SPOT52PNG_4 COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
  ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
  otbMultiChannelExtractROI
  LARGEINPUT{SPOT5/TEHERAN/}
  ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
  -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
  -channels 3 )

otb_add_test(NAME   ioTvMultiChannelROI_SPOT52PNG_5 COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
  ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
  otbMultiChannelExtractROI
  LARGEINPUT{SPOT5/TEHERAN/}
  ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
  -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
  -channels 4 )

# ---  Ikonos PAN  ---
otb_add_test(NAME   ioTvMultiChannelROI_IkonosPAN2PNG COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosPAN2PNG_100_100_77_300.png
  ${TEMP}/ioExtractROI_IkonosPAN2PNG_100_100_77_300.png
  otbMultiChannelExtractROI
  LARGEINPUT{IKONOS/PARIS/po_79039_pan_0000000.tif}
  ${TEMP}/ioExtractROI_IkonosPAN2PNG_100_100_77_300.png
  -startX 100 -startY 100 -sizeX 77 -sizeY 300
  -channels 1)

# ---  Ikonos XS  ---
otb_add_test(NAME   ioTvMultiChannelROI_IkonosRED2PNG COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosRED2PNG_100_100_77_300.png
  ${TEMP}/ioExtractROI_IkonosRED2PNG_100_100_77_300.png
  otbMultiChannelExtractROI
  LARGEINPUT{IKONOS/PARIS/po_79039_red_0000000.tif}
  ${TEMP}/ioExtractROI_IkonosRED2PNG_100_100_77_300.png
  -startX 100 -startY 100 -sizeX 77 -sizeY 300
  -channels 1)

otb_add_test(NAME   ioTvMultiChannelROI_IkonosGRN2PNG COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosGRN2PNG_100_100_77_300.png
  ${TEMP}/ioExtractROI_IkonosGRN2PNG_100_100_77_300.png
  otbMultiChannelExtractROI
  LARGEINPUT{IKONOS/PARIS/po_79039_grn_0000000.tif}
  ${TEMP}/ioExtractROI_IkonosGRN2PNG_100_100_77_300.png
  -startX 100 -startY 100 -sizeX 77 -sizeY 300
  -channels 1)

otb_add_test(NAME   ioTvMultiChannelROI_IkonosBLU2PNG COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosBLU2PNG_100_100_77_300.png
  ${TEMP}/ioExtractROI_IkonosBLU2PNG_100_100_77_300.png
  otbMultiChannelExtractROI
  LARGEINPUT{IKONOS/PARIS/po_79039_blu_0000000.tif}
  ${TEMP}/ioExtractROI_IkonosBLU2PNG_100_100_77_300.png
  -startX 100 -startY 100 -sizeX 77 -sizeY 300
  -channels 1)

otb_add_test(NAME   ioTvMultiChannelROI_IkonosNIR2PNG COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
  ${TEMP}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
  otbMultiChannelExtractROI
  LARGEINPUT{IKONOS/PARIS/po_79039_nir_0000000.tif}
  ${TEMP}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
  -startX 100 -startY 100 -sizeX 77 -sizeY 300
  -channels 1)

# ---  LUM ----
otb_add_test(NAME   ioTvMultiChannelROI_IkonosNIR2LUM COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosNIR2PNG_100_100_77_300_nogeoinfo.png
  ${TEMP}/ioExtractROI_IkonosNIR2LUM_100_100_77_300.lum
  otbMultiChannelExtractROI
  LARGEINPUT{IKONOS/PARIS/po_79039_nir_0000000.tif}
  ${TEMP}/ioExtractROI_IkonosNIR2LUM_100_100_77_300.lum
  -startX 100 -startY 100 -sizeX 77 -sizeY 300
  -channels 1)

# ---  BSQ ----
otb_add_test(NAME   ioTvMultiChannelROI_IkonosNIR2BSQ COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}       ${BASELINE}/ioExtractROI_IkonosNIR2PNG_100_100_77_300_nogeoinfo.png
  ${TEMP}/ioExtractROI_IkonosNIR2BSQ_100_100_77_300.hd
  otbMultiChannelExtractROI
  LARGEINPUT{IKONOS/PARIS/po_79039_nir_0000000.tif}
  ${TEMP}/ioExtractROI_IkonosNIR2BSQ_100_100_77_300.hd
  -startX 100 -startY 100 -sizeX 77 -sizeY 300
  -channels 1)

# ---  Quickbird PAN  ---
otb_add_test(NAME   ioTvMultiChannelROI_QuickbirdPAN2TIFF COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_QuickbirdPAN2TIFF_1000_1000_100_150.tif
  ${TEMP}/ioExtractROI_QuickbirdPAN2TIFF_1000_1000_100_150.tif
  otbMultiChannelExtractROI
  -ushort LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  -ushort ${TEMP}/ioExtractROI_QuickbirdPAN2TIFF_1000_1000_100_150.tif
  -startX 1000 -startY 1000 -sizeX 100 -sizeY 150
  )

# ---  Quickbird XS  ---
otb_add_test(NAME   ioTvMultiChannelROI_QuickbirdXS2TIFF COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_QuickbirdXS2TIFF_1000_1000_100_150.tif
  ${TEMP}/ioExtractROI_QuickbirdXS2TIFF_1000_1000_100_150.tif
  otbMultiChannelExtractROI
  -ushort LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
  -ushort ${TEMP}/ioExtractROI_QuickbirdXS2TIFF_1000_1000_100_150.tif
  -startX 1000 -startY 1000 -sizeX 100 -sizeY 150
  )

if(GDAL_HAS_HDF4)
  # Tests which read data in HDF files.
  otb_add_test(NAME   ioTvMultiChannelROI_HDF4_2_TIF COMMAND otbImageBaseTestDriver
    --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_HDF2TIF_MOD09Q1G_20_25_100_150_channel_1.tif
    ${TEMP}/ioExtractROI_HDF2TIF_MOD09Q1G_20_25_100_150_channel_1_OUT.tif
    otbMultiChannelExtractROI
    ${INPUTDATA}/MOD09Q1G_EVI.A2006233.h07v03.005.2008338190308.hdf?&sdataidx=5
    ${TEMP}/ioExtractROI_HDF2TIF_MOD09Q1G_20_25_100_150_channel_1_OUT.tif
    -startX 1400 -startY 3700 -sizeX 100 -sizeY 150
    -channels 1 )
endif()

if(GDAL_HAS_HDF5)
  otb_add_test(NAME   ioTvMultiChannelROI_HDF5_2_TIF COMMAND otbImageBaseTestDriver
    --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_HDF2TIF_GSSTF_NCEP_100_20_200_100_channel_1.tif
    ${TEMP}/ioExtractROI_HDF2TIF_GSSTF_NCEP_100_20_200_100_channel_1_OUT.tif
    otbMultiChannelExtractROI
    ${INPUTDATA}/GSSTF_NCEP.2b.2008.12.31.he5?&sdataidx=3
    ${TEMP}/ioExtractROI_HDF2TIF_GSSTF_NCEP_100_20_200_100_channel_1_OUT.tif
    -startX 100 -startY 20 -sizeX 200 -sizeY 100
    -channels 1 )
endif()

# Read an area inside one tile at resolution 0 (jpeg2000 conformance file with
# specific tile size at different resolution).
otb_add_test(NAME   ioTvMultiChannelROI_p1_06_JPEG2000_2_TIF_res0 COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_p1_06.tif
  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_p1_06_OUT.tif
  otbMultiChannelExtractROI
  ${INPUTDATA}/jpeg2000_conf_p1_06.j2k?&resol=0
  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_p1_06_OUT.tif
  )

# Only do the following test if we are using the OTB Openjpeg driver,
# because gdal does not report jpeg2000 overview with size < 128
# pixel, and will therefore interpolate.
if(OTB_USE_OPENJPEG)

# Read an area inside one tile at resolution 4 (jpeg2000 conformance file with
# specific tile size at different resolution).
otb_add_test(NAME   ioTvMultiChannelROI_p1_06_JPEG2000_2_TIF_res4 COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_p1_06_res4.tif
  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_p1_06_res4_OUT.tif
  otbMultiChannelExtractROI
  ${INPUTDATA}/jpeg2000_conf_p1_06.j2k?&resol=4
  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_p1_06_res4_OUT.tif
  )

endif()

# Read an area inside one tile at resolution 0 (quite similar coding parameter with pleiade
# except it a lossless image with no quatization style).
otb_add_test(NAME   ioTvMultiChannelROI_lena_JPEG2000_2_TIF_res0 COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_lena_150_75_100_50.tif
  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_lena_150_75_100_50_OUT.tif
  otbMultiChannelExtractROI
  ${INPUTDATA}/lena512color.jp2?&resol=0
  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_lena_150_75_100_50_OUT.tif
  -startX 150 -startY 75 -sizeX 100 -sizeY 50 )

# Read an area inside one tile at resolution 5 (quite similar coding parameter with pleiade
# except it a lossless image with no quatization style).
otb_add_test(NAME   ioTvMultiChannelROI_lena_JPEG2000_2_TIF_res1 COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_lena_res1.tif
  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_lena_res1_OUT.tif
  otbMultiChannelExtractROI
  ${INPUTDATA}/lena512color.jp2?&resol=1
  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_lena_res1_OUT.tif
  )


# Read an area inside one tile at resolution 0
otb_add_test(NAME   ioTvMultiChannelROI_JPEG2000_2_TIF_res0_1 COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_bretagne_700_1000_50_40.tif
  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_700_1000_50_40_OUT.tif
  otbMultiChannelExtractROI
  ${INPUTDATA}/bretagne.j2k?&resol=0
  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_700_1000_50_40_OUT.tif
  -startX 700 -startY 1000 -sizeX 50 -sizeY 40 )

# Read an area which need to decode 4 tiles at resolution 0
otb_add_test(NAME   ioTvMultiChannelROI_JPEG2000_2_TIF_res0_2 COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_bretagne_1260_950_40_20.tif
  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_1260_950_40_20_OUT.tif
  otbMultiChannelExtractROI
  ${INPUTDATA}/bretagne.j2k?&resol=0
  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_1260_950_40_20_OUT.tif
  -startX 1260 -startY 950 -sizeX 40 -sizeY 20 )

# Read all the image with a resolution 3
otb_add_test(NAME   ioTvMultiChannelROI_JPEG2000_2_TIF_res3 COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3.tif
  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_OUT.tif
  otbMultiChannelExtractROI
  ${INPUTDATA}/bretagne.j2k?&resol=3
  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_OUT.tif )

# Read area inside one tile at resolution 3
otb_add_test(NAME   ioTvMultiChannelROI_JPEG2000_2_TIF_res3_1 COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_90_70_20_30.tif
  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_90_70_20_30_OUT.tif
  otbMultiChannelExtractROI
  ${INPUTDATA}/bretagne.j2k?&resol=3
  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_90_70_20_30_OUT.tif
  -startX 90 -startY 70 -sizeX 20 -sizeY 30 )

# Read an area which need to decode 4 tiles at resolution 3
otb_add_test(NAME   ioTvMultiChannelROI_JPEG2000_2_TIF_res3_2 COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_40_30_80_60.tif
  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_40_30_80_60_OUT.tif
  otbMultiChannelExtractROI
  ${INPUTDATA}/bretagne.j2k?&resol=3
  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_40_30_80_60_OUT.tif
  -startX 40 -startY 30 -sizeX 80 -sizeY 60 )

# ---  RADARSAT (GDAL)  ---
otb_add_test(NAME  ioTvMultiMonoChannelROI_RADARSAT2ENVI COMMAND otbImageBaseTestDriver
  otbMultiToMonoChannelExtractROI
  -ushort LARGEINPUT{RADARSAT1/GOMA/SCENE01/}
  -ushort ${TEMP}/ioMultiMonoChannelROI_RADARSAT2ENVI_2559_3591_337_280.hdr
  -startX 2559 -startY 3591 -sizeX 337 -sizeY 280 )

# FIXME this test depends on an other test which requires largeinput. Need to circle the test with it(otb_data_use_large_input). It should not happen
if(OTB_DATA_USE_LARGEINPUT)
  # ---  RADARSAT (GDAL)  dir name ---
  otb_add_test(NAME  ioTvMultiMonoChannelROI_RADARSAT_ENVI2ENVI COMMAND otbImageBaseTestDriver
    # THOMAS (provisoire) : Images OK mais erreur au DIFF ITK !!!
    #   --compare-image ${EPSILON_9} ${BASELINE}/ioMultiMonoChannelROI_RADARSAT2ENVI_2559_3591_337_280.hdr
    #                      ${TEMP}/ioMultiMonoChannelROI_RADARSAT_ENVI2ENVI_2559_3591_337_280_2.hdr
    otbMultiToMonoChannelExtractROI
    -ushort ${TEMP}/ioMultiMonoChannelROI_RADARSAT2ENVI_2559_3591_337_280.hdr
    -ushort ${TEMP}/ioMultiMonoChannelROI_RADARSAT_ENVI2ENVI_2559_3591_337_280_2.hdr
    -startX 0 -startY 0 -sizeX ParseHdfSubsetName337 -sizeY 280 )
  
  set_tests_properties(ioTvMultiMonoChannelROI_RADARSAT_ENVI2ENVI PROPERTIES DEPENDS ioTvMultiMonoChannelROI_RADARSAT2ENVI)
endif()

# ---  ENVI (GDAL) ---
otb_add_test(NAME  ioTvMultiMonoChannelROI_ENVI2PNG COMMAND otbImageBaseTestDriver
  --compare-image ${EPSILON_9}   ${BASELINE}/ioMultiMonoChannelROI_ENVI2PNG_poupees_1canal_302_2_134_330.png
  ${TEMP}/ioMultiMonoChannelROI_ENVI2PNG_poupees_1canal_302_2_134_330.png
  otbMultiToMonoChannelExtractROI
  ${INPUTDATA}/poupees_1canal.c1.hdr
  ${TEMP}/ioMultiMonoChannelROI_ENVI2PNG_poupees_1canal_302_2_134_330.png
  -startX 302 -startY 2 -sizeX 134 -sizeY 330
  -channel 1)

# Tests to check if we can open JPEG2000 files specifying the resolution factor.
# no access to data only PrintSelf() method.
otb_add_test(NAME  ioTvMultiResolutionReading0 COMMAND otbImageBaseTestDriver
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiResolutionReading0.txt
  ${TEMP}/ioTvMultiResolutionReading0.txt
  --ignore-lines-with 7 VectorImage Source PipelineMTime ImportImageContainer Pointer CacheSizeInBytes SubDatasetIndex
  otbVectorImageTest
  ${INPUTDATA}/bretagne.j2k?&resol=0
  ${TEMP}/ioTvMultiResolutionReading0.txt
  )

otb_add_test(NAME  ioTvMultiResolutionReading3 COMMAND otbImageBaseTestDriver
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiResolutionReading3.txt
  ${TEMP}/ioTvMultiResolutionReading3.txt
  --ignore-lines-with 7 VectorImage Source PipelineMTime ImportImageContainer Pointer CacheSizeInBytes SubDatasetIndex
  otbVectorImageTest
  ${INPUTDATA}/bretagne.j2k?&resol=3
  ${TEMP}/ioTvMultiResolutionReading3.txt
  )

otb_add_test(NAME ioTvOtbVectorImageTestSpot5 COMMAND otbImageBaseTestDriver
  --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/ioOtbVectorImageTestSpot5.txt
  ${TEMP}/ioOtbVectorImageTestSpot5.txt
  otbVectorImageLegacyTest
  LARGEINPUT{/SPOT5/TEHERAN/}
  ${TEMP}/ioOtbVectorImageTestSpot5.txt)

otb_add_test(NAME ioTvOtbVectorImageTestFORMOSAT2 COMMAND otbImageBaseTestDriver
  --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/ioOtbVectorImageTestFORMOSAT2.txt
  ${TEMP}/ioOtbVectorImageTestFORMOSAT2.txt
  otbVectorImageLegacyTest
  LARGEINPUT{/FORMOSAT/Sudouest_20071013_MS_fmsat/}
  ${TEMP}/ioOtbVectorImageTestFORMOSAT2.txt)

if(GDAL_HAS_HDF5)
  otb_add_test(NAME ioTvOtbVectorImageTestCOSMOSKYMED COMMAND otbImageBaseTestDriver
    --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/ioOtbVectorImageTestCOSMOSKYMED.txt
    ${TEMP}/ioOtbVectorImageTestCOSMOSKYMED.txt
    otbVectorImageLegacyTest
    LARGEINPUT{/COSMOSKYMED/Toulouse_spotlight/CSKS3_GTC_B_S2_08_HH_RD_SF_20110418180325_20110418180332.h5}
    ${TEMP}/ioOtbVectorImageTestCOSMOSKYMED.txt)
endif()

otb_add_test(NAME ioTvOtbVectorImageTestRadarsat COMMAND otbImageBaseTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/ioOtbImageTestRadarsat.txt
  ${TEMP}/ioOtbVectorImageTestRadarsat.txt
  otbVectorImageLegacyTest
  LARGEINPUT{/RADARSAT1/GOMA/SCENE01/}
  ${TEMP}/ioOtbVectorImageTestRadarsat.txt)
