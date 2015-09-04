otb_add_test(NAME leTvLibSVMMachineLearningModel COMMAND otbSupervisedTestDriver
  otbLibSVMMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/libsvm_model.txt
  )
otb_add_test(NAME leTuLibSVMMachineLearningModelNew COMMAND otbSupervisedTestDriver
  otbLibSVMMachineLearningModelNew)


otb_add_test(NAME leTvImageClassificationFilterLibSVM COMMAND otbSupervisedTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leSVMImageClassificationFilterOutput.tif
  ${TEMP}/leImageClassificationFilterLibSVMOutput.tif
  otbImageClassificationFilter
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/svm_model_image
  ${TEMP}/leImageClassificationFilterLibSVMOutput.tif
  )

otb_add_test(NAME leTuLibSVMMachineLearningModelCanRead COMMAND otbSupervisedTestDriver
  otbLibSVMMachineLearningModelCanRead
  ${TEMP}/libsvm_model.txt
  )
set_property(TEST leTuLibSVMMachineLearningModelCanRead PROPERTY DEPENDS leTvLibSVMMachineLearningModel)

otb_add_test(NAME leTvLibSVMMachineLearningModelReg COMMAND otbSupervisedTestDriver
  otbLibSVMRegressionTests
  )
