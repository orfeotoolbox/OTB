otb_add_test(NAME leTuSharkRFMachineLearningModelNew COMMAND otbSharkLearningTestDriver
  otbSharkRFMachineLearningModelNew)

otb_add_test(NAME leTvSharkRFMachineLearningModel COMMAND otbSharkLearningTestDriver
  otbSharkRFMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/shark_rf_model.txt
  )

otb_add_test(NAME leTuSharkRFMachineLearningModelCanRead COMMAND otbSharkLearningTestDriver
  otbSharkRFMachineLearningModelCanRead
  ${TEMP}/shark_rf_model.txt
  )

otb_add_test(NAME leTuSharkRFMachineLearningModelCanReadFail COMMAND otbSharkLearningTestDriver
  otbSharkRFMachineLearningModelCanRead
  ${INPUTDATA}/ROI_QB_MUL_4_svmModel.txt
  )
set_property(TEST leTuSharkRFMachineLearningModelCanReadFail PROPERTY WILL_FAIL true)

otb_add_test(NAME leTvImageClassificationFilterSharkLoadModel COMMAND otbSharkLearningTestDriver
  otbImageClassificationFilterLoadModel
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${TEMP}/shark_rf_model.txt
  )

otb_add_test(NAME leTvImageClassificationFilterSharkFast COMMAND  otbSharkLearningTestDriver
  otbSharkImageClassificationFilter
  ${INPUTDATA}/Classification/QB_1_ortho.tif
  ${TEMP}/leSharkImageClassificationFilterOutput.tif
  1
  ${INPUTDATA}/Classification/otbSharkImageClassificationFilter_RFmodel.txt
  )

otb_add_test(NAME leTvImageClassificationFilterSharkClassic COMMAND  otbSharkLearningTestDriver
  otbSharkImageClassificationFilter
  ${INPUTDATA}/Classification/QB_1_ortho.tif
  ${TEMP}/leSharkImageClassificationFilterOutput.tif
  0
  ${INPUTDATA}/Classification/otbSharkImageClassificationFilter_RFmodel.txt
  )
