
# ------- SVMImageClassificationExampleTest----------

otb_add_test(NAME leTeSVMImageClassificationExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/ROI_QB_MUL_1_SVN_CLASS.png
    ${TEMP}/ROI_QB_MUL_1_SVN_CLASS.png
  Execute $<TARGET_FILE:SVMImageClassificationExample>
    ${INPUTDATA}/ROI_QB_MUL_1.png
    ${TEMP}/ROI_QB_MUL_1_SVN_CLASS.png
    ${OTB_DATA_ROOT}/Examples/svm_image_model.svm
)

# ------- SVMImageEstimatorClassificationMultiExampleTest----------

otb_add_test(NAME leTeSVMImageEstimatorClassificationMultiExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
    ${TEMP}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
  Execute $<TARGET_FILE:SVMImageEstimatorClassificationMultiExample>
    ${INPUTDATA}/ROI_QB_MUL_1.png
    ${INPUTDATA}/ROI_mask_multi.png
    ${TEMP}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
    ${TEMP}/ROI_QB_MUL_1_SVN_CLASS_MULTI_Rescaled.png
)

# ------- GenerateTrainingImageExampleTest----------

otb_add_test(NAME leTeGenerateTrainingImageExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/ROI_QB_MUL_4_training.png
    ${TEMP}/ROI_QB_MUL_4_training.png
  Execute $<TARGET_FILE:GenerateTrainingImageExample>
    ${INPUTDATA}/ROI_QB_MUL_4.tif
    ${INPUTDATA}/LearningROIs.txt
    ${TEMP}/ROI_QB_MUL_4_training.png
)

# ------- SVMImageModelEstimatorExampleTest----------

otb_add_test(NAME leTeSVMImageModelEstimatorExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-ascii ${EPSILON_3}
    ${BASELINE}/svm_image_model.svn
    ${TEMP}/svm_image_model.svn
  --ignore-lines-with 2 probA probB
  Execute $<TARGET_FILE:SVMImageModelEstimatorExample>
    ${INPUTDATA}/ROI_QB_MUL_1.png
    ${INPUTDATA}/ROI_mask.png
    ${TEMP}/svm_image_model.svn
)


# ------- SOMExampleTest----------

otb_add_test(NAME leTeSOMExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/ROI_QB_MUL_SOM.png
    ${TEMP}/ROI_QB_MUL_SOM.png
    ${BASELINE}/ROI_QB_MUL_SOMACT.png
    ${TEMP}/ROI_QB_MUL_SOMACT.png
  Execute $<TARGET_FILE:SOMExample>
    ${INPUTDATA}/ROI_QB_MUL_1.png
    ${TEMP}/ROI_QB_MUL_SOM.png
    ${TEMP}/ROI_QB_MUL_SOMACT.png
    4 4 4 4 15 1.0 0.1 128
)

# ------- SOMClassifierExampleTest----------

otb_add_test(NAME leTeSOMClassifierExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/ROI_QB_MUL_extractSOMCLASS.tif
    ${TEMP}/ROI_QB_MUL_extractSOMCLASS.tif
  Execute $<TARGET_FILE:SOMClassifierExample>
    ${INPUTDATA}/ROI_QB_MUL_1_extract.tif
    ${INPUTDATA}/ROI_QB_MUL_extractSOM.tif
    ${TEMP}/ROI_QB_MUL_extractSOMCLASS.tif
)

# ------- SEMModelEstimatorExampleTest----------

otb_add_test(NAME leTeSEMModelEstimatorExampleTest COMMAND ${OTB_TEST_DRIVER}
#        --compare-image ${NOTOL}
#        ${BASELINE}/ROI_QB_MUL_SEMMODEL_ESTIMATOR.png
#        ${TEMP}/ROI_QB_MUL_SEMMODEL_ESTIMATOR.png
  Execute $<TARGET_FILE:SEMModelEstimatorExample>
    ${INPUTDATA}/ROI_QB_MUL_1.tif
    ${TEMP}/ROI_QB_MUL_SEMMODEL_ESTIMATOR.png
    4 40 5
)

# ------- SVMGenericKernelImageModelEstimatorExampleTest----------

otb_add_test(NAME leTeSVMGenericKernelImageModelEstimatorExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-ascii ${EPSILON_3}
    ${BASELINE}/svm_image_model_generic_kernel.svn
    ${TEMP}/svm_image_model_generic_kernel.svn
  --ignore-lines-with 2 probA probB
  Execute $<TARGET_FILE:SVMGenericKernelImageModelEstimatorExample>
    ${INPUTDATA}/ROI_QB_MUL_1.png
    ${INPUTDATA}/ROI_mask_multi.png
    ${TEMP}/svm_image_model_generic_kernel.svn
)

# ------- SVMGenericKernelImageClassificationExampleTest----------

otb_add_test(NAME leTeSVMGenericKernelImageClassificationExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${EPSILON_3}
    ${BASELINE}/ROI_QB_MUL_GENERIC_KERNEL_IMAGE_CLASSIF.png
    ${TEMP}/ROI_QB_MUL_GENERIC_KERNEL_IMAGE_CLASSIF.png
  Execute $<TARGET_FILE:SVMGenericKernelImageClassificationExample>
    ${INPUTDATA}/ROI_QB_MUL_1.png
    ${TEMP}/ROI_QB_MUL_GENERIC_KERNEL_IMAGE_CLASSIF.png
    ${INPUTDATA}/svm_image_model_generic_kernel.svn
)
