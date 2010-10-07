IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(EXAMPLESDATA ${OTB_DATA_ROOT}/Examples)
IF(OTB_DATA_USE_LARGEINPUT)
  SET(LARGEINPUTDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


# Tolerances
SET(NOTOL 0.0)

# Test programs
SET(VISUALIZATION_TESTS1 ${CXX_TEST_PATH}/otbVisualizationTests1)

#------------ otb::ImageWidget -------------------------

ADD_TEST(vrTuImageWidgetNew ${VISUALIZATION_TESTS1}
otbImageWidgetNew
)

ADD_TEST(vrTvImageWidgetWithoutGlAcceleration ${VISUALIZATION_TESTS1}
otbImageWidget
${INPUTDATA}/poupees.png 0
)

IF(OTB_GL_USE_ACCEL)
ADD_TEST(vrTvImageWidgetWithGlAcceleration ${VISUALIZATION_TESTS1}
otbImageWidget
${INPUTDATA}/poupees.png 1
)
ENDIF(OTB_GL_USE_ACCEL)

#------------ otb::ImageWidgetActionHandler -------------

ADD_TEST(vrTuImageWidgetActionHandlerNew ${VISUALIZATION_TESTS1}
otbImageWidgetActionHandlerNew
)

#------------ otb::ImageWidgetController ----------------

ADD_TEST(vrTuImageWidgetControllerNew ${VISUALIZATION_TESTS1}
otbImageWidgetControllerNew
)

ADD_TEST(vrTvImageWidgetController ${VISUALIZATION_TESTS1}
otbImageWidgetController
)

#------------ otb::ImageLayer ----------------------------

ADD_TEST(vrTuImageLayerNew ${VISUALIZATION_TESTS1}
otbImageLayerNew
)

ADD_TEST(vrTvImageLayerScalar ${VISUALIZATION_TESTS1}
--compare-n-images ${NOTOL} 3
${BASELINE}/vrTvImageLayerScalarQuicklookOutput.png
${TEMP}/vrTvImageLayerScalarQuicklookOutput.png
${BASELINE}/vrTvImageLayerScalarExtractOutput.png
${TEMP}/vrTvImageLayerScalarExtractOutput.png
${BASELINE}/vrTvImageLayerScalarScaledExtractOutput.png
${TEMP}/vrTvImageLayerScalarScaledExtractOutput.png
otbImageLayerScalar
${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
${TEMP}/vrTvImageLayerScalarQuicklookOutput.png
${TEMP}/vrTvImageLayerScalarExtractOutput.png
${TEMP}/vrTvImageLayerScalarScaledExtractOutput.png
2 127 417
)

ADD_TEST(vrTvImageLayerVector ${VISUALIZATION_TESTS1}
--compare-n-images ${NOTOL} 3
${BASELINE}/vrTvImageLayerVectorQuicklookOutput.png
${TEMP}/vrTvImageLayerVectorQuicklookOutput.png
${BASELINE}/vrTvImageLayerVectorExtractOutput.png
${TEMP}/vrTvImageLayerVectorExtractOutput.png
${BASELINE}/vrTvImageLayerVectorScaledExtractOutput.png
${TEMP}/vrTvImageLayerVectorScaledExtractOutput.png
otbImageLayerVector
${INPUTDATA}/couleurs.tif
${TEMP}/vrTvImageLayerVectorQuicklookOutput.png
${TEMP}/vrTvImageLayerVectorExtractOutput.png
${TEMP}/vrTvImageLayerVectorScaledExtractOutput.png
10 # sampling rate
50 200 #min/max
50 200 #min/max
50 200 #min/max
)

#------------ otb::StandardRenderingFunction ------------

ADD_TEST(vrTuStandardRenderingFunctionNew ${VISUALIZATION_TESTS1}
otbStandardRenderingFunctionNew
)

#------------ otb::RenderingImageFilter ------------

ADD_TEST(vrTuRenderingImageFilterNew ${VISUALIZATION_TESTS1}
otbRenderingImageFilterNew
)

ADD_TEST(vrTvRenderingImageFilterScalar ${VISUALIZATION_TESTS1}
--compare-image ${NOTOL}
${BASELINE}/vrTvRenderingImageFilterScalarOutput.png
${TEMP}/vrTvRenderingImageFilterScalarOutput.png
otbRenderingImageFilterScalar
${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
${TEMP}/vrTvRenderingImageFilterScalarOutput.png
127 417
)

ADD_TEST(vrTvRenderingImageFilterVector ${VISUALIZATION_TESTS1}
--compare-image ${NOTOL}
${BASELINE}/vrTvRenderingImageFilterVectorOutput.png
${TEMP}/vrTvRenderingImageFilterVectorOutput.png
otbRenderingImageFilterVector
${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
${TEMP}/vrTvRenderingImageFilterVectorOutput.png
2 1 0 #channel to extract
117 390 #min max
223 430 #min max
190 287 #min max
)

ADD_TEST(vrTvRenderingImageFilterVectorWithExpNegativeTransfer ${VISUALIZATION_TESTS1}
--compare-image ${NOTOL}
${BASELINE}/vrTvRenderingImageFilterVectorWithExpNegativeTransferOutput.png
${TEMP}/vrTvRenderingImageFilterVectorWithExpNegativeTransferOutput.png
otbRenderingImageFilterVectorWithExpNegativeTransfer
${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
${TEMP}/vrTvRenderingImageFilterVectorWithExpNegativeTransferOutput.png
0 1 2 #channel to extract
0.05
179 802 #min max
100 600 #min max
200 700 #min max
)

ADD_TEST(vrTvRenderingImageFilterAmplitude ${VISUALIZATION_TESTS1}
--compare-image ${NOTOL}
${BASELINE}/vrTvRenderingImageFilterAmplitudeOutput.png
${TEMP}/vrTvRenderingImageFilterAmplitudeOutput.png
otbRenderingImageFilterAmplitude
${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
${TEMP}/vrTvRenderingImageFilterAmplitudeOutput.png
0 1 #channel to use to compute module
50 800
)

ADD_TEST(vrTvRenderingImageFilterPhase ${VISUALIZATION_TESTS1}
--compare-image ${NOTOL}
${BASELINE}/vrTvRenderingImageFilterPhaseOutput.png
${TEMP}/vrTvRenderingImageFilterPhaseOutput.png
otbRenderingImageFilterPhase
${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
${TEMP}/vrTvRenderingImageFilterPhaseOutput.png
0 1 #channel to use to compute module
0 1.57 #not very significant with an optical image
)
#------------ otb::UniformAlphaBlendingFunction ------------

ADD_TEST(vrTuUniformAlphaBlendingFunctionNew ${VISUALIZATION_TESTS1}
otbUniformAlphaBlendingFunctionNew
)

#------------ otb::MultiplyBlendingFunction ------------

ADD_TEST(vrTuMultiplyBlendingFunctionNew ${VISUALIZATION_TESTS1}
otbMultiplyBlendingFunctionNew
)

#------------ otb::BlendingImageFilter ----------------------

ADD_TEST(vrTuBlendingImageFilterNew ${VISUALIZATION_TESTS1}
otbBlendingImageFilterNew
)

ADD_TEST(vrTvBlendingImageFilter ${VISUALIZATION_TESTS1}
otbBlendingImageFilter
${EXAMPLESDATA}/SpotBefore.png
${EXAMPLESDATA}/SpotAfter.png
${TEMP}/vrTvBlendingImageFilterOutput.png
)

#------------ otb::LayerBasedModel ------------

ADD_TEST(vrTuLayerBasedModelNew ${VISUALIZATION_TESTS1}
otbLayerBasedModelNew
)

#------------ otb::ImageLayerRenderingModel ------------

ADD_TEST(vrTuImageLayerRenderingModelNew ${VISUALIZATION_TESTS1}
otbImageLayerRenderingModelNew
)

ADD_TEST(vrTvImageLayerRenderingModelSingleLayer ${VISUALIZATION_TESTS1}
--compare-n-images ${NOTOL} 3
${BASELINE}/vrTvImageLayerRenderingModelSingleLayerQuicklookOutput.png
${TEMP}/vrTvImageLayerRenderingModelSingleLayerQuicklookOutput.png
${BASELINE}/vrTvImageLayerRenderingModelSingleLayerExtractOutput.png
${TEMP}/vrTvImageLayerRenderingModelSingleLayerExtractOutput.png
${BASELINE}/vrTvImageLayerRenderingModelSingleLayerScaledExtractOutput.png
${TEMP}/vrTvImageLayerRenderingModelSingleLayerScaledExtractOutput.png
otbImageLayerRenderingModelSingleLayer
${INPUTDATA}/couleurs.tif
${TEMP}/vrTvImageLayerRenderingModelSingleLayerQuicklookOutput.png
${TEMP}/vrTvImageLayerRenderingModelSingleLayerExtractOutput.png
${TEMP}/vrTvImageLayerRenderingModelSingleLayerScaledExtractOutput.png
10
)

#------------ otb::ImageLayerGenerator ------------

ADD_TEST(vrTuImageLayerGeneratorNew ${VISUALIZATION_TESTS1}
otbImageLayerGeneratorNew
)

ADD_TEST(vrTvImageLayerGeneratorScalar ${VISUALIZATION_TESTS1}
otbImageLayerGeneratorScalar
${INPUTDATA}/couleurs.tif
1
)

ADD_TEST(vrTvImageLayerGeneratorVector ${VISUALIZATION_TESTS1}
otbImageLayerGeneratorVector
${INPUTDATA}/couleurs.tif
1
)
#------------ otb::ImageView ------------
ADD_TEST(vrTuImageViewNew ${VISUALIZATION_TESTS1}
otbImageViewNew
)

ADD_TEST(vrTvImageViewerEndToEndSingleLayer ${VISUALIZATION_TESTS1}
otbImageViewerEndToEndSingleLayer
${INPUTDATA}/couleurs.tif
200 500 200 0
)

ADD_TEST(vrTvImageViewerEndToEndSingleLayerWithSelectAreaHandler ${VISUALIZATION_TESTS1}
otbImageViewerEndToEndSingleLayerWithSelectAreaHandler
${INPUTDATA}/couleurs.tif
200 500 200 0
)

ADD_TEST(vrTvImageViewerEndToEndTwoLayers ${VISUALIZATION_TESTS1}
otbImageViewerEndToEndTwoLayers
${INPUTDATA}/couleurs.tif
200 500 200 0
)

ADD_TEST(vrTvImageViewerEndToEndGlComponent ${VISUALIZATION_TESTS1}
otbImageViewerEndToEndGlComponent
${INPUTDATA}/couleurs.tif
200 500 200 0
)

#------------ otb::PixelDescriptionModel ------------

ADD_TEST(vrTuPixelDescriptionModelNew ${VISUALIZATION_TESTS1}
otbPixelDescriptionModelNew
)

ADD_TEST(vrTvPixelDescriptionModelSingleLayer ${VISUALIZATION_TESTS1}
otbPixelDescriptionModelSingleLayer
${INPUTDATA}/couleurs.tif
)

#------------ otb::HistogramCurve ------------

ADD_TEST(vrTuHistogramCurveNew ${VISUALIZATION_TESTS1}
otbHistogramCurveNew
)

#------------ otb::Curves2DWidget ------------

ADD_TEST(vrTuCurves2DWidgetNew ${VISUALIZATION_TESTS1}
otbCurves2DWidgetNew
)

ADD_TEST(vrTvCurves2DWidget ${VISUALIZATION_TESTS1}
otbCurves2DWidget 0 0 100 200 0 0 10 10 0
)

ADD_TEST(vrTvCurves2DWidgetWithHistogram ${VISUALIZATION_TESTS1}
otbCurves2DWidgetWithHistogram
${INPUTDATA}/poupees.png
0
)

#------------ otb::VectorDataGlComponent ------------
ADD_TEST(vrTuVectorDataGlComponentNew ${VISUALIZATION_TESTS1}
otbVectorDataGlComponentNew
)

ADD_TEST(vrTvImageWidgetWithVectorDataGlComponent ${VISUALIZATION_TESTS1}
otbImageWidgetWithVectorDataGlComponent
${INPUTDATA}/poupees.png 0
)

IF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(vrTvImageViewerEndToEndWithVectorData ${VISUALIZATION_TESTS1}
otbImageViewerEndToEndWithVectorData
${LARGEINPUTDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
${INPUTDATA}/ToulouseRoad-examples.shp
${INPUTDATA}/DEM/srtm_directory
200 500 200 0
)

ENDIF(OTB_DATA_USE_LARGEINPUT)

#------------ otb::CircleGlComponent ------------
ADD_TEST(vrTuCircleGlComponentNew ${VISUALIZATION_TESTS1}
otbCircleGlComponentNew
)

#------------ otb::CrossGlComponent ------------
ADD_TEST(vrTuCrossGlComponentNew ${VISUALIZATION_TESTS1}
otbCrossGlComponentNew
)


#------------ otb::StandardImageViewer ------------
ADD_TEST(vrTuStandardImageViewerNew ${VISUALIZATION_TESTS1}
otbStandardImageViewerNew
)

#------------ otb::StandardImageViewer ------------
ADD_TEST(vrTuStandardImageViewerRGBNew ${VISUALIZATION_TESTS1}
otbStandardImageViewerRGBNew
)

ADD_TEST(vrTvStandardImageViewer ${VISUALIZATION_TESTS1}
otbStandardImageViewer
${INPUTDATA}/couleurs.tif 0
)

ADD_TEST(vrTvStandardImageViewerSplitted ${VISUALIZATION_TESTS1}
otbStandardImageViewerSplitted
${INPUTDATA}/couleurs.tif 0
)


IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(vrTvStandardImageViewerToulouseWithWaterwaysShp ${VISUALIZATION_TESTS1}
otbStandardImageViewer
${LARGEINPUTDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
0
${LARGEINPUTDATA}/VECTOR/MidiPyrenees/waterways.shp
${INPUTDATA}/DEM/srtm_directory
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

#---------- otb::WidgetManager and Sons--------
ADD_TEST(vrTuPackedWidgetManagerNew ${VISUALIZATION_TESTS1}
otbPackedWidgetManagerNew
)

ADD_TEST(vrTuSplittedWidgetManagerNew ${VISUALIZATION_TESTS1}
otbSplittedWidgetManagerNew
)

#---------- otb::VerticalAsymptote --------
ADD_TEST(vrTuVerticalAsymptoteCurveNew ${VISUALIZATION_TESTS1}
otbVerticalAsymptoteCurveNew
)

#---------- otb::VectorDataModel ----------
ADD_TEST(vrTuVectorDataModelNew ${VISUALIZATION_TESTS1}
otbVectorDataModelNew
)

#IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(vrTvVectorDataModelTest ${VISUALIZATION_TESTS1}
otbVectorDataModelTest
#${LARGEINPUTDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
200 500 200 0
)
#ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(vrTvVectorDataModelAddVectorDataTest ${VISUALIZATION_TESTS1}
otbVectorDataModelAddVectorDataTest
${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
200 500 200 0
${INPUTDATA}/Capitole-Shadows.shp
)

#------------ otb::AmplitudeFuntor ----------------------

ADD_TEST(vrTuAmplitudeFunctorTest ${VISUALIZATION_TESTS1}
	otbAmplitudeFunctorTest
)




# Testing srcs
SET(Visualization_SRCS1
otbVisualizationTests1.cxx
otbImageWidgetNew.cxx
otbImageWidget.cxx
otbImageWidgetActionHandlerNew.cxx
otbImageWidgetControllerNew.cxx
otbImageWidgetController.cxx
otbImageLayerNew.cxx
otbStandardRenderingFunctionNew.cxx
otbRenderingImageFilterNew.cxx
otbRenderingImageFilterScalar.cxx
otbRenderingImageFilterVector.cxx
otbImageLayerScalar.cxx
otbImageLayerVector.cxx
otbUniformAlphaBlendingFunctionNew.cxx
otbBlendingImageFilterNew.cxx
otbBlendingImageFilter.cxx
otbLayerBasedModelNew.cxx
otbImageLayerRenderingModelNew.cxx
otbImageLayerRenderingModelSingleLayer.cxx
otbImageLayerGeneratorNew.cxx
otbImageLayerGeneratorScalar.cxx
otbImageLayerGeneratorVector.cxx
otbMultiplyBlendingFunctionNew.cxx
otbRenderingImageFilterVectorWithExpNegativeTransfer.cxx
otbRenderingImageFilterAmplitude.cxx
otbRenderingImageFilterPhase.cxx
otbImageViewNew.cxx
otbImageViewerEndToEndSingleLayer.cxx
otbImageViewerEndToEndSingleLayerWithSelectAreaHandler.cxx
otbImageViewerEndToEndTwoLayers.cxx
otbImageViewerEndToEndGlComponent.cxx
otbPixelDescriptionModelNew.cxx
otbPixelDescriptionModelSingleLayer.cxx
otbCurves2DWidgetNew.cxx
otbCurves2DWidget.cxx
otbHistogramCurveNew.cxx
otbCurves2DWidgetWithHistogram.cxx
otbVectorDataGlComponentNew.cxx
otbImageWidgetWithVectorDataGlComponent.cxx
otbCircleGlComponentTest.cxx
otbCrossGlComponentTest.cxx
otbImageViewerEndToEndWithVectorData.cxx
otbStandardImageViewerNew.cxx
otbStandardImageViewerRGBNew.cxx
otbStandardImageViewer.cxx
otbPackedWidgetManagerNew.cxx
otbSplittedWidgetManagerNew.cxx
otbVerticalAsymptoteCurveNew.cxx
otbVectorDataModelNew.cxx
otbVectorDataModelTest.cxx
otbVectorDataModelAddVectorDataTest.cxx
otbAmplitudeFunctorTest.cxx
)

OTB_ADD_EXECUTABLE(otbVisualizationTests1 "${Visualization_SRCS1}" "OTBVisualization;OTBIO;OTBTesting")

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
