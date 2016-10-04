otb_add_test(NAME leTuSharkRFMachineLearningModelNew COMMAND otbSupervisedTestDriver
  otbSharkRFMachineLearningModelNew)

otb_add_test(NAME leTvSharkRFMachineLearningModel COMMAND otbSupervisedTestDriver
  otbSharkRFMachineLearningModel
  ${INPUTDATA}/letter.scale
  ${TEMP}/shark_rf_model.txt
  )

otb_add_test(NAME leTuSharkRFMachineLearningModelCanRead COMMAND otbSupervisedTestDriver
  otbSharkRFMachineLearningModelCanRead
  ${INPUTDATA}/Classification/otbSharkImageClassificationFilter_RFmodel.txt
  )

otb_add_test(NAME leTuSharkRFMachineLearningModelCanReadFail COMMAND otbSupervisedTestDriver
  otbSharkRFMachineLearningModelCanRead
  ${INPUTDATA}/ROI_QB_MUL_4_svmModel.txt
  )
set_property(TEST leTuSharkRFMachineLearningModelCanReadFail PROPERTY WILL_FAIL true)


otb_add_test(NAME leTvImageClassificationFilterSharkFast COMMAND  otbSupervisedTestDriver
  --compare-n-images ${NOTOL} 2 
  ${BASELINE}/leSharkImageClassificationFilterOutput.tif
  ${TEMP}/leSharkImageClassificationFilterOutput.tif
  ${BASELINE}/leSharkImageClassificationFilterConfidence.tif
  ${TEMP}/leSharkImageClassificationFilterConfidence.tif   
  otbSharkImageClassificationFilter
  ${INPUTDATA}/Classification/QB_1_ortho.tif
  ${TEMP}/leSharkImageClassificationFilterOutput.tif
  ${TEMP}/leSharkImageClassificationFilterConfidence.tif
  1
  ${INPUTDATA}/Classification/otbSharkImageClassificationFilter_RFmodel.txt
  )

# This test has been added for benchmarking purposes. However, it is
# far too long to be part of regression testing

# otb_add_test(NAME leTvImageClassificationFilterSharkClassic COMMAND  otbSupervisedTestDriver
#   otbSharkImageClassificationFilter
#   ${INPUTDATA}/Classification/QB_1_ortho.tif
#   ${TEMP}/leSharkImageClassificationFilterOutput.tif
#   ${TEMP}/leSharkImageClassificationFilterConfidence.tif
#   0
#   ${INPUTDATA}/Classification/otbSharkImageClassificationFilter_RFmodel.txt
#   )

otb_add_test(NAME leTvImageClassificationFilterSharkFastMask COMMAND  otbSupervisedTestDriver
  --compare-n-images ${NOTOL} 2 
  ${BASELINE}/leSharkImageClassificationFilterWithMaskOutput.tif
  ${TEMP}/leSharkImageClassificationFilterWithMaskOutput.tif
  ${BASELINE}/leSharkImageClassificationFilterWithMaskConfidence.tif
  ${TEMP}/leSharkImageClassificationFilterWithMaskConfidence.tif   
  otbSharkImageClassificationFilter
  ${INPUTDATA}/Classification/QB_1_ortho.tif
  ${TEMP}/leSharkImageClassificationFilterWithMaskOutput.tif
  ${TEMP}/leSharkImageClassificationFilterWithMaskConfidence.tif
  1
  ${INPUTDATA}/Classification/otbSharkImageClassificationFilter_RFmodel.txt
  ${INPUTDATA}/Classification/QB_1_ortho_mask.tif
  )
