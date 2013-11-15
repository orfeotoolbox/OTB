
if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


set(MULTISCALE_TESTS1 ${CXX_TEST_PATH}/otbMultiScaleTests1)
set(MULTISCALE_TESTS2 ${CXX_TEST_PATH}/otbMultiScaleTests2)
set(MULTISCALE_TESTS3 ${CXX_TEST_PATH}/otbMultiScaleTests3)
set(MULTISCALE_TESTS4 ${CXX_TEST_PATH}/otbMultiScaleTests4)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbMULTISCALE_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::MorphologicalPyramidResampler  ----------

add_test(msTuMorphoPyrResamplerNew ${MULTISCALE_TESTS1}
         otbMorphologicalPyramidResamplerNew)

add_test(msTvMorphoPyrResampler ${MULTISCALE_TESTS1}
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

# -------            otb::MorphologicalPyramidAnalysisFilter   ----------

add_test(msTuMorphoPyrAnalysisFilterNew ${MULTISCALE_TESTS1}
         otbMorphologicalPyramidAnalysisFilterNew)

add_test(msTvMorphoPyrAnalysisFilter ${MULTISCALE_TESTS1}
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

# -------            otb::MorphologicalPyramidSynthesisFilter   ----------

add_test(msTuMorphoPyrSynthesisFilterNew ${MULTISCALE_TESTS1}
         otbMorphologicalPyramidSynthesisFilterNew)

add_test(msTvMorphoPyrSynthesisFilter ${MULTISCALE_TESTS1}
   --compare-image ${NOTOL}
                    ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
                    ${TEMP}/msPyrSynth_IKO_Halles_4_2.tif
         otbMorphologicalPyramidSynthesisFilter
         ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
         ${TEMP}/msPyrSynth_IKO_Halles_4_2.tif
         4
         2.0)

# -------            otb::MorphologicalPyramidMRToMSConverter   ----------

add_test(msTuMorphoPyrMRToMSConverterNew ${MULTISCALE_TESTS1}
         otbMorphologicalPyramidMRToMSConverterNew)

add_test(msTvMorphoPyrMRToMSConverter ${MULTISCALE_TESTS1}
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

# -------            otb::MorphologicalPyramidSegmenter   ----------

add_test(msTuMorphoPyrSegmenterNew ${MULTISCALE_TESTS1}
         otbMorphologicalPyramidSegmenterNew)

add_test(msTvMorphoPyrSegmentBright ${MULTISCALE_TESTS1}
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
set_tests_properties(msTvMorphoPyrSegmentBright PROPERTIES DEPENDS msTvMorphoPyrMRToMSConverter)

add_test(msTvMorphoPyrSegmentDark ${MULTISCALE_TESTS1}
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
set_tests_properties(msTvMorphoPyrSegmentDark PROPERTIES DEPENDS msTvMorphoPyrMRToMSConverter)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbMULTISCALE_TESTS2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::MorphologicalPyramidSegmentationFilter   ----------

add_test(msTuMorphoPyrSegmentationFilterNew ${MULTISCALE_TESTS2}
         otbMorphologicalPyramidSegmentationFilterNew)

add_test(msTvMorphoPyrSegmentationFilter ${MULTISCALE_TESTS2}
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

# -------            otb::GeodesicMorphologyDecompositionImageFilter   ----------

add_test(msTuGeodesicMorphologyDecompositionImageFilterNew ${MULTISCALE_TESTS2}
         otbGeodesicMorphologyDecompositionImageFilterNew)

add_test(msTvGeodesicMorphologyDecompositionImageFilter ${MULTISCALE_TESTS2}
   --compare-n-images ${NOTOL} 3
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

add_test(msTuGeodesicMorphologyLevelingFilterNew ${MULTISCALE_TESTS2}
         otbGeodesicMorphologyLevelingFilterNew)

add_test(msTvGeodesicMorphologyLevelingFilter ${MULTISCALE_TESTS2}
   --compare-image ${NOTOL}
                   ${BASELINE}/msGeodesicMorphologyLevelingFilterOutput.tif
                    ${TEMP}/msGeodesicMorphologyLevelingFilterOutput.tif
         otbGeodesicMorphologyLevelingFilter
         ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
         ${INPUTDATA}/ROI_IKO_PAN_LesHalles_convex.tif
         ${INPUTDATA}/ROI_IKO_PAN_LesHalles_concave.tif
         ${TEMP}/msGeodesicMorphologyLevelingFilterOutput.tif
)

# -------            otb::GeodesicMorphologyLevelingFilter   ----------

add_test(msTuConvexOrConcaveClassificationFilterNew ${MULTISCALE_TESTS2}
         otbConvexOrConcaveClassificationFilterNew)

add_test(msTvConvexOrConcaveClassificationFilter ${MULTISCALE_TESTS2}
   --compare-image ${NOTOL}
                   ${BASELINE}/msConvexOrConcaveClassificationFilterOutput.png
                    ${TEMP}/msConvexOrConcaveClassificationFilterOutput.png
         otbConvexOrConcaveClassificationFilter
         ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
         ${INPUTDATA}/ROI_IKO_PAN_LesHalles_leveling.tif
         ${TEMP}/msConvexOrConcaveClassificationFilterOutput.png
         0.5
)

# -------            otb::MorphologicalOpeningProfileFilter   ----------

add_test(msTuMorphologicalOpeningProfileFilterNew ${MULTISCALE_TESTS2}
         otbMorphologicalOpeningProfileFilterNew)

add_test(msTvMorphologicalOpeningProfileFilter ${MULTISCALE_TESTS2}
   --compare-n-images ${NOTOL} 4
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

add_test(msTuMorphologicalClosingProfileFilterNew ${MULTISCALE_TESTS2}
         otbMorphologicalClosingProfileFilterNew)

add_test(msTvMorphologicalClosingProfileFilter ${MULTISCALE_TESTS2}
   --compare-n-images ${NOTOL} 4
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

add_test(msTuProfileToProfileDerivativeFilterNew ${MULTISCALE_TESTS3}
         otbProfileToProfileDerivativeFilterNew)

add_test(msTvProfileToProfileDerivativeFilter ${MULTISCALE_TESTS3}
   --compare-n-images ${NOTOL} 4
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

add_test(msTuProfileDerivativeToMultiScaleCharacteristicsFilterNew ${MULTISCALE_TESTS3}
         otbProfileDerivativeToMultiScaleCharacteristicsFilterNew)

add_test(msTvProfileDerivativeToMultiScaleCharacteristicsFilter ${MULTISCALE_TESTS3}
   --compare-n-images ${NOTOL} 2
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

add_test(msTuMultiScaleConvexOrConcaveClassificationFilterNew ${MULTISCALE_TESTS3}
         otbMultiScaleConvexOrConcaveClassificationFilterNew)

add_test(msTvMultiScaleConvexOrConcaveClassificationFilter ${MULTISCALE_TESTS3}
   --compare-image ${NOTOL}
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

add_test(msTuGeodesicMorphologyIterativeDecompositionImageFilterNew ${MULTISCALE_TESTS3}
         otbGeodesicMorphologyIterativeDecompositionImageFilterNew)

add_test(msTvGeodesicMorphologyIterativeDecompositionImageFilter ${MULTISCALE_TESTS3}
   --compare-n-images ${NOTOL} 6
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


# -------            otb::WaveletOperator    ----------

add_test(msTuWaveletOperatorNew ${MULTISCALE_TESTS4}
         otbWaveletOperatorNew )

add_test(msTuWaveletOperator ${MULTISCALE_TESTS4}
         otbWaveletOperator )

# -------            otb::WaveletFilterBankNew   ----------
add_test(msTuWaveletFilterBankNew ${MULTISCALE_TESTS4}
         otbWaveletFilterBankNew)

add_test(msTuWaveletInverseFilterBankNew ${MULTISCALE_TESTS4}
         otbWaveletInverseFilterBankNew)

add_test(msTvWaveletFilterBank ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_FilterBank.tif
         otbWaveletFilterBank
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
                  2
          ${TEMP}/ROI_IKO_PAN_LesHalles_FilterBank.tif
)

add_test(msTvWaveletMultiScaleFilterBank ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_FilterBankMultiScale.tif
         otbWaveletFilterBank
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
                  1
          ${TEMP}/ROI_IKO_PAN_LesHalles_FilterBankMultiScale.tif
)

# -------            otb::WaveletTransform   ----------
add_test(msTuWaveletTransformNew ${MULTISCALE_TESTS4}
         otbWaveletTransformNew)

add_test(msTuWaveletInverseTransformNew ${MULTISCALE_TESTS4}
         otbWaveletInverseTransformNew)

add_test(msTvWaveletTransformHaar ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
                          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltHaar.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltHaar.tif
          2 2
          0 #Haar
)



add_test(msTvWaveletMultiScaleTransformHaar ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
                          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar.tif
          2 1
          0 #Haar
)




add_test(msTvWaveletMultiScaleTransformHaar_1 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
                          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar_1.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar_1.tif
          2 1
          0 #Haar
          1
)



add_test(msTvWaveletMultiScaleTransformHaar_4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
                          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar_4.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar_4.tif
          2 1
          0 #Haar
          4
)



add_test(msTvWaveletMultiScaleTransformHaar_8 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
                          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar_8.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar_8.tif
          2 1
          0 #Haar
          8
)


add_test(msTvWaveletTransformDB4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB4.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB4.tif
          2 2
          1 #DB4
)

add_test(msTvWaveletMultiScaleTransformDB4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4.tif
          2 1
          1 #DB4
)

add_test(msTvWaveletMultiScaleTransformDB4_1 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4_1.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4_1.tif
          2 1
          1 #DB4
          1
)


add_test(msTvWaveletMultiScaleTransformDB4_4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4_4.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4_4.tif
          2 1
          1 #DB4
          4
)


add_test(msTvWaveletMultiScaleTransformDB4_8 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4_8.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4_8.tif
          2 1
          1 #DB4
          8
)




add_test(msTvWaveletTransformDB6 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB6.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB6.tif
          2 2
          2 #DB6
)

add_test(msTvWaveletMultiScaleTransformDB6 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB6.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB6.tif
          2 1
          2 #DB6
)


add_test(msTvWaveletMultiScaleTransformDB6_1 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB6_1.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB6_1.tif
          2 1
          2 #DB6
          1
)



add_test(msTvWaveletTransformDB8 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB8.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB8.tif
          2 2
          3 #DB8
)

add_test(msTvWaveletMultiScaleTransformDB8 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB8.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB8.tif
          2 1
          3 #DB8
)


add_test(msTvWaveletMultiScaleTransformDB8_1 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB8_1.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB8_1.tif
          2 1
          3 #DB8
          1
)


add_test(msTvWaveletTransformDB12 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB12.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB12.tif
          2 2
          4 #DB12
)

add_test(msTvWaveletMultiScaleTransformDB12 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB12.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB12.tif
          2 1
          4 #DB12
)

add_test(msTvWaveletTransformDB20 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB20.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB20.tif
          2 2
          5 #DB20
)

add_test(msTvWaveletMultiScaleTransformDB20 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB20.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB20.tif
          2 1
          5 #DB20
)

add_test(msTvWaveletTransformSPLINE_BIORTHOGONAL_2_4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltSPLINE_BIORTHOGONAL_2_4.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltSPLINE_BIORTHOGONAL_2_4.tif
          2 2
          6 #SPLINE_BIORTHOGONAL_2_4
)

add_test(msTvWaveletMultiScaleTransformSPLINE_BIORTHOGONAL_2_4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSPLINE_BIORTHOGONAL_2_4.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSPLINE_BIORTHOGONAL_2_4.tif
          2 1
          6 #SPLINE_BIORTHOGONAL_2_4
)
add_test(msTvWaveletTransformSPLINE_BIORTHOGONAL_4_4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltSPLINE_BIORTHOGONAL_4_4.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltSPLINE_BIORTHOGONAL_4_4.tif
          2 2
          7 #SPLINE_BIORTHOGONAL_4_4
)

add_test(msTvWaveletMultiScaleTransformSPLINE_BIORTHOGONAL_4_4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSPLINE_BIORTHOGONAL_4_4.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSPLINE_BIORTHOGONAL_4_4.tif
          2 1
          7 #SPLINE_BIORTHOGONAL_4_4
)
add_test(msTvWaveletTransformSYMLET8 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltSYMLET8.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltSYMLET8.tif
          2 2
          8 #SYMLET8
)

add_test(msTvWaveletMultiScaleTransformSYMLET8 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSYMLET8.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSYMLET8.tif
          2 1
          8 #SYMLET8
)

# -------            otb::WaveletPacketTransform   ----------
add_test(msTuWaveletPacketTransformNew ${MULTISCALE_TESTS4}
         otbWaveletPacketTransformNew)

add_test(msTuWaveletPacketInverseTransformNew ${MULTISCALE_TESTS4}
         otbWaveletPacketInverseTransformNew)

add_test(msTvWaveletPacketTransformHaar ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketHaar.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketHaar.tif
          2 2
          0 #Haar
)

add_test(msTvWaveletPacketMultiScaleTransformHaar ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleHaar.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleHaar.tif
          2 1
          0 #Haar
)

add_test(msTvWaveletPacketTransformDB4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB4.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB4.tif
          2 2
          1 #DB4
)

add_test(msTvWaveletPacketMultiScaleTransformDB4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB4.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB4.tif
          2 1
          1 #DB4
)

add_test(msTvWaveletPacketTransformDB6 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB6.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB6.tif
          2 2
          2 #DB6
)

add_test(msTvWaveletPacketMultiScaleTransformDB6 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB6.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB6.tif
          2 1
          2 #DB6
)

add_test(msTvWaveletPacketTransformDB8 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB8.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB8.tif
          2 2
          3 #DB8
)

add_test(msTvWaveletPacketMultiScaleTransformDB8 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB8.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB8.tif
          2 1
          3 #DB8
)

add_test(msTvWaveletPacketTransformDB12 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB12.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB12.tif
          2 2
          4 #DB12
)

add_test(msTvWaveletPacketMultiScaleTransformDB12 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB12.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB12.tif
          2 1
          4 #DB12
)

add_test(msTvWaveletPacketTransformDB20 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB20.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB20.tif
          2 2
          5 #DB20
)

add_test(msTvWaveletPacketMultiScaleTransformDB20 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB20.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB20.tif
          2 1
          5 #DB20
)

add_test(msTvWaveletPacketTransformSPLINE_BIORTHOGONAL_2_4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketSPLINE_BIORTHOGONAL_2_4.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketSPLINE_BIORTHOGONAL_2_4.tif
          2 2
          6 #SPLINE_BIORTHOGONAL_2_4
)

add_test(msTvWaveletPacketMultiScaleTransformSPLINE_BIORTHOGONAL_2_4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSPLINE_BIORTHOGONAL_2_4.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSPLINE_BIORTHOGONAL_2_4.tif
          2 1
          6 #SPLINE_BIORTHOGONAL_2_4
)
add_test(msTvWaveletPacketTransformSPLINE_BIORTHOGONAL_4_4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketSPLINE_BIORTHOGONAL_4_4.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketSPLINE_BIORTHOGONAL_4_4.tif
          2 2
          7 #SPLINE_BIORTHOGONAL_4_4
)

add_test(msTvWaveletPacketMultiScaleTransformSPLINE_BIORTHOGONAL_4_4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSPLINE_BIORTHOGONAL_4_4.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSPLINE_BIORTHOGONAL_4_4.tif
          2 1
          7 #SPLINE_BIORTHOGONAL_4_4
)
add_test(msTvWaveletPacketTransformSYMLET8 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketSYMLET8.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketSYMLET8.tif
          2 2
          8 #SYMLET8
)

add_test(msTvWaveletPacketMultiScaleTransformSYMLET8 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSYMLET8.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSYMLET8.tif
          2 1
          8 #SYMLET8
)

# ----- otbSubsampleImageFilter --------------------------------------
add_test(msTuSubsampleImageFilterNew ${MULTISCALE_TESTS4}
        otbSubsampleImageFilterNew)

add_test(msTvSubsampleImageFilter ${MULTISCALE_TESTS4}
        --compare-image ${NOTOL}
                ${TEMP}/msSubsampleImageFilterOversampled.tif
                ${TEMP}/msSubsampleImageFilterOnePixelOutOf2.tif
        otbSubsampleImageFilter
                ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
                ${TEMP}/msSubsampleImageFilterOversampled.tif
                ${TEMP}/msSubsampleImageFilterOnePixelOutOf2.tif
)

# -------       Fichiers sources CXX -----------------------------------
set(BasicMultiScale_SRCS1
otbMultiScaleTests1.cxx
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
set(BasicMultiScale_SRCS2
otbMultiScaleTests2.cxx
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
set(BasicMultiScale_SRCS3
otbMultiScaleTests3.cxx
otbProfileToProfileDerivativeFilterNew.cxx
otbProfileToProfileDerivativeFilter.cxx
otbProfileDerivativeToMultiScaleCharacteristicsFilterNew.cxx
otbProfileDerivativeToMultiScaleCharacteristicsFilter.cxx
otbMultiScaleConvexOrConcaveClassificationFilterNew.cxx
otbMultiScaleConvexOrConcaveClassificationFilter.cxx
otbGeodesicMorphologyIterativeDecompositionImageFilterNew.cxx
otbGeodesicMorphologyIterativeDecompositionImageFilter.cxx
)
set(BasicMultiScale_SRCS4
otbMultiScaleTests4.cxx
otbWaveletOperatorNew.cxx
otbWaveletOperator.cxx
otbWaveletFilterBankNew.cxx
otbWaveletInverseFilterBankNew.cxx
otbWaveletFilterBank.cxx
otbWaveletTransformNew.cxx
otbWaveletInverseTransformNew.cxx
otbWaveletTransform.cxx
otbWaveletPacketTransformNew.cxx
otbWaveletPacketInverseTransformNew.cxx
otbWaveletPacketTransform.cxx
otbSubsampleImageFilterNew.cxx
otbSubsampleImageFilter.cxx
)

OTB_ADD_EXECUTABLE(otbMultiScaleTests1 "${BasicMultiScale_SRCS1}" "OTBMultiScale;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbMultiScaleTests2 "${BasicMultiScale_SRCS2}" "OTBMultiScale;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbMultiScaleTests3 "${BasicMultiScale_SRCS3}" "OTBMultiScale;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbMultiScaleTests4 "${BasicMultiScale_SRCS4}" "OTBMultiScale;OTBIO;OTBTesting")

endif()
