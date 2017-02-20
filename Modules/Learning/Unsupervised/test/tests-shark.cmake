# kMeans Shark related tests

otb_add_test(NAME leTvSharkKMeansMachineLearningModelNew COMMAND otbUnsupervisedTestDriver
  otbSharkKMeansMachineLearningModelNew
  )

otb_add_test(NAME leTvSharkKMeansMachineLearningModel COMMAND otbUnsupervisedTestDriver
  otbSharkKMeansMachineLearningModelTrain
  ${INPUTDATA}/letter.scale
  ${TEMP}/shark_km_model.txt
  )

otb_add_test(NAME leTvSharkKMeansMachineLearningModelCanRead COMMAND otbUnsupervisedTestDriver
  otbSharkKMeansMachineLearningModelPredict
  ${INPUTDATA}/letter.scale
  ${INPUTDATA}/Classification/otbSharkImageClassificationFilter_KMeansmodel.txt
  )

otb_add_test(NAME leTvSharkKMeansMachineLearningModelCanReadFail COMMAND otbUnsupervisedTestDriver
  otbSharkKMeansMachineLearningModelPredict
  ${INPUTDATA}/letter.scale
  ${INPUTDATA}/Classification/otbSharkImageClassificationFilter_RFmodel.txt
  )

set_property(TEST leTvSharkKMeansMachineLearningModelCanReadFail PROPERTY WILL_FAIL true)
