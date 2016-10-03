otb_module_test()

set(OTBMajorityVotingTests
otbMajorityVotingTestDriver.cxx
otbNeighborhoodMajorityVotingImageFilterNew.cxx
otbNeighborhoodMajorityVotingImageFilterTest.cxx
)

add_executable(otbMajorityVotingTestDriver ${OTBMajorityVotingTests})
target_link_libraries(otbMajorityVotingTestDriver ${OTBMajorityVoting-Test_LIBRARIES})
otb_module_target_label(otbMajorityVotingTestDriver)

# Tests Declaration

otb_add_test(NAME leTuNeighborhoodMajorityVotingImageFilterNew COMMAND otbMajorityVotingTestDriver
  otbNeighborhoodMajorityVotingImageFilterNew)

otb_add_test(NAME leTvNeighborhoodMajorityVotingImageFilterTest COMMAND otbMajorityVotingTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/QB_1_ortho_4Cls_N_Classified_OTB_NMV_x2_y5_nodv10_undv7.tif
  ${TEMP}/QB_1_ortho_4Cls_N_Classified_OTB_NMV_x2_y5_nodv10_undv7.tif
  otbNeighborhoodMajorityVotingImageFilterTest
  ${INPUTDATA}/Classification/QB_1_ortho_4Cls_N_Classified_OTB.tif
  ${TEMP}/QB_1_ortho_4Cls_N_Classified_OTB_NMV_x2_y5_nodv10_undv7.tif
  false #KeepOriginalLabelBool
  2 #xRadius
  5 #yRadius
  10 #LabelForNoDataPixels
  7 #LabelForUndecidedPixels
  0 #OnlyIsolatedPixels
  )

otb_add_test(NAME leTvNeighborhoodMajorityVotingIsolThresPixTest COMMAND otbMajorityVotingTestDriver
  otbNeighborhoodMajorityVotingImageFilterIsolatedTest
  )

otb_add_test(NAME leTvSVMImageClassificationFilterWithNeighborhoodMajorityVoting COMMAND otbMajorityVotingTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leSVMImageClassificationWithNMVFilterOutput.tif
  ${TEMP}/leSVMImageClassificationWithNMVFilterOutput.tif
  otbNeighborhoodMajorityVotingImageFilterTest
  ${TEMP}/leSVMImageClassificationFilterOutput.tif
  ${TEMP}/leSVMImageClassificationWithNMVFilterOutput.tif
  false
  )
set_property(TEST leTvSVMImageClassificationFilterWithNeighborhoodMajorityVoting PROPERTY DEPENDS leTvSVMImageClassificationFilter)

