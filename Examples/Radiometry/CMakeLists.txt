PROJECT(ImageRadiometryExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

SET(FUSION_EXAMPLES ${CXX_TEST_PATH}/RadiometryExamplesTests)


ADD_EXECUTABLE(ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter.cxx )
TARGET_LINK_LIBRARIES(ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter OTBRadiometry OTBCommon OTBIO ITKNumerics ITKIO)

ADD_EXECUTABLE(NDVIRAndNIRVegetationIndexImageFilter NDVIRAndNIRVegetationIndexImageFilter.cxx )
TARGET_LINK_LIBRARIES(NDVIRAndNIRVegetationIndexImageFilter OTBRadiometry OTBCommon OTBIO ITKNumerics ITKIO)

IF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Radiometry)

SET(INPUTDATA ${OTB_SOURCE_DIR}/Examples/Data)
SET(TEMP ${OTB_BINARY_DIR}/Examples/Radiometry/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbRadiometryExamplesTests)

#Tolerance sur diff pixel image
SET(TOL 0.0)
SET(EPSILON 0.000000000001)


# -------            ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter   ------------------------------

ADD_TEST(ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilterTest ${EXE_TESTS}  
  --compare-image ${EPSILON}   
  		  ${BASELINE}/ARVIRAndBAndNIRVegetationIndex.tif
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
 	)



INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbRadiometryExamplesTests otbRadiometryExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbRadiometryExamplesTests gdal ITKIO ITKAlgorithms ITKStatistics ITKNumerics ITKCommon OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBFusion)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING )

