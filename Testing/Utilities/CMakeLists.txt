 
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
SET(EPSILON_10e6 0.000001)


SET(UTILITIES_TESTS ${CXX_TEST_PATH}/otbUtilitiesTests)


# Commentaire parce que trop long. Supprimer commentaire si besoin de tester integration lib Ossim
#ADD_TEST(utOssimIntegrationTest ${UTILITIES_TESTS}
#        ossimIntegrationTest
#        ossimTiffWriter
#        ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
#        ${TEMP}/utOssimIntegration.tif
#)

ADD_TEST(utOssimKeywordlistToulouseQuickBirdTest ${UTILITIES_TESTS}
--compare-ascii ${EPSILON_10e6}  ${BASELINE_FILES}/utOssimKeywordlistToulouseQuickBird.txt
                        ${TEMP}/utOssimKeywordlistToulouseQuickBird.txt
        ossimKeywordlistTest
        ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/utOssimKeywordlistToulouseQuickBird.txt
        )

ADD_TEST(utOssimKeywordlistParisIkonosTest ${UTILITIES_TESTS}
--compare-ascii ${TOL}  ${BASELINE_FILES}/utOssimKeywordlistParisIkonos.txt
                        ${TEMP}/utOssimKeywordlistParisIkonos.txt
        ossimKeywordlistTest
        ${IMAGEDATA}/PARIS/Ikonos/po_79039_pan_0000000.tif
        ${TEMP}/utOssimKeywordlistParisIkonos.txt
        )

ADD_TEST(utOssimElevManagerTest ${UTILITIES_TESTS}
        ossimElevManagerTest
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
--compare-ascii ${TOL}  ${INPUTDATA}/svm_model
                ${TEMP}/svm_model_test
                svmTest
                ${INPUTDATA}/svm_model
                ${TEMP}/svm_model_test
)

ADD_TEST(utTvSvmTestNewVersionFileGenericModelTest ${UTILITIES_TESTS}
--compare-ascii ${TOL}  ${BASELINE_FILES}/utTvsvm_model_image_generic_version_test
                ${TEMP}/utTvsvm_model_image_generic_version_test
                svmTest
                ${INPUTDATA}/svm_model_image_generic_version
                ${TEMP}/utTvsvm_model_image_generic_version_test
)

ADD_TEST(utTvSvmTestGenericModelTest ${UTILITIES_TESTS}
--compare-ascii ${TOL}  ${INPUTDATA}/svm_model_image_generic_kernel
                ${TEMP}/svm_model_image_generic_kernel_test
                svmGenericKernelTest
                ${INPUTDATA}/svm_model_image_generic_kernel
                ${TEMP}/svm_model_image_generic_kernel_test
)

# -------            lib otbsvm   ------------------------------

# The file is not read. It a screen user scan. main.exe < 132.149.107.66.wloip20967.INP
#ADD_TEST(utTv6SMainTest ${UTILITIES_TESTS}
#                SIXSMainTest
#                ${INPUTDATA}/132.149.107.66.wloip20967.INP
#)

ADD_TEST(utTv6SFunctionMainTest ${UTILITIES_TESTS}
#--compare-ascii ${EPSILON_10e6}  ${BASELINE_FILES}/ut6S_function_main_test.txt
--compare-ascii ${TOL}  ${BASELINE_FILES}/ut6S_function_main_test.txt
                ${TEMP}/ut6S_function_main_test.txt
                SIXSFunctionMainOtbTest
                ${INPUTDATA}/in6S_otb
                ${TEMP}/ut6S_function_main_test.txt
)

# -------       Fichiers sources CXX -----------------------------------
SET(UtilitiesTests_SRCS
ossimIntegrationTest.cxx
ossimKeywordlistTest.cxx
ossimElevManagerTest.cxx
ijBSplineScatteredDataPointSetToImageFilterTest.cxx
svmGenericKernelFunctor.cxx
svmTest.cxx
svmGenericKernelTest.cxx
6SFunctionMainTest.cxx
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbUtilitiesTests otbUtilitiesTests.cxx ${UtilitiesTests_SRCS})
TARGET_LINK_LIBRARIES(otbUtilitiesTests OTBIO OTBCommon gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon otbossim otbsvm otb6S)
IF(UNIX)
    TARGET_LINK_LIBRARIES (otbUtilitiesTests m)
ENDIF(UNIX)

# Original 6S main executable in C
#     ADD_EXECUTABLE(otb6SMain 6SMainTest.cxx )
#     TARGET_LINK_LIBRARIES(otb6SMain OTBCommon ITKCommon otb6S)
#     IF(UNIX)
#         TARGET_LINK_LIBRARIES (otb6SMain m)
#     ENDIF(UNIX)

# OTB 6S main executable in C (call 6S main function.
#     ADD_EXECUTABLE(otb6SMainWithCallFunction 6SMainOtbWithCallFunctionTest.cxx )
#     TARGET_LINK_LIBRARIES(otb6SMainWithCallFunction OTBCommon ITKCommon otb6S)
#     IF(UNIX)
#         TARGET_LINK_LIBRARIES (otb6SMainWithCallFunction m)
#     ENDIF(UNIX)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )
