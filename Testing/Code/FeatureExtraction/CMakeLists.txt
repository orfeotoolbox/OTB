
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance de base
SET(TOL 0.0)

#Tolerance sur la difference des valeurs numeriques dans le --compare-ascii
SET(EPS 0.001)

#Tolerance sur diff pixel image
#EPSILON est different de 0.0 pour les tests multiplateformes avec différentes options de compilation.
SET(EPSILON 0.00000001)

SET(FEATUREEXTRACTION_TESTS1 ${CXX_TEST_PATH}/otbFeatureExtractionTests1)
SET(FEATUREEXTRACTION_TESTS2 ${CXX_TEST_PATH}/otbFeatureExtractionTests2)
SET(FEATUREEXTRACTION_TESTS3 ${CXX_TEST_PATH}/otbFeatureExtractionTests3)
SET(FEATUREEXTRACTION_TESTS4 ${CXX_TEST_PATH}/otbFeatureExtractionTests4)
SET(FEATUREEXTRACTION_TESTS5 ${CXX_TEST_PATH}/otbFeatureExtractionTests5)
SET(FEATUREEXTRACTION_TESTS6 ${CXX_TEST_PATH}/otbFeatureExtractionTests6)
SET(FEATUREEXTRACTION_TESTS7 ${CXX_TEST_PATH}/otbFeatureExtractionTests7)
SET(FEATUREEXTRACTION_TESTS8 ${CXX_TEST_PATH}/otbFeatureExtractionTests8)
SET(FEATUREEXTRACTION_TESTS9 ${CXX_TEST_PATH}/otbFeatureExtractionTests9)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests1 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 

ADD_TEST(feTvAlignMV2ITK ${FEATUREEXTRACTION_TESTS1}  
   --compare-ascii ${EPS}     ${BASELINE_FILES}/feAlign.txt
                        ${TEMP}/feAlign.txt
        otbAlignImageToPath 
	${INPUTDATA}/poupees.hdr
	${TEMP}/feAlign.txt)

# -------            otb::Draw   ------------------------------
		
ADD_TEST(feTvDrawPathTestCarre ${FEATUREEXTRACTION_TESTS1}  
  --compare-image ${TOL}  ${BASELINE}/feDrawPathDessinCarre.png
                          ${TEMP}/feDrawPathDessinCarre.png
        otbDrawPathDessinCarre
	${INPUTDATA}/DeuxTraits.png
	${TEMP}/feDrawPathDessinCarre.png)

ADD_TEST(feTvDrawPathTestAlign ${FEATUREEXTRACTION_TESTS1}  
  --compare-image ${TOL}  ${BASELINE}/feDrawPathAlignDeuxTraits.png
                          ${TEMP}/feDrawPathAlignDeuxTraits.png
        otbDrawPathAlign
	${INPUTDATA}/DeuxTraits.png
	${TEMP}/feDrawPathAlignDeuxTraits.png)
	
ADD_TEST(feTuComplexMomentImage ${FEATUREEXTRACTION_TESTS1}  
        otbComplexMomentImage
	${INPUTDATA}/TeteAToto.png 1 1)

ADD_TEST(feTuHuImage ${FEATUREEXTRACTION_TESTS1}  
        otbHuImage
	${INPUTDATA}/TeteAToto.png)

ADD_TEST(feTuFlusserImage ${FEATUREEXTRACTION_TESTS1}  
        otbFlusserImage
	${INPUTDATA}/TeteAToto.png)
	
ADD_TEST(feTuComplexMomentPathNew ${FEATUREEXTRACTION_TESTS1}  
        otbComplexMomentPathNew)
	
ADD_TEST(feTuComplexMomentPath ${FEATUREEXTRACTION_TESTS1}  
        otbComplexMomentPath 1 1)

ADD_TEST(feTuComplexMomentPathFloat ${FEATUREEXTRACTION_TESTS1}  
        otbComplexMomentPath 1 1)

ADD_TEST(feTuHuPathNew ${FEATUREEXTRACTION_TESTS1}  
        otbHuPathNew)

