if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


set(VISU_TESTS1 ${CXX_TEST_PATH}/otbVisuTests1)
set(VISU_TESTS2 ${CXX_TEST_PATH}/otbVisuTests2)

if(OTB_USE_DEPRECATED)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbVISU_Tests1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------       otb::ImageWidgetBase -----------------------------------

add_test(viTuImageWidgetBaseNew ${VISU_TESTS1}
        otbImageWidgetBaseNew
        )

# -------       otb::FixedSizeFullImageWidget -----------------------------------

add_test(viTuFixedSizeFullImageWidgetNew ${VISU_TESTS1}
        otbFixedSizeFullImageWidgetNew
        )

add_test(viTvFixedSizFullImageWidget ${VISU_TESTS1}
        otbFixedSizeFullImageWidget
        ${INPUTDATA}/poupees.png
        )

# -------       otb::FullResolutionImageWidget -----------------------------------

add_test(viTuFullResolutionImageWidgetNew ${VISU_TESTS1}
        otbFullResolutionImageWidgetNew
        )

add_test(viTvFullResolutionImageWidget ${VISU_TESTS1}
        otbFullResolutionImageWidget
        ${INPUTDATA}/poupees.png
        )

# -------   otb::ZoomableImageWidget  -----------------------------------

add_test(viTuZoomableImageWidgetNew ${VISU_TESTS1}
        otbZoomableImageWidgetNew
        )

add_test(viTvZoomableImageWidget ${VISU_TESTS1}
        otbZoomableImageWidget
        ${INPUTDATA}/poupees.png
        )

# -------   otb::ImageViewer  -----------------------------------

add_test(viTuImageViewerNew ${VISU_TESTS1}
        otbImageViewerNew
        )

add_test(viTvImageViewerWithOtbVectorImage ${VISU_TESTS1}
        otbImageViewer
        ${INPUTDATA}/couleurs.jpg 3
        )

add_test(viTvImageViewerWithOtbImage ${VISU_TESTS1}
        otbImageViewerWithOtbImage
        ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
        )

