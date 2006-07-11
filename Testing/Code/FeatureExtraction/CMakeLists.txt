
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_DIR}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_DIR}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_DIR}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_DIR}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)

SET(FEATUREEXTRACTION_TESTS ${CXX_TEST_PATH}/otbFeatureExtractionTests)



ADD_TEST(feTvAlignMV2ITK ${FEATUREEXTRACTION_TESTS}  
  --compare-ascii ${BASELINE_FILES}/feAlign.txt
                          ${TEMP}/feAlign.txt
        otbAlignImageToPath
	${INPUTDATA}/poupees.hdr
	${TEMP}/feAlign.txt)

# -------            otb::Draw   ------------------------------
		
ADD_TEST(feTvDrawSegmentIntoImage ${FEATUREEXTRACTION_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/feDrawPathIntoPoupees.png
                          ${TEMP}/feDrawPathIntoPoupees.png
        otbDrawPathList
	${INPUTDATA}/poupees.hdr
	${TEMP}/feDrawPathIntoPoupees.png)

ADD_TEST(feTvDrawSegmentIntoImageLine ${FEATUREEXTRACTION_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/feDrawDeuxLignes.png
                          ${TEMP}/feDrawDeuxLignes.png
        otbDrawPathListTracerLignes
	${INPUTDATA}/DeuxTraits.png
	${TEMP}/feDrawDeuxLignes.png)

ADD_TEST(feTvDrawSegmentIntoImage ${FEATUREEXTRACTION_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/feDrawPathIntoDeuxTraits.png
                          ${TEMP}/feDrawPathIntoDeuxTraits.png
        otbDrawPathList
	${INPUTDATA}/DeuxTraits.png
	${TEMP}/feDrawPathIntoDeuxTraits.png)

ADD_TEST(feTvDrawPathTestCarre ${FEATUREEXTRACTION_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/feDrawPathDessinCarre.png
                          ${TEMP}/feDrawPathDessinCarre.png
        otbDrawPathDessinCarre
	${INPUTDATA}/DeuxTraits.png
	${TEMP}/feDrawPathDessinCarre.png)

ADD_TEST(feTvDrawPathTestAlign ${FEATUREEXTRACTION_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/feDrawPathAlignDeuxTraits.png
                          ${TEMP}/feDrawPathAlignDeuxTraits.png
        otbDrawPathAlign
	${INPUTDATA}/DeuxTraits.png
	${TEMP}/feDrawPathAlignDeuxTraits.png)
	
ADD_TEST(feTuComplexMomentImage ${FEATUREEXTRACTION_TESTS}  
        otbComplexMomentImage
	${INPUTDATA}/TeteAToto.png 1 1)

ADD_TEST(feTuHuImage ${FEATUREEXTRACTION_TESTS}  
        otbHuImage
	${INPUTDATA}/TeteAToto.png)

ADD_TEST(feTuFlusserImage ${FEATUREEXTRACTION_TESTS}  
        otbFlusserImage
	${INPUTDATA}/TeteAToto.png)
	
ADD_TEST(feTuComplexMomentPathNew ${FEATUREEXTRACTION_TESTS}  
        otbComplexMomentPathNew)
	
ADD_TEST(feTuComplexMomentPath ${FEATUREEXTRACTION_TESTS}  
        otbComplexMomentPath 1 1)

ADD_TEST(feTuComplexMomentPathFloat ${FEATUREEXTRACTION_TESTS}  
        otbComplexMomentPath 1 1)

ADD_TEST(feTuHuPathNew ${FEATUREEXTRACTION_TESTS}  
        otbHuPathNew)

ADD_TEST(feTuHuPath ${FEATUREEXTRACTION_TESTS}  
        otbHuPath)

ADD_TEST(feTuFlusserPathNew ${FEATUREEXTRACTION_TESTS}  
        otbFlusserPathNew)

ADD_TEST(feTuFlusserPath ${FEATUREEXTRACTION_TESTS}  
        otbFlusserPath)

# -------            otb::TouziEdgeDetector   ------------------------------

ADD_TEST(feTuTouziNew ${FEATUREEXTRACTION_TESTS}  
        otbTouziEdgeDetectorNew)
		
ADD_TEST(feTvTouzi ${FEATUREEXTRACTION_TESTS}
  --compare-image ${TOL}  ${BASELINE}/feFiltreTouzi_amst_3.hdr
                          ${TEMP}/feFiltreTouzi_amst_3.hdr
        otbTouziEdgeDetector
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreTouzi_amst_3.hdr
	3)
	
ADD_TEST(feTvTouziDir ${FEATUREEXTRACTION_TESTS}
  --compare-image ${TOL}  ${BASELINE}/feFiltreTouzi_amst_dir_3.hdr
                          ${TEMP}/feFiltreTouzi_amst_dir_3.hdr  
        otbTouziEdgeDetectorDirection
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreTouzi_amst_dir_3.hdr
	3)	

# -------            otb::LineDetectorBase   ------------------------------

ADD_TEST(feTuLineDetectorBaseNew ${FEATUREEXTRACTION_TESTS}  
        otbLineDetectorBaseNew)
	
# -------            otb::LineCorrelationDetector   ------------------------------
	
ADD_TEST(feTuLineCorrelationNew ${FEATUREEXTRACTION_TESTS}  
        otbLineCorrelationDetectorNew)
        
ADD_TEST(feTvLineCorrelationLinear ${FEATUREEXTRACTION_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/feFiltreLineCorrelationLinear_amst_2_3.hdr
                          ${TEMP}/feFiltreLineCorrelationLinear_amst_2_3.hdr
        otbLineCorrelationDetectorLinear
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreLineCorrelationLinear_amst_2_3.hdr
	2 3)
      
ADD_TEST(feTvLineCorrelationLinearDir ${FEATUREEXTRACTION_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/feFiltreLineCorrelationLinear_amst_dir_2_3.hdr
                          ${TEMP}/feFiltreLineCorrelationLinear_amst_dir_2_3.hdr
        otbLineCorrelationDetectorLinearDirection
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreLineCorrelationLinear_amst_dir_2_3.hdr
	2 3)
		
ADD_TEST(feTvLineCorrelation ${FEATUREEXTRACTION_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/feFiltreLineCorrelation_amst_2_3.hdr
                          ${TEMP}/feFiltreLineCorrelation_amst_2_3.hdr
        otbLineCorrelationDetector
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreLineCorrelation_amst_2_3.hdr
	2 3)
	
# -------            otb::LineRatioDetector   ------------------------------

ADD_TEST(feTuLineRatioNew ${FEATUREEXTRACTION_TESTS}  
        otbLineRatioDetectorNew)
        
ADD_TEST(feTvLineRatioLinear ${FEATUREEXTRACTION_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/feFiltreLineRatioLinear_amst_2_3.hdr
                          ${TEMP}/feFiltreLineRatioLinear_amst_2_3.hdr
        otbLineRatioDetectorLinear
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreLineRatioLinear_amst_2_3.hdr
	2 3)

ADD_TEST(feTvLineRatioLinearDir ${FEATUREEXTRACTION_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/feFiltreLineRatioLinear_amst_dir_2_3.hdr
                          ${TEMP}/feFiltreLineRatioLinear_amst_dir_2_3.hdr
        otbLineRatioDetectorLinearDirection
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreLineRatioLinear_amst_dir_2_3.hdr
	2 3)
	
ADD_TEST(feTvLineRatioLinearAmsters1 ${FEATUREEXTRACTION_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/feFiltreLineRatioLinear_amst_ers1_ima_extrait.hdr
                          ${TEMP}/feFiltreLineRatioLinear_amst_ers1_ima_extrait.hdr
        otbLineRatioDetectorLinear
	${INPUTDATA}/amst_ers1.ima.extrait.419_187_70_66.hdr
	${TEMP}/feFiltreLineRatioLinear_amst_ers1_ima_extrait.hdr
	1 2)
				
ADD_TEST(feTvLineRatio ${FEATUREEXTRACTION_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/feFiltreLineRatio_amst_2_3.hdr
                          ${TEMP}/feFiltreLineRatio_amst_2_3.hdr
        otbLineRatioDetector
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreLineRatio_amst_2_3.hdr
	2 3)


# -------            otb::AssociativeSymmetricalSum   ------------------------------
	
ADD_TEST(feTuAssociativeSymmetricalSumNew ${FEATUREEXTRACTION_TESTS}  
        otbAssociativeSymmetricalSumNew)
        
ADD_TEST(feTvAssociativeSymmetricalSum ${FEATUREEXTRACTION_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/feFiltreASS_amst_2_3.hdr
                          ${TEMP}/feFiltreASS_amst_2_3.hdr
        otbAssociativeSymmetricalSum
        ${TEMP}/feFiltreLineRatioLinear_amst_2_3.hdr
        ${TEMP}/feFiltreLineCorrelationLinear_amst_2_3.hdr
        ${TEMP}/feFiltreASS_amst_2_3.hdr)

# -------            otb::AssymmetricFusionOfLineDetector   ------------------------------
        
ADD_TEST(feTuAssymmetricFusionOfLineDetectorNew ${FEATUREEXTRACTION_TESTS}  
        otbAssymmetricFusionOfLineDetectorNew)
        
ADD_TEST(feTvAssymmetricFusionOfLineDetector ${FEATUREEXTRACTION_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/feFiltreAssymmetricFusion_amst_2_3.hdr
                          ${TEMP}/feFiltreAssymmetricFusion_amst_2_3.hdr
        otbAssymmetricFusionOfLineDetector
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreAssymmetricFusion_amst_2_3.hdr
	2 3)

# -------            otb::HarrisImage   ------------------------------
        
ADD_TEST(feTvHarrisImage ${FEATUREEXTRACTION_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/feHarrisImage.png
                          ${TEMP}/feHarrisImage.png
        otbHarrisImage 
	${INPUTDATA}/TeteAToto.png
	${TEMP}/feHarrisImage.png
	1.0 2.0 0.0)

ADD_TEST(feTvHarrisPoupee ${FEATUREEXTRACTION_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/feHarris_poupees.png
                          ${TEMP}/feHarris_poupees.png
        otbHarrisImage 
	${INPUTDATA}/poupees_1canal.c1.hdr
	${TEMP}/feHarris_poupees.png
	1.0 1.0 1.0)

ADD_TEST(feTvMultiplyByScalarImage ${FEATUREEXTRACTION_TESTS}  
         otbMultiplyByScalarImageFilterTest)

ADD_TEST(feTuThresholdImageToPointSetTest ${FEATUREEXTRACTION_TESTS}  
  --compare-ascii  ${BASELINE_FILES}/feThresholdImageToPointSet.txt
                   ${TEMP}/feThresholdImageToPointSet.txt
        otbThresholdImageToPointSetTest
	${INPUTDATA}/TeteAToto.png
	${TEMP}/feThresholdImageToPointSet.txt )

ADD_TEST(feTvHarrisToPointSet ${FEATUREEXTRACTION_TESTS}  
#  --compare-ascii  ${BASELINE_FILES}/feHarrisToPointSet.txt
#                   ${TEMP}/feHarrisToPointSet.txt
        otbHarrisToPointSet
	${INPUTDATA}/TeteAToto.png
	${TEMP}/feHarrisToPointSet.txt
	1.0 2.0 0.0 2.0)

# -------            otb::PixelSuppressionByDirection   ------------------------------
	
ADD_TEST(feTuPixelSuppressionByDirectionNew ${FEATUREEXTRACTION_TESTS}  
        otbPixelSuppressionByDirectionNew)
        
ADD_TEST(feTvPixelSuppressionByDirection ${FEATUREEXTRACTION_TESTS}
  --compare-image ${TOL}  ${BASELINE}/feFiltrePixelSuppr_ImageLine_2_0_3.hdr
                          ${TEMP}/feFiltrePixelSuppr_ImageLine_2_0_3.hdr
        otbPixelSuppressionByDirection
	${INPUTDATA}/ImageLine_hd.bsq.hdr
	${INPUTDATA}/ImageLineDir.bsq.hdr
	${TEMP}/feFiltrePixelSuppr_ImageLine_2_0_3.hdr
	2 0.3)

# -------            otb::LocalHough   ------------------------------

ADD_TEST(feTuLocalHoughNew ${FEATUREEXTRACTION_TESTS}  
        otbLocalHoughNew)

ADD_TEST(feTuLocalHough ${FEATUREEXTRACTION_TESTS}  
        otbLocalHough
	${INPUTDATA}/ImageLine.bsq.hdr
	32 32 2)
	
ADD_TEST(feTvLocalHoughDraw ${FEATUREEXTRACTION_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/feFiltreLocalHoughDraw_ImageLine_20_20_2.hdr
                          ${TEMP}/feFiltreLocalHoughDraw_ImageLine_20_20_2.hdr
        otbLocalHoughDraw
	${INPUTDATA}/ImageLine.bsq.hdr
	${TEMP}/feFiltreLocalHoughDraw_ImageLine_20_20_2.hdr
	20 20 1)        

ADD_TEST(feTvLocalHoughDraw ${FEATUREEXTRACTION_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/feFiltreLocalHoughDraw_Line_20_10_2.png
                          ${TEMP}/feFiltreLocalHoughDraw_Line_20_10_2.png
        otbLocalHoughDraw
	${INPUTDATA}/Line.png
	${TEMP}/feFiltreLocalHoughDraw_Line_20_10_2.png
	20 10 2)   
	        
# -------            otb::FillGapsFilter   ------------------------------        

ADD_TEST(feTuFillGapsFilterNew ${FEATUREEXTRACTION_TESTS}  
        otbFillGapsFilterNew)
        
#ADD_TEST(feTuFillGapsFilter ${FEATUREEXTRACTION_TESTS}  
#        otbFillGapsFilter
#	${INPUTDATA}/vide.png
#	${TEMP}/feFillGaps.png)

        
# -------            otb::ExtractSegments   ------------------------------

ADD_TEST(feTuExtractSegmentsNew ${FEATUREEXTRACTION_TESTS}  
        otbExtractSegmentsNew)
        
ADD_TEST(feTvExtractSegments ${FEATUREEXTRACTION_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/feFiltreExtractSegments_ImageLine.png
                          ${TEMP}/feFiltreExtractSegments_ImageLine.png
        otbExtractSegments
	${INPUTDATA}/ImageLine_hd.bsq.hdr
	${INPUTDATA}/ImageLineDir.bsq.hdr
	${TEMP}/feFiltreExtractSegments_ImageLine.png
	2 0.3 20 20 2 10 0.5)                

# -----------------------------------------------------------------------
			        


# A enrichir
SET(BasicFeatureExtraction_SRCS
otbAlignImageToPath.cxx
otbDrawPathList.cxx
otbDrawPathListTracerLignes.cxx
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
otbFlusserPathNew.cxx
otbFlusserPath.cxx
otbTouziEdgeDetectorNew.cxx
otbTouziEdgeDetector.cxx
otbTouziEdgeDetectorDirection.cxx
otbLineDetectorBaseNew.cxx
otbLineCorrelationDetectorNew.cxx
otbLineCorrelationDetectorLinear.cxx
otbLineCorrelationDetectorLinearDirection.cxx
otbLineCorrelationDetector.cxx
otbLineRatioDetectorNew.cxx
otbLineRatioDetectorLinear.cxx
otbLineRatioDetectorLinearDirection.cxx
otbLineRatioDetector.cxx
otbAssociativeSymmetricalSumNew.cxx
otbAssociativeSymmetricalSum.cxx
otbAssymmetricFusionOfLineDetectorNew.cxx
otbAssymmetricFusionOfLineDetector.cxx
otbHarrisImage.cxx
otbMultiplyByScalarImageTest.cxx
otbThresholdImageToPointSetTest.cxx
otbHarrisToPointSet.cxx
otbPixelSuppressionByDirectionNew.cxx
otbPixelSuppressionByDirection.cxx
otbFillGapsFilterNew.cxx
otbLocalHoughNew.cxx
otbLocalHough.cxx
otbLocalHoughDraw.cxx
otbExtractSegmentsNew.cxx
otbExtractSegments.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbFeatureExtractionTests otbFeatureExtractionTests.cxx ${BasicFeatureExtraction_SRCS})
TARGET_LINK_LIBRARIES(otbFeatureExtractionTests  OTBFeatureExtraction OTBCommon OTBIO gdal ITKIO ITKCommon)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




