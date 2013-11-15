project(LearningExamples)
include_regular_expression("^.*$")

add_executable(GenerateTrainingImageExample GenerateTrainingImageExample.cxx )
target_link_libraries(GenerateTrainingImageExample OTBCommon OTBIO OTBLearning)

add_executable(SVMPointSetExample SVMPointSetExample.cxx )
target_link_libraries(SVMPointSetExample OTBIO OTBCommon
${ITK_LIBRARIES}  OTBLearning)


add_executable(SVMPointSetModelEstimatorExample SVMPointSetModelEstimatorExample.cxx )
target_link_libraries(SVMPointSetModelEstimatorExample OTBIO
OTBCommon  OTBLearning)

add_executable(SVMImageModelEstimatorExample SVMImageModelEstimatorExample.cxx )
target_link_libraries(SVMImageModelEstimatorExample OTBIO
OTBCommon  OTBLearning)

add_executable(SVMPointSetClassificationExample SVMPointSetClassificationExample.cxx )
target_link_libraries(SVMPointSetClassificationExample OTBIO
OTBCommon  OTBLearning)

add_executable(SVMImageClassificationExample SVMImageClassificationExample.cxx )
target_link_libraries(SVMImageClassificationExample OTBIO
OTBCommon  OTBLearning)

add_executable(SVMImageEstimatorClassificationMultiExample SVMImageEstimatorClassificationMultiExample.cxx )
target_link_libraries(SVMImageEstimatorClassificationMultiExample OTBIO
OTBCommon  OTBLearning)

add_executable(SOMExample SOMExample.cxx )
target_link_libraries(SOMExample OTBIO OTBCommon  OTBLearning)

add_executable(SOMClassifierExample SOMClassifierExample.cxx )
target_link_libraries(SOMClassifierExample OTBIO OTBCommon  OTBLearning)

add_executable(SEMModelEstimatorExample SEMModelEstimatorExample.cxx )
target_link_libraries(SEMModelEstimatorExample OTBIO OTBCommon  OTBLearning)

add_executable(SVMGenericKernelImageModelEstimatorExample SVMGenericKernelImageModelEstimatorExample.cxx)
target_link_libraries(SVMGenericKernelImageModelEstimatorExample OTBLearning OTBCommon OTBIO)

add_executable(SVMGenericKernelImageClassificationExample SVMGenericKernelImageClassificationExample.cxx)
target_link_libraries(SVMGenericKernelImageClassificationExample OTBLearning OTBCommon OTBIO)


# ------- MachineLearningModel framework examples ----------
if(OTB_USE_OPENCV)
  add_executable(TrainMachineLearningModelFromSamplesExample TrainMachineLearningModelFromSamplesExample.cxx)
  target_link_libraries(TrainMachineLearningModelFromSamplesExample OTBLearning OTBCommon OTBIO)

  add_executable(TrainMachineLearningModelFromImagesExample TrainMachineLearningModelFromImagesExample.cxx)
  target_link_libraries(TrainMachineLearningModelFromImagesExample OTBLearning OTBCommon OTBIO)
endif()

if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Learning)

set(INPUTDATA ${OTB_DATA_ROOT}/Examples)
set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS ${CXX_TEST_PATH}/otbLearningExamplesTests)

set(TOL 0.0)
#Tolerance sur la difference des valeurs numeriques dans le --compare-ascii
set(EPS 0.001)

# ------- SVMImageClassificationExampleTest----------

add_test(leTeSVMImageClassificationExampleTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ROI_QB_MUL_1_SVN_CLASS.png
        ${TEMP}/ROI_QB_MUL_1_SVN_CLASS.png
        SVMImageClassificationExampleTest
        ${INPUTDATA}/ROI_QB_MUL_1.png
        ${TEMP}/ROI_QB_MUL_1_SVN_CLASS.png
    ${OTB_DATA_ROOT}/Examples/svm_image_model.svm
)

# ------- SVMImageEstimatorClassificationMultiExampleTest----------

add_test(leTeSVMImageEstimatorClassificationMultiExampleTest ${EXE_TESTS}
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

add_test(leTeGenerateTrainingImageExampleTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ROI_QB_MUL_4_training.png
        ${TEMP}/ROI_QB_MUL_4_training.png
        GenerateTrainingImageExampleTest
        ${INPUTDATA}/ROI_QB_MUL_4.tif
        ${INPUTDATA}/LearningROIs.txt
        ${TEMP}/ROI_QB_MUL_4_training.png
)

# ------- SVMImageModelEstimatorExampleTest----------

add_test(leTeSVMImageModelEstimatorExampleTest ${EXE_TESTS}
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

add_test(leTeSOMExampleTest ${EXE_TESTS}
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

add_test(leTeSOMClassifierExampleTest ${EXE_TESTS}
        --compare-image ${TOL}
        ${BASELINE}/ROI_QB_MUL_extractSOMCLASS.tif
        ${TEMP}/ROI_QB_MUL_extractSOMCLASS.tif
        SOMClassifierExampleTest
        ${INPUTDATA}/ROI_QB_MUL_1_extract.tif
        ${INPUTDATA}/ROI_QB_MUL_extractSOM.tif
        ${TEMP}/ROI_QB_MUL_extractSOMCLASS.tif
)

# ------- SEMModelEstimatorExampleTest----------

add_test(leTeSEMModelEstimatorExampleTest ${EXE_TESTS}
#        --compare-image ${TOL}
#        ${BASELINE}/ROI_QB_MUL_SEMMODEL_ESTIMATOR.png
#        ${TEMP}/ROI_QB_MUL_SEMMODEL_ESTIMATOR.png
        SEMModelEstimatorExampleTest
        ${INPUTDATA}/ROI_QB_MUL_1.tif
        ${TEMP}/ROI_QB_MUL_SEMMODEL_ESTIMATOR.png
        4 40 5
)

# ------- SVMGenericKernelImageModelEstimatorExampleTest----------

add_test(leTeSVMGenericKernelImageModelEstimatorExampleTest ${EXE_TESTS}
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

add_test(leTeSVMGenericKernelImageClassificationExampleTest ${EXE_TESTS}
        --compare-image ${EPS}
        ${BASELINE}/ROI_QB_MUL_GENERIC_KERNEL_IMAGE_CLASSIF.png
        ${TEMP}/ROI_QB_MUL_GENERIC_KERNEL_IMAGE_CLASSIF.png
        SVMGenericKernelImageClassificationExampleTest
        ${INPUTDATA}/ROI_QB_MUL_1.png
        ${TEMP}/ROI_QB_MUL_GENERIC_KERNEL_IMAGE_CLASSIF.png
        ${INPUTDATA}/svm_image_model_generic_kernel.svn
)

include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbLearningExamplesTests otbLearningExamplesTests.cxx)
target_link_libraries(otbLearningExamplesTests   ${ITK_LIBRARIES} OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBTesting)

endif()
