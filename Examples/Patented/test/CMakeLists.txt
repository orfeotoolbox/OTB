set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Patented)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)

# -----EstimateAffineTransformationExampleTest---------
otb_add_test(NAME dmTeEstimateAffineTransformationExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/EstimateAffineTransformationOutput.png
    ${TEMP}/AffineTransformationOutput.png
  Execute $<TARGET_FILE:EstimateAffineTransformationExample>
    ${INPUTDATA}/QB_Suburb.png
    ${INPUTDATA}/QB_SuburbR10X13Y17.png
    ${TEMP}/AffineTransformationOutput.png
    ${TEMP}/AffineTransformationTxtOutput.txt
    2 3 0 0 0.5 1
)

# -----SIFTDisparityMapEstimationTest---------
if(Module_OTBSiftFast)

  otb_add_test(NAME dmTeSIFTDisparityMapEstimationTest COMMAND ${OTB_TEST_DRIVER}
    --compare-image ${NOTOL}
      ${BASELINE}/SIFTdeformationFieldOutput.png
      ${TEMP}/SIFTdeformationFieldOutput.png
    Execute $<TARGET_FILE:SIFTDisparityMapEstimation>
      ${INPUTDATA}/ROISpot5.png
      ${INPUTDATA}/ROISpot5Warped.png
      ${TEMP}/SIFTdeformationFieldOutput.png
  )

  otb_add_test(NAME feTeSIFTFastExampleTest COMMAND ${OTB_TEST_DRIVER}
    --compare-image ${NOTOL}
     ${BASELINE}/ROISpot5SiftFast.png
     ${TEMP}/ROISpot5SiftFast.png
    Execute $<TARGET_FILE:SIFTFastExample>
     ${INPUTDATA}/ROISpot5.png
     ${TEMP}/ROISpot5SiftFast.png
     3
  )

endif()
