
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Images)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)

SET(FEATUREEXTRACTION_TESTS ${CXX_TEST_PATH}/otbFeatureExtractionTests)


ADD_TEST(feTuFiltreLee1CanalPoupees ${FEATUREEXTRACTION_TESTS}  
        otbLeeFilter
	${INPUTDATA}/poupees_1canal.hd
	${TEMP}/feFiltreLee_05_05_04.png
	05 05 4.0)

ADD_TEST(feTuFiltreLee ${FEATUREEXTRACTION_TESTS}  
        otbLeeFilter
	${INPUTDATA}/poupees.hd
	${TEMP}/feFiltreLee_05_05_12.png
	05 05 12.0)
	
ADD_TEST(feTuFiltreFrost ${FEATUREEXTRACTION_TESTS}  
        otbFrostFilter
	${INPUTDATA}/poupees.hd
	${TEMP}/feFiltreFrost_poupees_05_05_01.png
	05 05 0.1)

ADD_TEST(feTuAlignMV2ITK ${FEATUREEXTRACTION_TESTS}  
        otbAlignImageToPath
	${INPUTDATA}/poupees.hd
	${TEMP}/feAlign.png)
	
		        
#ADD_TEST(tvFEPatrick2 ${FEATUREEXTRACTION_TESTS}  
#  --compare-image ${TOL}  ${BASELINE}/otbExtractROI_cthead1_26_97_209_100.png
#                          ${TEMP}/otbExtractROI_cthead1_26_97_209_100.png
#        otbFeatureExtraction
#        ${INPUTDATA}/cthead1.png 
#        ${TEMP}/otbExtractROI_cthead1_26_97_209_100.png 
#        26 97 209 100 )


# A enrichir
SET(BasicFeatureExtraction_SRCS
otbLeeFilter.cxx
otbFrostFilter.cxx
otbAlignImageToPath.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbFeatureExtractionTests otbFeatureExtractionTests.cxx ${BasicFeatureExtraction_SRCS})
TARGET_LINK_LIBRARIES(otbFeatureExtractionTests ${CAI_LIBRARY} ${GDAL_LIBRARY} OTBFeatureExtraction OTBCommon OTBIO ${CAI_LIBRARY} ITKIO ITKCommon)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




