if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


set(COMMON_TESTS1 ${CXX_TEST_PATH}/otbCommonTests1)
set(COMMON_TESTS2 ${CXX_TEST_PATH}/otbCommonTests2)
set(COMMON_TESTS3 ${CXX_TEST_PATH}/otbCommonTests3)
set(COMMON_TESTS4 ${CXX_TEST_PATH}/otbCommonTests4)
set(COMMON_TESTS5 ${CXX_TEST_PATH}/otbCommonTests5)
set(COMMON_TESTS6 ${CXX_TEST_PATH}/otbCommonTests6)
set(COMMON_TESTS7 ${CXX_TEST_PATH}/otbCommonTests7)
set(COMMON_TESTS8 ${CXX_TEST_PATH}/otbCommonTests8)
if(OTB_USE_MAPNIK)
  set(COMMON_TESTS9 ${CXX_TEST_PATH}/otbCommonTests9)
endif()
set(COMMON_TESTS11 ${CXX_TEST_PATH}/otbCommonTests11)
set(COMMON_TESTS12 ${CXX_TEST_PATH}/otbCommonTests12)
set(COMMON_TESTS13 ${CXX_TEST_PATH}/otbCommonTests13)
set(COMMON_TESTS14 ${CXX_TEST_PATH}/otbCommonTests14)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::System  -------------
add_test(coTuSystemTests_IsA_Methods ${COMMON_TESTS1}
        otbSystemTest
        ${OTB_DATA_ROOT}/README-OTB-Data
        ${OTB_DATA_ROOT}
        )

add_test(coTvParseHdfSubsetName ${COMMON_TESTS1}
        otbParseHdfSubsetName)

add_test(coTvParseHdfFileName ${COMMON_TESTS1}
        otbParseHdfFileName)

# -------            otb::Configuration  -------------
add_test(coTuConfigurationTest ${COMMON_TESTS1}
        otbConfigurationTest
        )

file(WRITE ${CMAKE_BINARY_DIR}/Testing/otbconf_OK
           "OTB_DEM_DIRECTORY = ${OTB_DATA_ROOT}/Input/DEM/srtm_directory")
file(WRITE ${CMAKE_BINARY_DIR}/Testing/otbconf_NotOK
           "OTB_DEM_DIRECTORY = ${OTB_DATA_ROOT}/Input/DEM/egm96.grd")

add_test(coTuConfigurationTestDEM ${COMMON_TESTS1}
        otbConfigurationTestDEM
        )


add_test(NAME coTuConfigurationTestDEM_WithConfigFile
         COMMAND ${EXECUTABLE_OUTPUT_PATH}/otbTestDriver
        --add-before-env OTB_CONFIG_FILE ${CMAKE_BINARY_DIR}/Testing/otbconf_OK
        Execute $<TARGET_FILE:otbCommonTests1>
        otbConfigurationTestDEM
        ${OTB_DATA_ROOT}/Input/DEM/srtm_directory
        )
add_test(NAME coTuConfigurationTestDEM_WithWrongConfigFile
         COMMAND ${EXECUTABLE_OUTPUT_PATH}/otbTestDriver
        --add-before-env OTB_CONFIG_FILE ${CMAKE_BINARY_DIR}/Testing/otbconf_NotOK
        Execute $<TARGET_FILE:otbCommonTests1>
        otbConfigurationTestDEM
        )
add_test(NAME coTuConfigurationTestDEM_WithNonExistantConfigFile
         COMMAND ${EXECUTABLE_OUTPUT_PATH}/otbTestDriver
        --add-before-env OTB_CONFIG_FILE ${CMAKE_BINARY_DIR}/Testing/otbconf_NonExistant
        Execute $<TARGET_FILE:otbCommonTests1>
        otbConfigurationTestDEM
        )
# -------            otb::CommandLibneArgumentParser  -------------

add_test(coTuCmdLineArgParserHelpWithArg ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserHelp
        --help -image image.png)

add_test(coTuCmdLineArgParserHelpWithoutArg ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserHelp
        --help)

add_test(coTuCmdLineArgParserDeuxEntiers_ExactNumberArg ${COMMON_TESTS1}
        otbTestCommandLineArgumentParser
        -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 )

add_test(coTuCmdLineArgParserDeuxEntier_LessArg ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserWithError
        -image image.png -entier 150 -double 12.25 -deuxentiers 78 )

add_test(coTuCmdLineArgParserDeuxEntier_MoreArg ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserWithError
        -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 32)

add_test(coTuCmdLineArgParser_MissingObligatoryArg ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserWithError
        -entier 150 -double 12.25 -deuxentiers 78 56 32)

add_test(coTuCmdLineArgParser_EmptyList ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserList
        -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 -doubles)

add_test(coTuCmdLineArgParser_List1Arg ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserList
        -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 -doubles 1.0 2.5)

add_test(coTuCmdLineArgParser_List2Arg ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserList
        -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 -doubles 1.0 2.0)

add_test(coTuCmdLineArgParser_List2ArgBis ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserList
        -image image.png -entier 150 -double 12.25 -doubles 1.0 2.0 -deuxentiers 78 56 )


add_test(coTuCmdLineArgParser_List3ArgWithError ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserList
        -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 -doubles 1.0 2.5 -3.5)

#add_test(coTuCmdLineArgParserGUIDeuxEntiers_ExactNumberArg ${COMMON_TESTS1}
#        otbTestCommandLineArgumentParserGUI
#        -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 )


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

add_test(coTuLineSpatialObjectList ${COMMON_TESTS2}
        otbLineSpatialObjectList
        )


# -------            otb::ExtractROI   ------------------------------

add_test(coTuExtractROINew ${COMMON_TESTS2}
        otbExtractROINew)