ADD_TEST(feTuHuPath ${FEATUREEXTRACTION_TESTS1}  
        otbHuPath)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbFeatureExtractionTests2 ~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 

ADD_TEST(feTuFlusserPathNew ${FEATUREEXTRACTION_TESTS2}  
        otbFlusserPathNew)

ADD_TEST(feTuFlusserPath ${FEATUREEXTRACTION_TESTS2}  
        otbFlusserPath)
	
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
  --compare-image ${EPSILON}  ${BASELINE}/feFiltreTouzi_amst_3.hdr
                          ${TEMP}/feFiltreTouzi_amst_3.hdr
        otbTouziEdgeDetector
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreTouzi_amst_3.hdr
	3)
	
ADD_TEST(feTvTouziDir ${FEATUREEXTRACTION_TESTS3}
  --compare-image ${EPSILON}  ${BASELINE}/feFiltreTouzi_amst_dir_3.hdr
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
  --compare-n-images ${EPSILON} 2
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
  --compare-image ${EPSILON}  ${BASELINE}/feFiltreLineCorrelation_amst_2_3.hdr
                          ${TEMP}/feFiltreLineCorrelation_amst_2_3.hdr
        otbLineCorrelationDetector
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreLineCorrelation_amst_2_3.hdr
	2 3)
	
# -------            otb::LineRatioDetector   ------------------------------

ADD_TEST(feTuLineRatioNew ${FEATUREEXTRACTION_TESTS3}  
        otbLineRatioDetectorNew)
        
ADD_TEST(feTvLineRatioLinear ${FEATUREEXTRACTION_TESTS3}  
  --compare-n-images ${EPSILON} 2
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
  --compare-n-images ${EPSILON} 2
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
  --compare-image ${EPSILON}  ${BASELINE}/feFiltreLineRatio_amst_2_3.hdr
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
  --compare-image ${EPSILON}  ${BASELINE}/feFiltreASS_amst_2_3.hdr
                          ${TEMP}/feFiltreASS_amst_2_3.hdr
        otbAssociativeSymmetricalSum
        ${BASELINE}/feFiltreLineRatioLinear_amst_2_3.hdr
        ${BASELINE}/feFiltreLineCorrelationLinear_amst_2_3.hdr
        ${TEMP}/feFiltreASS_amst_2_3.hdr)

# -------            otb::AssymmetricFusionOfLineDetector   ------------------------------
        
ADD_TEST(feTuAssymmetricFusionOfLineDetectorNew ${FEATUREEXTRACTION_TESTS4}  
        otbAssymmetricFusionOfLineDetectorNew)
       
ADD_TEST(feTvAssymmetricFusionOfLineDetector ${FEATUREEXTRACTION_TESTS4}  
  --compare-image ${EPSILON}  ${BASELINE}/feFiltreAssymmetricFusion_amst_2_3.hdr
                          ${TEMP}/feFiltreAssymmetricFusion_amst_2_3.hdr
        otbAssymmetricFusionOfLineDetector
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreAssymmetricFusion_amst_2_3.hdr
	2 3)

# -------            otb::HarrisImage   ------------------------------
        
ADD_TEST(feTvHarrisImage ${FEATUREEXTRACTION_TESTS4}  
  --compare-image ${TOL}  ${BASELINE}/feHarrisImage.png
                          ${TEMP}/feHarrisImage.png
        otbHarrisImage 
	${INPUTDATA}/TeteAToto.png
	${TEMP}/feHarrisImage.png
	1.0 2.0 0.0)

ADD_TEST(feTvHarrisPoupee ${FEATUREEXTRACTION_TESTS4}  
  --compare-image ${TOL}  ${BASELINE}/feHarris_poupees.png
                          ${TEMP}/feHarris_poupees.png
        otbHarrisImage 
	${INPUTDATA}/poupees_1canal.c1.hdr
	${TEMP}/feHarris_poupees.png
	1.0 1.0 1.0)

