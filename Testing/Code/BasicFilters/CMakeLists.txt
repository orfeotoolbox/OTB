
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

# -------            otb::LogPolarResampleImageFilter   ------------------------------

ADD_TEST(bfTuLogPolarResampleImageFilterNew ${BASICFILTERS_TESTS}  
        otbLogPolarResampleImageFilterNew )

ADD_TEST(bfTvLogPolarResampleImageFilter ${BASICFILTERS_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/bfLogPolarResampleImageFilter.hdr
                          ${TEMP}/bfLogPolarResampleImageFilter.hdr 
        otbLogPolarResampleImageFilter
        ${INPUTDATA}/DeuxCercles.hdr
	${TEMP}/bfLogPolarResampleImageFilter.hdr)

# -------            otb::PointSetSource   ------------------------------

ADD_TEST(bfTuImageToPointSetFilterTest ${BASICFILTERS_TESTS} 
         otbImageToPointSetFilterTest)  

# -------            otb::OpeningClosingMorphologicalFilter   ------------------------------

ADD_TEST(bfTuOpeningClosingFilterNew ${BASICFILTERS_TESTS}  
        otbOpeningClosingMorphologicalFilterNew )

ADD_TEST(bfTvOpeningClosingFilter ${BASICFILTERS_TESTS}
   --compare-image ${TOL}  ${BASELINE}/bfFiltreOpeningClosing_1.tif
                           ${TEMP}/ROI_IKO_PAN_LesHalles_OC_MorphoFilter_1.tif
        otbOpeningClosingMorphologicalFilter
        ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	${TEMP}/ROI_IKO_PAN_LesHalles_OC_MorphoFilter_1.tif
	1)

# -------            otb::ClosingOpeningMorphologicalFilter   ------------------------------

ADD_TEST(bfTuClosingOpeningFilterNew ${BASICFILTERS_TESTS}  
        otbClosingOpeningMorphologicalFilterNew )

ADD_TEST(bfTvClosingOpeningFilter ${BASICFILTERS_TESTS}
   --compare-image ${TOL}  ${BASELINE}/bfFiltreClosingOpening_1.tif
                           ${TEMP}/ROI_IKO_PAN_LesHalles_CO_MorphoFilter_1.tif
        otbClosingOpeningMorphologicalFilter
        ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	${TEMP}/ROI_IKO_PAN_LesHalles_CO_MorphoFilter_1.tif
	1)

# A enrichir
SET(BasicFilters_SRCS
otbLeeFilter.cxx
otbFrostFilterTest.cxx
otbFrostFilterNew.cxx
otbFrostFilter.cxx
otbImageToPointSetFilterTest.cxx
otbLogPolarResampleImageFilterNew.cxx
otbLogPolarResampleImageFilter.cxx
otbOpeningClosingMorphologicalFilterNew.cxx
otbOpeningClosingMorphologicalFilter.cxx
otbClosingOpeningMorphologicalFilterNew.cxx
otbClosingOpeningMorphologicalFilter.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbBasicFiltersTests otbBasicFiltersTests.cxx ${BasicFilters_SRCS})
TARGET_LINK_LIBRARIES(otbBasicFiltersTests  OTBBasicFilters OTBCommon OTBIO gdal ITKIO ITKCommon)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




