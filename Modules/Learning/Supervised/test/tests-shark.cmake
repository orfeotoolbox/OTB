otb_add_test(NAME leTuSharkRFMachineLearningModelNew COMMAND otbSupervisedTestDriver
  otbSharkRFMachineLearningModelNew)

otb_add_test(NAME leTvSharkRFMachineLearningModel COMMAND otbSupervisedTestDriver
  otbSharkRFMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/rf_model.txt
  )


