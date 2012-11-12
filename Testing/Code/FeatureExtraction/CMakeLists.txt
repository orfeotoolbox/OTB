
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


SET(FEATUREEXTRACTION_TESTS1 ${CXX_TEST_PATH}/otbFeatureExtractionTests1)
SET(FEATUREEXTRACTION_TESTS2 ${CXX_TEST_PATH}/otbFeatureExtractionTests2)
SET(FEATUREEXTRACTION_TESTS3 ${CXX_TEST_PATH}/otbFeatureExtractionTests3)
SET(FEATUREEXTRACTION_TESTS4 ${CXX_TEST_PATH}/otbFeatureExtractionTests4)
SET(FEATUREEXTRACTION_TESTS5 ${CXX_TEST_PATH}/otbFeatureExtractionTests5)
SET(FEATUREEXTRACTION_TESTS6 ${CXX_TEST_PATH}/otbFeatureExtractionTests6)
SET(FEATUREEXTRACTION_TESTS7 ${CXX_TEST_PATH}/otbFeatureExtractionTests7)
SET(FEATUREEXTRACTION_TESTS8 ${CXX_TEST_PATH}/otbFeatureExtractionTests8)
SET(FEATUREEXTRACTION_TESTS9 ${CXX_TEST_PATH}/otbFeatureExtractionTests9)
SET(FEATUREEXTRACTION_TESTS10 ${CXX_TEST_PATH}/otbFeatureExtractionTests10)
SET(FEATUREEXTRACTION_TESTS11 ${CXX_TEST_PATH}/otbFeatureExtractionTests11)
SET(FEATUREEXTRACTION_TESTS12 ${CXX_TEST_PATH}/otbFeatureExtractionTests12)
SET(FEATUREEXTRACTION_TESTS13 ${CXX_TEST_PATH}/otbFeatureExtractionTests13)
SET(FEATUREEXTRACTION_TESTS14 ${CXX_TEST_PATH}/otbFeatureExtractionTests14)
SET(FEATUREEXTRACTION_TESTS15 ${CXX_TEST_PATH}/otbFeatureExtractionTests15)
SET(FEATUREEXTRACTION_TESTS16 ${CXX_TEST_PATH}/otbFeatureExtractionTests16)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests1 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ADD_TEST(feTvAlignMV2ITK ${FEATUREEXTRACTION_TESTS1}
   --compare-ascii ${EPSILON_3}     ${BASELINE_FILES}/feAlign.txt
                        ${TEMP}/feAlign.txt
        otbAlignImageToPath
 ${INPUTDATA}/poupees.hdr
 ${TEMP}/feAlign.txt)

# -------            otb::Draw   ------------------------------

ADD_TEST(feTvDrawPathTestCarre ${FEATUREEXTRACTION_TESTS1}
  --compare-image ${NOTOL}  ${BASELINE}/feDrawPathDessinCarre.png
                          ${TEMP}/feDrawPathDessinCarre.png
        otbDrawPathDessinCarre
 ${INPUTDATA}/DeuxTraits.png
 ${TEMP}/feDrawPathDessinCarre.png)

ADD_TEST(feTvDrawPathTestAlign ${FEATUREEXTRACTION_TESTS1}
  --compare-image ${NOTOL}  ${BASELINE}/feDrawPathAlignDeuxTraits.png
                          ${TEMP}/feDrawPathAlignDeuxTraits.png
        otbDrawPathAlign
 ${INPUTDATA}/DeuxTraits.png
 ${TEMP}/feDrawPathAlignDeuxTraits.png)

ADD_TEST(feTuComplexMomentsImageFunctionNew ${FEATUREEXTRACTION_TESTS1}
  otbComplexMomentsImageFunctionNew
  )

ADD_TEST(feTvComplexMomentsImageFunction ${FEATUREEXTRACTION_TESTS1}
  --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feTvComplexMomentsImageFunction.txt
                               ${TEMP}/feTvComplexMomentsImageFunction.txt
  otbComplexMomentsImageFunction
  ${INPUTDATA}/poupees.png
  4
  4
  ${TEMP}/feTvComplexMomentsImageFunction.txt
  )

ADD_TEST(feTvComplexMomentsImageFunctionScaleInvariant ${FEATUREEXTRACTION_TESTS1}
  otbComplexMomentsImageFunctionScaleInvariant
  ${INPUTDATA}/poupees.png
  4
  4
  )

ADD_TEST(feTuRealMomentsImageFunctionNew ${FEATUREEXTRACTION_TESTS1}
  otbRealMomentsImageFunctionNew
  )

ADD_TEST(feTvRealMomentsImageFunction ${FEATUREEXTRACTION_TESTS1}
   --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feTvRealMomentsImageFunction.txt
                        ${TEMP}/feTvRealMomentsImageFunction.txt
        otbRealMomentsImageFunction
 ${INPUTDATA}/poupees.png
        4
        4
        ${TEMP}/feTvRealMomentsImageFunction.txt)

ADD_TEST(feTuRadiometricMomentsImageFunctionNew ${FEATUREEXTRACTION_TESTS1}
  otbRadiometricMomentsImageFunctionNew
  )

ADD_TEST(feTvRadiometricMomentsImageFunction ${FEATUREEXTRACTION_TESTS1}
   --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feTvRadiometricMomentsImageFunction.txt
                                ${TEMP}/feTvRadiometricMomentsImageFunction.txt
        otbRadiometricMomentsImageFunction
 ${INPUTDATA}/poupees.png
        ${TEMP}/feTvRadiometricMomentsImageFunction.txt)

ADD_TEST(feTuHuMomentsImageFunctionNew ${FEATUREEXTRACTION_TESTS1}
        otbHuMomentsImageFunctionNew
        )

ADD_TEST(feTvHuMomentsImageFunction ${FEATUREEXTRACTION_TESTS1}
   --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feTvHuMomentsImageFunction.txt
                        ${TEMP}/feTvHuMomentsImageFunction.txt
        otbHuMomentsImageFunction
 ${INPUTDATA}/poupees.png
        ${TEMP}/feTvHuMomentsImageFunction.txt
        )

ADD_TEST(feTvHuMomentsImageFunctionScaleInvariant ${FEATUREEXTRACTION_TESTS1}
        otbHuMomentsImageFunctionScaleInvariant
 ${INPUTDATA}/poupees.png
        )

ADD_TEST(feTvHuMomentsImageFunctionRotationInvariant ${FEATUREEXTRACTION_TESTS1}
        otbHuMomentsImageFunctionRotationInvariant
 ${INPUTDATA}/poupees.png
        90
        )

ADD_TEST(feTuFlusserMomentsImageFunctionNew ${FEATUREEXTRACTION_TESTS1}
        otbFlusserMomentsImageFunctionNew
)

ADD_TEST(feTvFlusserMomentsImageFunction ${FEATUREEXTRACTION_TESTS1}
   --compare-ascii ${EPSILON_8}     ${BASELINE_FILES}/feTvFlusserMomentsImageFunction.txt
                        ${TEMP}/feTvFlusserMomentsImageFunction.txt
        otbFlusserMomentsImageFunction
 ${INPUTDATA}/poupees.png
        ${TEMP}/feTvFlusserMomentsImageFunction.txt)

ADD_TEST(feTvFlusserMomentsImageFunctionScaleInvariant ${FEATUREEXTRACTION_TESTS1}
        otbFlusserMomentsImageFunctionScaleInvariant
 ${INPUTDATA}/poupees.png
        )

ADD_TEST(feTvFlusserMomentsImageFunctionRotationInvariant ${FEATUREEXTRACTION_TESTS1}
        otbFlusserMomentsImageFunctionRotationInvariant
 ${INPUTDATA}/poupees.png
        90
        )

