IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Remote sensing images (large images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(LARGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

#Tolerance sur diff pixel image
SET(TOL 0.0)

#Tolerance sur la difference des valeurs numeriques dans le --compare-ascii
SET(EPS 0.001)

SET(COMMON_TESTS1 ${CXX_TEST_PATH}/otbCommonTests1)
SET(COMMON_TESTS2 ${CXX_TEST_PATH}/otbCommonTests2)
SET(COMMON_TESTS3 ${CXX_TEST_PATH}/otbCommonTests3)
SET(COMMON_TESTS4 ${CXX_TEST_PATH}/otbCommonTests4)
SET(COMMON_TESTS5 ${CXX_TEST_PATH}/otbCommonTests5)
SET(COMMON_TESTS6 ${CXX_TEST_PATH}/otbCommonTests6)
SET(COMMON_TESTS7 ${CXX_TEST_PATH}/otbCommonTests7)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::System  -------------
ADD_TEST(coTuSystemTests_IsA_Methods ${COMMON_TESTS1}
        otbSystemTest
        ${OTB_DATA_ROOT}/OTBData.readme
        ${OTB_DATA_ROOT}
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
        -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 -doubles 1.0)

ADD_TEST(coTuCmdLineArgParser_List2Arg ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserList
        -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 -doubles 1.0 2.0)

ADD_TEST(coTuCmdLineArgParser_List2ArgBis ${COMMON_TESTS1}
        otbTestCommandLineArgumentParserList
        -image image.png -entier 150 -double 12.25 -doubles 1.0 2.0 -deuxentiers 78 56 )

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
  --compare-image ${TOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
                    ${TEMP}/coExtractROI_cthead1_26_97_209_100.png
        otbExtractROI
        ${INPUTDATA}/cthead1.png
        ${TEMP}/coExtractROI_cthead1_26_97_209_100.png
        26 97 209 100 )
ADD_TEST(coTvExtractROI2 ${COMMON_TESTS2}
  --compare-image ${TOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
                    ${TEMP}/coExtractROI2_cthead1_26_97_209_100.png
        otbExtractROI2
        ${INPUTDATA}/cthead1.png
        ${TEMP}/coExtractROI2_cthead1_26_97_209_100.png
        26 97 209 100 )

ADD_TEST(coTvExtractROI_RGB ${COMMON_TESTS2}
  --compare-image ${TOL}   ${BASELINE}/coExtractROI_RGB_300_10_200_50.png
                    ${TEMP}/coExtractROI_RGB_300_10_200_50.png
        otbExtractROI_RGB
        ${INPUTDATA}/couleurs_extrait.png
        ${TEMP}/coExtractROI_RGB_300_10_200_50.png
        300 10 200 50 )
ADD_TEST(coTvExtractROI_RGB2 ${COMMON_TESTS2}
  --compare-image ${TOL}   ${BASELINE}/coExtractROI_RGB_poupees_302_1_134_330.jpg
                    ${TEMP}/coExtractROI_RGB_poupees_302_1_134_330.jpg
        otbExtractROI_RGB
        ${INPUTDATA}/poupees.jpg
        ${TEMP}/coExtractROI_RGB_poupees_302_1_134_330.jpg
        303 2 134 330 )

# -------            otb::VectorDataExtractROI   ------------------------------
ADD_TEST(coTuVectorDataExtractROINew ${COMMON_TESTS2}
	otbVectorDataExtractROINew)

ADD_TEST(coTvVectorDataExtractROI ${COMMON_TESTS2}
  --compare-binary    
              ${BASELINE}/coVectorDataExtractROIOutput.shp 
              ${TEMP}/coVectorDataExtractROIOutput.shp 
	 otbVectorDataExtractROI	
	${INPUTDATA}/ToulousePoints-examples.shp   #${LARGEDATA}/TOULOUSE/QuickBird/GIS_FILES/000000128955_01_ORDER_SHAPE.shp
	${TEMP}/coVectorDataExtractROIOutput.shp
	 374369.48850211215904 4828951.58612491376698  # Origin of the CartoRegion 
         1000.25 25000.2                               # Size of the Cartoregion
 )

# -------            otb::MultiChannelExtractROI   ------------------------------

ADD_TEST(coTuMultiChannelROINew ${COMMON_TESTS2}
         otbMultiChannelExtractROINew)

ADD_TEST(coTvMultiChannelROI_RGB2RGB_PNG ${COMMON_TESTS2}
  --compare-image ${TOL}   ${INPUTDATA}/couleurs_extrait.png
                           ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/couleurs_extrait.png
         ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG.png
         -channels 1 2 3 )
#         -sizeX 800 -sizeY 600 )

ADD_TEST(coTvMultiChannelROI_RGB2RGB_PNG2 ${COMMON_TESTS2}
  --compare-image ${TOL}   ${BASELINE}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_123.png
                           ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_123.png
         otbMultiChannelExtractROI
         -uchar ${INPUTDATA}/couleurs_extrait.png
         -uchar ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_123.png
         -startX 300 -startY 10 -sizeX 250 -sizeY 50 -channels 1 2 3  )

ADD_TEST(coTvMultiChannelROI_RGB2RGB_PNG3 ${COMMON_TESTS2}
  --compare-image ${TOL}  ${BASELINE}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_321.png
                    ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_321.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/couleurs_extrait.png
         ${TEMP}/coMultiChannelExtractROI_RGB2RGB_PNG_300_10_250_50_channels_321.png
         -startX 300 -startY 10 -sizeX 250 -sizeY 50  -channels 3 2 1  )
ADD_TEST(coTvMultiChannelROI_RGB2NG_PNG1 ${COMMON_TESTS2}
  --compare-image ${TOL}  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
                    ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/couleurs_extrait.png
         ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
         -startX 300 -startY 10 -sizeX 250 -sizeY 50 -channels 1 )


ADD_TEST(coTvMultiChannelROI_RGB2NG_PNG2 ${COMMON_TESTS2}
  --compare-image ${TOL}  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_2.png
                    ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_2.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/couleurs_extrait.png
         ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_2.png
         -startX 300 -startY 10 -sizeX 250 -sizeY 50  -channels 2 )
ADD_TEST(coTvMultiChannelROI_RGB2NG_PNG3 ${COMMON_TESTS2}
  --compare-image ${TOL}  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_3.png
                    ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_3.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/couleurs_extrait.png
         ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_3.png
         -startX 300 -startY 10 -sizeX 250 -sizeY 50  -channels 3 )

ADD_TEST(coTvMultiChannelROI_5c_BSQ ${COMMON_TESTS2}
#  --compare-binary  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_poupees_303_2_134_330_channels_12321.c1
#                    ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_poupees_303_2_134_330_channels_12321.c1
  --compare-image ${TOL} ${BASELINE}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_12321.hdr
                    ${TEMP}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_12321.hdr
         otbMultiChannelExtractROI
         ${INPUTDATA}/poupees.hdr
         ${TEMP}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_12321.hdr
         -startX 303 -startY 2 -sizeX 134  -sizeY 330 -channels 1 2 3 2 1 )

ADD_TEST(coTvMultiChannelROI_3c_BSQ ${COMMON_TESTS2}
#  --compare-binary  ${BASELINE}/coMultiChannelExtractROI_RGB2NG_PNG_poupees_303_2_134_330_channels_345.c1
#                    ${TEMP}/coMultiChannelExtractROI_RGB2NG_PNG_poupees_303_2_134_330_channels_345.c1
  --compare-image  ${TOL}  ${BASELINE}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_345.hdr
                    ${TEMP}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_345.hdr
         otbMultiChannelExtractROI
         ${TEMP}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_12321.hdr
         ${TEMP}/coMultiChannelExtractROI_RGB_ENVI_poupees_303_2_134_330_channels_345.hdr
         -startX 0 -startY 0 -sizeX 134  -sizeY 330 -channels 3 4 5 )


ADD_TEST(coTvTestMultiExtractMultiUpdate ${COMMON_TESTS2}
--compare-image  ${TOL}
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

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbCommonTests3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::MultiToMonoChannelExtractROI   ------------------------------

ADD_TEST(coTuMultiToMonoROINew ${COMMON_TESTS3}
         otbMultiToMonoChannelExtractROINew)

ADD_TEST(coTvMultiToMonoROI_RGB2NG_PNG ${COMMON_TESTS3}
  --compare-image ${TOL}   ${BASELINE}/coMultiToMonoChannelExtractROI_RGB2NG_PNG_300_10_250_50_channel_1.png
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
	--compare-image ${TOL}   ${INPUTDATA}/amst.png
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
         --compare-image ${TOL}
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
--compare-image ${TOL}
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

# -------            otb::ArcSpatialObject -------------------------------------------
ADD_TEST(coTuArcSpatialObjectNew ${COMMON_TESTS5}
         otbArcSpatialObjectNew)

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
--compare-ascii ${EPS}
		${BASELINE_FILES}/otbPolygonTest.txt
                ${TEMP}/otbPolygonTest.txt
         otbPolygon
	        ${TEMP}/otbPolygonTest.txt
		2 10 7 10 7 9 5 9 5 8 7 8 7 6 9 6 9 5 7 5 7 3 2 3
		next 6 10 11 10 11 2 4 2 3 4 8 4 8 7 6 7
)

#-------- otb::PolyLineImageIterator -----------------------------------------

 ADD_TEST(coTvPolyLineImageIterator ${COMMON_TESTS6}
--compare-image ${TOL}
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
--compare-image ${TOL}
		${BASELINE}/coTvDrawPathListFilterOutput.png
		${TEMP}/coTvDrawPathListFilterOutput.png
		otbDrawPathListFilter
		512 512 20
		${TEMP}/coTvDrawPathListFilterOutput.png
)

ADD_TEST(coTvDrawPathListFilterWithValue ${COMMON_TESTS6}
--compare-image ${TOL}
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
--compare-image ${TOL}
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
--compare-image ${TOL}
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
--compare-ascii ${EPS}
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

# -------------  otb::MirrorBoundaryCondition ----------------------------
ADD_TEST(coTvMirrorBoundaryCondition ${COMMON_TESTS7}
     otbMirrorBoundaryConditionTest
     ${INPUTDATA}/qb_RoadExtract.img
     10
)

# -------------  otb::StandardFilterWatcherNew ----------------------------
ADD_TEST(coTuStandardFilterWatcherNew ${COMMON_TESTS7}
     otbStandardFilterWatcherNew
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

ADD_TEST(coTvVectorData ${COMMON_TESTS7}
     otbVectorData
)

ADD_TEST(coTuShiftScaleImageAdaptorNew ${COMMON_TESTS7}
     otbShiftScaleImageAdaptorNew
)

ADD_TEST(coTvShiftScaleImageAdaptor ${COMMON_TESTS7}
  --compare-image 10e-9
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
--compare-image ${TOL}
	        ${INPUTDATA}/poupees.tif
                ${TEMP}/coTvUnaryFunctorNeighborhoodWithOffsetImageFilter.tif
    otbUnaryFunctorNeighborhoodWithOffsetImageFilter
        ${INPUTDATA}/poupees.tif
	${TEMP}/coTvUnaryFunctorNeighborhoodWithOffsetImageFilter.tif
	2 # radius
	2 # offset[0]
	2  #offset[1]
)


#------------------------ otb::CartoRegion---------------------------------

ADD_TEST(coTuCartoRegionNew ${COMMON_TESTS7}
                     otbCartoRegionNew )

ADD_TEST(ioTvCartoRegion ${COMMON_TESTS7}
	 otbCartoRegion
	 1.21 2.33 58.47 59.49  #size et index of the region
	 0. 0.                  # Point to check 
 )

# -------       Fichiers sources CXX -----------------------------------
SET(BasicCommon_SRCS1
otbSystemTest.cxx
otbTestCommandLineArgumentParser.cxx
#otbTestCommandLineArgumentParserGUI.cxx
otbTestCommandLineArgumentParserHelp.cxx
otbTestCommandLineArgumentParserList.cxx
otbTestCommandLineArgumentParserWithError.cxx
)
SET(BasicCommon_SRCS2
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
otbMultiToMonoChannelExtractROI.cxx
otbMultiToMonoChannelExtractROINew.cxx
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
otbImageListToImageListFilterNew.cxx
otbConcatenateVectorImageFilterNew.cxx
otbConcatenateVectorImageFilter.cxx
otbImageToVectorImageCastFilterNew.cxx
otbImageToVectorImageCastFilter.cxx
otbImageToPathFilterNew.cxx
otbSpatialObjectSourceNew.cxx
otbLineSpatialObjectNew.cxx
otbArcSpatialObjectNew.cxx
otbPathListToPathListFilterNew.cxx
)
SET(BasicCommon_SRCS6
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
otbGenericInterpolateImageFunctionNew.cxx
otbMirrorBoundaryConditionTest.cxx
otbStandardFilterWatcherNew.cxx
otbDataNodeTest.cxx
otbVectorDataNew.cxx
otbVectorData.cxx
otbShiftScaleImageAdaptorNew.cxx
otbShiftScaleImageAdaptor.cxx
otbStandardWriterWatcher.cxx
otbUnaryFunctorNeighborhoodWithOffsetImageFilterNew.cxx
otbUnaryFunctorNeighborhoodWithOffsetImageFilter.cxx
otbCartoRegionNew.cxx
otbCartoRegion.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

# ${TIFF_LIBRARY}

ADD_EXECUTABLE(otbCommonTests1 otbCommonTests1.cxx ${BasicCommon_SRCS1})
TARGET_LINK_LIBRARIES(otbCommonTests1 OTBIO)

ADD_EXECUTABLE(otbCommonTests2 otbCommonTests2.cxx ${BasicCommon_SRCS2})
TARGET_LINK_LIBRARIES(otbCommonTests2 OTBIO)

ADD_EXECUTABLE(otbCommonTests3 otbCommonTests3.cxx ${BasicCommon_SRCS3})
TARGET_LINK_LIBRARIES(otbCommonTests3 OTBIO)

ADD_EXECUTABLE(otbCommonTests4 otbCommonTests4.cxx ${BasicCommon_SRCS4})
TARGET_LINK_LIBRARIES(otbCommonTests4 OTBIO)

ADD_EXECUTABLE(otbCommonTests5 otbCommonTests5.cxx ${BasicCommon_SRCS5})
TARGET_LINK_LIBRARIES(otbCommonTests5 OTBIO)

ADD_EXECUTABLE(otbCommonTests6 otbCommonTests6.cxx ${BasicCommon_SRCS6})
TARGET_LINK_LIBRARIES(otbCommonTests6 OTBIO)

ADD_EXECUTABLE(otbCommonTests7 otbCommonTests7.cxx ${BasicCommon_SRCS7})
TARGET_LINK_LIBRARIES(otbCommonTests7 OTBIO)


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
