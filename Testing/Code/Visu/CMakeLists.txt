
IF( NOT OTB_DISABLE_CXX_TESTING )

# SUBDIRS( ImageViewer ImageColorViewer )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)

SET(VISU_TESTS ${CXX_TEST_PATH}/otbVisuTests)

ADD_TEST(viTuImageViewNew ${VISU_TESTS} 
        otbImageView
        )

ADD_TEST(viTuImageViewerNew ${VISU_TESTS} 
        otbImageViewerNew
        )

ADD_TEST(viTvImageViewerRGB ${CXX_TEST_PATH}/otbImageViewer 
        ${INPUTDATA}/sbuv.png
        0 )

ADD_TEST(viTvImageViewerNB ${CXX_TEST_PATH}/otbImageViewer 
        ${INPUTDATA}/cthead1.png
        0 )

# -------       otb::ImageWidgetBase -----------------------------------

ADD_TEST(viTuImageWidgetBaseNew ${VISU_TESTS} 
        otbImageWidgetBaseNew
        )

# -------       otb::FixedSizeFullImageWidget -----------------------------------

ADD_TEST(viTuFixedSizeFullImageWidgetNew ${VISU_TESTS} 
        otbFixedSizeFullImageWidgetNew
        )

ADD_TEST(viTvFixedSizFullImageWidget ${VISU_TESTS} 
        otbFixedSizeFullImageWidget
	${INPUTDATA}/poupees.png
        )

ADD_TEST(viTvFullLargeImageWidget ${VISU_TESTS} 
        otbFullLargeImageWidget
	//${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
	${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
	
        )


# -------       otb::FullResolutionImageWidget -----------------------------------

ADD_TEST(viTuFullResolutionImageWidgetNew ${VISU_TESTS} 
        otbFullResolutionImageWidgetNew
        )

ADD_TEST(viTvFullResolutionImageWidget ${VISU_TESTS} 
        otbFullResolutionImageWidget
	${INPUTDATA}/poupees.png
        )

# -------   otb::ZoomableImageWidget  -----------------------------------

ADD_TEST(viTuZoomableImageWidgetNew ${VISU_TESTS} 
        otbZoomableImageWidgetNew
        )

ADD_TEST(viTvZoomableImageWidget ${VISU_TESTS} 
        otbZoomableImageWidget
	${INPUTDATA}/poupees.png
        )


# -------       Fichiers sources CXX -----------------------------------
SET(BasicVisu_SRCS
otbImageView.cxx
otbImageViewerNew.cxx
otbImageWidgetBaseNew.cxx
otbFixedSizeFullImageWidgetNew.cxx
otbFixedSizeFullImageWidget.cxx
otbFullLargeImageWidget.cxx
otbFullResolutionImageWidgetNew.cxx
otbFullResolutionImageWidget.cxx
otbZoomableImageWidgetNew.cxx
otbZoomableImageWidget.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbVisuTests otbVisuTests.cxx ${BasicVisu_SRCS})
TARGET_LINK_LIBRARIES(otbVisuTests OTBVisu OTBIO OTBCommon gdal ITKIO ITKCommon)

ADD_EXECUTABLE(otbImageViewer otbImageViewer.cxx)
TARGET_LINK_LIBRARIES(otbImageViewer OTBVisu OTBIO OTBCommon gdal ITKIO ITKCommon)

ADD_EXECUTABLE(otbImageViewerWithitkVectorImage otbImageViewerWithitkVectorImage.cxx)
TARGET_LINK_LIBRARIES(otbImageViewerWithitkVectorImage OTBVisu OTBIO OTBCommon gdal ITKIO ITKCommon)

ADD_EXECUTABLE(otbImageViewerWithitkImage otbImageViewerWithitkImage.cxx)
TARGET_LINK_LIBRARIES(otbImageViewerWithitkImage OTBVisu OTBIO OTBCommon gdal ITKIO ITKCommon)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )


# -----------------------------------------------------------------------
# Exemple test simple avec un "main"
# 

