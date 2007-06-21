 
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance sur diff pixel image
SET(TOL 0.0)
SET(EPSILON 0.00000000001)


SET(UTILITIES_TESTS ${CXX_TEST_PATH}/otbUtilitiesTests)



ADD_TEST(utOssimIntegrationTest ${UTILITIES_TESTS}
ossimIntegrationTest
)

ADD_TEST(utIjBSplineScatteredDataPointSetToImageFilterTest ${UTILITIES_TESTS}
--compare-image ${EPSILON}
		${BASELINE}/ijBSplineScatteredDataPointSetToImageFilterTestOutput.hdr
		${TEMP}/ijBSplineScatteredDataPointSetToImageFilterTestOutput.hdr
		ijBSplineScatteredDataPointSetToImageFilterTest
		${INPUTDATA}/brain.png
		${TEMP}/ijBSplineScatteredDataPointSetToImageFilterTestOutput.hdr
)

# -------            lib otbsvm   ------------------------------

ADD_TEST(utTuSvmKernelFunctorTest ${UTILITIES_TESTS}
                svmGenericKernelFunctor
)

ADD_TEST(utTvSvmTest ${UTILITIES_TESTS}
--compare-ascii 0 ${INPUTDATA}/svm_model
                ${TEMP}/svm_model_test
                svmTest
                ${INPUTDATA}/svm_model
                ${TEMP}/svm_model_test
)

ADD_TEST(utTvSvmTestNewVersionFileGenericModelTest ${UTILITIES_TESTS}
--compare-ascii 0 ${BASELINE_FILES}/utTvsvm_model_image_generic_version_test
                ${TEMP}/utTvsvm_model_image_generic_version_test
                svmTest
                ${INPUTDATA}/svm_model_image_generic_version
                ${TEMP}/utTvsvm_model_image_generic_version_test
)

ADD_TEST(utTvSvmTestGenericModelTest ${UTILITIES_TESTS}
--compare-ascii 0 ${INPUTDATA}/svm_model_image_generic_kernel
                ${TEMP}/svm_model_image_generic_kernel_test
                svmGenericKernelTest
                ${INPUTDATA}/svm_model_image_generic_kernel
                ${TEMP}/svm_model_image_generic_kernel_test
)




# -------       Fichiers sources CXX -----------------------------------
SET(UtilitiesTests_SRCS
ossimIntegrationTest.cxx
ijBSplineScatteredDataPointSetToImageFilterTest.cxx
svmGenericKernelFunctor.cxx
svmTest.cxx
svmGenericKernelTest.cxx
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbUtilitiesTests otbUtilitiesTests.cxx ${UtilitiesTests_SRCS})
TARGET_LINK_LIBRARIES(otbUtilitiesTests OTBIO OTBCommon gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon ossim otbsvm)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )
