project(FeatureExtractionExamples)
include_regular_expression("^.*$")

add_executable(AlignmentsExample AlignmentsExample.cxx )
target_link_libraries(AlignmentsExample OTBFeatureExtraction OTBCommon OTBIO
${ITK_LIBRARIES})

add_executable(TouziEdgeDetectorExample TouziEdgeDetectorExample.cxx )
target_link_libraries(TouziEdgeDetectorExample OTBFeatureExtraction OTBCommon OTBIO)

add_executable(HarrisExample HarrisExample.cxx )
target_link_libraries(HarrisExample OTBFeatureExtraction OTBCommon OTBIO)

add_executable(ComplexMomentsImageFunctionExample ComplexMomentsImageFunctionExample.cxx )
target_link_libraries(ComplexMomentsImageFunctionExample OTBFeatureExtraction OTBCommon OTBIO)

add_executable(ComplexMomentPathExample ComplexMomentPathExample.cxx )
target_link_libraries(ComplexMomentPathExample OTBFeatureExtraction OTBCommon OTBIO)

add_executable(HuMomentsImageFunctionExample HuMomentsImageFunctionExample.cxx )
target_link_libraries(HuMomentsImageFunctionExample OTBFeatureExtraction OTBCommon OTBIO)

add_executable(FlusserMomentsImageFunctionExample FlusserMomentsImageFunctionExample.cxx )
target_link_libraries(FlusserMomentsImageFunctionExample OTBFeatureExtraction OTBCommon OTBIO)

add_executable(RatioLineDetectorExample RatioLineDetectorExample.cxx )
target_link_libraries(RatioLineDetectorExample OTBFeatureExtraction OTBCommon OTBIO)

add_executable(CorrelationLineDetectorExample CorrelationLineDetectorExample.cxx )
target_link_libraries(CorrelationLineDetectorExample OTBFeatureExtraction OTBCommon OTBIO)

add_executable(AssymmetricFusionOfLineDetectorExample
AssymmetricFusionOfLineDetectorExample.cxx)
target_link_libraries(AssymmetricFusionOfLineDetectorExample OTBFeatureExtraction OTBCommon OTBIO)

add_executable(LocalHoughExample LocalHoughExample.cxx )
target_link_libraries(LocalHoughExample OTBIO OTBFeatureExtraction OTBCommon)

add_executable(ExtractSegmentsByStepsExample
ExtractSegmentsByStepsExample.cxx)
target_link_libraries(ExtractSegmentsByStepsExample OTBFeatureExtraction OTBCommon OTBIO)

if(OTB_USE_MAPNIK)
add_executable(LineSegmentDetectorExample LineSegmentDetectorExample.cxx)
target_link_libraries(LineSegmentDetectorExample OTBFeatureExtraction OTBCommon OTBIO)

add_executable(RightAngleDetectionExample RightAngleDetectionExample.cxx)
target_link_libraries(RightAngleDetectionExample OTBFeatureExtraction OTBCommon OTBIO)
endif(OTB_USE_MAPNIK)

add_executable(ExtractSegmentsExample
ExtractSegmentsExample.cxx)
target_link_libraries(ExtractSegmentsExample OTBFeatureExtraction OTBCommon OTBIO)

add_executable(ExtractRoadByStepsExample ExtractRoadByStepsExample.cxx)
target_link_libraries(ExtractRoadByStepsExample OTBIO OTBCommon OTBFeatureExtraction
${ITK_LIBRARIES}  )

add_executable(ExtractRoadExample ExtractRoadExample.cxx)
target_link_libraries(ExtractRoadExample OTBIO OTBCommon OTBFeatureExtraction
${ITK_LIBRARIES}  )

add_executable(SeamCarvingExample SeamCarvingExample.cxx)
target_link_libraries(SeamCarvingExample OTBIO OTBCommon OTBFeatureExtraction
${ITK_LIBRARIES}  )

