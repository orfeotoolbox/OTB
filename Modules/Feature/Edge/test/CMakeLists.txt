otb_module_test()

set(OTBEdgeTests
otbEdgeTestDriver.cxx
otbPixelSuppressionByDirectionNew.cxx
otbEdgeDetectorImageFilter.cxx
otbHorizontalSobelVectorImageFilter.cxx
otbAssymmetricFusionOfLineDetector.cxx
otbLocalHoughDraw.cxx
otbAssociativeSymmetricalSum.cxx
otbPersistentVectorizationFilterNew.cxx
otbLineCorrelationDetectorNew.cxx
otbPersistentVectorizationFilter.cxx
otbEdgeDensityImageFilter.cxx
otbLineCorrelationDetector.cxx
otbSobelVectorImageFilter.cxx
otbAssociativeSymmetricalSumNew.cxx
otbHoughTransform2DLinesImageTest.cxx
otbPixelSuppressionByDirection.cxx
otbLocalHoughNew.cxx
otbEdgeDetectorImageFilterNew.cxx
otbEdgeDensityImageFilterNew.cxx
otbLocalHough.cxx
otbExtractSegments.cxx
otbExtractSegmentsNew.cxx
otbAssymmetricFusionOfLineDetectorNew.cxx
otbLineRatioDetector.cxx
otbTouziEdgeDetectorDirection.cxx
otbTouziEdgeDetectorNew.cxx
otbVerticalSobelVectorImageFilter.cxx
otbStreamingLineSegmentDetector.cxx
otbLineRatioDetectorNew.cxx
otbTouziEdgeDetector.cxx
otbLineRatioDetectorLinear.cxx
otbLineSegmentDetector.cxx
otbLineCorrelationDetectorLinear.cxx
otbLineDetectorBaseNew.cxx
otbFillGapsFilter.cxx
otbFillGapsFilterNew.cxx
)

add_executable(otbEdgeTestDriver ${OTBEdgeTests})
target_link_libraries(otbEdgeTestDriver ${OTBEdge-Test_LIBRARIES})
otb_module_target_label(otbEdgeTestDriver)

# Tests Declaration

otb_add_test(NAME feTuPixelSuppressionByDirectionNew COMMAND otbEdgeTestDriver
  otbPixelSuppressionByDirectionNew)

otb_add_test(NAME bfTvEdgeDetectorImageFilter COMMAND otbEdgeTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvEdgeDetectorImageFilter.tif
  ${TEMP}/bfTvEdgeDetectorImageFilter.tif
  otbEdgeDetectorImageFilter
  ${INPUTDATA}/poupees_sub_c1.png
  ${TEMP}/bfTvEdgeDetectorImageFilter.tif
  0   # lower threshold
  255 # upper  threshold
  )

otb_add_test(NAME bfTvHorizontalSobelVectorImageFilter COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvHorizontalSobelVectorImageFilter.tif
  ${TEMP}/bfTvHorizontalSobelVectorImageFilter.tif
  otbHorizontalSobelVectorImageFilterTest
  -in ${INPUTDATA}/cupriteSubHsi.tif
  -out ${TEMP}/bfTvHorizontalSobelVectorImageFilter.tif)

otb_add_test(NAME bfTuHorizontalSobelVectorImageFilterNew COMMAND otbEdgeTestDriver
  otbHorizontalSobelVectorImageFilterNewTest)

otb_add_test(NAME feTvAssymmetricFusionOfLineDetector COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreAssymmetricFusion_amst_2_3.hdr
  ${TEMP}/feFiltreAssymmetricFusion_amst_2_3.hdr
  otbAssymmetricFusionOfLineDetector
  ${INPUTDATA}/amst.png
  ${TEMP}/feFiltreAssymmetricFusion_amst_2_3.hdr
  2 3)


otb_add_test(NAME feTvLocalHoughDrawBis COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreLocalHoughDraw_Line_20_10_2.png
  ${TEMP}/feFiltreLocalHoughDraw_Line_20_10_2.png
  otbLocalHoughDraw
  ${INPUTDATA}/Line.png
  ${TEMP}/feFiltreLocalHoughDraw_Line_20_10_2.png
  20 10 2)

otb_add_test(NAME feTvLocalHoughDraw2 COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_8}  ${BASELINE}/feFilterLocalHoughDraw_amst_20_20_1.png
  ${TEMP}/feFilterLocalHoughDraw_amst_20_20_1.png
  otbLocalHoughDraw
  ${INPUTDATA}/amst.png
  ${TEMP}/feFilterLocalHoughDraw_amst_20_20_1.png
  20 20 1)

otb_add_test(NAME feTvLocalHoughDraw COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreLocalHoughDraw_ImageLine_20_20_2.hdr
  ${TEMP}/feFiltreLocalHoughDraw_ImageLine_20_20_2.hdr
  otbLocalHoughDraw
  ${INPUTDATA}/ImageLine.bsq.hdr
  ${TEMP}/feFiltreLocalHoughDraw_ImageLine_20_20_2.hdr
  20 20 1)

otb_add_test(NAME feTvAssociativeSymmetricalSum COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreASS_amst_2_3.hdr
  ${TEMP}/feFiltreASS_amst_2_3.hdr
  otbAssociativeSymmetricalSum
  ${BASELINE}/feFiltreLineRatioLinear_amst_2_3.hdr
  ${BASELINE}/feFiltreLineCorrelationLinear_amst_2_3.hdr
  ${TEMP}/feFiltreASS_amst_2_3.hdr)



otb_add_test(NAME bfTuPersistentVectorizationImageFilterNew COMMAND otbEdgeTestDriver
  otbPersistentVectorizationFilterNew
  )

otb_add_test(NAME feTuLineCorrelationNew COMMAND otbEdgeTestDriver
  otbLineCorrelationDetectorNew)

otb_add_test(NAME bfTvPersistentVectorizationImageFilter COMMAND otbEdgeTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvPersistentVectorizationImageFilterOutput.txt
  ${TEMP}/bfTvPersistentVectorizationImageFilterOutput.txt
  otbPersistentVectorizationFilter
  ${INPUTDATA}/Seg1InputForRCC8Graph.tif
  ${TEMP}/bfTvPersistentVectorizationImageFilterOutput.txt
  )

otb_add_test(NAME bfTvEdgeDensityImageFilter COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvEdgeDensityImageFilterOutputImage.tif
  ${TEMP}/bfTvEdgeDensityImageFilterOutputImage.tif
  otbEdgeDensityImageFilter
  ${INPUTDATA}/scene.png
  ${TEMP}/bfTvEdgeDensityImageFilterOutputImage.tif
  1 # radius
  15 3  1. 0.01  #Canny Parameters
  )

otb_add_test(NAME feTvLineCorrelation COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreLineCorrelation_amst_2_3.hdr
  ${TEMP}/feFiltreLineCorrelation_amst_2_3.hdr
  otbLineCorrelationDetector
  ${INPUTDATA}/amst.png
  ${TEMP}/feFiltreLineCorrelation_amst_2_3.hdr
  2 3)

otb_add_test(NAME bfTuSobelVectorImageFilterNew COMMAND otbEdgeTestDriver
  otbSobelVectorImageFilterNewTest)

otb_add_test(NAME bfTvSobelVectorImageFilter COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvSobelVectorImageFilter.tif
  ${TEMP}/bfTvSobelVectorImageFilter.tif
  otbSobelVectorImageFilterTest
  -in ${INPUTDATA}/cupriteSubHsi.tif
  -out ${TEMP}/bfTvSobelVectorImageFilter.tif)

otb_add_test(NAME feTuAssociativeSymmetricalSumNew COMMAND otbEdgeTestDriver
  otbAssociativeSymmetricalSumNew)

otb_add_test(NAME feTvHoughTransform2DLinesImage COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_8}
  ${BASELINE}/feFilterHoughTransformDraw.tif
  ${TEMP}/feFilterHoughTransformDraw.tif
  otbHoughTransform2DLinesImageTest
  ${TEMP}/feFilterHoughTransformDraw.tif
  )

otb_add_test(NAME feTvPixelSuppressionByDirection COMMAND otbEdgeTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/feFiltrePixelSuppr_ImageLine_2_0_3.hdr
  ${TEMP}/feFiltrePixelSuppr_ImageLine_2_0_3.hdr
  otbPixelSuppressionByDirection
  ${INPUTDATA}/ImageLine_hd.bsq.hdr
  ${INPUTDATA}/ImageLineDir.bsq.hdr
  ${TEMP}/feFiltrePixelSuppr_ImageLine_2_0_3.hdr
  2 0.3)

otb_add_test(NAME feTuLocalHoughNew COMMAND otbEdgeTestDriver
  otbLocalHoughNew)

otb_add_test(NAME bfTuEdgeDetectorImageFilterNew COMMAND otbEdgeTestDriver
  otbEdgeDetectorImageFilterNew
  )

otb_add_test(NAME bfTuEdgeDensityImageFilterNew COMMAND  otbEdgeTestDriver
  otbEdgeDensityImageFilterNew
  )

otb_add_test(NAME feTvLocalHough COMMAND otbEdgeTestDriver
  --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/feTvLocaLHoughLinesDetected.txt
  ${TEMP}/feTvLocaLHoughLinesDetected.txt
  otbLocalHough
  ${INPUTDATA}/ImageLine.bsq.hdr
  ${TEMP}/feTvLocaLHoughLinesDetected.txt
  20 20 1)

otb_add_test(NAME feTvExtractSegments COMMAND otbEdgeTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/feFiltreExtractSegments_ImageLine.hdr
  ${TEMP}/feFiltreExtractSegments_ImageLine.hdr
  otbExtractSegments
  ${INPUTDATA}/ImageLine_hd.bsq.hdr
  ${INPUTDATA}/ImageLineDir.bsq.hdr
  ${TEMP}/feFiltreExtractSegments_ImageLine.hdr
  10  0.3 10 10 3 10 0.5)

otb_add_test(NAME feTuExtractSegmentsNew COMMAND otbEdgeTestDriver
  otbExtractSegmentsNew)

otb_add_test(NAME feTuAssymmetricFusionOfLineDetectorNew COMMAND otbEdgeTestDriver
  otbAssymmetricFusionOfLineDetectorNew)

otb_add_test(NAME feTvLineRatio COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreLineRatio_amst_2_3.hdr
  ${TEMP}/feFiltreLineRatio_amst_2_3.hdr
  otbLineRatioDetector
  ${INPUTDATA}/amst.png
  ${TEMP}/feFiltreLineRatio_amst_2_3.hdr
  2 3)

otb_add_test(NAME feTvTouziDir COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreTouzi_amst_dir_3.hdr
  ${TEMP}/feFiltreTouzi_amst_dir_3.hdr
  otbTouziEdgeDetectorDirection
  ${INPUTDATA}/amst.png
  ${TEMP}/feFiltreTouzi_amst_dir_3.hdr
  3)

otb_add_test(NAME feTuTouziNew COMMAND otbEdgeTestDriver
  otbTouziEdgeDetectorNew)

otb_add_test(NAME bfTvVerticalSobelVectorImageFilter COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvVerticalSobelVectorImageFilter.tif
  ${TEMP}/bfTvVerticalSobelVectorImageFilter.tif
  otbVerticalSobelVectorImageFilterTest
  -in ${INPUTDATA}/cupriteSubHsi.tif
  -out ${TEMP}/bfTvVerticalSobelVectorImageFilter.tif)

otb_add_test(NAME bfTuVerticalSobelVectorImageFilterNew COMMAND otbEdgeTestDriver
  otbVerticalSobelVectorImageFilterNewTest)

otb_add_test(NAME feTuStreamingLineSegmentDetectorNew COMMAND otbEdgeTestDriver
  otbStreamingLineSegmentDetectorNew)

otb_add_test(NAME feTvStreamingLineSegmentDetector10 COMMAND otbEdgeTestDriver
  --compare-ogr  ${EPSILON_8}
  ${BASELINE_FILES}/feTvStreamingLineSegmentDetectorOutput10.shp
  ${TEMP}/feTvStreamingLineSegmentDetectorOutput10.shp
  otbStreamingLineSegmentDetector
  ${INPUTDATA}/scene.png
  ${TEMP}/feTvStreamingLineSegmentDetectorOutput10.shp
  10
  )

otb_add_test(NAME feTvStreamingLineSegmentDetector1000 COMMAND otbEdgeTestDriver
  --compare-ogr  ${EPSILON_8}
  ${BASELINE_FILES}/feTvStreamingLineSegmentDetectorOutput1000.shp
  ${TEMP}/feTvStreamingLineSegmentDetectorOutput1000.shp
  otbStreamingLineSegmentDetector
  ${INPUTDATA}/scene.png
  ${TEMP}/feTvStreamingLineSegmentDetectorOutput1000.shp
  1000
  )

