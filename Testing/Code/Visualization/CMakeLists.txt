if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


# Test programs
set(VISUALIZATION_TESTS1 ${CXX_TEST_PATH}/otbVisualizationTests1)
set(VISUALIZATION_TESTS2 ${CXX_TEST_PATH}/otbVisualizationTests2)
set(VISUALIZATION_TESTS3 ${CXX_TEST_PATH}/otbVisualizationTests3)

#---------------------------------------------------------------------------------
#--------------------- otbVisualizationTests1 ------------------------------------
#---------------------------------------------------------------------------------

#------------ otb::ImageWidget -------------------------

add_test(vrTuImageWidgetNew ${VISUALIZATION_TESTS1}
otbImageWidgetNew
)

add_test(vrTvImageWidgetWithoutGlAcceleration ${VISUALIZATION_TESTS1}
otbImageWidget
${INPUTDATA}/poupees.png 0
)

#if(OTB_GL_USE_ACCEL)
#add_test(vrTvImageWidgetWithGlAcceleration ${VISUALIZATION_TESTS1}
#otbImageWidget
#${INPUTDATA}/poupees.png 1
#)
#endif()


add_test(vrTvImageWidgetImageDumpWithoutGlAcceleration ${VISUALIZATION_TESTS1}
--compare-image ${NOTOL}
${BASELINE}/vrTvImageWidgetImageDumpWithoutGlAcceleration.png
${TEMP}/vrTvImageWidgetImageDumpWithoutGlAcceleration.png
otbImageWidgetImageDump
${INPUTDATA}/qb_RoadExtract2sub200x200.tif # input image
0 # use glAcceleration
${TEMP}/vrTvImageWidgetImageDumpWithoutGlAcceleration.png
2 # resize factor
)

#if(OTB_GL_USE_ACCEL)
#add_test(vrTvImageWidgetImageDumpWithGlAcceleration ${VISUALIZATION_TESTS1}
#--compare-image ${NOTOL}
#${BASELINE}/vrTvImageWidgetImageDumpWithGlAcceleration.png
#${TEMP}/vrTvImageWidgetImageDumpWithGlAcceleration.png
#otbImageWidgetImageDump
#${INPUTDATA}/qb_RoadExtract2sub200x200.tif
#1
#${TEMP}/vrTvImageWidgetImageDumpWithGlAcceleration.png
#)
#endif()

#------------ otb::ImageWidgetActionHandler -------------

add_test(vrTuImageWidgetActionHandlerNew ${VISUALIZATION_TESTS1}
otbImageWidgetActionHandlerNew
)

#------------ otb::ImageWidgetController ----------------

add_test(vrTuImageWidgetControllerNew ${VISUALIZATION_TESTS1}
otbImageWidgetControllerNew
)

add_test(vrTvImageWidgetController ${VISUALIZATION_TESTS1}
otbImageWidgetController
)

#------------ otb::ImageLayer ----------------------------

add_test(vrTuImageLayerNew ${VISUALIZATION_TESTS1}
otbImageLayerNew
)

