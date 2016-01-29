otb_module_test()

set(OTBSVMLearningTests
otbSVMLearningTestDriver.cxx
otbSVMImageClassificationWithRuleFilter.cxx
otbLabelMapSVMClassifier.cxx
otbSVMPointSetModelEstimatorTrain.cxx
otbExhaustiveExponentialOptimizerNew.cxx
otbSVMImageModelEstimatorModelAccessor.cxx
otbSVMImageClassificationFilter.cxx
otbSVMModelNew.cxx
otbSVMImageModelEstimatorNew.cxx
otbSVMImageModelEstimatorTrainOneClass.cxx
otbSVMCrossValidationCostFunctionNew.cxx
otbSVMModelLoadSave.cxx
otbSVMClassifierPointSet.cxx
otbSVMImageClassificationWithRuleFilterNew.cxx
##>otbSVMImageModelEstimatorTrain.cxx
otbObjectDetectionClassifier.cxx
otbSVMClassifierNew.cxx
otbSVMSampleListModelEstimatorTest.cxx
otbSVMModelAccessor.cxx
otbSVMModelLoad.cxx
otbSVMImageClassificationFilterNew.cxx
otbSVMPointSetModelEstimatorNew.cxx
otbSVMMarginSampler.cxx
otbExhaustiveExponentialOptimizerTest.cxx
)

add_executable(otbSVMLearningTestDriver ${OTBSVMLearningTests})
target_link_libraries(otbSVMLearningTestDriver ${OTBSVMLearning-Test_LIBRARIES})
otb_module_target_label(otbSVMLearningTestDriver)

# Tests Declaration

otb_add_test(NAME leTvSVMImageClassificationWithRuleFilter COMMAND otbSVMLearningTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leSVMImageClassificationWithRuleFilterOutput.tif
  ${TEMP}/leSVMImageClassificationWithRuleFilterOutput.tif
  otbSVMImageClassificationWithRuleFilter
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/svm_model_image
  ${TEMP}/leSVMImageClassificationWithRuleFilterOutput.tif)

##>otb_add_test(NAME obTvLabelMapSVMClassifier COMMAND otbSVMLearningTestDriver
##>  otbLabelMapSVMClassifier
##>  ${INPUTDATA}/maur.tif
##>  ${INPUTDATA}/maur_labelled.tif
##>  ${TEMP}/obTvLabelMapSVMClassifierLabeledOutput.tif)

otb_add_test(NAME obTuLabelMapSVMClassifierNew COMMAND otbSVMLearningTestDriver
  otbLabelMapSVMClassifierNew)
  
otb_add_test(NAME leTvSVMPointSetModelEstimatorTrain COMMAND otbSVMLearningTestDriver
  --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/leTvsvm_model_pointset
  ${TEMP}/leTvsvm_model_pointset
  otbSVMPointSetModelEstimatorTrain
  ${TEMP}/leTvsvm_model_pointset)

otb_add_test(NAME leTuExhaustiveExponentialOptimizerNew COMMAND otbSVMLearningTestDriver
  otbExhaustiveExponentialOptimizerNew)

otb_add_test(NAME leTvSVMImageModelEstimatorModelAccessor COMMAND otbSVMLearningTestDriver
  --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/leSVMModelEstimatorModelAccessor.txt
  ${TEMP}/leSVMModelEstimatorModelAccessor.txt
  otbSVMImageModelEstimatorModelAccessor
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/ROI_QB_MUL_4_train.c1.hdr
  ${TEMP}/leSVMModelEstimatorModelAccessor.txt)

otb_add_test(NAME leTvSVMImageClassificationFilter COMMAND otbSVMLearningTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leSVMImageClassificationFilterOutput.tif
  ${TEMP}/leSVMImageClassificationFilterOutput.tif
  otbSVMImageClassificationFilter
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/svm_model_image
  ${TEMP}/leSVMImageClassificationFilterOutput.tif)



otb_add_test(NAME leTuSVMModelNew COMMAND otbSVMLearningTestDriver
  otbSVMModelNew)

otb_add_test(NAME leTuSVMImageModelEstimatorNew COMMAND otbSVMLearningTestDriver
  otbSVMImageModelEstimatorNew)

otb_add_test(NAME leTvSVMImageModelEstimatorTrainOneClass COMMAND otbSVMLearningTestDriver
  --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/leTvsvm_model_image_one_class
  ${TEMP}/leTvsvm_model_image_one_class
  otbSVMImageModelEstimatorTrainOneClass
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/ROI_QB_MUL_4_train_one_class.tif
  ${TEMP}/leTvsvm_model_image_one_class)

otb_add_test(NAME leTuSVMCrossValidationCostFunctionNew COMMAND otbSVMLearningTestDriver
  otbSVMCrossValidationCostFunctionNew)

otb_add_test(NAME leTvSVMModelLoadSave COMMAND otbSVMLearningTestDriver
  --compare-ascii ${NOTOL}  ${INPUTDATA}/svm_model
  ${TEMP}/svmmodel_test
  otbSVMModelLoadSave
  ${INPUTDATA}/svm_model
  ${TEMP}/svmmodel_test)

otb_add_test(NAME leTvSVMClassifierPointSet COMMAND otbSVMLearningTestDriver
  otbSVMClassifierPointSet
  ${INPUTDATA}/svm_model_pointset)

otb_add_test(NAME leTuSVMImageClassificationWithRuleFilterNew COMMAND otbSVMLearningTestDriver
  otbSVMImageClassificationWithRuleFilterNew)

##>otb_add_test(NAME leTvSVMImageModelEstimatorTrain COMMAND otbSVMLearningTestDriver
##>  --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/leTvsvm_model_image
##>  ${TEMP}/leTvsvm_model_image
##>  otbSVMImageModelEstimatorTrain
##>  ${INPUTDATA}/ROI_QB_MUL_4.tif
##>  ${INPUTDATA}/ROI_QB_MUL_4_train.c1.hdr
##>  ${TEMP}/leTvsvm_model_image
##>  0 )

##>otb_add_test(NAME leTvSVMImageModelEstimatorTrainWithOptimization COMMAND otbSVMLearningTestDriver
##>  --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/leTvsvm_model_image_opt
##>  ${TEMP}/leTvsvm_model_image_opt
##>  otbSVMImageModelEstimatorTrain
##>  ${INPUTDATA}/ROI_QB_MUL_4.tif
##>  ${INPUTDATA}/ROI_QB_MUL_4_train.c1.hdr
##>  ${TEMP}/leTvsvm_model_image_opt
##>  1)

otb_add_test(NAME odTuObjectDetectionClassifierNew COMMAND otbSVMLearningTestDriver
  otbObjectDetectionClassifierNew
  )

otb_add_test(NAME odTvObjectDetectionClassifierStreaming COMMAND otbSVMLearningTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/TvObjectDetectionClassifierOutput.txt
  ${TEMP}/TvObjectDetectionClassifierOutputStreaming.txt
  otbObjectDetectionClassifier
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01_extract_amplitude.tif
  ${INPUTDATA}/ObjectReco/Boats/FeatureStats_RadiometricMoments_amplitude.xml
  ${BASELINE_FILES}/TvDescriptorsSVMModelCreation.svm
  ${TEMP}/TvObjectDetectionClassifierOutputStreaming.txt
  50 # streaming
  5 # neighborhood radius
  )

otb_add_test(NAME odTvObjectDetectionClassifier COMMAND otbSVMLearningTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/TvObjectDetectionClassifierOutput.txt
  ${TEMP}/TvObjectDetectionClassifierOutput.txt
  otbObjectDetectionClassifier
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01_extract_amplitude.tif
  ${INPUTDATA}/ObjectReco/Boats/FeatureStats_RadiometricMoments_amplitude.xml
  ${BASELINE_FILES}/TvDescriptorsSVMModelCreation.svm
  ${TEMP}/TvObjectDetectionClassifierOutput.txt
  0 # streaming
  5 # neighborhood radius
  )

otb_add_test(NAME leTuSVMClassifierNew COMMAND otbSVMLearningTestDriver
  otbSVMClassifierNew)

otb_add_test(NAME leTvSVMSampleListModelEstimatorTest COMMAND otbSVMLearningTestDriver
  --compare-ascii ${EPSILON_4}
  ${BASELINE_FILES}/leTvSVMSampleListModelEstimatorModel.txt
  ${TEMP}/leTvSVMSampleListModelEstimatorModel.txt
  otbSVMSampleListModelEstimatorTest
  ${EXAMPLEDATA}/qb_RoadExtract.tif
  ${EXAMPLEDATA}/qb_RoadExtract_easyClassification.shp
  ${TEMP}/leTvSVMSampleListModelEstimatorModel.txt
  )

otb_add_test(NAME leTuSVMSampleListModelEstimatorNew COMMAND otbSVMLearningTestDriver
  otbSVMSampleListModelEstimatorNew)

otb_add_test(NAME leTvSVMModelAccessor COMMAND otbSVMLearningTestDriver
  --compare-ascii ${EPSILON_5}  ${BASELINE_FILES}/leSVMModelAccessor.txt
  ${TEMP}/leSVMModelAccessor.txt
  otbSVMModelAccessor
  ${INPUTDATA}/svm_model
  ${TEMP}/leSVMModelAccessor.txt)

otb_add_test(NAME leTuSVMModelLoad COMMAND otbSVMLearningTestDriver
  otbSVMModelLoad
  ${INPUTDATA}/svm_model)

otb_add_test(NAME leTuSVMImageClassificationFilterNew COMMAND otbSVMLearningTestDriver
  otbSVMImageClassificationFilterNew)

otb_add_test(NAME leTuSVMPointSetModelEstimatorNew COMMAND otbSVMLearningTestDriver
  otbSVMPointSetModelEstimatorNew)

otb_add_test(NAME leTuSVMMarginSamplerNew COMMAND otbSVMLearningTestDriver
  otbSVMMarginSamplerNew)

otb_add_test(NAME leTvExhaustiveExponentialOptimizerTest COMMAND otbSVMLearningTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/leTvExhaustiveExponentialOptimizerOutput.txt
  ${TEMP}/leTvExhaustiveExponentialOptimizerTestOutput.txt
  otbExhaustiveExponentialOptimizerTest
  ${TEMP}/leTvExhaustiveExponentialOptimizerTestOutput.txt)

