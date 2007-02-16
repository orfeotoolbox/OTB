IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)

SET(VISU_TESTS ${CXX_TEST_PATH}/otbVisuTests)

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
otbImageWidgetBaseNew.cxx
otbFixedSizeFullImageWidgetNew.cxx
otbFixedSizeFullImageWidget.cxx
otbFullResolutionImageWidgetNew.cxx
otbFullResolutionImageWidget.cxx
otbZoomableImageWidgetNew.cxx
otbZoomableImageWidget.cxx
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}") 

ADD_EXECUTABLE(otbVisuTests otbVisuTests.cxx ${BasicVisu_SRCS})
TARGET_LINK_LIBRARIES(otbVisuTests OTBVisu OTBIO OTBCommon gdal ITKIO ITKCommon)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )


