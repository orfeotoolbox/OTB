
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE_EXAMPLE ${OTB_DATA_ROOT}/Baseline/Examples/DisparityMap)  

SET(DISPARITYMAP_TESTS1 ${CXX_TEST_PATH}/otbDisparityMapTests1)
SET(DISPARITYMAP_TESTS2 ${CXX_TEST_PATH}/otbDisparityMapTests2)
SET(DISPARITYMAP_TESTS3 ${CXX_TEST_PATH}/otbDisparityMapTests3)
SET(DISPARITYMAP_TESTS4 ${CXX_TEST_PATH}/otbDisparityMapTests4)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbDisparityMapTests1 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::DisparityMapEstimationMethod   ----------

ADD_TEST(dmTuDisparityMapEstimationMethodNew ${DISPARITYMAP_TESTS1}
         otbDisparityMapEstimationMethodNew)

ADD_TEST(dmTvDisparityMapEstimationMethod ${DISPARITYMAP_TESTS1}
   --compare-ascii ${NOTOL}
       ${BASELINE_FILES}/dmDisparityMapEstimationOutput.txt
       ${TEMP}/dmDisparityMapEstimationOutput.txt
   otbDisparityMapEstimationMethod
             ${INPUTDATA}/fixed.png
             ${INPUTDATA}/moving.png
             ${INPUTDATA}/pointSet.png
             ${TEMP}/dmDisparityMapEstimationOutput.txt
       2 3
)
ADD_TEST(dmTvDisparityMapEstimationMethod ${DISPARITYMAP_TESTS1}
   --compare-ascii ${NOTOL}
       ${BASELINE_FILES}/dmDisparityMapEstimationOutput1.txt
       ${TEMP}/dmDisparityMapEstimationOutput1.txt
   otbDisparityMapEstimationMethod
             ${INPUTDATA}/fixed.png
             ${INPUTDATA}/moving.png
             ${INPUTDATA}/pointSet.png
             ${TEMP}/dmDisparityMapEstimationOutput1.txt
       20 20
)

# -------            otb::PointSetToDeformationFieldGenerator   ----------

ADD_TEST(dmTuPointSetToDeformationFieldGeneratorNew ${DISPARITYMAP_TESTS1}
         otbPointSetToDeformationFieldGeneratorNew)

# -------            otb::NearestPointDeformationFieldGenerator   ----------

ADD_TEST(dmTuNearestPointDeformationFieldGeneratorNew ${DISPARITYMAP_TESTS1}
         otbNearestPointDeformationFieldGeneratorNew)

ADD_TEST(dmTvNearestPointDeformationFieldGenerator ${DISPARITYMAP_TESTS1}
              --compare-image ${EPSILON_10}
                ${BASELINE}/dmTvNearestPointDeformationField.hdr
                ${TEMP}/dmTvNearestPointDeformationField.hdr
         otbNearestPointDeformationFieldGenerator
         ${TEMP}/dmTvNearestPointDeformationField.hdr
)

# -------            otb::NCCRegistrationFilter   ----------

ADD_TEST(dmTuNCCRegistrationFilterNew ${DISPARITYMAP_TESTS1}
         otbNCCRegistrationFilterNew)

ADD_TEST(dmTvNCCRegistrationFilter ${DISPARITYMAP_TESTS1}
--compare-image ${EPSILON_10}
                ${BASELINE}/dmNCCRegistrationFilterOutput.tif
                ${TEMP}/dmNCCRegistrationFilterOutput.tif
                otbNCCRegistrationFilter
                ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
                ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub_warped_sinus.tif
                ${TEMP}/dmNCCRegistrationFilterOutput.tif
                5 1.0 2)

# -------  otb::NNearestPointsLinearInterpolateDeformationFieldGenerator   ----------

ADD_TEST(dmTuNNearestPointsLinearInterpolateDeformationFieldGeneratorNew ${DISPARITYMAP_TESTS1}
         otbNNearestPointsLinearInterpolateDeformationFieldGeneratorNew)

ADD_TEST(dmTvNNearestPointsLinearInterpolateDeformationFieldGenerator ${DISPARITYMAP_TESTS1}
              --compare-image ${EPSILON_10}
                ${BASELINE}/dmTvNNearestPointsLinearInterpolateDeformationField.hdr
                ${TEMP}/dmTvNNearestPointsLinearInterpolateDeformationField.hdr
         otbNNearestPointsLinearInterpolateDeformationFieldGenerator
         ${TEMP}/dmTvNNearestPointsLinearInterpolateDeformationField.hdr
)


# -------  otb::BSplinesInterpolateDeformationFieldGenerator   ----------

ADD_TEST(dmTuBSplinesInterpolateDeformationFieldGeneratorNew ${DISPARITYMAP_TESTS1}
         otbNNearestPointsLinearInterpolateDeformationFieldGeneratorNew)

ADD_TEST(dmTvBSplinesInterpolateDeformationFieldGenerator ${DISPARITYMAP_TESTS1}
              --compare-image ${EPSILON_10}
                ${BASELINE}/dmTvBSplinesInterpolateDeformationField.hdr
                ${TEMP}/dmTvBSplinesInterpolateDeformationField.hdr
         otbBSplinesInterpolateDeformationFieldGenerator
         ${TEMP}/dmTvBSplinesInterpolateDeformationField.hdr
)


# -------            otb::PointSetWithTransformToDeformationFieldGenerator   ----------

ADD_TEST(dmTuPointSetWithTransformToDeformationFieldGeneratorNew ${DISPARITYMAP_TESTS1}
         otbPointSetWithTransformToDeformationFieldGeneratorNew)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbDisparityMapTests2 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::NearestTransformDeformationFieldGenerator   ----------

ADD_TEST(dmTuNearestTransformDeformationFieldGeneratorNew ${DISPARITYMAP_TESTS2}
         otbNearestTransformDeformationFieldGeneratorNew)

ADD_TEST(dmTvNearestTransformDeformationFieldGenerator ${DISPARITYMAP_TESTS2}
              --compare-image ${EPSILON_10}
                ${BASELINE}/dmTvNearestTransformDeformationField.hdr
                ${TEMP}/dmTvNearestTransformDeformationField.hdr
         otbNearestTransformDeformationFieldGenerator
         ${TEMP}/dmTvNearestTransformDeformationField.hdr
)


# -------  otb::NNearestTransformsLinearInterpolateDeformationFieldGenerator   ----------

ADD_TEST(dmTuNNearestTransformsLinearInterpolateDeformationFieldGeneratorNew ${DISPARITYMAP_TESTS2}
         otbNNearestTransformsLinearInterpolateDeformationFieldGeneratorNew)

ADD_TEST(dmTvNNearestTransformsLinearInterpolateDeformationFieldGenerator ${DISPARITYMAP_TESTS2}
              --compare-image ${EPSILON_10}
                ${BASELINE}/dmTvNNearestTransformsLinearInterpolateDeformationField.hdr
                ${TEMP}/dmTvNNearestTransformsLinearInterpolateDeformationField.hdr
         otbNNearestTransformsLinearInterpolateDeformationFieldGenerator
         ${TEMP}/dmTvNNearestTransformsLinearInterpolateDeformationField.hdr
)

# -------  otb::BSplinesInterpolateTransformDeformationFieldGenerator ----------

ADD_TEST(dmTuBSplinesInterpolateTransformDeformationFieldGeneratorNew ${DISPARITYMAP_TESTS2}
         otbBSplinesInterpolateTransformDeformationFieldGeneratorNew)

ADD_TEST(dmTvBSplinesInterpolateTransformDeformationFieldGenerator ${DISPARITYMAP_TESTS2}
              --compare-image ${EPSILON_4}
                ${BASELINE}/dmTvBSplinesInterpolateTransformDeformationFieldGenerator.hdr
                ${TEMP}/dmTvBSplinesInterpolateTransformDeformationFieldGenerator.hdr
         otbBSplinesInterpolateTransformDeformationFieldGenerator
         ${TEMP}/dmTvBSplinesInterpolateTransformDeformationFieldGenerator.hdr
)

# -------  Additional tests for deformation fields estimation   ----------

ADD_TEST(dmTvTranslationDeformationFieldEstimation ${DISPARITYMAP_TESTS2}
              --compare-n-images ${EPSILON_4} 12
              ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_bs_df.hdr
              ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_bs_df.hdr
              ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_bs_oi.tif
              ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_bs_oi.tif
              ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_np_df.hdr
              ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_np_df.hdr
              ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_np_oi.tif
              ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_np_oi.tif
              ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_nnp_df.hdr
              ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_nnp_df.hdr
              ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_nnp_oi.tif
              ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_nnp_oi.tif
        ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_nt_df.hdr
        ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_nt_df.hdr
        ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_nt_oi.tif
        ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_nt_oi.tif
        ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_nnt_df.hdr
        ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_nnt_df.hdr
        ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_nnt_oi.tif
        ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_nnt_oi.tif
        ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_bst_df.hdr
        ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_bst_df.hdr
        ${BASELINE}/dmTvTranslationDeformationFieldEstimationOutput_bst_oi.tif
        ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput_bst_oi.tif
         otbTranslationDeformationFieldEstimation
         ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
         ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub_warped_translation.tif
         ${TEMP}/dmTvTranslationDeformationFieldEstimationOutput
          15 10 10 250 0.95 4 50
)

