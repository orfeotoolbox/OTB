otb_module_test()

set(OTBSupervisedTests
otbSupervisedTestDriver.cxx
otbConfusionMatrixCalculatorTest.cxx
otbConfusionMatrixMeasurementsTest.cxx
otbTrainMachineLearningModel.cxx
otbImageClassificationFilter.cxx
)

add_executable(otbSupervisedTestDriver ${OTBSupervisedTests})
target_link_libraries(otbSupervisedTestDriver ${OTBSupervised-Test_LIBRARIES})
otb_module_target_label(otbSupervisedTestDriver)

# Tests Declaration

otb_add_test(NAME leTvConfusionMatrixCalculatorWrongSize COMMAND otbSupervisedTestDriver
  otbConfusionMatrixCalculatorWrongSize 1000 4)
set_property(TEST leTvConfusionMatrixCalculatorWrongSize PROPERTY WILL_FAIL true)

otb_add_test(NAME leTvConfusionMatrixCalculatorUpdate COMMAND otbSupervisedTestDriver
  otbConfusionMatrixCalculatorCompute 4 3)

otb_add_test(NAME leTvConfusionMatrixCalculatorUpdateWithBaseline COMMAND otbSupervisedTestDriver
  otbConfusionMatrixCalculatorComputeWithBaseline)

otb_add_test(NAME leTvConfusionMatrixCalculatorSetListSamples COMMAND otbSupervisedTestDriver
  otbConfusionMatrixCalculatorSetListSamples 1000 4)

otb_add_test(NAME leTuConfusionMatrixCalculatorNew COMMAND otbSupervisedTestDriver
  otbConfusionMatrixCalculatorNew)

otb_add_test(NAME leTvConfusionMatrixMeasurementsTest COMMAND otbSupervisedTestDriver
  otbConfusionMatrixMeasurementsTest)

otb_add_test(NAME leTuConfusionMatrixMeasurementsNew COMMAND otbSupervisedTestDriver
  otbConfusionMatrixMeasurementsNew)

otb_add_test(NAME leTvConfusionMatrixConcatenateTest COMMAND otbSupervisedTestDriver
  otbConfusionMatrixConcatenateTest
  ${INPUTDATA}/Classification/QB_1_ortho_C1.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C2.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C3.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C4.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C5.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C6.csv)

otb_add_test(NAME leTuNormalBayesMachineLearningModelNew COMMAND otbSupervisedTestDriver
  otbNormalBayesMachineLearningModelNew)

otb_add_test(NAME leTuRandomForestsMachineLearningModelNew COMMAND otbSupervisedTestDriver
  otbRandomForestsMachineLearningModelNew)

otb_add_test(NAME leTvLibSVMMachineLearningModel COMMAND otbSupervisedTestDriver
  otbLibSVMMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/libsvm_model.txt
  )

otb_add_test(NAME leTvANNMachineLearningModel COMMAND otbSupervisedTestDriver
  otbANNMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/ann_model.txt
  )

otb_add_test(NAME leTuSVMMachineLearningModelNew COMMAND otbSupervisedTestDriver
  otbSVMMachineLearningModelNew)

otb_add_test(NAME leTuDecisionTreeMachineLearningModelNew COMMAND otbSupervisedTestDriver
  otbDecisionTreeMachineLearningModelNew)

otb_add_test(NAME leTuKNearestNeighborsMachineLearningModelNew COMMAND otbSupervisedTestDriver
  otbKNearestNeighborsMachineLearningModelNew)

otb_add_test(NAME leTvSVMMachineLearningRegressionModel COMMAND otbSupervisedTestDriver
  otbSVMMachineLearningRegressionModel
  ${INPUTDATA}/abalone.scale
  ${TEMP}/svm_model_regression.txt
  )

otb_add_test(NAME leTvSVMMachineLearningModel COMMAND otbSupervisedTestDriver
  otbSVMMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/svm_model.txt
  )

otb_add_test(NAME leTuBoostMachineLearningModelNew COMMAND otbSupervisedTestDriver
  otbBoostMachineLearningModelNew)

otb_add_test(NAME leTuLibSVMMachineLearningModelNew COMMAND otbSupervisedTestDriver
  otbLibSVMMachineLearningModelNew)

otb_add_test(NAME leTvNormalBayesMachineLearningModel COMMAND otbSupervisedTestDriver
  otbNormalBayesMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/normalbayes_model.txt
  )

otb_add_test(NAME leTvGradientBoostedTreeMachineLearningModel COMMAND otbSupervisedTestDriver
  otbGradientBoostedTreeMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/gbt_model.txt
  )

otb_add_test(NAME leTvRandomForestsMachineLearningModel COMMAND otbSupervisedTestDriver
  otbRandomForestsMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/rf_model.txt
  )

otb_add_test(NAME leTuGradientBoostedTreeMachineLearningModelNew COMMAND otbSupervisedTestDriver
  otbGradientBoostedTreeMachineLearningModelNew)

otb_add_test(NAME leTuANNMachineLearningModelNew COMMAND otbSupervisedTestDriver
  otbANNMachineLearningModelNew)

otb_add_test(NAME leTvKNearestNeighborsMachineLearningModel COMMAND otbSupervisedTestDriver
  otbKNearestNeighborsMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/knn_model.txt
  )

otb_add_test(NAME leTvDecisionTreeMachineLearningModel COMMAND otbSupervisedTestDriver
  otbDecisionTreeMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/decisiontree_model.txt
  )

otb_add_test(NAME leTvBoostMachineLearningModel COMMAND otbSupervisedTestDriver
  otbBoostMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/boost_model.txt
  )

otb_add_test(NAME leTuImageClassificationFilterNew COMMAND otbSupervisedTestDriver
  otbImageClassificationFilterNew)

otb_add_test(NAME leTvImageClassificationFilterLibSVM COMMAND otbSupervisedTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leSVMImageClassificationFilterOutput.tif
  ${TEMP}/leImageClassificationFilterLibSVMOutput.tif
  otbImageClassificationFilter
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/svm_model_image
  ${TEMP}/leImageClassificationFilterLibSVMOutput.tif
  )

otb_add_test(NAME leTvImageClassificationFilterSVM COMMAND otbSupervisedTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leImageClassificationFilterSVMOutput.tif
  ${TEMP}/leImageClassificationFilterSVMOutput.tif
  otbImageClassificationFilter
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/ROI_QB_MUL_4_svmModel.txt
  ${TEMP}/leImageClassificationFilterSVMOutput.tif
  )
