
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


SET(LEARNING_TESTS1 ${CXX_TEST_PATH}/otbLearningTests1)
SET(LEARNING_TESTS2 ${CXX_TEST_PATH}/otbLearningTests2)
SET(LEARNING_TESTS3 ${CXX_TEST_PATH}/otbLearningTests3)
SET(LEARNING_TESTS4 ${CXX_TEST_PATH}/otbLearningTests4)
SET(LEARNING_TESTS5 ${CXX_TEST_PATH}/otbLearningTests5)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbLearningTESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::SVMModel   ------------------------------

ADD_TEST(leTuSVMModelNew ${LEARNING_TESTS1}
        otbSVMModelNew)

ADD_TEST(leTuSVMModelLoad ${LEARNING_TESTS1}
        otbSVMModelLoad
 ${INPUTDATA}/svm_model)

ADD_TEST(leTvSVMModelAccessor ${LEARNING_TESTS1}
--compare-ascii ${EPSILON_5}  ${BASELINE_FILES}/leSVMModelAccessor.txt
                        ${TEMP}/leSVMModelAccessor.txt
        otbSVMModelAccessor
 ${INPUTDATA}/svm_model
 ${TEMP}/leSVMModelAccessor.txt)


ADD_TEST(leTvSVMModelLoadSave ${LEARNING_TESTS1}
--compare-ascii ${NOTOL}  ${INPUTDATA}/svm_model
                        ${TEMP}/svmmodel_test
        otbSVMModelLoadSave
 ${INPUTDATA}/svm_model
 ${TEMP}/svmmodel_test)

ADD_TEST(leTvSVMModelWithGenericKeywordKernelLoadSave ${LEARNING_TESTS1}
--compare-ascii ${NOTOL}  ${BASELINE_FILES}/leTvsvmmodel_image_generic_version_test
                        ${TEMP}/leTvsvmmodel_image_generic_version_test
        otbSVMModelGenericKernelLoadSave
 ${INPUTDATA}/svm_model_image_generic_version
 ${TEMP}/leTvsvmmodel_image_generic_version_test)

ADD_TEST(leTvSVMModelKernelTypeGenericLoadSave ${LEARNING_TESTS1}
--compare-ascii ${NOTOL}  ${INPUTDATA}/svm_model_image_generic_kernel
                        ${TEMP}/leTvsvmmodel_image_generic_kernel_test
        otbSVMModelGenericKernelLoadSave
 ${INPUTDATA}/svm_model_image_generic_kernel
 ${TEMP}/leTvsvmmodel_image_generic_kernel_test)


# -------            otb::SVMImageModelEstimator   --------------------

ADD_TEST(leTuSVMImageModelEstimatorNew ${LEARNING_TESTS1}
        otbSVMImageModelEstimatorNew)


ADD_TEST(leTvSVMImageModelEstimatorTrain ${LEARNING_TESTS1}
--compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/leTvsvm_model_image
                        ${TEMP}/leTvsvm_model_image
        otbSVMImageModelEstimatorTrain
 ${INPUTDATA}/ROI_QB_MUL_4.tif
 ${INPUTDATA}/ROI_QB_MUL_4_train.c1.hdr
 ${TEMP}/leTvsvm_model_image
 0 )

ADD_TEST(leTvSVMImageModelEstimatorTrainWithOptimization ${LEARNING_TESTS1}
--compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/leTvsvm_model_image_opt
                        ${TEMP}/leTvsvm_model_image_opt
        otbSVMImageModelEstimatorTrain
 ${INPUTDATA}/ROI_QB_MUL_4.tif
 ${INPUTDATA}/ROI_QB_MUL_4_train.c1.hdr
 ${TEMP}/leTvsvm_model_image_opt
 1)

ADD_TEST(leTvSVMImageModelEstimatorTrainOneClass ${LEARNING_TESTS1}
--compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/leTvsvm_model_image_one_class
                        ${TEMP}/leTvsvm_model_image_one_class
        otbSVMImageModelEstimatorTrainOneClass
 ${INPUTDATA}/ROI_QB_MUL_4.tif
 ${INPUTDATA}/ROI_QB_MUL_4_train_one_class.tif
 ${TEMP}/leTvsvm_model_image_one_class)

