
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



ADD_TEST(feTuAlignMV2ITK ${FEATUREEXTRACTION_TESTS}  
        otbAlignImageToPath
	${INPUTDATA}/poupees.hdr
	${TEMP}/feAlign.txt)

# -------            otb::Draw   ------------------------------
		
ADD_TEST(feTuDrawSegmentIntoImage ${FEATUREEXTRACTION_TESTS}  
        otbDrawPathList
	${INPUTDATA}/poupees.hdr
	${TEMP}/feDrawPathIntoPoupees.png)

ADD_TEST(feTuDrawSegmentIntoImageLine ${FEATUREEXTRACTION_TESTS}  
        otbDrawPathListTracerLignes
	${INPUTDATA}/DeuxTraits.png
	${TEMP}/feDrawDeuxLignes.png)

ADD_TEST(feTuDrawSegmentIntoImage ${FEATUREEXTRACTION_TESTS}  
        otbDrawPathList
	${INPUTDATA}/DeuxTraits.png
	${TEMP}/feDrawPathIntoDeuxTraits.png)

ADD_TEST(feTuDrawPathTestCarre ${FEATUREEXTRACTION_TESTS}  
        otbDrawPathDessinCarre
	${INPUTDATA}/DeuxTraits.png
	${TEMP}/feDrawPathDessinCarre.png)

ADD_TEST(feTuDrawPathTestAlign ${FEATUREEXTRACTION_TESTS}  
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
        
ADD_TEST(feTuLineCorrelationLinear ${FEATUREEXTRACTION_TESTS}  
        otbLineCorrelationDetectorLinear
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreLineCorrelationLinear_amst_2_3.hdr
	2 3)
      
ADD_TEST(feTuLineCorrelationLinearDir ${FEATUREEXTRACTION_TESTS}  
        otbLineCorrelationDetectorLinearDirection
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreLineCorrelationLinear_amst_dir_2_3.hdr
	2 3)
		
ADD_TEST(feTuLineCorrelation ${FEATUREEXTRACTION_TESTS}  
        otbLineCorrelationDetector
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreLineCorrelation_amst_2_3.hdr
	2 3)
	
# -------            otb::LineRatioDetector   ------------------------------

ADD_TEST(feTuLineRatioNew ${FEATUREEXTRACTION_TESTS}  
        otbLineRatioDetectorNew)
        
ADD_TEST(feTuLineRatioLinear ${FEATUREEXTRACTION_TESTS}  
        otbLineRatioDetectorLinear
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreLineRatioLinear_amst_2_3.hdr
	2 3)

ADD_TEST(feTuLineRatioLinearDir ${FEATUREEXTRACTION_TESTS}  
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
				
ADD_TEST(feTuLineRatio ${FEATUREEXTRACTION_TESTS}  
        otbLineRatioDetector
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreLineRatio_amst_2_3.hdr
	2 3)


# -------            otb::AssociativeSymmetricalSum   ------------------------------
	
ADD_TEST(feTuAssociativeSymmetricalSumNew ${FEATUREEXTRACTION_TESTS}  
        otbAssociativeSymmetricalSumNew)
        
ADD_TEST(feTuAssociativeSymmetricalSum ${FEATUREEXTRACTION_TESTS}  
        otbAssociativeSymmetricalSum
        ${TEMP}/feFiltreLineRatioLinear_amst_2_3.hdr
        ${TEMP}/feFiltreLineCorrelationLinear_amst_2_3.hdr
        ${TEMP}/feFiltreASS_amst_2_3.hdr)

# -------            otb::AssymmetricFusionOfLineDetector   ------------------------------
        
ADD_TEST(feTuAssymmetricFusionOfLineDetectorNew ${FEATUREEXTRACTION_TESTS}  
        otbAssymmetricFusionOfLineDetectorNew)
        
ADD_TEST(feTuAssymmetricFusionOfLineDetector ${FEATUREEXTRACTION_TESTS}  
        otbAssymmetricFusionOfLineDetector
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreAssymmetricFusion_amst_2_3.hdr
	2 3)

# -------            otb::HarrisImage   ------------------------------
        
ADD_TEST(feTvHarrisImage ${FEATUREEXTRACTION_TESTS}  
        otbHarrisImage 
	${INPUTDATA}/TeteAToto.png
	${TEMP}/feHarrisImage.png
	1.0 2.0 0.0)

ADD_TEST(feTvHarrisPoupee ${FEATUREEXTRACTION_TESTS}  
        otbHarrisImage 
	${INPUTDATA}/poupees_1canal.c1.hdr
	${TEMP}/feHarris_poupees.png
	1.0 1.0 1.0)

ADD_TEST(feTvMultiplyByScalarImage ${FEATUREEXTRACTION_TESTS}  
         otbMultiplyByScalarImageFilterTest)

ADD_TEST(feTuThresholdImageToPointSetTest ${FEATUREEXTRACTION_TESTS}  
        otbThresholdImageToPointSetTest
	${INPUTDATA}/TeteAToto.png)

ADD_TEST(feTvHarrisImageToPointSet ${FEATUREEXTRACTION_TESTS}  
        otbHarrisImageToPointSet 
	${INPUTDATA}/poupees_1canal.c1.hdr
	${TEMP}/feHarris_PointSet_poupees.png
	1.0 1.0 1.0  2.0)

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
	${INPUTDATA}/ImageLine.bsq
	32 32 2)
	
ADD_TEST(feTuLocalHoughDraw ${FEATUREEXTRACTION_TESTS}  
        otbLocalHoughDraw
	${INPUTDATA}/ImageLine.bsq.hdr
	${TEMP}/feFiltreLocalHoughDraw_ImageLine_20_20_2.hdr
	20 20 1)        

ADD_TEST(feTuLocalHoughDraw ${FEATUREEXTRACTION_TESTS}  
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
        
ADD_TEST(feTuExtractSegments ${FEATUREEXTRACTION_TESTS}  
        otbExtractSegments
	${INPUTDATA}/ImageLine_hd.bsq.hdr
	${INPUTDATA}/ImageLineDir.bsq.hdr
	${TEMP}/feFiltreExtractSegments_ImageLine.png
	2 0.3 20 20 2)                

# -----------------------------------------------------------------------
			        


# A enrichir
SET(BasicFeatureExtraction_SRCS
otbAlignImageToPath.cxx
otbDrawPathList.cxx
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




