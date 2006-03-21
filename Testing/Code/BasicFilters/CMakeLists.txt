
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Images)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)

SET(BASICFILTERS_TESTS ${CXX_TEST_PATH}/otbBasicFiltersTests)


ADD_TEST(bfTuFiltreLee1CanalPoupees ${BASICFILTERS_TESTS}  
        otbLeeFilter
	${INPUTDATA}/poupees_1canal.hd
	${TEMP}/bfFiltreLee_05_05_04.png
	05 05 4.0)

ADD_TEST(bfTuFiltreLee ${BASICFILTERS_TESTS}  
        otbLeeFilter
	${INPUTDATA}/poupees.hd
	${TEMP}/bfFiltreLee_05_05_12.png
	05 05 12.0)
	
ADD_TEST(bfTuFiltreFrost ${BASICFILTERS_TESTS}  
        otbFrostFilter
	${INPUTDATA}/poupees.hd
	${TEMP}/bfFiltreFrost_poupees_05_05_01.png
	05 05 0.1)


# A enrichir
SET(BasicFilters_SRCS
otbLeeFilter.cxx
otbFrostFilter.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbBasicFiltersTests otbBasicFiltersTests.cxx ${BasicFilters_SRCS})
TARGET_LINK_LIBRARIES(otbBasicFiltersTests ${CAI_LIBRARY} ${GDAL_LIBRARY} OTBBasicFilters OTBCommon OTBIO ${CAI_LIBRARY} ITKIO ITKCommon)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




