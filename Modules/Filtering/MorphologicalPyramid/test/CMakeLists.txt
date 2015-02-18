otb_module_test()

set(OTBMorphologicalPyramidTests
otbMorphologicalPyramidTestDriver.cxx
otbMorphologicalPyramidSegmentationFilterNew.cxx
otbMorphologicalPyramidAnalysisFilter.cxx
otbMorphologicalPyramidSegmenter.cxx
otbMorphologicalPyramidSegmenterNew.cxx
otbMorphologicalPyramidSynthesisFilter.cxx
otbMorphologicalPyramidMRToMSConverter.cxx
otbMorphologicalPyramidSynthesisFilterNew.cxx
otbMorphologicalPyramidMRToMSConverterNew.cxx
otbMorphologicalPyramidResampler.cxx
otbMorphologicalPyramidAnalysisFilterNew.cxx
otbMorphologicalPyramidResamplerNew.cxx
otbMorphologicalPyramidSegmentationFilter.cxx
)

add_executable(otbMorphologicalPyramidTestDriver ${OTBMorphologicalPyramidTests})
target_link_libraries(otbMorphologicalPyramidTestDriver ${OTBMorphologicalPyramid-Test_LIBRARIES})
otb_module_target_label(otbMorphologicalPyramidTestDriver)

# Tests Declaration

otb_add_test(NAME msTuMorphoPyrSegmentationFilterNew COMMAND otbMorphologicalPyramidTestDriver
  otbMorphologicalPyramidSegmentationFilterNew)

otb_add_test(NAME msTvMorphoPyrAnalysisFilter COMMAND otbMorphologicalPyramidTestDriver
  --compare-n-images ${NOTOL} 5
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

otb_add_test(NAME msTvMorphoPyrSegmentBright COMMAND otbMorphologicalPyramidTestDriver
  --compare-image ${NOTOL}
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
set_property(TEST msTvMorphoPyrSegmentBright PROPERTY DEPENDS msTvMorphoPyrMRToMSConverter)

otb_add_test(NAME msTvMorphoPyrSegmentDark COMMAND otbMorphologicalPyramidTestDriver
  --compare-image ${NOTOL}
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
set_property(TEST msTvMorphoPyrSegmentDark PROPERTY DEPENDS msTvMorphoPyrMRToMSConverter)

otb_add_test(NAME msTuMorphoPyrSegmenterNew COMMAND otbMorphologicalPyramidTestDriver
  otbMorphologicalPyramidSegmenterNew)

otb_add_test(NAME msTvMorphoPyrSynthesisFilter COMMAND otbMorphologicalPyramidTestDriver
  --compare-image ${NOTOL}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/msPyrSynth_IKO_Halles_4_2.tif
  otbMorphologicalPyramidSynthesisFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/msPyrSynth_IKO_Halles_4_2.tif
  4
  2.0)

otb_add_test(NAME msTvMorphoPyrMRToMSConverter COMMAND otbMorphologicalPyramidTestDriver
  --compare-n-images ${EPSILON_8} 3
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

otb_add_test(NAME msTuMorphoPyrSynthesisFilterNew COMMAND otbMorphologicalPyramidTestDriver
  otbMorphologicalPyramidSynthesisFilterNew)

otb_add_test(NAME msTuMorphoPyrMRToMSConverterNew COMMAND otbMorphologicalPyramidTestDriver
  otbMorphologicalPyramidMRToMSConverterNew)

otb_add_test(NAME msTvMorphoPyrResampler COMMAND otbMorphologicalPyramidTestDriver
  --compare-n-images ${EPSILON_8} 2
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

otb_add_test(NAME msTuMorphoPyrAnalysisFilterNew COMMAND otbMorphologicalPyramidTestDriver
  otbMorphologicalPyramidAnalysisFilterNew)

otb_add_test(NAME msTuMorphoPyrResamplerNew COMMAND otbMorphologicalPyramidTestDriver
  otbMorphologicalPyramidResamplerNew)

otb_add_test(NAME msTvMorphoPyrSegmentationFilter COMMAND otbMorphologicalPyramidTestDriver
  --compare-n-images ${NOTOL} 4
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

