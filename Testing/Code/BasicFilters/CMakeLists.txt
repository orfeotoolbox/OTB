
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)

SET(BASICFILTERS_TESTS ${CXX_TEST_PATH}/otbBasicFiltersTests)


ADD_TEST(bfTvFiltreLee1CanalPoupees ${BASICFILTERS_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/bfFiltreLee_05_05_04.png
                          ${TEMP}/bfFiltreLee_05_05_04.png  
        otbLeeFilter
	${INPUTDATA}/poupees_1canal.c1.hdr
	${TEMP}/bfFiltreLee_05_05_04.png
	05 05 4.0)

ADD_TEST(bfTvFiltreLee ${BASICFILTERS_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/bfFiltreLee_05_05_12.png
                           ${TEMP}/bfFiltreLee_05_05_12.png  
        otbLeeFilter
	${INPUTDATA}/poupees.hdr
	${TEMP}/bfFiltreLee_05_05_12.png
	05 05 12.0)

#
# Filtre de FROST:
#
	
ADD_TEST(bfTvFiltreFrost ${BASICFILTERS_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/bfFiltreFrost_poupees_05_05_01.png
                          ${TEMP}/bfFiltreFrost_poupees_05_05_01.png 
        otbFrostFilter
	${INPUTDATA}/poupees.hdr
	${TEMP}/bfFiltreFrost_poupees_05_05_01.png
	05 05 0.1)

# -------            otb::PointSetSource   ------------------------------

ADD_TEST(bfTuImageToPointSetFilterTest ${BASICFILTERS_TESTS} 
         otbImageToPointSetFilterTest)  

# A enrichir
SET(BasicFilters_SRCS
otbLeeFilter.cxx
otbFrostFilterTest.cxx
otbFrostFilterNew.cxx
otbFrostFilter.cxx
otbImageToPointSetFilterTest.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbBasicFiltersTests otbBasicFiltersTests.cxx ${BasicFilters_SRCS})
TARGET_LINK_LIBRARIES(otbBasicFiltersTests  OTBBasicFilters OTBCommon OTBIO gdal ITKIO ITKCommon)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




