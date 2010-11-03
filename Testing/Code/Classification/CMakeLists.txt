
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(NOTOL 0.0)
SET(EPSILON_10 0.0000000001)

SET(CLASSIFICATION_TESTS1 ${CXX_TEST_PATH}/otbClassificationTests1)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CLASSIFICATION_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::testname   ------------------------------

ADD_TEST(clTvBinaryDecisionTreeClassifier ${CLASSIFICATION_TESTS1}
        otbBinaryDecisionTreeClassifierTest
	 )


# -------  CXX Source Files -----------------------------------
SET(BasicClassification_SRCS1 
otbBinaryDecisionTreeClassifierTest.cxx
)

OTB_ADD_EXECUTABLE(otbClassificationTests1 "${BasicClassification_SRCS1}" "OTBClassification;OTBIO;OTBTesting")


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
