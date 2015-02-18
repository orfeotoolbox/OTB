otb_module_test()

set(OTBSVMLearningTests
otbSVMLearningTestDriver.cxx
otbSVMImageClassificationWithRuleFilter.cxx
otbSVMModelCopyGenericKernelTest.cxx
otbLabelMapSVMClassifier.cxx
otbSVMModelCopyTest.cxx
otbSVMPointSetModelEstimatorTrain.cxx
otbExhaustiveExponentialOptimizerNew.cxx
otbSVMImageModelEstimatorModelAccessor.cxx
otbSVMImageClassificationFilter.cxx
otbSVMModelGenericKernelLoadSave.cxx
otbSVMModelNew.cxx
otbSVMImageModelEstimatorNew.cxx
otbSVMModelGenericKernelsTest.cxx
otbSVMImageModelEstimatorTrainOneClass.cxx
otbSVMCrossValidationCostFunctionNew.cxx
otbSVMModelLoadSave.cxx
otbMachineLearningModelCanRead.cxx
otbSVMClassifierPointSet.cxx
otbSVMImageClassificationWithRuleFilterNew.cxx
otbSVMImageModelEstimatorTrain.cxx
otbObjectDetectionClassifier.cxx
otbSVMClassifierImage.cxx
otbSVMClassifierNew.cxx
otbSVMSampleListModelEstimatorTest.cxx
otbSVMModelAccessor.cxx
otbSVMModelLoad.cxx
otbSVMImageClassificationFilterNew.cxx
otbSVMInverseCosSpectralAngleKernelFunctorImageModelEstimatorTest.cxx
otbSVMInverseCosSpectralAngleKernelFunctorImageClassificationTest.cxx
otbSVMPointSetModelEstimatorNew.cxx
otbSVMMarginSampler.cxx
otbSVMModelCopyComposedKernelTest.cxx
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

otb_add_test(NAME leTvSVMModelCopyGenericKernel COMMAND otbSVMLearningTestDriver
  --compare-ascii ${NOTOL}  ${INPUTDATA}/svm_model_generic
  ${TEMP}/svmcopygeneric_test
  otbSVMModelCopyGenericKernelTest
  ${INPUTDATA}/svm_model_generic
  ${TEMP}/svmcopygeneric_test)

otb_add_test(NAME obTvLabelMapSVMClassifier COMMAND otbSVMLearningTestDriver
  otbLabelMapSVMClassifier
  ${INPUTDATA}/maur.tif
  ${INPUTDATA}/maur_labelled.tif
  ${TEMP}/obTvLabelMapSVMClassifierLabeledOutput.tif)

otb_add_test(NAME obTuLabelMapSVMClassifierNew COMMAND otbSVMLearningTestDriver
  otbLabelMapSVMClassifierNew)

otb_add_test(NAME leTvSVMModelCopy COMMAND otbSVMLearningTestDriver
  --compare-ascii ${NOTOL}  ${INPUTDATA}/svm_model
  ${TEMP}/svmcopymodel_test
  otbSVMModelCopyTest
  ${INPUTDATA}/svm_model
  ${TEMP}/svmcopymodel_test)
  
foreach(kernelType RANGE 0 18)
  otb_add_test(NAME leTvSVMKernelTest${kernelType} COMMAND otbSVMLearningTestDriver
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

otb_add_test(NAME leTvSVMModelWithGenericKeywordKernelLoadSave COMMAND otbSVMLearningTestDriver
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/leTvsvmmodel_image_generic_version_test
  ${TEMP}/leTvsvmmodel_image_generic_version_test
  otbSVMModelGenericKernelLoadSave
  ${INPUTDATA}/svm_model_image_generic_version
  ${TEMP}/leTvsvmmodel_image_generic_version_test)

otb_add_test(NAME leTvSVMModelKernelTypeGenericLoadSave COMMAND otbSVMLearningTestDriver
  --compare-ascii ${NOTOL}  ${INPUTDATA}/svm_model_image_generic_kernel
  ${TEMP}/leTvsvmmodel_image_generic_kernel_test
  otbSVMModelGenericKernelLoadSave
  ${INPUTDATA}/svm_model_image_generic_kernel
  ${TEMP}/leTvsvmmodel_image_generic_kernel_test)

otb_add_test(NAME leTuSVMModelNew COMMAND otbSVMLearningTestDriver
  otbSVMModelNew)

otb_add_test(NAME leTuSVMImageModelEstimatorNew COMMAND otbSVMLearningTestDriver
  otbSVMImageModelEstimatorNew)

otb_add_test(NAME leTvSVMModelGenericKernelsTest COMMAND otbSVMLearningTestDriver
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

otb_add_test(NAME leTuDecisionTreeMachineLearningModelCanRead COMMAND otbSVMLearningTestDriver
  otbDecisionTreeMachineLearningModelCanRead
  ${TEMP}/decisiontree_model.txt
  )
set_property(TEST leTuDecisionTreeMachineLearningModelCanRead PROPERTY DEPENDS leTvDecisionTreeMachineLearningModel)

otb_add_test(NAME leTuGradientBoostedTreeMachineLearningModelCanRead COMMAND otbSVMLearningTestDriver
  otbGradientBoostedTreeMachineLearningModelCanRead
  ${TEMP}/gbt_model.txt
  )
set_property(TEST leTuGradientBoostedTreeMachineLearningModelCanRead PROPERTY DEPENDS leTvGradientBoostedTreeMachineLearningModel)

otb_add_test(NAME leTuNormalBayesMachineLearningModelCanRead COMMAND otbSVMLearningTestDriver
  otbNormalBayesMachineLearningModelCanRead
  ${TEMP}/normalbayes_model.txt
  )
set_property(TEST leTuNormalBayesMachineLearningModelCanRead PROPERTY DEPENDS leTvNormalBayesMachineLearningModel)

otb_add_test(NAME leTuANNMachineLearningModelCanRead COMMAND otbSVMLearningTestDriver
  otbNeuralNetworkMachineLearningModelCanRead
  ${TEMP}/ann_model.txt
  )
set_property(TEST leTuANNMachineLearningModelCanRead PROPERTY DEPENDS leTvANNMachineLearningModel)

otb_add_test(NAME leTuSVMMachineLearningModelCanRead COMMAND otbSVMLearningTestDriver
  otbSVMMachineLearningModelCanRead
  ${TEMP}/svm_model.txt
  )
set_property(TEST leTuSVMMachineLearningModelCanRead PROPERTY DEPENDS leTvSVMMachineLearningModel)

otb_add_test(NAME leTuLibSVMMachineLearningModelCanRead COMMAND otbSVMLearningTestDriver
  otbLibSVMMachineLearningModelCanRead
  ${TEMP}/libsvm_model.txt
  )
set_property(TEST leTuLibSVMMachineLearningModelCanRead PROPERTY DEPENDS leTvLibSVMMachineLearningModel)

otb_add_test(NAME leTuBoostMachineLearningModelCanRead COMMAND otbSVMLearningTestDriver
  otbBoostMachineLearningModelCanRead
  ${TEMP}/boost_model.txt
  )
set_property(TEST leTuBoostMachineLearningModelCanRead PROPERTY DEPENDS leTvBoostMachineLearningModel)

otb_add_test(NAME leTuRandomForestsMachineLearningModelCanRead COMMAND otbSVMLearningTestDriver
  otbRandomForestsMachineLearningModelCanRead
  ${TEMP}/rf_model.txt
  )
set_property(TEST leTuRandomForestsMachineLearningModelCanRead PROPERTY DEPENDS leTvRandomForestsMachineLearningModel)

otb_add_test(NAME leTuKNNMachineLearningModelCanRead COMMAND otbSVMLearningTestDriver
  otbKNNMachineLearningModelCanRead
  ${TEMP}/knn_model.txt
  )
set_property(TEST leTuKNNMachineLearningModelCanRead PROPERTY DEPENDS leTvKNearestNeighborsMachineLearningModel)

otb_add_test(NAME leTvSVMClassifierPointSet COMMAND otbSVMLearningTestDriver
  otbSVMClassifierPointSet
  ${INPUTDATA}/svm_model_pointset)

otb_add_test(NAME leTuSVMImageClassificationWithRuleFilterNew COMMAND otbSVMLearningTestDriver
  otbSVMImageClassificationWithRuleFilterNew)

otb_add_test(NAME leTvSVMImageModelEstimatorTrain COMMAND otbSVMLearningTestDriver
  --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/leTvsvm_model_image
  ${TEMP}/leTvsvm_model_image
  otbSVMImageModelEstimatorTrain
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/ROI_QB_MUL_4_train.c1.hdr
  ${TEMP}/leTvsvm_model_image
  0 )

otb_add_test(NAME leTvSVMImageModelEstimatorTrainWithOptimization COMMAND otbSVMLearningTestDriver
  --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/leTvsvm_model_image_opt
  ${TEMP}/leTvsvm_model_image_opt
  otbSVMImageModelEstimatorTrain
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/ROI_QB_MUL_4_train.c1.hdr
  ${TEMP}/leTvsvm_model_image_opt
  1)

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

otb_add_test(NAME leTvSVMClassifierImageWithGenericKernalLinear COMMAND otbSVMLearningTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/leTvSVMClassifierImageWithGenericKernalLinearOutput.png
  ${TEMP}/leTvSVMClassifierImageWithGenericKernalLinearOutput.png
  otbSVMClassifierImage
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/svm_model_image_generic_linear
  ${TEMP}/leTvSVMClassifierImageWithGenericKernalLinearOutput.png)

otb_add_test(NAME leTvSVMClassifierImageOneClass COMMAND otbSVMLearningTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leSVMClassifierImageOneClassOutput.png
  ${TEMP}/leSVMClassifierImageOneClassOutput.png
  otbSVMClassifierImage
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/svm_model_image_one_class
  ${TEMP}/leSVMClassifierImageOneClassOutput.png)

otb_add_test(NAME leTvSVMClassifierImage COMMAND otbSVMLearningTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leSVMClassifierImageOutput.png
  ${TEMP}/leSVMClassifierImageOutput.png
  otbSVMClassifierImage
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/svm_model_image
  ${TEMP}/leSVMClassifierImageOutput.png)

otb_add_test(NAME leTuSVMClassifierNew COMMAND otbSVMLearningTestDriver
  otbSVMClassifierNew)

otb_add_test(NAME leTvSVMSampleListModelEstimatorTest COMMAND otbSVMLearningTestDriver
  --compare-ascii ${EPSILON_9}
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

otb_add_test(NAME leTvSVMInverseCosSpectralAngleKernelFunctorImageModelEstimatorTest COMMAND otbSVMLearningTestDriver
  otbSVMInverseCosSpectralAngleKernelFunctorImageModelEstimatorTest
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/ROI_QB_MUL_4_train_one_class.tif
  ${TEMP}/leTvsvm_model_image_one_class)

otb_add_test(NAME leTvSVMInverseCosSpectralAngleKernelFunctorImageClassificationTest COMMAND otbSVMLearningTestDriver
  otbSVMInverseCosSpectralAngleKernelFunctorImageClassificationTest
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/ROI_QB_MUL_4_train_one_class.tif
  ${TEMP}/leTvsvm_model_image_one_class)

otb_add_test(NAME leTuSVMPointSetModelEstimatorNew COMMAND otbSVMLearningTestDriver
  otbSVMPointSetModelEstimatorNew)

otb_add_test(NAME leTuSVMMarginSamplerNew COMMAND otbSVMLearningTestDriver
  otbSVMMarginSamplerNew)

otb_add_test(NAME leTvSVMModelCopyComposedKernel COMMAND otbSVMLearningTestDriver
  --compare-ascii ${NOTOL}  ${INPUTDATA}/svm_model_composed
  ${TEMP}/svmcopycomposed_test
  otbSVMModelCopyComposedKernelTest
  ${INPUTDATA}/svm_model_composed
  ${TEMP}/svmcopycomposed_test)

otb_add_test(NAME leTvExhaustiveExponentialOptimizerTest COMMAND otbSVMLearningTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/leTvExhaustiveExponentialOptimizerOutput.txt
  ${TEMP}/leTvExhaustiveExponentialOptimizerTestOutput.txt
  otbExhaustiveExponentialOptimizerTest
  ${TEMP}/leTvExhaustiveExponentialOptimizerTestOutput.txt)

