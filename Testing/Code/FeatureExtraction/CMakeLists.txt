
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Images)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)

SET(FEATUREEXTRACTION_TESTS ${CXX_TEST_PATH}/otbFeatureExtractionTests)

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
	${TEMP}/feFLST.png)

ADD_TEST(feTuComplexMomentImage ${FEATUREEXTRACTION_TESTS}  
        otbComplexMomentImage
	${INPUTDATA}/TeteAToto.png 1 1)

ADD_TEST(feTuHuImage ${FEATUREEXTRACTION_TESTS}  
        otbHuImage
	${INPUTDATA}/TeteAToto.png)

ADD_TEST(feTuFlusserImage ${FEATUREEXTRACTION_TESTS}  
        otbFlusserImage
	${INPUTDATA}/TeteAToto.png)

ADD_TEST(feTuTouziNew ${FEATUREEXTRACTION_TESTS}  
        otbTouziEdgeDetectorNew)
		
ADD_TEST(feTvTouzi ${FEATUREEXTRACTION_TESTS}
  --compare-image ${TOL}  ${BASELINE}/feFiltreTouzi_poupees_3.hd
                          ${TEMP}/feFiltreTouzi_poupees_3.hd  
        otbTouziEdgeDetector
	${INPUTDATA}/poupees_1canal.c1
	${TEMP}/feFiltreTouzi_poupees_3.hd
	3)
	
ADD_TEST(feTvTouziDir ${FEATUREEXTRACTION_TESTS}
  --compare-image ${TOL}  ${BASELINE}/feFiltreTouzi_poupees_dir_3.hd
                          ${TEMP}/feFiltreTouzi_poupees_dir_3.hd  
        otbTouziEdgeDetectorDirection
	${INPUTDATA}/poupees_1canal.c1
	${TEMP}/feFiltreTouzi_poupees_dir_3.hd
	3)	

ADD_TEST(feTuLineRatioNew ${FEATUREEXTRACTION_TESTS}  
        otbLineRatioDetectorNew)
        
ADD_TEST(feTuLineRatioLinear ${FEATUREEXTRACTION_TESTS}  
        otbLineRatioDetectorLinear
	${INPUTDATA}/poupees_1canal.hd
	${TEMP}/feFiltreLineRatioLinear_poupees_2_3.hd
	2 3)
		
ADD_TEST(feTuLineRatio ${FEATUREEXTRACTION_TESTS}  
        otbLineRatioDetector
	${INPUTDATA}/poupees_1canal.hd
	${TEMP}/feFiltreLineRatio_poupees_2_3.hd
	2 3)
	
ADD_TEST(feTuLineCorrelationNew ${FEATUREEXTRACTION_TESTS}  
        otbLineCorrelationDetectorNew)
        
ADD_TEST(feTuLineCorrelationLinear ${FEATUREEXTRACTION_TESTS}  
        otbLineCorrelationDetectorLinear
	${INPUTDATA}/poupees_1canal.hd
	${TEMP}/feFiltreLineCorrelationLinear_poupees_2_3.hd
	2 3)
        		        
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
otbTouziEdgeDetectorNew.cxx
otbTouziEdgeDetector.cxx
otbTouziEdgeDetectorDirection.cxx
otbLineRatioDetectorNew.cxx
otbLineRatioDetectorLinear.cxx
otbLineRatioDetector.cxx
otbLineCorrelationDetectorNew.cxx
otbLineCorrelationDetectorLinear.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbFeatureExtractionTests otbFeatureExtractionTests.cxx ${BasicFeatureExtraction_SRCS})
TARGET_LINK_LIBRARIES(otbFeatureExtractionTests ${CAI_LIBRARY} ${GDAL_LIBRARY} OTBFeatureExtraction OTBCommon OTBIO ${CAI_LIBRARY} ITKIO ITKCommon)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