ADD_TEST(dmTvEuler2DDeformationFieldEstimation ${DISPARITYMAP_TESTS2}
              --compare-n-images ${EPSILON_4} 12
              ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_bs_df.hdr
              ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_bs_df.hdr
              ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_bs_oi.tif
              ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_bs_oi.tif
              ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_np_df.hdr
              ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_np_df.hdr
              ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_np_oi.tif
              ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_np_oi.tif
              ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_nnp_df.hdr
              ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_nnp_df.hdr
              ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_nnp_oi.tif
              ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_nnp_oi.tif
              ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_nt_df.hdr
              ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_nt_df.hdr
              ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_nt_oi.tif
              ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_nt_oi.tif
              ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_nnt_df.hdr
              ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_nnt_df.hdr
              ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_nnt_oi.tif
              ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_nnt_oi.tif
              ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_bst_df.hdr
              ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_bst_df.hdr
              ${BASELINE}/dmTvEuler2DDeformationFieldEstimationOutput_bst_oi.tif
              ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput_bst_oi.tif
         otbEuler2DDeformationFieldEstimation
         ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
         ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub_warped_euler2D.tif
         ${TEMP}/dmTvEuler2DDeformationFieldEstimationOutput
         15 10 0.01 250 0.95 4 50 128 128
)

ADD_TEST(dmTvCenteredRigidDeformationFieldEstimation ${DISPARITYMAP_TESTS2}
              --compare-n-images ${EPSILON_4} 12
              ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_bs_df.hdr
              ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_bs_df.hdr
              ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_bs_oi.tif
              ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_bs_oi.tif
              ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_np_df.hdr
              ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_np_df.hdr
              ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_np_oi.tif
              ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_np_oi.tif
              ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_nnp_df.hdr
              ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_nnp_df.hdr
              ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_nnp_oi.tif
              ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_nnp_oi.tif
        ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_nt_df.hdr
        ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_nt_df.hdr
        ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_nt_oi.tif
        ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_nt_oi.tif
        ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_nnt_df.hdr
        ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_nnt_df.hdr
        ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_nnt_oi.tif
        ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_nnt_oi.tif
        ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_bst_df.hdr
        ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_bst_df.hdr
        ${BASELINE}/dmTvCenteredRigidDeformationFieldEstimationOutput_bst_oi.tif
        ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput_bst_oi.tif
         otbCenteredRigidDeformationFieldEstimation
         ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
         ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub_warped_centered_rigid.tif
         ${TEMP}/dmTvCenteredRigidDeformationFieldEstimationOutput
         15 5 0.01 250 0.95 4 50 127 65
)


ADD_TEST(dmTvSinusoidDeformationFieldEstimation ${DISPARITYMAP_TESTS2}
              --compare-n-images ${EPSILON_4} 12
              ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_bs_df.hdr
              ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_bs_df.hdr
              ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_bs_oi.tif
              ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_bs_oi.tif
              ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_np_df.hdr
              ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_np_df.hdr
              ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_np_oi.tif
              ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_np_oi.tif
              ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_nnp_df.hdr
              ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_nnp_df.hdr
              ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_nnp_oi.tif
              ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_nnp_oi.tif
        ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_nt_df.hdr
        ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_nt_df.hdr
        ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_nt_oi.tif
        ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_nt_oi.tif
        ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_nnt_df.hdr
        ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_nnt_df.hdr
        ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_nnt_oi.tif
        ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_nnt_oi.tif
        ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_bst_df.hdr
        ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_bst_df.hdr
        ${BASELINE}/dmTvSinusoidDeformationFieldEstimationOutput_bst_oi.tif
        ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput_bst_oi.tif
         otbTranslationDeformationFieldEstimation
         ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
         ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub_warped_sinus.tif
         ${TEMP}/dmTvSinusoidDeformationFieldEstimationOutput
          15 10 10 250 0.95 4 25
)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbDisparityMapTests3 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# ------- otb::StreamingWarpImageFilter  ----------


ADD_TEST(dmTuStreamingWarpImageFilterNew ${DISPARITYMAP_TESTS3}
        otbStreamingWarpImageFilterNew)

