otb_module_test()

set(OTBLearningBaseTests
otbLearningBaseTestDriver.cxx
otbDecisionTreeBuild.cxx
otbConfusionMatrixCalculatorTest.cxx
otbConfusionMatrixMeasurementsTest.cxx
otbGaussianModelComponent.cxx
otbKMeansImageClassificationFilter.cxx
otbDecisionTreeWithRealValues.cxx
otbSEMClassifierNew.cxx
otbTrainMachineLearningModel.cxx
otbImageClassificationFilter.cxx
otbDecisionTreeNew.cxx
otbKMeansImageClassificationFilterNew.cxx
)

add_executable(otbLearningBaseTestDriver ${OTBLearningBaseTests})
target_link_libraries(otbLearningBaseTestDriver ${OTBLearningBase-Test_LIBRARIES})
otb_module_target_label(otbLearningBaseTestDriver)

# Tests Declaration

otb_add_test(NAME leTvDecisionTreeBuild COMMAND otbLearningBaseTestDriver
  otbDecisionTreeBuild)

otb_add_test(NAME leTvConfusionMatrixCalculatorWrongSize COMMAND otbLearningBaseTestDriver
  otbConfusionMatrixCalculatorWrongSize 1000 4)
set_property(TEST leTvConfusionMatrixCalculatorWrongSize PROPERTY WILL_FAIL true)

otb_add_test(NAME leTvConfusionMatrixCalculatorUpdate COMMAND otbLearningBaseTestDriver
  otbConfusionMatrixCalculatorCompute 4 3)

otb_add_test(NAME leTvConfusionMatrixCalculatorUpdateWithBaseline COMMAND otbLearningBaseTestDriver
  otbConfusionMatrixCalculatorComputeWithBaseline)

otb_add_test(NAME leTvConfusionMatrixCalculatorSetListSamples COMMAND otbLearningBaseTestDriver
  otbConfusionMatrixCalculatorSetListSamples 1000 4)

otb_add_test(NAME leTuConfusionMatrixCalculatorNew COMMAND otbLearningBaseTestDriver
  otbConfusionMatrixCalculatorNew)

otb_add_test(NAME leTvConfusionMatrixMeasurementsTest COMMAND otbLearningBaseTestDriver
  otbConfusionMatrixMeasurementsTest)

otb_add_test(NAME leTuConfusionMatrixMeasurementsNew COMMAND otbLearningBaseTestDriver
  otbConfusionMatrixMeasurementsNew)

otb_add_test(NAME leTvConfusionMatrixConcatenateTest COMMAND otbLearningBaseTestDriver
  otbConfusionMatrixConcatenateTest
  ${INPUTDATA}/Classification/QB_1_ortho_C1.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C2.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C3.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C4.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C5.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C6.csv)

otb_add_test(NAME leTuGaussianModelComponentNew COMMAND otbLearningBaseTestDriver
  otbGaussianModelComponentNew)

otb_add_test(NAME leTvDecisionTreeWithRealValues COMMAND otbLearningBaseTestDriver
  otbDecisionTreeWithRealValues)