ADD_TEST(feTuComplexMomentPathNew ${FEATUREEXTRACTION_TESTS1}
        otbComplexMomentPathNew)

ADD_TEST(feTvComplexMomentPath ${FEATUREEXTRACTION_TESTS1}
   --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feComplexMomentPath.txt
                        ${TEMP}/feComplexMomentPath.txt
        otbComplexMomentPath
  1
  1
  ${TEMP}/feComplexMomentPath.txt)

ADD_TEST(feTvComplexMomentPathFloat ${FEATUREEXTRACTION_TESTS1}
   --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feComplexMomentPathFloat.txt
                        ${TEMP}/feComplexMomentPathFloat.txt
        otbComplexMomentPathFloat
  1
  1
  ${TEMP}/feComplexMomentPathFloat.txt)

ADD_TEST(feTuHuPathNew ${FEATUREEXTRACTION_TESTS1}
        otbHuPathNew)

ADD_TEST(feTvHuPath ${FEATUREEXTRACTION_TESTS1}
   --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feHuPath.txt
                        ${TEMP}/feHuPath.txt
        otbHuPath
  ${TEMP}/feHuPath.txt)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests2 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ADD_TEST(feTuFlusserPathNew ${FEATUREEXTRACTION_TESTS2}
        otbFlusserPathNew)

ADD_TEST(feTvFlusserPath ${FEATUREEXTRACTION_TESTS2}
   --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feFlusserPath.txt
                        ${TEMP}/feFlusserPath.txt
        otbFlusserPath
  ${TEMP}/feFlusserPath.txt)


# -------            otb::OrientationPathFunction   ------------------------

ADD_TEST(feTuOrientationPathNew ${FEATUREEXTRACTION_TESTS2}
        otbOrientationPathNew)

ADD_TEST(feTuOrientationPath_000 ${FEATUREEXTRACTION_TESTS2}
        otbOrientationPath 0.0)

ADD_TEST(feTuOrientationPath_045 ${FEATUREEXTRACTION_TESTS2}
        otbOrientationPath 45.0)

ADD_TEST(feTuOrientationPath_090 ${FEATUREEXTRACTION_TESTS2}
        otbOrientationPath 90.0)

ADD_TEST(feTuOrientationPath1_80 ${FEATUREEXTRACTION_TESTS2}
        otbOrientationPath 180.0)

# -------            otb::CompacityPathFunction   ------------------------

ADD_TEST(feTuCompacityPathNew ${FEATUREEXTRACTION_TESTS2}
        otbCompacityPathNew)

ADD_TEST(feTuCompacityPathCircle ${FEATUREEXTRACTION_TESTS2}
        otbCompacityPathCircle 1000)

ADD_TEST(feTuCompacityPathSquare ${FEATUREEXTRACTION_TESTS2}
        otbCompacityPathSquare 10.0)

ADD_TEST(feTuCompacityPathRectangle ${FEATUREEXTRACTION_TESTS2}
        otbCompacityPathRectangle 10.0  20.0)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests3 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# -------            otb::TouziEdgeDetector   ------------------------------

ADD_TEST(feTuTouziNew ${FEATUREEXTRACTION_TESTS3}
        otbTouziEdgeDetectorNew)

ADD_TEST(feTvTouzi ${FEATUREEXTRACTION_TESTS3}
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreTouzi_amst_3.hdr
                          ${TEMP}/feFiltreTouzi_amst_3.hdr
        otbTouziEdgeDetector
 ${INPUTDATA}/amst.png
 ${TEMP}/feFiltreTouzi_amst_3.hdr
 3)

ADD_TEST(feTvTouziDir ${FEATUREEXTRACTION_TESTS3}
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreTouzi_amst_dir_3.hdr
                          ${TEMP}/feFiltreTouzi_amst_dir_3.hdr
        otbTouziEdgeDetectorDirection
 ${INPUTDATA}/amst.png
 ${TEMP}/feFiltreTouzi_amst_dir_3.hdr
 3)

# -------            otb::LineDetectorBase   ------------------------------

ADD_TEST(feTuLineDetectorBaseNew ${FEATUREEXTRACTION_TESTS3}
        otbLineDetectorBaseNew)

# -------            otb::LineCorrelationDetector   ------------------------------

ADD_TEST(feTuLineCorrelationNew ${FEATUREEXTRACTION_TESTS3}
        otbLineCorrelationDetectorNew)