ADD_TEST(dmTvStreamingWarpImageFilter ${DISPARITYMAP_TESTS3}
        --compare-image ${EPSILON_10}
        ${BASELINE}/dmStreamingWarpImageFilterOutput.tif
        ${TEMP}/dmStreamingWarpImageFilterOutput.tif
        otbStreamingWarpImageFilter
        ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
        ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub_deformation_field.tif
        ${TEMP}/dmStreamingWarpImageFilterOutput.tif
        5
)

# ------- otb::FineRegistrationImageFilter ----------

ADD_TEST(feTuFineRegistrationImageFilterNew ${DISPARITYMAP_TESTS3}
        otbFineRegistrationImageFilterNew
)

ADD_TEST(feTvFineRegistrationImageFilterTestWithCorrelation ${DISPARITYMAP_TESTS3}
     --compare-n-images ${EPSILON_10} 2
        ${BASELINE}/feTvFineRegistrationImageFilterTestWithCorrelationMetric.tif
        ${TEMP}/feTvFineRegistrationImageFilterTestWithCorrelationMetric.tif
        ${BASELINE}/feTvFineRegistrationImageFilterTestWithCorrelationField.tif
        ${TEMP}/feTvFineRegistrationImageFilterTestWithCorrelationField.tif
        otbFineRegistrationImageFilterTest
            ${EXAMPLEDATA}/StereoFixed.png # fixedFileName
            ${EXAMPLEDATA}/StereoMoving.png # movingFileName
            ${TEMP}/feTvFineRegistrationImageFilterTestWithCorrelationMetric.tif # output correlFileName
        ${TEMP}/feTvFineRegistrationImageFilterTestWithCorrelationField.tif  # output fieldFileName
        3 # radius
            2 # sradius
            0.1 # precision
            0 # Correlation
            1 # Grid step
            0 # Initial offset x
            0 # Initial offset y
            0 0 80 130 # region to proceed
)

ADD_TEST(feTvFineRegistrationImageFilterTestWithNormalizedCorrelation ${DISPARITYMAP_TESTS3}
     --compare-n-images ${EPSILON_10} 2
        ${BASELINE}/feTvFineRegistrationImageFilterTestWithNormalizedCorrelationMetric.tif
        ${TEMP}/feTvFineRegistrationImageFilterTestWithNormalizedCorrelationMetric.tif
        ${BASELINE}/feTvFineRegistrationImageFilterTestWithNormalizedCorrelationField.tif
        ${TEMP}/feTvFineRegistrationImageFilterTestWithNormalizedCorrelationField.tif
        otbFineRegistrationImageFilterTest
            ${EXAMPLEDATA}/StereoFixed.png # fixedFileName
            ${EXAMPLEDATA}/StereoMoving.png # movingFileName
            ${TEMP}/feTvFineRegistrationImageFilterTestWithNormalizedCorrelationMetric.tif # output correlFileName
        ${TEMP}/feTvFineRegistrationImageFilterTestWithNormalizedCorrelationField.tif  # output fieldFileName
        3 # radius
            2 # sradius
            0.1 # precision
            1 # Normalized Correlation
            1 # Grid step
            0 # Initial offset x
            0 # Initial offset y
            0 0 80 130 # region to proceed
)

ADD_TEST(feTvFineRegistrationImageFilterTestWithMeanSquare ${DISPARITYMAP_TESTS3}
     --compare-n-images ${EPSILON_10} 2
        ${BASELINE}/feTvFineRegistrationImageFilterTestWithMeanSquareMetric.tif
        ${TEMP}/feTvFineRegistrationImageFilterTestWithMeanSquareMetric.tif
        ${BASELINE}/feTvFineRegistrationImageFilterTestWithMeanSquareField.tif
        ${TEMP}/feTvFineRegistrationImageFilterTestWithMeanSquareField.tif
        otbFineRegistrationImageFilterTest
            ${EXAMPLEDATA}/StereoFixed.png # fixedFileName
            ${EXAMPLEDATA}/StereoMoving.png # movingFileName
            ${TEMP}/feTvFineRegistrationImageFilterTestWithMeanSquareMetric.tif # output correlFileName
        ${TEMP}/feTvFineRegistrationImageFilterTestWithMeanSquareField.tif  # output fieldFileName
        3 # radius
            2 # sradius
            0.1 # precision
            2 # Mean square
            1 # Grid step
            0 # Initial offset x
            0 # Initial offset y
            0 0 80 65 # region to proceed
)

ADD_TEST(feTvFineRegistrationImageFilterTestWithMeanReciprocalDifference ${DISPARITYMAP_TESTS3}
     --compare-n-images ${EPSILON_10} 2
        ${BASELINE}/feTvFineRegistrationImageFilterTestWithMeanReciprocalDifferenceMetric.tif
        ${TEMP}/feTvFineRegistrationImageFilterTestWithMeanReciprocalDifferenceMetric.tif
        ${BASELINE}/feTvFineRegistrationImageFilterTestWithMeanReciprocalDifferenceField.tif
        ${TEMP}/feTvFineRegistrationImageFilterTestWithMeanReciprocalDifferenceField.tif
        otbFineRegistrationImageFilterTest
            ${EXAMPLEDATA}/StereoFixed.png # fixedFileName
            ${EXAMPLEDATA}/StereoMoving.png # movingFileName
            ${TEMP}/feTvFineRegistrationImageFilterTestWithMeanReciprocalDifferenceMetric.tif # output correlFileName
        ${TEMP}/feTvFineRegistrationImageFilterTestWithMeanReciprocalDifferenceField.tif  # output fieldFileName
        3 # radius
            2 # sradius
            0.1 # precision
            3 # Mean reciprocal difference
            1 # Grid step
            0 # Initial offset x
            0 # Initial offset y
            0 0 80 130 # region to proceed
)

#--------- otb::StereoSensorModelToElevationFilter -----------------------------------------
ADD_TEST(feTuStereoSensorModelToElevationFilterNew ${DISPARITYMAP_TESTS3}
otbStereoSensorModelToElevationMapFilterNew)


ADD_TEST(feTvStereoSensorModelToElevationFilter ${DISPARITYMAP_TESTS3}
  --compare-image ${EPSILON_10}
  ${BASELINE}/feTvStereoSensorModelToElevationFilterOutput.tif
  ${TEMP}/feTvStereoSensorModelToElevationFilterOutput.tif
otbStereoSensorModelToElevationMapFilter
${EXAMPLEDATA}/sensor_stereo_left.tif
${EXAMPLEDATA}/sensor_stereo_right.tif
${TEMP}/feTvStereoSensorModelToElevationFilterOutput.tif
${INPUTDATA}/DEM/srtm_directory/
${INPUTDATA}/DEM/egm96.grd
-20
20
1
3
)

#--------- otb::StereorectificationDeformationFieldSource -----------------------------------------
ADD_TEST(feTuStereorectificationDeformationFieldSourceNew ${DISPARITYMAP_TESTS3}
otbStereorectificationDeformationFieldSourceNew)

ADD_TEST(feTvStereorectificationDeformationFieldSource ${DISPARITYMAP_TESTS3}
--compare-n-images ${EPSILON_10} 2
${BASELINE}/feTvStereorectificationDeformationFieldSourceOutput1.tif
${TEMP}/feTvStereorectificationDeformationFieldSourceOutput1.tif
${BASELINE}/feTvStereorectificationDeformationFieldSourceOutput2.tif
${TEMP}/feTvStereorectificationDeformationFieldSourceOutput2.tif
otbStereorectificationDeformationFieldSource
${EXAMPLEDATA}/sensor_stereo_left.tif
${EXAMPLEDATA}/sensor_stereo_right.tif
${TEMP}/feTvStereorectificationDeformationFieldSourceOutput1.tif
${TEMP}/feTvStereorectificationDeformationFieldSourceOutput2.tif
300
0.5
5
)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbDisparityMapTests4 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#--------- otb::HorizontalPixelWiseBlockMatchingImageFilter -----------------------------------------
ADD_TEST(dmTuPixelWiseBlockMatchingImageFilterNew ${DISPARITYMAP_TESTS4}
otbPixelWiseBlockMatchingImageFilterNew)

