IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


# Common generic tests
SET(Fuzzy_TESTS1 ${CXX_TEST_PATH}/otbFuzzyTests1)
# Advanced tests
SET(Fuzzy_TESTS2 ${CXX_TEST_PATH}/otbFuzzyTests2)
SET(Fuzzy_TESTS3 ${CXX_TEST_PATH}/otbFuzzyTests3)

# FuzzyTests1
ADD_TEST(fzTuFuzzyVariableNew ${Fuzzy_TESTS1}
        otbFuzzyVariableNew)

ADD_TEST(fzTvFuzzyVariable2Values ${Fuzzy_TESTS1}
        otbFuzzyVariable2Values)

ADD_TEST(fzTvFuzzyVariableSetValue ${Fuzzy_TESTS1}
        otbFuzzyVariableSetValue)

ADD_TEST(fzTvFuzzyVariableGetMaxVar ${Fuzzy_TESTS1}
        otbFuzzyVariableGetMaxVar)

ADD_TEST(fzTvMassOfBelief ${Fuzzy_TESTS1}
        otbMassOfBelief)

ADD_TEST(fzTvJointMassOfBeliefFilter ${Fuzzy_TESTS1}
        otbJointMassOfBeliefFilter)

ADD_TEST(fzTvJointMassOfBeliefFilterLimit ${Fuzzy_TESTS1}
        otbJointMassOfBeliefFilterLimit)

# FuzzyTests2
ADD_TEST(fzTvFuzzyVariableDSApplied ${Fuzzy_TESTS2}
        otbFuzzyVariableDSApplied)

ADD_TEST(fzTvMassOfBeliefDSApplied-H1H2 ${Fuzzy_TESTS2}
        otbMassOfBeliefDSApplied
        "H1"  # describe the hypothesis
        "H2"  # here the hypothesis is {H1, H2}
        0.9
        0.9
        )

ADD_TEST(fzTvMassOfBeliefDSApplied-H1H2_ ${Fuzzy_TESTS2}
        otbMassOfBeliefDSApplied
        "H1"
        "H2_"  # here the hypothesis is {H1, H2_}
        0.9
        0.9
        )

ADD_TEST(fzTvMassOfBeliefDSApplied-H1_H2 ${Fuzzy_TESTS2}
        otbMassOfBeliefDSApplied
        "H1_"
        "H2"  # here the hypothesis is {H1_, H2}
        0.9
        0.9
        )

ADD_TEST(fzTvMassOfBeliefDSApplied-H1_H2_ ${Fuzzy_TESTS2}
        otbMassOfBeliefDSApplied
        "H1_"
        "H2_" # here the hypothesis is {H1_, H2_}
        0.9
        0.9
        )

ADD_TEST(fzTuVectorDataToSpecificDescriptionFilterBaseNew ${Fuzzy_TESTS2}
        otbVectorDataToSpecificDescriptionFilterBaseNew)

ADD_TEST(fzTuVectorDataToRoadDescriptionFilterNew ${Fuzzy_TESTS2}
        otbVectorDataToRoadDescriptionFilterNew)

ADD_TEST(fzTvVectorDataToRoadDescriptionFilter ${Fuzzy_TESTS2}
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

ADD_TEST(fzTuVectorDataToDSValidatedVectorDataFilterNew ${Fuzzy_TESTS2}
        otbVectorDataToDSValidatedVectorDataFilterNew)

ADD_TEST(fzTvVectorDataToDSValidatedVectorDataFilter ${Fuzzy_TESTS2}
 --compare-ogr ${NOTOL}
 ${BASELINE_FILES}/fzTvDSValidatedRoadVectorDataOutput.shp
 ${TEMP}/fzTvDSValidatedRoadVectorDataOutput.shp
  otbVectorDataToDSValidatedVectorDataFilter
    ${TEMP}/fzTvVectorDataToRoadDescriptionFilterOutput.shp
    ${TEMP}/fzTvDSValidatedRoadVectorDataOutput.shp
)
SET_TESTS_PROPERTIES(fzTvVectorDataToDSValidatedVectorDataFilter PROPERTIES DEPENDS fzTvVectorDataToRoadDescriptionFilter)


ADD_TEST(fzTuFuzzyDescriptorsModelManagerNew ${Fuzzy_TESTS2}
  otbFuzzyDescriptorsModelManagerNew)

ADD_TEST(fzTvFuzzyDescriptorsModelManagerTest ${Fuzzy_TESTS2}
  otbFuzzyDescriptorsModelManagerTest
${TEMP}/fzTvFuzzyDescriptorsModelManager.xml
)

ADD_TEST(fzTuStandardDSCostFunctionNew ${Fuzzy_TESTS2}
  otbStandardDSCostFunctionNew)
  
ADD_TEST(fzDSFusionTestSimple ${Fuzzy_TESTS3}
  otbDempsterShaferFusionTest
 a b c
 )
 
 ADD_TEST(fzDSFusionTestWithExtraLabels ${Fuzzy_TESTS3}
  otbDempsterShaferFusionTest
 a b c d e
 )
 
 ADD_TEST(fzDSFusionOptTestSimple ${Fuzzy_TESTS3}
  otbDempsterShaferFusionOptTest
 a b c
 )
 
ADD_TEST(fzDSFusionOptTestWithExtraLabels ${Fuzzy_TESTS3}
  otbDempsterShaferFusionOptTest
 a b c d e
 )
 
 ADD_TEST(fzDSFusionOptRecTestSimple ${Fuzzy_TESTS3}
  otbDempsterShaferFusionOptRecTest
 a b c
 )
 
ADD_TEST(fzDSFusionOptRecTestWithExtraLabelsSmall ${Fuzzy_TESTS3}
  otbDempsterShaferFusionOptRecTest
 a b c d e
 )
  
ADD_TEST(fzDSFusionOptRecTestWithExtraLabelsBig ${Fuzzy_TESTS3}
  otbDempsterShaferFusionOptRecTest
 a b c d e f g h i j k l m
 n o p q r s t u v w x y z
 )


SET(BasicFuzzy_SRCS1
otbFuzzyTests1.cxx
otbFuzzyVariableNew.cxx
otbFuzzyVariable2Values.cxx
otbFuzzyVariableSetValue.cxx
otbFuzzyVariableGetMaxVar.cxx
otbMassOfBelief.cxx
otbJointMassOfBeliefFilter.cxx
)

SET(BasicFuzzy_SRCS2
otbFuzzyTests2.cxx
otbFuzzyVariableDSApplied.cxx
otbMassOfBeliefDSApplied.cxx
otbVectorDataToDSValidatedVectorDataFilter.cxx
otbVectorDataToSpecificDescriptionFilterBase.cxx
otbVectorDataToRoadDescriptionFilter.cxx
otbFuzzyDescriptorsModelManager.cxx
otbStandardDSCostFunction.cxx
)

SET(BasicFuzzy_SRCS3
otbFuzzyTests3.cxx
otbDempsterShaferFusionTests.cxx
)

ADD_EXECUTABLE(otbFuzzyTests1 otbFuzzyTests1.cxx ${BasicFuzzy_SRCS1})
TARGET_LINK_LIBRARIES(otbFuzzyTests1 OTBIO OTBFuzzy  OTBTesting)

ADD_EXECUTABLE(otbFuzzyTests2 otbFuzzyTests2.cxx ${BasicFuzzy_SRCS2})
TARGET_LINK_LIBRARIES(otbFuzzyTests2 OTBIO OTBFuzzy OTBTesting)

ADD_EXECUTABLE(otbFuzzyTests3 otbFuzzyTests3.cxx ${BasicFuzzy_SRCS3})
TARGET_LINK_LIBRARIES(otbFuzzyTests3 OTBIO OTBFuzzy OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
