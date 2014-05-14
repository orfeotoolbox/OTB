
if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


set(FEATUREEXTRACTION_TESTS1 ${CXX_TEST_PATH}/otbFeatureExtractionTests1)
set(FEATUREEXTRACTION_TESTS2 ${CXX_TEST_PATH}/otbFeatureExtractionTests2)
set(FEATUREEXTRACTION_TESTS3 ${CXX_TEST_PATH}/otbFeatureExtractionTests3)
set(FEATUREEXTRACTION_TESTS4 ${CXX_TEST_PATH}/otbFeatureExtractionTests4)
set(FEATUREEXTRACTION_TESTS5 ${CXX_TEST_PATH}/otbFeatureExtractionTests5)
set(FEATUREEXTRACTION_TESTS6 ${CXX_TEST_PATH}/otbFeatureExtractionTests6)
set(FEATUREEXTRACTION_TESTS7 ${CXX_TEST_PATH}/otbFeatureExtractionTests7)
set(FEATUREEXTRACTION_TESTS8 ${CXX_TEST_PATH}/otbFeatureExtractionTests8)
set(FEATUREEXTRACTION_TESTS9 ${CXX_TEST_PATH}/otbFeatureExtractionTests9)
set(FEATUREEXTRACTION_TESTS10 ${CXX_TEST_PATH}/otbFeatureExtractionTests10)
set(FEATUREEXTRACTION_TESTS11 ${CXX_TEST_PATH}/otbFeatureExtractionTests11)
set(FEATUREEXTRACTION_TESTS12 ${CXX_TEST_PATH}/otbFeatureExtractionTests12)
set(FEATUREEXTRACTION_TESTS13 ${CXX_TEST_PATH}/otbFeatureExtractionTests13)
set(FEATUREEXTRACTION_TESTS14 ${CXX_TEST_PATH}/otbFeatureExtractionTests14)
set(FEATUREEXTRACTION_TESTS15 ${CXX_TEST_PATH}/otbFeatureExtractionTests15)
set(FEATUREEXTRACTION_TESTS16 ${CXX_TEST_PATH}/otbFeatureExtractionTests16)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests1 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

add_test(feTvAlignMV2ITK ${FEATUREEXTRACTION_TESTS1}
   --compare-ascii ${EPSILON_3}     ${BASELINE_FILES}/feAlign.txt
                        ${TEMP}/feAlign.txt
        otbAlignImageToPath
 ${INPUTDATA}/poupees.hdr
 ${TEMP}/feAlign.txt)

# -------            otb::Draw   ------------------------------

add_test(feTvDrawPathTestCarre ${FEATUREEXTRACTION_TESTS1}
  --compare-image ${NOTOL}  ${BASELINE}/feDrawPathDessinCarre.png
                          ${TEMP}/feDrawPathDessinCarre.png
        otbDrawPathDessinCarre
 ${INPUTDATA}/DeuxTraits.png
 ${TEMP}/feDrawPathDessinCarre.png)

add_test(feTvDrawPathTestAlign ${FEATUREEXTRACTION_TESTS1}
  --compare-image ${NOTOL}  ${BASELINE}/feDrawPathAlignDeuxTraits.png
                          ${TEMP}/feDrawPathAlignDeuxTraits.png
        otbDrawPathAlign
 ${INPUTDATA}/DeuxTraits.png
 ${TEMP}/feDrawPathAlignDeuxTraits.png)

add_test(feTuComplexMomentsImageFunctionNew ${FEATUREEXTRACTION_TESTS1}
  otbComplexMomentsImageFunctionNew
  )

add_test(feTvComplexMomentsImageFunction ${FEATUREEXTRACTION_TESTS1}
  --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feTvComplexMomentsImageFunction.txt
                               ${TEMP}/feTvComplexMomentsImageFunction.txt
  otbComplexMomentsImageFunction
  ${INPUTDATA}/poupees.png
  4
  4
  ${TEMP}/feTvComplexMomentsImageFunction.txt
  )

add_test(feTvComplexMomentsImageFunctionScaleInvariant ${FEATUREEXTRACTION_TESTS1}
  otbComplexMomentsImageFunctionScaleInvariant
  ${INPUTDATA}/poupees.png
  4
  4
  )

add_test(feTuRealMomentsImageFunctionNew ${FEATUREEXTRACTION_TESTS1}
  otbRealMomentsImageFunctionNew
  )

add_test(feTvRealMomentsImageFunction ${FEATUREEXTRACTION_TESTS1}
   --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feTvRealMomentsImageFunction.txt
                        ${TEMP}/feTvRealMomentsImageFunction.txt
        otbRealMomentsImageFunction
 ${INPUTDATA}/poupees.png
        4
        4
        ${TEMP}/feTvRealMomentsImageFunction.txt)

add_test(feTuRadiometricMomentsImageFunctionNew ${FEATUREEXTRACTION_TESTS1}
  otbRadiometricMomentsImageFunctionNew
  )

add_test(feTvRadiometricMomentsImageFunction ${FEATUREEXTRACTION_TESTS1}
   --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feTvRadiometricMomentsImageFunction.txt
                                ${TEMP}/feTvRadiometricMomentsImageFunction.txt
        otbRadiometricMomentsImageFunction
 ${INPUTDATA}/poupees.png
        ${TEMP}/feTvRadiometricMomentsImageFunction.txt)

add_test(feTuHuMomentsImageFunctionNew ${FEATUREEXTRACTION_TESTS1}
        otbHuMomentsImageFunctionNew
        )

add_test(feTvHuMomentsImageFunction ${FEATUREEXTRACTION_TESTS1}
   --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feTvHuMomentsImageFunction.txt
                        ${TEMP}/feTvHuMomentsImageFunction.txt
        otbHuMomentsImageFunction
 ${INPUTDATA}/poupees.png
        ${TEMP}/feTvHuMomentsImageFunction.txt
        )

add_test(feTvHuMomentsImageFunctionScaleInvariant ${FEATUREEXTRACTION_TESTS1}
        otbHuMomentsImageFunctionScaleInvariant
 ${INPUTDATA}/poupees.png
        )

add_test(feTvHuMomentsImageFunctionRotationInvariant ${FEATUREEXTRACTION_TESTS1}
        otbHuMomentsImageFunctionRotationInvariant
 ${INPUTDATA}/poupees.png
        90
        )

add_test(feTuFlusserMomentsImageFunctionNew ${FEATUREEXTRACTION_TESTS1}
        otbFlusserMomentsImageFunctionNew
)

add_test(feTvFlusserMomentsImageFunction ${FEATUREEXTRACTION_TESTS1}
   --compare-ascii ${EPSILON_8}     ${BASELINE_FILES}/feTvFlusserMomentsImageFunction.txt
                        ${TEMP}/feTvFlusserMomentsImageFunction.txt
        otbFlusserMomentsImageFunction
 ${INPUTDATA}/poupees.png
        ${TEMP}/feTvFlusserMomentsImageFunction.txt)

add_test(feTvFlusserMomentsImageFunctionScaleInvariant ${FEATUREEXTRACTION_TESTS1}
        otbFlusserMomentsImageFunctionScaleInvariant
 ${INPUTDATA}/poupees.png
        )

add_test(feTvFlusserMomentsImageFunctionRotationInvariant ${FEATUREEXTRACTION_TESTS1}
        otbFlusserMomentsImageFunctionRotationInvariant
 ${INPUTDATA}/poupees.png
        90
        )

add_test(feTuComplexMomentPathNew ${FEATUREEXTRACTION_TESTS1}
        otbComplexMomentPathNew)

add_test(feTvComplexMomentPath ${FEATUREEXTRACTION_TESTS1}
   --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feComplexMomentPath.txt
                        ${TEMP}/feComplexMomentPath.txt
        otbComplexMomentPath
  1
  1
  ${TEMP}/feComplexMomentPath.txt)

add_test(feTvComplexMomentPathFloat ${FEATUREEXTRACTION_TESTS1}
   --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feComplexMomentPathFloat.txt
                        ${TEMP}/feComplexMomentPathFloat.txt
        otbComplexMomentPathFloat
  1
  1
  ${TEMP}/feComplexMomentPathFloat.txt)

add_test(feTuHuPathNew ${FEATUREEXTRACTION_TESTS1}
        otbHuPathNew)

add_test(feTvHuPath ${FEATUREEXTRACTION_TESTS1}
   --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feHuPath.txt
                        ${TEMP}/feHuPath.txt
        otbHuPath
  ${TEMP}/feHuPath.txt)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests2 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

add_test(feTuFlusserPathNew ${FEATUREEXTRACTION_TESTS2}
        otbFlusserPathNew)

add_test(feTvFlusserPath ${FEATUREEXTRACTION_TESTS2}
   --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feFlusserPath.txt
                        ${TEMP}/feFlusserPath.txt
        otbFlusserPath
  ${TEMP}/feFlusserPath.txt)


# -------            otb::OrientationPathFunction   ------------------------

add_test(feTuOrientationPathNew ${FEATUREEXTRACTION_TESTS2}
        otbOrientationPathNew)

add_test(feTuOrientationPath_000 ${FEATUREEXTRACTION_TESTS2}
        otbOrientationPath 0.0)

add_test(feTuOrientationPath_045 ${FEATUREEXTRACTION_TESTS2}
        otbOrientationPath 45.0)

add_test(feTuOrientationPath_090 ${FEATUREEXTRACTION_TESTS2}
        otbOrientationPath 90.0)

add_test(feTuOrientationPath1_80 ${FEATUREEXTRACTION_TESTS2}
        otbOrientationPath 180.0)

# -------            otb::CompacityPathFunction   ------------------------

add_test(feTuCompacityPathNew ${FEATUREEXTRACTION_TESTS2}
        otbCompacityPathNew)

add_test(feTuCompacityPathCircle ${FEATUREEXTRACTION_TESTS2}
        otbCompacityPathCircle 1000)

add_test(feTuCompacityPathSquare ${FEATUREEXTRACTION_TESTS2}
        otbCompacityPathSquare 10.0)