ADD_TEST(dmTvPixelWiseBlockMatchingImageFilter ${DISPARITYMAP_TESTS4}
--compare-n-images ${NOTOL} 2
${BASELINE}/dmTvPixelWiseBlockMatchingImageFilterOutputDisparity.tif
${TEMP}/dmTvPixelWiseBlockMatchingImageFilterOutputDisparity.tif
${BASELINE}/dmTvPixelWiseBlockMatchingImageFilterOutputMetric.tif
${TEMP}/dmTvPixelWiseBlockMatchingImageFilterOutputMetric.tif
otbPixelWiseBlockMatchingImageFilter
${EXAMPLEDATA}/StereoFixed.png
${EXAMPLEDATA}/StereoMoving.png
${TEMP}/dmTvPixelWiseBlockMatchingImageFilterOutputDisparity.tif
${TEMP}/dmTvPixelWiseBlockMatchingImageFilterOutputMetric.tif
2
-10 +10
)
ADD_TEST(dmTvPixelWiseBlockMatchingImageFilterNCC ${DISPARITYMAP_TESTS4}
--compare-n-images ${NOTOL} 2
${BASELINE}/dmTvPixelWiseBlockMatchingImageFilterNCCOutputDisparity.tif
${TEMP}/dmTvPixelWiseBlockMatchingImageFilterNCCOutputDisparity.tif
${BASELINE}/dmTvPixelWiseBlockMatchingImageFilterNCCOutputMetric.tif
${TEMP}/dmTvPixelWiseBlockMatchingImageFilterNCCOutputMetric.tif
otbPixelWiseBlockMatchingImageFilterNCC
${EXAMPLEDATA}/StereoFixed.png
${EXAMPLEDATA}/StereoMoving.png
${TEMP}/dmTvPixelWiseBlockMatchingImageFilterNCCOutputDisparity.tif
${TEMP}/dmTvPixelWiseBlockMatchingImageFilterNCCOutputMetric.tif
2
-10 +10
)


# -------            otb::SubPixelDisparityImageFilterr   ----------
ADD_TEST(dmTuSubPixelDisparityImageFilterNew ${DISPARITYMAP_TESTS4}
         otbSubPixelDisparityImageFilterNew)

        
ADD_TEST(dmTvSubPixelDisparityImageFilterNCC ${DISPARITYMAP_TESTS4}
--compare-n-images ${NOTOL} 3
${BASELINE}/dmTvSubPixelWiseBlockMatchingImageFilterNCCOutputHDisparity.tif
${TEMP}/dmTvSubPixelWiseBlockMatchingImageFilterNCCOutputHDisparity.tif
${BASELINE}/dmTvSubPixelWiseBlockMatchingImageFilterNCCOutputVDisparity.tif
${TEMP}/dmTvSubPixelWiseBlockMatchingImageFilterNCCOutputVDisparity.tif
${BASELINE}/dmTvSubPixelWiseBlockMatchingImageFilterNCCOutputMetric.tif
${TEMP}/dmTvSubPixelWiseBlockMatchingImageFilterNCCOutputMetric.tif
otbSubPixelDisparityImageFilter
${EXAMPLEDATA}/StereoFixed.png
${EXAMPLEDATA}/StereoMoving.png
${INPUTDATA}/StereoPixelWiseBlockMatchingDisparity.tif
${INPUTDATA}/StereoPixelWiseBlockMatchingMetric.tif
${TEMP}/dmTvSubPixelWiseBlockMatchingImageFilterNCCOutputHDisparity.tif
${TEMP}/dmTvSubPixelWiseBlockMatchingImageFilterNCCOutputVDisparity.tif
${TEMP}/dmTvSubPixelWiseBlockMatchingImageFilterNCCOutputMetric.tif
1 # NCC
1 # Parabolic
2 # radius
-3 +3 # hdisp treshold
-2 2 # vdisp treshold
)

# -------            otb::DisparityMapMedianFilter   ----------

ADD_TEST(dmTuDisparityMapMedianFilterNew ${DISPARITYMAP_TESTS4}
         otbDisparityMapMedianFilterNew)
         
ADD_TEST(dmTvDisparityMapMedianFilter ${DISPARITYMAP_TESTS4}
--compare-image ${NOTOL} 
		 ${BASELINE}/dmTvDisparityMapMedianFilterOutput.tif
		 ${TEMP}/dmTvDisparityMapMedianFilterOutput.tif
         otbDisparityMapMedianFilter
         ${INPUTDATA}/StereoHDisparity.tif
         ${TEMP}/dmTvDisparityMapMedianFilterOutput.tif
         2
         2.0
         )         

# -------            otb::AdhesionCorrectionFilter   ----------

ADD_TEST(dmTuAdhesionCorrectionFilterNew ${DISPARITYMAP_TESTS4}
         otbAdhesionCorrectionFilterNew)
         
