IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


SET(COMMON_TESTS1 ${CXX_TEST_PATH}/otbCommonTests1)
SET(COMMON_TESTS2 ${CXX_TEST_PATH}/otbCommonTests2)
SET(COMMON_TESTS3 ${CXX_TEST_PATH}/otbCommonTests3)
SET(COMMON_TESTS4 ${CXX_TEST_PATH}/otbCommonTests4)
SET(COMMON_TESTS5 ${CXX_TEST_PATH}/otbCommonTests5)
SET(COMMON_TESTS6 ${CXX_TEST_PATH}/otbCommonTests6)
SET(COMMON_TESTS7 ${CXX_TEST_PATH}/otbCommonTests7)
SET(COMMON_TESTS8 ${CXX_TEST_PATH}/otbCommonTests8)
IF(OTB_USE_MAPNIK)
  SET(COMMON_TESTS9 ${CXX_TEST_PATH}/otbCommonTests9)
ENDIF(OTB_USE_MAPNIK)
IF(OTB_USE_PQXX)
  SET(COMMON_TESTS10 ${CXX_TEST_PATH}/otbCommonTests10)
ENDIF(OTB_USE_PQXX)
SET(COMMON_TESTS11 ${CXX_TEST_PATH}/otbCommonTests11)
SET(COMMON_TESTS12 ${CXX_TEST_PATH}/otbCommonTests12)
SET(COMMON_TESTS13 ${CXX_TEST_PATH}/otbCommonTests13)
SET(COMMON_TESTS14 ${CXX_TEST_PATH}/otbCommonTests14)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::System  -------------
ADD_TEST(coTuSystemTests_IsA_Methods ${COMMON_TESTS1}
        otbSystemTest
        ${OTB_DATA_ROOT}/OTBData.readme
        ${OTB_DATA_ROOT}
        )

ADD_TEST(coTvParseHdfSubsetName ${COMMON_TESTS1}
        otbParseHdfSubsetName)

ADD_TEST(coTvParseHdfFileName ${COMMON_TESTS1}
        otbParseHdfFileName)

# -------            otb::Configuration  -------------
ADD_TEST(coTuConfigurationTest ${COMMON_TESTS1}
        otbConfigurationTest
        )

FILE(WRITE ${CMAKE_BINARY_DIR}/Testing/otbconf_OK
           "OTB_DEM_DIRECTORY = ${OTB_DATA_ROOT}/Input/DEM/srtm_directory")
FILE(WRITE ${CMAKE_BINARY_DIR}/Testing/otbconf_NotOK
           "OTB_DEM_DIRECTORY = ${OTB_DATA_ROOT}/Input/DEM/egm96.grd")

ADD_TEST(coTuConfigurationTestDEM ${COMMON_TESTS1}
        otbConfigurationTestDEM
        )


ADD_TEST(NAME coTuConfigurationTestDEM_WithConfigFile
         COMMAND ${EXECUTABLE_OUTPUT_PATH}/otbTestDriver
        --add-before-env OTB_CONFIG_FILE ${CMAKE_BINARY_DIR}/Testing/otbconf_OK
        Execute $<TARGET_FILE:otbCommonTests1>
        otbConfigurationTestDEM
        ${OTB_DATA_ROOT}/Input/DEM/srtm_directory
        )
ADD_TEST(NAME coTuConfigurationTestDEM_WithWrongConfigFile
         COMMAND ${EXECUTABLE_OUTPUT_PATH}/otbTestDriver
        --add-before-env OTB_CONFIG_FILE ${CMAKE_BINARY_DIR}/Testing/otbconf_NotOK
        Execute $<TARGET_FILE:otbCommonTests1>
        otbConfigurationTestDEM
        )
ADD_TEST(NAME coTuConfigurationTestDEM_WithNonExistantConfigFile
         COMMAND ${EXECUTABLE_OUTPUT_PATH}/otbTestDriver
        --add-before-env OTB_CONFIG_FILE ${CMAKE_BINARY_DIR}/Testing/otbconf_NonExistant
        Execute $<TARGET_FILE:otbCommonTests1>
        otbConfigurationTestDEM
        )
# -------            otb::CommandLibneArgumentParser  -------------

ADD_TEST(coTuCmdLineArgParserHelpWithArg ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserHelp
        --help -image image.png)

ADD_TEST(coTuCmdLineArgParserHelpWithoutArg ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserHelp
        --help)

ADD_TEST(coTuCmdLineArgParserDeuxEntiers_ExactNumberArg ${COMMON_TESTS1}
        otbTestCommandLineArgumentParser
        -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 )

ADD_TEST(coTuCmdLineArgParserDeuxEntier_LessArg ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserWithError
        -image image.png -entier 150 -double 12.25 -deuxentiers 78 )

ADD_TEST(coTuCmdLineArgParserDeuxEntier_MoreArg ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserWithError
        -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 32)

ADD_TEST(coTuCmdLineArgParser_MissingObligatoryArg ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserWithError
        -entier 150 -double 12.25 -deuxentiers 78 56 32)

ADD_TEST(coTuCmdLineArgParser_EmptyList ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserList
        -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 -doubles)

ADD_TEST(coTuCmdLineArgParser_List1Arg ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserList
        -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 -doubles 1.0 2.5)

ADD_TEST(coTuCmdLineArgParser_List2Arg ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserList
        -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 -doubles 1.0 2.0)

ADD_TEST(coTuCmdLineArgParser_List2ArgBis ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserList
        -image image.png -entier 150 -double 12.25 -doubles 1.0 2.0 -deuxentiers 78 56 )


ADD_TEST(coTuCmdLineArgParser_List3ArgWithError ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserList
        -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 -doubles 1.0 2.5 -3.5)

#ADD_TEST(coTuCmdLineArgParserGUIDeuxEntiers_ExactNumberArg ${COMMON_TESTS1}
#        otbTestCommandLineArgumentParserGUI
#        -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 )


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ADD_TEST(coTuLineSpatialObjectList ${COMMON_TESTS2}
        otbLineSpatialObjectList
        )


# -------            otb::ExtractROI   ------------------------------

ADD_TEST(coTuExtractROINew ${COMMON_TESTS2}
        otbExtractROINew)

