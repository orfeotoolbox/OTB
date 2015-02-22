set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/FeatureExtraction)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)

# ------- CorrelationLineDetectorExampleTest----------

otb_add_test(NAME feTeCorrelationLineDetectorExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/amstLineCorrelations.png
    ${TEMP}/amstLineCorrelations.png
    ${BASELINE}/amstLineCorrelationDirections.png
    ${TEMP}/amstLineCorrelationDirections.png
  Execute $<TARGET_FILE:CorrelationLineDetectorExample>
    ${INPUTDATA}/amst.png
    ${TEMP}/amstLineCorrelations.png
    ${TEMP}/amstLineCorrelationDirections.png
    5 1
)

# ------- LocalHoughExampleTest----------

otb_add_test(NAME feTeLocalHoughExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/detected_local_hough.png
    ${TEMP}/detected_local_hough.png
  Execute $<TARGET_FILE:LocalHoughExample>
    ${INPUTDATA}/detected_lines.png
    ${TEMP}/detected_local_hough.png
    30 10 1 50
)

# ------- HarrisExampleTest----------

otb_add_test(NAME feTeHarrisExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/ROISpot5Harris.png
    ${TEMP}/ROISpot5Harris.png
  Execute $<TARGET_FILE:HarrisExample>
    ${INPUTDATA}/ROISpot5.png
    ${TEMP}/ROISpot5Harris.png
    1.5 2 0.1
)

# ------- TouziEdgeDetectorExampleTest----------

otb_add_test(NAME feTeTouziEdgeDetectorExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/amstTouziEdges.png
    ${TEMP}/amstTouziEdges.png
    ${BASELINE}/amstTouziDirections.png
    ${TEMP}/amstTouziDirections.png
  Execute $<TARGET_FILE:TouziEdgeDetectorExample>
    ${INPUTDATA}/amst.png
    ${TEMP}/amstTouziEdges.png
    ${TEMP}/amstTouziDirections.png
    3
)

# ------- ExtractSegmentsByStepsExampleTest----------

otb_add_test(NAME feTeExtractSegmentsByStepsExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/amstSegmentExtractionBySteps.png
    ${TEMP}/amstSegmentExtractionBySteps.png
  Execute $<TARGET_FILE:ExtractSegmentsByStepsExample>
    ${INPUTDATA_OTB}/amst_ers1.ima.extrait.419_187_70_66.hdr
    ${TEMP}/amstSegmentExtractionBySteps.png
    10 3 10 0.5 10 10 3 10 0.5
)

# ------- AssymmetricFusionOfLineDetectorExampleTest----------

otb_add_test(NAME feTeAssymmetricFusionOfLineDetectorExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/amstLineFusion.png
    ${TEMP}/amstLineFusion.png
  Execute $<TARGET_FILE:AssymmetricFusionOfLineDetectorExample>
    ${INPUTDATA}/amst.png
    ${TEMP}/amstLineFusion.png
    5 1
)

# ------- ExtractSegmentsExampleTest----------

otb_add_test(NAME feTeExtractSegmentsExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/amstSegmentExtraction.png
    ${TEMP}/amstSegmentExtraction.png
  Execute $<TARGET_FILE:ExtractSegmentsExample>
    ${INPUTDATA_OTB}/amst_ers1.ima.extrait.419_187_70_66.hdr
    ${TEMP}/amstSegmentExtraction.png
    10 3 10 0.5 10 10 3 10 0.5
)

# ------- RatioLineDetectorExampleTest----------

otb_add_test(NAME feTeRatioLineDetectorExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/amstLineRatios.png
    ${TEMP}/amstLineRatios.png
    ${BASELINE}/amstLineRatioDirections.png
    ${TEMP}/amstLineRatioDirections.png
  Execute $<TARGET_FILE:RatioLineDetectorExample>
    ${INPUTDATA}/amst.png
    ${TEMP}/amstLineRatios.png
    ${TEMP}/amstLineRatioDirections.png
    5 1
)

# ------- AlignmentsExampleTest----------

otb_add_test(NAME feTeAlignmentsExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/QB_SuburbAlign.png
    ${TEMP}/QB_SuburbAlign.png
  Execute $<TARGET_FILE:AlignmentsExample>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/QB_SuburbAlign.png
    5
)