otb_add_test(NAME leTvKMeansImageClassificationFilter COMMAND otbLearningBaseTestDriver
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

otb_add_test(NAME leTuSEMClassifierNew COMMAND otbLearningBaseTestDriver
  otbSEMClassifierNew)

otb_add_test(NAME leTuNormalBayesMachineLearningModelNew COMMAND otbLearningBaseTestDriver
  otbNormalBayesMachineLearningModelNew)

otb_add_test(NAME leTuRandomForestsMachineLearningModelNew COMMAND otbLearningBaseTestDriver
  otbRandomForestsMachineLearningModelNew)

otb_add_test(NAME leTvLibSVMMachineLearningModel COMMAND otbLearningBaseTestDriver
  otbLibSVMMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/libsvm_model.txt
  )

otb_add_test(NAME leTvANNMachineLearningModel COMMAND otbLearningBaseTestDriver
  otbANNMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/ann_model.txt
  )

otb_add_test(NAME leTuSVMMachineLearningModelNew COMMAND otbLearningBaseTestDriver
  otbSVMMachineLearningModelNew)

otb_add_test(NAME leTuDecisionTreeMachineLearningModelNew COMMAND otbLearningBaseTestDriver
  otbDecisionTreeMachineLearningModelNew)

otb_add_test(NAME leTuKNearestNeighborsMachineLearningModelNew COMMAND otbLearningBaseTestDriver
  otbKNearestNeighborsMachineLearningModelNew)

otb_add_test(NAME leTvSVMMachineLearningRegressionModel COMMAND otbLearningBaseTestDriver
  otbSVMMachineLearningRegressionModel
  ${INPUTDATA}/abalone.scale
  ${TEMP}/svm_model_regression.txt
  )

otb_add_test(NAME leTvSVMMachineLearningModel COMMAND otbLearningBaseTestDriver
  otbSVMMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/svm_model.txt
  )

otb_add_test(NAME leTuBoostMachineLearningModelNew COMMAND otbLearningBaseTestDriver
  otbBoostMachineLearningModelNew)

otb_add_test(NAME leTuLibSVMMachineLearningModelNew COMMAND otbLearningBaseTestDriver
  otbLibSVMMachineLearningModelNew)

otb_add_test(NAME leTvNormalBayesMachineLearningModel COMMAND otbLearningBaseTestDriver
  otbNormalBayesMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/normalbayes_model.txt
  )

otb_add_test(NAME leTvGradientBoostedTreeMachineLearningModel COMMAND otbLearningBaseTestDriver
  otbGradientBoostedTreeMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/gbt_model.txt
  )

otb_add_test(NAME leTvRandomForestsMachineLearningModel COMMAND otbLearningBaseTestDriver
  otbRandomForestsMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/rf_model.txt
  )

otb_add_test(NAME leTuGradientBoostedTreeMachineLearningModelNew COMMAND otbLearningBaseTestDriver
  otbGradientBoostedTreeMachineLearningModelNew)

otb_add_test(NAME leTuANNMachineLearningModelNew COMMAND otbLearningBaseTestDriver
  otbANNMachineLearningModelNew)

otb_add_test(NAME leTvKNearestNeighborsMachineLearningModel COMMAND otbLearningBaseTestDriver
  otbKNearestNeighborsMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/knn_model.txt
  )

otb_add_test(NAME leTvDecisionTreeMachineLearningModel COMMAND otbLearningBaseTestDriver
  otbDecisionTreeMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/decisiontree_model.txt
  )

otb_add_test(NAME leTvBoostMachineLearningModel COMMAND otbLearningBaseTestDriver
  otbBoostMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/boost_model.txt
  )

otb_add_test(NAME leTuImageClassificationFilterNew COMMAND otbLearningBaseTestDriver
  otbImageClassificationFilterNew)

otb_add_test(NAME leTvImageClassificationFilterLibSVM COMMAND otbLearningBaseTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leSVMImageClassificationFilterOutput.tif
  ${TEMP}/leImageClassificationFilterLibSVMOutput.tif
  otbImageClassificationFilter
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/svm_model_image
  ${TEMP}/leImageClassificationFilterLibSVMOutput.tif
  )

otb_add_test(NAME leTvImageClassificationFilterSVM COMMAND otbLearningBaseTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leImageClassificationFilterSVMOutput.tif
  ${TEMP}/leImageClassificationFilterSVMOutput.tif
  otbImageClassificationFilter
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/ROI_QB_MUL_4_svmModel.txt
  ${TEMP}/leImageClassificationFilterSVMOutput.tif
  )

otb_add_test(NAME leTuDecisionTreeNew COMMAND otbLearningBaseTestDriver
  otbDecisionTreeNew)

otb_add_test(NAME leTuKMeansImageClassificationFilterNew COMMAND otbLearningBaseTestDriver
  otbKMeansImageClassificationFilterNew)

