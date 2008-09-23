PROJECT(ImagePolarimetricSynthesisExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(PiOver4ExamplePolarimetricSynthesisFilter PiOver4ExamplePolarimetricSynthesisFilter.cxx )
TARGET_LINK_LIBRARIES(PiOver4ExamplePolarimetricSynthesisFilter OTBSARPolarimetry OTBCommon OTBIO ITKNumerics ITKIO)


IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/SARPolarimetry)

SET(INPUTDATA ${OTB_SOURCE_DIR}/Examples/Data)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbSARPolarimetryExamplesTests)

#Tolerance sur diff pixel image
SET(TOL 0.0)
SET(EPSILON 0.000000000001)


# -------            PiOver4ExamplePolarimetricSynthesisFilterTest   ------------------------------

# Tutorial example . pi / 4   
ADD_TEST(rdTvPiOver4ExamplePolarimetricSynthesisFilterTest ${SARPOLARIMETRY_TESTS1}  
#  --compare-image ${EPSILON}   
#  		  ${BASELINE}/ARVIMultiChannelRAndBAndNIRVegetationIndex.tif
#                  ${TEMP}/ARVIMultiChannelRAndBAndNIRVegetationIndex.tif
        PiOver4ExamplePolarimetricSynthesisFilterTest
        ${INPUTDATA}/RSAT_imagery_HH.tif
        ${INPUTDATA}/RSAT_imagery_HV.tif
        ${INPUTDATA}/RSAT_imagery_VH.tif        
        ${INPUTDATA}/RSAT_imagery_VV.tif        
        ${TEMP}/resPiOver4PolarimetricSynthesis.tif
	)        


ADD_EXECUTABLE(otbSARPolarimetryExamplesTests otbSARPolarimetryExamplesTests.cxx )
TARGET_LINK_LIBRARIES(otbSARPolarimetryExamplesTests OTBSARPolarimetry OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters)


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
