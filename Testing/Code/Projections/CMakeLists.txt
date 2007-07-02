
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)
SET(EPSILON 0.0000000001)

SET(PROJECTIONS_TESTS ${CXX_TEST_PATH}/otbProjectionsTests)


ADD_TEST(prTuProjectionBaseNew ${PROJECTIONS_TESTS}  otbProjectionBaseNew )

ADD_TEST(prTuMapProjectionsNew ${PROJECTIONS_TESTS}  otbMapProjectionsNew )

ADD_TEST(prTuSensorModelsNew ${PROJECTIONS_TESTS}  otbSensorModelsNew )


SET(Projections_SRCS
otbProjectionBaseNew.cxx
otbMapProjectionsNew.cxx
otbSensorModelsNew.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbProjectionsTests otbProjectionsTests.cxx ${Projections_SRCS})
TARGET_LINK_LIBRARIES(otbProjectionsTests  OTBProjections OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters otbossim)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




