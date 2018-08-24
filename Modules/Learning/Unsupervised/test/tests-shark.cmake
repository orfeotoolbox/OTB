# kMeans Shark related tests



otb_add_test(NAME leTvSharkKMeansMachineLearningModel COMMAND otbUnsupervisedTestDriver
  otbSharkKMeansMachineLearningModelTrain
  ${INPUTDATA}/letter.scale
  ${TEMP}/shark_km_model.txt
  )

otb_add_test(NAME otbSharkKMeansMachineLearningModelPredict COMMAND otbUnsupervisedTestDriver
  otbSharkKMeansMachineLearningModelPredict
  ${INPUTDATA}/letter.scale
  ${INPUTDATA}/Classification/shark_km_model.txt
  )

otb_add_test(NAME leTvSharkKMeansMachineLearningModelCanRead COMMAND otbUnsupervisedTestDriver
  otbSharkKMeansMachineLearningModelCanRead
  ${INPUTDATA}/Classification/otbSharkImageClassificationFilter_KMeansmodel.txt
  )

otb_add_test(NAME leTvSharkKMeansMachineLearningModelCanReadFail COMMAND otbUnsupervisedTestDriver
  otbSharkKMeansMachineLearningModelCanRead
  ${INPUTDATA}/Classification/otbSharkImageClassificationFilter_RFmodel.txt
  )

set_property(TEST leTvSharkKMeansMachineLearningModelCanReadFail PROPERTY WILL_FAIL true)



otb_add_test(NAME leTvImageClassificationFilterSharkKMeans COMMAND  otbUnsupervisedTestDriver
  --compare-n-images ${NOTOL} 1
  ${BASELINE}/leSharkUnsupervisedImageClassificationFilterOutput.tif
  ${TEMP}/leSharkUnsupervisedImageClassificationFilterOutput.tif
  otbSharkUnsupervisedImageClassificationFilter
  ${INPUTDATA}/Classification/QB_1_ortho.tif
  ${TEMP}/leSharkUnsupervisedImageClassificationFilterOutput.tif
  ${TEMP}/leSharkUnsupervisedImageClassificationFilterConfidence.tif
  1
  ${INPUTDATA}/Classification/otbSharkImageClassificationFilter_KMeansmodel.txt
  )


otb_add_test(NAME leTvImageClassificationFilterSharkKMeansMask COMMAND  otbUnsupervisedTestDriver
  --compare-n-images ${NOTOL} 1
  ${BASELINE}/leSharkUnsupervisedImageClassificationFilterWithMaskOutput.tif
  ${TEMP}/leSharkUnsupervisedImageClassificationFilterWithMaskOutput.tif
  otbSharkUnsupervisedImageClassificationFilter
  ${INPUTDATA}/Classification/QB_1_ortho.tif
  ${TEMP}/leSharkUnsupervisedImageClassificationFilterWithMaskOutput.tif
  ${TEMP}/leSharkUnsupervisedImageClassificationFilterWithMaskConfidence.tif
  1
  ${INPUTDATA}/Classification/otbSharkImageClassificationFilter_KMeansmodel.txt
  ${INPUTDATA}/Classification/QB_1_ortho_mask.tif
  )
