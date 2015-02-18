otb_module_test()

set(OTBDempsterShaferTests
otbDempsterShaferTestDriver.cxx
otbMassOfBeliefDSApplied.cxx
otbConfusionMatrixToMassOfBeliefTest.cxx
otbVectorDataToDSValidatedVectorDataFilter.cxx
otbDempsterShaferFusionTests.cxx
otbDSFusionOfClassifiersImageFilterTest.cxx
otbJointMassOfBeliefFilter.cxx
otbMassOfBelief.cxx
otbStandardDSCostFunction.cxx
)

add_executable(otbDempsterShaferTestDriver ${OTBDempsterShaferTests})
target_link_libraries(otbDempsterShaferTestDriver ${OTBDempsterShafer-Test_LIBRARIES})
otb_module_target_label(otbDempsterShaferTestDriver)

# Tests Declaration

otb_add_test(NAME fzTvMassOfBeliefDSApplied-H1_H2_ COMMAND otbDempsterShaferTestDriver
  otbMassOfBeliefDSApplied
  "H1_"
  "H2_" # here the hypothesis is {H1_, H2_}
  0.9
  0.9
  )

otb_add_test(NAME fzTvMassOfBeliefDSApplied-H1_H2 COMMAND otbDempsterShaferTestDriver
  otbMassOfBeliefDSApplied
  "H1_"
  "H2"  # here the hypothesis is {H1_, H2}
  0.9
  0.9
  )

otb_add_test(NAME fzTvMassOfBeliefDSApplied-H1H2 COMMAND otbDempsterShaferTestDriver
  otbMassOfBeliefDSApplied
  "H1"  # describe the hypothesis
  "H2"  # here the hypothesis is {H1, H2}
  0.9
  0.9
  )

otb_add_test(NAME fzTvMassOfBeliefDSApplied-H1H2_ COMMAND otbDempsterShaferTestDriver
  otbMassOfBeliefDSApplied
  "H1"
  "H2_"  # here the hypothesis is {H1, H2_}
  0.9
  0.9
  )

otb_add_test(NAME fzTvConfusionMatrixToMassOfBeliefTestAccuracy COMMAND otbDempsterShaferTestDriver
  otbConfusionMatrixToMassOfBeliefTest
  ACCURACY)

otb_add_test(NAME fzTuConfusionMatrixToMassOfBeliefNew COMMAND otbDempsterShaferTestDriver
  otbConfusionMatrixToMassOfBeliefNew)

otb_add_test(NAME fzTvConfusionMatrixToMassOfBeliefTestPrecision COMMAND otbDempsterShaferTestDriver
  otbConfusionMatrixToMassOfBeliefTest
  PRECISION)

otb_add_test(NAME fzTvConfusionMatrixToMassOfBeliefTestKappa COMMAND otbDempsterShaferTestDriver
  otbConfusionMatrixToMassOfBeliefTest
  KAPPA)

otb_add_test(NAME fzTvConfusionMatrixToMassOfBeliefTestRecall COMMAND otbDempsterShaferTestDriver
  otbConfusionMatrixToMassOfBeliefTest
  RECALL)

otb_add_test(NAME fzTvVectorDataToDSValidatedVectorDataFilter COMMAND otbDempsterShaferTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/fzTvDSValidatedRoadVectorDataOutput.shp
  ${TEMP}/fzTvDSValidatedRoadVectorDataOutput.shp
  otbVectorDataToDSValidatedVectorDataFilter
  ${TEMP}/fzTvVectorDataToRoadDescriptionFilterOutput.shp
  ${TEMP}/fzTvDSValidatedRoadVectorDataOutput.shp
  )
set_property(TEST fzTvVectorDataToDSValidatedVectorDataFilter PROPERTY DEPENDS fzTvVectorDataToRoadDescriptionFilter)

otb_add_test(NAME fzTuVectorDataToDSValidatedVectorDataFilterNew COMMAND otbDempsterShaferTestDriver
  otbVectorDataToDSValidatedVectorDataFilterNew)

otb_add_test(NAME fzDSFusionTestConfMatFileVCMTestPrecision COMMAND otbDempsterShaferTestDriver
  otbDempsterShaferFusionConfMatFileTest
  ${INPUTDATA}/Classification/QB_1_ortho_C1_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C2_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C3_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C4_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C5_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C6_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C7_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C8_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C9_V.csv
  PRECISION
  )

