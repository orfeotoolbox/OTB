
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


SET(MULTISCALE_TESTS1 ${CXX_TEST_PATH}/otbMultiScaleTests1)
SET(MULTISCALE_TESTS2 ${CXX_TEST_PATH}/otbMultiScaleTests2)
SET(MULTISCALE_TESTS3 ${CXX_TEST_PATH}/otbMultiScaleTests3)
SET(MULTISCALE_TESTS4 ${CXX_TEST_PATH}/otbMultiScaleTests4)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbMULTISCALE_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::MorphologicalPyramidResampler  ----------

ADD_TEST(msTuMorphoPyrResamplerNew ${MULTISCALE_TESTS1}
         otbMorphologicalPyramidResamplerNew)

ADD_TEST(msTvMorphoPyrResampler ${MULTISCALE_TESTS1}
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

ADD_TEST(msTuMorphoPyrAnalysisFilterNew ${MULTISCALE_TESTS1}
         otbMorphologicalPyramidAnalysisFilterNew)

ADD_TEST(msTvMorphoPyrAnalysisFilter ${MULTISCALE_TESTS1}
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

ADD_TEST(msTuMorphoPyrSynthesisFilterNew ${MULTISCALE_TESTS1}
         otbMorphologicalPyramidSynthesisFilterNew)

ADD_TEST(msTvMorphoPyrSynthesisFilter ${MULTISCALE_TESTS1}
   --compare-image ${NOTOL}
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

ADD_TEST(msTuMorphoPyrSegmenterNew ${MULTISCALE_TESTS1}
         otbMorphologicalPyramidSegmenterNew)

ADD_TEST(msTvMorphoPyrSegmentBright ${MULTISCALE_TESTS1}
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
SET_TESTS_PROPERTIES(msTvMorphoPyrSegmentBright PROPERTIES DEPENDS msTvMorphoPyrMRToMSConverter)

ADD_TEST(msTvMorphoPyrSegmentDark ${MULTISCALE_TESTS1}
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
SET_TESTS_PROPERTIES(msTvMorphoPyrSegmentDark PROPERTIES DEPENDS msTvMorphoPyrMRToMSConverter)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbMULTISCALE_TESTS2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::MorphologicalPyramidSegmentationFilter   ----------

ADD_TEST(msTuMorphoPyrSegmentationFilterNew ${MULTISCALE_TESTS2}
         otbMorphologicalPyramidSegmentationFilterNew)

ADD_TEST(msTvMorphoPyrSegmentationFilter ${MULTISCALE_TESTS2}
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

ADD_TEST(msTuGeodesicMorphologyDecompositionImageFilterNew ${MULTISCALE_TESTS2}
         otbGeodesicMorphologyDecompositionImageFilterNew)

ADD_TEST(msTvGeodesicMorphologyDecompositionImageFilter ${MULTISCALE_TESTS2}
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

ADD_TEST(msTuGeodesicMorphologyLevelingFilterNew ${MULTISCALE_TESTS2}
         otbGeodesicMorphologyLevelingFilterNew)

ADD_TEST(msTvGeodesicMorphologyLevelingFilter ${MULTISCALE_TESTS2}
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

ADD_TEST(msTuConvexOrConcaveClassificationFilterNew ${MULTISCALE_TESTS2}
         otbConvexOrConcaveClassificationFilterNew)

ADD_TEST(msTvConvexOrConcaveClassificationFilter ${MULTISCALE_TESTS2}
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

ADD_TEST(msTuMorphologicalOpeningProfileFilterNew ${MULTISCALE_TESTS2}
         otbMorphologicalOpeningProfileFilterNew)

ADD_TEST(msTvMorphologicalOpeningProfileFilter ${MULTISCALE_TESTS2}
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

ADD_TEST(msTuMorphologicalClosingProfileFilterNew ${MULTISCALE_TESTS2}
         otbMorphologicalClosingProfileFilterNew)

ADD_TEST(msTvMorphologicalClosingProfileFilter ${MULTISCALE_TESTS2}
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

ADD_TEST(msTuProfileToProfileDerivativeFilterNew ${MULTISCALE_TESTS3}
         otbProfileToProfileDerivativeFilterNew)

ADD_TEST(msTvProfileToProfileDerivativeFilter ${MULTISCALE_TESTS3}
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

ADD_TEST(msTuProfileDerivativeToMultiScaleCharacteristicsFilterNew ${MULTISCALE_TESTS3}
         otbProfileDerivativeToMultiScaleCharacteristicsFilterNew)

ADD_TEST(msTvProfileDerivativeToMultiScaleCharacteristicsFilter ${MULTISCALE_TESTS3}
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

ADD_TEST(msTuMultiScaleConvexOrConcaveClassificationFilterNew ${MULTISCALE_TESTS3}
         otbMultiScaleConvexOrConcaveClassificationFilterNew)

ADD_TEST(msTvMultiScaleConvexOrConcaveClassificationFilter ${MULTISCALE_TESTS3}
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

ADD_TEST(msTuGeodesicMorphologyIterativeDecompositionImageFilterNew ${MULTISCALE_TESTS3}
         otbGeodesicMorphologyIterativeDecompositionImageFilterNew)

ADD_TEST(msTvGeodesicMorphologyIterativeDecompositionImageFilter ${MULTISCALE_TESTS3}
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

ADD_TEST(msTuWaveletOperatorNew ${MULTISCALE_TESTS4}
         otbWaveletOperatorNew )

ADD_TEST(msTuWaveletOperator ${MULTISCALE_TESTS4}
         otbWaveletOperator )

# -------            otb::WaveletFilterBankNew   ----------
ADD_TEST(msTuWaveletFilterBankNew ${MULTISCALE_TESTS4}
         otbWaveletFilterBankNew)

ADD_TEST(msTuWaveletInverseFilterBankNew ${MULTISCALE_TESTS4}
         otbWaveletInverseFilterBankNew)

ADD_TEST(msTvWaveletFilterBank ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_FilterBank.tif
         otbWaveletFilterBank
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
                  2
          ${TEMP}/ROI_IKO_PAN_LesHalles_FilterBank.tif
)

ADD_TEST(msTvWaveletMultiScaleFilterBank ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_FilterBankMultiScale.tif
         otbWaveletFilterBank
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
                  1
          ${TEMP}/ROI_IKO_PAN_LesHalles_FilterBankMultiScale.tif
)

# -------            otb::WaveletTransform   ----------
ADD_TEST(msTuWaveletTransformNew ${MULTISCALE_TESTS4}
         otbWaveletTransformNew)

ADD_TEST(msTuWaveletInverseTransformNew ${MULTISCALE_TESTS4}
         otbWaveletInverseTransformNew)

ADD_TEST(msTvWaveletTransformHaar ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
                          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltHaar.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltHaar.tif
          2 2
          0 #Haar
)



ADD_TEST(msTvWaveletMultiScaleTransformHaar ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
                          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar.tif
          2 1
          0 #Haar
)




ADD_TEST(msTvWaveletMultiScaleTransformHaar_1 ${MULTISCALE_TESTS4}
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



ADD_TEST(msTvWaveletMultiScaleTransformHaar_4 ${MULTISCALE_TESTS4}
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



ADD_TEST(msTvWaveletMultiScaleTransformHaar_8 ${MULTISCALE_TESTS4}
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


ADD_TEST(msTvWaveletTransformDB4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB4.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB4.tif
          2 2
          1 #DB4
)

ADD_TEST(msTvWaveletMultiScaleTransformDB4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4.tif
          2 1
          1 #DB4
)

ADD_TEST(msTvWaveletMultiScaleTransformDB4_1 ${MULTISCALE_TESTS4}
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


ADD_TEST(msTvWaveletMultiScaleTransformDB4_4 ${MULTISCALE_TESTS4}
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


ADD_TEST(msTvWaveletMultiScaleTransformDB4_8 ${MULTISCALE_TESTS4}
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




ADD_TEST(msTvWaveletTransformDB6 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB6.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB6.tif
          2 2
          2 #DB6
)

ADD_TEST(msTvWaveletMultiScaleTransformDB6 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB6.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB6.tif
          2 1
          2 #DB6
)


ADD_TEST(msTvWaveletMultiScaleTransformDB6_1 ${MULTISCALE_TESTS4}
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



ADD_TEST(msTvWaveletTransformDB8 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB8.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB8.tif
          2 2
          3 #DB8
)

ADD_TEST(msTvWaveletMultiScaleTransformDB8 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB8.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB8.tif
          2 1
          3 #DB8
)


ADD_TEST(msTvWaveletMultiScaleTransformDB8_1 ${MULTISCALE_TESTS4}
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


ADD_TEST(msTvWaveletTransformDB12 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB12.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB12.tif
          2 2
          4 #DB12
)

ADD_TEST(msTvWaveletMultiScaleTransformDB12 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB12.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB12.tif
          2 1
          4 #DB12
)

ADD_TEST(msTvWaveletTransformDB20 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB20.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB20.tif
          2 2
          5 #DB20
)

ADD_TEST(msTvWaveletMultiScaleTransformDB20 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB20.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB20.tif
          2 1
          5 #DB20
)

ADD_TEST(msTvWaveletTransformSPLINE_BIORTHOGONAL_2_4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltSPLINE_BIORTHOGONAL_2_4.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltSPLINE_BIORTHOGONAL_2_4.tif
          2 2
          6 #SPLINE_BIORTHOGONAL_2_4
)

ADD_TEST(msTvWaveletMultiScaleTransformSPLINE_BIORTHOGONAL_2_4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSPLINE_BIORTHOGONAL_2_4.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSPLINE_BIORTHOGONAL_2_4.tif
          2 1
          6 #SPLINE_BIORTHOGONAL_2_4
)
ADD_TEST(msTvWaveletTransformSPLINE_BIORTHOGONAL_4_4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltSPLINE_BIORTHOGONAL_4_4.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltSPLINE_BIORTHOGONAL_4_4.tif
          2 2
          7 #SPLINE_BIORTHOGONAL_4_4
)

ADD_TEST(msTvWaveletMultiScaleTransformSPLINE_BIORTHOGONAL_4_4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSPLINE_BIORTHOGONAL_4_4.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSPLINE_BIORTHOGONAL_4_4.tif
          2 1
          7 #SPLINE_BIORTHOGONAL_4_4
)
ADD_TEST(msTvWaveletTransformSYMLET8 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltSYMLET8.tif
         otbWaveletTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltSYMLET8.tif
          2 2
          8 #SYMLET8
)

ADD_TEST(msTvWaveletMultiScaleTransformSYMLET8 ${MULTISCALE_TESTS4}
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
ADD_TEST(msTuWaveletPacketTransformNew ${MULTISCALE_TESTS4}
         otbWaveletPacketTransformNew)

ADD_TEST(msTuWaveletPacketInverseTransformNew ${MULTISCALE_TESTS4}
         otbWaveletPacketInverseTransformNew)

ADD_TEST(msTvWaveletPacketTransformHaar ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketHaar.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketHaar.tif
          2 2
          0 #Haar
)

ADD_TEST(msTvWaveletPacketMultiScaleTransformHaar ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleHaar.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleHaar.tif
          2 1
          0 #Haar
)

ADD_TEST(msTvWaveletPacketTransformDB4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB4.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB4.tif
          2 2
          1 #DB4
)

ADD_TEST(msTvWaveletPacketMultiScaleTransformDB4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB4.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB4.tif
          2 1
          1 #DB4
)

ADD_TEST(msTvWaveletPacketTransformDB6 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB6.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB6.tif
          2 2
          2 #DB6
)

ADD_TEST(msTvWaveletPacketMultiScaleTransformDB6 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB6.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB6.tif
          2 1
          2 #DB6
)

ADD_TEST(msTvWaveletPacketTransformDB8 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB8.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB8.tif
          2 2
          3 #DB8
)

ADD_TEST(msTvWaveletPacketMultiScaleTransformDB8 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB8.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB8.tif
          2 1
          3 #DB8
)

ADD_TEST(msTvWaveletPacketTransformDB12 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB12.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB12.tif
          2 2
          4 #DB12
)

ADD_TEST(msTvWaveletPacketMultiScaleTransformDB12 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB12.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB12.tif
          2 1
          4 #DB12
)

ADD_TEST(msTvWaveletPacketTransformDB20 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB20.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB20.tif
          2 2
          5 #DB20
)

ADD_TEST(msTvWaveletPacketMultiScaleTransformDB20 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB20.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB20.tif
          2 1
          5 #DB20
)

ADD_TEST(msTvWaveletPacketTransformSPLINE_BIORTHOGONAL_2_4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketSPLINE_BIORTHOGONAL_2_4.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketSPLINE_BIORTHOGONAL_2_4.tif
          2 2
          6 #SPLINE_BIORTHOGONAL_2_4
)

ADD_TEST(msTvWaveletPacketMultiScaleTransformSPLINE_BIORTHOGONAL_2_4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSPLINE_BIORTHOGONAL_2_4.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSPLINE_BIORTHOGONAL_2_4.tif
          2 1
          6 #SPLINE_BIORTHOGONAL_2_4
)
ADD_TEST(msTvWaveletPacketTransformSPLINE_BIORTHOGONAL_4_4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketSPLINE_BIORTHOGONAL_4_4.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketSPLINE_BIORTHOGONAL_4_4.tif
          2 2
          7 #SPLINE_BIORTHOGONAL_4_4
)

ADD_TEST(msTvWaveletPacketMultiScaleTransformSPLINE_BIORTHOGONAL_4_4 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSPLINE_BIORTHOGONAL_4_4.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSPLINE_BIORTHOGONAL_4_4.tif
          2 1
          7 #SPLINE_BIORTHOGONAL_4_4
)
ADD_TEST(msTvWaveletPacketTransformSYMLET8 ${MULTISCALE_TESTS4}
        --compare-image ${EPSILON_6}
              ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
              ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketSYMLET8.tif
         otbWaveletPacketTransform
          ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
          ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketSYMLET8.tif
          2 2
          8 #SYMLET8
)

ADD_TEST(msTvWaveletPacketMultiScaleTransformSYMLET8 ${MULTISCALE_TESTS4}
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
ADD_TEST(msTuSubsampleImageFilterNew ${MULTISCALE_TESTS4}
        otbSubsampleImageFilterNew)

ADD_TEST(msTvSubsampleImageFilter ${MULTISCALE_TESTS4}
        --compare-image ${NOTOL}
                ${TEMP}/msSubsampleImageFilterOversampled.tif
                ${TEMP}/msSubsampleImageFilterOnePixelOutOf2.tif
        otbSubsampleImageFilter
                ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
                ${TEMP}/msSubsampleImageFilterOversampled.tif
                ${TEMP}/msSubsampleImageFilterOnePixelOutOf2.tif
)

# -------       Fichiers sources CXX -----------------------------------
SET(BasicMultiScale_SRCS1
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
SET(BasicMultiScale_SRCS2
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
SET(BasicMultiScale_SRCS3
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
SET(BasicMultiScale_SRCS4
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

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