ADD_TEST(feTvLineCorrelationLinear ${FEATUREEXTRACTION_TESTS3}
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

ADD_TEST(feTvLineCorrelation ${FEATUREEXTRACTION_TESTS3}
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreLineCorrelation_amst_2_3.hdr
                          ${TEMP}/feFiltreLineCorrelation_amst_2_3.hdr
        otbLineCorrelationDetector
 ${INPUTDATA}/amst.png
 ${TEMP}/feFiltreLineCorrelation_amst_2_3.hdr
 2 3)

# -------            otb::LineRatioDetector   ------------------------------

ADD_TEST(feTuLineRatioNew ${FEATUREEXTRACTION_TESTS3}
        otbLineRatioDetectorNew)

ADD_TEST(feTvLineRatioLinear ${FEATUREEXTRACTION_TESTS3}
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

ADD_TEST(feTvLineRatioLinearAmsters1 ${FEATUREEXTRACTION_TESTS3}
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

ADD_TEST(feTvLineRatio ${FEATUREEXTRACTION_TESTS3}
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

ADD_TEST(feTuAssociativeSymmetricalSumNew ${FEATUREEXTRACTION_TESTS4}
        otbAssociativeSymmetricalSumNew)

ADD_TEST(feTvAssociativeSymmetricalSum ${FEATUREEXTRACTION_TESTS4}
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreASS_amst_2_3.hdr
                          ${TEMP}/feFiltreASS_amst_2_3.hdr
        otbAssociativeSymmetricalSum
        ${BASELINE}/feFiltreLineRatioLinear_amst_2_3.hdr
        ${BASELINE}/feFiltreLineCorrelationLinear_amst_2_3.hdr
        ${TEMP}/feFiltreASS_amst_2_3.hdr)

# -------            otb::AssymmetricFusionOfLineDetector   ------------------------------

ADD_TEST(feTuAssymmetricFusionOfLineDetectorNew ${FEATUREEXTRACTION_TESTS4}
        otbAssymmetricFusionOfLineDetectorNew)

ADD_TEST(feTvAssymmetricFusionOfLineDetector ${FEATUREEXTRACTION_TESTS4}
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreAssymmetricFusion_amst_2_3.hdr
                          ${TEMP}/feFiltreAssymmetricFusion_amst_2_3.hdr
        otbAssymmetricFusionOfLineDetector
 ${INPUTDATA}/amst.png
 ${TEMP}/feFiltreAssymmetricFusion_amst_2_3.hdr
 2 3)

# -------            otb::HarrisImage   ------------------------------

ADD_TEST(feTvHarrisImage ${FEATUREEXTRACTION_TESTS4}
  --compare-image ${NOTOL}  ${BASELINE}/feHarrisImage.png
                          ${TEMP}/feHarrisImage.png
        otbHarrisImage
 ${INPUTDATA}/TeteAToto.png
 ${TEMP}/feHarrisImage.png
 1.0 2.0 0.0)

ADD_TEST(feTvHarrisPoupee ${FEATUREEXTRACTION_TESTS4}
  --compare-image ${NOTOL}  ${BASELINE}/feHarris_poupees.png
                          ${TEMP}/feHarris_poupees.png
        otbHarrisImage
 ${INPUTDATA}/poupees_1canal.c1.hdr
 ${TEMP}/feHarris_poupees.png
 1.0 1.0 1.0)

ADD_TEST(feTvHarrisImageQB ${FEATUREEXTRACTION_TESTS4}
  --compare-image ${NOTOL}  ${BASELINE}/feHarrisImage_QB.png
                          ${TEMP}/feHarrisImage_QB.png
        otbHarrisImage
 ${INPUTDATA}/QB_Suburb.png
 ${TEMP}/feHarrisImage_QB.png
 1.0 1.0 1.0)

ADD_TEST(feTvMultiplyByScalarImage ${FEATUREEXTRACTION_TESTS4}
         otbMultiplyByScalarImageFilterTest)

ADD_TEST(feTuThresholdImageToPointSetFilterNew ${FEATUREEXTRACTION_TESTS4}
         otbThresholdImageToPointSetFilterNew
)


ADD_TEST(feTuThresholdImageToPointSet1 ${FEATUREEXTRACTION_TESTS4}
  --compare-ascii ${EPSILON_3} ${BASELINE_FILES}/feThresholdImageToPointSet_Threshold_0To255.txt
                   ${TEMP}/feThresholdImageToPointSet_Threshold_0To255.txt
        otbThresholdImageToPointSetTest
 ${INPUTDATA}/small_points.hdr
 ${TEMP}/feThresholdImageToPointSet_Threshold_0To255.txt
 0 255 )
ADD_TEST(feTuThresholdImageToPointSet2 ${FEATUREEXTRACTION_TESTS4}
  --compare-ascii ${EPSILON_3} ${BASELINE_FILES}/feThresholdImageToPointSet_Threshold_0To0.txt
                   ${TEMP}/feThresholdImageToPointSet_Threshold_0To0.txt
        otbThresholdImageToPointSetTest
 ${INPUTDATA}/small_points.hdr
 ${TEMP}/feThresholdImageToPointSet_Threshold_0To0.txt
 0 0 )
ADD_TEST(feTuThresholdImageToPointSet3 ${FEATUREEXTRACTION_TESTS4}
  --compare-ascii ${EPSILON_3} ${BASELINE_FILES}/feThresholdImageToPointSet_Threshold_255To255.txt
                   ${TEMP}/feThresholdImageToPointSet_Threshold_255To255.txt
        otbThresholdImageToPointSetTest
 ${INPUTDATA}/small_points.hdr
 ${TEMP}/feThresholdImageToPointSet_Threshold_255To255.txt
 255 255 )
ADD_TEST(feTuThresholdImageToPointSet4 ${FEATUREEXTRACTION_TESTS4}
  --compare-ascii ${EPSILON_3} ${BASELINE_FILES}/feThresholdImageToPointSet_Threshold_1To200.txt
                   ${TEMP}/feThresholdImageToPointSet_Threshold_1To200.txt
        otbThresholdImageToPointSetTest
 ${INPUTDATA}/small_points.hdr
 ${TEMP}/feThresholdImageToPointSet_Threshold_1To200.txt
 1 200 )

ADD_TEST(feTvHarrisProduceImageToValidHarrisToPointSet ${FEATUREEXTRACTION_TESTS4}
        otbHarrisImage
 ${INPUTDATA}/small_points.hdr
 ${TEMP}/small_points_harris.hdr
 1.0 1.0 1.0)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests5 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ADD_TEST(feTvHarrisToPointSet1 ${FEATUREEXTRACTION_TESTS5}
  --compare-ascii ${EPSILON_3}        ${BASELINE_FILES}/feHarrisToPointSet_Threshold_2To255.txt
                                ${TEMP}/feHarrisToPointSet_Threshold_2To255.txt
        otbHarrisToPointSet
 ${INPUTDATA}/small_points.hdr
 ${TEMP}/feHarrisToPointSet_Threshold_2To255.txt
 1.0 1.0 1.0 2.0 255.0)
ADD_TEST(feTvHarrisToPointSet2 ${FEATUREEXTRACTION_TESTS5}
  --compare-ascii ${EPSILON_3}        ${BASELINE_FILES}/feHarrisToPointSet_Threshold_20To90.txt
                                ${TEMP}/feHarrisToPointSet_Threshold_20To90.txt
        otbHarrisToPointSet
 ${INPUTDATA}/small_points.hdr
 ${TEMP}/feHarrisToPointSet_Threshold_20To90.txt
 1.0 1.0 1.0 20 90)
ADD_TEST(feTvHarrisToPointSet3 ${FEATUREEXTRACTION_TESTS5}
  --compare-ascii ${EPSILON_3}        ${BASELINE_FILES}/feHarrisToPointSet_Threshold_100To160.txt
                                ${TEMP}/feHarrisToPointSet_Threshold_100To160.txt
        otbHarrisToPointSet
 ${INPUTDATA}/small_points.hdr
 ${TEMP}/feHarrisToPointSet_Threshold_100To160.txt
 1.0 1.0 1.0 100 160)

# -------            otb::PixelSuppressionByDirection   ------------------------------

ADD_TEST(feTuPixelSuppressionByDirectionNew ${FEATUREEXTRACTION_TESTS5}
        otbPixelSuppressionByDirectionNew)

ADD_TEST(feTvPixelSuppressionByDirection ${FEATUREEXTRACTION_TESTS5}
  --compare-image ${NOTOL}  ${BASELINE}/feFiltrePixelSuppr_ImageLine_2_0_3.hdr
                          ${TEMP}/feFiltrePixelSuppr_ImageLine_2_0_3.hdr
        otbPixelSuppressionByDirection
 ${INPUTDATA}/ImageLine_hd.bsq.hdr
 ${INPUTDATA}/ImageLineDir.bsq.hdr
 ${TEMP}/feFiltrePixelSuppr_ImageLine_2_0_3.hdr
 2 0.3)

# -------            otb::LocalHough   ------------------------------

ADD_TEST(feTuLocalHoughNew ${FEATUREEXTRACTION_TESTS5}
        otbLocalHoughNew)


ADD_TEST(feTvLocalHough ${FEATUREEXTRACTION_TESTS5}
  --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/feTvLocaLHoughLinesDetected.txt
                                ${TEMP}/feTvLocaLHoughLinesDetected.txt
        otbLocalHough
 ${INPUTDATA}/ImageLine.bsq.hdr
 ${TEMP}/feTvLocaLHoughLinesDetected.txt
 20 20 1)

ADD_TEST(feTvLocalHoughDraw ${FEATUREEXTRACTION_TESTS5}
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreLocalHoughDraw_ImageLine_20_20_2.hdr
                          ${TEMP}/feFiltreLocalHoughDraw_ImageLine_20_20_2.hdr
        otbLocalHoughDraw
 ${INPUTDATA}/ImageLine.bsq.hdr
 ${TEMP}/feFiltreLocalHoughDraw_ImageLine_20_20_2.hdr
 20 20 1)

ADD_TEST(feTvLocalHoughDraw2 ${FEATUREEXTRACTION_TESTS5}
  --compare-image ${EPSILON_8}  ${BASELINE}/feFilterLocalHoughDraw_amst_20_20_1.png
                          ${TEMP}/feFilterLocalHoughDraw_amst_20_20_1.png
        otbLocalHoughDraw
 ${INPUTDATA}/amst.png
 ${TEMP}/feFilterLocalHoughDraw_amst_20_20_1.png
 20 20 1)


ADD_TEST(feTvLocalHoughDrawBis ${FEATUREEXTRACTION_TESTS5}
  --compare-image ${EPSILON_8}  ${BASELINE}/feFiltreLocalHoughDraw_Line_20_10_2.png
                   ${TEMP}/feFiltreLocalHoughDraw_Line_20_10_2.png
        otbLocalHoughDraw
 ${INPUTDATA}/Line.png
 ${TEMP}/feFiltreLocalHoughDraw_Line_20_10_2.png
 20 10 2)

# -------            otb::FillGapsFilter   ------------------------------

ADD_TEST(feTuFillGapsFilterNew ${FEATUREEXTRACTION_TESTS5}
        otbFillGapsFilterNew)

ADD_TEST(feTvFillGapsFilter ${FEATUREEXTRACTION_TESTS5}
   --compare-image ${EPSILON_8}
        ${BASELINE}/feTvFillGapfilter.png
               ${TEMP}/feTvFillGapfilter.png
        otbFillGapsFilter
        ${INPUTDATA}/ImageLine.bsq.hdr
        ${TEMP}/feTvFillGapfilter.png)

# -------            otb::HoughTransform2DLinesImageTest ------------------------------

ADD_TEST(feTvHoughTransform2DLinesImage ${FEATUREEXTRACTION_TESTS5}
  --compare-image ${EPSILON_8}
    ${BASELINE}/feFilterHoughTransformDraw.tif
           ${TEMP}/feFilterHoughTransformDraw.tif
        otbHoughTransform2DLinesImageTest
 ${TEMP}/feFilterHoughTransformDraw.tif
 )


# -------            otb::ImageToModulusAndDirectionImageFilter   ------------------------------
# -------            otb::ModulusAndDirectionImageToImageFilter   ------------------------------

ADD_TEST(feTuModulusAndDirectionImageFiltersNew ${FEATUREEXTRACTION_TESTS5}
        otbModulusAndDirectionImageFiltersNew)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests6 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::ExtractSegments   ------------------------------

ADD_TEST(feTuExtractSegmentsNew ${FEATUREEXTRACTION_TESTS6}
        otbExtractSegmentsNew)

ADD_TEST(feTvExtractSegments ${FEATUREEXTRACTION_TESTS6}
  --compare-image ${NOTOL}  ${BASELINE}/feFiltreExtractSegments_ImageLine.hdr
                          ${TEMP}/feFiltreExtractSegments_ImageLine.hdr
        otbExtractSegments
 ${INPUTDATA}/ImageLine_hd.bsq.hdr
 ${INPUTDATA}/ImageLineDir.bsq.hdr
 ${TEMP}/feFiltreExtractSegments_ImageLine.hdr
 10  0.3 10 10 3 10 0.5)

# -------            otb::ForwardFourierMellinTransformImageFilter   -------------

ADD_TEST(feTuForwardFourierMellinImageFilterNew ${FEATUREEXTRACTION_TESTS6}
         otbFourierMellinImageFilterNew)

#ADD_TEST(feTuForwardFourierMellinImageFilterCroix ${FEATUREEXTRACTION_TESTS6}
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

#ADD_TEST(feTuForwardFourierMellinImageFilter3Croix ${FEATUREEXTRACTION_TESTS6}
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

ADD_TEST(feTvForwardFourierMellinImageFilter ${FEATUREEXTRACTION_TESTS6}
  --compare-n-images ${EPSILON_8} 2
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

ADD_TEST(feTuImageToEdgePathFilterNew ${FEATUREEXTRACTION_TESTS6}
         otbImageToEdgePathFilterNew)

ADD_TEST(feTvImageToEdgePathFilter ${FEATUREEXTRACTION_TESTS6}
  --compare-image ${EPSILON_8}
       ${BASELINE}/feImageToEdgePathFilter_rcc8_mire4.png
             ${TEMP}/feImageToEdgePathFilter_rcc8_mire4.png
        otbImageToEdgePathFilter
        ${INPUTDATA}/rcc8_mire4.png
        ${TEMP}/feImageToEdgePathFilter_rcc8_mire4.png
        255
        )
ADD_TEST(feTvImageToEdgePathFilterBis ${FEATUREEXTRACTION_TESTS6}
  --compare-image ${EPSILON_8}
       ${BASELINE}/feImageToEdgePathFilter_maskrom.png
             ${TEMP}/feImageToEdgePathFilter_maskrom.png
        otbImageToEdgePathFilter
        ${INPUTDATA}/maskrom.png
        ${TEMP}/feImageToEdgePathFilter_maskrom.png
        0
        )
# -------            otb::NeighborhoodScalarProductFilter   -------------


ADD_TEST(feTuNeighborhoodScalarProductFilterNew ${FEATUREEXTRACTION_TESTS6}
         otbNeighborhoodScalarProductFilterNew)

ADD_TEST(feTvNeighborhoodScalarProductFilter ${FEATUREEXTRACTION_TESTS6}
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

ADD_TEST(feTuRemoveIsolatedByDirectionFilterNew ${FEATUREEXTRACTION_TESTS7}
         otbRemoveIsolatedByDirectionFilterNew)

ADD_TEST(feTvRemoveIsolatedByDirectionFilter ${FEATUREEXTRACTION_TESTS7}
  --compare-image ${EPSILON_8}
        ${BASELINE}/feTvRemoveIsolatedByDirectionOutput.hdr
        ${TEMP}/feTvRemoveIsolatedByDirectionOutput.hdr
        otbRemoveIsolatedByDirectionFilter
       ${INPUTDATA}/InputForRoadDetectionScalarProductMod.hdr
       ${INPUTDATA}/InputForRoadDetectionScalarProductDir.hdr
       ${TEMP}/feTvRemoveIsolatedByDirectionOutput.hdr
        )

# -------            otb::RemoveWrongDirectionFilter   -------------

ADD_TEST(feTuRemoveWrongDirectionFilterNew ${FEATUREEXTRACTION_TESTS7}
         otbRemoveWrongDirectionFilterNew)

ADD_TEST(feTvRemoveWrongDirectionFilter ${FEATUREEXTRACTION_TESTS7}
  --compare-image ${EPSILON_8}
        ${BASELINE}/feTvRemoveWrongDirectionOutput.hdr
        ${TEMP}/feTvRemoveWrongDirectionOutput.hdr
        otbRemoveWrongDirectionFilter
       ${INPUTDATA}/InputForRoadDetection_IsolatedRem.hdr
       ${INPUTDATA}/InputForRoadDetectionScalarProductDir.hdr
       ${TEMP}/feTvRemoveWrongDirectionOutput.hdr
        )

# -------            otb::NonMaxRemovalByDirectionFilter   -------------

ADD_TEST(feTuNonMaxRemovalByDirectionFilterNew ${FEATUREEXTRACTION_TESTS7}
         otbNonMaxRemovalByDirectionFilterNew)

ADD_TEST(feTvNonMaxRemovalByDirectionFilter ${FEATUREEXTRACTION_TESTS7}
  --compare-image ${EPSILON_8}
        ${BASELINE}/feTvNonMaxRemovalByDirectionOutput.hdr
        ${TEMP}/feTvNonMaxRemovalByDirectionOutput.hdr
        otbNonMaxRemovalByDirectionFilter
       ${INPUTDATA}/InputForRoadDetection_WrongRem.hdr
       ${INPUTDATA}/InputForRoadDetectionScalarProductDir.hdr
       ${TEMP}/feTvNonMaxRemovalByDirectionOutput.hdr
        )

# -------            otb::VectorizationPathListFilter   -------------

ADD_TEST(feTuVectorizationPathListFilterNew ${FEATUREEXTRACTION_TESTS7}
         otbVectorizationPathListFilterNew)

ADD_TEST(feTvVectorizationPathListFilterAsciiOutput ${FEATUREEXTRACTION_TESTS7}
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

ADD_TEST(feTvVectorizationPathListFilterImageOutput ${FEATUREEXTRACTION_TESTS7}
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

ADD_TEST(feTuSimplifyPathListFilterNew ${FEATUREEXTRACTION_TESTS7}
         otbSimplifyPathListFilterNew)

ADD_TEST(feTvSimplifyPathListFilter ${FEATUREEXTRACTION_TESTS7}
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

ADD_TEST(feTuBreakAngularPathListFilterNew ${FEATUREEXTRACTION_TESTS7}
         otbBreakAngularPathListFilterNew)

ADD_TEST(feTvBreakAngularPathListFilter ${FEATUREEXTRACTION_TESTS7}
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

ADD_TEST(feTuRemoveTortuousPathListFilterNew ${FEATUREEXTRACTION_TESTS8}
         otbRemoveTortuousPathListFilterNew)

ADD_TEST(feTvRemoveTortuousPathListFilter ${FEATUREEXTRACTION_TESTS8}
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

ADD_TEST(feTuLinkPathListFilterNew ${FEATUREEXTRACTION_TESTS8}
         otbLinkPathListFilterNew)


ADD_TEST(feTvLinkPathListFilter ${FEATUREEXTRACTION_TESTS8}
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

ADD_TEST(feTuLikelihoodPathListFilterNew ${FEATUREEXTRACTION_TESTS8}
         otbLikelihoodPathListFilterNew)

ADD_TEST(feTvLikelihoodPathListFilter ${FEATUREEXTRACTION_TESTS8}
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

ADD_TEST(feTuParallelLinePathListFilterNew ${FEATUREEXTRACTION_TESTS8}
         otbParallelLinePathListFilterNew)

ADD_TEST(feTvParallelLinePathListFilter ${FEATUREEXTRACTION_TESTS8}
         otbParallelLinePathListFilter)


# -------            otb::RoadExtractionFilter   -------------

ADD_TEST(feTuRoadExtractionFilterNew ${FEATUREEXTRACTION_TESTS8}
         otbRoadExtractionFilterNew)

ADD_TEST(feTvRoadExtractionFilter ${FEATUREEXTRACTION_TESTS8}
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
ADD_TEST(feTuAddCarvingPathFilterNew ${FEATUREEXTRACTION_TESTS8}
         otbAddCarvingPathFilterNew)


# -------            otb::RemoveCarvingPathFilter   -------------
ADD_TEST(feTuRemoveCarvingPathFilterNew ${FEATUREEXTRACTION_TESTS8}
         otbRemoveCarvingPathFilterNew)

# -------            otb::ImageToCarvingPathFilter   -------------
ADD_TEST(feTuImageToCarvingPathFilterNew ${FEATUREEXTRACTION_TESTS8}
         otbImageToCarvingPathFilterNew)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTest9 ~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# -------            otb::ImageToSIFTKeyPointSetFilter   -------------

ADD_TEST(feTuImageToSIFTKeyPointSetFilterNew ${FEATUREEXTRACTION_TESTS9}
         otbImageToSIFTKeyPointSetFilterNew)


ADD_TEST(feTvImageToSIFTKeyPointSetFilterDistanceMap ${FEATUREEXTRACTION_TESTS9}
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

ADD_TEST(feTvImageToSIFTKeyPointSetFilterSceneOutputInterestPointAscii ${FEATUREEXTRACTION_TESTS9}
--ignore-order --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
         otbImageToSIFTKeyPointSetFilterOutputInterestPointAscii
  ${INPUTDATA}/scene.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
  6 3 0.08 10.0
)

ADD_TEST(feTvImageToSIFTKeyPointSetFilterSceneDescriptorAscii ${FEATUREEXTRACTION_TESTS9}
--ignore-order --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
         otbImageToSIFTKeyPointSetFilterOutputDescriptorAscii
  ${INPUTDATA}/scene.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
  6 3 0.08 10.0
)

ADD_TEST(feTvImageToSIFTKeyPointSetFilterSquareOutputImage ${FEATUREEXTRACTION_TESTS9}
--compare-image ${EPSILON_8}
  ${BASELINE}/feTvImageToSIFTKeyPointSetFilterSquareImageOutput.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareImageOutput.png
         otbImageToSIFTKeyPointSetFilterOutputImage
  ${INPUTDATA}/carre.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareImageOutput.png
  7 3 0.01 10.0
)

ADD_TEST(feTvImageToSIFTKeyPointSetFilterSquareOutputAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput.txt
         otbImageToSIFTKeyPointSetFilterOutputAscii
  ${INPUTDATA}/carre.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput.txt
  7 3 0.01 10.0
)

ADD_TEST(feTvImageToSIFTKeyPointSetFilterSquareRotatedOutputAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput.txt
         otbImageToSIFTKeyPointSetFilterOutputAscii
  ${INPUTDATA}/carre_ori.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput.txt
  7 3 0.01 10.0
)

ADD_TEST(feTvImageToSIFTKeyPointSetFilterGridOutputImage ${FEATUREEXTRACTION_TESTS9}
--compare-image ${EPSILON_8}
  ${BASELINE}/feTvImageToSIFTKeyPointSetFilterGridImageOutput.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterGridImageOutput.png
         otbImageToSIFTKeyPointSetFilterOutputImage
  ${INPUTDATA}/damier.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterGridImageOutput.png
  7 3 0.01 10.0
)

ADD_TEST(feTvImageToSIFTKeyPointSetFilterGridOutputAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterGridKeysOutput.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterGridKeysOutput.txt
         otbImageToSIFTKeyPointSetFilterOutputAscii
  ${INPUTDATA}/damier.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterGridKeysOutput.txt
  7 3 0.01 10.0
)

ADD_TEST(feTvImageToSIFTKeyPointSetFilterSquare2OutputAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput2.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput2.txt
         otbImageToSIFTKeyPointSetFilterOutputAscii
  ${INPUTDATA}/carre.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput2.txt
  1 3 0.2 0.9
)

ADD_TEST(feTvImageToSIFTKeyPointSetFilterSquareRotated2OutputAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput2.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput2.txt
         otbImageToSIFTKeyPointSetFilterOutputAscii
  ${INPUTDATA}/carre_ori.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput2.txt
  1 3 0.215 10.0
)

ADD_TEST(feTvImageToSIFTKeyPointSetFilterQB_SuburbOutputAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterQB_SuburbOutputAscii.txt
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterQB_SuburbOutputAscii.txt
         otbImageToSIFTKeyPointSetFilterOutputAscii
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/feTvImageToSIFTKeyPointSetFilterQB_SuburbOutputAscii.txt
  2 3 5.0 0.0
)


# -------            otb::HessianToHessianDeterminantImageFilter   -------------
ADD_TEST(feTuImageToHessianDeterminantImageFilterNew ${FEATUREEXTRACTION_TESTS9}
         otbImageToHessianDeterminantImageFilterNew)


ADD_TEST(feTvImageToHessianDeterminantImageFilter ${FEATUREEXTRACTION_TESTS9}
--compare-image ${EPSILON_3}
  ${BASELINE}/feTvImageToHessianDeterminantImageFilterOutput.tif
  ${TEMP}/feTvImageToHessianDeterminantImageFilterOutput.tif
         otbImageToHessianDeterminantImageFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
  ${TEMP}/feTvImageToHessianDeterminantImageFilterOutput.tif
  1.5
   )


# -------            otb::ImageFittingPolygonListFilter   -------------
ADD_TEST(feTuImageFittingPolygonListFilterNew ${FEATUREEXTRACTION_TESTS9}
         otbImageFittingPolygonListFilterNew)

ADD_TEST(feTvImageFittingPolygonListFilter ${FEATUREEXTRACTION_TESTS9}
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

ADD_TEST(feTvSimplePointCountStrategyTest ${FEATUREEXTRACTION_TESTS9}
    otbSimplePointCountStrategyTest)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests10 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::ImageToSURFKeyPointSetFilterNew   -------------

ADD_TEST(feTuImageToSURFKeyPointSetFilterNew ${FEATUREEXTRACTION_TESTS10}
         otbImageToSURFKeyPointSetFilterNew)


ADD_TEST(feTvImageToSURFKeyPointSetFilterSceneOutputInterestPointAscii ${FEATUREEXTRACTION_TESTS10}
--compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputInterestPoint.txt
  ${TEMP}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputInterestPoint.txt
         otbImageToSURFKeyPointSetFilterOutputInterestPointAscii
  ${INPUTDATA}/scene.png
  ${TEMP}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputInterestPoint.txt
  3 3
)

ADD_TEST(feTvImageToSURFKeyPointSetFilterSceneOutputDescriptorAscii ${FEATUREEXTRACTION_TESTS10}
--compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputDescriptor.txt
  ${TEMP}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputDescriptor.txt
         otbImageToSURFKeyPointSetFilterOutputDescriptorAscii
  ${INPUTDATA}/scene.png
  ${TEMP}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputDescriptor.txt
  3 3
)



# -------            otb::ImageToFastSIFTKeyPointSetFilter   -------------
IF(OTB_USE_SIFTFAST)

ADD_TEST(feTuImageToFastSIFTKeyPointSetFilterNew ${FEATUREEXTRACTION_TESTS10}
         otbImageToFastSIFTKeyPointSetFilterNew)

#Note the epsilon is according to relative difference
ADD_TEST(feTvImageToFastSIFTKeyPointSetFilterSceneOutputDescriptorAscii ${FEATUREEXTRACTION_TESTS10}
--ignore-order --epsilon-boundary 0.01 --compare-ascii ${EPSILON_2}
  ${BASELINE_FILES}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
  ${TEMP}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
         otbImageToFastSIFTKeyPointSetFilterOutputDescriptorAscii
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
  ${TEMP}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
  6
)

ADD_TEST(feTvImageToFastSIFTKeyPointSetFilterSceneOutputInterestPointAscii ${FEATUREEXTRACTION_TESTS10}
--ignore-order --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
  ${TEMP}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
         otbImageToFastSIFTKeyPointSetFilterOutputInterestPointAscii
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
  ${TEMP}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
  6
)

ENDIF(OTB_USE_SIFTFAST)


# --------- MatchingFilter ----------------
ADD_TEST(feTuKeyPointSetsMatchingFilterNew ${FEATUREEXTRACTION_TESTS10}
 otbKeyPointSetsMatchingFilterNew)

ADD_TEST(feTvKeyPointSetsMatchingFilter ${FEATUREEXTRACTION_TESTS10}
--compare-ascii ${EPSILON_3}
 ${BASELINE_FILES}/feTvKeyPointSetsMatchingFilterOutputAscii.txt
 ${TEMP}/feTvKeyPointSetsMatchingFilterOutputAscii.txt
    otbKeyPointSetsMatchingFilter
 ${TEMP}/feTvKeyPointSetsMatchingFilterOutputAscii.txt
 0.6 0
)

#--------- LandMark
ADD_TEST(feTuLandmarkNew ${FEATUREEXTRACTION_TESTS10}
 otbLandmarkNew)



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests11 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# -------            otb::CloudEstimatorFilterNew   -------------
ADD_TEST(feTuCloudEstimatorFilterNew ${FEATUREEXTRACTION_TESTS11}
    otbCloudEstimatorFilterNew)

ADD_TEST(feTvCloudEstimatorDefaultFilter ${FEATUREEXTRACTION_TESTS11}
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

ADD_TEST(feTvCloudEstimatorFilter ${FEATUREEXTRACTION_TESTS11}
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

ADD_TEST(feTuCloudDetectionFilterNew ${FEATUREEXTRACTION_TESTS11}
    otbCloudDetectionFilterNew)

ADD_TEST(feTvCloudDetectionFilter ${FEATUREEXTRACTION_TESTS11}
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


ADD_TEST(feTpSimplifyManyPathListFilter ${FEATUREEXTRACTION_TESTS11}
        otbSimplifyManyPathListFilter
 ${TEMP}/feTvSimplifyManyPathOutput.txt
         1.0
        )


# -------            otb::LineSegmentDetector   -------------
ADD_TEST(feTuLineSegmentDetectorNew ${FEATUREEXTRACTION_TESTS11}
    otbLineSegmentDetectorNew)

ADD_TEST(feTvLineSegmentDetector ${FEATUREEXTRACTION_TESTS11}
--compare-ogr  ${EPSILON_8}
            ${BASELINE_FILES}/feTvLineSegmentDetectorOutput.shp
            ${TEMP}/feTvLineSegmentDetectorOutput.shp
    otbLineSegmentDetector
     ${INPUTDATA}/scene.png
     ${TEMP}/feTvLineSegmentDetectorOutput.shp
)

# -------            otb::LineSpatialObjectListToRightAnglePointSetFilter   -------------
ADD_TEST(feTuLineSpatialObjectListToRightAnglePointSetFilterNew ${FEATUREEXTRACTION_TESTS11}
    otbLineSpatialObjectListToRightAnglePointSetFilterNew)

ADD_TEST(feTvLineSpatialObjectListToRightAnglePointSetFilter ${FEATUREEXTRACTION_TESTS11}
 --compare-ascii ${EPSILON_3}
 ${BASELINE_FILES}/feTvLineSpatialObjectListToRightAnglePointSetFilterOutputAscii.txt
 ${TEMP}/feTvLineSpatialObjectListToRightAnglePointSetFilterOutputAscii.txt
    otbLineSpatialObjectListToRightAnglePointSetFilter
    ${INPUTDATA}/carre.png
    ${TEMP}/feTvLineSpatialObjectListToRightAnglePointSetFilterOutputAscii.txt
)

ADD_TEST(feTvLineSpatialObjectListToRightAnglePointSetFilterBySteps ${FEATUREEXTRACTION_TESTS11}
 --compare-ascii ${EPSILON_3}
 ${BASELINE_FILES}/feTvLineSpatialObjectListToRightAnglePointSetFilterOutputAscii.txt
 ${TEMP}/feTvLineSpatialObjectListToRightAnglePointSetFilterByStepsOutputAscii.txt
    otbLineSpatialObjectListToRightAnglePointSetFilterByStepsOutputAscii
    ${INPUTDATA}/carre.png
    ${TEMP}/feTvLineSpatialObjectListToRightAnglePointSetFilterByStepsOutputAscii.txt
)

#ADD_TEST(feTvLineSpatialObjectListToRightAnglePointSetFilterOutputImage ${FEATUREEXTRACTION_TESTS11}
# --compare-ascii ${EPSILON_3}
# ${BASELINE_FILES}/feTvLineSpatialObjectListToRightAnglePointSetFilterByStepsOutputAscii.txt
# ${TEMP}/feTvLineSpatialObjectListToRightAnglePointSetFilterByStepsOutputAscii.txt
#    otbLineSpatialObjectListToRightAnglePointSetFilterOutputImage
#    ${INPUTDATA}/prison_toulouse.tif
#    ${TEMP}/feTvLineSpatialObjectListToRightAnglePointSetFilterByStepsOutputAscii.txt
#    ${TEMP}/feTvLineSpatialObjectListToRightAnglePointSetFilterOutputImage.tif
#)

# -------            otb::VectorDataToRightAnglePointSetFilter   -------------
ADD_TEST(feTuVectorDataToRightAngleVectorDataFilterNew ${FEATUREEXTRACTION_TESTS11}
    otbVectorDataToRightAngleVectorDataFilterNew)

ADD_TEST(feTvVectorDataToRightAngleVectorDataFilter ${FEATUREEXTRACTION_TESTS11}
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
ADD_TEST(feTuSFSTexturesImageFilterNew ${FEATUREEXTRACTION_TESTS14}
    otbSFSTexturesImageFilterNew)

ADD_TEST(feTvSFSTexturesImageFilterTest ${FEATUREEXTRACTION_TESTS14}
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
ADD_TEST(raTuUrbanAreaDetectionImageFilterNew ${FEATUREEXTRACTION_TESTS14}
        otbUrbanAreaDetectionImageFilterNew
)

ADD_TEST(raTvUrbanAreaDetectionImageFilter ${FEATUREEXTRACTION_TESTS14}
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
ADD_TEST(feTuRegionImageToRectangularPathListFilterNew ${FEATUREEXTRACTION_TESTS14}
         otbRegionImageToRectangularPathListFilterNew)

ADD_TEST(feTvRegionImageToRectangularPathListFilter ${FEATUREEXTRACTION_TESTS14}
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
ADD_TEST(feTuStreamingLineSegmentDetectorNew ${FEATUREEXTRACTION_TESTS15}
    otbStreamingLineSegmentDetectorNew)

ADD_TEST(feTvStreamingLineSegmentDetector10 ${FEATUREEXTRACTION_TESTS15}
--compare-ogr  ${EPSILON_8}
     ${BASELINE_FILES}/feTvStreamingLineSegmentDetectorOutput10.shp
     ${TEMP}/feTvStreamingLineSegmentDetectorOutput10.shp
    otbStreamingLineSegmentDetector
      ${INPUTDATA}/scene.png
      ${TEMP}/feTvStreamingLineSegmentDetectorOutput10.shp
      10
)

ADD_TEST(feTvStreamingLineSegmentDetector1000 ${FEATUREEXTRACTION_TESTS15}
--compare-ogr  ${EPSILON_8}
     ${BASELINE_FILES}/feTvStreamingLineSegmentDetectorOutput1000.shp
     ${TEMP}/feTvStreamingLineSegmentDetectorOutput1000.shp
    otbStreamingLineSegmentDetector
      ${INPUTDATA}/scene.png
      ${TEMP}/feTvStreamingLineSegmentDetectorOutput1000.shp
      1000
)

# -------            otb::SqrtSpectralAngleImageFilter (Ossman)------------------------------
ADD_TEST(feTvSqrtSpectralAngleImageFilter ${FEATUREEXTRACTION_TESTS15}
 --compare-image ${EPSILON_8}   ${BASELINE}/feSqrtSpectralAngle.tif
                   ${TEMP}/feSqrtSpectralAngle.tif
        otbSqrtSpectralAngleImageFilter
        ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
        ${TEMP}/feSqrtSpectralAngle.tif
        185 270 150 619 #reference pixel
)

# -------            otb::ScalarImageToTexturesFilter (Ossman)------------------------------
ADD_TEST(feTuScalarImageToTexturesFilterNew ${FEATUREEXTRACTION_TESTS15}
        otbScalarImageToTexturesFilterNew
)

ADD_TEST(feTuMaskedScalarImageToGreyLevelRunLengthMatrixGeneratorNew ${FEATUREEXTRACTION_TESTS15}
        otbMaskedScalarImageToGreyLevelRunLengthMatrixGeneratorNew
)

ADD_TEST(feTvScalarImageToTexturesFilter ${FEATUREEXTRACTION_TESTS15}
        --compare-n-images ${EPSILON_8} 8
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

ADD_TEST(feTuotbGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorNew ${FEATUREEXTRACTION_TESTS15}
         otbGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorNew
)

ADD_TEST(feTuotbGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator ${FEATUREEXTRACTION_TESTS15}
         otbGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator
)

ADD_TEST(feTuScalarImageToAdvancedTexturesFilterNew ${FEATUREEXTRACTION_TESTS15}
        otbScalarImageToAdvancedTexturesFilterNew
)

ADD_TEST(feTvScalarImageToAdvancedTexturesFilter ${FEATUREEXTRACTION_TESTS15}
        --compare-n-images ${EPSILON_8} 9
            ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputVariance.tif
            ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputVariance.tif
            ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputMean.tif
            ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputMean.tif
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


ADD_TEST(feTuScalarImageToHigherOrderTexturesFilterNew ${FEATUREEXTRACTION_TESTS15}
        otbScalarImageToHigherOrderTexturesFilterNew
)

ADD_TEST(feTvScalarImageToHigherOrderTexturesFilter ${FEATUREEXTRACTION_TESTS15}
        --compare-n-images ${EPSILON_8} 11
            ${BASELINE}/feTvScalarImageToHigherOrderTexturesFilterOutputShortRunEmphasis.tif
            ${TEMP}/feTvScalarImageToHigherOrderTexturesFilterOutputShortRunEmphasis.tif
            ${BASELINE}/feTvScalarImageToHigherOrderTexturesFilterOutputLongRunEmphasis.tif
            ${TEMP}/feTvScalarImageToHigherOrderTexturesFilterOutputLongRunEmphasis.tif
            ${BASELINE}/feTvScalarImageToHigherOrderTexturesFilterOutputGreyLevelNonuniformity.tif
            ${TEMP}/feTvScalarImageToHigherOrderTexturesFilterOutputGreyLevelNonuniformity.tif
            ${BASELINE}/feTvScalarImageToHigherOrderTexturesFilterOutputRunLengthNonuniformity.tif
            ${TEMP}/feTvScalarImageToHigherOrderTexturesFilterOutputRunLengthNonuniformity.tif
            ${BASELINE}/feTvScalarImageToHigherOrderTexturesFilterOutputRunPercentage.tif
            ${TEMP}/feTvScalarImageToHigherOrderTexturesFilterOutputRunPercentage.tif
            ${BASELINE}/feTvScalarImageToHigherOrderTexturesFilterOutputLowGreyLevelRunEmphasis.tif
            ${TEMP}/feTvScalarImageToHigherOrderTexturesFilterOutputLowGreyLevelRunEmphasis.tif
            ${BASELINE}/feTvScalarImageToHigherOrderTexturesFilterOutputHighGreyLevelRunEmphasis.tif
            ${TEMP}/feTvScalarImageToHigherOrderTexturesFilterOutputHighGreyLevelRunEmphasis.tif
            ${BASELINE}/feTvScalarImageToHigherOrderTexturesFilterOutputShortRunLowGreyLevelEmphasis.tif
            ${TEMP}/feTvScalarImageToHigherOrderTexturesFilterOutputShortRunLowGreyLevelEmphasis.tif
            ${BASELINE}/feTvScalarImageToHigherOrderTexturesFilterOutputShortRunHighGreyLevelEmphasis.tif
            ${TEMP}/feTvScalarImageToHigherOrderTexturesFilterOutputShortRunHighGreyLevelEmphasis.tif
            ${BASELINE}/feTvScalarImageToHigherOrderTexturesFilterOutputLongRunLowGreyLevelEmphasis.tif
            ${TEMP}/feTvScalarImageToHigherOrderTexturesFilterOutputLongRunLowGreyLevelEmphasis.tif
            ${BASELINE}/feTvScalarImageToHigherOrderTexturesFilterOutputLongRunHighGreyLevelEmphasis.tif
            ${TEMP}/feTvScalarImageToHigherOrderTexturesFilterOutputLongRunHighGreyLevelEmphasis.tif
        otbScalarImageToHigherOrderTexturesFilter
        ${INPUTDATA}/Mire_Cosinus.png
        ${TEMP}/feTvScalarImageToHigherOrderTexturesFilterOutput
        8 5 1 1)

ADD_TEST(feTuScalarImageToPanTexTextureFilterNew ${FEATUREEXTRACTION_TESTS15}
        otbScalarImageToPanTexTextureFilterNew
)

ADD_TEST(feTvScalarImageToPanTexTextureFilter ${FEATUREEXTRACTION_TESTS15}
        --compare-image ${EPSILON_8}
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

ADD_TEST(feTuFourierMellinDescriptorsNew ${FEATUREEXTRACTION_TESTS16}
  otbFourierMellinDescriptorsNew
  )

ADD_TEST(feTvFourierMellinDescriptors ${FEATUREEXTRACTION_TESTS16}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/feTvFourierMellinDescriptors.txt
  ${TEMP}/feTvFourierMellinDescriptors.txt
  otbFourierMellinDescriptors
  ${INPUTDATA}/poupees.png
  4
  4
  ${TEMP}/feTvFourierMellinDescriptors.txt
  )

ADD_TEST(feTvFourierMellinDescriptorsScaleInvariant ${FEATUREEXTRACTION_TESTS16}
  otbFourierMellinDescriptorsScaleInvariant
  ${INPUTDATA}/poupees.png
  4
  4
  )

ADD_TEST(feTvFourierMellinDescriptorsRotationInvariant ${FEATUREEXTRACTION_TESTS16}
  otbFourierMellinDescriptorsRotationInvariant
  ${INPUTDATA}/poupees.png
  4
  4
  90
  )

# -------            otb::LocalHistogramImageFunction   -------------

ADD_TEST(feTuLocalHistogramImageFunctionNew ${FEATUREEXTRACTION_TESTS16}
        otbLocalHistogramImageFunctionNew
)

ADD_TEST(feTvLocalHistogramImageFunctionTest ${FEATUREEXTRACTION_TESTS16}
--compare-ascii ${EPSILON_8}
 ${BASELINE_FILES}/feLocalHistogramImage.txt
    ${TEMP}/feLocalHistogramImage.txt
 otbLocalHistogramImageFunctionTest
 ${INPUTDATA}/pyramide.tif
 ${TEMP}/feLocalHistogramImage.txt
 127 127 127 0 128 128
)

# -------   otb::ImageFunctionAdapter   -------------

ADD_TEST(feTuImageFunctionAdaptorNew ${FEATUREEXTRACTION_TESTS16}
        otbImageFunctionAdaptorNew
)

ADD_TEST(feTvImageFunctionAdaptor ${FEATUREEXTRACTION_TESTS16}
        otbImageFunctionAdaptor
                ${INPUTDATA}/poupees.png
         )

# -------   otb::MetaImageFunction   -------------
ADD_TEST(feTuMetaImageFunctionNew ${FEATUREEXTRACTION_TESTS16}
        otbMetaImageFunctionNew
)

ADD_TEST(feTvMetaImageFunction ${FEATUREEXTRACTION_TESTS16}
--compare-ascii ${EPSILON_8}
    ${BASELINE_FILES}/feTvMetaImageFunction.txt
    ${TEMP}/feTvMetaImageFunction.txt
    otbMetaImageFunction
 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
    ${TEMP}/feTvMetaImageFunction.txt
    451846.014047961 5412466.57452216
)

# -------   otb::HaralickTexturesImageFunction   -------------
ADD_TEST(feTuHaralickTexturesImageFunctionNew ${FEATUREEXTRACTION_TESTS16}
        otbHaralickTexturesImageFunctionNew
)

ADD_TEST(feTvHaralickTexturesImageFunction ${FEATUREEXTRACTION_TESTS16}
--compare-ascii ${EPSILON_8}
    ${BASELINE_FILES}/feTvHaralickTexturesImageFunction.txt
    ${TEMP}/feTvHaralickTexturesImageFunction.txt
    otbHaralickTexturesImageFunction
 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
    ${TEMP}/feTvHaralickTexturesImageFunction.txt
    451846.014047961 5412466.57452216
)

# ------ otb::HistogramOfOrientedGradientCovariantImageFunction --------
ADD_TEST(feTuHistogramOfOrientedGradientCovariantImageFunction ${FEATUREEXTRACTION_TESTS16}
         otbHistogramOfOrientedGradientCovariantImageFunctionNew)

ADD_TEST(feTvHistogramOfOrientedGradientCovariantImageFunction ${FEATUREEXTRACTION_TESTS16}
--compare-ascii ${EPSILON_8}
  ${BASELINE_FILES}/feTvHistogramOfOrientedGradientCovariantImageFunction.txt
  ${TEMP}/feTvHistogramOfOrientedGradientCovariantImageFunction.txt
         otbHistogramOfOrientedGradientCovariantImageFunction
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/feTvHistogramOfOrientedGradientCovariantImageFunction.txt
  5 273 64
)

# ------ otb::RadiometricMomentsImageFilter --------
ADD_TEST(feTuRadiometricMomentsImageFilterNew ${FEATUREEXTRACTION_TESTS16}
         otbRadiometricMomentsImageFilterNew)
         
ADD_TEST(feTvRadiometricMomentsImageFilter ${FEATUREEXTRACTION_TESTS16}
         --compare-image ${EPSILON_8}  
			${BASELINE}/feTvRadiometricMomentsImageFilter.tif
            ${TEMP}/feTvRadiometricMomentsImageFilter.tif
        otbRadiometricMomentsImageFilter
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/feTvRadiometricMomentsImageFilter.tif
        3 #radius
        )

# A enrichir
SET(BasicFeatureExtraction_SRCS1
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

IF(OTB_USE_DEPRECATED)
SET(BasicFeatureExtraction_SRCS1
${BasicFeatureExtraction_SRCS1}
)
ENDIF(OTB_USE_DEPRECATED)

SET(BasicFeatureExtraction_SRCS2
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
SET(BasicFeatureExtraction_SRCS3
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
SET(BasicFeatureExtraction_SRCS4
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
SET(BasicFeatureExtraction_SRCS5
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
SET(BasicFeatureExtraction_SRCS6
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
SET(BasicFeatureExtraction_SRCS7
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
SET(BasicFeatureExtraction_SRCS8
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
SET(BasicFeatureExtraction_SRCS9
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
SET(BasicFeatureExtraction_SRCS10
otbFeatureExtractionTests10.cxx
otbImageToSURFKeyPointSetFilterNew.cxx
otbImageToSURFKeyPointSetFilterOutputInterestPointAscii.cxx
otbImageToSURFKeyPointSetFilterOutputDescriptorAscii.cxx
otbKeyPointSetsMatchingFilterNew.cxx
otbKeyPointSetsMatchingFilter.cxx
otbLandmarkNew.cxx
)
IF(OTB_USE_SIFTFAST)
SET(BasicFeatureExtraction_SRCS10
      ${BasicFeatureExtraction_SRCS10}
      otbImageToFastSIFTKeyPointSetFilterNew.cxx
      otbImageToFastSIFTKeyPointSetFilterOutputDescriptorAscii.cxx
      otbImageToFastSIFTKeyPointSetFilterOutputInterestPointAscii.cxx )
ENDIF(OTB_USE_SIFTFAST)

SET(BasicFeatureExtraction_SRCS11
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


SET(BasicFeatureExtraction_SRCS14
otbFeatureExtractionTests14.cxx
otbSFSTexturesImageFilterNew.cxx
otbSFSTexturesImageFilterTest.cxx
otbUrbanAreaDetectionImageFilterNew.cxx
otbUrbanAreaDetectionImageFilter.cxx
otbRegionImageToRectangularPathListFilterNew.cxx
otbRegionImageToRectangularPathListFilter.cxx
)

SET(BasicFeatureExtraction_SRCS15
otbFeatureExtractionTests15.cxx
otbStreamingLineSegmentDetector.cxx
otbSqrtSpectralAngleImageFilter.cxx
otbMaskedScalarImageToGreyLevelRunLengthMatrixGeneratorNew.cxx
otbScalarImageToTexturesFilterNew.cxx
otbScalarImageToTexturesFilter.cxx
otbGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculatorNew.cxx
otbGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator.cxx
otbScalarImageToAdvancedTexturesFilterNew.cxx
otbScalarImageToAdvancedTexturesFilter.cxx
otbScalarImageToPanTexTextureFilterNew.cxx
otbScalarImageToPanTexTextureFilter.cxx
otbScalarImageToHigherOrderTexturesFilterNew.cxx
otbScalarImageToHigherOrderTexturesFilter.cxx
)

SET(BasicFeatureExtraction_SRCS16
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

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