add_executable(SeamCarvingOtherExample SeamCarvingOtherExample.cxx)
target_link_libraries(SeamCarvingOtherExample OTBIO OTBCommon OTBFeatureExtraction
${ITK_LIBRARIES}  )

add_executable(ThresholdToPointSetExample ThresholdToPointSetExample.cxx)
target_link_libraries(ThresholdToPointSetExample OTBIO OTBCommon OTBFeatureExtraction
${ITK_LIBRARIES}  )


add_executable(SURFExample SURFExample.cxx)
target_link_libraries(SURFExample OTBIO OTBCommon OTBFeatureExtraction
${ITK_LIBRARIES}  )


add_executable(ParallelLineDetectionExample ParallelLineDetectionExample.cxx)
target_link_libraries(ParallelLineDetectionExample OTBIO OTBCommon OTBFeatureExtraction
${ITK_LIBRARIES}  )

add_executable(CloudDetectionExample CloudDetectionExample.cxx)
target_link_libraries(CloudDetectionExample OTBIO OTBCommon OTBFeatureExtraction ${ITK_LIBRARIES}  )

add_executable(TextureExample TextureExample.cxx)
target_link_libraries(TextureExample OTBIO OTBCommon OTBFeatureExtraction ${ITK_LIBRARIES}  )

add_executable(PanTexExample PanTexExample.cxx)
target_link_libraries(PanTexExample OTBIO OTBCommon OTBFeatureExtraction ${ITK_LIBRARIES}  )

add_executable(SFSExample SFSExample.cxx)
target_link_libraries(SFSExample OTBIO OTBCommon OTBFeatureExtraction ${ITK_LIBRARIES}  )


add_executable(EdgeDensityExample EdgeDensityExample.cxx)
target_link_libraries(EdgeDensityExample OTBIO OTBCommon OTBBasicFilters ${ITK_LIBRARIES}  )


if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/FeatureExtraction)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)
set(INPUTDATA_OTB ${OTB_DATA_ROOT}/Input)
set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS1 ${CXX_TEST_PATH}/otbFeatureExtractionExamplesTests1)
set(EXE_TESTS2 ${CXX_TEST_PATH}/otbFeatureExtractionExamplesTests2)

set(TOL 0.0)


# ------- CorrelationLineDetectorExampleTest----------

add_test(feTeCorrelationLineDetectorExampleTest ${EXE_TESTS1}
        --compare-n-images ${TOL} 2
        ${BASELINE}/amstLineCorrelations.png
        ${TEMP}/amstLineCorrelations.png
        ${BASELINE}/amstLineCorrelationDirections.png
        ${TEMP}/amstLineCorrelationDirections.png
        CorrelationLineDetectorExampleTest
        ${INPUTDATA}/amst.png
        ${TEMP}/amstLineCorrelations.png
        ${TEMP}/amstLineCorrelationDirections.png
    5 1
)

# ------- LocalHoughExampleTest----------

add_test(feTeLocalHoughExampleTest ${EXE_TESTS1}
        --compare-n-images ${TOL} 1
        ${BASELINE}/detected_local_hough.png
        ${TEMP}/detected_local_hough.png
        LocalHoughExampleTest
        ${INPUTDATA}/detected_lines.png
        ${TEMP}/detected_local_hough.png
  30 10 1 50
)

# ------- HarrisExampleTest----------

add_test(feTeHarrisExampleTest ${EXE_TESTS1}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ROISpot5Harris.png
        ${TEMP}/ROISpot5Harris.png
        HarrisExampleTest
        ${INPUTDATA}/ROISpot5.png
        ${TEMP}/ROISpot5Harris.png
    1.5 2 0.1
)

# ------- TouziEdgeDetectorExampleTest----------

