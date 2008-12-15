PROJECT(ImageRadiometryExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter.cxx )
TARGET_LINK_LIBRARIES(ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter OTBRadiometry OTBCommon OTBIO ITKNumerics ITKIO)

ADD_EXECUTABLE(NDVIRAndNIRVegetationIndexImageFilter NDVIRAndNIRVegetationIndexImageFilter.cxx )
TARGET_LINK_LIBRARIES(NDVIRAndNIRVegetationIndexImageFilter OTBRadiometry OTBCommon OTBIO ITKNumerics ITKIO)

ADD_EXECUTABLE(AtmosphericCorrectionSequencement AtmosphericCorrectionSequencement.cxx )
TARGET_LINK_LIBRARIES(AtmosphericCorrectionSequencement OTBRadiometry OTBCommon OTBIO ITKNumerics ITKIO)

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Radiometry)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbRadiometryExamplesTests)

#Tolerance sur diff pixel image
SET(TOL 0.0)
SET(EPSILON 0.000000000001)


# -------            ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter   ------------------------------

ADD_TEST(ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilterTest ${EXE_TESTS}  
  --compare-image ${EPSILON}   
  		  ${BASELINE}/ARVIMultiChannelRAndBAndNIRVegetationIndex.tif
                  ${TEMP}/ARVIMultiChannelRAndBAndNIRVegetationIndex.tif
       ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilterTest
        	  ${INPUTDATA}/poupees_sub.png
	          ${TEMP}/ARVIMultiChannelRAndBAndNIRVegetationIndex.tif
		  ${TEMP}/ARVIMultiChannelRAndBAndNIRVegetationIndex3.tif		
	          ${TEMP}/ARVIMultiChannelRAndBAndNIRVegetationIndex2.tif 
 	          1 
 	          2 
 	          3	
         	  0.6 # Gamma parameter
 	)



# -------            NDVIRAndNIRVegetationIndexImageFilter   ------------------------------
        
ADD_TEST(NDVIRAndNIRVegetationIndexImageFilterTest ${EXE_TESTS}  
  --compare-image ${TOL}   
  	          ${BASELINE}/NDVIRAndNIRVegetationIndex.tif
                  ${TEMP}/NDVIRAndNIRVegetationIndex.tif
       NDVIRAndNIRVegetationIndexImageFilterTest
 	${INPUTDATA}/poupees_sub_c1.png
	${INPUTDATA}/poupees_sub_c2.png
 	${TEMP}/NDVIRAndNIRVegetationIndex.tif
 	${TEMP}/NDVIRAndNIRVegetationIndex2.tif
 	${TEMP}/NDVIRAndNIRVegetationIndex3.tif
	${TEMP}/NDVIRAndNIRVegetationIndex4.tif
 	)

ADD_TEST(AtmosphericCorrectionSequencementTest ${EXE_TESTS}  
        --compare-image ${EPSILON}  
		${BASELINE}/Example_RomaniaAtmosphericCorrectionSequencement.tif
                ${TEMP}/Example_RomaniaAtmosphericCorrectionSequencement.tif
        AtmosphericCorrectionSequencementTest
        ${INPUTDATA}/Romania_Extract.tif
	${TEMP}/Example_RomaniaAtmosphericCorrectionSequencement.tif
        ${INPUTDATA}/atmosphericCorrectionSequencement_alpha_beta.txt
        ${INPUTDATA}/atmosphericCorrectionSequencement_solar_illumination.txt
        ${INPUTDATA}/atmosphericCorrectionSequencement_wavelenght_spectral_bands_spot4_1.txt
        
# ImageToLuminance parameters                
# LuminanceToReflectance parameters                
        27.3    #solar azimuthal elevation
	4       #day
	12      #mounth
# AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms parameters                
        152.7
	2.5	
        -77.0
        1013.
        2.48134
        0.34400
        1		
        0.199854		
# ReflectanceToSurfaceReflectance parameters                
# SurfaceAdjencyEffect6SCorrectionSchemeFilter parameters                
	2       # Radius;	
	0.020   # pixel spacing in kilometers
        )



ADD_EXECUTABLE(otbRadiometryExamplesTests otbRadiometryExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbRadiometryExamplesTests OTBBasicFilters OTBCommon OTBDisparityMap OTBRadiometry OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBFusion  gdal ITKIO ITKAlgorithms ITKStatistics ITKNumerics ITKCommon )

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