ADD_TEST(leTvSVMImageModelEstimatorModelAccessor ${LEARNING_TESTS1}
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

ADD_TEST(leTuSVMPointSetModelEstimatorNew ${LEARNING_TESTS2}
        otbSVMPointSetModelEstimatorNew)

ADD_TEST(leTvSVMPointSetModelEstimatorTrain ${LEARNING_TESTS2}
--compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/leTvsvm_model_pointset
                        ${TEMP}/leTvsvm_model_pointset
        otbSVMPointSetModelEstimatorTrain
 ${TEMP}/leTvsvm_model_pointset)

# -------            otb::SVMSampleListModelEstimator   --------------------

ADD_TEST(leTuSVMSampleListModelEstimatorNew ${LEARNING_TESTS2}
        otbSVMSampleListModelEstimatorNew)

ADD_TEST(leTvSVMSampleListModelEstimatorTest ${LEARNING_TESTS2}
 --compare-ascii ${EPSILON_9}
    ${BASELINE_FILES}/leTvSVMSampleListModelEstimatorModel.txt
    ${TEMP}/leTvSVMSampleListModelEstimatorModel.txt
        otbSVMSampleListModelEstimatorTest
    ${EXAMPLEDATA}/qb_RoadExtract.tif
    ${EXAMPLEDATA}/qb_RoadExtract_easyClassification.shp
    ${TEMP}/leTvSVMSampleListModelEstimatorModel.txt
)

# -------            otb::SVMClassifier   -----------------------------

ADD_TEST(leTuSVMClassifierNew ${LEARNING_TESTS2}
        otbSVMClassifierNew)

ADD_TEST(leTvSVMClassifierImage ${LEARNING_TESTS2}
--compare-image ${NOTOL}
  ${BASELINE}/leSVMClassifierImageOutput.png
  ${TEMP}/leSVMClassifierImageOutput.png
        otbSVMClassifierImage
 ${INPUTDATA}/ROI_QB_MUL_4.tif
 ${INPUTDATA}/svm_model_image
 ${TEMP}/leSVMClassifierImageOutput.png)

ADD_TEST(leTvSVMClassifierImageOneClass ${LEARNING_TESTS2}
--compare-image ${NOTOL}
  ${BASELINE}/leSVMClassifierImageOneClassOutput.png
  ${TEMP}/leSVMClassifierImageOneClassOutput.png
        otbSVMClassifierImage
 ${INPUTDATA}/ROI_QB_MUL_4.tif
 ${INPUTDATA}/svm_model_image_one_class
 ${TEMP}/leSVMClassifierImageOneClassOutput.png)

ADD_TEST(leTvSVMClassifierImageWithGenericKernalLinear ${LEARNING_TESTS2}
--compare-image ${NOTOL}  ${BASELINE}/leTvSVMClassifierImageWithGenericKernalLinearOutput.png
                        ${TEMP}/leTvSVMClassifierImageWithGenericKernalLinearOutput.png
 otbSVMClassifierImage
 ${INPUTDATA}/ROI_QB_MUL_4.tif
 ${INPUTDATA}/svm_model_image_generic_linear
 ${TEMP}/leTvSVMClassifierImageWithGenericKernalLinearOutput.png)



ADD_TEST(leTvSVMClassifierPointSet ${LEARNING_TESTS2}
        otbSVMClassifierPointSet
 ${INPUTDATA}/svm_model_pointset)


# -------            otb::SOMMap   -----------------------------

ADD_TEST(leTuSOMMapNew ${LEARNING_TESTS2}
        otbSOMMapNew)

ADD_TEST(leTvSOMMap ${LEARNING_TESTS2}
        otbSOMMap)

# -------            otb::SOM   -----------------------------

ADD_TEST(leTuSOMNew ${LEARNING_TESTS2}
        otbSOMNew)

ADD_TEST(leTvSOM ${LEARNING_TESTS2}
        --compare-image ${EPSILON_10}
 ${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
 ${TEMP}/leSOMPoupeesSubOutputMap1.hdr
 otbSOM
 ${INPUTDATA}/poupees_sub.png
 ${TEMP}/leSOMPoupeesSubOutputMap1.hdr
 32 32 10 10 5 1.0 0.1 0)

# -------            otb::PeriodicSOM   -----------------------------

ADD_TEST(leTuPeriodicSOMNew ${LEARNING_TESTS2}
        otbPeriodicSOMNew)

ADD_TEST(leTvPeriodicSOM ${LEARNING_TESTS2}
        --compare-image ${EPSILON_10}
 ${BASELINE}/lePeriodicSOMPoupeesSubOutputMap1.hdr
 ${TEMP}/lePeriodicSOMPoupeesSubOutputMap1.hdr
 otbPeriodicSOMTest
 ${INPUTDATA}/poupees_sub.png
 ${TEMP}/lePeriodicSOMPoupeesSubOutputMap1.hdr
 32 32 10 10 5 1.0 0.1 0)

# -------            otb::SOMActivationBuilder   -----------------------------

ADD_TEST(leTuSOMActivationBuilderNew ${LEARNING_TESTS2}
        otbSOMActivationBuilderNew)

ADD_TEST(leTvSOMActivationBuilder ${LEARNING_TESTS2}
        --compare-image ${NOTOL}
 ${BASELINE}/leSOMPoupeesSubActivationMap1.hdr
 ${TEMP}/leSOMPoupeesSubActivationMap1.hdr
 otbSOMActivationBuilder
 ${INPUTDATA}/poupees_sub.png
 ${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
 ${TEMP}/leSOMPoupeesSubActivationMap1.hdr
 32 32 10 10 5 1.0 0.1 128)

# -------            otb::SOMbasedImageFilter -----------------------------------

ADD_TEST(leTuSOMbasedImageFilterNew ${LEARNING_TESTS2}
         otbSOMbasedImageFilterNew )

ADD_TEST(leTvSOMbasedImageFilter ${LEARNING_TESTS2}
        --compare-image ${NOTOL}
 ${BASELINE}/leSOMbasedImageFilterOuptut.hdr
 ${TEMP}/leSOMbasedImageFilterOuptut.hdr
 otbSOMbasedImageFilterTest
 ${INPUTDATA}/poupees_sub.png
 ${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
 ${TEMP}/leSOMbasedImageFilterOuptut.hdr
 )


# ------             otb::SOMWithMissingValue -----------------------------------

ADD_TEST(leTuSOMWithMissingValueNew ${LEARNING_TESTS2}
         otbSOMWithMissingValueNew )

ADD_TEST(leTvSOMWithMissingValue ${LEARNING_TESTS2}
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

ADD_TEST(leTuSOMClassifierNew ${LEARNING_TESTS3}
        otbSOMClassifierNew)

ADD_TEST(leTvSOMClassifier ${LEARNING_TESTS3}
        --compare-image ${NOTOL}
 ${BASELINE}/leSOMPoupeesClassified.hdr
 ${TEMP}/leSOMPoupeesClassified.hdr
 otbSOMClassifier
 ${INPUTDATA}/poupees_sub.png
 ${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
 ${TEMP}/leSOMPoupeesClassified.hdr
 )

# -------            otb::SOMImageClassificationFilter   -----------------------------

ADD_TEST(leTuSOMImageClassificationFilterNew ${LEARNING_TESTS3}
        otbSOMImageClassificationFilterNew)

ADD_TEST(leTvSOMImageClassificationFilter ${LEARNING_TESTS3}
        --compare-image ${NOTOL}
 ${BASELINE}/leSOMPoupeesClassified.hdr
 ${TEMP}/leSOMImageClassificationFilterOuptut.hdr
 otbSOMImageClassificationFilter
 ${INPUTDATA}/poupees_sub.png
 ${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
 ${TEMP}/leSOMImageClassificationFilterOuptut.hdr
 )


# -------            otb::KMeansImageClassificationFilter   -----------------------------

ADD_TEST(leTuKMeansImageClassificationFilterNew ${LEARNING_TESTS3}
        otbKMeansImageClassificationFilterNew)

ADD_TEST(leTvKMeansImageClassificationFilter ${LEARNING_TESTS3}
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

ADD_TEST(leTuSEMClassifierNew ${LEARNING_TESTS3}
        otbSEMClassifierNew)


# ------- otb::SVMImageClassificationFilter ---------------------------

ADD_TEST(leTuSVMImageClassificationFilterNew ${LEARNING_TESTS3}
 otbSVMImageClassificationFilterNew)

ADD_TEST(leTvSVMImageClassificationFilter ${LEARNING_TESTS3}
 --compare-image ${NOTOL}
 ${BASELINE}/leSVMImageClassificationFilterOutput.tif
 ${TEMP}/leSVMImageClassificationFilterOutput.tif
 otbSVMImageClassificationFilter
 ${INPUTDATA}/ROI_QB_MUL_4.tif
 ${INPUTDATA}/svm_model_image
 ${TEMP}/leSVMImageClassificationFilterOutput.tif)
 

# ------- otb::SVMImageClassificationFilterWithNeighborhoodMajorityVoting ---------------------------
 
ADD_TEST(leTvSVMImageClassificationFilterWithNeighborhoodMajorityVoting ${LEARNING_TESTS5}
 --compare-image ${NOTOL}
 ${BASELINE}/leSVMImageClassificationWithNMVFilterOutput.tif
 ${TEMP}/leSVMImageClassificationWithNMVFilterOutput.tif
otbNeighborhoodMajorityVotingImageFilterTest
 ${TEMP}/leSVMImageClassificationFilterOutput.tif
 ${TEMP}/leSVMImageClassificationWithNMVFilterOutput.tif
 false
 )
 
SET_TESTS_PROPERTIES(leTvSVMImageClassificationFilterWithNeighborhoodMajorityVoting PROPERTIES DEPENDS leTvSVMImageClassificationFilter)


# ------- otb::SVMImageClassificationWithRuleFilter ----------------------

ADD_TEST(leTuSVMImageClassificationWithRuleFilterNew ${LEARNING_TESTS3}
          otbSVMImageClassificationWithRuleFilterNew)

ADD_TEST(leTvSVMImageClassificationWithRuleFilter ${LEARNING_TESTS3}
 --compare-image ${NOTOL}
 ${BASELINE}/leSVMImageClassificationWithRuleFilterOutput.tif
 ${TEMP}/leSVMImageClassificationWithRuleFilterOutput.tif
 otbSVMImageClassificationWithRuleFilter
 ${INPUTDATA}/ROI_QB_MUL_4.tif
 ${INPUTDATA}/svm_model_image
 ${TEMP}/leSVMImageClassificationWithRuleFilterOutput.tif)



ADD_TEST(leTvSVMModelGenericKernelsTest ${LEARNING_TESTS3}
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


FOREACH(kernelType RANGE 0 18)
  ADD_TEST(leTvSVMKernelTest${kernelType} ${LEARNING_TESTS3}
   --compare-ascii ${EPSILON_9}
      ${BASELINE_FILES}/leTvSVMKernelTest${kernelType}.txt
      ${TEMP}/leTvSVMKernelTest${kernelType}.txt
        otbSVMKernelsTest
      ${EXAMPLEDATA}/qb_RoadExtract.tif
      ${EXAMPLEDATA}/qb_RoadExtract_easyClassification.shp
      ${TEMP}/leTvSVMKernelTest${kernelType}.txt
      ${kernelType}
  )
ENDFOREACH(kernelType)

ADD_TEST(leTvSVMModelCopy ${LEARNING_TESTS3}
--compare-ascii ${NOTOL}  ${INPUTDATA}/svm_model
                        ${TEMP}/svmcopymodel_test
        otbSVMModelCopyTest
 ${INPUTDATA}/svm_model
 ${TEMP}/svmcopymodel_test)

ADD_TEST(leTvSVMModelCopyGenericKernel ${LEARNING_TESTS3}
--compare-ascii ${NOTOL}  ${INPUTDATA}/svm_model_generic
                        ${TEMP}/svmcopygeneric_test
        otbSVMModelCopyGenericKernelTest
 ${INPUTDATA}/svm_model_generic
 ${TEMP}/svmcopygeneric_test)

ADD_TEST(leTvSVMModelCopyComposedKernel ${LEARNING_TESTS3}
--compare-ascii ${NOTOL}  ${INPUTDATA}/svm_model_composed
                        ${TEMP}/svmcopycomposed_test
        otbSVMModelCopyComposedKernelTest
 ${INPUTDATA}/svm_model_composed
 ${TEMP}/svmcopycomposed_test)

ADD_TEST(leTvSVMInverseCosSpectralAngleKernelFunctorImageModelEstimatorTest ${LEARNING_TESTS3}
        otbSVMInverseCosSpectralAngleKernelFunctorImageModelEstimatorTest
 ${INPUTDATA}/ROI_QB_MUL_4.tif
 ${INPUTDATA}/ROI_QB_MUL_4_train_one_class.tif
 ${TEMP}/leTvsvm_model_image_one_class)

ADD_TEST(leTvSVMInverseCosSpectralAngleKernelFunctorImageClassificationTest ${LEARNING_TESTS3}
        otbSVMInverseCosSpectralAngleKernelFunctorImageClassificationTest
 ${INPUTDATA}/ROI_QB_MUL_4.tif
 ${INPUTDATA}/ROI_QB_MUL_4_train_one_class.tif
 ${TEMP}/leTvsvm_model_image_one_class)





# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbLearningTESTS4 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::RoidataConversion   -----------------------------
ADD_TEST(leTuROIdataConversionNew ${LEARNING_TESTS4}
        otbROIdataConversionNew)

ADD_TEST(leTvROIdataConversion ${LEARNING_TESTS4}
 --compare-ascii ${NOTOL}
 ${BASELINE_FILES}/leTvROIdataConversion.txt
 ${TEMP}/leTvROIdataConversion.txt
        otbROIdataConversion
 ${INPUTDATA}/ImageLineDir.bsq.hdr
 ${INPUTDATA}/ImageLine.bsq.hdr
 ${TEMP}/leTvROIdataConversion.txt
)


ADD_TEST(leTuSVMCrossValidationCostFunctionNew ${LEARNING_TESTS4}
        otbSVMCrossValidationCostFunctionNew)

ADD_TEST(leTuExhaustiveExponentialOptimizerNew ${LEARNING_TESTS4}
        otbExhaustiveExponentialOptimizerNew)

ADD_TEST(leTvExhaustiveExponentialOptimizerTest ${LEARNING_TESTS4}
        --compare-ascii ${NOTOL}
          ${BASELINE_FILES}/leTvExhaustiveExponentialOptimizerOutput.txt
          ${TEMP}/leTvExhaustiveExponentialOptimizerTestOutput.txt
        otbExhaustiveExponentialOptimizerTest
          ${TEMP}/leTvExhaustiveExponentialOptimizerTestOutput.txt)

# -------            otb::ListSampleGenerator   -----------------------------
ADD_TEST(leTuListSampleGeneratorNew ${LEARNING_TESTS4}
        otbListSampleGeneratorNew)

ADD_TEST(leTvListSampleGenerator1 ${LEARNING_TESTS4}
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
ADD_TEST(leTvListSampleGenerator2 ${LEARNING_TESTS4}
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
ADD_TEST(leTvListSampleGenerator3 ${LEARNING_TESTS4}
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
ADD_TEST(leTuConfusionMatrixCalculatorNew ${LEARNING_TESTS4}
otbConfusionMatrixCalculatorNew)

ADD_TEST(leTvConfusionMatrixCalculatorSetListSamples ${LEARNING_TESTS4}
otbConfusionMatrixCalculatorSetListSamples 1000 4)

ADD_TEST(leTvConfusionMatrixCalculatorWrongSize ${LEARNING_TESTS4}
otbConfusionMatrixCalculatorWrongSize 1000 4)
SET_TESTS_PROPERTIES(leTvConfusionMatrixCalculatorWrongSize PROPERTIES WILL_FAIL true)

ADD_TEST(leTvConfusionMatrixCalculatorUpdate ${LEARNING_TESTS4}
otbConfusionMatrixCalculatorUpdate 4 3)

# ShiftScaleSampleListFilter tests ----------
ADD_TEST(leTuShiftScaleSampleListFilterNew ${LEARNING_TESTS4}
otbShiftScaleSampleListFilterNew)

ADD_TEST(leTvShiftScaleSampleListFilter ${LEARNING_TESTS4}
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
ADD_TEST(leTuGaussianAdditiveNoiseSampleListFilterNew ${LEARNING_TESTS4}
otbGaussianAdditiveNoiseSampleListFilterNew)

ADD_TEST(leTvGaussianAdditiveNoiseSampleListFilter ${LEARNING_TESTS4}
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
ADD_TEST(leTuConcatenateSampleListFilterNew ${LEARNING_TESTS4}
otbConcatenateSampleListFilterNew)

ADD_TEST(leTvConcatenateSampleListFilter ${LEARNING_TESTS4}
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
ADD_TEST(leTuListSampleToBalancedListSampleFilterNew ${LEARNING_TESTS4}
otbListSampleToBalancedListSampleFilterNew)

ADD_TEST(leTvListSampleToBalancedListSampleFilter ${LEARNING_TESTS4}
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
ADD_TEST(leTuDecisionTreeNew ${LEARNING_TESTS5}
otbDecisionTreeNew)

ADD_TEST(leTvDecisionTreeBuild ${LEARNING_TESTS5}
otbDecisionTreeBuild)

ADD_TEST(leTvDecisionTreeWithRealValues ${LEARNING_TESTS5}
otbDecisionTreeWithRealValues)


ADD_TEST(leTuNeighborhoodMajorityVotingImageFilterNew ${LEARNING_TESTS5}
otbNeighborhoodMajorityVotingImageFilterNew)

ADD_TEST(leTvNeighborhoodMajorityVotingImageFilterTest ${LEARNING_TESTS5}
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


# Testing srcs
SET(BasicLearning_SRCS1
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
SET(BasicLearning_SRCS2
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
SET(BasicLearning_SRCS3
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
)


SET(BasicLearning_SRCS4
otbLearningTests4.cxx
otbROIdataConversionNew.cxx
otbROIdataConversion.cxx
otbSVMCrossValidationCostFunctionNew.cxx
otbExhaustiveExponentialOptimizerNew.cxx
otbExhaustiveExponentialOptimizerTest.cxx
otbListSampleGeneratorTest.cxx
otbConfusionMatrixCalculatorTest.cxx
otbShiftScaleSampleListFilter.cxx
otbGaussianAdditiveNoiseSampleListFilter.cxx
otbConcatenateSampleListFilter.cxx
otbListSampleToBalancedListSampleFilter.cxx
)

SET(BasicLearning_SRCS5
otbLearningTests5.cxx
otbDecisionTreeNew.cxx
otbDecisionTreeBuild.cxx
otbDecisionTreeWithRealValues.cxx
otbNeighborhoodMajorityVotingImageFilterNew.cxx
otbNeighborhoodMajorityVotingImageFilterTest.cxx
)

OTB_ADD_EXECUTABLE(otbLearningTests1 "${BasicLearning_SRCS1}" "OTBLearning;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbLearningTests2 "${BasicLearning_SRCS2}" "OTBLearning;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbLearningTests3 "${BasicLearning_SRCS3}" "OTBLearning;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbLearningTests4 "${BasicLearning_SRCS4}" "OTBLearning;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbLearningTests5 "${BasicLearning_SRCS5}" "OTBLearning;OTBIO;OTBTesting")

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )




