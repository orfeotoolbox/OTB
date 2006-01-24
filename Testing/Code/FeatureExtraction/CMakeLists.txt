
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Images)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

SET(FEATUREEXTRACTION_TESTS ${CXX_TEST_PATH}/otbFeatureExtractionTests)



ADD_TEST(tuFeFiltreLee ${FEATUREEXTRACTION_TESTS}  
        otbLeeFilter
	${INPUTDATA}/poupees.hd
	${TEMP}/otbFiltreLee_05_05_03.png
	05 05 12.0)
        
#ADD_TEST(tvFEPatrick2 ${FEATUREEXTRACTION_TESTS}  
#  --compare-image   ${BASELINE}/otbExtractROI_cthead1_26_97_209_100.png
#                    ${TEMP}/otbExtractROI_cthead1_26_97_209_100.png
#        otbFeatureExtraction
#        ${INPUTDATA}/cthead1.png 
#        ${TEMP}/otbExtractROI_cthead1_26_97_209_100.png 
#        26 97 209 100 )


# A enrichir
SET(BasicFeatureExtraction_SRCS
otbLeeFilter.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbFeatureExtractionTests otbFeatureExtractionTests.cxx ${BasicFeatureExtraction_SRCS})
TARGET_LINK_LIBRARIES(otbFeatureExtractionTests ${CAI_LIBRARY} ${GDAL_LIBRARY} OTBFeatureExtraction OTBCommon OTBIO ${CAI_LIBRARY} ITKIO ITKCommon)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




