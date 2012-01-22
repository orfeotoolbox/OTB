SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

# Failing tests showing bugs

# -------  Compare region propagation of ExtractROI filters  ------------------------------
ADD_TEST(FA-000307-ExtractROIRegion ${CXX_TEST_PATH}/0000307-ExtractROICompareRegion)

ADD_EXECUTABLE(0000307-ExtractROICompareRegion  0000307-ExtractROICompareRegionsImplementations.cxx)
TARGET_LINK_LIBRARIES(0000307-ExtractROICompareRegion OTBIO OTBCommon)

# -------  Write an int8 image ------------------------------
ADD_TEST(FA-0000479-WriteInt8Image ${CXX_TEST_PATH}/0000479-WriteInt8Image 
  ${TEMP}/FA-0000479-WriteInt8Image.tif
  -10)

ADD_EXECUTABLE(0000479-WriteInt8Image 0000479-WriteInt8Image.cxx)
TARGET_LINK_LIBRARIES(0000479-WriteInt8Image OTBIO OTBCommon)

ADD_TEST(FA-000209-SVMValidationLinearlySeparableWithProbEstimate_KO ${CXX_TEST_PATH}/0000209-SVMValidationLinearlySeparableProbEstimation
0000209-SVMValidationLinearlySeparableProbEstimation 500 500 0.0025 0.0075 0.0075 0.0025 0. 0.0025 0. 0.0025 0 1)

# ------- Uncaught exception in applications ----------------------
ADD_TEST(NAME FA-0000478-UncaughtException
         COMMAND  ${CXX_TEST_PATH}/0000478-UncaughtException
                  $<TARGET_FILE_DIR:otbapp_StereoSensorModelToElevationMap>
                  ${OTB_DATA_ROOT}/Examples/sensor_stereo_left.tif 
                  ${OTB_DATA_ROOT}/Examples/sensor_stereo_right.tif 
                  ${TEMP}/FA-0000478-output.tif)

ADD_EXECUTABLE(0000478-UncaughtException 0000478-UncaughtException.cxx)
TARGET_LINK_LIBRARIES(0000478-UncaughtException OTBIO OTBCommon OTBApplicationEngine)