ADD_TEST(dmTvAdhesionCorrectionFilter ${DISPARITYMAP_TESTS4}
   --compare-n-images ${EPSILON_4} 3
        ${BASELINE}/dmTuAdhesionCorrectionMethod_Corrected.tif
        ${TEMP}/dmTuAdhesionCorrectionMethod_Corrected.tif
        ${BASELINE}/dmTuAdhesionCorrectionMethod_CorrectedMask.png
        ${TEMP}/dmTuAdhesionCorrectionMethod_CorrectedMask.png
        ${BASELINE}/dmTuAdhesionCorrectionMethod_RiskEdges.tif
        ${TEMP}/dmTuAdhesionCorrectionMethod_RiskEdges.tif
	    otbAdhesionCorrectionFilter
        ${INPUTDATA}/AdhesionCorrectionFilter_MedianInput
        ${INPUTDATA}/AdhesionCorrectionFilter_MedianMaskInput.png
        ${INPUTDATA}/AdhesionCorrectionFilter_SubpixelDisparityInput
        ${INPUTDATA}/AdhesionCorrectionFilter_SubpixelMaskInput.png
        ${INPUTDATA}/AdhesionCorrectionFilter_CannyRefInput
        ${INPUTDATA}/AdhesionCorrectionFilter_CannyMedianInput
        4 0.25
        ${TEMP}/dmTuAdhesionCorrectionMethod_Corrected.tif
        ${TEMP}/dmTuAdhesionCorrectionMethod_CorrectedMask.png
        ${TEMP}/dmTuAdhesionCorrectionMethod_RiskEdges.tif
        )                 

# -------            otb::DisparityMapToDEMFilter   ----------

ADD_TEST(dmTuDisparityMapToDEMFilterNew ${DISPARITYMAP_TESTS4}
         otbDisparityMapToDEMFilterNew)
         


ADD_TEST(dmTvDisparityMapToDEMFilter ${DISPARITYMAP_TESTS4}
--compare-image ${NOTOL}
		 ${BASELINE_EXAMPLE}/elevationOutput.tif
		 ${TEMP}/dmTvDisparityMapToDEMFilterOutput.tif
         otbDisparityMapToDEMFilter
         ${INPUTDATA}/sensor_stereo_blockmatching_output.tif 
         ${EXAMPLEDATA}/sensor_stereo_left.tif
         ${EXAMPLEDATA}/sensor_stereo_right.tif
         ${INPUTDATA}/sensor_stereo_rectif_left.tif
         ${INPUTDATA}/sensor_stereo_rectif_right.tif
         ${TEMP}/dmTvDisparityMapToDEMFilterOutput.tif
         130
         220
         140
         2.5
         ${INPUTDATA}/sensor_stereo_dmap_mask.tif
         )                              

# -------            otb::DisparityMapTo3DFilter   ----------

ADD_TEST(dmTuDisparityMapTo3DFilterNew ${DISPARITYMAP_TESTS4}
         otbDisparityMapTo3DFilterNew)
         
ADD_TEST(dmTvDisparityMapTo3DFilter ${DISPARITYMAP_TESTS4}
--compare-image ${EPSILON_6}
     ${BASELINE}/dmTvDisparityMapTo3DFilterOutput.tif
     ${TEMP}/dmTvDisparityMapTo3DFilterOutput.tif
         otbDisparityMapTo3DFilter
         ${INPUTDATA}/sensor_stereo_blockmatching_output.tif 
         ${EXAMPLEDATA}/sensor_stereo_left.tif
         ${EXAMPLEDATA}/sensor_stereo_right.tif
         ${INPUTDATA}/sensor_stereo_rectif_left.tif
         ${INPUTDATA}/sensor_stereo_rectif_right.tif
         ${TEMP}/dmTvDisparityMapTo3DFilterOutput.tif
         ${INPUTDATA}/sensor_stereo_dmap_mask.tif
         )                              

# -------            otb::MultiDisparityMapTo3DFilter   ----------
ADD_TEST(dmTuMultiDisparityMapTo3DFilterNew ${DISPARITYMAP_TESTS4}
         otbMultiDisparityMapTo3DFilterNew)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(dmTvMultiDisparityMapTo3DFilter ${DISPARITYMAP_TESTS4}
--compare-n-images ${EPSILON_6} 2
     ${BASELINE}/dmTvMultiDisparityMapTo3DFilterOutput.tif
     ${TEMP}/dmTvMultiDisparityMapTo3DFilterOutput.tif
     ${BASELINE}/dmTvMultiDisparityMapTo3DFilterResidue.tif
     ${TEMP}/dmTvMultiDisparityMapTo3DFilterResidue.tif
         otbMultiDisparityMapTo3DFilter
         ${LARGEINPUT}/PLEIADES/tristereo_sample/master_pan.tif
         ${LARGEINPUT}/PLEIADES/tristereo_sample/slave_pan_1.tif
         ${LARGEINPUT}/PLEIADES/tristereo_sample/slave_pan_2.tif
         ${LARGEINPUT}/PLEIADES/tristereo_sample/phys_disp_1.tif
         ${LARGEINPUT}/PLEIADES/tristereo_sample/phys_disp_2.tif
         ${LARGEINPUT}/PLEIADES/tristereo_sample/mask_1.tif
         ${LARGEINPUT}/PLEIADES/tristereo_sample/mask_2.tif
         ${TEMP}/dmTvMultiDisparityMapTo3DFilterOutput.tif
         ${TEMP}/dmTvMultiDisparityMapTo3DFilterResidue.tif
         )