add_test(feTuCompacityPathRectangle ${FEATUREEXTRACTION_TESTS2}
        otbCompacityPathRectangle 10.0  20.0)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests3 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# -------            otb::TouziEdgeDetector   ------------------------------

add_test(feTuTouziNew ${FEATUREEXTRACTION_TESTS3}
        otbTouziEdgeDetectorNew)

add_test(feTvTouzi ${FEATUREEXTRACTION_TESTS3}
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreTouzi_amst_3.hdr
                          ${TEMP}/feFiltreTouzi_amst_3.hdr
        otbTouziEdgeDetector
 ${INPUTDATA}/amst.png
 ${TEMP}/feFiltreTouzi_amst_3.hdr
 3)

add_test(feTvTouziDir ${FEATUREEXTRACTION_TESTS3}
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreTouzi_amst_dir_3.hdr
                          ${TEMP}/feFiltreTouzi_amst_dir_3.hdr
        otbTouziEdgeDetectorDirection
 ${INPUTDATA}/amst.png
 ${TEMP}/feFiltreTouzi_amst_dir_3.hdr
 3)

# -------            otb::LineDetectorBase   ------------------------------

add_test(feTuLineDetectorBaseNew ${FEATUREEXTRACTION_TESTS3}
        otbLineDetectorBaseNew)

# -------            otb::LineCorrelationDetector   ------------------------------

add_test(feTuLineCorrelationNew ${FEATUREEXTRACTION_TESTS3}
        otbLineCorrelationDetectorNew)