otb_add_test(NAME feTuLineRatioNew COMMAND otbEdgeTestDriver
  otbLineRatioDetectorNew)

otb_add_test(NAME feTvTouzi COMMAND otbEdgeTestDriver
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreTouzi_amst_3.hdr
  ${TEMP}/feFiltreTouzi_amst_3.hdr
  otbTouziEdgeDetector
  ${INPUTDATA}/amst.png
  ${TEMP}/feFiltreTouzi_amst_3.hdr
  3)

otb_add_test(NAME feTvLineRatioLinearAmsters1 COMMAND otbEdgeTestDriver
  --compare-n-images ${EPSILON_8} 2
  ${BASELINE}/feFiltreLineRatioLinear_amst_ers1_ima_extrait.hdr
  ${TEMP}/feFiltreLineRatioLinear_amst_ers1_ima_extrait.hdr
  ${BASELINE}/feFiltreLineRatioLinear_amst_dir_ers1_ima_extrait.hdr
  ${TEMP}/feFiltreLineRatioLinear_amst_dir_ers1_ima_extrait.hdr
  otbLineRatioDetectorLinear
  ${INPUTDATA}/amst_ers1.ima.extrait.419_187_70_66.hdr
  ${TEMP}/feFiltreLineRatioLinear_amst_ers1_ima_extrait.hdr
  ${TEMP}/feFiltreLineRatioLinear_amst_dir_ers1_ima_extrait.hdr
  1 2)

otb_add_test(NAME feTvLineRatioLinear COMMAND otbEdgeTestDriver
  --compare-n-images ${EPSILON_8} 2
  ${BASELINE}/feFiltreLineRatioLinear_amst_2_3.hdr
  ${TEMP}/feFiltreLineRatioLinear_amst_2_3.hdr
  ${BASELINE}/feFiltreLineRatioLinear_amst_dir_2_3.hdr
  ${TEMP}/feFiltreLineRatioLinear_amst_dir_2_3.hdr
  otbLineRatioDetectorLinear
  ${INPUTDATA}/amst.png
  ${TEMP}/feFiltreLineRatioLinear_amst_2_3.hdr
  ${TEMP}/feFiltreLineRatioLinear_amst_dir_2_3.hdr
  2 3)

otb_add_test(NAME feTuLineSegmentDetectorNew COMMAND otbEdgeTestDriver
  otbLineSegmentDetectorNew)

otb_add_test(NAME feTvLineSegmentDetector COMMAND otbEdgeTestDriver
  --compare-ogr  ${EPSILON_8}
  ${BASELINE_FILES}/feTvLineSegmentDetectorOutput.shp
  ${TEMP}/feTvLineSegmentDetectorOutput.shp
  otbLineSegmentDetector
  ${INPUTDATA}/scene.png
  ${TEMP}/feTvLineSegmentDetectorOutput.shp
  )

otb_add_test(NAME feTvLineCorrelationLinear COMMAND otbEdgeTestDriver
  --compare-n-images ${EPSILON_8} 2
  ${BASELINE}/feFiltreLineCorrelationLinear_amst_2_3.hdr
  ${TEMP}/feFiltreLineCorrelationLinear_amst_2_3.hdr
  ${BASELINE}/feFiltreLineCorrelationLinear_amst_dir_2_3.hdr
  ${TEMP}/feFiltreLineCorrelationLinear_amst_dir_2_3.hdr
  otbLineCorrelationDetectorLinear
  ${INPUTDATA}/amst.png
  ${TEMP}/feFiltreLineCorrelationLinear_amst_2_3.hdr
  ${TEMP}/feFiltreLineCorrelationLinear_amst_dir_2_3.hdr
  2 3)

otb_add_test(NAME feTuLineDetectorBaseNew COMMAND otbEdgeTestDriver
  otbLineDetectorBaseNew)

otb_add_test(NAME feTvFillGapsFilter COMMAND otbSpatialObjectsTestDriver
  --compare-image ${EPSILON_8}
  ${BASELINE}/feTvFillGapfilter.png
  ${TEMP}/feTvFillGapfilter.png
  otbFillGapsFilter
  ${INPUTDATA}/ImageLine.bsq.hdr
  ${TEMP}/feTvFillGapfilter.png)
otb_add_test(NAME feTuFillGapsFilterNew COMMAND otbSpatialObjectsTestDriver
  otbFillGapsFilterNew)