# ------- RoadExtractionExamplesTest----------


otb_add_test(NAME feTeExtractRoadByStepsExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/qb_ExtractRoad_pretty.png
    ${TEMP}/qb_ExtractRoad_pretty.png
    ${BASELINE}/ExtractRoadByStepsExampleOutput.png
    ${TEMP}/ExtractRoadByStepsExampleOutput.png
  Execute $<TARGET_FILE:ExtractRoadByStepsExample>
    ${INPUTDATA}/qb_RoadExtract.tif
    ${TEMP}/ExtractRoadByStepsExampleOutput.png
    ${TEMP}/qb_ExtractRoad_pretty.png
    337 557 432 859 0.00005 1.0
)


otb_add_test(NAME feTeExtractRoadExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/ExtractRoadExampleOutput.png
    ${TEMP}/ExtractRoadExampleOutput.png
  Execute $<TARGET_FILE:ExtractRoadExample>
    ${INPUTDATA}/qb_RoadExtract.tif
    ${TEMP}/ExtractRoadExampleOutput.png
    337 557 432 859 1.0 0.00005 1.0 0.39269 1.0 10.0 25.
)

# ------- SeamCarvingExamplesTest----------

otb_add_test(NAME feTeSeamCarvingExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/SeamCarvingExampleOutput.png
    ${TEMP}/SeamCarvingExampleOutput.png
  SeamCarvingExampleTest
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/SeamCarvingExampleOutput.png
    50
)

otb_add_test(NAME feTeSeamCarvingOtherExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/SeamCarvingOtherExampleOutput.png
    ${TEMP}/SeamCarvingOtherExampleOutput.png
  Execute $<TARGET_FILE:SeamCarvingOtherExample>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/SeamCarvingOtherExampleOutput.png
    50
)

#otb_add_test(NAME feTeImageToSIFTKeyPointSetFilterTest COMMAND ${OTB_TEST_DRIVER}
#        --compare-image ${NOTOL}
#        ${BASELINE}/ImageToSIFTKeyPointSetFilterOutput.png
#        ${TEMP}/ImageToSIFTKeyPointSetFilterOutput.png
#        Execute $<TARGET_FILE:ImageToSIFTKeyPointSetFilter>
#        ${INPUTDATA}/Scene.png
#        ${TEMP}/ImageToSIFTKeyPointSetFilterOutput.png
#        6 3 0.08 10
#)


# ------- CloudDetectionExampleTest----------

otb_add_test(NAME feTeCloudDetectionExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/CloudDetectionOutput.png
    ${TEMP}/PrettyCloudDetectionOutput.png
  Execute $<TARGET_FILE:CloudDetectionExample>
    ${INPUTDATA}/CloudsOnReunion.tif
    ${TEMP}/CloudDetectionOutput.tif
    ${TEMP}/PrettyCloudDetectionInput.png
    ${TEMP}/PrettyCloudDetectionOutput.png
    553 467 734 581 0.4 0.6 1.0
)

if (Module_OTBVectorDataRendering)

  # ------- Line Segment Detector test----------
  otb_add_test(NAME feTeLineSegmentDetectorExampleTest  COMMAND ${OTB_TEST_DRIVER}
    --compare-image ${NOTOL}
      ${BASELINE}/feTeLineSegmentDetectorExampleOutput.png
      ${TEMP}/feTeLineSegmentDetectorExampleOutput.png
    Execute $<TARGET_FILE:LineSegmentDetectorExample>
      ${INPUTDATA}/Scene.png
      ${TEMP}/feTeLineSegmentDetectorExampleOutput.png
  )

  # ------- Right Angle Detection test----------

  otb_add_test(NAME feTeRightAngleDetectionExample  COMMAND ${OTB_TEST_DRIVER}
    --compare-image ${NOTOL}
      ${BASELINE}/feTeRightAngleDetectionExampleOutput.png
      ${TEMP}/feTeRightAngleDetectionExampleOutput.png
    Execute $<TARGET_FILE:RightAngleDetectionExample>
      ${INPUTDATA}/Scene.png
      ${TEMP}/feTeRightAngleDetectionExampleOutput.png
      ${TEMP}/PrettyRightAngleInput.png
      ${TEMP}/PrettyRightAngleOutput.png
      0.1
      10.
  )
endif()