ADD_TEST(feTvMultiplyByScalarImage ${FEATUREEXTRACTION_TESTS4}  
         otbMultiplyByScalarImageFilterTest)

ADD_TEST(feTuThresholdImageToPointSetFilterNew ${FEATUREEXTRACTION_TESTS4}
         otbThresholdImageToPointSetFilterNew
)


ADD_TEST(feTuThresholdImageToPointSet1 ${FEATUREEXTRACTION_TESTS4}  
  --compare-ascii ${EPS} ${BASELINE_FILES}/feThresholdImageToPointSet_Threshold_0To255.txt
                   ${TEMP}/feThresholdImageToPointSet_Threshold_0To255.txt
        otbThresholdImageToPointSetTest 
	${INPUTDATA}/small_points.hdr
	${TEMP}/feThresholdImageToPointSet_Threshold_0To255.txt
	0 255 )
ADD_TEST(feTuThresholdImageToPointSet2 ${FEATUREEXTRACTION_TESTS4}  
  --compare-ascii ${EPS} ${BASELINE_FILES}/feThresholdImageToPointSet_Threshold_0To0.txt
                   ${TEMP}/feThresholdImageToPointSet_Threshold_0To0.txt
        otbThresholdImageToPointSetTest 
	${INPUTDATA}/small_points.hdr
	${TEMP}/feThresholdImageToPointSet_Threshold_0To0.txt
	0 0 )
ADD_TEST(feTuThresholdImageToPointSet3 ${FEATUREEXTRACTION_TESTS4}  
  --compare-ascii ${EPS} ${BASELINE_FILES}/feThresholdImageToPointSet_Threshold_255To255.txt
                   ${TEMP}/feThresholdImageToPointSet_Threshold_255To255.txt
        otbThresholdImageToPointSetTest 
	${INPUTDATA}/small_points.hdr
	${TEMP}/feThresholdImageToPointSet_Threshold_255To255.txt
	255 255 )
ADD_TEST(feTuThresholdImageToPointSet4 ${FEATUREEXTRACTION_TESTS4}  
  --compare-ascii ${EPS} ${BASELINE_FILES}/feThresholdImageToPointSet_Threshold_1To200.txt
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
  --compare-ascii ${EPS}        ${BASELINE_FILES}/feHarrisToPointSet_Threshold_2To255.txt
                                ${TEMP}/feHarrisToPointSet_Threshold_2To255.txt
        otbHarrisToPointSet 
	${INPUTDATA}/small_points.hdr
	${TEMP}/feHarrisToPointSet_Threshold_2To255.txt
	1.0 1.0 1.0 2.0 255.0)
ADD_TEST(feTvHarrisToPointSet2 ${FEATUREEXTRACTION_TESTS5}  
  --compare-ascii ${EPS}        ${BASELINE_FILES}/feHarrisToPointSet_Threshold_20To90.txt
                                ${TEMP}/feHarrisToPointSet_Threshold_20To90.txt
        otbHarrisToPointSet 
	${INPUTDATA}/small_points.hdr
	${TEMP}/feHarrisToPointSet_Threshold_20To90.txt
	1.0 1.0 1.0 20 90)
ADD_TEST(feTvHarrisToPointSet3 ${FEATUREEXTRACTION_TESTS5}  
  --compare-ascii ${EPS}        ${BASELINE_FILES}/feHarrisToPointSet_Threshold_100To160.txt
                                ${TEMP}/feHarrisToPointSet_Threshold_100To160.txt
        otbHarrisToPointSet 
	${INPUTDATA}/small_points.hdr
	${TEMP}/feHarrisToPointSet_Threshold_100To160.txt
	1.0 1.0 1.0 100 160)

# -------            otb::PixelSuppressionByDirection   ------------------------------
	
ADD_TEST(feTuPixelSuppressionByDirectionNew ${FEATUREEXTRACTION_TESTS5}  
        otbPixelSuppressionByDirectionNew)
        
ADD_TEST(feTvPixelSuppressionByDirection ${FEATUREEXTRACTION_TESTS5}
  --compare-image ${TOL}  ${BASELINE}/feFiltrePixelSuppr_ImageLine_2_0_3.hdr
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
  --compare-ascii ${EPS}  ${BASELINE_FILES}/feTvLocaLHoughLinesDetected.txt
                                ${TEMP}/feTvLocaLHoughLinesDetected.txt
        otbLocalHough
	${INPUTDATA}/ImageLine.bsq.hdr
	${TEMP}/feTvLocaLHoughLinesDetected.txt
	20 20 1)  

ADD_TEST(feTvLocalHoughDraw ${FEATUREEXTRACTION_TESTS5}  
  --compare-image ${EPSILON}  ${BASELINE}/feFiltreLocalHoughDraw_ImageLine_20_20_2.hdr
                          ${TEMP}/feFiltreLocalHoughDraw_ImageLine_20_20_2.hdr
        otbLocalHoughDraw
	${INPUTDATA}/ImageLine.bsq.hdr
	${TEMP}/feFiltreLocalHoughDraw_ImageLine_20_20_2.hdr
	20 20 1)        

ADD_TEST(feTvLocalHoughDraw2 ${FEATUREEXTRACTION_TESTS5}  
  --compare-image ${EPSILON}  ${BASELINE}/feFilterLocalHoughDraw_amst_20_20_1.tif
                          ${TEMP}/feFilterLocalHoughDraw_amst_20_20_1.png
        otbLocalHoughDraw
	${INPUTDATA}/amst.png
	${TEMP}/feFilterLocalHoughDraw_amst_20_20_1.png
	20 20 1) 


ADD_TEST(feTvLocalHoughDrawBis ${FEATUREEXTRACTION_TESTS5}  
  --compare-image ${EPSILON}  ${BASELINE}/feFiltreLocalHoughDraw_Line_20_10_2.png
                   ${TEMP}/feFiltreLocalHoughDraw_Line_20_10_2.png
        otbLocalHoughDraw
	${INPUTDATA}/Line.png
	${TEMP}/feFiltreLocalHoughDraw_Line_20_10_2.png
	20 10 2)   
	        
# -------            otb::FillGapsFilter   ------------------------------        

ADD_TEST(feTuFillGapsFilterNew ${FEATUREEXTRACTION_TESTS5}  
        otbFillGapsFilterNew)
        
ADD_TEST(feTuFillGapsFilter ${FEATUREEXTRACTION_TESTS5}  
	  --compare-image ${EPSILON}  
	       ${BASELINE}/feTvFillGapfilter.png
               ${TEMP}/feTvFillGapfilter.png
        otbFillGapsFilter
	       ${INPUTDATA}/ImageLine.bsq.hdr
	       ${TEMP}/feTvFillGapfilter.png)

# -------            otb::HoughTransform2DLinesImageTest ------------------------------    

ADD_TEST(feTvHoughTransform2DLinesImage ${FEATUREEXTRACTION_TESTS5}  
  --compare-image ${EPSILON}  
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
  --compare-image ${TOL}  ${BASELINE}/feFiltreExtractSegments_ImageLine.hdr
                          ${TEMP}/feFiltreExtractSegments_ImageLine.hdr
        otbExtractSegments
	${INPUTDATA}/ImageLine_hd.bsq.hdr
	${INPUTDATA}/ImageLineDir.bsq.hdr
	${TEMP}/feFiltreExtractSegments_ImageLine.hdr
	2 0.3 20 20 2 10 0.5)                

# -------            otb::ForwardFourierMellinTransformImageFilter   -------------

#ADD_TEST(feTuForwardFourierMellinImageFilterNew ${FEATUREEXTRACTION_TESTS6} 
#         otbFourierMellinImageFilterNew)  