if(OTB_DATA_USE_LARGEINPUT)
add_test(viTvImageViewerWithLargeOtbVectorImage ${VISU_TESTS1}
        otbImageViewer
        ${IMAGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF 0
        )

add_test(viTvModulusImageViewerWithComplexImage ${VISU_TESTS1}
        otbImageViewer
        ${IMAGEDATA}/RADARSAT1/GOMA2/SCENE01/DAT_01.001 1
        )

add_test(viTvPhaseImageViewerWithComplexImage ${VISU_TESTS1}
        otbImageViewer
        ${IMAGEDATA}/RADARSAT1/GOMA2/SCENE01/DAT_01.001 2
        )

add_test(viTvImageViewerWithLargeOtbImage ${VISU_TESTS1}
        otbImageViewerWithOtbImage
        ${IMAGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF 0
        )
endif()

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbVISU_Tests2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
add_test(viTvImageViewerWithMultiBandFilter ${VISU_TESTS2}
        otbImageViewerWithMultiBandFilter
        ${INPUTDATA}/couleurs.jpg
        )

add_test(viTvDoubleImageViewerWithLink ${VISU_TESTS2}
        otbDoubleImageViewerWithLink
        ${INPUTDATA}/couleurs.jpg
        ${INPUTDATA}/couleurs.jpg
        )

# -------   otb::ImageToGrayscaleAnaglyphImageFilter  -----------------------------------

add_test(viTuImageToGrayscaleAnaglyphImageFilterNew ${VISU_TESTS2}
        otbImageToGrayscaleAnaglyphImageFilterNew
)

add_test(viTvImageToGrayscaleAnaglyphImageFilter ${VISU_TESTS2}
        --compare-image  ${EPSILON_6}
        ${BASELINE}/viTvGrayscaleAnaglyphFilterResult.tif
        ${TEMP}/viTvGrayscaleAnaglyphFilterResult.tif
        otbImageToGrayscaleAnaglyphImageFilter
        ${INPUTDATA}/anaglyphInput1.tif
        ${INPUTDATA}/anaglyphInput2.tif
        ${TEMP}/viTvGrayscaleAnaglyphFilterResult.tif
)

# -------   otb::VectorImageToColorAnaglyphVectorImageFilter  ---------------------------

add_test(viTuVectorImageToColorAnaglyphVectorImageFilterNew ${VISU_TESTS2}
        otbVectorImageToColorAnaglyphVectorImageFilterNew
)

add_test(viTvVectorImageToColorAnaglyphVectorImageFilter ${VISU_TESTS2}
        --compare-image ${EPSILON_6}
        ${BASELINE}/viTvColorAnaglyphFilterResult.tif
        ${TEMP}/viTvColorAnaglyphFilterResult.tif
        otbVectorImageToColorAnaglyphVectorImageFilter
        ${INPUTDATA}/anaglyphInput1.tif
        ${INPUTDATA}/anaglyphInput2.tif
        ${TEMP}/viTvColorAnaglyphFilterResult.tif
)

# -------  otb::ImageWidgetPolygonForm -------------------------------------------
add_test(viTuImageWidgetPolygonFormNew ${VISU_TESTS2}
         otbImageWidgetPolygonFormNew)

add_test(viTuImageWidgetPolygonForm ${VISU_TESTS2}
         otbImageWidgetPolygonForm
                      ${INPUTDATA}/poupees.tif
                20 100 70 100 70 90 50 90 50 80 70 80 70 60 90 60 90 50 70 50 70 30 20 30
)

# -------       otb::HistogramAndTransferFunctionWidgtet -----------------------------------

add_test(viTuHistogramAndTransferFunctionWidgetNew ${VISU_TESTS2}
        otbHistogramAndTransferFunctionWidgetNew
        )

add_test(viTvHistogramAndTransferFunctionWidget ${VISU_TESTS2}
        otbHistogramAndTransferFunctionWidget
        ${INPUTDATA}/GomaAvant.png
        )

# -------       otb::ImageWidgetTransferFunctions -----------------------------------

add_test(viTuImageWidgetTransferFunctionsNew ${VISU_TESTS2}
        otbImageWidgetTransferFunctionsNew
        )

add_test(viTvImageWidgetTransferFunctions ${VISU_TESTS2}
--compare-ascii ${EPSILON_6}
                ${BASELINE_FILES}/viTvImageWidgetTransferFunctionsOutput.txt
                ${TEMP}/viTvImageWidgetTransferFunctionsOutput.txt
        otbImageWidgetTransferFunctions
        ${TEMP}/viTvImageWidgetTransferFunctionsOutput.txt
        )

#----------   otb::ImageViewer (Testing overlay, related to bug 35 -----------
add_test(viTvImageViewerWithOverlay ${VISU_TESTS2}
        otbImageViewerWithOverlay
                ${INPUTDATA}/couleurs.tif
                128
                128
)

# -------       Fichiers sources CXX -----------------------------------
set(BasicVisu_SRCS1
otbVisuTests1.cxx
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
set(BasicVisu_SRCS2
otbVisuTests2.cxx
otbImageViewerWithMultiBandFilter.cxx
otbDoubleImageViewerWithLink.cxx
otbImageToGrayscaleAnaglyphImageFilterNew.cxx
otbImageToGrayscaleAnaglyphImageFilter.cxx
otbVectorImageToColorAnaglyphVectorImageFilterNew.cxx
otbVectorImageToColorAnaglyphVectorImageFilter.cxx
otbImageWidgetPolygonFormNew.cxx
otbImageWidgetPolygonForm.cxx
otbHistogramAndTransferFunctionWidgetNew.cxx
otbHistogramAndTransferFunctionWidget.cxx
otbImageWidgetTransferFunctions.cxx
otbImageWidgetTransferFunctionsNew.cxx
otbImageViewerWithOverlay.cxx
)

OTB_ADD_EXECUTABLE(otbVisuTests1 "${BasicVisu_SRCS1}" "OTBVisu;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbVisuTests2 "${BasicVisu_SRCS2}" "OTBVisu;OTBIO;OTBTesting")

endif()

endif()
