 
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


# -------            otb::MorphologicalPyramidResampler  ----------

ADD_TEST(msTuMorphoPyrResamplerNew ${MULTISCALE_TESTS} 
         otbMorphologicalPyramidResamplerNew)

ADD_TEST(msTvMorphoPyrResampler ${MULTISCALE_TESTS} 
  --compare-n-images ${TOL} 2
		   ${BASELINE}/msPyrResampler_IKO_LesHalles_256_2.tif
		   ${TEMP}/msPyrResampler_IKO_LesHalles_256_2.tif
		   ${BASELINE}/msPyrResampler_IKO_LesHalles_full.tif
		   ${TEMP}/msPyrResampler_IKO_LesHalles_full.tif
         otbMorphologicalPyramidResampler
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	 ${TEMP}/msPyrResampler_IKO_LesHalles_256_2.tif
	 ${TEMP}/msPyrResampler_IKO_LesHalles_full.tif
	 256
	 256
	 )

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

# -------            otb::MorphologicalPyramidSynthesisFilter   ----------

ADD_TEST(msTuMorphoPyrSynthesisFilterNew ${MULTISCALE_TESTS} 
         otbMorphologicalPyramidSynthesisFilterNew)

ADD_TEST(msTvMorphoPyrSynthesisFilter ${MULTISCALE_TESTS} 
   --compare-image ${TOL} 
 		   ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
 		   ${TEMP}/msPyrSynth_IKO_Halles_4_2.tif
         otbMorphologicalPyramidSynthesisFilter
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	 ${TEMP}/msPyrSynth_IKO_Halles_4_2.tif
	 4
	 2.0)

# -------            otb::MorphologicalPyramidMRToMSConverter   ----------

ADD_TEST(msTuMorphoPyrMRToMSConverterNew ${MULTISCALE_TESTS} 
         otbMorphologicalPyramidMRToMSConverterNew)

ADD_TEST(msTvMorphoPyrMRToMSConverter ${MULTISCALE_TESTS} 
   --compare-n-images ${TOL} 3
 		   ${BASELINE}/msPyrMRToMS_IKO_Halles_4_2_sf_full.tif
 		   ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_sf_full.tif
		   ${BASELINE}/msPyrMRToMS_IKO_Halles_4_2_if_full.tif
 		   ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_if_full.tif
		   ${BASELINE}/msPyrMRToMS_IKO_Halles_4_2_an_full.tif
 		   ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_an_full.tif

         otbMorphologicalPyramidMRToMSConverter
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	 ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_sf_full.tif
	 ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_if_full.tif
	 ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_an_full.tif
	 4
	 2.0)

# -------            otb::MorphologicalPyramidSegmenter   ----------

ADD_TEST(msTuMorphoPyrSegmenterNew ${MULTISCALE_TESTS} 
         otbMorphologicalPyramidSegmenterNew)

ADD_TEST(msTvMorphoPyrSegmentBright ${MULTISCALE_TESTS} 
   --compare-image ${TOL} 
 		   ${BASELINE}/msPyrSegmenter_IKO_Halles_4_2_sf.tif
 		   ${TEMP}/msPyrSegmenter_IKO_Halles_4_2_sf.tif
         otbMorphologicalPyramidSegmenter
	 ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_sf_full.tif
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	 ${TEMP}/msPyrSegmenter_IKO_Halles_4_2_sf.tif
	 0
	 0.9
	 0.9
	 10
)

ADD_TEST(msTvMorphoPyrSegmentDark ${MULTISCALE_TESTS} 
   --compare-image ${TOL} 
 		   ${BASELINE}/msPyrSegmenter_IKO_Halles_4_2_if.tif
 		   ${TEMP}/msPyrSegmenter_IKO_Halles_4_2_if.tif
         otbMorphologicalPyramidSegmenter
	 ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_if_full.tif
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	 ${TEMP}/msPyrSegmenter_IKO_Halles_4_2_if.tif
	 1
	 0.9
	 0.9
	 10
)

# -------       Fichiers sources CXX -----------------------------------
SET(BasicMultiScale_SRCS
otbMorphologicalPyramidResamplerNew.cxx
otbMorphologicalPyramidResampler.cxx
otbMorphologicalPyramidAnalyseFilterNew.cxx
otbMorphologicalPyramidAnalyseFilter.cxx
otbMorphologicalPyramidSynthesisFilterNew.cxx
otbMorphologicalPyramidSynthesisFilter.cxx
otbMorphologicalPyramidMRToMSConverterNew.cxx
otbMorphologicalPyramidMRToMSConverter.cxx
otbMorphologicalPyramidSegmenterNew.cxx
otbMorphologicalPyramidSegmenter.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbMultiScaleTests otbMultiScaleTests.cxx ${BasicMultiScale_SRCS})
TARGET_LINK_LIBRARIES(otbMultiScaleTests OTBIO OTBMultiScale gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )
