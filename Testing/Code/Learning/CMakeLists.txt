
if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


set(LEARNING_TESTS1 ${CXX_TEST_PATH}/otbLearningTests1)
set(LEARNING_TESTS2 ${CXX_TEST_PATH}/otbLearningTests2)
set(LEARNING_TESTS3 ${CXX_TEST_PATH}/otbLearningTests3)
set(LEARNING_TESTS4 ${CXX_TEST_PATH}/otbLearningTests4)
set(LEARNING_TESTS5 ${CXX_TEST_PATH}/otbLearningTests5)
if(OTB_USE_OPENCV)
	set(LEARNING_TESTS6 ${CXX_TEST_PATH}/otbLearningTests6)
endif()	

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbLearningTESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::SVMModel   ------------------------------

add_test(leTuSVMModelNew ${LEARNING_TESTS1}
        otbSVMModelNew)

add_test(leTuSVMModelLoad ${LEARNING_TESTS1}
        otbSVMModelLoad
 ${INPUTDATA}/svm_model)

add_test(leTvSVMModelAccessor ${LEARNING_TESTS1}
--compare-ascii ${EPSILON_5}  ${BASELINE_FILES}/leSVMModelAccessor.txt
                        ${TEMP}/leSVMModelAccessor.txt
        otbSVMModelAccessor
 ${INPUTDATA}/svm_model
 ${TEMP}/leSVMModelAccessor.txt)


add_test(leTvSVMModelLoadSave ${LEARNING_TESTS1}
--compare-ascii ${NOTOL}  ${INPUTDATA}/svm_model
                        ${TEMP}/svmmodel_test
        otbSVMModelLoadSave
 ${INPUTDATA}/svm_model
 ${TEMP}/svmmodel_test)

add_test(leTvSVMModelWithGenericKeywordKernelLoadSave ${LEARNING_TESTS1}
--compare-ascii ${NOTOL}  ${BASELINE_FILES}/leTvsvmmodel_image_generic_version_test
                        ${TEMP}/leTvsvmmodel_image_generic_version_test
        otbSVMModelGenericKernelLoadSave
 ${INPUTDATA}/svm_model_image_generic_version
 ${TEMP}/leTvsvmmodel_image_generic_version_test)

add_test(leTvSVMModelKernelTypeGenericLoadSave ${LEARNING_TESTS1}
--compare-ascii ${NOTOL}  ${INPUTDATA}/svm_model_image_generic_kernel
                        ${TEMP}/leTvsvmmodel_image_generic_kernel_test
        otbSVMModelGenericKernelLoadSave
 ${INPUTDATA}/svm_model_image_generic_kernel
 ${TEMP}/leTvsvmmodel_image_generic_kernel_test)


# -------            otb::SVMImageModelEstimator   --------------------

add_test(leTuSVMImageModelEstimatorNew ${LEARNING_TESTS1}
        otbSVMImageModelEstimatorNew)


add_test(leTvSVMImageModelEstimatorTrain ${LEARNING_TESTS1}
--compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/leTvsvm_model_image
                        ${TEMP}/leTvsvm_model_image
        otbSVMImageModelEstimatorTrain
 ${INPUTDATA}/ROI_QB_MUL_4.tif
 ${INPUTDATA}/ROI_QB_MUL_4_train.c1.hdr
 ${TEMP}/leTvsvm_model_image
 0 )

add_test(leTvSVMImageModelEstimatorTrainWithOptimization ${LEARNING_TESTS1}
--compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/leTvsvm_model_image_opt
                        ${TEMP}/leTvsvm_model_image_opt
        otbSVMImageModelEstimatorTrain
 ${INPUTDATA}/ROI_QB_MUL_4.tif
 ${INPUTDATA}/ROI_QB_MUL_4_train.c1.hdr
 ${TEMP}/leTvsvm_model_image_opt
 1)

add_test(leTvSVMImageModelEstimatorTrainOneClass ${LEARNING_TESTS1}
--compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/leTvsvm_model_image_one_class
                        ${TEMP}/leTvsvm_model_image_one_class
        otbSVMImageModelEstimatorTrainOneClass
 ${INPUTDATA}/ROI_QB_MUL_4.tif
 ${INPUTDATA}/ROI_QB_MUL_4_train_one_class.tif
 ${TEMP}/leTvsvm_model_image_one_class)

add_test(leTvSVMImageModelEstimatorModelAccessor ${LEARNING_TESTS1}
--compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/leSVMModelEstimatorModelAccessor.txt
                        ${TEMP}/leSVMModelEstimatorModelAccessor.txt
        otbSVMImageModelEstimatorModelAccessor
 ${INPUTDATA}/ROI_QB_MUL_4.tif
 ${INPUTDATA}/ROI_QB_MUL_4_train.c1.hdr
 ${TEMP}/leSVMModelEstimatorModelAccessor.txt)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbLearningTESTS2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# -------            otb::SVMPointSetModelEstimator   --------------------

add_test(leTuSVMPointSetModelEstimatorNew ${LEARNING_TESTS2}
        otbSVMPointSetModelEstimatorNew)

add_test(leTvSVMPointSetModelEstimatorTrain ${LEARNING_TESTS2}
--compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/leTvsvm_model_pointset
                        ${TEMP}/leTvsvm_model_pointset
        otbSVMPointSetModelEstimatorTrain
 ${TEMP}/leTvsvm_model_pointset)

# -------            otb::SVMSampleListModelEstimator   --------------------

add_test(leTuSVMSampleListModelEstimatorNew ${LEARNING_TESTS2}
        otbSVMSampleListModelEstimatorNew)