#ADD_TEST(feTuForwardFourierMellinImageFilterCroix ${FEATUREEXTRACTION_TESTS6}  
# --compare-n-images ${TOL} 2 
#		    ${BASELINE}/feFourierMellinCroixOutputModulus.hdr
#		    ${TEMP}/feFourierMellinCroixOutputModulus.hdr
#		    ${BASELINE}/feFourierMellinCroixOutputPhase.hdr
#		    ${TEMP}/feFourierMellinCroixOutputPhase.hdr
#        otbFourierMellinImageFilter
#        ${INPUTDATA}/TestFFT.hdr
#	${TEMP}/feFourierMellinCroixOutputModulus.hdr
#	${TEMP}/feFourierMellinCroixOutputPhase.hdr
#)

#ADD_TEST(feTuForwardFourierMellinImageFilter3Croix ${FEATUREEXTRACTION_TESTS6}  
#   --compare-n-images   ${TOL} 2  
#			${BASELINE}/feFourierMellin3CroixOutputModulus.hdr
#                        ${TEMP}/feFourierMellin3CroixOutputModulus.hdr
#			${BASELINE}/feFourierMellin3CroixOutputPhase.hdr
#                        ${TEMP}/feFourierMellin3CroixOutputPhase.hdr
#    otbFourierMellinImageFilter
#        ${INPUTDATA}/TestFFT3Croix.hdr
#	${TEMP}/feFourierMellin3CroixOutputModulus.hdr
#	${TEMP}/feFourierMellin3CroixOutputPhase.hdr
#)

ADD_TEST(feTvForwardFourierMellinImageFilter ${FEATUREEXTRACTION_TESTS6}  
  --compare-n-images ${EPSILON} 2 
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
  --compare-image ${EPSILON}
		     ${BASELINE}/feImageToEdgePathFilter_rcc8_mire4.png
             ${TEMP}/feImageToEdgePathFilter_rcc8_mire4.png
        otbImageToEdgePathFilter
        ${INPUTDATA}/rcc8_mire4.png
        ${TEMP}/feImageToEdgePathFilter_rcc8_mire4.png
        255
        )
ADD_TEST(feTvImageToEdgePathFilterBis ${FEATUREEXTRACTION_TESTS6}  
  --compare-image ${EPSILON}
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
  --compare-n-images ${EPSILON} 2
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
  --compare-image ${EPSILON}
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
  --compare-image ${EPSILON}
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
  --compare-image ${EPSILON}
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
--compare-ascii ${EPS}  
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
--compare-image ${EPSILON}
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
--compare-ascii ${EPS}  
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
--compare-ascii ${TOL}  ${BASELINE_FILES}/feBreakAngularPathListOutput.txt
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
--compare-ascii ${EPS}  
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
--compare-ascii ${EPS}  
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
--compare-ascii ${EPS}  
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
       --compare-image ${EPSILON}
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
         25.	          # DistanceThreshold for LinkPathFiler
         22.5	          # AngularThreshold for LinkPathFiler
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



ADD_TEST(feTvImageToSIFTKeyPointSetFilterSceneOutputInterestPointAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPS}
		${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
		${TEMP}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
         otbImageToSIFTKeyPointSetFilterOutputInterestPointAscii
		${INPUTDATA}/scene.png
		${TEMP}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
		6 3 0.08 10.0 
)

ADD_TEST(feTvImageToSIFTKeyPointSetFilterSceneDescriptorAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPS}
		${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
		${TEMP}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
         otbImageToSIFTKeyPointSetFilterOutputDescriptorAscii
		${INPUTDATA}/scene.png
		${TEMP}/feTvImageToSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
		6 3 0.08 10.0 
)

ADD_TEST(feTvImageToSIFTKeyPointSetFilterSquareOutputImage ${FEATUREEXTRACTION_TESTS9}
--compare-image ${EPSILON}
		${BASELINE}/feTvImageToSIFTKeyPointSetFilterSquareImageOutput.png
		${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareImageOutput.png
         otbImageToSIFTKeyPointSetFilterOutputImage
		${INPUTDATA}/carre.png
		${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareImageOutput.png
		7 3 0.01 10.0 
)

ADD_TEST(feTvImageToSIFTKeyPointSetFilterSquareOutputAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPS}
		${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput.txt
		${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput.txt
         otbImageToSIFTKeyPointSetFilterOutputAscii
		${INPUTDATA}/carre.png
		${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput.txt
		7 3 0.01 10.0 
)

ADD_TEST(feTvImageToSIFTKeyPointSetFilterSquareRotatedOutputAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPS}
		${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput.txt
		${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput.txt
         otbImageToSIFTKeyPointSetFilterOutputAscii
		${INPUTDATA}/carre_ori.png
		${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput.txt
		7 3 0.01 10.0
)

ADD_TEST(feTvImageToSIFTKeyPointSetFilterGridOutputImage ${FEATUREEXTRACTION_TESTS9}
--compare-image ${EPSILON}
		${BASELINE}/feTvImageToSIFTKeyPointSetFilterGridImageOutput.png
		${TEMP}/feTvImageToSIFTKeyPointSetFilterGridImageOutput.png
         otbImageToSIFTKeyPointSetFilterOutputImage
		${INPUTDATA}/damier.png
		${TEMP}/feTvImageToSIFTKeyPointSetFilterGridImageOutput.png
		7 3 0.01 10.0 
)

ADD_TEST(feTvImageToSIFTKeyPointSetFilterGridOutputAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPS}
		${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterGridKeysOutput.txt
		${TEMP}/feTvImageToSIFTKeyPointSetFilterGridKeysOutput.txt
         otbImageToSIFTKeyPointSetFilterOutputAscii
		${INPUTDATA}/damier.png
		${TEMP}/feTvImageToSIFTKeyPointSetFilterGridKeysOutput.txt
		7 3 0.01 10.0 
)

ADD_TEST(feTvImageToSIFTKeyPointSetFilterSquare2OutputAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPS}
		${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput2.txt
		${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput2.txt
         otbImageToSIFTKeyPointSetFilterOutputAscii
		${INPUTDATA}/carre.png
		${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareKeysOutput2.txt
		1 3 0.5 10.0 
)

ADD_TEST(feTvImageToSIFTKeyPointSetFilterSquareRotated2OutputAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPS}
		${BASELINE_FILES}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput2.txt
		${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput2.txt
         otbImageToSIFTKeyPointSetFilterOutputAscii
		${INPUTDATA}/carre_ori.png
		${TEMP}/feTvImageToSIFTKeyPointSetFilterSquareRotatedKeysOutput2.txt
		1 3 0.215 10.0
)

ADD_TEST(feTvImageToSIFTKeyPointSetFilterQB_SuburbOutputAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPS}
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
--compare-image ${EPS}
	 ${BASELINE}/feTvImageToHessianDeterminantImageFilterOutput.tif
	 ${TEMP}/feTvImageToHessianDeterminantImageFilterOutput.tif
         otbImageToHessianDeterminantImageFilter
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
	 ${TEMP}/feTvImageToHessianDeterminantImageFilterOutput.tif
	 1.5
		 )

# -------            otb::ImageToSURFKeyPointSetFilterNew   -------------

ADD_TEST(feTuImageToSURFKeyPointSetFilterNew ${FEATUREEXTRACTION_TESTS9} 
         otbImageToSURFKeyPointSetFilterNew)


ADD_TEST(feTvImageToSURFKeyPointSetFilterSceneOutputInterestPointAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPS}
		${BASELINE_FILES}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputInterestPoint.txt
		${TEMP}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputInterestPoint.txt
         otbImageToSURFKeyPointSetFilterOutputInterestPointAscii
		${INPUTDATA}/scene.png
		${TEMP}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputInterestPoint.txt
		1 3 
)

ADD_TEST(feTvImageToSURFKeyPointSetFilterSceneOutputDescriptorAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPS}
		${BASELINE_FILES}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputDescriptor.txt
		${TEMP}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputDescriptor.txt
         otbImageToSURFKeyPointSetFilterOutputDescriptorAscii
		${INPUTDATA}/scene.png
		${TEMP}/feTvImageToSURFKeyPointSetFilterSceneKeysOutputDescriptor.txt
		1 3 
)
	 
# -------            otb::ImageFittingPolygonListFilter   -------------
ADD_TEST(feTuImageFittingPolygonListFilterNew ${FEATUREEXTRACTION_TESTS9} 
         otbImageFittingPolygonListFilterNew)

ADD_TEST(feTvImageFittingPolygonListFilter ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPS}
		${BASELINE_FILES}/feTvImageFittingPolygonListFilter_Output.kml
		${TEMP}/feTvImageFittingPolygonListFilter_Output.kml
         otbImageFittingPolygonListFilter
		${INPUTDATA}/polygon.png
		${INPUTDATA}/polygon-start.kml
		${TEMP}/feTvImageFittingPolygonListFilter_Output.kml
		5 10
)

# -------            otb::ImageToFastSIFTKeyPointSetFilter   -------------

ADD_TEST(feTuImageToFastSIFTKeyPointSetFilterNew ${FEATUREEXTRACTION_TESTS9} 
         otbImageToFastSIFTKeyPointSetFilterNew)

ADD_TEST(feTvImageToFastSIFTKeyPointSetFilterSceneOutputDescriptorAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPS}
		${BASELINE_FILES}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
		${TEMP}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
         otbImageToFastSIFTKeyPointSetFilterOutputDescriptorAscii
		${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif 
		${TEMP}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputDescriptor.txt
		6  
)

ADD_TEST(feTvImageToFastSIFTKeyPointSetFilterSceneOutputInterestPointAscii ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPS}
		${BASELINE_FILES}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
		${TEMP}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
         otbImageToFastSIFTKeyPointSetFilterOutputInterestPointAscii
		${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
		${TEMP}/feTvImageToFastSIFTKeyPointSetFilterSceneKeysOutputInterestPoint.txt
		6  
)





# --------- MatchingFilter ----------------
ADD_TEST(KeyPointSetsMatchingFilterNew ${FEATUREEXTRACTION_TESTS9} 
	otbKeyPointSetsMatchingFilterNew)

ADD_TEST(KeyPointSetsMatchingFilter ${FEATUREEXTRACTION_TESTS9}
--compare-ascii ${EPS}				   
	${BASELINE}/feTvKeyPointSetsMatchingFilterOutputAscii.txt
	${TEMP}/feTvKeyPointSetsMatchingFilterOutputAscii.txt	
    otbKeyPointSetsMatchingFilter
	${TEMP}/feTvKeyPointSetsMatchingFilterOutputAscii.txt
	0.6 0
)
	
#--------- LandMark
ADD_TEST(LandmarkNew ${FEATUREEXTRACTION_TESTS9} 
	otbLandmarkNew)			     

# A enrichir
SET(BasicFeatureExtraction_SRCS1
otbAlignImageToPath.cxx
otbDrawPath.cxx
otbDrawPathAlign.cxx
otbComplexMomentImage.cxx
otbHuImage.cxx
otbFlusserImage.cxx
otbComplexMomentPathNew.cxx
otbComplexMomentPath.cxx
otbComplexMomentPathFloat.cxx
otbHuPathNew.cxx
otbHuPath.cxx
)
SET(BasicFeatureExtraction_SRCS2
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
otbExtractSegmentsNew.cxx
otbExtractSegments.cxx
#otbFourierMellinImageFilterNew.cxx #Comment because generate error "duplicate vnl_fft_base<3...>" with otbFourierMellinImageFilter.cxx test on Mac OSX platform
#otbFourierMellinImageFilterTestFFT.cxx
otbFourierMellinImageFilter.cxx
otbImageToEdgePathFilterNew.cxx
otbImageToEdgePathFilter.cxx
otbNeighborhoodScalarProductFilterNew.cxx
otbNeighborhoodScalarProductFilter.cxx
)
SET(BasicFeatureExtraction_SRCS7
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
otbImageToSIFTKeyPointSetFilterNew.cxx
otbImageToSIFTKeyPointSetFilterValid.cxx
otbImageToSIFTKeyPointSetFilterDistanceMap.cxx
otbImageToSIFTKeyPointSetFilterOutputInterestPointAscii.cxx
otbImageToSIFTKeyPointSetFilterOutputDescriptorAscii.cxx
otbImageToHessianDeterminantImageFilterNew.cxx
otbImageToHessianDeterminantImageFilter.cxx
otbImageFittingPolygonListFilter.cxx
otbImageFittingPolygonListFilterNew.cxx
otbImageToSURFKeyPointSetFilterNew.cxx	
otbImageToSURFKeyPointSetFilterOutputInterestPointAscii.cxx
otbImageToSURFKeyPointSetFilterOutputDescriptorAscii.cxx
otbImageToFastSIFTKeyPointSetFilterNew.cxx
otbImageToFastSIFTKeyPointSetFilterOutputDescriptorAscii.cxx
otbImageToFastSIFTKeyPointSetFilterOutputInterestPointAscii.cxx
otbImageToSIFTKeyPointSetFilterOutputAscii.cxx
otbImageToSIFTKeyPointSetFilterOutputImage.cxx
otbKeyPointSetsMatchingFilterNew.cxx
otbKeyPointSetsMatchingFilter.cxx
otbLandmarkNew.cxx
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbFeatureExtractionTests1 otbFeatureExtractionTests1.cxx ${BasicFeatureExtraction_SRCS1})
TARGET_LINK_LIBRARIES(otbFeatureExtractionTests1  OTBFeatureExtraction OTBIO)
ADD_EXECUTABLE(otbFeatureExtractionTests2 otbFeatureExtractionTests2.cxx ${BasicFeatureExtraction_SRCS2})
TARGET_LINK_LIBRARIES(otbFeatureExtractionTests2  OTBFeatureExtraction OTBIO)
ADD_EXECUTABLE(otbFeatureExtractionTests3 otbFeatureExtractionTests3.cxx ${BasicFeatureExtraction_SRCS3})
TARGET_LINK_LIBRARIES(otbFeatureExtractionTests3  OTBFeatureExtraction OTBIO)
ADD_EXECUTABLE(otbFeatureExtractionTests4 otbFeatureExtractionTests4.cxx ${BasicFeatureExtraction_SRCS4})
TARGET_LINK_LIBRARIES(otbFeatureExtractionTests4  OTBFeatureExtraction OTBIO)
ADD_EXECUTABLE(otbFeatureExtractionTests5 otbFeatureExtractionTests5.cxx ${BasicFeatureExtraction_SRCS5})
TARGET_LINK_LIBRARIES(otbFeatureExtractionTests5  OTBFeatureExtraction OTBIO)
ADD_EXECUTABLE(otbFeatureExtractionTests6 otbFeatureExtractionTests6.cxx ${BasicFeatureExtraction_SRCS6})
TARGET_LINK_LIBRARIES(otbFeatureExtractionTests6  OTBFeatureExtraction OTBIO)
ADD_EXECUTABLE(otbFeatureExtractionTests7 otbFeatureExtractionTests7.cxx ${BasicFeatureExtraction_SRCS7})
TARGET_LINK_LIBRARIES(otbFeatureExtractionTests7  OTBFeatureExtraction OTBIO)
ADD_EXECUTABLE(otbFeatureExtractionTests8 otbFeatureExtractionTests8.cxx ${BasicFeatureExtraction_SRCS8})
TARGET_LINK_LIBRARIES(otbFeatureExtractionTests8  OTBFeatureExtraction OTBIO)
ADD_EXECUTABLE(otbFeatureExtractionTests9 otbFeatureExtractionTests9.cxx ${BasicFeatureExtraction_SRCS9})
TARGET_LINK_LIBRARIES(otbFeatureExtractionTests9  OTBFeatureExtraction OTBIO)

# ADD_EXECUTABLE(roadDetect roadDetect.cxx)
# TARGET_LINK_LIBRARIES(roadDetect  OTBFeatureExtraction OTBIO)


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

