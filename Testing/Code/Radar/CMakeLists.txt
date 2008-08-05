
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
SET(RADAR_TESTS1 ${CXX_TEST_PATH}/otbRADARTests1)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbRADAR_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# A enrichir
SET(RADAR_SRCS1
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbRADARTests1 otbRADARTests1.cxx ${RADAR_SRCS1})
TARGET_LINK_LIBRARIES(otbRADARTests1 OTBRADAR OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