add_test(feTeTouziEdgeDetectorExampleTest ${EXE_TESTS1}
        --compare-n-images ${TOL} 2
        ${BASELINE}/amstTouziEdges.png
        ${TEMP}/amstTouziEdges.png
        ${BASELINE}/amstTouziDirections.png
        ${TEMP}/amstTouziDirections.png
        TouziEdgeDetectorExampleTest
        ${INPUTDATA}/amst.png
        ${TEMP}/amstTouziEdges.png
        ${TEMP}/amstTouziDirections.png
    3
)

# ------- ExtractSegmentsByStepsExampleTest----------

add_test(feTeExtractSegmentsByStepsExampleTest ${EXE_TESTS1}
        --compare-n-images ${TOL} 1
        ${BASELINE}/amstSegmentExtractionBySteps.png
        ${TEMP}/amstSegmentExtractionBySteps.png
        ExtractSegmentsByStepsExampleTest
        ${INPUTDATA_OTB}/amst_ers1.ima.extrait.419_187_70_66.hdr
        ${TEMP}/amstSegmentExtractionBySteps.png
  10 3 10 0.5 10 10 3 10 0.5
)

# ------- AssymmetricFusionOfLineDetectorExampleTest----------

add_test(feTeAssymmetricFusionOfLineDetectorExampleTest ${EXE_TESTS1}
        --compare-n-images ${TOL} 1
        ${BASELINE}/amstLineFusion.png
        ${TEMP}/amstLineFusion.png
        AssymmetricFusionOfLineDetectorExampleTest
        ${INPUTDATA}/amst.png
        ${TEMP}/amstLineFusion.png
    5 1
)

# ------- ExtractSegmentsExampleTest----------

add_test(feTeExtractSegmentsExampleTest ${EXE_TESTS1}
        --compare-n-images ${TOL} 1
        ${BASELINE}/amstSegmentExtraction.png
        ${TEMP}/amstSegmentExtraction.png
        ExtractSegmentsExampleTest
        ${INPUTDATA_OTB}/amst_ers1.ima.extrait.419_187_70_66.hdr
        ${TEMP}/amstSegmentExtraction.png
  10 3 10 0.5 10 10 3 10 0.5
)

# ------- RatioLineDetectorExampleTest----------

add_test(feTeRatioLineDetectorExampleTest ${EXE_TESTS1}
        --compare-n-images ${TOL} 2
        ${BASELINE}/amstLineRatios.png
        ${TEMP}/amstLineRatios.png
        ${BASELINE}/amstLineRatioDirections.png
        ${TEMP}/amstLineRatioDirections.png
        RatioLineDetectorExampleTest
        ${INPUTDATA}/amst.png
        ${TEMP}/amstLineRatios.png
        ${TEMP}/amstLineRatioDirections.png
    5 1
)

# ------- AlignmentsExampleTest----------

add_test(feTeAlignmentsExampleTest ${EXE_TESTS1}
        --compare-n-images ${TOL} 1
        ${BASELINE}/QB_SuburbAlign.png
        ${TEMP}/QB_SuburbAlign.png
        AlignmentsExampleTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/QB_SuburbAlign.png
        5
)

# ------- RoadExtractionExamplesTest----------


add_test(feTeExtractRoadByStepsExampleTest ${EXE_TESTS2}
        --compare-n-images ${TOL} 2
        ${BASELINE}/qb_ExtractRoad_pretty.png
        ${TEMP}/qb_ExtractRoad_pretty.png
        ${BASELINE}/ExtractRoadByStepsExampleOutput.png
        ${TEMP}/ExtractRoadByStepsExampleOutput.png
        ExtractRoadByStepsExampleTest
        ${INPUTDATA}/qb_RoadExtract.tif
        ${TEMP}/ExtractRoadByStepsExampleOutput.png
        ${TEMP}/qb_ExtractRoad_pretty.png
        337 557 432 859 0.00005 1.0
)


