 
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance sur diff pixel image
SET(TOL 0.0)
SET(EPSILON 0.00000001)


SET(MULTISCALE_TESTS1 ${CXX_TEST_PATH}/otbMultiScaleTests1)
SET(MULTISCALE_TESTS2 ${CXX_TEST_PATH}/otbMultiScaleTests2)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbMULTISCALE_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::MorphologicalPyramidResampler  ----------

ADD_TEST(msTuMorphoPyrResamplerNew ${MULTISCALE_TESTS1} 
         otbMorphologicalPyramidResamplerNew)

ADD_TEST(msTvMorphoPyrResampler ${MULTISCALE_TESTS1} 
  --compare-n-images ${EPSILON} 2
		   ${BASELINE}/msPyrResampler_IKO_LesHalles_256_2.hdr
		   ${TEMP}/msPyrResampler_IKO_LesHalles_256_2.hdr
		   ${BASELINE}/msPyrResampler_IKO_LesHalles_full.hdr
		   ${TEMP}/msPyrResampler_IKO_LesHalles_full.hdr
         otbMorphologicalPyramidResampler
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	 ${TEMP}/msPyrResampler_IKO_LesHalles_256_2.hdr
	 ${TEMP}/msPyrResampler_IKO_LesHalles_full.hdr
	 256
	 256
	 )

# -------            otb::MorphologicalPyramidAnalysisFilter   ----------

ADD_TEST(msTuMorphoPyrAnalysisFilterNew ${MULTISCALE_TESTS1} 
         otbMorphologicalPyramidAnalysisFilterNew)

ADD_TEST(msTvMorphoPyrAnalysisFilter ${MULTISCALE_TESTS1} 
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
         otbMorphologicalPyramidAnalysisFilter
	 ${INPUTDATA}/mire_log.png
	 ${TEMP}/msPyrAn_mire_log
	 png
	 2
	 2.0)

# -------            otb::MorphologicalPyramidSynthesisFilter   ----------

ADD_TEST(msTuMorphoPyrSynthesisFilterNew ${MULTISCALE_TESTS1} 
         otbMorphologicalPyramidSynthesisFilterNew)

ADD_TEST(msTvMorphoPyrSynthesisFilter ${MULTISCALE_TESTS1} 
   --compare-image ${TOL} 
 		   ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
 		   ${TEMP}/msPyrSynth_IKO_Halles_4_2.tif
         otbMorphologicalPyramidSynthesisFilter
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	 ${TEMP}/msPyrSynth_IKO_Halles_4_2.tif
	 4
	 2.0)

# -------            otb::MorphologicalPyramidMRToMSConverter   ----------

ADD_TEST(msTuMorphoPyrMRToMSConverterNew ${MULTISCALE_TESTS1} 
         otbMorphologicalPyramidMRToMSConverterNew)

ADD_TEST(msTvMorphoPyrMRToMSConverter ${MULTISCALE_TESTS1} 
   --compare-n-images ${EPSILON} 3
 		   ${BASELINE}/msPyrMRToMS_IKO_Halles_4_2_sf_full.hdr
 		   ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_sf_full.hdr
		   ${BASELINE}/msPyrMRToMS_IKO_Halles_4_2_if_full.hdr
 		   ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_if_full.hdr
		   ${BASELINE}/msPyrMRToMS_IKO_Halles_4_2_an_full.hdr
 		   ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_an_full.hdr

         otbMorphologicalPyramidMRToMSConverter
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	 ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_sf_full.hdr
	 ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_if_full.hdr
	 ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_an_full.hdr
	 4
	 2.0)

# -------            otb::MorphologicalPyramidSegmenter   ----------

ADD_TEST(msTuMorphoPyrSegmenterNew ${MULTISCALE_TESTS1} 
         otbMorphologicalPyramidSegmenterNew)