otb_add_test(NAME fzDSFusionOptRecTestConfMatFileRCMTestPrecision COMMAND otbDempsterShaferTestDriver
  otbDempsterShaferFusionOptRecConfMatFileTest
  ${INPUTDATA}/Classification/QB_1_ortho_C1.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C2.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C3.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C4.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C5.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C6.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C7.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C8.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C9.csv
  PRECISION)

otb_add_test(NAME fzDSFusionTestConfMatFileRCMTestPrecision COMMAND otbDempsterShaferTestDriver
  otbDempsterShaferFusionConfMatFileTest
  ${INPUTDATA}/Classification/QB_1_ortho_C1.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C2.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C3.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C4.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C5.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C6.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C7.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C8.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C9.csv
  PRECISION
  )

otb_add_test(NAME fzDSFusionOptRecTestConfMatTestAccuracy COMMAND otbDempsterShaferTestDriver
  otbDempsterShaferFusionOptRecConfMatTest
  ACCURACY)

otb_add_test(NAME fzDSFusionTestWithExtraLabels COMMAND otbDempsterShaferTestDriver
  otbDempsterShaferFusionTest
  a b c d e
  )

otb_add_test(NAME fzDSFusionOptRecTestConfMatFileVCMTestPrecision COMMAND otbDempsterShaferTestDriver
  otbDempsterShaferFusionOptRecConfMatFileTest
  ${INPUTDATA}/Classification/QB_1_ortho_C1_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C2_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C3_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C4_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C5_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C6_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C7_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C8_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C9_V.csv
  PRECISION)

otb_add_test(NAME fzDSFusionOptRecTestConfMatTestKappa COMMAND otbDempsterShaferTestDriver
  otbDempsterShaferFusionOptRecConfMatTest
  KAPPA)

otb_add_test(NAME fzDSFusionOptRecTestConfMatTestPrecision COMMAND otbDempsterShaferTestDriver
  otbDempsterShaferFusionOptRecConfMatTest
  PRECISION)

otb_add_test(NAME fzDSFusionOptTestWithExtraLabels COMMAND otbDempsterShaferTestDriver
  otbDempsterShaferFusionOptTest
  a b c d e
  )

otb_add_test(NAME fzDSFusionOptRecTestConfMatTestRecall COMMAND otbDempsterShaferTestDriver
  otbDempsterShaferFusionOptRecConfMatTest
  RECALL)

otb_add_test(NAME fzDSFusionOptRecTestSimple COMMAND otbDempsterShaferTestDriver
  otbDempsterShaferFusionOptRecTest
  a b c
  )

otb_add_test(NAME fzDSFusionOptRecTestWithExtraLabelsBig COMMAND otbDempsterShaferTestDriver
  otbDempsterShaferFusionOptRecTest
  a b c d e f g h i j k l m
  n o p q r s t u v w x y z
  )

otb_add_test(NAME fzDSFusionOptTestSimple COMMAND otbDempsterShaferTestDriver
  otbDempsterShaferFusionOptTest
  a b c
  )

otb_add_test(NAME fzDSFusionTestSimple COMMAND otbDempsterShaferTestDriver
  otbDempsterShaferFusionTest
  a b c
  )

otb_add_test(NAME fzDSFusionOptRecTestWithExtraLabelsSmall COMMAND otbDempsterShaferTestDriver
  otbDempsterShaferFusionOptRecTest
  a b c d e
  )

otb_add_test(NAME fzTvDSFusionOfClassifiersImageFilter9ClkVCMPrecision COMMAND otbDempsterShaferTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/QB_1_ortho_DS_FUSED_9_Clk_VCM_PRECISION.tif
  ${TEMP}/QB_1_ortho_DS_FUSED_9_Clk_VCM_PRECISION.tif
  otbDSFusionOfClassifiersImageFilterTest
  ${INPUTDATA}/Classification/QB_1_ortho_C1.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C2.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C3.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C4.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C5.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C6.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C7.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C8.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C9.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C1_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C2_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C3_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C4_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C5_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C6_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C7_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C8_V.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C9_V.csv
  PRECISION # Method to estimate the Masses of Belief of each label
  10 #LabelForNoDataPixels
  7 #LabelForUndecidedPixels
  ${TEMP}/QB_1_ortho_DS_FUSED_9_Clk_VCM_PRECISION.tif
  )