add_test(leTvSVMSampleListModelEstimatorTest ${LEARNING_TESTS2}
 --compare-ascii ${EPSILON_9}
    ${BASELINE_FILES}/leTvSVMSampleListModelEstimatorModel.txt
    ${TEMP}/leTvSVMSampleListModelEstimatorModel.txt
        otbSVMSampleListModelEstimatorTest
    ${EXAMPLEDATA}/qb_RoadExtract.tif
    ${EXAMPLEDATA}/qb_RoadExtract_easyClassification.shp
    ${TEMP}/leTvSVMSampleListModelEstimatorModel.txt
)

# -------            otb::SVMClassifier   -----------------------------

add_test(leTuSVMClassifierNew ${LEARNING_TESTS2}
        otbSVMClassifierNew)

add_test(leTvSVMClassifierImage ${LEARNING_TESTS2}
--compare-image ${NOTOL}
  ${BASELINE}/leSVMClassifierImageOutput.png
  ${TEMP}/leSVMClassifierImageOutput.png
        otbSVMClassifierImage
 ${INPUTDATA}/ROI_QB_MUL_4.tif
 ${INPUTDATA}/svm_model_image
 ${TEMP}/leSVMClassifierImageOutput.png)

add_test(leTvSVMClassifierImageOneClass ${LEARNING_TESTS2}
--compare-image ${NOTOL}
  ${BASELINE}/leSVMClassifierImageOneClassOutput.png
  ${TEMP}/leSVMClassifierImageOneClassOutput.png
        otbSVMClassifierImage
 ${INPUTDATA}/ROI_QB_MUL_4.tif
 ${INPUTDATA}/svm_model_image_one_class
 ${TEMP}/leSVMClassifierImageOneClassOutput.png)

add_test(leTvSVMClassifierImageWithGenericKernalLinear ${LEARNING_TESTS2}
--compare-image ${NOTOL}  ${BASELINE}/leTvSVMClassifierImageWithGenericKernalLinearOutput.png
                        ${TEMP}/leTvSVMClassifierImageWithGenericKernalLinearOutput.png
 otbSVMClassifierImage
 ${INPUTDATA}/ROI_QB_MUL_4.tif
 ${INPUTDATA}/svm_model_image_generic_linear
 ${TEMP}/leTvSVMClassifierImageWithGenericKernalLinearOutput.png)



add_test(leTvSVMClassifierPointSet ${LEARNING_TESTS2}
        otbSVMClassifierPointSet
 ${INPUTDATA}/svm_model_pointset)


# -------            otb::SOMMap   -----------------------------

add_test(leTuSOMMapNew ${LEARNING_TESTS2}
        otbSOMMapNew)

add_test(leTvSOMMap ${LEARNING_TESTS2}
        otbSOMMap)

# -------            otb::SOM   -----------------------------

add_test(leTuSOMNew ${LEARNING_TESTS2}
        otbSOMNew)

add_test(leTvSOM ${LEARNING_TESTS2}
        --compare-image ${EPSILON_10}
 ${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
 ${TEMP}/leSOMPoupeesSubOutputMap1.hdr
 otbSOM
 ${INPUTDATA}/poupees_sub.png
 ${TEMP}/leSOMPoupeesSubOutputMap1.hdr
 32 32 10 10 5 1.0 0.1 0)

# -------            otb::PeriodicSOM   -----------------------------

add_test(leTuPeriodicSOMNew ${LEARNING_TESTS2}
        otbPeriodicSOMNew)

add_test(leTvPeriodicSOM ${LEARNING_TESTS2}
        --compare-image ${EPSILON_10}
 ${BASELINE}/lePeriodicSOMPoupeesSubOutputMap1.hdr
 ${TEMP}/lePeriodicSOMPoupeesSubOutputMap1.hdr
 otbPeriodicSOMTest
 ${INPUTDATA}/poupees_sub.png
 ${TEMP}/lePeriodicSOMPoupeesSubOutputMap1.hdr
 32 32 10 10 5 1.0 0.1 0)

# -------            otb::SOMActivationBuilder   -----------------------------

add_test(leTuSOMActivationBuilderNew ${LEARNING_TESTS2}
        otbSOMActivationBuilderNew)

add_test(leTvSOMActivationBuilder ${LEARNING_TESTS2}
        --compare-image ${NOTOL}
 ${BASELINE}/leSOMPoupeesSubActivationMap1.hdr
 ${TEMP}/leSOMPoupeesSubActivationMap1.hdr
 otbSOMActivationBuilder
 ${INPUTDATA}/poupees_sub.png
 ${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
 ${TEMP}/leSOMPoupeesSubActivationMap1.hdr
 32 32 10 10 5 1.0 0.1 128)

# -------            otb::SOMbasedImageFilter -----------------------------------

add_test(leTuSOMbasedImageFilterNew ${LEARNING_TESTS2}
         otbSOMbasedImageFilterNew )

add_test(leTvSOMbasedImageFilter ${LEARNING_TESTS2}
        --compare-image ${NOTOL}
 ${BASELINE}/leSOMbasedImageFilterOuptut.hdr
 ${TEMP}/leSOMbasedImageFilterOuptut.hdr
 otbSOMbasedImageFilterTest
 ${INPUTDATA}/poupees_sub.png
 ${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
 ${TEMP}/leSOMbasedImageFilterOuptut.hdr
 )


# ------             otb::SOMWithMissingValue -----------------------------------

add_test(leTuSOMWithMissingValueNew ${LEARNING_TESTS2}
         otbSOMWithMissingValueNew )

add_test(leTvSOMWithMissingValue ${LEARNING_TESTS2}
        --compare-image ${EPSILON_10}
 ${BASELINE}/leSOMWithMissingValue.hdr
 ${TEMP}/leSOMWithMissingValue.hdr
 otbSOMWithMissingValueTest
 ${INPUTDATA}/poupees_sub.png
 ${TEMP}/leSOMWithMissingValue.hdr
 32 32 10 10 5 1.0 0.1 0)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbLearningTESTS3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::SOMClassifier   -----------------------------

add_test(leTuSOMClassifierNew ${LEARNING_TESTS3}
        otbSOMClassifierNew)

add_test(leTvSOMClassifier ${LEARNING_TESTS3}
        --compare-image ${NOTOL}
 ${BASELINE}/leSOMPoupeesClassified.hdr
 ${TEMP}/leSOMPoupeesClassified.hdr
 otbSOMClassifier
 ${INPUTDATA}/poupees_sub.png
 ${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
 ${TEMP}/leSOMPoupeesClassified.hdr
 )

# -------            otb::SOMImageClassificationFilter   -----------------------------

add_test(leTuSOMImageClassificationFilterNew ${LEARNING_TESTS3}
        otbSOMImageClassificationFilterNew)

add_test(leTvSOMImageClassificationFilter ${LEARNING_TESTS3}
        --compare-image ${NOTOL}
 ${BASELINE}/leSOMPoupeesClassified.hdr
 ${TEMP}/leSOMImageClassificationFilterOuptut.hdr
 otbSOMImageClassificationFilter
 ${INPUTDATA}/poupees_sub.png
 ${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
 ${TEMP}/leSOMImageClassificationFilterOuptut.hdr
 )


# -------            otb::KMeansImageClassificationFilter   -----------------------------

add_test(leTuKMeansImageClassificationFilterNew ${LEARNING_TESTS3}
        otbKMeansImageClassificationFilterNew)

add_test(leTvKMeansImageClassificationFilter ${LEARNING_TESTS3}
        --compare-image ${NOTOL}
 ${BASELINE}/leKMeansImageClassificationFilterOuptut.hdr
 ${TEMP}/leKMeansImageClassificationFilterOuptut.hdr
 otbKMeansImageClassificationFilter
 ${INPUTDATA}/poupees_sub.png
 ${TEMP}/leKMeansImageClassificationFilterOuptut.hdr
 2
 0 0 0 0
 255 255 255 255
 )

# -------            otb::SEMClassifier   -----------------------------

add_test(leTuSEMClassifierNew ${LEARNING_TESTS3}
        otbSEMClassifierNew)


# ------- otb::SVMImageClassificationFilter ---------------------------

add_test(leTuSVMImageClassificationFilterNew ${LEARNING_TESTS3}
 otbSVMImageClassificationFilterNew)

add_test(leTvSVMImageClassificationFilter ${LEARNING_TESTS3}
 --compare-image ${NOTOL}
 ${BASELINE}/leSVMImageClassificationFilterOutput.tif
 ${TEMP}/leSVMImageClassificationFilterOutput.tif
 otbSVMImageClassificationFilter
 ${INPUTDATA}/ROI_QB_MUL_4.tif
 ${INPUTDATA}/svm_model_image
 ${TEMP}/leSVMImageClassificationFilterOutput.tif)


# ------- otb::SVMImageClassificationFilterWithNeighborhoodMajorityVoting ---------------------------

add_test(leTvSVMImageClassificationFilterWithNeighborhoodMajorityVoting ${LEARNING_TESTS5}
 --compare-image ${NOTOL}
 ${BASELINE}/leSVMImageClassificationWithNMVFilterOutput.tif
 ${TEMP}/leSVMImageClassificationWithNMVFilterOutput.tif
otbNeighborhoodMajorityVotingImageFilterTest
 ${TEMP}/leSVMImageClassificationFilterOutput.tif
 ${TEMP}/leSVMImageClassificationWithNMVFilterOutput.tif
 false
 )

set_tests_properties(leTvSVMImageClassificationFilterWithNeighborhoodMajorityVoting PROPERTIES DEPENDS leTvSVMImageClassificationFilter)


# ------- otb::SVMImageClassificationWithRuleFilter ----------------------

add_test(leTuSVMImageClassificationWithRuleFilterNew ${LEARNING_TESTS3}
          otbSVMImageClassificationWithRuleFilterNew)

add_test(leTvSVMImageClassificationWithRuleFilter ${LEARNING_TESTS3}
 --compare-image ${NOTOL}
 ${BASELINE}/leSVMImageClassificationWithRuleFilterOutput.tif
 ${TEMP}/leSVMImageClassificationWithRuleFilterOutput.tif
 otbSVMImageClassificationWithRuleFilter
 ${INPUTDATA}/ROI_QB_MUL_4.tif
 ${INPUTDATA}/svm_model_image
 ${TEMP}/leSVMImageClassificationWithRuleFilterOutput.tif)



add_test(leTvSVMModelGenericKernelsTest ${LEARNING_TESTS3}
    --compare-n-ascii ${NOTOL} 17
        ${BASELINE_FILES}/leTvSVMModelGenericKernelsTest1.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest1.txt
        ${BASELINE_FILES}/leTvSVMModelGenericKernelsTest2.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest2.txt
        ${BASELINE_FILES}/leTvSVMModelGenericKernelsTest3.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest3.txt
        ${BASELINE_FILES}/leTvSVMModelGenericKernelsTest4.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest4.txt
        ${BASELINE_FILES}/leTvSVMModelGenericKernelsTest5.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest5.txt
        ${BASELINE_FILES}/leTvSVMModelGenericKernelsTest6.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest6.txt
        ${BASELINE_FILES}/leTvSVMModelGenericKernelsTest7.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest7.txt
        ${BASELINE_FILES}/leTvSVMModelGenericKernelsTest8.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest8.txt
        ${BASELINE_FILES}/leTvSVMModelGenericKernelsTest9.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest9.txt
        ${BASELINE_FILES}/leTvSVMModelGenericKernelsTest10.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest10.txt
        ${BASELINE_FILES}/leTvSVMModelGenericKernelsTest11.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest11.txt
        ${BASELINE_FILES}/leTvSVMModelGenericKernelsTest12.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest12.txt
        ${BASELINE_FILES}/leTvSVMModelGenericKernelsTest13.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest13.txt
        ${BASELINE_FILES}/leTvSVMModelGenericKernelsTest14.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest14.txt
        ${BASELINE_FILES}/leTvSVMModelGenericKernelsTest15.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest15.txt
        ${BASELINE_FILES}/leTvSVMModelGenericKernelsTest16.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest16.txt
        ${BASELINE_FILES}/leTvSVMModelGenericKernelsTest17.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest17.txt
 otbSVMModelGenericKernelsTest
        ${TEMP}/leTvSVMModelGenericKernelsTest1.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest2.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest3.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest4.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest5.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest6.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest7.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest8.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest9.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest10.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest11.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest12.txt
        ${TEMP}/leTvSVMModelGenericKernelsTest13.txt
 ${TEMP}/leTvSVMModelGenericKernelsTest14.txt
 ${TEMP}/leTvSVMModelGenericKernelsTest15.txt
 ${TEMP}/leTvSVMModelGenericKernelsTest16.txt
    ${TEMP}/leTvSVMModelGenericKernelsTest17.txt
    )


foreach(kernelType RANGE 0 18)
  add_test(leTvSVMKernelTest${kernelType} ${LEARNING_TESTS3}
   --compare-ascii ${EPSILON_9}
      ${BASELINE_FILES}/leTvSVMKernelTest${kernelType}.txt
      ${TEMP}/leTvSVMKernelTest${kernelType}.txt
        otbSVMKernelsTest
      ${EXAMPLEDATA}/qb_RoadExtract.tif
      ${EXAMPLEDATA}/qb_RoadExtract_easyClassification.shp
      ${TEMP}/leTvSVMKernelTest${kernelType}.txt
      ${kernelType}
  )
endforeach()

add_test(leTvSVMModelCopy ${LEARNING_TESTS3}
--compare-ascii ${NOTOL}  ${INPUTDATA}/svm_model
                        ${TEMP}/svmcopymodel_test
        otbSVMModelCopyTest
 ${INPUTDATA}/svm_model
 ${TEMP}/svmcopymodel_test)

add_test(leTvSVMModelCopyGenericKernel ${LEARNING_TESTS3}
--compare-ascii ${NOTOL}  ${INPUTDATA}/svm_model_generic
                        ${TEMP}/svmcopygeneric_test
        otbSVMModelCopyGenericKernelTest
 ${INPUTDATA}/svm_model_generic
 ${TEMP}/svmcopygeneric_test)

add_test(leTvSVMModelCopyComposedKernel ${LEARNING_TESTS3}
--compare-ascii ${NOTOL}  ${INPUTDATA}/svm_model_composed
                        ${TEMP}/svmcopycomposed_test
        otbSVMModelCopyComposedKernelTest
 ${INPUTDATA}/svm_model_composed
 ${TEMP}/svmcopycomposed_test)

add_test(leTvSVMInverseCosSpectralAngleKernelFunctorImageModelEstimatorTest ${LEARNING_TESTS3}
        otbSVMInverseCosSpectralAngleKernelFunctorImageModelEstimatorTest
 ${INPUTDATA}/ROI_QB_MUL_4.tif
 ${INPUTDATA}/ROI_QB_MUL_4_train_one_class.tif
 ${TEMP}/leTvsvm_model_image_one_class)

add_test(leTvSVMInverseCosSpectralAngleKernelFunctorImageClassificationTest ${LEARNING_TESTS3}
        otbSVMInverseCosSpectralAngleKernelFunctorImageClassificationTest
 ${INPUTDATA}/ROI_QB_MUL_4.tif
 ${INPUTDATA}/ROI_QB_MUL_4_train_one_class.tif
 ${TEMP}/leTvsvm_model_image_one_class)

# Test SVMMargin Sampler
add_test(leTuSVMMarginSamplerNew ${LEARNING_TESTS3}
        otbSVMMarginSamplerNew)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbLearningTESTS4 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::RoidataConversion   -----------------------------
add_test(leTuROIdataConversionNew ${LEARNING_TESTS4}
        otbROIdataConversionNew)

add_test(leTvROIdataConversion ${LEARNING_TESTS4}
 --compare-ascii ${NOTOL}
 ${BASELINE_FILES}/leTvROIdataConversion.txt
 ${TEMP}/leTvROIdataConversion.txt
        otbROIdataConversion
 ${INPUTDATA}/ImageLineDir.bsq.hdr
 ${INPUTDATA}/ImageLine.bsq.hdr
 ${TEMP}/leTvROIdataConversion.txt
)


add_test(leTuSVMCrossValidationCostFunctionNew ${LEARNING_TESTS4}
        otbSVMCrossValidationCostFunctionNew)

add_test(leTuExhaustiveExponentialOptimizerNew ${LEARNING_TESTS4}
        otbExhaustiveExponentialOptimizerNew)

add_test(leTvExhaustiveExponentialOptimizerTest ${LEARNING_TESTS4}
        --compare-ascii ${NOTOL}
          ${BASELINE_FILES}/leTvExhaustiveExponentialOptimizerOutput.txt
          ${TEMP}/leTvExhaustiveExponentialOptimizerTestOutput.txt
        otbExhaustiveExponentialOptimizerTest
          ${TEMP}/leTvExhaustiveExponentialOptimizerTestOutput.txt)

# -------            otb::ListSampleGenerator   -----------------------------
add_test(leTuListSampleGeneratorNew ${LEARNING_TESTS4}
        otbListSampleGeneratorNew)

add_test(leTvListSampleGenerator1 ${LEARNING_TESTS4}
 --compare-n-ascii ${NOTOL} 2
    ${BASELINE_FILES}/leTvListSampleGenerator1.txt
    ${TEMP}/leTvListSampleGenerator1.txt
    ${BASELINE_FILES}/leTvListSampleGeneratorValidation1.txt
    ${TEMP}/leTvListSampleGeneratorValidation1.txt
        otbListSampleGenerator
    ${EXAMPLEDATA}/qb_RoadExtract.tif
    ${EXAMPLEDATA}/qb_RoadExtract_classification.shp
    ${TEMP}/leTvListSampleGenerator1.txt
    ${TEMP}/leTvListSampleGeneratorValidation1.txt
    -1
    -1
    0
)

# test the proportion between training and validation
add_test(leTvListSampleGenerator2 ${LEARNING_TESTS4}
 --compare-n-ascii ${NOTOL} 2
    ${BASELINE_FILES}/leTvListSampleGenerator2.txt
    ${TEMP}/leTvListSampleGenerator2.txt
    ${BASELINE_FILES}/leTvListSampleGeneratorValidation2.txt
    ${TEMP}/leTvListSampleGeneratorValidation2.txt
        otbListSampleGenerator
    ${EXAMPLEDATA}/qb_RoadExtract.tif
    ${EXAMPLEDATA}/qb_RoadExtract_classification.shp
    ${TEMP}/leTvListSampleGenerator2.txt
    ${TEMP}/leTvListSampleGeneratorValidation2.txt
    -1
    -1
    0.5
)

# test the size limit for training and validation
add_test(leTvListSampleGenerator3 ${LEARNING_TESTS4}
 --compare-n-ascii ${NOTOL} 2
    ${BASELINE_FILES}/leTvListSampleGenerator3.txt
    ${TEMP}/leTvListSampleGenerator3.txt
    ${BASELINE_FILES}/leTvListSampleGeneratorValidation3.txt
    ${TEMP}/leTvListSampleGeneratorValidation3.txt
        otbListSampleGenerator
    ${EXAMPLEDATA}/qb_RoadExtract.tif
    ${EXAMPLEDATA}/qb_RoadExtract_classification.shp
    ${TEMP}/leTvListSampleGenerator3.txt
    ${TEMP}/leTvListSampleGeneratorValidation3.txt
    100
    200
    0.5
)
# -------            otb::ConfusionMatrixCalculator   -----------------------------
add_test(leTuConfusionMatrixCalculatorNew ${LEARNING_TESTS4}
otbConfusionMatrixCalculatorNew)

add_test(leTvConfusionMatrixCalculatorSetListSamples ${LEARNING_TESTS4}
otbConfusionMatrixCalculatorSetListSamples 1000 4)

add_test(leTvConfusionMatrixCalculatorWrongSize ${LEARNING_TESTS4}
otbConfusionMatrixCalculatorWrongSize 1000 4)
set_tests_properties(leTvConfusionMatrixCalculatorWrongSize PROPERTIES WILL_FAIL true)

add_test(leTvConfusionMatrixCalculatorUpdate ${LEARNING_TESTS4}
otbConfusionMatrixCalculatorCompute 4 3)

add_test(leTvConfusionMatrixCalculatorUpdateWithBaseline ${LEARNING_TESTS4}
otbConfusionMatrixCalculatorComputeWithBaseline)

# -------            otb::ConfusionMatrixMeasurements   -----------------------------
add_test(leTuConfusionMatrixMeasurementsNew ${LEARNING_TESTS4}
otbConfusionMatrixMeasurementsNew)

add_test(leTvConfusionMatrixMeasurementsTest ${LEARNING_TESTS4}
otbConfusionMatrixMeasurementsTest)

add_test(leTvConfusionMatrixConcatenateTest ${LEARNING_TESTS4}
otbConfusionMatrixConcatenateTest
 ${INPUTDATA}/Classification/QB_1_ortho_C1.csv
 ${INPUTDATA}/Classification/QB_1_ortho_C2.csv
 ${INPUTDATA}/Classification/QB_1_ortho_C3.csv
 ${INPUTDATA}/Classification/QB_1_ortho_C4.csv
 ${INPUTDATA}/Classification/QB_1_ortho_C5.csv
 ${INPUTDATA}/Classification/QB_1_ortho_C6.csv)



# otbGaussianModelComponent
add_test(leTuGaussianModelComponentNew ${LEARNING_TESTS4}
otbGaussianModelComponentNew)


# ShiftScaleSampleListFilter tests ----------
add_test(leTuShiftScaleSampleListFilterNew ${LEARNING_TESTS4}
otbShiftScaleSampleListFilterNew)

add_test(leTvShiftScaleSampleListFilter ${LEARNING_TESTS4}
--compare-ascii ${NOTOL}
 ${BASELINE_FILES}/leTvShiftScaleSampleListFilterOutput.txt
 ${TEMP}/leTvShiftScaleSampleListFilterOutput.txt
otbShiftScaleSampleListFilter
${TEMP}/leTvShiftScaleSampleListFilterOutput.txt
 2
-1 -3
 1  2
-2 -5
-1 -3
 0 -1
)
# Gaussian Additive
add_test(leTuGaussianAdditiveNoiseSampleListFilterNew ${LEARNING_TESTS4}
otbGaussianAdditiveNoiseSampleListFilterNew)

add_test(leTvGaussianAdditiveNoiseSampleListFilter ${LEARNING_TESTS4}
--compare-ascii ${NOTOL}
        ${BASELINE_FILES}/leTvGaussianAdditiveNoiseSampleListFilterOutput.txt
        ${TEMP}/leTvGaussianAdditiveNoiseSampleListFilterOutput.txt
otbGaussianAdditiveNoiseSampleListFilter
${TEMP}/leTvGaussianAdditiveNoiseSampleListFilterOutput.txt
 2
-1 -3
 1  2
-2 -5
-1 -3
 0 -1
)

# ConcatenateSampleListFilter tests ----------
add_test(leTuConcatenateSampleListFilterNew ${LEARNING_TESTS4}
otbConcatenateSampleListFilterNew)

add_test(leTvConcatenateSampleListFilter ${LEARNING_TESTS4}
--compare-ascii ${NOTOL}
 ${BASELINE_FILES}/leTvConcatenateSampleListFilterOutput.txt
 ${TEMP}/leTvConcatenateSampleListFilterOutput.txt
otbConcatenateSampleListFilter
${TEMP}/leTvConcatenateSampleListFilterOutput.txt
 2 3 2
-1 -3
 1  2
-2 -5
-1 -3
 0 -1
)

#ListSampleToBalancedListSampleFilterNew tests ----------
add_test(leTuListSampleToBalancedListSampleFilterNew ${LEARNING_TESTS4}
otbListSampleToBalancedListSampleFilterNew)

add_test(leTvListSampleToBalancedListSampleFilter ${LEARNING_TESTS4}
--compare-ascii ${NOTOL}
${BASELINE_FILES}/leTvListSampleToBalancedListSampleFilterOutput.txt
 ${TEMP}/leTvListSampleToBalancedListSampleFilterOutput.txt
otbListSampleToBalancedListSampleFilter
${TEMP}/leTvListSampleToBalancedListSampleFilterOutput.txt
 2
-1 -3  0 # The third element is the label of the SampleList
 1  2  1
-2 -5  0
-1 -3  1
 0 -1  1
-3  1  1
-5  2  1
 2  1  1
 2  8  1
 1 -4  0
-1  5  4
 2  5  1
 0 -5  0
 1 -1  2
)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbLearningTESTS5 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::DecisionTree   ------------------------------
add_test(leTuDecisionTreeNew ${LEARNING_TESTS5}
otbDecisionTreeNew)

add_test(leTvDecisionTreeBuild ${LEARNING_TESTS5}
otbDecisionTreeBuild)

add_test(leTvDecisionTreeWithRealValues ${LEARNING_TESTS5}
otbDecisionTreeWithRealValues)


add_test(leTuNeighborhoodMajorityVotingImageFilterNew ${LEARNING_TESTS5}
otbNeighborhoodMajorityVotingImageFilterNew)

add_test(leTvNeighborhoodMajorityVotingImageFilterTest ${LEARNING_TESTS5}
 --compare-image ${NOTOL}
 ${BASELINE}/QB_1_ortho_4Cls_N_Classified_OTB_NMV_x2_y5_nodv10_undv7.tif
 ${TEMP}/QB_1_ortho_4Cls_N_Classified_OTB_NMV_x2_y5_nodv10_undv7.tif
otbNeighborhoodMajorityVotingImageFilterTest
 ${INPUTDATA}/Classification/QB_1_ortho_4Cls_N_Classified_OTB.tif
 ${TEMP}/QB_1_ortho_4Cls_N_Classified_OTB_NMV_x2_y5_nodv10_undv7.tif
 false #KeepOriginalLabelBool
 2 #xRadius
 5 #yRadius
 10 #LabelForNoDataPixels
 7 #LabelForUndecidedPixels
 )


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbLearningTESTS6 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
if(OTB_USE_OPENCV)
	add_test(leTuRandomForestsMachineLearningModelNew ${LEARNING_TESTS6}
           otbRandomForestsMachineLearningModelNew)

	add_test(leTvRandomForestsMachineLearningModel ${LEARNING_TESTS6}
         otbRandomForestsMachineLearningModel
         ${INPUTDATA}/letter.scale
         ${TEMP}/rf_model.txt
         )

	add_test(leTuKNearestNeighborsMachineLearningModelNew ${LEARNING_TESTS6}
           otbKNearestNeighborsMachineLearningModelNew)

	add_test(leTvKNearestNeighborsMachineLearningModel ${LEARNING_TESTS6}
         otbKNearestNeighborsMachineLearningModel
         ${INPUTDATA}/letter.scale
         ${TEMP}/knn_model.txt
         )

	add_test(leTuSVMMachineLearningModelNew ${LEARNING_TESTS6}
           otbSVMMachineLearningModelNew)

	add_test(leTvSVMMachineLearningModel ${LEARNING_TESTS6}
         otbSVMMachineLearningModel
         ${INPUTDATA}/letter.scale
         ${TEMP}/svm_model.txt
         )

	add_test(leTuLibSVMMachineLearningModelNew ${LEARNING_TESTS6}
       	 otbLibSVMMachineLearningModelNew)

	add_test(leTvLibSVMMachineLearningModel ${LEARNING_TESTS6}
	     otbLibSVMMachineLearningModel
	     ${INPUTDATA}/letter.scale
	     ${TEMP}/libsvm_model.txt
     	 )

    add_test(leTuBoostMachineLearningModelNew ${LEARNING_TESTS6}
           otbBoostMachineLearningModelNew)

	add_test(leTvBoostMachineLearningModel ${LEARNING_TESTS6}
         otbBoostMachineLearningModel
         ${INPUTDATA}/letter.scale
         ${TEMP}/boost_model.txt
         )

    add_test(leTuANNMachineLearningModelNew ${LEARNING_TESTS6}
         otbANNMachineLearningModelNew)

	add_test(leTvANNMachineLearningModel ${LEARNING_TESTS6}
         otbANNMachineLearningModel
         ${INPUTDATA}/letter.scale
         ${TEMP}/ann_model.txt
         )

     add_test(leTuNormalBayesMachineLearningModelNew ${LEARNING_TESTS6}
         otbNormalBayesMachineLearningModelNew)

     add_test(leTvNormalBayesMachineLearningModel ${LEARNING_TESTS6}
         otbNormalBayesMachineLearningModel
         ${INPUTDATA}/letter.scale
         ${TEMP}/normalbayes_model.txt
         )

     add_test(leTuDecisionTreeMachineLearningModelNew ${LEARNING_TESTS6}
         otbDecisionTreeMachineLearningModelNew)

     add_test(leTvDecisionTreeMachineLearningModel ${LEARNING_TESTS6}
         otbDecisionTreeMachineLearningModel
         ${INPUTDATA}/letter.scale
         ${TEMP}/decisiontree_model.txt
         )

     add_test(leTuGradientBoostedTreeMachineLearningModelNew ${LEARNING_TESTS6}
         otbGradientBoostedTreeMachineLearningModelNew)

     add_test(leTvGradientBoostedTreeMachineLearningModel ${LEARNING_TESTS6}
         otbGradientBoostedTreeMachineLearningModel
         ${INPUTDATA}/letter.scale
         ${TEMP}/gbt_model.txt
         )

     add_test(leTuImageClassificationFilterNew ${LEARNING_TESTS6}
       	 otbImageClassificationFilterNew)
       	
     add_test(leTvImageClassificationFilterLibSVM ${LEARNING_TESTS6}
     	--compare-image ${NOTOL}
       	 ${BASELINE}/leSVMImageClassificationFilterOutput.tif
		 ${TEMP}/leImageClassificationFilterLibSVMOutput.tif
		 otbImageClassificationFilter
		 ${INPUTDATA}/ROI_QB_MUL_4.tif
		 ${INPUTDATA}/svm_model_image
		 ${TEMP}/leImageClassificationFilterLibSVMOutput.tif
       	 )
       	
     add_test(leTvImageClassificationFilterSVM ${LEARNING_TESTS6}
     	--compare-image ${NOTOL}
       	 ${BASELINE}/leImageClassificationFilterSVMOutput.tif
		 ${TEMP}/leImageClassificationFilterSVMOutput.tif
		 otbImageClassificationFilter
		 ${INPUTDATA}/ROI_QB_MUL_4.tif
		 ${INPUTDATA}/ROI_QB_MUL_4_svmModel.txt
		 ${TEMP}/leImageClassificationFilterSVMOutput.tif
       	 )
       	
     add_test(leTuLibSVMMachineLearningModelCanRead ${LEARNING_TESTS6}
       	 otbLibSVMMachineLearningModelCanRead
       	 ${TEMP}/libsvm_model.txt
       	 )
     set_tests_properties(leTuLibSVMMachineLearningModelCanRead
     					PROPERTIES DEPENDS leTvLibSVMMachineLearningModel)
       	
     add_test(leTuSVMMachineLearningModelCanRead ${LEARNING_TESTS6}
       	 otbSVMMachineLearningModelCanRead
       	 ${TEMP}/svm_model.txt
       	 )
     set_tests_properties(leTuSVMMachineLearningModelCanRead
     					PROPERTIES DEPENDS leTvSVMMachineLearningModel)
     					
     add_test(leTuRandomForestsMachineLearningModelCanRead ${LEARNING_TESTS6}
       	 otbRandomForestsMachineLearningModelCanRead
       	 ${TEMP}/rf_model.txt
       	 )
     set_tests_properties(leTuRandomForestsMachineLearningModelCanRead
     					PROPERTIES DEPENDS leTvRandomForestsMachineLearningModel)
     					
     add_test(leTuBoostMachineLearningModelCanRead ${LEARNING_TESTS6}
       	 otbBoostMachineLearningModelCanRead
       	 ${TEMP}/boost_model.txt
       	 )
     set_tests_properties(leTuBoostMachineLearningModelCanRead
     					PROPERTIES DEPENDS leTvBoostMachineLearningModel)
     	       	
     add_test(leTuANNMachineLearningModelCanRead ${LEARNING_TESTS6}
       	 otbNeuralNetworkMachineLearningModelCanRead
       	 ${TEMP}/ann_model.txt
       	 )
     set_tests_properties(leTuANNMachineLearningModelCanRead
     					PROPERTIES DEPENDS leTvANNMachineLearningModel)
     					      	
     add_test(leTuNormalBayesMachineLearningModelCanRead ${LEARNING_TESTS6}
       	 otbNormalBayesMachineLearningModelCanRead
       	 ${TEMP}/normalbayes_model.txt
       	 )
     set_tests_properties(leTuNormalBayesMachineLearningModelCanRead
     					PROPERTIES DEPENDS leTvNormalBayesMachineLearningModel)
     					       	
	add_test(leTuDecisionTreeMachineLearningModelCanRead ${LEARNING_TESTS6}
       	 otbDecisionTreeMachineLearningModelCanRead
       	 ${TEMP}/decisiontree_model.txt
       	 )
     set_tests_properties(leTuDecisionTreeMachineLearningModelCanRead
     					PROPERTIES DEPENDS leTvDecisionTreeMachineLearningModel)
     					       	
    add_test(leTuGradientBoostedTreeMachineLearningModelCanRead ${LEARNING_TESTS6}
       	 otbGradientBoostedTreeMachineLearningModelCanRead
       	 ${TEMP}/gbt_model.txt
       	 )
    set_tests_properties(leTuGradientBoostedTreeMachineLearningModelCanRead
     					PROPERTIES DEPENDS leTvGradientBoostedTreeMachineLearningModel)

    add_test(leTuKNNMachineLearningModelCanRead ${LEARNING_TESTS6}
       	 otbKNNMachineLearningModelCanRead
       	 ${TEMP}/knn_model.txt
       	 )
    set_tests_properties(leTuKNNMachineLearningModelCanRead
     					PROPERTIES DEPENDS leTvKNearestNeighborsMachineLearningModel)
     					
endif()

# Testing srcs
set(BasicLearning_SRCS1
otbLearningTests1.cxx
otbSVMModelNew.cxx
otbSVMModelLoad.cxx
otbSVMModelAccessor.cxx
otbSVMModelLoadSave.cxx
otbSVMModelGenericKernelLoadSave.cxx
otbSVMImageModelEstimatorNew.cxx
otbSVMImageModelEstimatorTrain.cxx
otbSVMImageModelEstimatorTrainOneClass.cxx
otbSVMImageModelEstimatorModelAccessor.cxx
)
set(BasicLearning_SRCS2
otbLearningTests2.cxx
otbSVMPointSetModelEstimatorNew.cxx
otbSVMPointSetModelEstimatorTrain.cxx
otbSVMSampleListModelEstimatorTest.cxx
otbSVMClassifierNew.cxx
otbSVMClassifierImage.cxx
otbSVMClassifierPointSet.cxx
otbSOMMapNew.cxx
otbSOMMap.cxx
otbSOMNew.cxx
otbSOM.cxx
otbPeriodicSOMNew.cxx
otbPeriodicSOM.cxx
otbSOMActivationBuilderNew.cxx
otbSOMActivationBuilder.cxx
otbSOMbasedImageFilterNew.cxx
otbSOMbasedImageFilter.cxx
otbSOMWithMissingValueNew.cxx
otbSOMWithMissingValue.cxx
)
set(BasicLearning_SRCS3
otbLearningTests3.cxx
otbSOMClassifierNew.cxx
otbSOMClassifier.cxx
otbSOMImageClassificationFilterNew.cxx
otbSOMImageClassificationFilter.cxx
otbKMeansImageClassificationFilterNew.cxx
otbKMeansImageClassificationFilter.cxx
otbSEMClassifierNew.cxx
otbSVMImageClassificationFilterNew.cxx
otbSVMImageClassificationFilter.cxx
otbSVMImageClassificationWithRuleFilterNew.cxx
otbSVMImageClassificationWithRuleFilter.cxx
otbSVMModelGenericKernelsTest.cxx
otbSVMModelCopyTest.cxx
otbSVMModelCopyGenericKernelTest.cxx
otbSVMModelCopyComposedKernelTest.cxx
otbSVMInverseCosSpectralAngleKernelFunctorImageModelEstimatorTest.cxx
otbSVMInverseCosSpectralAngleKernelFunctorImageClassificationTest.cxx
otbSVMMarginSampler.cxx
)


set(BasicLearning_SRCS4
otbLearningTests4.cxx
otbROIdataConversionNew.cxx
otbROIdataConversion.cxx
otbSVMCrossValidationCostFunctionNew.cxx
otbExhaustiveExponentialOptimizerNew.cxx
otbExhaustiveExponentialOptimizerTest.cxx
otbListSampleGeneratorTest.cxx
otbConfusionMatrixCalculatorTest.cxx
otbConfusionMatrixMeasurementsTest.cxx
otbGaussianModelComponent.cxx
otbShiftScaleSampleListFilter.cxx
otbGaussianAdditiveNoiseSampleListFilter.cxx
otbConcatenateSampleListFilter.cxx
otbListSampleToBalancedListSampleFilter.cxx
)

set(BasicLearning_SRCS5
otbLearningTests5.cxx
otbDecisionTreeNew.cxx
otbDecisionTreeBuild.cxx
otbDecisionTreeWithRealValues.cxx
otbNeighborhoodMajorityVotingImageFilterNew.cxx
otbNeighborhoodMajorityVotingImageFilterTest.cxx
)

if(OTB_USE_OPENCV)
	set(BasicLearning_SRCS6
	otbLearningTests6.cxx
	otbTrainMachineLearningModel.cxx
	otbImageClassificationFilter.cxx
	otbMachineLearningModelCanRead.cxx
	)
endif()

OTB_ADD_EXECUTABLE(otbLearningTests1 "${BasicLearning_SRCS1}" "OTBLearning;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbLearningTests2 "${BasicLearning_SRCS2}" "OTBLearning;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbLearningTests3 "${BasicLearning_SRCS3}" "OTBLearning;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbLearningTests4 "${BasicLearning_SRCS4}" "OTBLearning;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbLearningTests5 "${BasicLearning_SRCS5}" "OTBLearning;OTBIO;OTBTesting")

if(OTB_USE_OPENCV)
	OTB_ADD_EXECUTABLE(otbLearningTests6 "${BasicLearning_SRCS6}" "OTBLearning;OTBIO;OTBTesting")
endif()


endif()