ADD_TEST(coTvExtractROI ${COMMON_TESTS2}
  --compare-image ${NOTOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
                    ${TEMP}/coExtractROI_cthead1_26_97_209_100.png
        otbExtractROI
        ${INPUTDATA}/cthead1.png
        ${TEMP}/coExtractROI_cthead1_26_97_209_100.png
        26 97 209 100 )
ADD_TEST(coTvExtractROI2 ${COMMON_TESTS2}
  --compare-image ${NOTOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
                    ${TEMP}/coExtractROI2_cthead1_26_97_209_100.png
        otbExtractROI2
        ${INPUTDATA}/cthead1.png
        ${TEMP}/coExtractROI2_cthead1_26_97_209_100.png
        26 97 209 100 )

ADD_TEST(coTvExtractROI_RGB ${COMMON_TESTS2}
  --compare-image ${NOTOL}   ${BASELINE}/coExtractROI_RGB_300_10_200_50.png
                    ${TEMP}/coExtractROI_RGB_300_10_200_50.png
        otbExtractROI_RGB
        ${INPUTDATA}/couleurs_extrait.png
        ${TEMP}/coExtractROI_RGB_300_10_200_50.png
        300 10 200 50 )
ADD_TEST(coTvExtractROI_RGB2 ${COMMON_TESTS2}
  --compare-image ${NOTOL}   ${BASELINE}/coExtractROI_RGB_poupees_302_1_134_330.jpg
                    ${TEMP}/coExtractROI_RGB_poupees_302_1_134_330.jpg
        otbExtractROI_RGB
        ${INPUTDATA}/poupees.jpg
        ${TEMP}/coExtractROI_RGB_poupees_302_1_134_330.jpg?&gdal:co:QUALITY=95
        303 2 134 330 )

ADD_TEST(coTvExtractROI_QB ${COMMON_TESTS2}
  --compare-image ${NOTOL}
  ${BASELINE}/coTvExtractROI_QB.png
  ${TEMP}/coTvExtractROI_QB.png
    otbExtractROI_RGB
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/coTvExtractROI_QB.png
  0 0 70 70
  )


ADD_TEST(coTvExtractROIQB ${COMMON_TESTS2}
  --compare-image ${NOTOL}
  ${BASELINE}/coTvExtractROIQB.png
  ${TEMP}/coTvExtractROIQB.png
    otbExtractROI
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/coTvExtractROIQB.png
  0 0 70 70
  )

# -------            otb::VectorDataExtractROI   ------------------------------
ADD_TEST(coTuVectorDataExtractROINew ${COMMON_TESTS2}
        otbVectorDataExtractROINew)

ADD_TEST(coTvVectorDataExtractROI ${COMMON_TESTS2}
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
ADD_TEST(coTvVectorDataExtractROI2 ${COMMON_TESTS2}
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

ADD_TEST(coTuMultiChannelROINew ${COMMON_TESTS2}
         otbMultiChannelExtractROINew)

ADD_TEST(coTvMultiChannelROI_RGB2RGB_PNG ${COMMON_TESTS2}
  --compare-image ${NOTOL}   ${INPUTDATA}/couleurs_extrait.png
                           ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/couleurs_extrait.png
         ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG.png
         -channels 1 2 3 )
#         -sizeX 800 -sizeY 600 )

ADD_TEST(coTvMultiChannelROI_RGB2RGB_PNG2 ${COMMON_TESTS2}
  --compare-image ${NOTOL}   ${BASELINE}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_123.png
                           ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_123.png
         otbMultiChannelExtractROI
         -uchar ${INPUTDATA}/couleurs_extrait.png
         -uchar ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_123.png
         -startX 300 -startY 10 -sizeX 250 -sizeY 50 -channels 1 2 3  )

ADD_TEST(coTvMultiChannelROI_RGB2RGB_PNG3 ${COMMON_TESTS2}
  --compare-image ${NOTOL}  ${BASELINE}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_321.png
                    ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_321.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/couleurs_extrait.png
         ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_321.png
         -startX 300 -startY 10 -sizeX 250 -sizeY 50  -channels 3 2 1  )
ADD_TEST(coTvMultiChannelROI_RGB2NG_PNG1 ${COMMON_TESTS2}
  --compare-image ${NOTOL}  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
                    ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/couleurs_extrait.png
         ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
         -startX 300 -startY 10 -sizeX 250 -sizeY 50 -channels 1 )

ADD_TEST(coTvMultiChannelROI_RGB2NG_PNG2 ${COMMON_TESTS2}
  --compare-image ${NOTOL}  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_2.png
                    ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_2.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/couleurs_extrait.png
         ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_2.png
         -startX 300 -startY 10 -sizeX 250 -sizeY 50  -channels 2 )
ADD_TEST(coTvMultiChannelROI_RGB2NG_PNG3 ${COMMON_TESTS2}
  --compare-image ${NOTOL}  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_3.png
                    ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_3.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/couleurs_extrait.png
         ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_3.png
         -startX 300 -startY 10 -sizeX 250 -sizeY 50  -channels 3 )

ADD_TEST(coTvMultiChannelROI_5c_BSQ ${COMMON_TESTS2}
#  --compare-binary  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_poupees_303_2_134_330_channels_12321.c1
#                    ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_poupees_303_2_134_330_channels_12321.c1
  --compare-image ${NOTOL} ${BASELINE}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_12321.hdr
                    ${TEMP}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_12321.hdr
         otbMultiChannelExtractROI
         ${INPUTDATA}/poupees.hdr
         ${TEMP}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_12321.hdr
         -startX 303 -startY 2 -sizeX 134  -sizeY 330 -channels 1 2 3 2 1 )

ADD_TEST(coTvMultiChannelROI_3c_BSQ ${COMMON_TESTS2}
#  --compare-binary  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_poupees_303_2_134_330_channels_345.c1
#                    ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_poupees_303_2_134_330_channels_345.c1
  --compare-image  ${NOTOL}  ${BASELINE}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_345.hdr
                    ${TEMP}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_345.hdr
         otbMultiChannelExtractROI
         ${TEMP}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_12321.hdr
         ${TEMP}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_345.hdr
         -startX 0 -startY 0 -sizeX 134  -sizeY 330 -channels 3 4 5 )
SET_TESTS_PROPERTIES(coTvMultiChannelROI_3c_BSQ PROPERTIES DEPENDS coTvMultiChannelROI_5c_BSQ)

ADD_TEST(coTvTestMultiExtractMultiUpdate ${COMMON_TESTS2}
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

ADD_TEST(coTvMultiChannelExtractROI_Romania ${COMMON_TESTS2}
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

ADD_TEST(coTuMultiToMonoROINew ${COMMON_TESTS3}
         otbMultiToMonoChannelExtractROINew)

ADD_TEST(coTvMultiToMonoROI_RGB2NG_PNG ${COMMON_TESTS3}
  --compare-image ${NOTOL}   ${BASELINE}/coMultiToMonoChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
                           ${TEMP}/coMultiToMonoChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
         otbMultiToMonoChannelExtractROI
         ${INPUTDATA}/couleurs_extrait.png
         ${TEMP}/coMultiToMonoChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
         -startX 300 -startY 10 -sizeX 250 -sizeY 50 -channel 1 )


# -------            otb::PointSetSource   ------------------------------

ADD_TEST(coTuPointSetSource ${COMMON_TESTS3}
         otbPointSetSourceTest)

# -------            otb::DrawLineSpatialObjectImageFilter   ------------------------------

ADD_TEST(coTuDrawLineSpatialObjectNew ${COMMON_TESTS3}
         otbDrawLineSpatialObjectNew)

ADD_TEST(coTuDrawLineSpatialObject ${COMMON_TESTS3}
         otbDrawLineSpatialObject
         ${INPUTDATA}/ImageLineDir.bsq
         ${TEMP}/coFiltreDrawLine_ImageLineDir.png 10. 22. 38. 42.)

# -------            otb::DrawLineSpatialObjectListImageFilter   ------------------------------

ADD_TEST(coTuDrawLineSpatialObjectListNew ${COMMON_TESTS3}
         otbDrawLineSpatialObjectListNew)

ADD_TEST(coTuDrawLineSpatialObjectList ${COMMON_TESTS3}
         otbDrawLineSpatialObjectList
         ${INPUTDATA}/ImageLineDir.bsq
         ${TEMP}/coFiltreDrawLineList_ImageLineDir.png )


# -------            otb::ImageToLineSpatialObjectListImageFilter   ------------------------------

ADD_TEST(coTuImageToLineSpatialObjectListNew ${COMMON_TESTS3}
         otbImageToLineSpatialObjectListNew)

# -------            otb::PathListToHistogramGenerator   ---------------------------

ADD_TEST(coTuPathListToHistogramGeneratorNew ${COMMON_TESTS3}
         otbPathListToHistogramGeneratorNew)

ADD_TEST(coTuPathListToHistogramGenerator ${COMMON_TESTS3}
         otbPathListToHistogramGenerator 100 2)

# -------            otb::LineSpatialObjectListToPointSetFilter   ------------------------------

ADD_TEST(coTuLineSpatialObjectListToPointSetFilter ${COMMON_TESTS3}
         otbLineSpatialObjectListToPointSetFilterNew)


IF(OTB_DATA_USE_LARGEINPUT)
# cf. bug 303
ADD_TEST(coTvExtractROITestMetaData_TIFF ${COMMON_TESTS3}
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

ADD_TEST(coTvExtractROITestMetaData_HDR ${COMMON_TESTS3}
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
ENDIF(OTB_DATA_USE_LARGEINPUT)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests4 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::HistogramStatisticFunction   ---------------------------

ADD_TEST(coTuHistogramStatisticsFunction ${COMMON_TESTS4}
         otbHistogramStatisticsFunction 100)

# -------            otb::ObjectList   -------------------------------------------

ADD_TEST(coTuObjectListNew ${COMMON_TESTS4}
         otbObjectListNew)

ADD_TEST(coTvObjectList ${COMMON_TESTS4}
         otbObjectList
         ${INPUTDATA}/couleurs.jpg
         ${INPUTDATA}/poupees.png
         ${INPUTDATA}/couleurs_extrait.png
 )

ADD_TEST(coTvObjectListTestNotValid ${COMMON_TESTS4}
         otbObjectList2
 )


# -------            otb::ImageList   -------------------------------------------

ADD_TEST(coTuImageListNew ${COMMON_TESTS4}
         otbImageListNew)

ADD_TEST(coTvImageList ${COMMON_TESTS4}
        --compare-image ${NOTOL}   ${INPUTDATA}/amst.png
                                 ${TEMP}/amst.png
         otbImageList
         ${INPUTDATA}/amst.png
         ${TEMP}/amst.png
)

# -------            otb::ImageListSource   -------------------------------------------

ADD_TEST(coTuImageListSourceNew ${COMMON_TESTS4}
         otbImageListSourceNew)

# -------            otb::ImageToImageListFilter   -------------------------------------------

ADD_TEST(coTuImageToImageListFilterNew ${COMMON_TESTS4}
         otbImageToImageListFilterNew)

# -------            otb::ImageListToImageFilter   -------------------------------------------

ADD_TEST(coTuImageListToImageFilterNew ${COMMON_TESTS4}
         otbImageListToImageFilterNew)


# -------            otb::ObjectListToObjectListFilterNew   -------------------------------------------
ADD_TEST(coTuObjectListToObjectListFilterNew ${COMMON_TESTS4}
         otbObjectListToObjectListFilterNew)

# -------            otb::VectorDataToVectorDataFilterNew   -------------------------------------------
ADD_TEST(coTuVectorDataToVectorDataFilterNew ${COMMON_TESTS4}
         otbVectorDataToVectorDataFilterNew)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests5 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::ImageListToImageListFilter   -------------------------------------------

ADD_TEST(coTvImageListToImageListFilterNew ${COMMON_TESTS5}
         otbImageListToImageListFilterNew)

# -------            otb::ConcatenateVectorImageFilter  -------------------------------------------

ADD_TEST(coTuConcatenateVectorImageFilterNew ${COMMON_TESTS5}
         otbConcatenateVectorImageFilterNew)

ADD_TEST(coTvConcatenateVectorImageFilter ${COMMON_TESTS5}
         --compare-image ${NOTOL}
         ${BASELINE}/coConcatenateVectorImageFilterOutput1.hdr
         ${TEMP}/coConcatenateVectorImageFilterOutput1.hdr
         otbConcatenateVectorImageFilter
         ${INPUTDATA}/poupees.png
         ${INPUTDATA}/poupees.png
         ${TEMP}/coConcatenateVectorImageFilterOutput1.hdr
)

#------------    otbImageToVectorImageCastFilter --------------------------------

ADD_TEST(coTuImageToVectorImageCastFilter ${COMMON_TESTS5}
        otbImageToVectorImageCastFilterNew)

ADD_TEST(coTvImageToVectorImageCastFilter ${COMMON_TESTS5}
--compare-image ${NOTOL}
                ${BASELINE}/coImageToVectorImageCastFilterOutput.tif
                ${TEMP}/coImageToVectorImageCastFilterOutput.tif
                otbImageToVectorImageCastFilter
                ${INPUTDATA}/poupees.hdr
                ${TEMP}/coImageToVectorImageCastFilterOutput.tif
)

# -------            otb::ImageToPathFilterNew  -------------------------------------------
ADD_TEST(coTuImageToPathFilterNew ${COMMON_TESTS5}
         otbImageToPathFilterNew)

# -------            otb::SpatialObjectSource  -------------------------------------------
ADD_TEST(coTuSpatialObjectSourceNew ${COMMON_TESTS5}
         otbSpatialObjectSourceNew)

# -------            otb::LineSpatialObject  -------------------------------------------
ADD_TEST(coTuLineSpatialObjectNew ${COMMON_TESTS5}
         otbLineSpatialObjectNew)

# -------  otb::PathListToPathListFilter -------------------------------------------
ADD_TEST(coTuPathListToPathListFilterNew ${COMMON_TESTS5}
         otbPathListToPathListFilterNew)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests6 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------  otb::PolyLineParametricPathWithValue -------------------------------------------
ADD_TEST(coTuPolyLineParametricPathWithValueNew ${COMMON_TESTS6}
         otbPolyLineParametricPathWithValueNew)


# -------  otb::Polygon -------------------------------------------
ADD_TEST(coTuPolygonNew ${COMMON_TESTS6}
         otbPolygonNew)

ADD_TEST(coTvPolygon ${COMMON_TESTS6}
--compare-ascii ${EPSILON_3}
                ${BASELINE_FILES}/otbPolygonTest.txt
                ${TEMP}/otbPolygonTest.txt
         otbPolygon
                ${TEMP}/otbPolygonTest.txt
                2 10 7 10 7 9 5 9 5 8 7 8 7 6 9 6 9 5 7 5 7 3 2 3
                next 6 10 11 10 11 2 4 2 3 4 8 4 8 7 6 7
)

#-------- otb::PolyLineImageIterator -----------------------------------------

 ADD_TEST(coTvPolyLineImageConstIterator ${COMMON_TESTS6}
    --compare-ascii ${NOTOL}
                ${BASELINE_FILES}/coTvPolyLineImageConstIterator.txt
                 ${TEMP}/coTvPolyLineImageConstIterator.txt
         otbPolyLineImageConstIterator
                 512 512 20
                 ${TEMP}/coTvPolyLineImageConstIterator.txt
)

ADD_TEST(coTvPolyLineImageIterator ${COMMON_TESTS6}
--compare-image ${NOTOL}
                ${BASELINE}/coTvPolyLineImageIterator.png
                ${TEMP}/coTvPolyLineImageIterator.png
         otbPolyLineImageIterator
                512 512 20
                ${TEMP}/coTvPolyLineImageIterator.png
)


# -------  otb::DrawPathListFilter -------------------------------------------

ADD_TEST(coTuDrawPathListFilterNew ${COMMON_TESTS6}
         otbDrawPathListFilterNew)

ADD_TEST(coTvDrawPathListFilter ${COMMON_TESTS6}
--compare-image ${NOTOL}
                ${BASELINE}/coTvDrawPathListFilterOutput.png
                ${TEMP}/coTvDrawPathListFilterOutput.png
                otbDrawPathListFilter
                512 512 20
                ${TEMP}/coTvDrawPathListFilterOutput.png
)

ADD_TEST(coTvDrawPathListFilterWithValue ${COMMON_TESTS6}
--compare-image ${NOTOL}
                ${BASELINE}/coTvDrawPathListFilterWithValueOutput.png
                ${TEMP}/coTvDrawPathListFilterWithValueOutput.png
                otbDrawPathListFilterWithValue
                512 512 20
                ${TEMP}/coTvDrawPathListFilterWithValueOutput.png
)


# -------  otb::DrawPathFilter -------------------------------------------

ADD_TEST(coTuDrawPatFilterNew ${COMMON_TESTS6}
otbDrawPathFilterNew
)

ADD_TEST(coTvDrawPatFilter ${COMMON_TESTS6}
--compare-image ${NOTOL}
                ${BASELINE}/coTvDrawPathFilterOutput.png
                ${TEMP}/coTvDrawPathFilterOutput.png
                otbDrawPathFilter
                512 512 20
                ${TEMP}/coTvDrawPathFilterOutput.png
)

# -------  otb::UnaryFunctorNeighborhoodImageFilter ----------------------
ADD_TEST(coTuUnaryFunctorNeighborhoodImageFilterNew ${COMMON_TESTS6}
otbUnaryFunctorNeighborhoodImageFilterNew
)

ADD_TEST(coTvUnaryFunctorNeighborhoodImageFilter ${COMMON_TESTS6}
--compare-image ${NOTOL}
                ${BASELINE}/coTvUnaryFunctorNeighborhoodImageFilter.png
                ${TEMP}/coTvUnaryFunctorNeighborhoodImageFilter.png
    otbUnaryFunctorNeighborhoodImageFilter
        ${INPUTDATA}/amst.png
        ${TEMP}/coTvUnaryFunctorNeighborhoodImageFilter.png
        3
)

#--------------------otb::Rectangle
ADD_TEST(coTuRectangleNew ${COMMON_TESTS6}
          otbRectangleNew)

ADD_TEST(coTvRectangle ${COMMON_TESTS6}
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
ADD_TEST(coTuGenericInterpolateImageFunctionNew ${COMMON_TESTS7}
otbGenericInterpolateImageFunctionNew
)

# -------------  otb::StandardFilterWatcherNew ----------------------------
ADD_TEST(coTuStandardFilterWatcherNew ${COMMON_TESTS7}
     otbStandardFilterWatcherNew
     ${INPUTDATA}/qb_RoadExtract.img
)

# -------------  otb::StandardOneLineFilterWatcherNew ----------------------------
ADD_TEST(coTuStandardOneLineFilterWatcher ${COMMON_TESTS7}
     otbStandardOneLineFilterWatcherTest
     ${INPUTDATA}/qb_RoadExtract.img
)


# -------------  otb::DataNode ----------------------------
ADD_TEST(coTvDataNode ${COMMON_TESTS7}
     otbDataNodeTest
)
# -------------  otb::VectorData ----------------------------
ADD_TEST(coTuVectorData ${COMMON_TESTS7}
     otbVectorDataNew
)

ADD_TEST(coTuVectorDataSource ${COMMON_TESTS7}
     otbVectorDataSourceNew
)


ADD_TEST(coTvVectorData ${COMMON_TESTS7}
      --compare-ascii ${EPSILON_3}
            ${BASELINE_FILES}/coTvVectorData.txt
            ${TEMP}/coTvVectorData.txt
            --ignore-lines-with 1 RTTI
     otbVectorData
       ${TEMP}/coTvVectorData.txt
)

ADD_TEST(coTuShiftScaleImageAdaptorNew ${COMMON_TESTS7}
     otbShiftScaleImageAdaptorNew
)

ADD_TEST(coTvShiftScaleImageAdaptor ${COMMON_TESTS7}
  --compare-image ${EPSILON_9}
                  ${BASELINE}/coTvShiftScaleImageAdaptor.tif    # copie the input image
                  ${TEMP}/coTvShiftScaleImageAdaptor.tif
     otbShiftScaleImageAdaptor
        ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
        ${TEMP}/coTvShiftScaleImageAdaptor.tif
)

# -------------  otb::StandardWriterWatcher ----------------------------

ADD_TEST(coTvStandardWriterWatcher ${COMMON_TESTS7}
     otbStandardWriterWatcher
        ${INPUTDATA}/couleurs.tif
        ${TEMP}/coTvStandardWriterWatcherOutput.tif
        20
)

# ------ otb::UnaryFunctorNeighborhoodWithOffsetImageFilterNew  ----

ADD_TEST(coTuUnaryFunctorNeighborhoodWithOffsetImageFilterNew ${COMMON_TESTS7}
     otbUnaryFunctorNeighborhoodWithOffsetImageFilterNew
)
ADD_TEST(coTvUnaryFunctorNeighborhoodWithOffsetImageFilter ${COMMON_TESTS7}
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

ADD_TEST(coTuRemoteSensingRegionNew ${COMMON_TESTS7}
                     otbRemoteSensingRegionNew )

ADD_TEST(coTvRemoteSensingRegion ${COMMON_TESTS7}
         otbRemoteSensingRegion
         1.21 2.33 58.47 59.49  #size et index of the region
         0. 0.                  # Point to check
 )



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests8 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



# -------------  otb::ImageRegionTileMapSplitter ----------------------------
ADD_TEST(coTuImageRegionTileMapSplitterNew ${COMMON_TESTS8}
otbImageRegionTileMapSplitterNew
)

ADD_TEST(coTvImageRegionTileMapSplitter ${COMMON_TESTS8}
    --compare-ascii ${NOTOL}
        ${BASELINE_FILES}/coImageRegionTileMapSplitter.txt
                ${TEMP}/coImageRegionTileMapSplitter.txt
    otbImageRegionTileMapSplitter
        ${TEMP}/coImageRegionTileMapSplitter.txt
)

# -------------  otb::ImageRegionSquareTileSplitter ----------------------------
ADD_TEST(coTuImageRegionSquareTileSplitterNew ${COMMON_TESTS8}
         otbImageRegionSquareTileSplitterNew
)

ADD_TEST(coTvImageRegionSquareTileSplitter ${COMMON_TESTS8}
    --compare-ascii ${NOTOL}
        ${BASELINE_FILES}/coImageRegionSquareTileSplitter.txt
        ${TEMP}/coImageRegionSquareTileSplitter.txt
    otbImageRegionSquareTileSplitter
        ${TEMP}/coImageRegionSquareTileSplitter.txt
)

# -------------  otb::ImageOfVectorsToMonoChannelExtractROI ----------------------------
ADD_TEST(coTuImageOfVectorsToMonoChannelExtractROINew ${COMMON_TESTS8}
otbImageOfVectorsToMonoChannelExtractROINew
)

ADD_TEST(coTvImageOfVectorsToMonoChannelExtractROI ${COMMON_TESTS8}
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
ADD_TEST(coTuImageRegionNonUniformMultidimensionalSplitterNew ${COMMON_TESTS8}
otbImageRegionNonUniformMultidimensionalSplitterNew
)

ADD_TEST(coTvImageRegionNonUniformMultidimensionalSplitter ${COMMON_TESTS8}
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
ADD_TEST(coTuVectorImageToASImageAdaptorNew ${COMMON_TESTS8}
    otbVectorImageToASImageAdaptorNew
)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests9 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

IF(OTB_USE_MAPNIK)

# -------------  otb::QuickLookImageGenerator ----------------------------
ADD_TEST(coTuVectorDataToMapFilterNew ${COMMON_TESTS9}
otbVectorDataToMapFilterNew
)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(coTvVectorDataToMapFilter ${COMMON_TESTS9}
     --compare-image ${NOTOL}
                        ${BASELINE}/coTvVectorDataToMapFilter.png
                     ${TEMP}/coTvVectorDataToMapFilter.png
   otbVectorDataToMapFilter
        ${LARGEINPUT}/VECTOR/MidiPyrenees/roads.shp
        ${TEMP}/coTvVectorDataToMapFilter.png
        ${INPUTDATA}/DejaVuSans.ttf # font
)

ADD_TEST(coTvVectorDataToMapFilterBinary ${COMMON_TESTS9}
     --compare-image ${NOTOL}
                        ${BASELINE}/coTvVectorDataToMapFilterBinary.png
                     ${TEMP}/coTvVectorDataToMapFilterBinary.png
   otbVectorDataToMapFilterBinary
        ${LARGEINPUT}/VECTOR/MidiPyrenees/roads.shp
        ${TEMP}/coTvVectorDataToMapFilterBinary.png
)

ADD_TEST(coTvVectorDataToMapFilterNoFonts ${COMMON_TESTS9}
     --compare-image ${NOTOL}
                        ${BASELINE}/coTvVectorDataToMapFilterNoFonts.png
                     ${TEMP}/coTvVectorDataToMapFilterNoFonts.png
   otbVectorDataToMapFilterNoFonts
        ${LARGEINPUT}/VECTOR/MidiPyrenees/roads.shp
        ${TEMP}/coTvVectorDataToMapFilterNoFonts.png
)


# Deprecated class
ADD_TEST(coTvVectorDataToImageFilter ${COMMON_TESTS9}
     --compare-image ${NOTOL}
                        ${BASELINE}/coTvVectorDataToMapFilter.png
                     ${TEMP}/coTvVectorDataToImageFilter.png
   otbVectorDataToImageFilter
        ${LARGEINPUT}/VECTOR/MidiPyrenees/roads.shp
        ${TEMP}/coTvVectorDataToImageFilter.png
        ${INPUTDATA}/DejaVuSans.ttf # font
)

ADD_TEST(coTvVectorDataToImageFilterBinary ${COMMON_TESTS9}
     --compare-image ${NOTOL}
                        ${BASELINE}/coTvVectorDataToMapFilterBinary.png
                     ${TEMP}/coTvVectorDataToImageFilterBinary.png
   otbVectorDataToImageFilterBinary
        ${LARGEINPUT}/VECTOR/MidiPyrenees/roads.shp
        ${TEMP}/coTvVectorDataToImageFilterBinary.png
)



ENDIF(OTB_DATA_USE_LARGEINPUT)




IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(coTvVectorDataToMapFilterSensorModel ${COMMON_TESTS9}
     --compare-image ${NOTOL}
                        ${BASELINE}/coTvVectorDataToMapFilterSensorModel.png
                     ${TEMP}/coTvVectorDataToMapFilterSensorModel.png
   otbVectorDataToMapFilterSensorModel
        ${LARGEINPUT}/VECTOR/MidiPyrenees/roads.shp
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/coTvVectorDataToMapFilterSensorModel.png
        ${INPUTDATA}/DejaVuSans.ttf # font
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(coTvVectorDataToMapFilterWorld ${COMMON_TESTS9}
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


ENDIF(OTB_USE_MAPNIK)




# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests10 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# ----------------  PostGIS Interface ---------------------#
IF(OTB_USE_PQXX)

ADD_TEST(coTuPostGISConnectionImplementationNew ${COMMON_TESTS10}
  otbPostGISConnectionImplementationNew
)

#This test need a runing PostGIS server on the localhost with trusted potgres user and an ecisting testgis base
ADD_TEST(coTvPostGISConnectionImplementationConnectToDB ${COMMON_TESTS10}
        otbPostGISConnectionImplementationConnectToDB
        localhost
        orfeotoolbox_test
        orfeotoolbox_test_user
        Bidfeud0
)

ADD_TEST(coTvPostGISConnectionImplementationPerformTransaction ${COMMON_TESTS10}
        otbPostGISConnectionImplementationPerformTransaction
        localhost
        orfeotoolbox_test
        orfeotoolbox_test_user
        Bidfeud0
)


ADD_TEST(coTuGISTableNew ${COMMON_TESTS10}
  otbGISTableNew
)

ADD_TEST(coTuPostGISTableNew ${COMMON_TESTS10}
  otbPostGISTableNew
)

ADD_TEST(coTuVectorDataToGISTableFilterNew ${COMMON_TESTS10}
  otbVectorDataToGISTableFilterNew
)


# ADD_TEST(coTuVectorDataToGISTableFilter ${COMMON_TESTS10}
# otbVectorDataToGISTableFilter
# ${INPUTDATA}/ToulousePoints-examples.shp
# test
# postgres
# postgres
# )

ADD_TEST(coTuGISTableToGISTableFilterNew ${COMMON_TESTS10}
  otbGISTableToGISTableFilterNew
)


ADD_TEST(coTuGISTableToVectorDataFilterNew ${COMMON_TESTS10}
  otbGISTableToVectorDataFilterNew
)

#need gdal 1.6 for POSTGIS tables connection via OGR string
ADD_TEST(coTvGISTableToVectorDataFilter ${COMMON_TESTS10}
otbGISTableToVectorDataFilter
${TEMP}/gistabletovectordatafilter.shp
orfeotoolbox_test
labelmaptogis_test_table
orfeotoolbox_test_user
Bidfeud0
)


# ----------------  otbGISTableSource  ---------------------#
ADD_TEST(coTuGISTableSourceNew ${COMMON_TESTS10}
otbGISTableSourceNew
)


ENDIF(OTB_USE_PQXX)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests11 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# ----------------  AttributesMapLabelObject ---------------------#

ADD_TEST(coTuUnaryFunctorWithIndexImageFilterNew ${COMMON_TESTS11}
otbUnaryFunctorWithIndexImageFilterNew
)

ADD_TEST(coTvUnaryFunctorWithIndexImageFilter ${COMMON_TESTS11}
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
  ADD_TEST(coTuParser ${COMMON_TESTS12}
    otbParserTestNew
    )

  ADD_TEST(coTvParser ${COMMON_TESTS12}
    otbParserTest
    )

# ----------------  otbVariableLengthVectorConverter  --------------------- #
ADD_TEST(coTuVariableLengthVectorConverter ${COMMON_TESTS12}
    otbVariableLengthVectorConverterNew
    )

#ADD_TEST(coTvVariableLengthVectorConverter ${COMMON_TESTS12}
#    otbVariableLengthVectorConverter
#    )

ADD_TEST(coTuRGBAPixelConverter ${COMMON_TESTS12}
    otbRGBAPixelConverterNew
    )

ADD_TEST(coTvRGBAPixelConverter ${COMMON_TESTS12}
    otbRGBAPixelConverter
    )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests13 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ADD_TEST(coTuPipelineMemoryPrintCalculatorNew ${COMMON_TESTS13}
  otbPipelineMemoryPrintCalculatorNew
    )

ADD_TEST(coTvPipelineMemoryPrintCalculator ${COMMON_TESTS13}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvPipelineMemoryPrintCalculatorOutput.txt
  ${TEMP}/coTvPipelineMemoryPrintCalculatorOutput.txt
  otbPipelineMemoryPrintCalculatorTest
  ${INPUTDATA}/qb_RoadExtract.img
  ${TEMP}/coTvPipelineMemoryPrintCalculatorOutput.txt
)

ADD_TEST(coTuStreamingManagerNew ${COMMON_TESTS13}
  otbStreamingManagerNew
)

ADD_TEST(coTvNumberOfLinesStrippedStreamingManager ${COMMON_TESTS13}
  --compare-ascii ${NOTOL}
    ${BASELINE_FILES}/coTvNumberOfLinesStrippedStreamingManager.txt
    ${TEMP}/coTvNumberOfLinesStrippedStreamingManager.txt
  otbNumberOfLinesStrippedStreamingManager
    ${TEMP}/coTvNumberOfLinesStrippedStreamingManager.txt
)
ADD_TEST(coTvRAMDrivenStrippedStreamingManager ${COMMON_TESTS13}
  --compare-ascii ${NOTOL}
    ${BASELINE_FILES}/coTvRAMDrivenStrippedStreamingManager.txt
    ${TEMP}/coTvRAMDrivenStrippedStreamingManager.txt
  otbRAMDrivenStrippedStreamingManager
    ${TEMP}/coTvRAMDrivenStrippedStreamingManager.txt
)
ADD_TEST(coTvTileDimensionTiledStreamingManager ${COMMON_TESTS13}
  --compare-ascii ${NOTOL}
    ${BASELINE_FILES}/coTvTileDimensionTiledStreamingManager.txt
    ${TEMP}/coTvTileDimensionTiledStreamingManager.txt
  otbTileDimensionTiledStreamingManager
    ${TEMP}/coTvTileDimensionTiledStreamingManager.txt
)
ADD_TEST(coTvRAMDrivenTiledStreamingManager ${COMMON_TESTS13}
  --compare-ascii ${NOTOL}
    ${BASELINE_FILES}/coTvRAMDrivenTiledStreamingManager.txt
    ${TEMP}/coTvRAMDrivenTiledStreamingManager.txt
  otbRAMDrivenTiledStreamingManager
    ${TEMP}/coTvRAMDrivenTiledStreamingManager.txt
)

ADD_TEST(coTvRAMDrivenAdaptativeStreamingManager ${COMMON_TESTS13}
  --compare-ascii ${NOTOL}
    ${BASELINE_FILES}/coTvRAMDrivenAdaptativeStreamingManager.txt
    ${TEMP}/coTvRAMDrivenAdaptativeStreamingManager.txt
    otbRAMDrivenAdaptativeStreamingManager
    ${TEMP}/coTvRAMDrivenAdaptativeStreamingManager.txt
)

ADD_TEST(coTuImageRegionAdaptativeSplitterNew ${COMMON_TESTS13}
         otbImageRegionAdaptativeSplitterNew)

ADD_TEST(coTvImageRegionAdaptativeSplitterSquaredTileLargeStream ${COMMON_TESTS13}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvImageRegionAdaptativeSplitterSquaredTileLargeStreamOutput.txt
  ${TEMP}/coTvImageRegionAdaptativeSplitterSquaredTileLargeStreamOutput.txt
  otbImageRegionAdaptativeSplitter
  0 0 8192 8192 2048 2048 8
  ${TEMP}/coTvImageRegionAdaptativeSplitterSquaredTileLargeStreamOutput.txt
)

ADD_TEST(coTvImageRegionAdaptativeSplitterStripLargeStream ${COMMON_TESTS13}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvImageRegionAdaptativeSplitterStripLargeStreamOutput.txt
  ${TEMP}/coTvImageRegionAdaptativeSplitterStripLargeStreamOutput.txt
  otbImageRegionAdaptativeSplitter
  0 0 8000 8003 8000 16 8
  ${TEMP}/coTvImageRegionAdaptativeSplitterStripLargeStreamOutput.txt
)

ADD_TEST(coTvImageRegionAdaptativeSplitterSquaredTileSmallStream ${COMMON_TESTS13}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvImageRegionAdaptativeSplitterSquaredTileSmallStreamOutput.txt
  ${TEMP}/coTvImageRegionAdaptativeSplitterSquaredTileSmallStreamOutput.txt
  otbImageRegionAdaptativeSplitter
  0 0 8192 8192 2048 2048 32
  ${TEMP}/coTvImageRegionAdaptativeSplitterSquaredTileSmallStreamOutput.txt
)

ADD_TEST(coTvImageRegionAdaptativeSplitterStripSmallStream ${COMMON_TESTS13}
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

ADD_TEST(obTuLabelImageToVectorDataFilterNew ${COMMON_TESTS14}
    otbLabelImageToVectorDataFilterNew)

ADD_TEST(obTvLabelImageToVectorDataFilter ${COMMON_TESTS14}
    --compare-ogr ${NOTOL}
    ${BASELINE_FILES}/obTuLabelImageToVectorDataFilter.shp
    ${TEMP}/obTuLabelImageToVectorDataFilter.shp
    otbLabelImageToVectorDataFilter
    ${INPUTDATA}/labelImage_UnsignedChar.tif
    ${TEMP}/obTuLabelImageToVectorDataFilter.shp
    )

ADD_TEST(obTuLabelImageToOGRDataSourceFilterNew ${COMMON_TESTS14}
    otbLabelImageToOGRDataSourceFilterNew)

ADD_TEST(obTvLabelImageToOGRDataSourceFilter ${COMMON_TESTS14}
    otbLabelImageToOGRDataSourceFilter
    ${INPUTDATA}/labelImage_UnsignedChar.tif
    )

# ---------------------    otbRasterizeVectorDataFilter---------------------
ADD_TEST(bfTuVectorDataRasterizeFilterNew ${COMMON_TESTS14}
  otbVectorDataRasterizeFilterNew
)

ADD_TEST(bfTvVectorDataRasterizeFilterSHP ${COMMON_TESTS14}
  --compare-image 0.0001
   ${BASELINE}/bfTvVectorDataRasterizeFilter_OutputSHP.tif
   ${TEMP}/bfTvVectorDataRasterizeFilter_OutputSHP.tif
  otbVectorDataRasterizeFilter
  ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
  ${INPUTDATA}/Capitole-Shadows.shp
  ${TEMP}/bfTvVectorDataRasterizeFilter_OutputSHP.tif
)

ADD_TEST(bfTvVectorDataRasterizeFilterKML ${COMMON_TESTS14}
  --compare-image 0.0001
   ${BASELINE}/bfTvVectorDataRasterizeFilter_OutputKML.tif
   ${TEMP}/bfTvVectorDataRasterizeFilter_OutputKML.tif
  otbVectorDataRasterizeFilter
  ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
  ${INPUTDATA}/Capitole-Shadows.kml
  ${TEMP}/bfTvVectorDataRasterizeFilter_OutputKML.tif
)

# ---------------------    otbRasterizeVectorDataFilter---------------------
ADD_TEST(bfTuVectorDataToLabelImageFilterNew ${COMMON_TESTS14}
  otbVectorDataToLabelImageFilterNew
)

ADD_TEST(bfTvVectorDataToLabelImageFilterSHP ${COMMON_TESTS14}
  --compare-image 0.0
  ${INPUTDATA}/QB_Toulouse_ortho_labelImage.tif
  ${TEMP}/bfTvVectorDataToLabelImageFilter_Output.tif
  otbVectorDataToLabelImageFilter
  ${INPUTDATA}/QB_Toulouse_ortho_labelImage.tif
  ${INPUTDATA}/QB_Toulouse_ortho.shp
  ${TEMP}/bfTvVectorDataToLabelImageFilter_Output.tif
)

# ---------------------    otbOGRDataSourceToLabelImageFilter---------------------
ADD_TEST(coTuOGRDataSourceToLabelImageFilterNew ${COMMON_TESTS14}
  otbOGRDataSourceToLabelImageFilterNew
)

ADD_TEST(coTvOGRDataSourceToLabelImageFilterSHP ${COMMON_TESTS14}
  --compare-image 0.0
  ${INPUTDATA}/QB_Toulouse_ortho_labelImage.tif
  ${TEMP}/bfTvOGRDataSourceToLabelImageFilter_Output.tif
  otbOGRDataSourceToLabelImageFilter
  ${INPUTDATA}/QB_Toulouse_ortho_labelImage.tif
  ${INPUTDATA}/QB_Toulouse_ortho.shp
  ${TEMP}/bfTvOGRDataSourceToLabelImageFilter_Output.tif
  1 0 255
)

ADD_TEST(bfTvOGRDataSourceToLabelImageFilterSHPForegroundMode ${COMMON_TESTS14}
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
ADD_TEST(bfTvPolygonizationRasterization_UTM ${COMMON_TESTS14}
  otbPolygonizationRasterizationTest
  ${INPUTDATA}/QB_Toulouse_ortho_labelImage.tif
)

ADD_TEST(bfTvPolygonizationRasterization_WGS84 ${COMMON_TESTS14}
  otbPolygonizationRasterizationTest
  ${INPUTDATA}/QB_Toulouse_ortho_labelImage_WGS84.tif
)
ADD_TEST(bfTvPolygonizationRasterization_SensorModel ${COMMON_TESTS14}
  otbPolygonizationRasterizationTest
  ${INPUTDATA}/QB_Toulouse_SensorModel_labelImage.tif
)

ADD_TEST(bfTvPolygonizationRasterization_Index ${COMMON_TESTS14}
  otbPolygonizationRasterizationTest
  ${INPUTDATA}/QB_Toulouse_labelImage_Index.tif
)


# -------            otb::ImageListToVectorImageFilter   ----------------------------

ADD_TEST(coTuImageListToVectorImageFilterNew ${COMMON_TESTS14}
         otbImageListToVectorImageFilterNew)


ADD_TEST(coTvImageListToVectorImageFilter ${COMMON_TESTS14}
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

ADD_TEST(coTvImageListToVectorImageFilter2 ${COMMON_TESTS14}
                    otbImageListToVectorImageFilter2
          ${INPUTDATA}/poupees_c1.hdr
          ${INPUTDATA}/poupees_c2.hdr
          ${INPUTDATA}/poupees_c3.hdr
)


# -------       Fichiers sources CXX -----------------------------------
SET(BasicCommon_SRCS1
otbCommonTests1.cxx
otbSystemTest.cxx
otbConfigurationTest.cxx
otbTestCommandLineArgumentParser.cxx
#otbTestCommandLineArgumentParserGUI.cxx
otbTestCommandLineArgumentParserHelp.cxx
otbTestCommandLineArgumentParserList.cxx
otbTestCommandLineArgumentParserWithError.cxx
)
SET(BasicCommon_SRCS2
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
SET(BasicCommon_SRCS3
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
SET(BasicCommon_SRCS4
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
SET(BasicCommon_SRCS5
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
SET(BasicCommon_SRCS6
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

SET(BasicCommon_SRCS7
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

SET(BasicCommon_SRCS8
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

IF(OTB_USE_MAPNIK)
SET(BasicCommon_SRCS9
otbCommonTests9.cxx
otbVectorDataToMapFilterNew.cxx
otbVectorDataToMapFilter.cxx
otbVectorDataToMapFilterSensorModel.cxx
otbVectorDataToMapFilterWorld.cxx
otbVectorDataToMapFilterNoFonts.cxx
otbVectorDataToImageFilter.cxx
)
ENDIF(OTB_USE_MAPNIK)

IF(OTB_USE_PQXX)
SET(BasicCommon_SRCS10
otbCommonTests10.cxx
otbPostGISConnectionImplementationNew.cxx
otbPostGISConnectionImplementationConnectToDB.cxx
otbPostGISConnectionImplementationPerformTransaction.cxx
otbGISTableNew.cxx
otbPostGISTableNew.cxx
otbVectorDataToGISTableFilterNew.cxx
# otbVectorDataToGISTableFilter.cxx
otbGISTableToGISTableFilterNew.cxx
otbGISTableToVectorDataFilterNew.cxx
otbGISTableToVectorDataFilter.cxx
otbGISTableSourceNew.cxx
)
ENDIF(OTB_USE_PQXX)


SET(BasicCommon_SRCS11
otbUnaryFunctorWithIndexImageFilterNew.cxx
otbUnaryFunctorWithIndexImageFilter.cxx
)


SET(BasicCommon_SRCS12
otbCommonTests12.cxx
otbParserTest.cxx
otbVariableLengthVectorConverter.cxx
otbRGBAPixelConverter.cxx
)


SET(BasicCommon_SRCS13
otbCommonTests13.cxx
otbPipelineMemoryPrintCalculatorTest.cxx
otbStreamingManager.cxx
otbImageRegionAdaptativeSplitter.cxx
)

SET(BasicCommon_SRCS14
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
IF(OTB_USE_MAPNIK)
  OTB_ADD_EXECUTABLE(otbCommonTests9 "${BasicCommon_SRCS9}" "OTBIO;OTBTesting;${MAPNIK_LIBRARY};${ICUUC_LIBRARY}")
ENDIF(OTB_USE_MAPNIK)
IF(OTB_USE_PQXX)
  #TODO this line should be refined when we will like to have this capability with windows
OTB_ADD_EXECUTABLE(otbCommonTests10 "${BasicCommon_SRCS10}" "OTBIO;OTBTesting")
TARGET_LINK_LIBRARIES(otbCommonTests10 OTBIO  OTBTesting OTBGeospatialAnalysis pq pqxx)
ENDIF(OTB_USE_PQXX)
ADD_EXECUTABLE(otbCommonTests11 otbCommonTests11.cxx ${BasicCommon_SRCS11})
TARGET_LINK_LIBRARIES(otbCommonTests11 OTBIO  OTBTesting itkvcl)

OTB_ADD_EXECUTABLE(otbCommonTests12 "${BasicCommon_SRCS12}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbCommonTests13 "${BasicCommon_SRCS13}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbCommonTests14 "${BasicCommon_SRCS14}" "OTBIO;OTBTesting;OTBOGRAdapters")

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
