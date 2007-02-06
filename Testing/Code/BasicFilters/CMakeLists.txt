
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

# -------            otb::OpeningClosingMorphologicalFilter   ------------------------------

ADD_TEST(bfTuOpeningClosingFilterNew ${BASICFILTERS_TESTS}  
        otbOpeningClosingMorphologicalFilterNew )

ADD_TEST(bfTvOpeningClosingFilter ${BASICFILTERS_TESTS}
   --compare-image ${TOL}  ${BASELINE}/bfFiltreOpeningClosing_1.tif
                           ${TEMP}/bfFiltreOpeningClosing_1.tif
        otbOpeningClosingMorphologicalFilter
        ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	${TEMP}/bfFiltreOpeningClosing_1.tif
	1)

# -------            otb::ClosingOpeningMorphologicalFilter   ------------------------------

ADD_TEST(bfTuClosingOpeningFilterNew ${BASICFILTERS_TESTS}  
        otbClosingOpeningMorphologicalFilterNew )

ADD_TEST(bfTvClosingOpeningFilter ${BASICFILTERS_TESTS}
   --compare-image ${TOL}  ${BASELINE}/bfFiltreClosingOpening_1.tif
                           ${TEMP}/bfFiltreClosingOpening_1.tif
        otbClosingOpeningMorphologicalFilter
        ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	${TEMP}/bfFiltreClosingOpening_1.tif
	1)


# -------            otb::BinaryImageMinimalBoundingRegionCalculator   ----------------------------

ADD_TEST(bfTuBoundingRegionCalculatorNew ${BASICFILTERS_TESTS}  
         otbBinaryImageMinimalBoundingRegionCalculatorNew)

ADD_TEST(bfTvBoundingRegionCalculator ${BASICFILTERS_TESTS}
   --compare-ascii ${TOL}
 		   ${BASELINE_FILES}/bfBoundingRegionCalculatorOutput.txt
 		   ${TEMP}/bfBoundingRegionCalculatorOutput.txt
        otbBinaryImageMinimalBoundingRegionCalculator
	4
	${TEMP}/bfBoundingRegionCalculatorOutput.txt
	${INPUTDATA}/rcc8_mire1.png
	${INPUTDATA}/rcc8_mire2.png
	${INPUTDATA}/rcc8_mire3.png
	${INPUTDATA}/rcc8_mire4.png
	)

# -------            otb::LogPolarTransform   ----------------------------

ADD_TEST(bfTuLogPolarTransformNew ${BASICFILTERS_TESTS}  
         otbLogPolarTransformNew)

ADD_TEST(bfTvLogPolarTransform ${BASICFILTERS_TESTS}
   --compare-ascii ${TOL}
 		   ${BASELINE_FILES}/bfLogPolarTransformResults.txt
 		   ${TEMP}/bfLogPolarTransformResults.txt
        otbLogPolarTransform
	1 1 
	${TEMP}/bfLogPolarTransformResults.txt
	5 0 0 0 10 45 0 3.14 3.14 18 1
	)

ADD_TEST(bfTvLogPolarTransformResample ${BASICFILTERS_TESTS}  
  --compare-image ${TOL}  ${BASELINE}/bfLogPolarTransformResampleOutput.hdr
                          ${TEMP}/bfLogPolarTransformResampleOutput.hdr
        otbLogPolarTransformResample
        ${INPUTDATA}/circle.hdr
	${TEMP}/bfLogPolarTransformResampleOutput.hdr
)


# -------            otb::InverseLogPolarTransform   ----------------------------

ADD_TEST(bfTuInverseLogPolarTransformNew ${BASICFILTERS_TESTS}  
         otbInverseLogPolarTransformNew)

ADD_TEST(bfTvInverseLogPolarTransform ${BASICFILTERS_TESTS}
     --compare-ascii ${TOL}
		     ${BASELINE_FILES}/bfInverseLogPolarTransformResults.txt
		     ${TEMP}/bfInverseLogPolarTransformResults.txt
        otbInverseLogPolarTransform
	1 1 
	${TEMP}/bfInverseLogPolarTransformResults.txt	
	6 0 0 10 20 45 60 3.14 3.14 1 0 0 1
	)

ADD_TEST(bfTvInverseLogPolarTransformResample ${BASICFILTERS_TESTS}  
--compare-image ${TOL}  ${BASELINE}/bfInverseLogPolarTransformResampleOutput.hdr
                          ${TEMP}/bfInverseLogPolarTransformResampleOutput.hdr
        otbInverseLogPolarTransformResample
	${BASELINE}/bfLogPolarTransformResampleOutput.hdr
	${TEMP}/bfInverseLogPolarTransformResampleOutput.hdr
)


# A enrichir
SET(BasicFilters_SRCS
otbLeeFilter.cxx
otbFrostFilterTest.cxx
otbFrostFilterNew.cxx
otbFrostFilter.cxx
otbImageToPointSetFilterTest.cxx
otbOpeningClosingMorphologicalFilterNew.cxx
otbOpeningClosingMorphologicalFilter.cxx
otbClosingOpeningMorphologicalFilterNew.cxx
otbClosingOpeningMorphologicalFilter.cxx
otbBinaryImageMinimalBoundingRegionCalculatorNew.cxx
otbBinaryImageMinimalBoundingRegionCalculator.cxx
otbLogPolarTransformNew.cxx
otbLogPolarTransform.cxx
otbLogPolarTransformResample.cxx
otbInverseLogPolarTransformNew.cxx
otbInverseLogPolarTransform.cxx
otbInverseLogPolarTransformResample.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbBasicFiltersTests otbBasicFiltersTests.cxx ${BasicFilters_SRCS})
TARGET_LINK_LIBRARIES(otbBasicFiltersTests  OTBBasicFilters OTBCommon OTBIO gdal ITKIO ITKCommon ITKBasicFilters)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




