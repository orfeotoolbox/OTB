
if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(BASELINE_EXAMPLE ${OTB_DATA_ROOT}/Baseline/Examples/DisparityMap)

set(DISPARITYMAP_TESTS1 ${CXX_TEST_PATH}/otbDisparityMapTests1)
set(DISPARITYMAP_TESTS2 ${CXX_TEST_PATH}/otbDisparityMapTests2)
set(DISPARITYMAP_TESTS3 ${CXX_TEST_PATH}/otbDisparityMapTests3)
set(DISPARITYMAP_TESTS4 ${CXX_TEST_PATH}/otbDisparityMapTests4)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbDisparityMapTests1 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::DisparityMapEstimationMethod   ----------

add_test(dmTuDisparityMapEstimationMethodNew ${DISPARITYMAP_TESTS1}
         otbDisparityMapEstimationMethodNew)

add_test(dmTvDisparityMapEstimationMethod ${DISPARITYMAP_TESTS1}
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
add_test(dmTvDisparityMapEstimationMethod ${DISPARITYMAP_TESTS1}
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

# -------            otb::PointSetToDisplacementFieldGenerator   ----------

add_test(dmTuPointSetToDisplacementFieldGeneratorNew ${DISPARITYMAP_TESTS1}
         otbPointSetToDisplacementFieldGeneratorNew)

# -------            otb::NearestPointDisplacementFieldGenerator   ----------

add_test(dmTuNearestPointDisplacementFieldGeneratorNew ${DISPARITYMAP_TESTS1}
         otbNearestPointDisplacementFieldGeneratorNew)

add_test(dmTvNearestPointDisplacementFieldGenerator ${DISPARITYMAP_TESTS1}
              --compare-image ${EPSILON_10}
                ${BASELINE}/dmTvNearestPointDeformationField.hdr
                ${TEMP}/dmTvNearestPointDisplacementField.hdr
         otbNearestPointDisplacementFieldGenerator
         ${TEMP}/dmTvNearestPointDisplacementField.hdr
)

# -------            otb::NCCRegistrationFilter   ----------

add_test(dmTuNCCRegistrationFilterNew ${DISPARITYMAP_TESTS1}
         otbNCCRegistrationFilterNew)

add_test(dmTvNCCRegistrationFilter ${DISPARITYMAP_TESTS1}
--compare-image ${EPSILON_10}
                ${BASELINE}/dmNCCRegistrationFilterOutput.tif
                ${TEMP}/dmNCCRegistrationFilterOutput.tif
                otbNCCRegistrationFilter
                ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
                ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub_warped_sinus.tif
                ${TEMP}/dmNCCRegistrationFilterOutput.tif
                5 1.0 2)

# -------  otb::NNearestPointsLinearInterpolateDisplacementFieldGenerator   ----------

add_test(dmTuNNearestPointsLinearInterpolateDisplacementFieldGeneratorNew ${DISPARITYMAP_TESTS1}
         otbNNearestPointsLinearInterpolateDisplacementFieldGeneratorNew)

add_test(dmTvNNearestPointsLinearInterpolateDisplacementFieldGenerator ${DISPARITYMAP_TESTS1}
              --compare-image ${EPSILON_10}
                ${BASELINE}/dmTvNNearestPointsLinearInterpolateDeformationField.hdr
                ${TEMP}/dmTvNNearestPointsLinearInterpolateDisplacementField.hdr
         otbNNearestPointsLinearInterpolateDisplacementFieldGenerator
         ${TEMP}/dmTvNNearestPointsLinearInterpolateDisplacementField.hdr
)


# -------  otb::BSplinesInterpolateDisplacementFieldGenerator   ----------

add_test(dmTuBSplinesInterpolateDisplacementFieldGeneratorNew ${DISPARITYMAP_TESTS1}
         otbNNearestPointsLinearInterpolateDisplacementFieldGeneratorNew)

add_test(dmTvBSplinesInterpolateDisplacementFieldGenerator ${DISPARITYMAP_TESTS1}
              --compare-image ${EPSILON_10}
                ${BASELINE}/dmTvBSplinesInterpolateDeformationField.hdr
                ${TEMP}/dmTvBSplinesInterpolateDisplacementField.hdr
         otbBSplinesInterpolateDisplacementFieldGenerator
         ${TEMP}/dmTvBSplinesInterpolateDisplacementField.hdr
)


# -------            otb::PointSetWithTransformToDisplacementFieldGenerator   ----------

add_test(dmTuPointSetWithTransformToDisplacementFieldGeneratorNew ${DISPARITYMAP_TESTS1}
         otbPointSetWithTransformToDisplacementFieldGeneratorNew)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbDisparityMapTests2 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::NearestTransformDisplacementFieldGenerator   ----------

add_test(dmTuNearestTransformDisplacementFieldGeneratorNew ${DISPARITYMAP_TESTS2}
         otbNearestTransformDisplacementFieldGeneratorNew)

add_test(dmTvNearestTransformDisplacementFieldGenerator ${DISPARITYMAP_TESTS2}
              --compare-image ${EPSILON_10}
                ${BASELINE}/dmTvNearestTransformDeformationField.hdr
                ${TEMP}/dmTvNearestTransformDisplacementField.hdr
         otbNearestTransformDisplacementFieldGenerator
         ${TEMP}/dmTvNearestTransformDisplacementField.hdr
)


# -------  otb::NNearestTransformsLinearInterpolateDisplacementFieldGenerator   ----------

add_test(dmTuNNearestTransformsLinearInterpolateDisplacementFieldGeneratorNew ${DISPARITYMAP_TESTS2}
         otbNNearestTransformsLinearInterpolateDisplacementFieldGeneratorNew)

add_test(dmTvNNearestTransformsLinearInterpolateDisplacementFieldGenerator ${DISPARITYMAP_TESTS2}
              --compare-image ${EPSILON_10}
                ${BASELINE}/dmTvNNearestTransformsLinearInterpolateDeformationField.hdr
                ${TEMP}/dmTvNNearestTransformsLinearInterpolateDisplacementField.hdr
         otbNNearestTransformsLinearInterpolateDisplacementFieldGenerator
         ${TEMP}/dmTvNNearestTransformsLinearInterpolateDisplacementField.hdr
)

# -------  otb::BSplinesInterpolateTransformDisplacementFieldGenerator ----------

add_test(dmTuBSplinesInterpolateTransformDisplacementFieldGeneratorNew ${DISPARITYMAP_TESTS2}
         otbBSplinesInterpolateTransformDisplacementFieldGeneratorNew)

add_test(dmTvBSplinesInterpolateTransformDisplacementFieldGenerator ${DISPARITYMAP_TESTS2}
              --compare-image ${EPSILON_4}
                ${BASELINE}/dmTvBSplinesInterpolateTransformDeformationFieldGenerator.hdr
                ${TEMP}/dmTvBSplinesInterpolateTransformDisplacementFieldGenerator.hdr
         otbBSplinesInterpolateTransformDisplacementFieldGenerator
         ${TEMP}/dmTvBSplinesInterpolateTransformDisplacementFieldGenerator.hdr
)




# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbDisparityMapTests3 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# ------- otb::StreamingWarpImageFilter  ----------


add_test(dmTuStreamingWarpImageFilterNew ${DISPARITYMAP_TESTS3}
        otbStreamingWarpImageFilterNew)

add_test(dmTvStreamingWarpImageFilter ${DISPARITYMAP_TESTS3}
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

add_test(dmTuFineRegistrationImageFilterNew ${DISPARITYMAP_TESTS3}
        otbFineRegistrationImageFilterNew
)

add_test(dmTvFineRegistrationImageFilterTestWithCorrelation ${DISPARITYMAP_TESTS3}
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

add_test(dmTvFineRegistrationImageFilterTestWithNormalizedCorrelation ${DISPARITYMAP_TESTS3}
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

add_test(dmTvFineRegistrationImageFilterTestWithMeanSquare ${DISPARITYMAP_TESTS3}
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

add_test(dmTvFineRegistrationImageFilterTestWithMeanReciprocalDifference ${DISPARITYMAP_TESTS3}
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
add_test(dmTuStereoSensorModelToElevationFilterNew ${DISPARITYMAP_TESTS3}
otbStereoSensorModelToElevationMapFilterNew)


add_test(dmTvStereoSensorModelToElevationFilter ${DISPARITYMAP_TESTS3}
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

#--------- otb::StereorectificationDisplacementFieldSource -----------------------------------------
add_test(dmTuStereorectificationDisplacementFieldSourceNew ${DISPARITYMAP_TESTS3}
otbStereorectificationDisplacementFieldSourceNew)

add_test(dmTvStereorectificationDisplacementFieldSource ${DISPARITYMAP_TESTS3}
--compare-n-images ${EPSILON_10} 2
${BASELINE}/feTvStereorectificationDeformationFieldSourceOutput1.tif
${TEMP}/feTvStereorectificationDisplacementFieldSourceOutput1.tif
${BASELINE}/feTvStereorectificationDeformationFieldSourceOutput2.tif
${TEMP}/feTvStereorectificationDisplacementFieldSourceOutput2.tif
otbStereorectificationDisplacementFieldSource
${EXAMPLEDATA}/sensor_stereo_left.tif
${EXAMPLEDATA}/sensor_stereo_right.tif
${TEMP}/feTvStereorectificationDisplacementFieldSourceOutput1.tif
${TEMP}/feTvStereorectificationDisplacementFieldSourceOutput2.tif
300
0.5
5
)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbDisparityMapTests4 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#--------- otb::HorizontalPixelWiseBlockMatchingImageFilter -----------------------------------------
add_test(dmTuPixelWiseBlockMatchingImageFilterNew ${DISPARITYMAP_TESTS4}
otbPixelWiseBlockMatchingImageFilterNew)

add_test(dmTvPixelWiseBlockMatchingImageFilter ${DISPARITYMAP_TESTS4}
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
add_test(dmTvPixelWiseBlockMatchingImageFilterNCC ${DISPARITYMAP_TESTS4}
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
add_test(dmTuSubPixelDisparityImageFilterNew ${DISPARITYMAP_TESTS4}
         otbSubPixelDisparityImageFilterNew)


add_test(dmTvSubPixelDisparityImageFilterNCC ${DISPARITYMAP_TESTS4}
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

add_test(dmTuDisparityMapMedianFilterNew ${DISPARITYMAP_TESTS4}
         otbDisparityMapMedianFilterNew)

add_test(dmTvDisparityMapMedianFilter ${DISPARITYMAP_TESTS4}
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

add_test(dmTuAdhesionCorrectionFilterNew ${DISPARITYMAP_TESTS4}
         otbAdhesionCorrectionFilterNew)

add_test(dmTvAdhesionCorrectionFilter ${DISPARITYMAP_TESTS4}
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

add_test(dmTuDisparityMapToDEMFilterNew ${DISPARITYMAP_TESTS4}
         otbDisparityMapToDEMFilterNew)




add_test(dmTvDisparityMapToDEMFilter ${DISPARITYMAP_TESTS4}
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

add_test(dmTuDisparityMapTo3DFilterNew ${DISPARITYMAP_TESTS4}
         otbDisparityMapTo3DFilterNew)

add_test(dmTvDisparityMapTo3DFilter ${DISPARITYMAP_TESTS4}
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
add_test(dmTuMultiDisparityMapTo3DFilterNew ${DISPARITYMAP_TESTS4}
         otbMultiDisparityMapTo3DFilterNew)

if(OTB_DATA_USE_LARGEINPUT)
add_test(dmTvMultiDisparityMapTo3DFilter ${DISPARITYMAP_TESTS4}
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
endif()

# -------            otb::BijectionCoherencyFilter   ----------
add_test(dmTuBijectionCoherencyFilterNew ${DISPARITYMAP_TESTS4}
         otbBijectionCoherencyFilterNew)

add_test(dmTvBijectionCoherencyFilter ${DISPARITYMAP_TESTS4}
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

# -------            otb::DisparityTranslateFilter   ----------
add_test(dmTuDisparityTranslateFilterNew ${DISPARITYMAP_TESTS4}
         otbDisparityTranslateFilterNew)

add_test(dmTvDisparityTranslateFilter ${DISPARITYMAP_TESTS4}
--compare-image ${EPSILON_6}
     ${BASELINE}/dmTvDisparityTranslateFilterOutput.tif
     ${TEMP}/dmTvDisparityTranslateFilterOutput.tif
         otbDisparityTranslateFilter
         ${INPUTDATA}/sensor_stereo_blockmatching_output.tif
         ${INPUTDATA}/sensor_stereo_rectif_inv_left.tif
         ${INPUTDATA}/sensor_stereo_rectif_right.tif
         ${EXAMPLEDATA}/sensor_stereo_left.tif
         ${TEMP}/dmTvDisparityTranslateFilterOutput.tif
         )

# -------            otb::Multi3DMapToDEMFilter   ----------
add_test(dmTuMulti3DMapTo3DEMFilterNew ${DISPARITYMAP_TESTS4}
         otbMulti3DMapToDEMFilterNew)

if(OTB_DATA_USE_LARGEINPUT)
add_test(dmTuMulti3DMapToDEMFilterMeanMultiThreadMultiStream ${DISPARITYMAP_TESTS4}
         otbMulti3DMapToDEMFilter
         ${BASELINE}/dmTvMultiDisparityMapTo3DFilterOutput.tif
         ${LARGEINPUT}/PLEIADES/tristereo_sample/mask_1.tif
         ${BASELINE}/dmTvMultiDisparityMapTo3DFilterOutput.tif
         ${LARGEINPUT}/PLEIADES/tristereo_sample/mask_1.tif
		 ${TEMP}/dmTuMulti3DMapToDEMFilterOutputMeanMultiThreadMultiStream.tif
		 2.5
		 2
		 8
		 4)
		
add_test(dmTuMulti3DMapToDEMFilterAccMultiThreadMultiStream ${DISPARITYMAP_TESTS4}
         otbMulti3DMapToDEMFilter
         ${BASELINE}/dmTvMultiDisparityMapTo3DFilterOutput.tif
         ${LARGEINPUT}/PLEIADES/tristereo_sample/mask_1.tif
         ${BASELINE}/dmTvMultiDisparityMapTo3DFilterOutput.tif
         ${LARGEINPUT}/PLEIADES/tristereo_sample/mask_1.tif
		 ${TEMP}/dmTuMulti3DMapToDEMFilterOutputAccMultiThreadMultiStream.tif
		 2.5
		 3
		 8
		 4		
)
endif()


add_test(dmTvMulti3DMapToDEMFilterStadiumMinROI ${DISPARITYMAP_TESTS4}
--compare-image ${EPSILON_6}
     ${BASELINE}/dmTvMulti3DMapToDEMFilterOutputStadiumMin.tif
     ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMinROI.tif
         otbMulti3DMapToDEMFilter
         ${INPUTDATA}/Stadium3DMap1.tif
         ${INPUTDATA}/Stadium3DMapMask1.tif
         ${INPUTDATA}/Stadium3DMap2.tif
         ${INPUTDATA}/Stadium3DMapMask2.tif
         ${INPUTDATA}/Stadium3DMap3.tif
         ${INPUTDATA}/Stadium3DMapMask3.tif
         ${INPUTDATA}/Stadium3DMap4.tif
         ${INPUTDATA}/Stadium3DMapMask4.tif
         ${INPUTDATA}/Stadium3DMap5.tif
         ${INPUTDATA}/Stadium3DMapMask5.tif
		 ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMinROI.tif
		 2.5
		 0
		 6
		 4
)



add_test(dmTuMulti3DMapToDEMFilterStadiumAcc ${DISPARITYMAP_TESTS4}
         otbMulti3DMapToDEMFilter
         ${INPUTDATA}/Stadium3DMap.tif
         ${INPUTDATA}/Stadium3DMapMask.tif
         ${INPUTDATA}/Stadium3DMap.tif
         ${INPUTDATA}/Stadium3DMapMask.tif
         ${TEMP}/dmTuMulti3DMapToDEMFilterOutputStadiumAcc.tif
		 2.5
		 3
		 1
		 1
)


add_test(dmTvMulti3DMapToDEMFilterStadiumMin ${DISPARITYMAP_TESTS4}
--compare-image ${EPSILON_6}
     ${BASELINE}/dmTvMulti3DMapToDEMFilterOutputStadiumMin.tif
     ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMin.tif
         otbMulti3DMapToDEMFilter
         ${INPUTDATA}/Stadium3DMap.tif
         ${INPUTDATA}/Stadium3DMapMask.tif
         ${INPUTDATA}/Stadium3DMapBis.tif
         ${INPUTDATA}/Stadium3DMapMask.tif
         ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMin.tif
		 2.5
		 0
		 1
		 1
)




add_test(dmTvMulti3DMapToDEMFilterStadiumMax ${DISPARITYMAP_TESTS4}
--compare-image ${EPSILON_6}
     ${BASELINE}/dmTvMulti3DMapToDEMFilterOutputStadiumMax.tif
     ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMax.tif
         otbMulti3DMapToDEMFilter
         ${INPUTDATA}/Stadium3DMap.tif
         ${INPUTDATA}/Stadium3DMapMask.tif
         ${INPUTDATA}/Stadium3DMapBis.tif
         ${INPUTDATA}/Stadium3DMapMask.tif
         ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMax.tif
		 2.5
		 1
		 1
		 1
)

add_test(dmTvMulti3DMapToDEMFilterStadiumMean ${DISPARITYMAP_TESTS4}
--compare-image ${EPSILON_6}
     ${BASELINE}/dmTvMulti3DMapToDEMFilterOutputStadiumMean.tif
     ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMean.tif
         otbMulti3DMapToDEMFilter
         ${INPUTDATA}/Stadium3DMap.tif
         ${INPUTDATA}/Stadium3DMapMask.tif
         ${INPUTDATA}/Stadium3DMapBis.tif
         ${INPUTDATA}/Stadium3DMapMask.tif
         ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMean.tif
		 2.5
		 2
		 1
		 1
)


add_test(dmTvMulti3DMapToDEMFilterStadiumMeanNoData ${DISPARITYMAP_TESTS4}
--compare-image ${EPSILON_6}
     ${BASELINE}/dmTvMulti3DMapToDEMFilterOutputStadiumMeanNoData.tif
     ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMeanNoData.tif
         otbMulti3DMapToDEMFilter
         ${INPUTDATA}/Stadium3DMap.tif
         ${INPUTDATA}/Stadium3DMapMask.tif
         ${INPUTDATA}/Stadium3DMapBis.tif
         ${INPUTDATA}/Stadium3DMapMask.tif
         ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMeanNoData.tif
		 0.7
		 2
		 1
		 1
)



add_test(dmTuMulti3DMapToDEMFilterStadiumMeanEPSGUTM31N ${DISPARITYMAP_TESTS4}
         otbMulti3DMapToDEMFilterEPSG
         ${INPUTDATA}/Stadium3DMap.tif
         ${INPUTDATA}/Stadium3DMapMask.tif
         ${INPUTDATA}/Stadium3DMapBis.tif
         ${INPUTDATA}/Stadium3DMapMask.tif
         ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMeanEPSGUTM31N.tif
		 2.5
		 2
		 1
		 1
		 0
		 0
		 373473
		 4826907
		 103
		 107
		 2.5
		 -2.5
		 32631
)

add_test(dmTvMulti3DMapToDEMFilterStadiumMeanEPSGWGS84 ${DISPARITYMAP_TESTS4}
--compare-image ${EPSILON_6}
     ${BASELINE}/dmTvMulti3DMapToDEMFilterOutputStadiumMean.tif
     ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMeanEPSGWGS84.tif
         otbMulti3DMapToDEMFilterEPSG
         ${INPUTDATA}/Stadium3DMap.tif
         ${INPUTDATA}/Stadium3DMapMask.tif
         ${INPUTDATA}/Stadium3DMapBis.tif
         ${INPUTDATA}/Stadium3DMapMask.tif
         ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMeanEPSGWGS84.tif
		 2.5
		 2
		 1
		 1
		 0
		 0
		 1.432817727834504
		 43.584395676937191
		 103
		 107
		 0.000031003154124
		 -0.000022457882103
		 4326
)

add_test(dmTvMulti3DMapToDEMFilterStadiumMeanMultiThreadMultiStream ${DISPARITYMAP_TESTS4}
--compare-image ${EPSILON_6}
     ${BASELINE}/dmTvMulti3DMapToDEMFilterOutputStadiumMean.tif
     ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMeanMultiThreadMultiStream.tif
         otbMulti3DMapToDEMFilter
         ${INPUTDATA}/Stadium3DMap.tif
         ${INPUTDATA}/Stadium3DMapMask.tif
         ${INPUTDATA}/Stadium3DMapBis.tif
         ${INPUTDATA}/Stadium3DMapMask.tif
         ${TEMP}/dmTvMulti3DMapToDEMFilterOutputStadiumMeanMultiThreadMultiStream.tif
		 2.5
		 2
		 6
		 4
)


add_test(dmTuMulti3DMapToDEMFilterStadiumMeanLarge ${DISPARITYMAP_TESTS4}
         otbMulti3DMapToDEMFilter
         ${INPUTDATA}/Stadium3DMap.tif
         ${INPUTDATA}/Stadium3DMapMask.tif
         ${INPUTDATA}/Stadium3DMapBis.tif
         ${INPUTDATA}/Stadium3DMapMask.tif
         ${TEMP}/dmTuMulti3DMapToDEMFilterOutputStadiumMeanLarge.tif
		 10
		 2
		 8
		 4
)


# -------       Fichiers sources CXX -----------------------------------
set(BasicDisparityMap_SRCS1
otbDisparityMapTests1.cxx
otbDisparityMapEstimationMethodNew.cxx
otbDisparityMapEstimationMethod.cxx
otbNCCRegistrationFilterNew.cxx
otbNCCRegistrationFilter.cxx
otbPointSetToDisplacementFieldGeneratorNew.cxx
otbNearestPointDisplacementFieldGeneratorNew.cxx
otbNearestPointDisplacementFieldGenerator.cxx
otbNNearestPointsLinearInterpolateDisplacementFieldGeneratorNew.cxx
otbNNearestPointsLinearInterpolateDisplacementFieldGenerator.cxx
otbBSplinesInterpolateDisplacementFieldGeneratorNew.cxx
otbBSplinesInterpolateDisplacementFieldGenerator.cxx
otbPointSetWithTransformToDisplacementFieldGeneratorNew.cxx
)
set(BasicDisparityMap_SRCS2
otbDisparityMapTests2.cxx
otbNearestTransformDisplacementFieldGeneratorNew.cxx
otbNearestTransformDisplacementFieldGenerator.cxx
otbNNearestTransformsLinearInterpolateDisplacementFieldGeneratorNew.cxx
otbNNearestTransformsLinearInterpolateDisplacementFieldGenerator.cxx
otbBSplinesInterpolateTransformDisplacementFieldGeneratorNew.cxx
otbBSplinesInterpolateTransformDisplacementFieldGenerator.cxx
)
set(BasicDisparityMap_SRCS3
otbDisparityMapTests3.cxx
otbStreamingWarpImageFilterNew.cxx
otbStreamingWarpImageFilter.cxx
otbFineRegistrationImageFilterNew.cxx
otbFineRegistrationImageFilterTest.cxx
otbStereoSensorModelToElevationMapFilter.cxx
otbStereorectificationDisplacementFieldSource.cxx
)

set(BasicDisparityMap_SRCS4
otbDisparityMapTests4.cxx
otbAdhesionCorrectionFilter.cxx
otbDisparityMapMedianFilter.cxx
otbDisparityMapToDEMFilter.cxx
otbPixelWiseBlockMatchingImageFilter.cxx
otbSubPixelDisparityImageFilter.cxx
otbDisparityMapTo3DFilter.cxx
otbMultiDisparityMapTo3DFilter.cxx
otbMulti3DMapToDEMFilter.cxx
otbBijectionCoherencyFilter.cxx
otbDisparityTranslateFilter.cxx
)

OTB_ADD_EXECUTABLE(otbDisparityMapTests1 "${BasicDisparityMap_SRCS1}" "OTBDisparityMap;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbDisparityMapTests2 "${BasicDisparityMap_SRCS2}" "OTBDisparityMap;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbDisparityMapTests3 "${BasicDisparityMap_SRCS3}" "OTBDisparityMap;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbDisparityMapTests4 "${BasicDisparityMap_SRCS4}" "OTBDisparityMap;OTBIO;OTBTesting")


endif()
