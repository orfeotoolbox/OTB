 
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
SET(MULTISCALE_TESTS3 ${CXX_TEST_PATH}/otbMultiScaleTests3)

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

# -------            otb::GeodesicMorphologyDecompositionImageFilter   ----------
 
ADD_TEST(msTuGeodesicMorphologyDecompositionImageFilterNew ${MULTISCALE_TESTS2} 
         otbGeodesicMorphologyDecompositionImageFilterNew)

ADD_TEST(msTvGeodesicMorphologyDecompositionImageFilter ${MULTISCALE_TESTS2} 
   --compare-n-images ${TOL} 3
		   ${BASELINE}/msGeodesicMorphologyDecompositionImageFilterConvexOutput.tif
 		   ${TEMP}/msGeodesicMorphologyDecompositionImageFilterConvexOutput.tif
		   ${BASELINE}/msGeodesicMorphologyDecompositionImageFilterConcaveOutput.tif
 		   ${TEMP}/msGeodesicMorphologyDecompositionImageFilterConcaveOutput.tif
		   ${BASELINE}/msGeodesicMorphologyDecompositionImageFilterLevelingOutput.tif
 		   ${TEMP}/msGeodesicMorphologyDecompositionImageFilterLevelingOutput.tif
         otbGeodesicMorphologyDecompositionImageFilter
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	 ${TEMP}/msGeodesicMorphologyDecompositionImageFilterConvexOutput.tif
	 ${TEMP}/msGeodesicMorphologyDecompositionImageFilterConcaveOutput.tif
	 ${TEMP}/msGeodesicMorphologyDecompositionImageFilterLevelingOutput.tif
	 4
)

# -------            otb::GeodesicMorphologyLevelingFilter   ----------
 
ADD_TEST(msTuGeodesicMorphologyLevelingFilterNew ${MULTISCALE_TESTS2} 
         otbGeodesicMorphologyLevelingFilterNew)

ADD_TEST(msTvGeodesicMorphologyLevelingFilter ${MULTISCALE_TESTS2} 
   --compare-image ${TOL}
		   ${BASELINE}/msGeodesicMorphologyLevelingFilterOutput.tif
 		   ${TEMP}/msGeodesicMorphologyLevelingFilterOutput.tif
         otbGeodesicMorphologyLevelingFilter
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles_convex.tif
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles_concave.tif
	 ${TEMP}/msGeodesicMorphologyLevelingFilterOutput.tif
)

# -------            otb::GeodesicMorphologyLevelingFilter   ----------
 
ADD_TEST(msTuConvexOrConcaveClassificationFilterNew ${MULTISCALE_TESTS2} 
         otbConvexOrConcaveClassificationFilterNew)

ADD_TEST(msTvConvexOrConcaveClassificationFilter ${MULTISCALE_TESTS2} 
   --compare-image ${TOL}
		   ${BASELINE}/msConvexOrConcaveClassificationFilterOutput.png
 		   ${TEMP}/msConvexOrConcaveClassificationFilterOutput.png
         otbConvexOrConcaveClassificationFilter
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles_leveling.tif
	 ${TEMP}/msConvexOrConcaveClassificationFilterOutput.png
	 0.5
)

# -------            otb::MorphologicalOpeningProfileFilter   ----------
 
ADD_TEST(msTuMorphologicalOpeningProfileFilterNew ${MULTISCALE_TESTS2} 
         otbMorphologicalOpeningProfileFilterNew)

ADD_TEST(msTvMorphologicalOpeningProfileFilter ${MULTISCALE_TESTS2} 
   --compare-n-images ${TOL} 4
		   ${BASELINE}/msMorphologicalOpeningProfileFilterOutput1.tif
 		   ${TEMP}/msMorphologicalOpeningProfileFilterOutput1.tif
		   ${BASELINE}/msMorphologicalOpeningProfileFilterOutput2.tif	
 		   ${TEMP}/msMorphologicalOpeningProfileFilterOutput2.tif
		   ${BASELINE}/msMorphologicalOpeningProfileFilterOutput3.tif
 		   ${TEMP}/msMorphologicalOpeningProfileFilterOutput3.tif
		   ${BASELINE}/msMorphologicalOpeningProfileFilterOutput4.tif
 		   ${TEMP}/msMorphologicalOpeningProfileFilterOutput4.tif		  
         otbMorphologicalOpeningProfileFilter
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	 ${TEMP}/msMorphologicalOpeningProfileFilterOutput
	 tif
	 4
	 1
	 1	 
)

