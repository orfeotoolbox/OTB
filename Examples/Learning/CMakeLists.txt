PROJECT(LearningExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(GenerateTrainingImageExample GenerateTrainingImageExample.cxx )
TARGET_LINK_LIBRARIES(GenerateTrainingImageExample OTBCommon OTBIO OTBLearning)

ADD_EXECUTABLE(SVMPointSetExample SVMPointSetExample.cxx )
TARGET_LINK_LIBRARIES(SVMPointSetExample OTBIO OTBCommon
ITKCommon  OTBLearning)


ADD_EXECUTABLE(SVMPointSetModelEstimatorExample SVMPointSetModelEstimatorExample.cxx )
TARGET_LINK_LIBRARIES(SVMPointSetModelEstimatorExample OTBIO
OTBCommon  OTBLearning)

ADD_EXECUTABLE(SVMImageModelEstimatorExample SVMImageModelEstimatorExample.cxx )
TARGET_LINK_LIBRARIES(SVMImageModelEstimatorExample OTBIO
OTBCommon  OTBLearning)

ADD_EXECUTABLE(SVMPointSetClassificationExample SVMPointSetClassificationExample.cxx )
TARGET_LINK_LIBRARIES(SVMPointSetClassificationExample OTBIO
OTBCommon  OTBLearning)

ADD_EXECUTABLE(SVMImageClassificationExample SVMImageClassificationExample.cxx )
TARGET_LINK_LIBRARIES(SVMImageClassificationExample OTBIO
OTBCommon  OTBLearning)

ADD_EXECUTABLE(SVMImageEstimatorClassificationMultiExample SVMImageEstimatorClassificationMultiExample.cxx )
TARGET_LINK_LIBRARIES(SVMImageEstimatorClassificationMultiExample OTBIO
OTBCommon  OTBLearning)

ADD_EXECUTABLE(SOMExample SOMExample.cxx )
TARGET_LINK_LIBRARIES(SOMExample OTBIO OTBCommon  OTBLearning)

ADD_EXECUTABLE(SOMClassifierExample SOMClassifierExample.cxx )
TARGET_LINK_LIBRARIES(SOMClassifierExample OTBIO OTBCommon  OTBLearning)

ADD_EXECUTABLE(SEMModelEstimatorExample SEMModelEstimatorExample.cxx )
TARGET_LINK_LIBRARIES(SEMModelEstimatorExample OTBIO OTBCommon  OTBLearning)

ADD_EXECUTABLE(SVMGenericKernelImageModelEstimatorExample SVMGenericKernelImageModelEstimatorExample.cxx)
TARGET_LINK_LIBRARIES(SVMGenericKernelImageModelEstimatorExample OTBLearning OTBCommon OTBIO)

ADD_EXECUTABLE(SVMGenericKernelImageClassificationExample SVMGenericKernelImageClassificationExample.cxx)
TARGET_LINK_LIBRARIES(SVMGenericKernelImageClassificationExample OTBLearning OTBCommon OTBIO)

#MachineLearningModel framework example
IF(OTB_USE_OPENCV)
  ADD_EXECUTABLE(TrainMachineLearningModelFromSamplesExample TrainMachineLearningModelFromSamplesExample.cxx)
TARGET_LINK_LIBRARIES(TrainMachineLearningModelFromSamplesExample OTBLearning OTBCommon OTBIO)
ENDIF(OTB_USE_OPENCV)

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Learning)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbLearningExamplesTests)

SET(TOL 0.0)
#Tolerance sur la difference des valeurs numeriques dans le --compare-ascii
SET(EPS 0.001)

# ------- SVMImageClassificationExampleTest----------

ADD_TEST(leTeSVMImageClassificationExampleTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ROI_QB_MUL_1_SVN_CLASS.png
        ${TEMP}/ROI_QB_MUL_1_SVN_CLASS.png
        SVMImageClassificationExampleTest
        ${INPUTDATA}/ROI_QB_MUL_1.png
        ${TEMP}/ROI_QB_MUL_1_SVN_CLASS.png
    ${OTB_DATA_ROOT}/Examples/svm_image_model.svm
)

# ------- SVMImageEstimatorClassificationMultiExampleTest----------

ADD_TEST(leTeSVMImageEstimatorClassificationMultiExampleTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
        ${TEMP}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
        SVMImageEstimatorClassificationMultiExampleTest
        ${INPUTDATA}/ROI_QB_MUL_1.png
        ${INPUTDATA}/ROI_mask_multi.png
        ${TEMP}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
        ${TEMP}/ROI_QB_MUL_1_SVN_CLASS_MULTI_Rescaled.png
)

