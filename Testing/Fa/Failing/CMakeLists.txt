SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

# Failing tests showing bugs

ADD_TEST(FA-000307-ExtractROIRegion ${CXX_TEST_PATH}/0000307-ExtractROICompareRegion)

ADD_TEST(FA-000479-WriteInt8Image ${CXX_TEST_PATH}/0000479-WriteInt8Image 
  ${OTBTesting_BINARY_DIR}/Temporary/FA-0000479-WriteInt8Image.tif
  -10)

ADD_TEST(FA-000209-SVMValidationLinearlySeparableWithProbEstimate_KO
         ${CXX_TEST_PATH}/0000209-SVMValidationLinearlySeparableProbEstimation
         0000209-SVMValidationLinearlySeparableProbEstimation
         500 500
         0.0025 0.0075 0.0075 0.0025 0. 0.0025 0. 0.0025 0 1)

ADD_TEST(NAME FA-000478-UncaughtException
         COMMAND  ${CXX_TEST_PATH}/0000478-UncaughtException
                  $<TARGET_FILE_DIR:otbapp_StereoSensorModelToElevationMap>
                  ${OTB_DATA_ROOT}/Examples/sensor_stereo_left.tif 
                  ${OTB_DATA_ROOT}/Examples/sensor_stereo_right.tif 
                  ${OTBTesting_BINARY_DIR}/Temporary/FA-0000478-output.tif)