ADD_TEST(msTvMorphoPyrSegmentBright ${MULTISCALE_TESTS1} 
   --compare-image ${TOL} 
 		   ${BASELINE}/msPyrSegmenter_IKO_Halles_4_2_sf.tif
 		   ${TEMP}/msPyrSegmenter_IKO_Halles_4_2_sf.tif
         otbMorphologicalPyramidSegmenter
	 ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_sf_full.hdr
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	 ${TEMP}/msPyrSegmenter_IKO_Halles_4_2_sf.tif
	 0
	 0.9
	 0.9
	 10
)

ADD_TEST(msTvMorphoPyrSegmentDark ${MULTISCALE_TESTS1} 
   --compare-image ${TOL} 
 		   ${BASELINE}/msPyrSegmenter_IKO_Halles_4_2_if.tif
 		   ${TEMP}/msPyrSegmenter_IKO_Halles_4_2_if.tif
         otbMorphologicalPyramidSegmenter
	 ${TEMP}/msPyrMRToMS_IKO_Halles_4_2_if_full.hdr
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	 ${TEMP}/msPyrSegmenter_IKO_Halles_4_2_if.tif
	 1
	 0.9
	 0.9
	 10
)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbMULTISCALE_TESTS2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::MorphologicalPyramidSegmentationFilter   ----------

ADD_TEST(msTuMorphoPyrSegmentationFilterNew ${MULTISCALE_TESTS2} 
         otbMorphologicalPyramidSegmentationFilterNew)

ADD_TEST(msTvMorphoPyrSegmentationFilter ${MULTISCALE_TESTS2} 
   --compare-n-images ${TOL} 4
		   ${BASELINE}/msPyrSegmentation_IKO_Halles_2.tif	
 		   ${TEMP}/msPyrSegmentation_IKO_Halles_2.tif
		   ${BASELINE}/msPyrSegmentation_IKO_Halles_4.tif
 		   ${TEMP}/msPyrSegmentation_IKO_Halles_4.tif
		   ${BASELINE}/msPyrSegmentation_IKO_Halles_5.tif
 		   ${TEMP}/msPyrSegmentation_IKO_Halles_5.tif
 		   ${BASELINE}/msPyrSegmentation_IKO_Halles_7.tif
 		   ${TEMP}/msPyrSegmentation_IKO_Halles_7.tif
         otbMorphologicalPyramidSegmentationFilter
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	 ${TEMP}/msPyrSegmentation_IKO_Halles_
	 tif
	 4
	 2.0
	 0.9
	 0.9
	 10
)



# -------       Fichiers sources CXX -----------------------------------
SET(BasicMultiScale_SRCS1
otbMorphologicalPyramidResamplerNew.cxx
otbMorphologicalPyramidResampler.cxx
otbMorphologicalPyramidAnalysisFilterNew.cxx
otbMorphologicalPyramidAnalysisFilter.cxx
otbMorphologicalPyramidSynthesisFilterNew.cxx
otbMorphologicalPyramidSynthesisFilter.cxx
otbMorphologicalPyramidMRToMSConverterNew.cxx
otbMorphologicalPyramidMRToMSConverter.cxx
otbMorphologicalPyramidSegmenterNew.cxx
otbMorphologicalPyramidSegmenter.cxx
)
SET(BasicMultiScale_SRCS2
otbMorphologicalPyramidSegmentationFilterNew.cxx
otbMorphologicalPyramidSegmentationFilter.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbMultiScaleTests1 otbMultiScaleTests1.cxx ${BasicMultiScale_SRCS1})
TARGET_LINK_LIBRARIES(otbMultiScaleTests1 OTBIO OTBMultiScale gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon)
ADD_EXECUTABLE(otbMultiScaleTests2 otbMultiScaleTests2.cxx ${BasicMultiScale_SRCS2})
TARGET_LINK_LIBRARIES(otbMultiScaleTests2 OTBIO OTBMultiScale gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )
