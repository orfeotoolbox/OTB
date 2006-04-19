
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Images)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)

SET(FEATUREEXTRACTION_TESTS ${CXX_TEST_PATH}/otbFeatureExtractionTests)

ADD_TEST(feTuFillGapsFilterNew ${FEATUREEXTRACTION_TESTS}  
        otbFillGapsFilterNew)

ADD_TEST(feTuAlignMV2ITK ${FEATUREEXTRACTION_TESTS}  
        otbAlignImageToPath
	${INPUTDATA}/poupees.hd
	${TEMP}/feAlign.txt)
	
ADD_TEST(feTuTestFLST ${FEATUREEXTRACTION_TESTS}  
        otbFlstTest
	${INPUTDATA}/poupees.hd
	${TEMP}/feoFlstListTree.txt)	
		
ADD_TEST(feTuDrawSegmentIntoImage ${FEATUREEXTRACTION_TESTS}  
        otbDrawPathList
	${INPUTDATA}/poupees.hd
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
	
ADD_TEST(feTuTestFLST ${FEATUREEXTRACTION_TESTS}  
        otbTestFlst
	${INPUTDATA}/TeteAToto.png
	${TEMP}/feFLSTsegment.txt)

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

ADD_TEST(feTuTouziNew ${FEATUREEXTRACTION_TESTS}  
        otbTouziEdgeDetectorNew)
		
ADD_TEST(feTvTouzi ${FEATUREEXTRACTION_TESTS}
  --compare-image ${TOL}  ${BASELINE}/feFiltreTouzi_amst_3.hd
                          ${TEMP}/feFiltreTouzi_amst_3.hd  
        otbTouziEdgeDetector
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreTouzi_amst_3.hd
	3)
	
ADD_TEST(feTvTouziDir ${FEATUREEXTRACTION_TESTS}
  --compare-image ${TOL}  ${BASELINE}/feFiltreTouzi_amst_dir_3.hd
                          ${TEMP}/feFiltreTouzi_amst_dir_3.hd  
        otbTouziEdgeDetectorDirection
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreTouzi_amst_dir_3.hd
	3)	

ADD_TEST(feTuLineRatioNew ${FEATUREEXTRACTION_TESTS}  
        otbLineRatioDetectorNew)
        
ADD_TEST(feTuLineRatioLinear ${FEATUREEXTRACTION_TESTS}  
        otbLineRatioDetectorLinear
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreLineRatioLinear_amst_2_3.hd
	2 3)

ADD_TEST(feTuLineRatioLinearDir ${FEATUREEXTRACTION_TESTS}  
        otbLineRatioDetectorLinearDirection
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreLineRatioLinear_amst_dir_2_3.hd
	2 3)
			
ADD_TEST(feTuLineRatio ${FEATUREEXTRACTION_TESTS}  
        otbLineRatioDetector
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreLineRatio_amst_2_3.hd
	2 3)
	
ADD_TEST(feTuLineCorrelationNew ${FEATUREEXTRACTION_TESTS}  
        otbLineCorrelationDetectorNew)
        
ADD_TEST(feTuLineCorrelationLinear ${FEATUREEXTRACTION_TESTS}  
        otbLineCorrelationDetectorLinear
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreLineCorrelationLinear_amst_2_3.hd
	2 3)
      
ADD_TEST(feTuLineCorrelationLinearDir ${FEATUREEXTRACTION_TESTS}  
        otbLineCorrelationDetectorLinearDirection
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreLineCorrelationLinear_amst_dir_2_3.hd
	2 3)
		
ADD_TEST(feTuLineCorrelation ${FEATUREEXTRACTION_TESTS}  
        otbLineCorrelationDetector
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreLineCorrelation_amst_2_3.hd
	2 3)
	
ADD_TEST(feTuAssociativeSymmetricalSumNew ${FEATUREEXTRACTION_TESTS}  
        otbAssociativeSymmetricalSumNew)
        
ADD_TEST(feTuAssociativeSymmetricalSum ${FEATUREEXTRACTION_TESTS}  
        otbAssociativeSymmetricalSum
        ${TEMP}/feFiltreLineRatioLinear_amst_2_3.hd
        ${TEMP}/feFiltreLineCorrelationLinear_amst_2_3.hd
        ${TEMP}/feFiltreASS_amst_2_3.hd)
        
ADD_TEST(feTuAssymmetricFusionOfLineDetectorNew ${FEATUREEXTRACTION_TESTS}  
        otbAssymmetricFusionOfLineDetectorNew)
        
ADD_TEST(feTuAssymmetricFusionOfLineDetector ${FEATUREEXTRACTION_TESTS}  
        otbAssymmetricFusionOfLineDetector
	${INPUTDATA}/amst.png
	${TEMP}/feFiltreAssymmetricFusion_amst_2_3.hd
	2 3)
        
ADD_TEST(feTvHarrisImage ${FEATUREEXTRACTION_TESTS}  
        otbHarrisImage 
	${INPUTDATA}/TeteAToto.png
	${TEMP}/feHarrisImage.png
	1.0 2.0 0.0)

ADD_TEST(feTvHarrisPoupee ${FEATUREEXTRACTION_TESTS}  
        otbHarrisImage 
	${INPUTDATA}/poupees_1canal.hd
	${TEMP}/feHarris_poupees.png
	1.0 1.0 1.0)

ADD_TEST(feTvMultiplyByScalarImage ${FEATUREEXTRACTION_TESTS}  
         otbMultiplyByScalarImageFilterTest)

ADD_TEST(feTuThresholdImageToPointSetTest ${FEATUREEXTRACTION_TESTS}  
        otbThresholdImageToPointSetTest
	${INPUTDATA}/TeteAToto.png)

ADD_TEST(feTvHarrisImageToPointSet ${FEATUREEXTRACTION_TESTS}  
        otbHarrisImageToPointSet 
	${INPUTDATA}/poupees_1canal.hd
	${TEMP}/feHarris_PointSet_poupees.png
	1.0 1.0 1.0  2.0)
	
ADD_TEST(feTuPixelSuppressionByDirectionNew ${FEATUREEXTRACTION_TESTS}  
        otbPixelSuppressionByDirectionNew)
        
ADD_TEST(feTuPixelSuppressionByDirection ${FEATUREEXTRACTION_TESTS}  
        otbPixelSuppressionByDirection
	${INPUTDATA}/ImageLine.bsq
	${INPUTDATA}/ImageLineDir.bsq
	${TEMP}/feFiltrePixelSuppr_ImageLine.hd
	3 0.1)
			        
#ADD_TEST(tvFEPatrick2 ${FEATUREEXTRACTION_TESTS}  
#  --compare-image ${TOL}  ${BASELINE}/otbExtractROI_cthead1_26_97_209_100.png
#                          ${TEMP}/otbExtractROI_cthead1_26_97_209_100.png
#        otbFeatureExtraction
#        ${INPUTDATA}/cthead1.png 
#        ${TEMP}/otbExtractROI_cthead1_26_97_209_100.png 
#        26 97 209 100 )


# A enrichir
SET(BasicFeatureExtraction_SRCS
otbAlignImageToPath.cxx
otbFlstTest.cxx
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
otbLineRatioDetectorNew.cxx
otbLineRatioDetectorLinear.cxx
otbLineRatioDetectorLinearDirection.cxx
otbLineRatioDetector.cxx
otbLineCorrelationDetectorNew.cxx
otbLineCorrelationDetectorLinear.cxx
otbLineCorrelationDetectorLinearDirection.cxx
otbLineCorrelationDetector.cxx
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
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbFeatureExtractionTests otbFeatureExtractionTests.cxx ${BasicFeatureExtraction_SRCS})
TARGET_LINK_LIBRARIES(otbFeatureExtractionTests ${CAI_LIBRARY} ${GDAL_LIBRARY} OTBFeatureExtraction OTBCommon OTBIO ${CAI_LIBRARY} ITKIO ITKCommon)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