ENDIF(OTB_DATA_USE_LARGEINPUT)

# -------            otb::BijectionCoherencyFilter   ----------
ADD_TEST(dmTuBijectionCoherencyFilterNew ${DISPARITYMAP_TESTS4}
         otbBijectionCoherencyFilterNew)

ADD_TEST(dmTvBijectionCoherencyFilter ${DISPARITYMAP_TESTS4}
--compare-image ${EPSILON_6}
     ${BASELINE}/dmTvBijectionCoherencyFilterOutput.tif
     ${TEMP}/dmTvBijectionCoherencyFilterOutput.tif
         otbBijectionCoherencyFilter
         ${INPUTDATA}/sensor_stereo_blockmatching_output.tif
         ${INPUTDATA}/sensor_stereo_reverse_blockmatching.tif
         -24
         0
         0
         0
         ${TEMP}/dmTvBijectionCoherencyFilterOutput.tif
         )


# -------       Fichiers sources CXX -----------------------------------
SET(BasicDisparityMap_SRCS1
otbDisparityMapTests1.cxx
otbDisparityMapEstimationMethodNew.cxx
otbDisparityMapEstimationMethod.cxx
otbNCCRegistrationFilterNew.cxx
otbNCCRegistrationFilter.cxx
otbPointSetToDeformationFieldGeneratorNew.cxx
otbNearestPointDeformationFieldGeneratorNew.cxx
otbNearestPointDeformationFieldGenerator.cxx
otbNNearestPointsLinearInterpolateDeformationFieldGeneratorNew.cxx
otbNNearestPointsLinearInterpolateDeformationFieldGenerator.cxx
otbBSplinesInterpolateDeformationFieldGeneratorNew.cxx
otbBSplinesInterpolateDeformationFieldGenerator.cxx
otbPointSetWithTransformToDeformationFieldGeneratorNew.cxx
)
SET(BasicDisparityMap_SRCS2
otbDisparityMapTests2.cxx
otbNearestTransformDeformationFieldGeneratorNew.cxx
otbNearestTransformDeformationFieldGenerator.cxx
otbNNearestTransformsLinearInterpolateDeformationFieldGeneratorNew.cxx
otbNNearestTransformsLinearInterpolateDeformationFieldGenerator.cxx
otbTranslationDeformationFieldEstimation.cxx
otbEuler2DDeformationFieldEstimation.cxx
otbCenteredRigidDeformationFieldEstimation.cxx
otbBSplinesInterpolateTransformDeformationFieldGeneratorNew.cxx
otbBSplinesInterpolateTransformDeformationFieldGenerator.cxx
)
SET(BasicDisparityMap_SRCS3
otbDisparityMapTests3.cxx
otbStreamingWarpImageFilterNew.cxx
otbStreamingWarpImageFilter.cxx
otbFineRegistrationImageFilterNew.cxx
otbFineRegistrationImageFilterTest.cxx
otbStereoSensorModelToElevationMapFilter.cxx
otbStereorectificationDeformationFieldSource.cxx
)

SET(BasicDisparityMap_SRCS4
otbDisparityMapTests4.cxx
otbAdhesionCorrectionFilter.cxx
otbDisparityMapMedianFilter.cxx
otbDisparityMapToDEMFilter.cxx
otbPixelWiseBlockMatchingImageFilter.cxx
otbSubPixelDisparityImageFilter.cxx
otbDisparityMapTo3DFilter.cxx
otbMultiDisparityMapTo3DFilter.cxx
otbBijectionCoherencyFilter.cxx
)

OTB_ADD_EXECUTABLE(otbDisparityMapTests1 "${BasicDisparityMap_SRCS1}" "OTBDisparityMap;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbDisparityMapTests2 "${BasicDisparityMap_SRCS2}" "OTBDisparityMap;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbDisparityMapTests3 "${BasicDisparityMap_SRCS3}" "OTBDisparityMap;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbDisparityMapTests4 "${BasicDisparityMap_SRCS4}" "OTBDisparityMap;OTBIO;OTBTesting")


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
