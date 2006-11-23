 
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance sur diff pixel image
SET(TOL 0.0)


SET(MULTISCALE_TESTS ${CXX_TEST_PATH}/otbMultiScaleTests)

# -------            otb::MorphologicalPyramidAnalyseFilter   ----------

ADD_TEST(msTuMorphoPyrAnalyseFilterNew ${MULTISCALE_TESTS} 
         otbMorphologicalPyramidAnalyseFilterNew)

ADD_TEST(msTvMorphoPyrAnalyseFilter ${MULTISCALE_TESTS} 
  --compare-n-images ${TOL} 5
		   ${BASELINE}/msPyrAn_mire_log_an_2.png
		   ${TEMP}/msPyrAn_mire_log_an_2.png
		   ${BASELINE}/msPyrAn_mire_log_sf_2.png
		   ${TEMP}/msPyrAn_mire_log_sf_2.png
		   ${BASELINE}/msPyrAn_mire_log_if_2.png
		   ${TEMP}/msPyrAn_mire_log_if_2.png
		   ${BASELINE}/msPyrAn_mire_log_sd_2.png
		   ${TEMP}/msPyrAn_mire_log_sd_2.png
		   ${BASELINE}/msPyrAn_mire_log_id_2.png
		   ${TEMP}/msPyrAn_mire_log_id_2.png
         otbMorphologicalPyramidAnalyseFilter
	 ${INPUTDATA}/mire_log.png
	 ${TEMP}/msPyrAn_mire_log
	 png
	 2
	 2.0)

# -------       Fichiers sources CXX -----------------------------------
SET(BasicMultiScale_SRCS
otbMorphologicalPyramidAnalyseFilterNew.cxx
otbMorphologicalPyramidAnalyseFilter.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbMultiScaleTests otbMultiScaleTests.cxx ${BasicMultiScale_SRCS})
TARGET_LINK_LIBRARIES(otbMultiScaleTests OTBIO OTBMultiScale gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )
