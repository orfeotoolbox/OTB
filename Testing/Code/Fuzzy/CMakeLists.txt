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

# FuzzyTests2
ADD_TEST(fzTvFuzzyVariableDSApplied ${Fuzzy_TESTS2}
        otbFuzzyVariableDSApplied)

ADD_TEST(fzTvMassOfBeliefDSApplied ${Fuzzy_TESTS2}
        otbMassOfBeliefDSApplied)

#ADD_TEST(fzTuVectorDataToSpecificDescriptionFilterBaseNew ${Fuzzy_TESTS2}
#        otbVectorDataToSpecificDescriptionFilterBaseNew)

#ADD_TEST(fzTuVectorDataToRoadDescriptionFilterNew ${Fuzzy_TESTS2}
#        otbVectorDataToRoadDescriptionFilterNew)

#ADD_TEST(fzTvVectorDataToRoadDescriptionFilter ${Fuzzy_TESTS2}
  #--compare-ogr ${NOTOL}
  #${INPUTDATA}/.shp
  #${TEMP}/.shp
  #otbVectorDataToRoadDescriptionFilter
  #${INPUTDATA}/ROI_QB_RANGUEUIL_ROADS.shp
  #${INPUTDATA}/ROI_QB_RANGUEUIL_MUL_4.TIF
  #${TEMP}/fzTvVectorDataToRoadDescriptionFilterOutput.shp
  #)


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
#otbVectorDataToSpecificDescriptionFilterBase.cxx
#otbVectorDataToRoadDescriptionFilter.cxx
)

ADD_EXECUTABLE(otbFuzzyTests1 otbFuzzyTests1.cxx ${BasicFuzzy_SRCS1})
TARGET_LINK_LIBRARIES(otbFuzzyTests1 OTBIO OTBFuzzy  OTBTesting)

ADD_EXECUTABLE(otbFuzzyTests2 otbFuzzyTests2.cxx ${BasicFuzzy_SRCS2})
TARGET_LINK_LIBRARIES(otbFuzzyTests2 OTBIO OTBFuzzy OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