add_test(vrTvImageLayerScalar ${VISUALIZATION_TESTS1}
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

add_test(vrTvImageLayerVector ${VISUALIZATION_TESTS1}
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

if(OTB_DATA_USE_LARGEINPUT)
add_test(vrTvImageLayerCheckChannelDisplay ${VISUALIZATION_TESTS1}
--compare-n-images ${NOTOL} 3
${BASELINE}/vrTvImageLayerCheckChannelDisplayQuicklookOutput.png
${TEMP}/vrTvImageLayerCheckChannelDisplayQuicklookOutput.png
${BASELINE}/vrTvImageLayerCheckChannelDisplayExtractOutput.png
${TEMP}/vrTvImageLayerCheckChannelDisplayExtractOutput.png
${BASELINE}/vrTvImageLayerCheckChannelDisplayScaledExtractOutput.png
${TEMP}/vrTvImageLayerCheckChannelDisplayScaledExtractOutput.png
otbImageLayerVector
${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
${TEMP}/vrTvImageLayerCheckChannelDisplayQuicklookOutput.png
${TEMP}/vrTvImageLayerCheckChannelDisplayExtractOutput.png
${TEMP}/vrTvImageLayerCheckChannelDisplayScaledExtractOutput.png
20 # sampling rate
100 1900 #min/max
100 1900 #min/max
100 1900 #min/max
)
endif()

#------------ otb::StandardRenderingFunction ------------

add_test(vrTuStandardRenderingFunctionNew ${VISUALIZATION_TESTS1}
otbStandardRenderingFunctionNew
)

#------------ otb::RenderingImageFilter ------------

add_test(vrTuRenderingImageFilterNew ${VISUALIZATION_TESTS1}
otbRenderingImageFilterNew
)

add_test(vrTvRenderingImageFilterScalar ${VISUALIZATION_TESTS1}
--compare-image ${NOTOL}
${BASELINE}/vrTvRenderingImageFilterScalarOutput.png
${TEMP}/vrTvRenderingImageFilterScalarOutput.png
otbRenderingImageFilterScalar
${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
${TEMP}/vrTvRenderingImageFilterScalarOutput.png
127 417
)

add_test(vrTvRenderingImageFilterVector ${VISUALIZATION_TESTS1}
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

add_test(vrTvRenderingImageFilterVectorWithExpNegativeTransfer ${VISUALIZATION_TESTS1}
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

add_test(vrTvRenderingImageFilterAmplitude ${VISUALIZATION_TESTS1}
--compare-image ${NOTOL}
${BASELINE}/vrTvRenderingImageFilterAmplitudeOutput.png
${TEMP}/vrTvRenderingImageFilterAmplitudeOutput.png
otbRenderingImageFilterAmplitude
${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
${TEMP}/vrTvRenderingImageFilterAmplitudeOutput.png
0 1 #channel to use to compute module
50 800
)

add_test(vrTvRenderingImageFilterPhase ${VISUALIZATION_TESTS1}
--compare-image ${NOTOL}
${BASELINE}/vrTvRenderingImageFilterPhaseOutput.png
${TEMP}/vrTvRenderingImageFilterPhaseOutput.png
otbRenderingImageFilterPhase
${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
${TEMP}/vrTvRenderingImageFilterPhaseOutput.png
0 1 #channel to use to compute module
0 1.57 #not very significant with an optical image
)

#------------ otb::LayerBasedModel ------------

add_test(vrTuLayerBasedModelNew ${VISUALIZATION_TESTS1}
otbLayerBasedModelNew
)

#------------ otb::ImageLayerRenderingModel ------------

add_test(vrTuImageLayerRenderingModelNew ${VISUALIZATION_TESTS1}
otbImageLayerRenderingModelNew
)

add_test(vrTvImageLayerRenderingModelSingleLayer ${VISUALIZATION_TESTS1}
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

add_test(vrTuImageLayerGeneratorNew ${VISUALIZATION_TESTS1}
otbImageLayerGeneratorNew
)

add_test(vrTvImageLayerGeneratorScalar ${VISUALIZATION_TESTS1}
otbImageLayerGeneratorScalar
${INPUTDATA}/couleurs.tif
1
)

add_test(vrTvImageLayerGeneratorVector ${VISUALIZATION_TESTS1}
otbImageLayerGeneratorVector
${INPUTDATA}/couleurs.tif
1
)

#---------------------------------------------------------------------------------
#--------------------- otbVisualizationTests2 ------------------------------------
#---------------------------------------------------------------------------------
#------------ otb::UniformAlphaBlendingFunction ------------

add_test(vrTuUniformAlphaBlendingFunctionNew ${VISUALIZATION_TESTS2}
otbUniformAlphaBlendingFunctionNew
)

#------------ otb::MultiplyBlendingFunction ------------

add_test(vrTuMultiplyBlendingFunctionNew ${VISUALIZATION_TESTS2}
otbMultiplyBlendingFunctionNew
)

#------------ otb::BlendingImageFilter ----------------------

add_test(vrTuBlendingImageFilterNew ${VISUALIZATION_TESTS2}
otbBlendingImageFilterNew
)

add_test(vrTvBlendingImageFilter ${VISUALIZATION_TESTS2}
otbBlendingImageFilter
${OTB_DATA_ROOT}/Examples/SpotBefore.png
${OTB_DATA_ROOT}/Examples/SpotAfter.png
${TEMP}/vrTvBlendingImageFilterOutput.png
)


#------------ otb::ImageView ------------
add_test(vrTuImageViewNew ${VISUALIZATION_TESTS2}
otbImageViewNew
)

add_test(vrTvImageViewerEndToEndSingleLayer ${VISUALIZATION_TESTS2}
otbImageViewerEndToEndSingleLayer
${INPUTDATA}/couleurs.tif
200 500 200 0
)

add_test(vrTvImageViewerEndToEndSingleLayerWithSelectAreaHandler ${VISUALIZATION_TESTS2}
otbImageViewerEndToEndSingleLayerWithSelectAreaHandler
${INPUTDATA}/couleurs.tif
200 500 200 0
)

add_test(vrTvImageViewerEndToEndTwoLayers ${VISUALIZATION_TESTS2}
otbImageViewerEndToEndTwoLayers
${INPUTDATA}/couleurs.tif
200 500 200 0
)

add_test(vrTvImageViewerEndToEndGlComponent ${VISUALIZATION_TESTS2}
otbImageViewerEndToEndGlComponent
${INPUTDATA}/couleurs.tif
200 500 200 0
)

#------------ otb::PixelDescriptionModel ------------

add_test(vrTuPixelDescriptionModelNew ${VISUALIZATION_TESTS2}
otbPixelDescriptionModelNew
)

add_test(vrTvPixelDescriptionModelSingleLayer ${VISUALIZATION_TESTS2}
otbPixelDescriptionModelSingleLayer
${INPUTDATA}/couleurs.tif
)

#------------ otb::HistogramCurve ------------

add_test(vrTuHistogramCurveNew ${VISUALIZATION_TESTS2}
otbHistogramCurveNew
)

#------------ otb::Curves2DWidget ------------

add_test(vrTuCurves2DWidgetNew ${VISUALIZATION_TESTS2}
otbCurves2DWidgetNew
)

add_test(vrTvCurves2DWidget ${VISUALIZATION_TESTS2}
otbCurves2DWidget
0 0 100 200 0 0 10 10 0
)

add_test(vrTvCurves2DWidgetWithHistogram ${VISUALIZATION_TESTS2}
otbCurves2DWidgetWithHistogram
${INPUTDATA}/poupees.png
0
)

#------------ otb::VectorDataGlComponent ------------
add_test(vrTuVectorDataGlComponentNew ${VISUALIZATION_TESTS2}
otbVectorDataGlComponentNew
)

add_test(vrTvImageWidgetWithVectorDataGlComponent ${VISUALIZATION_TESTS2}
otbImageWidgetWithVectorDataGlComponent
${INPUTDATA}/poupees.png 0
)

if(OTB_DATA_USE_LARGEINPUT)

add_test(vrTvImageViewerEndToEndWithVectorData ${VISUALIZATION_TESTS2}
otbImageViewerEndToEndWithVectorData
${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
${INPUTDATA}/ToulouseRoad-examples.shp
${INPUTDATA}/DEM/srtm_directory
200 500 200 0
)

endif()

#------------ otb::CircleGlComponent ------------
add_test(vrTuCircleGlComponentNew ${VISUALIZATION_TESTS2}
otbCircleGlComponentNew
)

#------------ otb::CrossGlComponent ------------
add_test(vrTuCrossGlComponentNew ${VISUALIZATION_TESTS2}
otbCrossGlComponentNew
)


#------------ otb::StandardImageViewer ------------
add_test(vrTuStandardImageViewerNew ${VISUALIZATION_TESTS2}
otbStandardImageViewerNew
)

#------------ otb::StandardImageViewer ------------
add_test(vrTuStandardImageViewerRGBNew ${VISUALIZATION_TESTS2}
otbStandardImageViewerRGBNew
)

add_test(vrTvStandardImageViewer ${VISUALIZATION_TESTS2}
otbStandardImageViewer
${INPUTDATA}/couleurs.tif 0
)

add_test(vrTvStandardImageViewerSplitted ${VISUALIZATION_TESTS2}
otbStandardImageViewerSplitted
${INPUTDATA}/couleurs.tif 0
)


if(OTB_DATA_USE_LARGEINPUT)
add_test(vrTvStandardImageViewerToulouseWithWaterwaysShp ${VISUALIZATION_TESTS2}
otbStandardImageViewer
${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
0
${LARGEINPUT}/VECTOR/MidiPyrenees/waterways.shp
${INPUTDATA}/DEM/srtm_directory
)
endif()

#---------- otb::WidgetManager and Sons--------
add_test(vrTuPackedWidgetManagerNew ${VISUALIZATION_TESTS2}
otbPackedWidgetManagerNew
)

add_test(vrTuSplittedWidgetManagerNew ${VISUALIZATION_TESTS2}
otbSplittedWidgetManagerNew
)

#---------- otb::VerticalAsymptote --------
add_test(vrTuVerticalAsymptoteCurveNew ${VISUALIZATION_TESTS2}
otbVerticalAsymptoteCurveNew
)


#---------------------------------------------------------------------------------
#--------------------- otbVisualizationTests3 ------------------------------------
#---------------------------------------------------------------------------------

#---------- otb::VectorDataModel ----------
add_test(vrTuVectorDataModelNew ${VISUALIZATION_TESTS3}
otbVectorDataModelNew
)

#if(OTB_DATA_USE_LARGEINPUT)
add_test(vrTvVectorDataModelTest ${VISUALIZATION_TESTS3}
otbVectorDataModelTest
#${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
200 500 200 0
)
#endif()

add_test(vrTvVectorDataModelAddVectorDataTest ${VISUALIZATION_TESTS3}
otbVectorDataModelAddVectorDataTest
${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
200 500 200 0
${INPUTDATA}/Capitole-Shadows.shp
)

#------------ otb::AmplitudeFuntor ----------------------

add_test(vrTuAmplitudeFunctorTest ${VISUALIZATION_TESTS3}
        otbAmplitudeFunctorTest
)




# Testing srcs
set(Visualization_SRCS1
otbVisualizationTests1.cxx
otbImageWidgetNew.cxx
otbImageWidget.cxx
otbImageWidgetImageDump.cxx
otbImageWidgetActionHandlerNew.cxx
otbImageWidgetControllerNew.cxx
otbImageWidgetController.cxx
otbImageLayerNew.cxx
otbStandardRenderingFunctionNew.cxx
otbRenderingImageFilterNew.cxx
otbRenderingImageFilterScalar.cxx
otbRenderingImageFilterVector.cxx
otbRenderingImageFilterVectorWithExpNegativeTransfer.cxx
otbRenderingImageFilterAmplitude.cxx
otbRenderingImageFilterPhase.cxx
otbImageLayerScalar.cxx
otbImageLayerVector.cxx
otbLayerBasedModelNew.cxx
otbImageLayerRenderingModelNew.cxx
otbImageLayerRenderingModelSingleLayer.cxx
otbImageLayerGeneratorNew.cxx
otbImageLayerGeneratorScalar.cxx
otbImageLayerGeneratorVector.cxx
)

set(Visualization_SRCS2
otbVisualizationTests2.cxx
otbUniformAlphaBlendingFunctionNew.cxx
otbBlendingImageFilterNew.cxx
otbBlendingImageFilter.cxx
otbMultiplyBlendingFunctionNew.cxx
otbImageViewNew.cxx
otbImageViewerEndToEndSingleLayer.cxx
otbImageViewerEndToEndSingleLayerWithSelectAreaHandler.cxx
otbImageViewerEndToEndTwoLayers.cxx
otbImageViewerEndToEndGlComponent.cxx
otbPixelDescriptionModelNew.cxx
otbPixelDescriptionModelSingleLayer.cxx
otbHistogramCurveNew.cxx
otbCurves2DWidgetNew.cxx
otbCurves2DWidget.cxx
otbCurves2DWidgetWithHistogram.cxx
otbVectorDataGlComponentNew.cxx
otbImageWidgetWithVectorDataGlComponent.cxx
otbCircleGlComponentTest.cxx
otbCrossGlComponentTest.cxx
otbImageViewerEndToEndWithVectorData.cxx
otbStandardImageViewerNew.cxx
otbStandardImageViewer.cxx
otbStandardImageViewerRGBNew.cxx
otbPackedWidgetManagerNew.cxx
otbSplittedWidgetManagerNew.cxx
otbVerticalAsymptoteCurveNew.cxx
)

set(Visualization_SRCS3
otbVisualizationTests3.cxx
otbVectorDataModelNew.cxx
otbVectorDataModelTest.cxx
otbVectorDataModelAddVectorDataTest.cxx
otbAmplitudeFunctorTest.cxx
)

OTB_ADD_EXECUTABLE(otbVisualizationTests1 "${Visualization_SRCS1}" "OTBVisualization;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbVisualizationTests2 "${Visualization_SRCS2}" "OTBVisualization;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbVisualizationTests3 "${Visualization_SRCS3}" "OTBVisualization;OTBIO;OTBTesting")


endif()

add_subdirectory( Viewer )
