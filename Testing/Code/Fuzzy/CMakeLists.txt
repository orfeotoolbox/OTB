IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE       ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA      ${OTB_DATA_ROOT}/Input)
SET(TEMP           ${OTBTesting_BINARY_DIR}/Temporary)

#Remote sensing images (large images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(LARGEINPUT ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

#Tolerance sur diff pixel image
SET(NOTOL 0.0)
SET(EPSILON_9 0.000000001)

#Tolerance sur la difference des valeurs numeriques dans le --compare-ascii
SET(EPSILON_3 0.001)

# Common generic tests
SET(Fuzzy_TESTS1 ${CXX_TEST_PATH}/otbFuzzyTests1)
# Advanced tests
SET(Fuzzy_TESTS2 ${CXX_TEST_PATH}/otbFuzzyTests2)

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
 ${OTB_DATA_LARGEINPUT_ROOT}/DEMPSTER-SHAFER/ROI_QB_RANGUEUIL_ROADS.shp
 ${OTB_DATA_LARGEINPUT_ROOT}/Dempster-Shafer/ROI_QB_RANGUEUIL.TIF
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

SET(BasicFuzzy_SRCS1
otbFuzzyVariableNew.cxx
otbFuzzyVariable2Values.cxx
otbFuzzyVariableSetValue.cxx
otbFuzzyVariableGetMaxVar.cxx
otbMassOfBelief.cxx
otbJointMassOfBeliefFilter.cxx
)

SET(BasicFuzzy_SRCS2
otbFuzzyVariableDSApplied.cxx
otbMassOfBeliefDSApplied.cxx
otbVectorDataToDSValidatedVectorDataFilter.cxx
otbVectorDataToSpecificDescriptionFilterBase.cxx
otbVectorDataToRoadDescriptionFilter.cxx
)

ADD_EXECUTABLE(otbFuzzyTests1 otbFuzzyTests1.cxx ${BasicFuzzy_SRCS1})
TARGET_LINK_LIBRARIES(otbFuzzyTests1 OTBIO OTBFuzzy  OTBTesting)

ADD_EXECUTABLE(otbFuzzyTests2 otbFuzzyTests2.cxx ${BasicFuzzy_SRCS2})
TARGET_LINK_LIBRARIES(otbFuzzyTests2 OTBIO OTBFuzzy OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
