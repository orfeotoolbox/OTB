
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance sur diff pixel image
SET(TOL 0.0)
SET(EPSILON 0.000000000001)
SET(EPSILON_10e6 0.000001)
SET(SARPOLARIMETRY_TESTS1 ${CXX_TEST_PATH}/otbSARPolarimetryTests1)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbRADAR_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#ADD_TEST(rdTuPolarimetricSynthesisFilterNew ${RADAR_TESTS1}  
#        otbPolarimetricSynthesisFilterNew
#	)



# A enrichir
SET(SARPOLARIMETRY_SRCS1
otbPolarimetricSynthesisFilterNew.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

#ADD_EXECUTABLE(otbSARPolarimetryTests1 otbSARPolarimetryTests1.cxx ${SARPOLARIMETRY_SRCS1})
#TARGET_LINK_LIBRARIES(otbSARPolarimetryTests1 OTBRadar OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