add_test(feTeExtractRoadExampleTest ${EXE_TESTS2}
        --compare-image ${TOL}
        ${BASELINE}/ExtractRoadExampleOutput.png
        ${TEMP}/ExtractRoadExampleOutput.png
        ExtractRoadExampleTest
        ${INPUTDATA}/qb_RoadExtract.tif
        ${TEMP}/ExtractRoadExampleOutput.png
        337 557 432 859 1.0 0.00005 1.0 0.39269 1.0 10.0 25.
)

# ------- SeamCarvingExamplesTest----------

add_test(feTeSeamCarvingExampleTest ${EXE_TESTS2}
        --compare-image ${TOL}
        ${BASELINE}/SeamCarvingExampleOutput.png
        ${TEMP}/SeamCarvingExampleOutput.png
        SeamCarvingExampleTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/SeamCarvingExampleOutput.png
        50
)

add_test(feTeSeamCarvingOtherExampleTest ${EXE_TESTS2}
        --compare-image ${TOL}
        ${BASELINE}/SeamCarvingOtherExampleOutput.png
        ${TEMP}/SeamCarvingOtherExampleOutput.png
        SeamCarvingOtherExampleTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/SeamCarvingOtherExampleOutput.png
        50
)

#add_test(feTeImageToSIFTKeyPointSetFilterTest ${EXE_TESTS}
#        --compare-image ${TOL}
#        ${BASELINE}/ImageToSIFTKeyPointSetFilterOutput.png
#        ${TEMP}/ImageToSIFTKeyPointSetFilterOutput.png
#        ImageToSIFTKeyPointSetFilterTest
#        ${INPUTDATA}/Scene.png
#        ${TEMP}/ImageToSIFTKeyPointSetFilterOutput.png
#        6 3 0.08 10
#)


# ------- CloudDetectionExampleTest----------

add_test(feTeCloudDetectionExampleTest ${EXE_TESTS2}
        --compare-image ${TOL}
        ${BASELINE}/CloudDetectionOutput.png
        ${TEMP}/PrettyCloudDetectionOutput.png
        CloudDetectionExampleTest
        ${INPUTDATA}/CloudsOnReunion.tif
        ${TEMP}/CloudDetectionOutput.tif
        ${TEMP}/PrettyCloudDetectionInput.png
        ${TEMP}/PrettyCloudDetectionOutput.png
        553 467 734 581 0.4 0.6 1.0
)

if(OTB_USE_MAPNIK)

# ------- Line Segment Detector test----------

add_test(feTeLineSegmentDetectorExampleTest  ${EXE_TESTS2}
        --compare-image ${TOL}
        ${BASELINE}/feTeLineSegmentDetectorExampleOutput.png
        ${TEMP}/feTeLineSegmentDetectorExampleOutput.png
       LineSegmentDetectorExampleTest
        ${INPUTDATA}/Scene.png
        ${TEMP}/feTeLineSegmentDetectorExampleOutput.png
)

# ------- Right Angle Detection test----------

add_test(feTeRightAngleDetectionExample  ${EXE_TESTS2}
        --compare-image ${TOL}
        ${BASELINE}/feTeRightAngleDetectionExampleOutput.png
        ${TEMP}/feTeRightAngleDetectionExampleOutput.png
       RightAngleDetectionExample
        ${INPUTDATA}/Scene.png
        ${TEMP}/feTeRightAngleDetectionExampleOutput.png
        ${TEMP}/PrettyRightAngleInput.png
        ${TEMP}/PrettyRightAngleOutput.png
        0.1
        10.
)

endif(OTB_USE_MAPNIK)

include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbFeatureExtractionExamplesTests1 otbFeatureExtractionExamplesTests1.cxx)
target_link_libraries(otbFeatureExtractionExamplesTests1   ${ITK_LIBRARIES} OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBTesting)

add_executable(otbFeatureExtractionExamplesTests2 otbFeatureExtractionExamplesTests2.cxx)
target_link_libraries(otbFeatureExtractionExamplesTests2   ${ITK_LIBRARIES} OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBTesting)


endif( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