# ------- GenerateTrainingImageExampleTest----------

ADD_TEST(leTeGenerateTrainingImageExampleTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ROI_QB_MUL_4_training.png
        ${TEMP}/ROI_QB_MUL_4_training.png
        GenerateTrainingImageExampleTest
        ${INPUTDATA}/ROI_QB_MUL_4.tif
        ${INPUTDATA}/LearningROIs.txt
        ${TEMP}/ROI_QB_MUL_4_training.png
)

# ------- SVMImageModelEstimatorExampleTest----------

ADD_TEST(leTeSVMImageModelEstimatorExampleTest ${EXE_TESTS}
        --compare-ascii ${EPS}
        ${BASELINE}/svm_image_model.svn
        ${TEMP}/svm_image_model.svn
        --ignore-lines-with 2 probA probB
        SVMImageModelEstimatorExampleTest
        ${INPUTDATA}/ROI_QB_MUL_1.png
        ${INPUTDATA}/ROI_mask.png
        ${TEMP}/svm_image_model.svn
)


# ------- SOMExampleTest----------

ADD_TEST(leTeSOMExampleTest ${EXE_TESTS}
        --compare-n-images ${TOL} 2
        ${BASELINE}/ROI_QB_MUL_SOM.png
        ${TEMP}/ROI_QB_MUL_SOM.png
        ${BASELINE}/ROI_QB_MUL_SOMACT.png
        ${TEMP}/ROI_QB_MUL_SOMACT.png
        SOMExampleTest
        ${INPUTDATA}/ROI_QB_MUL_1.png
        ${TEMP}/ROI_QB_MUL_SOM.png
        ${TEMP}/ROI_QB_MUL_SOMACT.png
        4 4 4 4 15 1.0 0.1 128
)

# ------- SOMClassifierExampleTest----------

ADD_TEST(leTeSOMClassifierExampleTest ${EXE_TESTS}
        --compare-image ${TOL}
        ${BASELINE}/ROI_QB_MUL_extractSOMCLASS.tif
        ${TEMP}/ROI_QB_MUL_extractSOMCLASS.tif
        SOMClassifierExampleTest
        ${INPUTDATA}/ROI_QB_MUL_1_extract.tif
        ${INPUTDATA}/ROI_QB_MUL_extractSOM.tif
        ${TEMP}/ROI_QB_MUL_extractSOMCLASS.tif
)

# ------- SEMModelEstimatorExampleTest----------

ADD_TEST(leTeSEMModelEstimatorExampleTest ${EXE_TESTS}
#        --compare-image ${TOL}
#        ${BASELINE}/ROI_QB_MUL_SEMMODEL_ESTIMATOR.png
#        ${TEMP}/ROI_QB_MUL_SEMMODEL_ESTIMATOR.png
        SEMModelEstimatorExampleTest
        ${INPUTDATA}/ROI_QB_MUL_1.tif
        ${TEMP}/ROI_QB_MUL_SEMMODEL_ESTIMATOR.png
        4 40 5
)

# ------- SVMGenericKernelImageModelEstimatorExampleTest----------

ADD_TEST(leTeSVMGenericKernelImageModelEstimatorExampleTest ${EXE_TESTS}
        --compare-ascii ${EPS}
        ${BASELINE}/svm_image_model_generic_kernel.svn
        ${TEMP}/svm_image_model_generic_kernel.svn
        --ignore-lines-with 2 probA probB
        SVMGenericKernelImageModelEstimatorExampleTest
        ${INPUTDATA}/ROI_QB_MUL_1.png
        ${INPUTDATA}/ROI_mask_multi.png
        ${TEMP}/svm_image_model_generic_kernel.svn
)

# ------- SVMGenericKernelImageClassificationExampleTest----------

ADD_TEST(leTeSVMGenericKernelImageClassificationExampleTest ${EXE_TESTS}
        --compare-image ${EPS}
        ${BASELINE}/ROI_QB_MUL_GENERIC_KERNEL_IMAGE_CLASSIF.png
        ${TEMP}/ROI_QB_MUL_GENERIC_KERNEL_IMAGE_CLASSIF.png
        SVMGenericKernelImageClassificationExampleTest
        ${INPUTDATA}/ROI_QB_MUL_1.png
        ${TEMP}/ROI_QB_MUL_GENERIC_KERNEL_IMAGE_CLASSIF.png
        ${INPUTDATA}/svm_image_model_generic_kernel.svn
)

INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbLearningExamplesTests otbLearningExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbLearningExamplesTests  ITKAlgorithms ITKStatistics OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