# -------            otb::MorphologicalClosingProfileFilter   ----------
 
ADD_TEST(msTuMorphologicalClosingProfileFilterNew ${MULTISCALE_TESTS2} 
         otbMorphologicalClosingProfileFilterNew)

ADD_TEST(msTvMorphologicalClosingProfileFilter ${MULTISCALE_TESTS2} 
   --compare-n-images ${TOL} 4
		   ${BASELINE}/msMorphologicalClosingProfileFilterOutput1.tif
 		   ${TEMP}/msMorphologicalClosingProfileFilterOutput1.tif
		   ${BASELINE}/msMorphologicalClosingProfileFilterOutput2.tif	
 		   ${TEMP}/msMorphologicalClosingProfileFilterOutput2.tif
		   ${BASELINE}/msMorphologicalClosingProfileFilterOutput3.tif
 		   ${TEMP}/msMorphologicalClosingProfileFilterOutput3.tif
		   ${BASELINE}/msMorphologicalClosingProfileFilterOutput4.tif
 		   ${TEMP}/msMorphologicalClosingProfileFilterOutput4.tif		  
         otbMorphologicalClosingProfileFilter
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	 ${TEMP}/msMorphologicalClosingProfileFilterOutput
	 tif
	 4
	 1
	 1	 
)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbMULTISCALE_TESTS3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::ProfileToProfileDerivativeFilter   ----------
 
ADD_TEST(msTuProfileToProfileDerivativeFilterNew ${MULTISCALE_TESTS3} 
         otbProfileToProfileDerivativeFilterNew)

ADD_TEST(msTvProfileToProfileDerivativeFilter ${MULTISCALE_TESTS3} 
   --compare-n-images ${TOL} 4
		   ${BASELINE}/msProfileToProfileDerivativeFilterOutput1.tif
 		   ${TEMP}/msProfileToProfileDerivativeFilterOutput1.tif
		   ${BASELINE}/msProfileToProfileDerivativeFilterOutput2.tif	
 		   ${TEMP}/msProfileToProfileDerivativeFilterOutput2.tif
		   ${BASELINE}/msProfileToProfileDerivativeFilterOutput3.tif
 		   ${TEMP}/msProfileToProfileDerivativeFilterOutput3.tif
		   ${BASELINE}/msProfileToProfileDerivativeFilterOutput4.tif
 		   ${TEMP}/msProfileToProfileDerivativeFilterOutput4.tif		  
         otbProfileToProfileDerivativeFilter
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	 ${TEMP}/msProfileToProfileDerivativeFilterOutput
	 tif
	 5
	 1
	 1	 
)

# -------            otb::ProfileDerivativeToMultiScaleCharacteristicsFilter   ----------
 
ADD_TEST(msTuProfileDerivativeToMultiScaleCharacteristicsFilterNew ${MULTISCALE_TESTS3} 
         otbProfileDerivativeToMultiScaleCharacteristicsFilterNew)

ADD_TEST(msTvProfileDerivativeToMultiScaleCharacteristicsFilter ${MULTISCALE_TESTS3} 
   --compare-n-images ${TOL} 2
		   ${BASELINE}/msProfileDerivativeToMultiScaleCharacteristicsFilterOutput.tif
 		   ${TEMP}/msProfileDerivativeToMultiScaleCharacteristicsFilterOutput.tif
		   ${BASELINE}/msProfileDerivativeToMultiScaleCharacteristicsFilterOutputCharacteristics.tif	
 		   ${TEMP}/msProfileDerivativeToMultiScaleCharacteristicsFilterOutputCharacteristics.tif		  
         otbProfileDerivativeToMultiScaleCharacteristicsFilter
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	 ${TEMP}/msProfileDerivativeToMultiScaleCharacteristicsFilterOutput.tif
	  ${TEMP}/msProfileDerivativeToMultiScaleCharacteristicsFilterOutputCharacteristics.tif
	 5
	 1
	 1	 
)
# -------            otb::MultiScaleConvexOrConcaveClassificationFilter   ----------
 
ADD_TEST(msTuMultiScaleConvexOrConcaveClassificationFilterNew ${MULTISCALE_TESTS3} 
         otbMultiScaleConvexOrConcaveClassificationFilterNew)

ADD_TEST(msTvMultiScaleConvexOrConcaveClassificationFilter ${MULTISCALE_TESTS3} 
   --compare-image ${TOL}
		   ${BASELINE}/msMultiScaleConvexOrConcaveClassificationFilterOutput.tif
 		   ${TEMP}/msMultiScaleConvexOrConcaveClassificationFilterOutput.tif	  
         otbMultiScaleConvexOrConcaveClassificationFilter
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	 ${TEMP}/msMultiScaleConvexOrConcaveClassificationFilterOutput.tif
	 5
	 1
	 1
	 1.	 
)