otb_add_test(NAME fzTvDSFusionOfClassifiersImageFilterPrecision COMMAND otbDempsterShaferTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/QB_1_ortho_DS_FUSED_PRECISION.tif
  ${TEMP}/QB_1_ortho_DS_FUSED_PRECISION.tif
  otbDSFusionOfClassifiersImageFilterTest
  ${INPUTDATA}/Classification/QB_1_ortho_C1.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C2.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C3.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C4.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C5.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C6.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C1.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C2.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C3.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C4.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C5.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C6.csv
  PRECISION # Method to estimate the Masses of Belief of each label
  10 #LabelForNoDataPixels
  7 #LabelForUndecidedPixels
  ${TEMP}/QB_1_ortho_DS_FUSED_PRECISION.tif
  )

otb_add_test(NAME fzTvDSFusionOfClassifiersImageFilterKappa COMMAND otbDempsterShaferTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/QB_1_ortho_DS_FUSED_KAPPA.tif
  ${TEMP}/QB_1_ortho_DS_FUSED_KAPPA.tif
  otbDSFusionOfClassifiersImageFilterTest
  ${INPUTDATA}/Classification/QB_1_ortho_C1.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C2.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C3.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C4.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C5.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C6.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C1.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C2.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C3.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C4.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C5.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C6.csv
  KAPPA # Method to estimate the Masses of Belief of each label
  10 #LabelForNoDataPixels
  7 #LabelForUndecidedPixels
  ${TEMP}/QB_1_ortho_DS_FUSED_KAPPA.tif
  )

otb_add_test(NAME fzTvDSFusionOfClassifiersImageFilter9ClkRCMPrecision COMMAND otbDempsterShaferTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/QB_1_ortho_DS_FUSED_9_Clk_RCM_PRECISION.tif
  ${TEMP}/QB_1_ortho_DS_FUSED_9_Clk_RCM_PRECISION.tif
  otbDSFusionOfClassifiersImageFilterTest
  ${INPUTDATA}/Classification/QB_1_ortho_C1.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C2.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C3.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C4.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C5.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C6.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C7.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C8.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C9.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C1.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C2.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C3.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C4.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C5.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C6.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C7.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C8.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C9.csv
  PRECISION # Method to estimate the Masses of Belief of each label
  10 #LabelForNoDataPixels
  7 #LabelForUndecidedPixels
  ${TEMP}/QB_1_ortho_DS_FUSED_9_Clk_RCM_PRECISION.tif
  )

otb_add_test(NAME fzTvDSFusionOfClassifiersImageFilterRecall COMMAND otbDempsterShaferTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/QB_1_ortho_DS_FUSED_RECALL.tif
  ${TEMP}/QB_1_ortho_DS_FUSED_RECALL.tif
  otbDSFusionOfClassifiersImageFilterTest
  ${INPUTDATA}/Classification/QB_1_ortho_C1.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C2.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C3.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C4.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C5.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C6.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C1.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C2.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C3.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C4.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C5.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C6.csv
  RECALL # Method to estimate the Masses of Belief of each label
  10 #LabelForNoDataPixels
  7 #LabelForUndecidedPixels
  ${TEMP}/QB_1_ortho_DS_FUSED_RECALL.tif
  )

otb_add_test(NAME fzTuDSFusionOfClassifiersImageFilterNew COMMAND otbDempsterShaferTestDriver
  otbDSFusionOfClassifiersImageFilterNew)

otb_add_test(NAME fzTvDSFusionOfClassifiersImageFilterAccuracy COMMAND otbDempsterShaferTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/QB_1_ortho_DS_FUSED_ACCURACY.tif
  ${TEMP}/QB_1_ortho_DS_FUSED_ACCURACY.tif
  otbDSFusionOfClassifiersImageFilterTest
  ${INPUTDATA}/Classification/QB_1_ortho_C1.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C2.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C3.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C4.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C5.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C6.tif
  ${INPUTDATA}/Classification/QB_1_ortho_C1.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C2.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C3.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C4.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C5.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C6.csv
  ACCURACY # Method to estimate the Masses of Belief of each label
  10 #LabelForNoDataPixels
  7 #LabelForUndecidedPixels
  ${TEMP}/QB_1_ortho_DS_FUSED_ACCURACY.tif
  )

otb_add_test(NAME fzTvJointMassOfBeliefFilterLimit COMMAND otbDempsterShaferTestDriver
  otbJointMassOfBeliefFilterLimit)

otb_add_test(NAME fzTvJointMassOfBeliefFilter COMMAND otbDempsterShaferTestDriver
  otbJointMassOfBeliefFilter)

otb_add_test(NAME fzTvMassOfBelief COMMAND otbDempsterShaferTestDriver
  otbMassOfBelief)

otb_add_test(NAME fzTuStandardDSCostFunctionNew COMMAND otbDempsterShaferTestDriver
  otbStandardDSCostFunctionNew)

