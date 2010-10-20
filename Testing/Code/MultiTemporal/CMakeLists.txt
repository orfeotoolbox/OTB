
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)
SET(EPSILON 0.0000000001)

SET(MULTITEMPORAL_TESTS1 ${CXX_TEST_PATH}/otbMultiTemporalTests1)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MULTITEMPORAL_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::testname   ------------------------------

ADD_TEST(cdTvCBAMI ${MULTITEMPORAL_TESTS1}
  --compare-image ${TOL}   ${BASELINE}/cdCBAMIImage.png
                    ${TEMP}/cdCBAMIImage.png
        otbCBAMIMultiTemporalTest
	 ${INPUTDATA}/GomaAvant.png
	 ${INPUTDATA}/GomaApres.png
	 2
	 ${TEMP}/cdCBAMIImage.png
	 )


# -------  CXX Source Files -----------------------------------
SET(BasicMultiTemporal_SRCS1 
otbMultiTemporalTests1.cxx 
)

OTB_ADD_EXECUTABLE(otbMultiTemporalTests1 "${BasicMultiTemporal_SRCS1}" "OTBMultiTemporal;OTBIO;OTBTesting")


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
