# Failing tests showing bugs

# -------  Compare region propagation of ExtractROI filters  ------------------------------
ADD_TEST(FA-000307-ExtractROIRegion ${CXX_TEST_PATH}/0000307-ExtractROICompareRegion)

ADD_TEST(FA-000209-SVMValidationLinearlySeparableWithProbEstimate_KO ${CXX_TEST_PATH}/0000209-SVMValidationLinearlySeparableProbEstimation
0000209-SVMValidationLinearlySeparableProbEstimation 500 500 0.0025 0.0075 0.0075 0.0025 0. 0.0025 0. 0.0025 0 1)

ADD_EXECUTABLE(0000307-ExtractROICompareRegion  0000307-ExtractROICompareRegionsImplementations.cxx)
TARGET_LINK_LIBRARIES(0000307-ExtractROICompareRegion OTBIO OTBCommon)