# -------            otb::GeodesicMorphologyIterativeDecompositionImageFilter   ----------
 
ADD_TEST(msTuGeodesicMorphologyIterativeDecompositionImageFilterNew ${MULTISCALE_TESTS3} 
         otbGeodesicMorphologyIterativeDecompositionImageFilterNew)

ADD_TEST(msTvGeodesicMorphologyIterativeDecompositionImageFilter ${MULTISCALE_TESTS3} 
   --compare-n-images ${TOL} 6
		   ${BASELINE}/msGeodesicMorphologyIterativeDecompositionImageFilter_concMap_1.tif
 		   ${TEMP}/msGeodesicMorphologyIterativeDecompositionImageFilter_concMap_1.tif
		   ${BASELINE}/msGeodesicMorphologyIterativeDecompositionImageFilter_concMap_2.tif
 		   ${TEMP}/msGeodesicMorphologyIterativeDecompositionImageFilter_concMap_2.tif
		   ${BASELINE}/msGeodesicMorphologyIterativeDecompositionImageFilter_convMap_1.tif
 		   ${TEMP}/msGeodesicMorphologyIterativeDecompositionImageFilter_convMap_1.tif
		   ${BASELINE}/msGeodesicMorphologyIterativeDecompositionImageFilter_convMap_2.tif
 		   ${TEMP}/msGeodesicMorphologyIterativeDecompositionImageFilter_convMap_2.tif
		   ${BASELINE}/msGeodesicMorphologyIterativeDecompositionImageFilter_leveling_1.tif
 		   ${TEMP}/msGeodesicMorphologyIterativeDecompositionImageFilter_leveling_1.tif
		   ${BASELINE}/msGeodesicMorphologyIterativeDecompositionImageFilter_leveling_2.tif
 		   ${TEMP}/msGeodesicMorphologyIterativeDecompositionImageFilter_leveling_2.tif
         otbGeodesicMorphologyIterativeDecompositionImageFilter
	 ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
	 ${TEMP}/msGeodesicMorphologyIterativeDecompositionImageFilter
	 tif
	 2
	 3
         2	 
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
otbGeodesicMorphologyDecompositionImageFilterNew.cxx
otbGeodesicMorphologyDecompositionImageFilter.cxx
otbGeodesicMorphologyLevelingFilterNew.cxx
otbGeodesicMorphologyLevelingFilter.cxx
otbConvexOrConcaveClassificationFilterNew.cxx
otbConvexOrConcaveClassificationFilter.cxx
otbMorphologicalOpeningProfileFilterNew.cxx
otbMorphologicalOpeningProfileFilter.cxx
otbMorphologicalClosingProfileFilterNew.cxx
otbMorphologicalClosingProfileFilter.cxx
)
SET(BasicMultiScale_SRCS3
otbProfileToProfileDerivativeFilterNew.cxx
otbProfileToProfileDerivativeFilter.cxx
otbProfileDerivativeToMultiScaleCharacteristicsFilterNew.cxx
otbProfileDerivativeToMultiScaleCharacteristicsFilter.cxx
otbMultiScaleConvexOrConcaveClassificationFilterNew.cxx
otbMultiScaleConvexOrConcaveClassificationFilter.cxx
otbGeodesicMorphologyIterativeDecompositionImageFilterNew.cxx
otbGeodesicMorphologyIterativeDecompositionImageFilter.cxx

)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbMultiScaleTests1 otbMultiScaleTests1.cxx ${BasicMultiScale_SRCS1})
TARGET_LINK_LIBRARIES(otbMultiScaleTests1 OTBIO OTBMultiScale otbopenjpeg ggdal ITKIO ITKAlgorithms ITKStatistics ITKCommon)
ADD_EXECUTABLE(otbMultiScaleTests2 otbMultiScaleTests2.cxx ${BasicMultiScale_SRCS2})
TARGET_LINK_LIBRARIES(otbMultiScaleTests2 OTBIO OTBMultiScale otbopenjpeg gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon)
ADD_EXECUTABLE(otbMultiScaleTests3 otbMultiScaleTests3.cxx ${BasicMultiScale_SRCS3})
TARGET_LINK_LIBRARIES(otbMultiScaleTests3 OTBIO OTBMultiScale otbopenjpeg gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )
