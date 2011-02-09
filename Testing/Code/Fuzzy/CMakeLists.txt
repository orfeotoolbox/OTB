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

SET(BasicFuzzy_SRCS1
otbFuzzyVariableNew.cxx
otbFuzzyVariable2Values.cxx
otbFuzzyVariableSetValue.cxx
otbFuzzyVariableGetMaxVar.cxx
otbMassOfBelief.cxx
otbJointMassOfBeliefFilter.cxx
)

ADD_EXECUTABLE(otbFuzzyTests1 otbFuzzyTests1.cxx ${BasicFuzzy_SRCS1})
TARGET_LINK_LIBRARIES(otbFuzzyTests1 OTBIO OTBFuzzy  OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
