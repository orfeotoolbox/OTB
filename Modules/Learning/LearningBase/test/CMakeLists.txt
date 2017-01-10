otb_module_test()

set(OTBLearningBaseTests
otbLearningBaseTestDriver.cxx
otbDecisionTreeBuild.cxx
otbGaussianModelComponent.cxx
otbKMeansImageClassificationFilter.cxx
otbDecisionTreeWithRealValues.cxx
otbSEMClassifierNew.cxx
otbDecisionTreeNew.cxx
otbKMeansImageClassificationFilterNew.cxx
)

add_executable(otbLearningBaseTestDriver ${OTBLearningBaseTests})
target_link_libraries(otbLearningBaseTestDriver ${OTBLearningBase-Test_LIBRARIES})
otb_module_target_label(otbLearningBaseTestDriver)

# Tests Declaration

otb_add_test(NAME leTvDecisionTreeBuild COMMAND otbLearningBaseTestDriver
  otbDecisionTreeBuild)

otb_add_test(NAME leTuGaussianModelComponentNew COMMAND otbLearningBaseTestDriver
  otbGaussianModelComponentNew)

otb_add_test(NAME leTvDecisionTreeWithRealValues COMMAND otbLearningBaseTestDriver
  otbDecisionTreeWithRealValues)

otb_add_test(NAME leTvKMeansImageClassificationFilter COMMAND otbLearningBaseTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leKMeansImageClassificationFilterOutput.hdr
  ${TEMP}/leKMeansImageClassificationFilterOutput.hdr
  otbKMeansImageClassificationFilter
  ${INPUTDATA}/poupees_sub.png
  ${TEMP}/leKMeansImageClassificationFilterOutput.hdr
  2
  0 0 0 0
  255 255 255 255
  )

otb_add_test(NAME leTuSEMClassifierNew COMMAND otbLearningBaseTestDriver
  otbSEMClassifierNew)

otb_add_test(NAME leTuDecisionTreeNew COMMAND otbLearningBaseTestDriver
  otbDecisionTreeNew)

otb_add_test(NAME leTuKMeansImageClassificationFilterNew COMMAND otbLearningBaseTestDriver
  otbKMeansImageClassificationFilterNew)

