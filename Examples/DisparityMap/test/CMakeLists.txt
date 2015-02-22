set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/DisparityMap)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)

# ------- SimpleDisparityMapEstimationExampleTest ----------

otb_add_test(NAME dmTeSimpleDisparityMapEstimationExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/deformationFieldOutput.png
    ${TEMP}/deformationFieldOutput.png
    ${BASELINE}/resampledMovingOutput.png
    ${TEMP}/resampledMovingOutput.png
  Execute $<TARGET_FILE:SimpleDisparityMapEstimationExample>
    ${INPUTDATA}/StereoFixed.png
    ${INPUTDATA}/StereoMoving.png
    ${TEMP}/deformationFieldOutput.png
    ${TEMP}/resampledMovingOutput.png
    100 10 15 10 10 250 0.9
)

otb_add_test(NAME dmTeStereoReconstructionExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/elevationOutput.tif
    ${TEMP}/elevationOutput.tif
  Execute $<TARGET_FILE:StereoReconstructionExample>
    ${INPUTDATA}/sensor_stereo_left.tif
    ${INPUTDATA}/sensor_stereo_right.tif
    ${TEMP}/elevationOutput.tif
    ${TEMP}/elevationOutputPrintable.png
    140
)
