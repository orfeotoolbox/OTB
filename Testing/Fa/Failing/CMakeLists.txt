set(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

# Failing tests showing bugs

add_test(FA-000307-ExtractROIRegion ${CXX_TEST_PATH}/0000307-ExtractROICompareRegion)

add_test(FA-000479-WriteInt8Image ${CXX_TEST_PATH}/0000479-WriteInt8Image
  ${OTBTesting_BINARY_DIR}/Temporary/FA-0000479-WriteInt8Image.tif
  -10)

add_test(FA-000209-SVMValidationLinearlySeparableWithProbEstimate_KO ${CXX_TEST_PATH}/0000209-SVMValidationLinearlySeparableProbEstimation
         0000209-SVMValidationLinearlySeparableProbEstimation
         500 500
         0.0025 0.0075 0.0075 0.0025 0. 0.0025 0. 0.0025 0 1)

# -------       Vectorization issue   -----------------------------------
# http://bugs.orfeo-toolbox.org/view.php?id=94
 add_test(FA-0000094-fe-PolygonsVectorization ${CXX_TEST_PATH}/0000094-PolygonsVectorization
         -in ${OTB_DATA_ROOT}/Input/objtest2.tif
         -ot ${TEMP}/output.txt
         -out ${TEMP}/objtest2-result.tif
         -s 0
         )
