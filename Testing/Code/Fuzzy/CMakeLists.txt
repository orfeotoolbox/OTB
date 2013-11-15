if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


# Common generic tests
set(Fuzzy_TESTS1 ${CXX_TEST_PATH}/otbFuzzyTests1)
# Advanced tests
set(Fuzzy_TESTS2 ${CXX_TEST_PATH}/otbFuzzyTests2)
set(Fuzzy_TESTS3 ${CXX_TEST_PATH}/otbFuzzyTests3)

# FuzzyTests1
add_test(fzTuFuzzyVariableNew ${Fuzzy_TESTS1}
        otbFuzzyVariableNew)

add_test(fzTvFuzzyVariable2Values ${Fuzzy_TESTS1}
        otbFuzzyVariable2Values)

add_test(fzTvFuzzyVariableSetValue ${Fuzzy_TESTS1}
        otbFuzzyVariableSetValue)

add_test(fzTvFuzzyVariableGetMaxVar ${Fuzzy_TESTS1}
        otbFuzzyVariableGetMaxVar)

add_test(fzTvMassOfBelief ${Fuzzy_TESTS1}
        otbMassOfBelief)

add_test(fzTvJointMassOfBeliefFilter ${Fuzzy_TESTS1}
        otbJointMassOfBeliefFilter)

add_test(fzTvJointMassOfBeliefFilterLimit ${Fuzzy_TESTS1}
        otbJointMassOfBeliefFilterLimit)

# FuzzyTests2
add_test(fzTvFuzzyVariableDSApplied ${Fuzzy_TESTS2}
        otbFuzzyVariableDSApplied)

add_test(fzTvMassOfBeliefDSApplied-H1H2 ${Fuzzy_TESTS2}
        otbMassOfBeliefDSApplied
        "H1"  # describe the hypothesis
        "H2"  # here the hypothesis is {H1, H2}
        0.9
        0.9
        )

add_test(fzTvMassOfBeliefDSApplied-H1H2_ ${Fuzzy_TESTS2}
        otbMassOfBeliefDSApplied
        "H1"
        "H2_"  # here the hypothesis is {H1, H2_}
        0.9
        0.9
        )

add_test(fzTvMassOfBeliefDSApplied-H1_H2 ${Fuzzy_TESTS2}
        otbMassOfBeliefDSApplied
        "H1_"
        "H2"  # here the hypothesis is {H1_, H2}
        0.9
        0.9
        )

add_test(fzTvMassOfBeliefDSApplied-H1_H2_ ${Fuzzy_TESTS2}
        otbMassOfBeliefDSApplied
        "H1_"
        "H2_" # here the hypothesis is {H1_, H2_}
        0.9
        0.9
        )

add_test(fzTuVectorDataToSpecificDescriptionFilterBaseNew ${Fuzzy_TESTS2}
        otbVectorDataToSpecificDescriptionFilterBaseNew)

add_test(fzTuVectorDataToRoadDescriptionFilterNew ${Fuzzy_TESTS2}
        otbVectorDataToRoadDescriptionFilterNew)

add_test(fzTvVectorDataToRoadDescriptionFilter ${Fuzzy_TESTS2}
 --compare-ogr ${NOTOL}
 ${BASELINE_FILES}/fzTvVectorDataToRoadDescriptionFilterOutput.shp
 ${TEMP}/fzTvVectorDataToRoadDescriptionFilterOutput.shp
 otbVectorDataToRoadDescriptionFilter
 ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_ROADS.shp
 ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
 ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_BUILDINGS.shp
 ${TEMP}/fzTvVectorDataToRoadDescriptionFilterOutput.shp
 ${INPUTDATA}/DEM/srtm_directory
 0
 )

add_test(fzTuVectorDataToDSValidatedVectorDataFilterNew ${Fuzzy_TESTS2}
        otbVectorDataToDSValidatedVectorDataFilterNew)

add_test(fzTvVectorDataToDSValidatedVectorDataFilter ${Fuzzy_TESTS2}
 --compare-ogr ${NOTOL}
 ${BASELINE_FILES}/fzTvDSValidatedRoadVectorDataOutput.shp
 ${TEMP}/fzTvDSValidatedRoadVectorDataOutput.shp
  otbVectorDataToDSValidatedVectorDataFilter
    ${TEMP}/fzTvVectorDataToRoadDescriptionFilterOutput.shp
    ${TEMP}/fzTvDSValidatedRoadVectorDataOutput.shp
)
set_tests_properties(fzTvVectorDataToDSValidatedVectorDataFilter PROPERTIES DEPENDS fzTvVectorDataToRoadDescriptionFilter)


add_test(fzTuFuzzyDescriptorsModelManagerNew ${Fuzzy_TESTS2}
  otbFuzzyDescriptorsModelManagerNew)

add_test(fzTvFuzzyDescriptorsModelManagerTest ${Fuzzy_TESTS2}
  otbFuzzyDescriptorsModelManagerTest
${TEMP}/fzTvFuzzyDescriptorsModelManager.xml
)

add_test(fzTuStandardDSCostFunctionNew ${Fuzzy_TESTS2}
  otbStandardDSCostFunctionNew)

add_test(fzDSFusionTestSimple ${Fuzzy_TESTS3}
  otbDempsterShaferFusionTest
 a b c
 )

 add_test(fzDSFusionTestWithExtraLabels ${Fuzzy_TESTS3}
  otbDempsterShaferFusionTest
 a b c d e
 )

 add_test(fzDSFusionOptTestSimple ${Fuzzy_TESTS3}
  otbDempsterShaferFusionOptTest
 a b c
 )

