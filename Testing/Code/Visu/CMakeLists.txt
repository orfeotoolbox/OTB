IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(IMAGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)
SET(EPSILON 0.000001)


SET(VISU_TESTS1 ${CXX_TEST_PATH}/otbVisuTests1)
SET(VISU_TESTS2 ${CXX_TEST_PATH}/otbVisuTests2)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbVISU_Tests1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# -------       otb::ImageWidgetBase -----------------------------------

ADD_TEST(viTuImageWidgetBaseNew ${VISU_TESTS1} 
        otbImageWidgetBaseNew
        )

# -------       otb::FixedSizeFullImageWidget -----------------------------------

ADD_TEST(viTuFixedSizeFullImageWidgetNew ${VISU_TESTS1} 
        otbFixedSizeFullImageWidgetNew
        )

ADD_TEST(viTvFixedSizFullImageWidget ${VISU_TESTS1} 
        otbFixedSizeFullImageWidget
	${INPUTDATA}/poupees.png
        )

# -------       otb::FullResolutionImageWidget -----------------------------------

ADD_TEST(viTuFullResolutionImageWidgetNew ${VISU_TESTS1} 
        otbFullResolutionImageWidgetNew
        )

ADD_TEST(viTvFullResolutionImageWidget ${VISU_TESTS1} 
        otbFullResolutionImageWidget
	${INPUTDATA}/poupees.png
        )

# -------   otb::ZoomableImageWidget  -----------------------------------

ADD_TEST(viTuZoomableImageWidgetNew ${VISU_TESTS1} 
        otbZoomableImageWidgetNew
        )

ADD_TEST(viTvZoomableImageWidget ${VISU_TESTS1} 
        otbZoomableImageWidget
	${INPUTDATA}/poupees.png
        )

# -------   otb::ImageViewer  -----------------------------------

ADD_TEST(viTuImageViewerNew ${VISU_TESTS1} 
        otbImageViewerNew
        )

ADD_TEST(viTvImageViewerWithOtbVectorImage ${VISU_TESTS1} 
        otbImageViewer
	${INPUTDATA}/couleurs.jpg 3
        )