add_test(feTvLineCorrelationLinear ${FEATUREEXTRACTION_TESTS3}
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

add_test(feTvLineCorrelation ${FEATUREEXTRACTION_TESTS3}
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreLineCorrelation_amst_2_3.hdr
                          ${TEMP}/feFiltreLineCorrelation_amst_2_3.hdr
        otbLineCorrelationDetector
 ${INPUTDATA}/amst.png
 ${TEMP}/feFiltreLineCorrelation_amst_2_3.hdr
 2 3)

# -------            otb::LineRatioDetector   ------------------------------

add_test(feTuLineRatioNew ${FEATUREEXTRACTION_TESTS3}
        otbLineRatioDetectorNew)

add_test(feTvLineRatioLinear ${FEATUREEXTRACTION_TESTS3}
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

add_test(feTvLineRatioLinearAmsters1 ${FEATUREEXTRACTION_TESTS3}
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

add_test(feTvLineRatio ${FEATUREEXTRACTION_TESTS3}
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreLineRatio_amst_2_3.hdr
                          ${TEMP}/feFiltreLineRatio_amst_2_3.hdr
        otbLineRatioDetector
 ${INPUTDATA}/amst.png
 ${TEMP}/feFiltreLineRatio_amst_2_3.hdr
 2 3)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests4 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::AssociativeSymmetricalSum   ------------------------------

add_test(feTuAssociativeSymmetricalSumNew ${FEATUREEXTRACTION_TESTS4}
        otbAssociativeSymmetricalSumNew)

add_test(feTvAssociativeSymmetricalSum ${FEATUREEXTRACTION_TESTS4}
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreASS_amst_2_3.hdr
                          ${TEMP}/feFiltreASS_amst_2_3.hdr
        otbAssociativeSymmetricalSum
        ${BASELINE}/feFiltreLineRatioLinear_amst_2_3.hdr
        ${BASELINE}/feFiltreLineCorrelationLinear_amst_2_3.hdr
        ${TEMP}/feFiltreASS_amst_2_3.hdr)

# -------            otb::AssymmetricFusionOfLineDetector   ------------------------------

add_test(feTuAssymmetricFusionOfLineDetectorNew ${FEATUREEXTRACTION_TESTS4}
        otbAssymmetricFusionOfLineDetectorNew)

add_test(feTvAssymmetricFusionOfLineDetector ${FEATUREEXTRACTION_TESTS4}
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreAssymmetricFusion_amst_2_3.hdr
                          ${TEMP}/feFiltreAssymmetricFusion_amst_2_3.hdr
        otbAssymmetricFusionOfLineDetector
 ${INPUTDATA}/amst.png
 ${TEMP}/feFiltreAssymmetricFusion_amst_2_3.hdr
 2 3)

# -------            otb::HarrisImage   ------------------------------

add_test(feTvHarrisImage ${FEATUREEXTRACTION_TESTS4}
  --compare-image ${NOTOL}  ${BASELINE}/feHarrisImage.png
                          ${TEMP}/feHarrisImage.png
        otbHarrisImage
 ${INPUTDATA}/TeteAToto.png
 ${TEMP}/feHarrisImage.png
 1.0 2.0 0.0)

add_test(feTvHarrisPoupee ${FEATUREEXTRACTION_TESTS4}
  --compare-image ${NOTOL}  ${BASELINE}/feHarris_poupees.png
                          ${TEMP}/feHarris_poupees.png
        otbHarrisImage
 ${INPUTDATA}/poupees_1canal.c1.hdr
 ${TEMP}/feHarris_poupees.png
 1.0 1.0 1.0)

add_test(feTvHarrisImageQB ${FEATUREEXTRACTION_TESTS4}
  --compare-image ${NOTOL}  ${BASELINE}/feHarrisImage_QB.png
                          ${TEMP}/feHarrisImage_QB.png
        otbHarrisImage
 ${INPUTDATA}/QB_Suburb.png
 ${TEMP}/feHarrisImage_QB.png
 1.0 1.0 1.0)

add_test(feTvMultiplyByScalarImage ${FEATUREEXTRACTION_TESTS4}
         otbMultiplyByScalarImageFilterTest)

add_test(feTuThresholdImageToPointSetFilterNew ${FEATUREEXTRACTION_TESTS4}
         otbThresholdImageToPointSetFilterNew
)


add_test(feTuThresholdImageToPointSet1 ${FEATUREEXTRACTION_TESTS4}
  --compare-ascii ${EPSILON_3} ${BASELINE_FILES}/feThresholdImageToPointSet_Threshold_0To255.txt
                   ${TEMP}/feThresholdImageToPointSet_Threshold_0To255.txt
        otbThresholdImageToPointSetTest
 ${INPUTDATA}/small_points.hdr
 ${TEMP}/feThresholdImageToPointSet_Threshold_0To255.txt
 0 255 )
add_test(feTuThresholdImageToPointSet2 ${FEATUREEXTRACTION_TESTS4}
  --compare-ascii ${EPSILON_3} ${BASELINE_FILES}/feThresholdImageToPointSet_Threshold_0To0.txt
                   ${TEMP}/feThresholdImageToPointSet_Threshold_0To0.txt
        otbThresholdImageToPointSetTest
 ${INPUTDATA}/small_points.hdr
 ${TEMP}/feThresholdImageToPointSet_Threshold_0To0.txt
 0 0 )
add_test(feTuThresholdImageToPointSet3 ${FEATUREEXTRACTION_TESTS4}
  --compare-ascii ${EPSILON_3} ${BASELINE_FILES}/feThresholdImageToPointSet_Threshold_255To255.txt
                   ${TEMP}/feThresholdImageToPointSet_Threshold_255To255.txt
        otbThresholdImageToPointSetTest
 ${INPUTDATA}/small_points.hdr
 ${TEMP}/feThresholdImageToPointSet_Threshold_255To255.txt
 255 255 )
add_test(feTuThresholdImageToPointSet4 ${FEATUREEXTRACTION_TESTS4}
  --compare-ascii ${EPSILON_3} ${BASELINE_FILES}/feThresholdImageToPointSet_Threshold_1To200.txt
                   ${TEMP}/feThresholdImageToPointSet_Threshold_1To200.txt
        otbThresholdImageToPointSetTest
 ${INPUTDATA}/small_points.hdr
 ${TEMP}/feThresholdImageToPointSet_Threshold_1To200.txt
 1 200 )

add_test(feTvHarrisProduceImageToValidHarrisToPointSet ${FEATUREEXTRACTION_TESTS4}
        otbHarrisImage
 ${INPUTDATA}/small_points.hdr
 ${TEMP}/small_points_harris.hdr
 1.0 1.0 1.0)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests5 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

add_test(feTvHarrisToPointSet1 ${FEATUREEXTRACTION_TESTS5}
  --compare-ascii ${EPSILON_3}        ${BASELINE_FILES}/feHarrisToPointSet_Threshold_2To255.txt
                                ${TEMP}/feHarrisToPointSet_Threshold_2To255.txt
        otbHarrisToPointSet
 ${INPUTDATA}/small_points.hdr
 ${TEMP}/feHarrisToPointSet_Threshold_2To255.txt
 1.0 1.0 1.0 2.0 255.0)
add_test(feTvHarrisToPointSet2 ${FEATUREEXTRACTION_TESTS5}
  --compare-ascii ${EPSILON_3}        ${BASELINE_FILES}/feHarrisToPointSet_Threshold_20To90.txt
                                ${TEMP}/feHarrisToPointSet_Threshold_20To90.txt
        otbHarrisToPointSet
 ${INPUTDATA}/small_points.hdr
 ${TEMP}/feHarrisToPointSet_Threshold_20To90.txt
 1.0 1.0 1.0 20 90)
add_test(feTvHarrisToPointSet3 ${FEATUREEXTRACTION_TESTS5}
  --compare-ascii ${EPSILON_3}        ${BASELINE_FILES}/feHarrisToPointSet_Threshold_100To160.txt
                                ${TEMP}/feHarrisToPointSet_Threshold_100To160.txt
        otbHarrisToPointSet
 ${INPUTDATA}/small_points.hdr
 ${TEMP}/feHarrisToPointSet_Threshold_100To160.txt
 1.0 1.0 1.0 100 160)

# -------            otb::PixelSuppressionByDirection   ------------------------------

add_test(feTuPixelSuppressionByDirectionNew ${FEATUREEXTRACTION_TESTS5}
        otbPixelSuppressionByDirectionNew)

add_test(feTvPixelSuppressionByDirection ${FEATUREEXTRACTION_TESTS5}
  --compare-image ${NOTOL}  ${BASELINE}/feFiltrePixelSuppr_ImageLine_2_0_3.hdr
                          ${TEMP}/feFiltrePixelSuppr_ImageLine_2_0_3.hdr
        otbPixelSuppressionByDirection
 ${INPUTDATA}/ImageLine_hd.bsq.hdr
 ${INPUTDATA}/ImageLineDir.bsq.hdr
 ${TEMP}/feFiltrePixelSuppr_ImageLine_2_0_3.hdr
 2 0.3)

# -------            otb::LocalHough   ------------------------------

add_test(feTuLocalHoughNew ${FEATUREEXTRACTION_TESTS5}
        otbLocalHoughNew)


add_test(feTvLocalHough ${FEATUREEXTRACTION_TESTS5}
  --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/feTvLocaLHoughLinesDetected.txt
                                ${TEMP}/feTvLocaLHoughLinesDetected.txt
        otbLocalHough
 ${INPUTDATA}/ImageLine.bsq.hdr
 ${TEMP}/feTvLocaLHoughLinesDetected.txt
 20 20 1)

add_test(feTvLocalHoughDraw ${FEATUREEXTRACTION_TESTS5}
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreLocalHoughDraw_ImageLine_20_20_2.hdr
                          ${TEMP}/feFiltreLocalHoughDraw_ImageLine_20_20_2.hdr
        otbLocalHoughDraw
 ${INPUTDATA}/ImageLine.bsq.hdr
 ${TEMP}/feFiltreLocalHoughDraw_ImageLine_20_20_2.hdr
 20 20 1)

add_test(feTvLocalHoughDraw2 ${FEATUREEXTRACTION_TESTS5}
  --compare-image ${EPSILON_8}  ${BASELINE}/feFilterLocalHoughDraw_amst_20_20_1.png
                          ${TEMP}/feFilterLocalHoughDraw_amst_20_20_1.png
        otbLocalHoughDraw
 ${INPUTDATA}/amst.png
 ${TEMP}/feFilterLocalHoughDraw_amst_20_20_1.png
 20 20 1)


add_test(feTvLocalHoughDrawBis ${FEATUREEXTRACTION_TESTS5}
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreLocalHoughDraw_Line_20_10_2.png
                   ${TEMP}/feFiltreLocalHoughDraw_Line_20_10_2.png
        otbLocalHoughDraw
 ${INPUTDATA}/Line.png
 ${TEMP}/feFiltreLocalHoughDraw_Line_20_10_2.png
 20 10 2)

# -------            otb::FillGapsFilter   ------------------------------

add_test(feTuFillGapsFilterNew ${FEATUREEXTRACTION_TESTS5}
        otbFillGapsFilterNew)

add_test(feTvFillGapsFilter ${FEATUREEXTRACTION_TESTS5}
   --compare-image ${EPSILON_8}
        ${BASELINE}/feTvFillGapfilter.png
               ${TEMP}/feTvFillGapfilter.png
        otbFillGapsFilter
        ${INPUTDATA}/ImageLine.bsq.hdr
        ${TEMP}/feTvFillGapfilter.png)

# -------            otb::HoughTransform2DLinesImageTest ------------------------------

add_test(feTvHoughTransform2DLinesImage ${FEATUREEXTRACTION_TESTS5}
  --compare-image ${EPSILON_8}
    ${BASELINE}/feFilterHoughTransformDraw.tif
           ${TEMP}/feFilterHoughTransformDraw.tif
        otbHoughTransform2DLinesImageTest
 ${TEMP}/feFilterHoughTransformDraw.tif
 )


# -------            otb::ImageToModulusAndDirectionImageFilter   ------------------------------
# -------            otb::ModulusAndDirectionImageToImageFilter   ------------------------------

add_test(feTuModulusAndDirectionImageFiltersNew ${FEATUREEXTRACTION_TESTS5}
        otbModulusAndDirectionImageFiltersNew)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests6 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::ExtractSegments   ------------------------------

add_test(feTuExtractSegmentsNew ${FEATUREEXTRACTION_TESTS6}
        otbExtractSegmentsNew)

add_test(feTvExtractSegments ${FEATUREEXTRACTION_TESTS6}
  --compare-image ${NOTOL}  ${BASELINE}/feFiltreExtractSegments_ImageLine.hdr
                          ${TEMP}/feFiltreExtractSegments_ImageLine.hdr
        otbExtractSegments
 ${INPUTDATA}/ImageLine_hd.bsq.hdr
 ${INPUTDATA}/ImageLineDir.bsq.hdr
 ${TEMP}/feFiltreExtractSegments_ImageLine.hdr
 10  0.3 10 10 3 10 0.5)

# -------            otb::ForwardFourierMellinTransformImageFilter   -------------

add_test(feTuForwardFourierMellinImageFilterNew ${FEATUREEXTRACTION_TESTS6}
         otbFourierMellinImageFilterNew)

#add_test(feTuForwardFourierMellinImageFilterCroix ${FEATUREEXTRACTION_TESTS6}
# --compare-n-images ${NOTOL} 2
#      ${BASELINE}/feFourierMellinCroixOutputModulus.hdr
#      ${TEMP}/feFourierMellinCroixOutputModulus.hdr
#      ${BASELINE}/feFourierMellinCroixOutputPhase.hdr
#      ${TEMP}/feFourierMellinCroixOutputPhase.hdr
#        otbFourierMellinImageFilter
#        ${INPUTDATA}/TestFFT.hdr
# ${TEMP}/feFourierMellinCroixOutputModulus.hdr
# ${TEMP}/feFourierMellinCroixOutputPhase.hdr
#)

#add_test(feTuForwardFourierMellinImageFilter3Croix ${FEATUREEXTRACTION_TESTS6}
#   --compare-n-images   ${NOTOL} 2
#   ${BASELINE}/feFourierMellin3CroixOutputModulus.hdr
#                        ${TEMP}/feFourierMellin3CroixOutputModulus.hdr
#   ${BASELINE}/feFourierMellin3CroixOutputPhase.hdr
#                        ${TEMP}/feFourierMellin3CroixOutputPhase.hdr
#    otbFourierMellinImageFilter
#        ${INPUTDATA}/TestFFT3Croix.hdr
# ${TEMP}/feFourierMellin3CroixOutputModulus.hdr
# ${TEMP}/feFourierMellin3CroixOutputPhase.hdr
#)

add_test(feTvForwardFourierMellinImageFilter ${FEATUREEXTRACTION_TESTS6}
  --compare-n-images ${EPSILON_6} 2
       ${BASELINE}/feForwardFourierMellinImageFilterReal.hdr
                     ${TEMP}/feForwardFourierMellinImageFilterReal.hdr
       ${BASELINE}/feForwardFourierMellinImageFilterImag.hdr
                     ${TEMP}/feForwardFourierMellinImageFilterImag.hdr
        otbFourierMellinImageFilter
        ${INPUTDATA}/circle.hdr
 ${TEMP}/feForwardFourierMellinImageFilterReal.hdr
 ${TEMP}/feForwardFourierMellinImageFilterImag.hdr)

# -----------------------------------------------------------------------
# -------            otb::ImageToEdgePathFilter   -------------

add_test(feTuImageToEdgePathFilterNew ${FEATUREEXTRACTION_TESTS6}
         otbImageToEdgePathFilterNew)

add_test(feTvImageToEdgePathFilter ${FEATUREEXTRACTION_TESTS6}
  --compare-image ${EPSILON_8}
       ${BASELINE}/feImageToEdgePathFilter_rcc8_mire4.png
             ${TEMP}/feImageToEdgePathFilter_rcc8_mire4.png
        otbImageToEdgePathFilter
        ${INPUTDATA}/rcc8_mire4.png
        ${TEMP}/feImageToEdgePathFilter_rcc8_mire4.png
        255
        )
add_test(feTvImageToEdgePathFilterBis ${FEATUREEXTRACTION_TESTS6}
  --compare-image ${EPSILON_8}
       ${BASELINE}/feImageToEdgePathFilter_maskrom.png
             ${TEMP}/feImageToEdgePathFilter_maskrom.png
        otbImageToEdgePathFilter
        ${INPUTDATA}/maskrom.png
        ${TEMP}/feImageToEdgePathFilter_maskrom.png
        0
        )
# -------            otb::NeighborhoodScalarProductFilter   -------------


add_test(feTuNeighborhoodScalarProductFilterNew ${FEATUREEXTRACTION_TESTS6}
         otbNeighborhoodScalarProductFilterNew)

add_test(feTvNeighborhoodScalarProductFilter ${FEATUREEXTRACTION_TESTS6}
  --compare-n-images ${EPSILON_6} 2
        ${BASELINE}/feTvNeigborhoodScalarProductModulusOutput.hdr
        ${TEMP}/feTvNeigborhoodScalarProductModulusOutput.hdr
        ${BASELINE}/feTvNeigborhoodScalarProductDirectionOutput.hdr
        ${TEMP}/feTvNeigborhoodScalarProductDirectionOutput.hdr
        otbNeighborhoodScalarProductFilter
        ${INPUTDATA}/InputForRoadDetection_SpectralAngle.hdr
        ${TEMP}/feTvNeigborhoodScalarProductModulusOutput.hdr
 ${TEMP}/feTvNeigborhoodScalarProductDirectionOutput.hdr
        1.0
        )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests7 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::RemoveIsolatedByDirectionFilter   -------------

add_test(feTuRemoveIsolatedByDirectionFilterNew ${FEATUREEXTRACTION_TESTS7}
         otbRemoveIsolatedByDirectionFilterNew)

add_test(feTvRemoveIsolatedByDirectionFilter ${FEATUREEXTRACTION_TESTS7}
  --compare-image ${EPSILON_8}
        ${BASELINE}/feTvRemoveIsolatedByDirectionOutput.hdr
        ${TEMP}/feTvRemoveIsolatedByDirectionOutput.hdr
        otbRemoveIsolatedByDirectionFilter
       ${INPUTDATA}/InputForRoadDetectionScalarProductMod.hdr
       ${INPUTDATA}/InputForRoadDetectionScalarProductDir.hdr
       ${TEMP}/feTvRemoveIsolatedByDirectionOutput.hdr
        )

# -------            otb::RemoveWrongDirectionFilter   -------------

add_test(feTuRemoveWrongDirectionFilterNew ${FEATUREEXTRACTION_TESTS7}
         otbRemoveWrongDirectionFilterNew)

add_test(feTvRemoveWrongDirectionFilter ${FEATUREEXTRACTION_TESTS7}
  --compare-image ${EPSILON_8}
        ${BASELINE}/feTvRemoveWrongDirectionOutput.hdr
        ${TEMP}/feTvRemoveWrongDirectionOutput.hdr
        otbRemoveWrongDirectionFilter
       ${INPUTDATA}/InputForRoadDetection_IsolatedRem.hdr
       ${INPUTDATA}/InputForRoadDetectionScalarProductDir.hdr
       ${TEMP}/feTvRemoveWrongDirectionOutput.hdr
        )

# -------            otb::NonMaxRemovalByDirectionFilter   -------------

add_test(feTuNonMaxRemovalByDirectionFilterNew ${FEATUREEXTRACTION_TESTS7}
         otbNonMaxRemovalByDirectionFilterNew)

add_test(feTvNonMaxRemovalByDirectionFilter ${FEATUREEXTRACTION_TESTS7}
  --compare-image ${EPSILON_8}
        ${BASELINE}/feTvNonMaxRemovalByDirectionOutput.hdr
        ${TEMP}/feTvNonMaxRemovalByDirectionOutput.hdr
        otbNonMaxRemovalByDirectionFilter
       ${INPUTDATA}/InputForRoadDetection_WrongRem.hdr
       ${INPUTDATA}/InputForRoadDetectionScalarProductDir.hdr
       ${TEMP}/feTvNonMaxRemovalByDirectionOutput.hdr
        )

# -------            otb::VectorizationPathListFilter   -------------

add_test(feTuVectorizationPathListFilterNew ${FEATUREEXTRACTION_TESTS7}
         otbVectorizationPathListFilterNew)

add_test(feTvVectorizationPathListFilterAsciiOutput ${FEATUREEXTRACTION_TESTS7}
--compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvVectorizationPathListOutput.txt
  ${TEMP}/feTvVectorizationPathListOutput.txt
        otbVectorizationPathListFilter
       ${INPUTDATA}/InputForRoadDetection_NonMaxRem.hdr
       ${INPUTDATA}/InputForRoadDetectionScalarProductDir.hdr
       ${TEMP}/feTvVectorizationPathListOutput.txt
       ${TEMP}/feTvVectorizationPathListOutput.png
       0.0005
        )

add_test(feTvVectorizationPathListFilterImageOutput ${FEATUREEXTRACTION_TESTS7}
--compare-image ${EPSILON_8}
  ${BASELINE}/feTvVectorizationPathListOutput.png
  ${TEMP}/feTvVectorizationPathListOutput.png
        otbVectorizationPathListFilter
       ${INPUTDATA}/InputForRoadDetection_NonMaxRem.hdr
       ${INPUTDATA}/InputForRoadDetectionScalarProductDir.hdr
       ${TEMP}/feTvVectorizationPathListOutput.txt
       ${TEMP}/feTvVectorizationPathListOutput.png
       0.0001
        )

# -------            otb::SimplifyPathFilter   -------------

add_test(feTuSimplifyPathListFilterNew ${FEATUREEXTRACTION_TESTS7}
         otbSimplifyPathListFilterNew)

add_test(feTvSimplifyPathListFilter ${FEATUREEXTRACTION_TESTS7}
--compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvSimplifyPathOutput.txt
  ${TEMP}/feTvSimplifyPathOutput.txt
        otbSimplifyPathListFilter
 ${TEMP}/feTvSimplifyPathOutput.txt
         1.0
         1 1   5  1  7  1  11  1  21  1  31  1 |
         1 1   5  1  7  1  11  1  21 11  31 11 |
         1 1   5  5  7  7  11 11  21 21  31 31 41 41 |
         1 1   5  1  7  1  11  1  21 11  31 11 41 11 58 11 70 11
        )

# -------            otb::BreakAngularPathListFilter   -------------

add_test(feTuBreakAngularPathListFilterNew ${FEATUREEXTRACTION_TESTS7}
         otbBreakAngularPathListFilterNew)

add_test(feTvBreakAngularPathListFilter ${FEATUREEXTRACTION_TESTS7}
--compare-ascii ${NOTOL}  ${BASELINE_FILES}/feBreakAngularPathListOutput.txt
             ${TEMP}/feBreakAngularPathListOutput.txt
         otbBreakAngularPathListFilter
         ${TEMP}/feBreakAngularPathListOutput.txt
         10 20 30 50 90 140 180 |
         1 1  5 1  7 1  11 1  21 1  31 1 |
         1 1  5 1  7 1  11 1  21 11  31 11 |
         1 1  5 1  7 1  11 1  21 11  31 11 41 11  |
         1 1  5 1  7 1  11 1  21 11  31 11 41 11 58 11 70 11 |
         1 11  5 11  7 11  11 11  21 1  31 1 41 1 |
         1 1  5 1  7 1  11 1  21 11  31 21 41 31  |
         1 1  5 1  5 11  5 21 |
         1 1  31 1 41 1
         )



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests8 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::RemoveTortuousPathListFilter   -------------

add_test(feTuRemoveTortuousPathListFilterNew ${FEATUREEXTRACTION_TESTS8}
         otbRemoveTortuousPathListFilterNew)

add_test(feTvRemoveTortuousPathListFilter ${FEATUREEXTRACTION_TESTS8}
--compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvRemoveTortuousPathOutput.txt
  ${TEMP}/feTvRemoveTortuousPathOutput.txt
        otbRemoveTortuousPathListFilter
 ${TEMP}/feTvRemoveTortuousPathOutput.txt
         10.0
         1 1   1 11  |
         1 1   5  1  7  1  11  1  |
         1 1   7  7 21 21  31 31 41 41 |
         1 1   5  1  7  1  11  1  21 11  31 11 41 11 58 11 70 11
        )

# -------            otb::LinkPathListFilter   -------------

add_test(feTuLinkPathListFilterNew ${FEATUREEXTRACTION_TESTS8}
         otbLinkPathListFilterNew)


add_test(feTvLinkPathListFilter ${FEATUREEXTRACTION_TESTS8}
--compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvLinkPathOutput.txt
  ${TEMP}/feTvLinkPathOutput.txt
        otbLinkPathListFilter
 ${TEMP}/feTvLinkPathOutput.txt
         30  40
   1 1 51 1 |
  61 1 91 1 |
 111 11 211 11 |
   1 51 51 51 |
  71 71 71 91 |
  71 131 71 201 |
  141 111 101 111 |
  151 111 191 111 |
  111  71 141  71 |
  181  71 151  71 |
   11 131  11 101 |
   11 171  11 141
        )

# -------            otb::LikelihoodPathListFilter   -------------

add_test(feTuLikelihoodPathListFilterNew ${FEATUREEXTRACTION_TESTS8}
         otbLikelihoodPathListFilterNew)

add_test(feTvLikelihoodPathListFilter ${FEATUREEXTRACTION_TESTS8}
--compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvLikelihoodPathOutput.txt
  ${TEMP}/feTvLikelihoodPathOutput.txt
        otbLikelihoodPathListFilter
 ${INPUTDATA}/InputForRoadDetection_NonMaxRem.hdr
 ${TEMP}/feTvLikelihoodPathOutput.txt
 157 335 204 376 |
 21 305 35 308 77 354 85 358 |
 73 160 126 173
        )

# -------            otb::ParallelLinePathListFilter   -------------

add_test(feTuParallelLinePathListFilterNew ${FEATUREEXTRACTION_TESTS8}
         otbParallelLinePathListFilterNew)

add_test(feTvParallelLinePathListFilter ${FEATUREEXTRACTION_TESTS8}
         otbParallelLinePathListFilter)


# -------            otb::RoadExtractionFilter   -------------

add_test(feTuRoadExtractionFilterNew ${FEATUREEXTRACTION_TESTS8}
         otbRoadExtractionFilterNew)

add_test(feTvRoadExtractionFilter ${FEATUREEXTRACTION_TESTS8}
       --compare-image ${EPSILON_8}
  ${BASELINE}/feTvOutputRoadDetection.hdr
  ${TEMP}/feTvOutputRoadDetection.hdr
  otbRoadExtractionFilter
   ${INPUTDATA}/qb_RoadExtract.img.hdr
                 ${TEMP}/feTvOutputRoadDetection.hdr
         337 557 432 859  # ReferencePixel
         1.0              # Alpha
         0.00005          # AmplitudeThreshold
         1.0              # Tolerance
         22.5             # (degre) MaxAngle (3.14159265358979323846/8.)
         1.0              # FirstMeanDistanceThreshold
         10.0             # SecondMeanDistanceThreshold
         25.           # DistanceThreshold for LinkPathFiler
         22.5           # AngularThreshold for LinkPathFiler
         )

# -------            otb::AddCarvingPathFilter   -------------
add_test(feTuAddCarvingPathFilterNew ${FEATUREEXTRACTION_TESTS8}
         otbAddCarvingPathFilterNew)


# -------            otb::RemoveCarvingPathFilter   -------------
add_test(feTuRemoveCarvingPathFilterNew ${FEATUREEXTRACTION_TESTS8}
         otbRemoveCarvingPathFilterNew)

# -------            otb::ImageToCarvingPathFilter   -------------
add_test(feTuImageToCarvingPathFilterNew ${FEATUREEXTRACTION_TESTS8}
         otbImageToCarvingPathFilterNew)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTest9 ~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# -------            otb::ImageToSIFTKeyPointSetFilter   -------------

add_test(feTuImageToSIFTKeyPointSetFilterNew ${FEATUREEXTRACTION_TESTS9}
         otbImageToSIFTKeyPointSetFilterNew)


add_test(feTvImageToSIFTKeyPointSetFilterDistanceMap ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPSILON_3}
        ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterDistanceMap.txt
        ${TEMP}/feTvImageToSIFTKeyPointSetFilterDistanceMap.txt
         otbImageToSIFTKeyPointSetFilterDistanceMap
        ${INPUTDATA}/scene.png
        6 3 0.08 10.0
        15.0 # rotation
        1.2 # zoom factor
        10 255
        ${TEMP}/feTvImageToSIFTKeyPointSetFilterDistanceMap.txt
)

add_test(feTvImageToSIFTKeyPointSetFilterSceneOutputInterestPointAscii ${FEATUREEXTRACTION_TESTS9}
--ignore-order --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
         otbImageToSIFTKeyPointSetFilterOutputInterestPointAscii
  ${INPUTDATA}/scene.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
  6 3 0.08 10.0
)

add_test(feTvImageToSIFTKeyPointSetFilterSceneDescriptorAscii ${FEATUREEXTRACTION_TESTS9}
--ignore-order --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
         otbImageToSIFTKeyPointSetFilterOutputDescriptorAscii
  ${INPUTDATA}/scene.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
  6 3 0.08 10.0
)

add_test(feTvImageToSIFTKeyPointSetFilterSquareOutputImage ${FEATUREEXTRACTION_TESTS9}
--compare-image ${EPSILON_8}
  ${BASELINE}/feTvImageToSIFTKeyPointSetFilterSquareImageOutput.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareImageOutput.png
         otbImageToSIFTKeyPointSetFilterOutputImage
  ${INPUTDATA}/carre.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareImageOutput.png
  7 3 0.01 10.0
)

add_test(feTvImageToSIFTKeyPointSetFilterSquareOutputAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput.txt
         otbImageToSIFTKeyPointSetFilterOutputAscii
  ${INPUTDATA}/carre.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput.txt
  7 3 0.01 10.0
)

add_test(feTvImageToSIFTKeyPointSetFilterSquareRotatedOutputAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput.txt
         otbImageToSIFTKeyPointSetFilterOutputAscii
  ${INPUTDATA}/carre_ori.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput.txt
  7 3 0.01 10.0
)

add_test(feTvImageToSIFTKeyPointSetFilterGridOutputImage ${FEATUREEXTRACTION_TESTS9}
--compare-image ${EPSILON_8}
  ${BASELINE}/feTvImageToSIFTKeyPointSetFilterGridImageOutput.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterGridImageOutput.png
         otbImageToSIFTKeyPointSetFilterOutputImage
  ${INPUTDATA}/damier.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterGridImageOutput.png
  7 3 0.01 10.0
)

add_test(feTvImageToSIFTKeyPointSetFilterGridOutputAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterGridKeysOutput.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterGridKeysOutput.txt
         otbImageToSIFTKeyPointSetFilterOutputAscii
  ${INPUTDATA}/damier.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterGridKeysOutput.txt
  7 3 0.01 10.0
)

add_test(feTvImageToSIFTKeyPointSetFilterSquare2OutputAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput2.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput2.txt
         otbImageToSIFTKeyPointSetFilterOutputAscii
  ${INPUTDATA}/carre.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput2.txt
  1 3 0.2 0.9
)

add_test(feTvImageToSIFTKeyPointSetFilterSquareRotated2OutputAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput2.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput2.txt
         otbImageToSIFTKeyPointSetFilterOutputAscii
  ${INPUTDATA}/carre_ori.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput2.txt
  1 3 0.215 10.0
)

add_test(feTvImageToSIFTKeyPointSetFilterQB_SuburbOutputAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterQB_SuburbOutputAscii.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterQB_SuburbOutputAscii.txt
         otbImageToSIFTKeyPointSetFilterOutputAscii
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterQB_SuburbOutputAscii.txt
  2 3 5.0 0.0
)


# -------            otb::HessianToHessianDeterminantImageFilter   -------------
add_test(feTuImageToHessianDeterminantImageFilterNew ${FEATUREEXTRACTION_TESTS9}
         otbImageToHessianDeterminantImageFilterNew)


add_test(feTvImageToHessianDeterminantImageFilter ${FEATUREEXTRACTION_TESTS9}
--compare-image ${EPSILON_3}
  ${BASELINE}/feTvImageToHessianDeterminantImageFilterOutput.tif
  ${TEMP}/feTvImageToHessianDeterminantImageFilterOutput.tif
         otbImageToHessianDeterminantImageFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
  ${TEMP}/feTvImageToHessianDeterminantImageFilterOutput.tif
  1.5
   )


# -------            otb::ImageFittingPolygonListFilter   -------------
add_test(feTuImageFittingPolygonListFilterNew ${FEATUREEXTRACTION_TESTS9}
         otbImageFittingPolygonListFilterNew)

add_test(feTvImageFittingPolygonListFilter ${FEATUREEXTRACTION_TESTS9}
--compare-ogr ${EPSILON_3}
        ${BASELINE_FILES}/feTvImageFittingPolygonListFilter_Output.shp
        ${TEMP}/feTvImageFittingPolygonListFilter_Output.shp
         otbImageFittingPolygonListFilter
        ${INPUTDATA}/polygon.png
        ${INPUTDATA}/polygon-start.shp
        ${TEMP}/feTvImageFittingPolygonListFilter_Output.shp
        5 10
)

#--------- SimpleCountStrategy --------------

add_test(feTvSimplePointCountStrategyTest ${FEATUREEXTRACTION_TESTS9}
    otbSimplePointCountStrategyTest)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests10 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::ImageToSURFKeyPointSetFilterNew   -------------

add_test(feTuImageToSURFKeyPointSetFilterNew ${FEATUREEXTRACTION_TESTS10}
         otbImageToSURFKeyPointSetFilterNew)


add_test(feTvImageToSURFKeyPointSetFilterSceneOutputInterestPointAscii ${FEATUREEXTRACTION_TESTS10}
--compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputInterestPoint.txt
  ${TEMP}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputInterestPoint.txt
         otbImageToSURFKeyPointSetFilterOutputInterestPointAscii
  ${INPUTDATA}/scene.png
  ${TEMP}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputInterestPoint.txt
  3 3
)

add_test(feTvImageToSURFKeyPointSetFilterSceneOutputDescriptorAscii ${FEATUREEXTRACTION_TESTS10}
--compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputDescriptor.txt
  ${TEMP}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputDescriptor.txt
         otbImageToSURFKeyPointSetFilterOutputDescriptorAscii
  ${INPUTDATA}/scene.png
  ${TEMP}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputDescriptor.txt
  3 3
)



# -------            otb::ImageToFastSIFTKeyPointSetFilter   -------------
if(OTB_USE_SIFTFAST)

add_test(feTuImageToFastSIFTKeyPointSetFilterNew ${FEATUREEXTRACTION_TESTS10}
         otbImageToFastSIFTKeyPointSetFilterNew)

#Note the epsilon is according to relative difference
add_test(feTvImageToFastSIFTKeyPointSetFilterSceneOutputDescriptorAscii ${FEATUREEXTRACTION_TESTS10}
--ignore-order --epsilon-boundary 0.01 --compare-ascii ${EPSILON_2}
  ${BASELINE_FILES}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
  ${TEMP}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
         otbImageToFastSIFTKeyPointSetFilterOutputDescriptorAscii
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
  ${TEMP}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
  6
)

add_test(feTvImageToFastSIFTKeyPointSetFilterSceneOutputInterestPointAscii ${FEATUREEXTRACTION_TESTS10}
--ignore-order --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
  ${TEMP}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
         otbImageToFastSIFTKeyPointSetFilterOutputInterestPointAscii
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
  ${TEMP}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
  6
)

endif()


# --------- MatchingFilter ----------------
add_test(feTuKeyPointSetsMatchingFilterNew ${FEATUREEXTRACTION_TESTS10}
 otbKeyPointSetsMatchingFilterNew)

add_test(feTvKeyPointSetsMatchingFilter ${FEATUREEXTRACTION_TESTS10}
--compare-ascii ${EPSILON_3}
 ${BASELINE_FILES}/feTvKeyPointSetsMatchingFilterOutputAscii.txt
 ${TEMP}/feTvKeyPointSetsMatchingFilterOutputAscii.txt
    otbKeyPointSetsMatchingFilter
 ${TEMP}/feTvKeyPointSetsMatchingFilterOutputAscii.txt
 0.6 0
)

#--------- LandMark
add_test(feTuLandmarkNew ${FEATUREEXTRACTION_TESTS10}
 otbLandmarkNew)



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests11 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# -------            otb::CloudEstimatorFilterNew   -------------
add_test(feTuCloudEstimatorFilterNew ${FEATUREEXTRACTION_TESTS11}
    otbCloudEstimatorFilterNew)

add_test(feTvCloudEstimatorDefaultFilter ${FEATUREEXTRACTION_TESTS11}
--compare-image ${EPSILON_3}
     ${BASELINE}/feTvSpectralAngleOutput.tif
     ${TEMP}/feTvSpectralAngleOutput.tif
    otbCloudEstimatorDefaultFilter
    ${INPUTDATA}/ExtrZoneNuageuse.tif
    ${TEMP}/feTvSpectralAngleOutput.tif
    500
    731
    500
    632
)

add_test(feTvCloudEstimatorFilter ${FEATUREEXTRACTION_TESTS11}
--compare-image ${EPSILON_3}
     ${BASELINE}/feTvCloudEstimatorOutput.tif
     ${TEMP}/feTvCloudEstimatorOutput.tif
    otbCloudEstimatorFilter
    ${INPUTDATA}/ExtrZoneNuageuse.tif
    ${TEMP}/feTvCloudEstimatorOutput.tif
    500
    731
    500
    632
    0.25    # variance
)

# -------            otb::CloudDetectionFilterNew   -------------

add_test(feTuCloudDetectionFilterNew ${FEATUREEXTRACTION_TESTS11}
    otbCloudDetectionFilterNew)

add_test(feTvCloudDetectionFilter ${FEATUREEXTRACTION_TESTS11}
 --compare-image ${EPSILON_3}
      ${BASELINE}/feTvCloudDetectionOutput.tif
      ${TEMP}/feTvCloudDetectionOutput.tif
     otbCloudDetectionFilter
     ${INPUTDATA}/ExtrZoneNuageuse.tif
     ${TEMP}/feTvCloudDetectionOutput.tif
     500
     731
     500
     632
     0.25   # variance
     0.95   # minthreshold
     1.0    # maxthreshold
 )


add_test(feTpSimplifyManyPathListFilter ${FEATUREEXTRACTION_TESTS11}
        otbSimplifyManyPathListFilter
 ${TEMP}/feTvSimplifyManyPathOutput.txt
         1.0
        )


# -------            otb::LineSegmentDetector   -------------
add_test(feTuLineSegmentDetectorNew ${FEATUREEXTRACTION_TESTS11}
    otbLineSegmentDetectorNew)

add_test(feTvLineSegmentDetector ${FEATUREEXTRACTION_TESTS11}
--compare-ogr  ${EPSILON_8}
            ${BASELINE_FILES}/feTvLineSegmentDetectorOutput.shp
            ${TEMP}/feTvLineSegmentDetectorOutput.shp
    otbLineSegmentDetector
     ${INPUTDATA}/scene.png
     ${TEMP}/feTvLineSegmentDetectorOutput.shp
)

# -------            otb::LineSpatialObjectListToRightAnglePointSetFilter   -------------
add_test(feTuLineSpatialObjectListToRightAnglePointSetFilterNew ${FEATUREEXTRACTION_TESTS11}
    otbLineSpatialObjectListToRightAnglePointSetFilterNew)

add_test(feTvLineSpatialObjectListToRightAnglePointSetFilter ${FEATUREEXTRACTION_TESTS11}
 --compare-ascii ${EPSILON_3}
 ${BASELINE_FILES}/feTvLineSpatialObjectListToRightAnglePointSetFilterOutputAscii.txt
 ${TEMP}/feTvLineSpatialObjectListToRightAnglePointSetFilterOutputAscii.txt
    otbLineSpatialObjectListToRightAnglePointSetFilter
    ${INPUTDATA}/carre.png
    ${TEMP}/feTvLineSpatialObjectListToRightAnglePointSetFilterOutputAscii.txt
)

add_test(feTvLineSpatialObjectListToRightAnglePointSetFilterBySteps ${FEATUREEXTRACTION_TESTS11}
 --compare-ascii ${EPSILON_3}
 ${BASELINE_FILES}/feTvLineSpatialObjectListToRightAnglePointSetFilterOutputAscii.txt
 ${TEMP}/feTvLineSpatialObjectListToRightAnglePointSetFilterByStepsOutputAscii.txt
    otbLineSpatialObjectListToRightAnglePointSetFilterByStepsOutputAscii
    ${INPUTDATA}/carre.png
    ${TEMP}/feTvLineSpatialObjectListToRightAnglePointSetFilterByStepsOutputAscii.txt
)

#add_test(feTvLineSpatialObjectListToRightAnglePointSetFilterOutputImage ${FEATUREEXTRACTION_TESTS11}
# --compare-ascii ${EPSILON_3}
# ${BASELINE_FILES}/feTvLineSpatialObjectListToRightAnglePointSetFilterByStepsOutputAscii.txt
# ${TEMP}/feTvLineSpatialObjectListToRightAnglePointSetFilterByStepsOutputAscii.txt
#    otbLineSpatialObjectListToRightAnglePointSetFilterOutputImage
#    ${INPUTDATA}/prison_toulouse.tif
#    ${TEMP}/feTvLineSpatialObjectListToRightAnglePointSetFilterByStepsOutputAscii.txt
#    ${TEMP}/feTvLineSpatialObjectListToRightAnglePointSetFilterOutputImage.tif
#)

# -------            otb::VectorDataToRightAnglePointSetFilter   -------------
add_test(feTuVectorDataToRightAngleVectorDataFilterNew ${FEATUREEXTRACTION_TESTS11}
    otbVectorDataToRightAngleVectorDataFilterNew)

add_test(feTvVectorDataToRightAngleVectorDataFilter ${FEATUREEXTRACTION_TESTS11}
 --compare-ogr ${EPSILON_8}
    ${BASELINE_FILES}/feTvVectorDataToRightAngleVectorDataFilterOutput.shp
    ${TEMP}/feTvVectorDataToRightAngleVectorDataFilterOutput.shp
  otbVectorDataToRightAngleVectorDataFilter
    ${BASELINE_FILES}/feTvLineSegmentDetectorOutput.shp     #input vectordata
    ${TEMP}/feTvVectorDataToRightAngleVectorDataFilterOutput.shp  #output text file
    #${TEMP}/feTvVectorDataToRightAngleVectorDataOutput.tif #output image [optional] UNDER CONSTRUCTION
)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests14 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------    otb::SFSTexturesImageFilter   -------------
add_test(feTuSFSTexturesImageFilterNew ${FEATUREEXTRACTION_TESTS14}
    otbSFSTexturesImageFilterNew)

add_test(feTvSFSTexturesImageFilterTest ${FEATUREEXTRACTION_TESTS14}
--compare-n-images ${EPSILON_8}
     5
                   ${BASELINE}/feTvSFSTexturesLengthImageFilterTest.tif
     ${TEMP}/feTvSFSTexturesLengthImageFilterTest.tif
     ${BASELINE}/feTvSFSTexturesWidthImageFilterTest.tif
     ${TEMP}/feTvSFSTexturesWidthImageFilterTest.tif
     ${BASELINE}/feTvSFSTexturesWMeanImageFilterTest.tif
     ${TEMP}/feTvSFSTexturesWMeanImageFilterTest.tif
     ${BASELINE}/feTvSFSTexturesRatioImageFilterTest.tif
     ${TEMP}/feTvSFSTexturesRatioImageFilterTest.tif
     ${BASELINE}/feTvSFSTexturesSDImageFilterTest.tif
     ${TEMP}/feTvSFSTexturesSDImageFilterTest.tif
   otbSFSTexturesImageFilterTest
    ${INPUTDATA}/poupees_sub_c1.png
    ${TEMP}/feTvSFSTexturesLengthImageFilterTest.tif
    ${TEMP}/feTvSFSTexturesWidthImageFilterTest.tif
    ${TEMP}/feTvSFSTexturesWMeanImageFilterTest.tif
    ${TEMP}/feTvSFSTexturesRatioImageFilterTest.tif
    ${TEMP}/feTvSFSTexturesSDImageFilterTest.tif
    50 # spectral threshold
    20 # spatial threshold
    15 # direction
    4 # max min/max number takes in care for ratio
    0.6 # alpha value
    # PSI is disable for texture selection test
)

# -------            otb::UrbanAreaDetectionImageFilter ------------------------------
add_test(raTuUrbanAreaDetectionImageFilterNew ${FEATUREEXTRACTION_TESTS14}
        otbUrbanAreaDetectionImageFilterNew
)

add_test(raTvUrbanAreaDetectionImageFilter ${FEATUREEXTRACTION_TESTS14}
   --compare-image ${EPSILON_8}   ${BASELINE}/raTvUrbanAreaDetection.png
                     ${TEMP}/raTvUrbanAreaDetection.png
        otbUrbanAreaDetectionImageFilter
        ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
        ${TEMP}/raTvUrbanAreaDetection.png
        3    # red
        2    # green
        4    # nir
        0.8  # threshold NDVI+NDWI
        0.005 # threshold EdgeDensity
)

# -------      otb::RegionImageToRectangularPathListFilter   -------------
add_test(feTuRegionImageToRectangularPathListFilterNew ${FEATUREEXTRACTION_TESTS14}
         otbRegionImageToRectangularPathListFilterNew)

add_test(feTvRegionImageToRectangularPathListFilter ${FEATUREEXTRACTION_TESTS14}
   --compare-ascii ${EPSILON_3}     ${BASELINE_FILES}/feRectangle.txt
                        ${TEMP}/feRectangle.txt
        otbRegionImageToRectangularPathListFilter
 ${INPUTDATA}/rectangle.png
 ${TEMP}/feRectangle.txt
        0.9 #fit score
 10)  #minimum size




# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests15 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::StreamingLineSegmentDetector   -------------
add_test(feTuStreamingLineSegmentDetectorNew ${FEATUREEXTRACTION_TESTS15}
    otbStreamingLineSegmentDetectorNew)

add_test(feTvStreamingLineSegmentDetector10 ${FEATUREEXTRACTION_TESTS15}
--compare-ogr  ${EPSILON_8}
     ${BASELINE_FILES}/feTvStreamingLineSegmentDetectorOutput10.shp
     ${TEMP}/feTvStreamingLineSegmentDetectorOutput10.shp
    otbStreamingLineSegmentDetector
      ${INPUTDATA}/scene.png
      ${TEMP}/feTvStreamingLineSegmentDetectorOutput10.shp
      10
)

add_test(feTvStreamingLineSegmentDetector1000 ${FEATUREEXTRACTION_TESTS15}
--compare-ogr  ${EPSILON_8}
     ${BASELINE_FILES}/feTvStreamingLineSegmentDetectorOutput1000.shp
     ${TEMP}/feTvStreamingLineSegmentDetectorOutput1000.shp
    otbStreamingLineSegmentDetector
      ${INPUTDATA}/scene.png
      ${TEMP}/feTvStreamingLineSegmentDetectorOutput1000.shp
      1000
)

# -------            otb::SqrtSpectralAngleImageFilter (Ossman)------------------------------
add_test(feTvSqrtSpectralAngleImageFilter ${FEATUREEXTRACTION_TESTS15}
 --compare-image ${EPSILON_8}   ${BASELINE}/feSqrtSpectralAngle.tif
                   ${TEMP}/feSqrtSpectralAngle.tif
        otbSqrtSpectralAngleImageFilter
        ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
        ${TEMP}/feSqrtSpectralAngle.tif
        185 270 150 619 #reference pixel
)

# -------            otb::ScalarImageToTexturesFilter (Ossman)------------------------------
add_test(feTuScalarImageToTexturesFilterNew ${FEATUREEXTRACTION_TESTS15}
        otbScalarImageToTexturesFilterNew
)

add_test(feTvScalarImageToTexturesFilter ${FEATUREEXTRACTION_TESTS15}
        --compare-n-images ${EPSILON_10} 8
            ${BASELINE}/feTvScalarImageToTexturesFilterOutputEnergy.tif
            ${TEMP}/feTvScalarImageToTexturesFilterOutputEnergy.tif
            ${BASELINE}/feTvScalarImageToTexturesFilterOutputEntropy.tif
            ${TEMP}/feTvScalarImageToTexturesFilterOutputEntropy.tif
            ${BASELINE}/feTvScalarImageToTexturesFilterOutputCorrelation.tif
            ${TEMP}/feTvScalarImageToTexturesFilterOutputCorrelation.tif
            ${BASELINE}/feTvScalarImageToTexturesFilterOutputInverseDifferenceMoment.tif
            ${TEMP}/feTvScalarImageToTexturesFilterOutputInverseDifferenceMoment.tif
            ${BASELINE}/feTvScalarImageToTexturesFilterOutputInertia.tif
            ${TEMP}/feTvScalarImageToTexturesFilterOutputInertia.tif
            ${BASELINE}/feTvScalarImageToTexturesFilterOutputClusterShade.tif
            ${TEMP}/feTvScalarImageToTexturesFilterOutputClusterShade.tif
            ${BASELINE}/feTvScalarImageToTexturesFilterOutputClusterProminence.tif
            ${TEMP}/feTvScalarImageToTexturesFilterOutputClusterProminence.tif
            ${BASELINE}/feTvScalarImageToTexturesFilterOutputHaralickCorrelation.tif
            ${TEMP}/feTvScalarImageToTexturesFilterOutputHaralickCorrelation.tif
        otbScalarImageToTexturesFilter
            ${INPUTDATA}/Mire_Cosinus.png
            ${TEMP}/feTvScalarImageToTexturesFilterOutput
            8 3 2 2)

add_test(feTuotbGreyLevelCooccurrenceIndexedListNew ${FEATUREEXTRACTION_TESTS15}
         otbGreyLevelCooccurrenceIndexedListNew
)

add_test(feTuScalarImageToAdvancedTexturesFilterNew ${FEATUREEXTRACTION_TESTS15}
        otbScalarImageToAdvancedTexturesFilterNew
)

add_test(feTvScalarImageToAdvancedTexturesFilter ${FEATUREEXTRACTION_TESTS15}
        --compare-n-images ${NOTOL} 10
            ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputVariance.tif
            ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputVariance.tif
            ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputMean.tif
            ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputMean.tif
            ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputDissimilarity.tif
            ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputDissimilarity.tif
            ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputSumAverage.tif
            ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputSumAverage.tif
            ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputSumVariance.tif
            ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputSumVariance.tif
            ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputSumEntropy.tif
            ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputSumEntropy.tif
            ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputDifferenceEntropy.tif
            ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputDifferenceEntropy.tif
            ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputDifferenceVariance.tif
            ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputDifferenceVariance.tif
            ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputIC1.tif
            ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputIC1.tif
            ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputIC2.tif
            ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputIC2.tif
        otbScalarImageToAdvancedTexturesFilter
        ${INPUTDATA}/Mire_Cosinus.png
        ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutput
        8 5 1 1)


add_test(feTuScalarImageToHigherOrderTexturesFilterNew ${FEATUREEXTRACTION_TESTS15}
        otbScalarImageToHigherOrderTexturesFilterNew
)

add_test(feTvScalarImageToHigherOrderTexturesFilter ${FEATUREEXTRACTION_TESTS15}
       otbScalarImageToHigherOrderTexturesFilter
        ${INPUTDATA}/Mire_Cosinus.png)

add_test(feTuScalarImageToPanTexTextureFilterNew ${FEATUREEXTRACTION_TESTS15}
        otbScalarImageToPanTexTextureFilterNew
)

add_test(feTvScalarImageToPanTexTextureFilter ${FEATUREEXTRACTION_TESTS15}
        --compare-image ${NOTOL}
            ${BASELINE}/feTvScalarImageToPanTexTextureFilterOutputPanTex.tif
            ${TEMP}/feTvScalarImageToPanTexTextureFilterOutputPanTex.tif
        otbScalarImageToPanTexTextureFilter
        ${INPUTDATA}/Mire_Cosinus.png
        ${TEMP}/feTvScalarImageToPanTexTextureFilterOutput
        8 5)



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests16 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::FourierMellinDescriptorsImageFunction   -------------

add_test(feTuFourierMellinDescriptorsNew ${FEATUREEXTRACTION_TESTS16}
  otbFourierMellinDescriptorsNew
  )

add_test(feTvFourierMellinDescriptors ${FEATUREEXTRACTION_TESTS16}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/feTvFourierMellinDescriptors.txt
  ${TEMP}/feTvFourierMellinDescriptors.txt
  otbFourierMellinDescriptors
  ${INPUTDATA}/poupees.png
  4
  4
  ${TEMP}/feTvFourierMellinDescriptors.txt
  )

add_test(feTvFourierMellinDescriptorsScaleInvariant ${FEATUREEXTRACTION_TESTS16}
  otbFourierMellinDescriptorsScaleInvariant
  ${INPUTDATA}/poupees.png
  4
  4
  )

add_test(feTvFourierMellinDescriptorsRotationInvariant ${FEATUREEXTRACTION_TESTS16}
  otbFourierMellinDescriptorsRotationInvariant
  ${INPUTDATA}/poupees.png
  4
  4
  90
  )

# -------            otb::LocalHistogramImageFunction   -------------

add_test(feTuLocalHistogramImageFunctionNew ${FEATUREEXTRACTION_TESTS16}
        otbLocalHistogramImageFunctionNew
)

add_test(feTvLocalHistogramImageFunctionTest ${FEATUREEXTRACTION_TESTS16}
--compare-ascii ${EPSILON_8}
 ${BASELINE_FILES}/feLocalHistogramImage.txt
    ${TEMP}/feLocalHistogramImage.txt
 otbLocalHistogramImageFunctionTest
 ${INPUTDATA}/pyramide.tif
 ${TEMP}/feLocalHistogramImage.txt
 127 127 127 0 128
)

# -------   otb::ImageFunctionAdapter   -------------

add_test(feTuImageFunctionAdaptorNew ${FEATUREEXTRACTION_TESTS16}
        otbImageFunctionAdaptorNew
)

add_test(feTvImageFunctionAdaptor ${FEATUREEXTRACTION_TESTS16}
        otbImageFunctionAdaptor
                ${INPUTDATA}/poupees.png
         )

# -------   otb::MetaImageFunction   -------------
add_test(feTuMetaImageFunctionNew ${FEATUREEXTRACTION_TESTS16}
        otbMetaImageFunctionNew
)

add_test(feTvMetaImageFunction ${FEATUREEXTRACTION_TESTS16}
--compare-ascii ${EPSILON_8}
    ${BASELINE_FILES}/feTvMetaImageFunction.txt
    ${TEMP}/feTvMetaImageFunction.txt
    otbMetaImageFunction
 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
    ${TEMP}/feTvMetaImageFunction.txt
    451846.014047961 5412466.57452216
)

# -------   otb::HaralickTexturesImageFunction   -------------
add_test(feTuHaralickTexturesImageFunctionNew ${FEATUREEXTRACTION_TESTS16}
        otbHaralickTexturesImageFunctionNew
)

add_test(feTvHaralickTexturesImageFunction ${FEATUREEXTRACTION_TESTS16}
--compare-ascii ${EPSILON_8}
    ${BASELINE_FILES}/feTvHaralickTexturesImageFunction.txt
    ${TEMP}/feTvHaralickTexturesImageFunction.txt
    otbHaralickTexturesImageFunction
 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
    ${TEMP}/feTvHaralickTexturesImageFunction.txt
    451846.014047961 5412466.57452216
)

# ------ otb::HistogramOfOrientedGradientCovariantImageFunction --------
add_test(feTuHistogramOfOrientedGradientCovariantImageFunction ${FEATUREEXTRACTION_TESTS16}
         otbHistogramOfOrientedGradientCovariantImageFunctionNew)

add_test(feTvHistogramOfOrientedGradientCovariantImageFunction ${FEATUREEXTRACTION_TESTS16}
--compare-ascii ${EPSILON_8}
  ${BASELINE_FILES}/feTvHistogramOfOrientedGradientCovariantImageFunction.txt
  ${TEMP}/feTvHistogramOfOrientedGradientCovariantImageFunction.txt
         otbHistogramOfOrientedGradientCovariantImageFunction
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/feTvHistogramOfOrientedGradientCovariantImageFunction.txt
  5 273 64
)

# ------ otb::RadiometricMomentsImageFilter --------
add_test(feTuRadiometricMomentsImageFilterNew ${FEATUREEXTRACTION_TESTS16}
         otbRadiometricMomentsImageFilterNew)

add_test(feTvRadiometricMomentsImageFilter ${FEATUREEXTRACTION_TESTS16}
         --compare-image ${EPSILON_8}
			${BASELINE}/feTvRadiometricMomentsImageFilter.tif
            ${TEMP}/feTvRadiometricMomentsImageFilter.tif
        otbRadiometricMomentsImageFilter
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/feTvRadiometricMomentsImageFilter.tif
        3 #radius
        )

# A enrichir
set(BasicFeatureExtraction_SRCS1
otbFeatureExtractionTests1.cxx
otbAlignImageToPath.cxx
otbDrawPath.cxx
otbDrawPathAlign.cxx
otbComplexMomentsImageFunction.cxx
otbRealMomentsImageFunction.cxx
otbRadiometricMomentsImageFunction.cxx
otbHuMomentsImageFunction.cxx
otbFlusserMomentsImageFunction.cxx
otbComplexMomentPathNew.cxx
otbComplexMomentPath.cxx
otbComplexMomentPathFloat.cxx
otbHuPathNew.cxx
otbHuPath.cxx
)

if(OTB_USE_DEPRECATED)
set(BasicFeatureExtraction_SRCS1
${BasicFeatureExtraction_SRCS1}
)
endif()

set(BasicFeatureExtraction_SRCS2
otbFeatureExtractionTests2.cxx
otbFlusserPathNew.cxx
otbFlusserPath.cxx
otbOrientationPathNew.cxx
otbOrientationPath.cxx
otbCompacityPathNew.cxx
otbCompacityPathCircle.cxx
otbCompacityPathSquare.cxx
otbCompacityPathRectangle.cxx
)
set(BasicFeatureExtraction_SRCS3
otbFeatureExtractionTests3.cxx
otbTouziEdgeDetectorNew.cxx
otbTouziEdgeDetector.cxx
otbTouziEdgeDetectorDirection.cxx
otbLineDetectorBaseNew.cxx
otbLineCorrelationDetectorNew.cxx
otbLineCorrelationDetectorLinear.cxx
otbLineCorrelationDetector.cxx
otbLineRatioDetectorNew.cxx
otbLineRatioDetectorLinear.cxx
otbLineRatioDetector.cxx
)
set(BasicFeatureExtraction_SRCS4
otbFeatureExtractionTests4.cxx
otbAssociativeSymmetricalSumNew.cxx
otbAssociativeSymmetricalSum.cxx
otbAssymmetricFusionOfLineDetectorNew.cxx
otbAssymmetricFusionOfLineDetector.cxx
otbHarrisImage.cxx
otbMultiplyByScalarImageTest.cxx
otbThresholdImageToPointSetFilterNew.cxx
otbThresholdImageToPointSetTest.cxx
)
set(BasicFeatureExtraction_SRCS5
otbFeatureExtractionTests5.cxx
otbHarrisToPointSet.cxx
otbPixelSuppressionByDirectionNew.cxx
otbPixelSuppressionByDirection.cxx
otbFillGapsFilterNew.cxx
otbFillGapsFilter.cxx
otbLocalHoughNew.cxx
otbLocalHough.cxx
otbLocalHoughDraw.cxx
otbModulusAndDirectionImageFiltersNew.cxx
otbHoughTransform2DLinesImageTest.cxx
)
set(BasicFeatureExtraction_SRCS6
otbFeatureExtractionTests6.cxx
otbExtractSegmentsNew.cxx
otbExtractSegments.cxx
otbFourierMellinImageFilterNew.cxx #Comment because generate error "duplicate vnl_fft_base<3...>" with otbFourierMellinImageFilter.cxx test on Mac OSX platform
#otbFourierMellinImageFilterTestFFT.cxx
otbFourierMellinImageFilter.cxx
otbImageToEdgePathFilterNew.cxx
otbImageToEdgePathFilter.cxx
otbNeighborhoodScalarProductFilterNew.cxx
otbNeighborhoodScalarProductFilter.cxx
)
set(BasicFeatureExtraction_SRCS7
otbFeatureExtractionTests7.cxx
otbRemoveIsolatedByDirectionFilterNew.cxx
otbRemoveIsolatedByDirectionFilter.cxx
otbRemoveWrongDirectionFilterNew.cxx
otbRemoveWrongDirectionFilter.cxx
otbNonMaxRemovalByDirectionFilterNew.cxx
otbNonMaxRemovalByDirectionFilter.cxx
otbVectorizationPathListFilterNew.cxx
otbVectorizationPathListFilter.cxx
otbSimplifyPathListFilterNew.cxx
otbSimplifyPathListFilter.cxx
otbBreakAngularPathListFilterNew.cxx
otbBreakAngularPathListFilter.cxx
)
set(BasicFeatureExtraction_SRCS8
otbFeatureExtractionTests8.cxx
otbRemoveTortuousPathListFilterNew.cxx
otbRemoveTortuousPathListFilter.cxx
otbLinkPathListFilterNew.cxx
otbLinkPathListFilter.cxx
otbLikelihoodPathListFilterNew.cxx
otbLikelihoodPathListFilter.cxx
otbParallelLinePathListFilterNew.cxx
otbParallelLinePathListFilter.cxx
otbRoadExtractionFilterNew.cxx
otbRoadExtractionFilter.cxx
otbAddCarvingPathFilterNew.cxx
otbRemoveCarvingPathFilterNew.cxx
otbImageToCarvingPathFilterNew.cxx
)
set(BasicFeatureExtraction_SRCS9
otbFeatureExtractionTests9.cxx
otbImageToSIFTKeyPointSetFilterNew.cxx
otbImageToSIFTKeyPointSetFilterDistanceMap.cxx
otbImageToSIFTKeyPointSetFilterOutputInterestPointAscii.cxx
otbImageToSIFTKeyPointSetFilterOutputDescriptorAscii.cxx
otbImageToSIFTKeyPointSetFilterOutputAscii.cxx
otbImageToSIFTKeyPointSetFilterOutputImage.cxx
otbImageToHessianDeterminantImageFilterNew.cxx
otbImageToHessianDeterminantImageFilter.cxx
otbImageFittingPolygonListFilter.cxx
otbImageFittingPolygonListFilterNew.cxx
otbSimplePointCountStrategyTest.cxx
)
set(BasicFeatureExtraction_SRCS10
otbFeatureExtractionTests10.cxx
otbImageToSURFKeyPointSetFilterNew.cxx
otbImageToSURFKeyPointSetFilterOutputInterestPointAscii.cxx
otbImageToSURFKeyPointSetFilterOutputDescriptorAscii.cxx
otbKeyPointSetsMatchingFilterNew.cxx
otbKeyPointSetsMatchingFilter.cxx
otbLandmarkNew.cxx
)
if(OTB_USE_SIFTFAST)
set(BasicFeatureExtraction_SRCS10
      ${BasicFeatureExtraction_SRCS10}
      otbImageToFastSIFTKeyPointSetFilterNew.cxx
      otbImageToFastSIFTKeyPointSetFilterOutputDescriptorAscii.cxx
      otbImageToFastSIFTKeyPointSetFilterOutputInterestPointAscii.cxx )
endif()

set(BasicFeatureExtraction_SRCS11
otbFeatureExtractionTests11.cxx
otbCloudEstimatorFilterNew.cxx
otbCloudEstimatorDefaultFilter.cxx
otbCloudEstimatorFilter.cxx
otbCloudDetectionFilterNew.cxx
otbCloudDetectionFilter.cxx
otbSimplifyManyPathListFilter.cxx
otbLineSegmentDetector.cxx
otbLineSpatialObjectListToRightAnglePointSetFilterNew.cxx
otbLineSpatialObjectListToRightAnglePointSetFilter.cxx
otbLineSpatialObjectListToRightAnglePointSetFilterByStepsOutputAscii.cxx
#otbLineSpatialObjectListToRightAnglePointSetFilterOutputImage.cxx
otbVectorDataToRightAngleVectorDataFilter.cxx
)


set(BasicFeatureExtraction_SRCS14
otbFeatureExtractionTests14.cxx
otbSFSTexturesImageFilterNew.cxx
otbSFSTexturesImageFilterTest.cxx
otbUrbanAreaDetectionImageFilterNew.cxx
otbUrbanAreaDetectionImageFilter.cxx
otbRegionImageToRectangularPathListFilterNew.cxx
otbRegionImageToRectangularPathListFilter.cxx
)

set(BasicFeatureExtraction_SRCS15
otbFeatureExtractionTests15.cxx
otbStreamingLineSegmentDetector.cxx
otbSqrtSpectralAngleImageFilter.cxx
otbScalarImageToTexturesFilterNew.cxx
otbScalarImageToTexturesFilter.cxx
otbGreyLevelCooccurrenceIndexedListNew.cxx
otbScalarImageToAdvancedTexturesFilterNew.cxx
otbScalarImageToAdvancedTexturesFilter.cxx
otbScalarImageToPanTexTextureFilterNew.cxx
otbScalarImageToPanTexTextureFilter.cxx
otbScalarImageToHigherOrderTexturesFilter.cxx
)

set(BasicFeatureExtraction_SRCS16
otbFeatureExtractionTests16.cxx
otbFourierMellinDescriptors.cxx
otbLocalHistogramImageFunctionNew.cxx
otbLocalHistogramImageFunctionTest.cxx
otbImageFunctionAdaptor.cxx
otbMetaImageFunction.cxx
otbHaralickTexturesImageFunction.cxx
otbHistogramOfOrientedGradientCovariantImageFunction.cxx
otbRadiometricMomentsImageFilter.cxx
)


OTB_ADD_EXECUTABLE(otbFeatureExtractionTests1 "${BasicFeatureExtraction_SRCS1}" "OTBFeatureExtraction;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbFeatureExtractionTests2 "${BasicFeatureExtraction_SRCS2}" "OTBFeatureExtraction;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbFeatureExtractionTests3 "${BasicFeatureExtraction_SRCS3}" "OTBFeatureExtraction;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbFeatureExtractionTests4 "${BasicFeatureExtraction_SRCS4}" "OTBFeatureExtraction;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbFeatureExtractionTests5 "${BasicFeatureExtraction_SRCS5}" "OTBFeatureExtraction;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbFeatureExtractionTests6 "${BasicFeatureExtraction_SRCS6}" "OTBFeatureExtraction;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbFeatureExtractionTests7 "${BasicFeatureExtraction_SRCS7}" "OTBFeatureExtraction;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbFeatureExtractionTests8 "${BasicFeatureExtraction_SRCS8}" "OTBFeatureExtraction;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbFeatureExtractionTests9 "${BasicFeatureExtraction_SRCS9}" "OTBFeatureExtraction;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbFeatureExtractionTests10 "${BasicFeatureExtraction_SRCS10}" "OTBFeatureExtraction;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbFeatureExtractionTests11 "${BasicFeatureExtraction_SRCS11}" "OTBFeatureExtraction;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbFeatureExtractionTests14 "${BasicFeatureExtraction_SRCS14}" "OTBFeatureExtraction;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbFeatureExtractionTests15 "${BasicFeatureExtraction_SRCS15}" "OTBFeatureExtraction;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbFeatureExtractionTests16 "${BasicFeatureExtraction_SRCS16}" "OTBFeatureExtraction;OTBIO;OTBTesting")

endif()