add_test(fzDSFusionOptTestWithExtraLabels ${Fuzzy_TESTS3}
  otbDempsterShaferFusionOptTest
 a b c d e
 )

 add_test(fzDSFusionOptRecTestSimple ${Fuzzy_TESTS3}
  otbDempsterShaferFusionOptRecTest
 a b c
 )

add_test(fzDSFusionOptRecTestWithExtraLabelsSmall ${Fuzzy_TESTS3}
  otbDempsterShaferFusionOptRecTest
 a b c d e
 )

add_test(fzDSFusionOptRecTestWithExtraLabelsBig ${Fuzzy_TESTS3}
  otbDempsterShaferFusionOptRecTest
 a b c d e f g h i j k l m
 n o p q r s t u v w x y z
 )

add_test(fzDSFusionOptRecTestConfMatTestPrecision ${Fuzzy_TESTS3}
  otbDempsterShaferFusionOptRecConfMatTest
 PRECISION)

add_test(fzDSFusionOptRecTestConfMatTestRecall ${Fuzzy_TESTS3}
  otbDempsterShaferFusionOptRecConfMatTest
 RECALL)

add_test(fzDSFusionOptRecTestConfMatTestAccuracy ${Fuzzy_TESTS3}
  otbDempsterShaferFusionOptRecConfMatTest
 ACCURACY)

add_test(fzDSFusionOptRecTestConfMatTestKappa ${Fuzzy_TESTS3}
  otbDempsterShaferFusionOptRecConfMatTest
 KAPPA)

 add_test(fzDSFusionTestConfMatFileRCMTestPrecision ${Fuzzy_TESTS3}
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

 add_test(fzDSFusionTestConfMatFileVCMTestPrecision ${Fuzzy_TESTS3}
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

 add_test(fzDSFusionOptRecTestConfMatFileRCMTestPrecision ${Fuzzy_TESTS3}
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

 add_test(fzDSFusionOptRecTestConfMatFileVCMTestPrecision ${Fuzzy_TESTS3}
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

add_test(fzTuConfusionMatrixToMassOfBeliefNew ${Fuzzy_TESTS3}
  otbConfusionMatrixToMassOfBeliefNew)

add_test(fzTvConfusionMatrixToMassOfBeliefTestPrecision ${Fuzzy_TESTS3}
  otbConfusionMatrixToMassOfBeliefTest
 PRECISION)

add_test(fzTvConfusionMatrixToMassOfBeliefTestRecall ${Fuzzy_TESTS3}
  otbConfusionMatrixToMassOfBeliefTest
 RECALL)

add_test(fzTvConfusionMatrixToMassOfBeliefTestAccuracy ${Fuzzy_TESTS3}
  otbConfusionMatrixToMassOfBeliefTest
 ACCURACY)

add_test(fzTvConfusionMatrixToMassOfBeliefTestKappa ${Fuzzy_TESTS3}
  otbConfusionMatrixToMassOfBeliefTest
 KAPPA)

add_test(fzTuDSFusionOfClassifiersImageFilterNew ${Fuzzy_TESTS3}
  otbDSFusionOfClassifiersImageFilterNew)

add_test(fzTvDSFusionOfClassifiersImageFilterPrecision ${Fuzzy_TESTS3}
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

 add_test(fzTvDSFusionOfClassifiersImageFilter9ClkRCMPrecision ${Fuzzy_TESTS3}
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

 add_test(fzTvDSFusionOfClassifiersImageFilter9ClkVCMPrecision ${Fuzzy_TESTS3}
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


add_test(fzTvDSFusionOfClassifiersImageFilterRecall ${Fuzzy_TESTS3}
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

 add_test(fzTvDSFusionOfClassifiersImageFilterAccuracy ${Fuzzy_TESTS3}
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

 add_test(fzTvDSFusionOfClassifiersImageFilterKappa ${Fuzzy_TESTS3}
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



set(BasicFuzzy_SRCS1
otbFuzzyTests1.cxx
otbFuzzyVariableNew.cxx
otbFuzzyVariable2Values.cxx
otbFuzzyVariableSetValue.cxx
otbFuzzyVariableGetMaxVar.cxx
otbMassOfBelief.cxx
otbJointMassOfBeliefFilter.cxx
)

set(BasicFuzzy_SRCS2
otbFuzzyTests2.cxx
otbFuzzyVariableDSApplied.cxx
otbMassOfBeliefDSApplied.cxx
otbVectorDataToDSValidatedVectorDataFilter.cxx
otbVectorDataToSpecificDescriptionFilterBase.cxx
otbVectorDataToRoadDescriptionFilter.cxx
otbFuzzyDescriptorsModelManager.cxx
otbStandardDSCostFunction.cxx
)

set(BasicFuzzy_SRCS3
otbFuzzyTests3.cxx
otbDempsterShaferFusionTests.cxx
otbConfusionMatrixToMassOfBeliefTest.cxx
otbDSFusionOfClassifiersImageFilterTest.cxx
)

add_executable(otbFuzzyTests1 otbFuzzyTests1.cxx ${BasicFuzzy_SRCS1})
target_link_libraries(otbFuzzyTests1 OTBIO OTBFuzzy  OTBTesting)

add_executable(otbFuzzyTests2 otbFuzzyTests2.cxx ${BasicFuzzy_SRCS2})
target_link_libraries(otbFuzzyTests2 OTBIO OTBFuzzy OTBTesting)

add_executable(otbFuzzyTests3 otbFuzzyTests3.cxx ${BasicFuzzy_SRCS3})
target_link_libraries(otbFuzzyTests3 OTBIO OTBFuzzy OTBTesting)

endif()