ADD_TEST(viTvImageViewerWithOtbImage ${VISU_TESTS1} 
        otbImageViewerWithOtbImage
        ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
        )

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(viTvImageViewerWithLargeOtbVectorImage ${VISU_TESTS1} 
        otbImageViewer
	${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF 0
        )

ADD_TEST(viTvModulusImageViewerWithComplexImage ${VISU_TESTS1} 
        otbImageViewer
	${IMAGEDATA}/RADARSAT2/SCENE01/DAT_01.001 1
        )

ADD_TEST(viTvPhaseImageViewerWithComplexImage ${VISU_TESTS1} 
        otbImageViewer
	${IMAGEDATA}/RADARSAT2/SCENE01/DAT_01.001 2
        )

ADD_TEST(viTvImageViewerWithLargeOtbImage ${VISU_TESTS1} 
        otbImageViewerWithOtbImage
	${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF 0
        )
ENDIF(OTB_DATA_USE_LARGEINPUT)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbVISU_Tests2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
ADD_TEST(viTvImageViewerWithMultiBandFilter ${VISU_TESTS2} 
        otbImageViewerWithMultiBandFilter
	${INPUTDATA}/couleurs.jpg
        )

ADD_TEST(viTvDoubleImageViewerWithLink ${VISU_TESTS2} 
        otbDoubleImageViewerWithLink
	${INPUTDATA}/couleurs.jpg
	${INPUTDATA}/couleurs.jpg
        )

# -------   otb::ImageToGrayscaleAnaglyphImageFilter  -----------------------------------

ADD_TEST(viTuImageToGrayscaleAnaglyphImageFilterNew ${VISU_TESTS2}
	otbImageToGrayscaleAnaglyphImageFilterNew
)

ADD_TEST(viTvImageToGrayscaleAnaglyphImageFilter ${VISU_TESTS2}
	--compare-image  ${EPSILON} 
	${BASELINE}/viTvGrayscaleAnaglyphFilterResult.tif
	${TEMP}/viTvGrayscaleAnaglyphFilterResult.tif
	otbImageToGrayscaleAnaglyphImageFilter
	${INPUTDATA}/anaglyphInput1.tif
	${INPUTDATA}/anaglyphInput2.tif
	${TEMP}/viTvGrayscaleAnaglyphFilterResult.tif
)

# -------   otb::VectorImageToColorAnaglyphVectorImageFilter  ---------------------------

ADD_TEST(viTuVectorImageToColorAnaglyphVectorImageFilterNew ${VISU_TESTS2}
	otbVectorImageToColorAnaglyphVectorImageFilterNew
)

ADD_TEST(viTvVectorImageToColorAnaglyphVectorImageFilter ${VISU_TESTS2}
	--compare-image ${EPSILON}  
	${BASELINE}/viTvColorAnaglyphFilterResult.tif
	${TEMP}/viTvColorAnaglyphFilterResult.tif
	otbVectorImageToColorAnaglyphVectorImageFilter
	${INPUTDATA}/anaglyphInput1.tif
	${INPUTDATA}/anaglyphInput2.tif
	${TEMP}/viTvColorAnaglyphFilterResult.tif
)

# -------  otb::ImageWidgetPolygonForm -------------------------------------------
ADD_TEST(viTuImageWidgetPolygonFormNew ${VISU_TESTS2} 
         otbImageWidgetPolygonFormNew)
  
ADD_TEST(viTuImageWidgetPolygonForm ${VISU_TESTS2} 
         otbImageWidgetPolygonForm
	      	${INPUTDATA}/poupees.tif	
		20 100 70 100 70 90 50 90 50 80 70 80 70 60 90 60 90 50 70 50 70 30 20 30 
)

# -------       otb::HistogramAndTransferFunctionWidgtet -----------------------------------

ADD_TEST(viTuHistogramAndTransferFunctionWidgetNew ${VISU_TESTS2} 
        otbHistogramAndTransferFunctionWidgetNew
        )

ADD_TEST(viTvHistogramAndTransferFunctionWidget ${VISU_TESTS2} 
        otbHistogramAndTransferFunctionWidget
	${INPUTDATA}/GomaAvant.png
        )

# -------       otb::ImageWidgetTransferFunctions -----------------------------------

ADD_TEST(viTuImageWidgetTransferFunctionsNew ${VISU_TESTS2} 
        otbImageWidgetTransferFunctionsNew
        )

ADD_TEST(viTvImageWidgetTransferFunctions ${VISU_TESTS2} 
--compare-ascii ${EPSILON}
		${BASELINE_FILES}/viTvImageWidgetTransferFunctionsOutput.txt
		${TEMP}/viTvImageWidgetTransferFunctionsOutput.txt
        otbImageWidgetTransferFunctions
	${TEMP}/viTvImageWidgetTransferFunctionsOutput.txt
        )

# -------       Fichiers sources CXX -----------------------------------
SET(BasicVisu_SRCS1
otbImageWidgetBaseNew.cxx
otbFixedSizeFullImageWidgetNew.cxx
otbFixedSizeFullImageWidget.cxx
otbFullResolutionImageWidgetNew.cxx
otbFullResolutionImageWidget.cxx
otbZoomableImageWidgetNew.cxx
otbZoomableImageWidget.cxx
otbImageViewer.cxx
otbImageViewerWithOtbImage.cxx
otbImageViewerNew.cxx
)
SET(BasicVisu_SRCS2
otbImageViewerWithMultiBandFilter.cxx
otbDoubleImageViewerWithLink.cxx
otbImageToGrayscaleAnaglyphImageFilterNew.cxx
otbImageToGrayscaleAnaglyphImageFilter.cxx
otbVectorImageToColorAnaglyphVectorImageFilterNew.cxx
otbVectorImageToColorAnaglyphVectorImageFilter.cxx
otbAlternateViewerTest.cxx
otbImageWidgetPolygonFormNew.cxx
otbImageWidgetPolygonForm.cxx
otbHistogramAndTransferFunctionWidgetNew.cxx
otbHistogramAndTransferFunctionWidget.cxx
otbImageWidgetTransferFunctions.cxx
otbImageWidgetTransferFunctionsNew.cxx
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}") 

ADD_EXECUTABLE(otbVisuTests1 otbVisuTests1.cxx ${BasicVisu_SRCS1})
TARGET_LINK_LIBRARIES(otbVisuTests1 OTBVisu OTBIO)
ADD_EXECUTABLE(otbVisuTests2 otbVisuTests2.cxx ${BasicVisu_SRCS2})
TARGET_LINK_LIBRARIES(otbVisuTests2 OTBVisu OTBIO)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