add_test(coTvExtractROI ${COMMON_TESTS2}
  --compare-image ${NOTOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
                    ${TEMP}/coExtractROI_cthead1_26_97_209_100.png
        otbExtractROI
        ${INPUTDATA}/cthead1.png
        ${TEMP}/coExtractROI_cthead1_26_97_209_100.png
        26 97 209 100 )
add_test(coTvExtractROI2 ${COMMON_TESTS2}
  --compare-image ${NOTOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
                    ${TEMP}/coExtractROI2_cthead1_26_97_209_100.png
        otbExtractROI2
        ${INPUTDATA}/cthead1.png
        ${TEMP}/coExtractROI2_cthead1_26_97_209_100.png
        26 97 209 100 )

add_test(coTvExtractROI_RGB ${COMMON_TESTS2}
  --compare-image ${NOTOL}   ${BASELINE}/coExtractROI_RGB_300_10_200_50.png
                    ${TEMP}/coExtractROI_RGB_300_10_200_50.png
        otbExtractROI_RGB
        ${INPUTDATA}/couleurs_extrait.png
        ${TEMP}/coExtractROI_RGB_300_10_200_50.png
        300 10 200 50 )
add_test(coTvExtractROI_RGB2 ${COMMON_TESTS2}
  --compare-image ${NOTOL}   ${BASELINE}/coExtractROI_RGB_poupees_302_1_134_330.jpg
                    ${TEMP}/coExtractROI_RGB_poupees_302_1_134_330.jpg
        otbExtractROI_RGB
        ${INPUTDATA}/poupees.jpg
        ${TEMP}/coExtractROI_RGB_poupees_302_1_134_330.jpg?&gdal:co:QUALITY=95
        303 2 134 330 )

add_test(coTvExtractROI_QB ${COMMON_TESTS2}
  --compare-image ${NOTOL}
  ${BASELINE}/coTvExtractROI_QB.png
  ${TEMP}/coTvExtractROI_QB.png
    otbExtractROI_RGB
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/coTvExtractROI_QB.png
  0 0 70 70
  )


add_test(coTvExtractROIQB ${COMMON_TESTS2}
  --compare-image ${NOTOL}
  ${BASELINE}/coTvExtractROIQB.png
  ${TEMP}/coTvExtractROIQB.png
    otbExtractROI
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/coTvExtractROIQB.png
  0 0 70 70
  )

# -------            otb::VectorDataExtractROI   ------------------------------
add_test(coTuVectorDataExtractROINew ${COMMON_TESTS2}
        otbVectorDataExtractROINew)

add_test(coTvVectorDataExtractROI ${COMMON_TESTS2}
  --compare-ogr ${NOTOL}
              ${BASELINE_FILES}/coVectorDataExtractROIOutput.shp
              ${TEMP}/coVectorDataExtractROIOutput.shp
         otbVectorDataExtractROI
        ${INPUTDATA}/ToulousePoints-examples.shp
        ${TEMP}/coVectorDataExtractROIOutput.shp
         374369.48850211215904 4853951.786124914  # Origin of the RemoteSensingRegion
         1000.25 -25000.2                               # Size of the Cartoregion
 )

# This test is almost the same as the previous one and should return the
# same output.
# This is to check that region can also be passed with origin
# at the south-west corner
add_test(coTvVectorDataExtractROI2 ${COMMON_TESTS2}
  --compare-ogr ${NOTOL}
              ${BASELINE_FILES}/coVectorDataExtractROIOutput2.shp
              ${TEMP}/coVectorDataExtractROIOutput2.shp
         otbVectorDataExtractROI
        ${INPUTDATA}/ToulousePoints-examples.shp
        ${TEMP}/coVectorDataExtractROIOutput2.shp
         374369.48850211215904 4828951.58612491376698  # Origin of the RemoteSensingRegion
         1000.25 25000.2                               # Size of the Cartoregion
 )

# -------            otb::MultiChannelExtractROI   ------------------------------

add_test(coTuMultiChannelROINew ${COMMON_TESTS2}
         otbMultiChannelExtractROINew)

add_test(coTvMultiChannelROI_RGB2RGB_PNG ${COMMON_TESTS2}
  --compare-image ${NOTOL}   ${INPUTDATA}/couleurs_extrait.png
                           ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/couleurs_extrait.png
         ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG.png
         -channels 1 2 3 )
#         -sizeX 800 -sizeY 600 )

add_test(coTvMultiChannelROI_RGB2RGB_PNG2 ${COMMON_TESTS2}
  --compare-image ${NOTOL}   ${BASELINE}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_123.png
                           ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_123.png
         otbMultiChannelExtractROI
         -uchar ${INPUTDATA}/couleurs_extrait.png
         -uchar ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_123.png
         -startX 300 -startY 10 -sizeX 250 -sizeY 50 -channels 1 2 3  )

add_test(coTvMultiChannelROI_RGB2RGB_PNG3 ${COMMON_TESTS2}
  --compare-image ${NOTOL}  ${BASELINE}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_321.png
                    ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_321.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/couleurs_extrait.png
         ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_321.png
         -startX 300 -startY 10 -sizeX 250 -sizeY 50  -channels 3 2 1  )
add_test(coTvMultiChannelROI_RGB2NG_PNG1 ${COMMON_TESTS2}
  --compare-image ${NOTOL}  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
                    ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/couleurs_extrait.png
         ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
         -startX 300 -startY 10 -sizeX 250 -sizeY 50 -channels 1 )

add_test(coTvMultiChannelROI_RGB2NG_PNG2 ${COMMON_TESTS2}
  --compare-image ${NOTOL}  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_2.png
                    ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_2.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/couleurs_extrait.png
         ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_2.png
         -startX 300 -startY 10 -sizeX 250 -sizeY 50  -channels 2 )
add_test(coTvMultiChannelROI_RGB2NG_PNG3 ${COMMON_TESTS2}
  --compare-image ${NOTOL}  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_3.png
                    ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_3.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/couleurs_extrait.png
         ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_3.png
         -startX 300 -startY 10 -sizeX 250 -sizeY 50  -channels 3 )

add_test(coTvMultiChannelROI_5c_BSQ ${COMMON_TESTS2}
#  --compare-binary  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_poupees_303_2_134_330_channels_12321.c1
#                    ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_poupees_303_2_134_330_channels_12321.c1
  --compare-image ${NOTOL} ${BASELINE}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_12321.hdr
                    ${TEMP}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_12321.hdr
         otbMultiChannelExtractROI
         ${INPUTDATA}/poupees.hdr
         ${TEMP}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_12321.hdr
         -startX 303 -startY 2 -sizeX 134  -sizeY 330 -channels 1 2 3 2 1 )

add_test(coTvMultiChannelROI_3c_BSQ ${COMMON_TESTS2}
#  --compare-binary  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_poupees_303_2_134_330_channels_345.c1
#                    ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_poupees_303_2_134_330_channels_345.c1
  --compare-image  ${NOTOL}  ${BASELINE}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_345.hdr
                    ${TEMP}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_345.hdr
         otbMultiChannelExtractROI
         ${TEMP}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_12321.hdr
         ${TEMP}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_345.hdr
         -startX 0 -startY 0 -sizeX 134  -sizeY 330 -channels 3 4 5 )
set_tests_properties(coTvMultiChannelROI_3c_BSQ PROPERTIES DEPENDS coTvMultiChannelROI_5c_BSQ)

add_test(coTvTestMultiExtractMultiUpdate ${COMMON_TESTS2}
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

add_test(coTvMultiChannelExtractROI_Romania ${COMMON_TESTS2}
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



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::MultiToMonoChannelExtractROI   ------------------------------

add_test(coTuMultiToMonoROINew ${COMMON_TESTS3}
         otbMultiToMonoChannelExtractROINew)

add_test(coTvMultiToMonoROI_RGB2NG_PNG ${COMMON_TESTS3}
  --compare-image ${NOTOL}   ${BASELINE}/coMultiToMonoChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
                           ${TEMP}/coMultiToMonoChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
         otbMultiToMonoChannelExtractROI
         ${INPUTDATA}/couleurs_extrait.png
         ${TEMP}/coMultiToMonoChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
         -startX 300 -startY 10 -sizeX 250 -sizeY 50 -channel 1 )


# -------            otb::PointSetSource   ------------------------------

add_test(coTuPointSetSource ${COMMON_TESTS3}
         otbPointSetSourceTest)

# -------            otb::DrawLineSpatialObjectImageFilter   ------------------------------

add_test(coTuDrawLineSpatialObjectNew ${COMMON_TESTS3}
         otbDrawLineSpatialObjectNew)

add_test(coTuDrawLineSpatialObject ${COMMON_TESTS3}
         otbDrawLineSpatialObject
         ${INPUTDATA}/ImageLineDir.bsq
         ${TEMP}/coFiltreDrawLine_ImageLineDir.png 10. 22. 38. 42.)

# -------            otb::DrawLineSpatialObjectListImageFilter   ------------------------------

add_test(coTuDrawLineSpatialObjectListNew ${COMMON_TESTS3}
         otbDrawLineSpatialObjectListNew)

add_test(coTuDrawLineSpatialObjectList ${COMMON_TESTS3}
         otbDrawLineSpatialObjectList
         ${INPUTDATA}/ImageLineDir.bsq
         ${TEMP}/coFiltreDrawLineList_ImageLineDir.png )


# -------            otb::ImageToLineSpatialObjectListImageFilter   ------------------------------

add_test(coTuImageToLineSpatialObjectListNew ${COMMON_TESTS3}
         otbImageToLineSpatialObjectListNew)

# -------            otb::PathListToHistogramGenerator   ---------------------------

add_test(coTuPathListToHistogramGeneratorNew ${COMMON_TESTS3}
         otbPathListToHistogramGeneratorNew)

add_test(coTuPathListToHistogramGenerator ${COMMON_TESTS3}
         otbPathListToHistogramGenerator 100 2)

# -------            otb::LineSpatialObjectListToPointSetFilter   ------------------------------

add_test(coTuLineSpatialObjectListToPointSetFilter ${COMMON_TESTS3}
         otbLineSpatialObjectListToPointSetFilterNew)


if(OTB_DATA_USE_LARGEINPUT)
# cf. bug 303
add_test(coTvExtractROITestMetaData_TIFF ${COMMON_TESTS3}
         --compare-ascii ${NOTOL}
         ${TEMP}/coTvExtractROITestMetaData1.txt
         ${TEMP}/coTvExtractROITestMetaData2.txt
         --ignore-lines-with 1 Origin:
         otbExtractROITestMetaData
         ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
         ${TEMP}/coTvExtractROITestMetaData1.tif
         ${TEMP}/coTvExtractROITestMetaData2.tif
         ${TEMP}/coTvExtractROITestMetaData1.txt
         ${TEMP}/coTvExtractROITestMetaData2.txt
         )

add_test(coTvExtractROITestMetaData_HDR ${COMMON_TESTS3}
         --compare-ascii ${NOTOL}
         ${TEMP}/coTvExtractROITestMetaData1_hdr.txt
         ${TEMP}/coTvExtractROITestMetaData2_hdr.txt
         --ignore-lines-with 1 Origin:
         otbExtractROITestMetaData
         ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
         ${TEMP}/coTvExtractROITestMetaData1.hdr
         ${TEMP}/coTvExtractROITestMetaData2.hdr
         ${TEMP}/coTvExtractROITestMetaData1_hdr.txt
         ${TEMP}/coTvExtractROITestMetaData2_hdr.txt
         )
endif()

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests4 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::HistogramStatisticFunction   ---------------------------

add_test(coTuHistogramStatisticsFunction ${COMMON_TESTS4}
         otbHistogramStatisticsFunction 100)

# -------            otb::ObjectList   -------------------------------------------

add_test(coTuObjectListNew ${COMMON_TESTS4}
         otbObjectListNew)

add_test(coTvObjectList ${COMMON_TESTS4}
         otbObjectList
         ${INPUTDATA}/couleurs.jpg
         ${INPUTDATA}/poupees.png
         ${INPUTDATA}/couleurs_extrait.png
 )

add_test(coTvObjectListTestNotValid ${COMMON_TESTS4}
         otbObjectList2
 )


# -------            otb::ImageList   -------------------------------------------

add_test(coTuImageListNew ${COMMON_TESTS4}
         otbImageListNew)

add_test(coTvImageList ${COMMON_TESTS4}
        --compare-image ${NOTOL}   ${INPUTDATA}/amst.png
                                 ${TEMP}/amst.png
         otbImageList
         ${INPUTDATA}/amst.png
         ${TEMP}/amst.png
)

# -------            otb::ImageListSource   -------------------------------------------

add_test(coTuImageListSourceNew ${COMMON_TESTS4}
         otbImageListSourceNew)

# -------            otb::ImageToImageListFilter   -------------------------------------------

add_test(coTuImageToImageListFilterNew ${COMMON_TESTS4}
         otbImageToImageListFilterNew)

# -------            otb::ImageListToImageFilter   -------------------------------------------

add_test(coTuImageListToImageFilterNew ${COMMON_TESTS4}
         otbImageListToImageFilterNew)


# -------            otb::ObjectListToObjectListFilterNew   -------------------------------------------
add_test(coTuObjectListToObjectListFilterNew ${COMMON_TESTS4}
         otbObjectListToObjectListFilterNew)

# -------            otb::VectorDataToVectorDataFilterNew   -------------------------------------------
add_test(coTuVectorDataToVectorDataFilterNew ${COMMON_TESTS4}
         otbVectorDataToVectorDataFilterNew)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests5 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::ImageListToImageListFilter   -------------------------------------------

add_test(coTvImageListToImageListFilterNew ${COMMON_TESTS5}
         otbImageListToImageListFilterNew)

# -------            otb::ConcatenateVectorImageFilter  -------------------------------------------

add_test(coTuConcatenateVectorImageFilterNew ${COMMON_TESTS5}
         otbConcatenateVectorImageFilterNew)

add_test(coTvConcatenateVectorImageFilter ${COMMON_TESTS5}
         --compare-image ${NOTOL}
         ${BASELINE}/coConcatenateVectorImageFilterOutput1.hdr
         ${TEMP}/coConcatenateVectorImageFilterOutput1.hdr
         otbConcatenateVectorImageFilter
         ${INPUTDATA}/poupees.png
         ${INPUTDATA}/poupees.png
         ${TEMP}/coConcatenateVectorImageFilterOutput1.hdr
)

#------------    otbImageToVectorImageCastFilter --------------------------------

add_test(coTuImageToVectorImageCastFilter ${COMMON_TESTS5}
        otbImageToVectorImageCastFilterNew)

add_test(coTvImageToVectorImageCastFilter ${COMMON_TESTS5}
--compare-image ${NOTOL}
                ${BASELINE}/coImageToVectorImageCastFilterOutput.tif
                ${TEMP}/coImageToVectorImageCastFilterOutput.tif
                otbImageToVectorImageCastFilter
                ${INPUTDATA}/poupees.hdr
                ${TEMP}/coImageToVectorImageCastFilterOutput.tif
)

# -------            otb::ImageToPathFilterNew  -------------------------------------------
add_test(coTuImageToPathFilterNew ${COMMON_TESTS5}
         otbImageToPathFilterNew)

# -------            otb::SpatialObjectSource  -------------------------------------------
add_test(coTuSpatialObjectSourceNew ${COMMON_TESTS5}
         otbSpatialObjectSourceNew)

# -------            otb::LineSpatialObject  -------------------------------------------
add_test(coTuLineSpatialObjectNew ${COMMON_TESTS5}
         otbLineSpatialObjectNew)

# -------  otb::PathListToPathListFilter -------------------------------------------
add_test(coTuPathListToPathListFilterNew ${COMMON_TESTS5}
         otbPathListToPathListFilterNew)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests6 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------  otb::PolyLineParametricPathWithValue -------------------------------------------
add_test(coTuPolyLineParametricPathWithValueNew ${COMMON_TESTS6}
         otbPolyLineParametricPathWithValueNew)


# -------  otb::Polygon -------------------------------------------
add_test(coTuPolygonNew ${COMMON_TESTS6}
         otbPolygonNew)

add_test(coTvPolygon ${COMMON_TESTS6}
--compare-ascii ${EPSILON_3}
                ${BASELINE_FILES}/otbPolygonTest.txt
                ${TEMP}/otbPolygonTest.txt
         otbPolygon
                ${TEMP}/otbPolygonTest.txt
                2 10 7 10 7 9 5 9 5 8 7 8 7 6 9 6 9 5 7 5 7 3 2 3
                next 6 10 11 10 11 2 4 2 3 4 8 4 8 7 6 7
)

#-------- otb::PolyLineImageIterator -----------------------------------------

 add_test(coTvPolyLineImageConstIterator ${COMMON_TESTS6}
    --compare-ascii ${NOTOL}
                ${BASELINE_FILES}/coTvPolyLineImageConstIterator.txt
                 ${TEMP}/coTvPolyLineImageConstIterator.txt
         otbPolyLineImageConstIterator
                 512 512 20
                 ${TEMP}/coTvPolyLineImageConstIterator.txt
)

add_test(coTvPolyLineImageIterator ${COMMON_TESTS6}
--compare-image ${NOTOL}
                ${BASELINE}/coTvPolyLineImageIterator.png
                ${TEMP}/coTvPolyLineImageIterator.png
         otbPolyLineImageIterator
                512 512 20
                ${TEMP}/coTvPolyLineImageIterator.png
)


# -------  otb::DrawPathListFilter -------------------------------------------

add_test(coTuDrawPathListFilterNew ${COMMON_TESTS6}
         otbDrawPathListFilterNew)

add_test(coTvDrawPathListFilter ${COMMON_TESTS6}
--compare-image ${NOTOL}
                ${BASELINE}/coTvDrawPathListFilterOutput.png
                ${TEMP}/coTvDrawPathListFilterOutput.png
                otbDrawPathListFilter
                512 512 20
                ${TEMP}/coTvDrawPathListFilterOutput.png
)

add_test(coTvDrawPathListFilterWithValue ${COMMON_TESTS6}
--compare-image ${NOTOL}
                ${BASELINE}/coTvDrawPathListFilterWithValueOutput.png
                ${TEMP}/coTvDrawPathListFilterWithValueOutput.png
                otbDrawPathListFilterWithValue
                512 512 20
                ${TEMP}/coTvDrawPathListFilterWithValueOutput.png
)


# -------  otb::DrawPathFilter -------------------------------------------

add_test(coTuDrawPatFilterNew ${COMMON_TESTS6}
otbDrawPathFilterNew
)

add_test(coTvDrawPatFilter ${COMMON_TESTS6}
--compare-image ${NOTOL}
                ${BASELINE}/coTvDrawPathFilterOutput.png
                ${TEMP}/coTvDrawPathFilterOutput.png
                otbDrawPathFilter
                512 512 20
                ${TEMP}/coTvDrawPathFilterOutput.png
)

# -------  otb::UnaryFunctorNeighborhoodImageFilter ----------------------
add_test(coTuUnaryFunctorNeighborhoodImageFilterNew ${COMMON_TESTS6}
otbUnaryFunctorNeighborhoodImageFilterNew
)

add_test(coTvUnaryFunctorNeighborhoodImageFilter ${COMMON_TESTS6}
--compare-image ${NOTOL}
                ${BASELINE}/coTvUnaryFunctorNeighborhoodImageFilter.png
                ${TEMP}/coTvUnaryFunctorNeighborhoodImageFilter.png
    otbUnaryFunctorNeighborhoodImageFilter
        ${INPUTDATA}/amst.png
        ${TEMP}/coTvUnaryFunctorNeighborhoodImageFilter.png
        3
)

#--------------------otb::Rectangle
add_test(coTuRectangleNew ${COMMON_TESTS6}
          otbRectangleNew)

add_test(coTvRectangle ${COMMON_TESTS6}
--compare-ascii ${EPSILON_3}
                ${BASELINE_FILES}/otbRectangleTest.txt
                ${TEMP}/otbRectangleTest.txt
         otbRectangle
                ${TEMP}/otbRectangleTest.txt
                2.12 2.35 12.54 2.45   3. 0.15  1. 1.


)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests7 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------------  otb::GenericInterpolateImageFunction ----------------------------
add_test(coTuGenericInterpolateImageFunctionNew ${COMMON_TESTS7}
otbGenericInterpolateImageFunctionNew
)

# -------------  otb::StandardFilterWatcherNew ----------------------------
add_test(coTuStandardFilterWatcherNew ${COMMON_TESTS7}
     otbStandardFilterWatcherNew
     ${INPUTDATA}/qb_RoadExtract.img
)

# -------------  otb::StandardOneLineFilterWatcherNew ----------------------------
add_test(coTuStandardOneLineFilterWatcher ${COMMON_TESTS7}
     otbStandardOneLineFilterWatcherTest
     ${INPUTDATA}/qb_RoadExtract.img
)


# -------------  otb::DataNode ----------------------------
add_test(coTvDataNode ${COMMON_TESTS7}
     otbDataNodeTest
)
# -------------  otb::VectorData ----------------------------
add_test(coTuVectorData ${COMMON_TESTS7}
     otbVectorDataNew
)

add_test(coTuVectorDataSource ${COMMON_TESTS7}
     otbVectorDataSourceNew
)


add_test(coTvVectorData ${COMMON_TESTS7}
      --compare-ascii ${EPSILON_3}
            ${BASELINE_FILES}/coTvVectorData.txt
            ${TEMP}/coTvVectorData.txt
            --ignore-lines-with 1 RTTI
     otbVectorData
       ${TEMP}/coTvVectorData.txt
)

add_test(coTuShiftScaleImageAdaptorNew ${COMMON_TESTS7}
     otbShiftScaleImageAdaptorNew
)

add_test(coTvShiftScaleImageAdaptor ${COMMON_TESTS7}
  --compare-image ${EPSILON_9}
                  ${BASELINE}/coTvShiftScaleImageAdaptor.tif    # copie the input image
                  ${TEMP}/coTvShiftScaleImageAdaptor.tif
     otbShiftScaleImageAdaptor
        ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
        ${TEMP}/coTvShiftScaleImageAdaptor.tif
)

# -------------  otb::StandardWriterWatcher ----------------------------

add_test(coTvStandardWriterWatcher ${COMMON_TESTS7}
     otbStandardWriterWatcher
        ${INPUTDATA}/couleurs.tif
        ${TEMP}/coTvStandardWriterWatcherOutput.tif
        20
)

# ------ otb::UnaryFunctorNeighborhoodWithOffsetImageFilterNew  ----

add_test(coTuUnaryFunctorNeighborhoodWithOffsetImageFilterNew ${COMMON_TESTS7}
     otbUnaryFunctorNeighborhoodWithOffsetImageFilterNew
)
add_test(coTvUnaryFunctorNeighborhoodWithOffsetImageFilter ${COMMON_TESTS7}
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


#------------------------ otb::RemoteSensingRegion---------------------------------

add_test(coTuRemoteSensingRegionNew ${COMMON_TESTS7}
                     otbRemoteSensingRegionNew )

add_test(coTvRemoteSensingRegion ${COMMON_TESTS7}
         otbRemoteSensingRegion
         1.21 2.33 58.47 59.49  #size et index of the region
         0. 0.                  # Point to check
 )



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests8 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



# -------------  otb::ImageRegionTileMapSplitter ----------------------------
add_test(coTuImageRegionTileMapSplitterNew ${COMMON_TESTS8}
otbImageRegionTileMapSplitterNew
)

add_test(coTvImageRegionTileMapSplitter ${COMMON_TESTS8}
    --compare-ascii ${NOTOL}
        ${BASELINE_FILES}/coImageRegionTileMapSplitter.txt
                ${TEMP}/coImageRegionTileMapSplitter.txt
    otbImageRegionTileMapSplitter
        ${TEMP}/coImageRegionTileMapSplitter.txt
)

# -------------  otb::ImageRegionSquareTileSplitter ----------------------------
add_test(coTuImageRegionSquareTileSplitterNew ${COMMON_TESTS8}
         otbImageRegionSquareTileSplitterNew
)

add_test(coTvImageRegionSquareTileSplitter ${COMMON_TESTS8}
    --compare-ascii ${NOTOL}
        ${BASELINE_FILES}/coImageRegionSquareTileSplitter.txt
        ${TEMP}/coImageRegionSquareTileSplitter.txt
    otbImageRegionSquareTileSplitter
        ${TEMP}/coImageRegionSquareTileSplitter.txt
)

# -------------  otb::ImageOfVectorsToMonoChannelExtractROI ----------------------------
add_test(coTuImageOfVectorsToMonoChannelExtractROINew ${COMMON_TESTS8}
otbImageOfVectorsToMonoChannelExtractROINew
)

add_test(coTvImageOfVectorsToMonoChannelExtractROI ${COMMON_TESTS8}
  --compare-image ${NOTOL}   ${BASELINE}/coImageOfVectorsToMonoChannelExtractROI.png
                           ${TEMP}/coImageOfVectorsToMonoChannelExtractROI.png
        otbImageOfVectorsToMonoChannelExtractROI
        ${INPUTDATA}/couleurs_extrait.png
        300 10  # Start X Y
        250 50  # Size X Y
        1       # Channel
        ${TEMP}/coImageOfVectorsToMonoChannelExtractROI.png
)

# -------------  otb::ImageRegionNonUniformMultidimensionalSplitter ----------------------------
add_test(coTuImageRegionNonUniformMultidimensionalSplitterNew ${COMMON_TESTS8}
otbImageRegionNonUniformMultidimensionalSplitterNew
)

add_test(coTvImageRegionNonUniformMultidimensionalSplitter ${COMMON_TESTS8}
    --compare-ascii ${NOTOL}
        ${BASELINE_FILES}/coImageRegionNonUniformMultidimensionalSplitter.txt
                ${TEMP}/coImageRegionNonUniformMultidimensionalSplitter.txt
    otbImageRegionNonUniformMultidimensionalSplitter
    45 45      # Index
    1000 1500  # Size
    10         # Nb split
    2          # Number of slipt
    ${TEMP}/coImageRegionNonUniformMultidimensionalSplitter.txt
)

# ------------- otb::VectorImageToASImageAdaptor ----------------------------
add_test(coTuVectorImageToASImageAdaptorNew ${COMMON_TESTS8}
    otbVectorImageToASImageAdaptorNew
)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests9 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

if(OTB_USE_MAPNIK)

# -------------  otb::QuickLookImageGenerator ----------------------------
add_test(coTuVectorDataToMapFilterNew ${COMMON_TESTS9}
otbVectorDataToMapFilterNew
)

if(OTB_DATA_USE_LARGEINPUT)
add_test(coTvVectorDataToMapFilter ${COMMON_TESTS9}
     --compare-image ${NOTOL}
                        ${BASELINE}/coTvVectorDataToMapFilter.png
                     ${TEMP}/coTvVectorDataToMapFilter.png
   otbVectorDataToMapFilter
        ${LARGEINPUT}/VECTOR/MidiPyrenees/roads.shp
        ${TEMP}/coTvVectorDataToMapFilter.png
        ${INPUTDATA}/DejaVuSans.ttf # font
)

add_test(coTvVectorDataToMapFilterBinary ${COMMON_TESTS9}
     --compare-image ${NOTOL}
                        ${BASELINE}/coTvVectorDataToMapFilterBinary.png
                     ${TEMP}/coTvVectorDataToMapFilterBinary.png
   otbVectorDataToMapFilterBinary
        ${LARGEINPUT}/VECTOR/MidiPyrenees/roads.shp
        ${TEMP}/coTvVectorDataToMapFilterBinary.png
)

add_test(coTvVectorDataToMapFilterNoFonts ${COMMON_TESTS9}
     --compare-image ${NOTOL}
                        ${BASELINE}/coTvVectorDataToMapFilterNoFonts.png
                     ${TEMP}/coTvVectorDataToMapFilterNoFonts.png
   otbVectorDataToMapFilterNoFonts
        ${LARGEINPUT}/VECTOR/MidiPyrenees/roads.shp
        ${TEMP}/coTvVectorDataToMapFilterNoFonts.png
)


# Deprecated class
add_test(coTvVectorDataToImageFilter ${COMMON_TESTS9}
     --compare-image ${NOTOL}
                        ${BASELINE}/coTvVectorDataToMapFilter.png
                     ${TEMP}/coTvVectorDataToImageFilter.png
   otbVectorDataToImageFilter
        ${LARGEINPUT}/VECTOR/MidiPyrenees/roads.shp
        ${TEMP}/coTvVectorDataToImageFilter.png
        ${INPUTDATA}/DejaVuSans.ttf # font
)

add_test(coTvVectorDataToImageFilterBinary ${COMMON_TESTS9}
     --compare-image ${NOTOL}
                        ${BASELINE}/coTvVectorDataToMapFilterBinary.png
                     ${TEMP}/coTvVectorDataToImageFilterBinary.png
   otbVectorDataToImageFilterBinary
        ${LARGEINPUT}/VECTOR/MidiPyrenees/roads.shp
        ${TEMP}/coTvVectorDataToImageFilterBinary.png
)



endif()




if(OTB_DATA_USE_LARGEINPUT)
add_test(coTvVectorDataToMapFilterSensorModel ${COMMON_TESTS9}
     --compare-image ${NOTOL}
                        ${BASELINE}/coTvVectorDataToMapFilterSensorModel.png
                     ${TEMP}/coTvVectorDataToMapFilterSensorModel.png
   otbVectorDataToMapFilterSensorModel
        ${LARGEINPUT}/VECTOR/MidiPyrenees/roads.shp
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/coTvVectorDataToMapFilterSensorModel.png
        ${INPUTDATA}/DejaVuSans.ttf # font
)
endif()

add_test(coTvVectorDataToMapFilterWorld ${COMMON_TESTS9}
     --compare-image ${NOTOL}
                     ${BASELINE}/coTvVectorDataToMapFilterWorld.png
                     ${TEMP}/coTvVectorDataToMapFilterWorld.png
   otbVectorDataToMapFilterWorld
        ${INPUTDATA}/world_boundaries/world_boundaries_m.shp
        ${INPUTDATA}/world_boundaries/places.shp
        ${TEMP}/coTvVectorDataToMapFilterWorld.png
        500 500 #Size
        -20 60 #lon/lat
        0.1 -0.1 #spacing lon/lat
        ${INPUTDATA}/DejaVuSans.ttf # font
)


endif()


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests11 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# ----------------  AttributesMapLabelObject ---------------------#

add_test(coTuUnaryFunctorWithIndexImageFilterNew ${COMMON_TESTS11}
otbUnaryFunctorWithIndexImageFilterNew
)

add_test(coTvUnaryFunctorWithIndexImageFilter ${COMMON_TESTS11}
     --compare-image ${NOTOL}
                     ${INPUTDATA}/poupees.tif
                     ${TEMP}/coTvUnaryFunctorWithIndexImageFilter.tif
otbUnaryFunctorWithIndexImageFilter
       ${INPUTDATA}/poupees.tif
       ${TEMP}/coTvUnaryFunctorWithIndexImageFilter.tif
)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests12 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# ----------------  otbParser  --------------------- #
  add_test(coTuParser ${COMMON_TESTS12}
    otbParserTestNew
    )

  add_test(coTvParser ${COMMON_TESTS12}
    otbParserTest
    )

# ----------------  otbVariableLengthVectorConverter  --------------------- #
add_test(coTuVariableLengthVectorConverter ${COMMON_TESTS12}
    otbVariableLengthVectorConverterNew
    )

#add_test(coTvVariableLengthVectorConverter ${COMMON_TESTS12}
#    otbVariableLengthVectorConverter
#    )

add_test(coTuRGBAPixelConverter ${COMMON_TESTS12}
    otbRGBAPixelConverterNew
    )

add_test(coTvRGBAPixelConverter ${COMMON_TESTS12}
    otbRGBAPixelConverter
    )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests13 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

add_test(coTuPipelineMemoryPrintCalculatorNew ${COMMON_TESTS13}
  otbPipelineMemoryPrintCalculatorNew
    )

add_test(coTvPipelineMemoryPrintCalculator ${COMMON_TESTS13}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvPipelineMemoryPrintCalculatorOutput.txt
  ${TEMP}/coTvPipelineMemoryPrintCalculatorOutput.txt
  otbPipelineMemoryPrintCalculatorTest
  ${INPUTDATA}/qb_RoadExtract.img
  ${TEMP}/coTvPipelineMemoryPrintCalculatorOutput.txt
)

add_test(coTuStreamingManagerNew ${COMMON_TESTS13}
  otbStreamingManagerNew
)

add_test(coTvNumberOfLinesStrippedStreamingManager ${COMMON_TESTS13}
  --compare-ascii ${NOTOL}
    ${BASELINE_FILES}/coTvNumberOfLinesStrippedStreamingManager.txt
    ${TEMP}/coTvNumberOfLinesStrippedStreamingManager.txt
  otbNumberOfLinesStrippedStreamingManager
    ${TEMP}/coTvNumberOfLinesStrippedStreamingManager.txt
)
add_test(coTvRAMDrivenStrippedStreamingManager ${COMMON_TESTS13}
  --compare-ascii ${NOTOL}
    ${BASELINE_FILES}/coTvRAMDrivenStrippedStreamingManager.txt
    ${TEMP}/coTvRAMDrivenStrippedStreamingManager.txt
  otbRAMDrivenStrippedStreamingManager
    ${TEMP}/coTvRAMDrivenStrippedStreamingManager.txt
)
add_test(coTvTileDimensionTiledStreamingManager ${COMMON_TESTS13}
  --compare-ascii ${NOTOL}
    ${BASELINE_FILES}/coTvTileDimensionTiledStreamingManager.txt
    ${TEMP}/coTvTileDimensionTiledStreamingManager.txt
  otbTileDimensionTiledStreamingManager
    ${TEMP}/coTvTileDimensionTiledStreamingManager.txt
)
add_test(coTvRAMDrivenTiledStreamingManager ${COMMON_TESTS13}
  --compare-ascii ${NOTOL}
    ${BASELINE_FILES}/coTvRAMDrivenTiledStreamingManager.txt
    ${TEMP}/coTvRAMDrivenTiledStreamingManager.txt
  otbRAMDrivenTiledStreamingManager
    ${TEMP}/coTvRAMDrivenTiledStreamingManager.txt
)

add_test(coTvRAMDrivenAdaptativeStreamingManager ${COMMON_TESTS13}
  --compare-ascii ${NOTOL}
    ${BASELINE_FILES}/coTvRAMDrivenAdaptativeStreamingManager.txt
    ${TEMP}/coTvRAMDrivenAdaptativeStreamingManager.txt
    otbRAMDrivenAdaptativeStreamingManager
    ${TEMP}/coTvRAMDrivenAdaptativeStreamingManager.txt
)

add_test(coTuImageRegionAdaptativeSplitterNew ${COMMON_TESTS13}
         otbImageRegionAdaptativeSplitterNew)

add_test(coTvImageRegionAdaptativeSplitterSquaredTileLargeStream ${COMMON_TESTS13}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvImageRegionAdaptativeSplitterSquaredTileLargeStreamOutput.txt
  ${TEMP}/coTvImageRegionAdaptativeSplitterSquaredTileLargeStreamOutput.txt
  otbImageRegionAdaptativeSplitter
  0 0 8192 8192 2048 2048 8
  ${TEMP}/coTvImageRegionAdaptativeSplitterSquaredTileLargeStreamOutput.txt
)

add_test(coTvImageRegionAdaptativeSplitterStripLargeStream ${COMMON_TESTS13}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvImageRegionAdaptativeSplitterStripLargeStreamOutput.txt
  ${TEMP}/coTvImageRegionAdaptativeSplitterStripLargeStreamOutput.txt
  otbImageRegionAdaptativeSplitter
  0 0 8000 8003 8000 16 8
  ${TEMP}/coTvImageRegionAdaptativeSplitterStripLargeStreamOutput.txt
)

add_test(coTvImageRegionAdaptativeSplitterSquaredTileSmallStream ${COMMON_TESTS13}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvImageRegionAdaptativeSplitterSquaredTileSmallStreamOutput.txt
  ${TEMP}/coTvImageRegionAdaptativeSplitterSquaredTileSmallStreamOutput.txt
  otbImageRegionAdaptativeSplitter
  0 0 8192 8192 2048 2048 32
  ${TEMP}/coTvImageRegionAdaptativeSplitterSquaredTileSmallStreamOutput.txt
)

add_test(coTvImageRegionAdaptativeSplitterStripSmallStream ${COMMON_TESTS13}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvImageRegionAdaptativeSplitterStripSmallStreamOutput.txt
  ${TEMP}/coTvImageRegionAdaptativeSplitterStripSmallStreamOutput.txt
  otbImageRegionAdaptativeSplitter
  0 0 8000 8003 8000 1000 20
  ${TEMP}/coTvImageRegionAdaptativeSplitterStripSmallStreamOutput.txt
)



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests14 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

add_test(obTuLabelImageToVectorDataFilterNew ${COMMON_TESTS14}
    otbLabelImageToVectorDataFilterNew)

add_test(obTvLabelImageToVectorDataFilter ${COMMON_TESTS14}
    --compare-ogr ${NOTOL}
    ${BASELINE_FILES}/obTuLabelImageToVectorDataFilter.shp
    ${TEMP}/obTuLabelImageToVectorDataFilter.shp
    otbLabelImageToVectorDataFilter
    ${INPUTDATA}/labelImage_UnsignedChar.tif
    ${TEMP}/obTuLabelImageToVectorDataFilter.shp
    )

add_test(obTuLabelImageToOGRDataSourceFilterNew ${COMMON_TESTS14}
    otbLabelImageToOGRDataSourceFilterNew)

add_test(obTvLabelImageToOGRDataSourceFilter ${COMMON_TESTS14}
    otbLabelImageToOGRDataSourceFilter
    ${INPUTDATA}/labelImage_UnsignedChar.tif
    )

# ---------------------    otbRasterizeVectorDataFilter---------------------
add_test(bfTuVectorDataRasterizeFilterNew ${COMMON_TESTS14}
  otbVectorDataRasterizeFilterNew
)

add_test(bfTvVectorDataRasterizeFilterSHP ${COMMON_TESTS14}
  --compare-image 0.0001
   ${BASELINE}/bfTvVectorDataRasterizeFilter_OutputSHP.tif
   ${TEMP}/bfTvVectorDataRasterizeFilter_OutputSHP.tif
  otbVectorDataRasterizeFilter
  ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
  ${INPUTDATA}/Capitole-Shadows.shp
  ${TEMP}/bfTvVectorDataRasterizeFilter_OutputSHP.tif
)

add_test(bfTvVectorDataRasterizeFilterKML ${COMMON_TESTS14}
  --compare-image 0.0001
   ${BASELINE}/bfTvVectorDataRasterizeFilter_OutputKML.tif
   ${TEMP}/bfTvVectorDataRasterizeFilter_OutputKML.tif
  otbVectorDataRasterizeFilter
  ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
  ${INPUTDATA}/Capitole-Shadows.kml
  ${TEMP}/bfTvVectorDataRasterizeFilter_OutputKML.tif
)

# ---------------------    otbRasterizeVectorDataFilter---------------------
add_test(bfTuVectorDataToLabelImageFilterNew ${COMMON_TESTS14}
  otbVectorDataToLabelImageFilterNew
)

add_test(bfTvVectorDataToLabelImageFilterSHP ${COMMON_TESTS14}
  --compare-image 0.0
  ${INPUTDATA}/QB_Toulouse_ortho_labelImage.tif
  ${TEMP}/bfTvVectorDataToLabelImageFilter_Output.tif
  otbVectorDataToLabelImageFilter
  ${INPUTDATA}/QB_Toulouse_ortho_labelImage.tif
  ${INPUTDATA}/QB_Toulouse_ortho.shp
  ${TEMP}/bfTvVectorDataToLabelImageFilter_Output.tif
)

# ---------------------    otbOGRDataSourceToLabelImageFilter---------------------
add_test(coTuOGRDataSourceToLabelImageFilterNew ${COMMON_TESTS14}
  otbOGRDataSourceToLabelImageFilterNew
)

add_test(coTvOGRDataSourceToLabelImageFilterSHP ${COMMON_TESTS14}
  --compare-image 0.0
  ${INPUTDATA}/QB_Toulouse_ortho_labelImage.tif
  ${TEMP}/bfTvOGRDataSourceToLabelImageFilter_Output.tif
  otbOGRDataSourceToLabelImageFilter
  ${INPUTDATA}/QB_Toulouse_ortho_labelImage.tif
  ${INPUTDATA}/QB_Toulouse_ortho.shp
  ${TEMP}/bfTvOGRDataSourceToLabelImageFilter_Output.tif
  1 0 255
)

add_test(bfTvOGRDataSourceToLabelImageFilterSHPForegroundMode ${COMMON_TESTS14}
  --compare-image 0.0
  ${BASELINE}/bfTvOGRDataSourceToLabelImageFilter_Output_ForegroundMode.tif
  ${TEMP}/bfTvOGRDataSourceToLabelImageFilter_Output_ForegroundMode.tif
  otbOGRDataSourceToLabelImageFilter
  ${INPUTDATA}/QB_Toulouse_ortho_labelImage.tif
  ${INPUTDATA}/QB_Toulouse_ortho_partial.shp
  ${TEMP}/bfTvOGRDataSourceToLabelImageFilter_Output_ForegroundMode.tif
  0 0 255
)


# ---------------------    otbPolygonizationRasterizationTest  ---------------------
# bijectivity tests
add_test(bfTvPolygonizationRasterization_UTM ${COMMON_TESTS14}
  otbPolygonizationRasterizationTest
  ${INPUTDATA}/QB_Toulouse_ortho_labelImage.tif
)

add_test(bfTvPolygonizationRasterization_WGS84 ${COMMON_TESTS14}
  otbPolygonizationRasterizationTest
  ${INPUTDATA}/QB_Toulouse_ortho_labelImage_WGS84.tif
)
add_test(bfTvPolygonizationRasterization_SensorModel ${COMMON_TESTS14}
  otbPolygonizationRasterizationTest
  ${INPUTDATA}/QB_Toulouse_SensorModel_labelImage.tif
)

add_test(bfTvPolygonizationRasterization_Index ${COMMON_TESTS14}
  otbPolygonizationRasterizationTest
  ${INPUTDATA}/QB_Toulouse_labelImage_Index.tif
)


# -------            otb::ImageListToVectorImageFilter   ----------------------------

add_test(coTuImageListToVectorImageFilterNew ${COMMON_TESTS14}
         otbImageListToVectorImageFilterNew)


add_test(coTvImageListToVectorImageFilter ${COMMON_TESTS14}
                --compare-image ${EPSILON_7}
                    ${BASELINE}/bfTvImageListToVectorImageFilter.png
                    ${TEMP}/coTvImageListToVectorImageFilter.png
                    otbImageListToVectorImageFilter
          ${INPUTDATA}/poupees_c1.hdr
          ${INPUTDATA}/poupees_c2.hdr
          ${INPUTDATA}/poupees_c3.hdr
            #${INPUTDATA}/poupees.c1.hdr
                    #${INPUTDATA}/poupees.c2.hdr
                    #${INPUTDATA}/poupees.c3.hdr
                    ${TEMP}/coTvImageListToVectorImageFilter.png
)

add_test(coTvImageListToVectorImageFilter2 ${COMMON_TESTS14}
                    otbImageListToVectorImageFilter2
          ${INPUTDATA}/poupees_c1.hdr
          ${INPUTDATA}/poupees_c2.hdr
          ${INPUTDATA}/poupees_c3.hdr
)


# -------       Fichiers sources CXX -----------------------------------
set(BasicCommon_SRCS1
otbCommonTests1.cxx
otbSystemTest.cxx
otbConfigurationTest.cxx
otbTestCommandLineArgumentParser.cxx
#otbTestCommandLineArgumentParserGUI.cxx
otbTestCommandLineArgumentParserHelp.cxx
otbTestCommandLineArgumentParserList.cxx
otbTestCommandLineArgumentParserWithError.cxx
)
set(BasicCommon_SRCS2
otbCommonTests2.cxx
otbLineSpatialObjectList.cxx
otbExtractROINew.cxx
otbExtractROI.cxx
otbExtractROI2.cxx
otbExtractROI_RGB.cxx
otbMultiChannelExtractROI.cxx
otbMultiChannelExtractROINew.cxx
otbTestMultiExtractMultiUpdate.cxx
otbVectorDataExtractROINew.cxx
otbVectorDataExtractROI.cxx
)
set(BasicCommon_SRCS3
otbCommonTests3.cxx
otbMultiToMonoChannelExtractROI.cxx
otbMultiToMonoChannelExtractROINew.cxx
otbExtractROITestMetaData.cxx
otbPointSetSourceTest.cxx
otbDrawLineSpatialObjectNew.cxx
otbDrawLineSpatialObject.cxx
otbDrawLineSpatialObjectListNew.cxx
otbDrawLineSpatialObjectList.cxx
otbImageToLineSpatialObjectListNew.cxx
otbPathListToHistogramGeneratorNew.cxx
otbPathListToHistogramGenerator.cxx
otbLineSpatialObjectListToPointSetFilterNew.cxx
)
set(BasicCommon_SRCS4
otbCommonTests4.cxx
otbHistogramStatisticsFunction.cxx
otbObjectListNew.cxx
otbObjectList.cxx
otbObjectList2.cxx
otbImageListNew.cxx
otbImageList.cxx
otbImageListSourceNew.cxx
otbImageToImageListFilterNew.cxx
otbImageListToImageFilterNew.cxx
otbObjectListToObjectListFilterNew.cxx
otbVectorDataToVectorDataFilterNew.cxx
)
set(BasicCommon_SRCS5
otbCommonTests5.cxx
otbImageListToImageListFilterNew.cxx
otbConcatenateVectorImageFilterNew.cxx
otbConcatenateVectorImageFilter.cxx
otbImageToVectorImageCastFilterNew.cxx
otbImageToVectorImageCastFilter.cxx
otbImageToPathFilterNew.cxx
otbSpatialObjectSourceNew.cxx
otbLineSpatialObjectNew.cxx
otbPathListToPathListFilterNew.cxx
)
set(BasicCommon_SRCS6
otbCommonTests6.cxx
otbPolyLineImageConstIterator.cxx
otbPolyLineImageIterator.cxx
otbDrawPathFilterNew.cxx
otbDrawPathFilter.cxx
otbDrawPathListFilterNew.cxx
otbDrawPathListFilter.cxx
otbDrawPathListFilterWithValue.cxx
otbPolyLineParametricPathWithValueNew.cxx
otbPolygonNew.cxx
otbPolygon.cxx
otbUnaryFunctorNeighborhoodImageFilterNew.cxx
otbUnaryFunctorNeighborhoodImageFilter.cxx
otbRectangleNew.cxx
otbRectangle.cxx
)

set(BasicCommon_SRCS7
otbCommonTests7.cxx
otbGenericInterpolateImageFunctionNew.cxx
otbStandardFilterWatcherNew.cxx
otbStandardOneLineFilterWatcherTest.cxx
otbDataNodeTest.cxx
otbVectorDataNew.cxx
otbVectorDataSourceNew.cxx
otbVectorData.cxx
otbShiftScaleImageAdaptorNew.cxx
otbShiftScaleImageAdaptor.cxx
otbStandardWriterWatcher.cxx
otbUnaryFunctorNeighborhoodWithOffsetImageFilterNew.cxx
otbUnaryFunctorNeighborhoodWithOffsetImageFilter.cxx
otbRemoteSensingRegionNew.cxx
otbRemoteSensingRegion.cxx
)

set(BasicCommon_SRCS8
otbCommonTests8.cxx
otbImageRegionTileMapSplitterNew.cxx
otbImageRegionTileMapSplitter.cxx
otbImageRegionSquareTileSplitter.cxx
otbImageOfVectorsToMonoChannelExtractROINew.cxx
otbImageOfVectorsToMonoChannelExtractROI.cxx
otbImageRegionNonUniformMultidimensionalSplitterNew.cxx
otbImageRegionNonUniformMultidimensionalSplitter.cxx
otbVectorImageToASImageAdaptorNew.cxx
)

if(OTB_USE_MAPNIK)
set(BasicCommon_SRCS9
otbCommonTests9.cxx
otbVectorDataToMapFilterNew.cxx
otbVectorDataToMapFilter.cxx
otbVectorDataToMapFilterSensorModel.cxx
otbVectorDataToMapFilterWorld.cxx
otbVectorDataToMapFilterNoFonts.cxx
otbVectorDataToImageFilter.cxx
)
endif()

set(BasicCommon_SRCS11
otbUnaryFunctorWithIndexImageFilterNew.cxx
otbUnaryFunctorWithIndexImageFilter.cxx
)


set(BasicCommon_SRCS12
otbCommonTests12.cxx
otbParserTest.cxx
otbVariableLengthVectorConverter.cxx
otbRGBAPixelConverter.cxx
)


set(BasicCommon_SRCS13
otbCommonTests13.cxx
otbPipelineMemoryPrintCalculatorTest.cxx
otbStreamingManager.cxx
otbImageRegionAdaptativeSplitter.cxx
)

set(BasicCommon_SRCS14
otbCommonTests14.cxx
otbLabelImageToVectorDataFilterNew.cxx
otbLabelImageToVectorDataFilter.cxx
otbLabelImageToOGRDataSourceFilter.cxx
otbVectorDataRasterizeFilter.cxx
otbVectorDataToLabelImageFilter.cxx
otbOGRDataSourceToLabelImageFilter.cxx
otbPolygonizationRasterizationTest.cxx
otbImageListToVectorImageFilterNew.cxx
otbImageListToVectorImageFilter.cxx
otbImageListToVectorImageFilter2.cxx
)

OTB_ADD_EXECUTABLE(otbCommonTests1 "${BasicCommon_SRCS1}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbCommonTests2 "${BasicCommon_SRCS2}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbCommonTests3 "${BasicCommon_SRCS3}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbCommonTests4 "${BasicCommon_SRCS4}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbCommonTests5 "${BasicCommon_SRCS5}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbCommonTests6 "${BasicCommon_SRCS6}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbCommonTests7 "${BasicCommon_SRCS7}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbCommonTests8 "${BasicCommon_SRCS8}" "OTBIO;OTBTesting")
if(OTB_USE_MAPNIK)
  OTB_ADD_EXECUTABLE(otbCommonTests9 "${BasicCommon_SRCS9}" "OTBIO;OTBTesting;${MAPNIK_LIBRARY};${ICUUC_LIBRARY}")
endif()
add_executable(otbCommonTests11 otbCommonTests11.cxx ${BasicCommon_SRCS11})
target_link_libraries(otbCommonTests11 OTBIO  OTBTesting itkvcl)

OTB_ADD_EXECUTABLE(otbCommonTests12 "${BasicCommon_SRCS12}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbCommonTests13 "${BasicCommon_SRCS13}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbCommonTests14 "${BasicCommon_SRCS14}" "OTBIO;OTBTesting;OTBOGRAdapters")

endif()
