#
# Examples on the use of segmentation algorithms
#

project(ImageSegmentationExamples)
include_regular_expression("^.*$")

add_executable(ConnectedThresholdImageFilter ConnectedThresholdImageFilter.cxx )
target_link_libraries(ConnectedThresholdImageFilter OTBCommon OTBIO )

add_executable(OtsuThresholdImageFilter OtsuThresholdImageFilter.cxx )
target_link_libraries(OtsuThresholdImageFilter OTBCommon OTBIO)

add_executable(OtsuMultipleThresholdImageFilter OtsuMultipleThresholdImageFilter.cxx )
target_link_libraries(OtsuMultipleThresholdImageFilter
                      OTBCommon OTBIO ${ITK_LIBRARIES})

add_executable(ConfidenceConnected ConfidenceConnected.cxx )
target_link_libraries(ConfidenceConnected OTBCommon OTBIO )

add_executable(VectorConfidenceConnected VectorConfidenceConnected.cxx )
target_link_libraries(VectorConfidenceConnected OTBCommon OTBIO )

add_executable(NeighborhoodConnectedImageFilter NeighborhoodConnectedImageFilter.cxx )
target_link_libraries(NeighborhoodConnectedImageFilter OTBCommon OTBIO
)

add_executable(IsolatedConnectedImageFilter IsolatedConnectedImageFilter.cxx )
target_link_libraries(IsolatedConnectedImageFilter OTBCommon OTBIO )

#add_executable(ConnectedThresholdImageFilter ConnectedThresholdImageFilter.cxx )
#target_link_libraries(ConnectedThresholdImageFilter )

add_executable(FastMarchingImageFilter FastMarchingImageFilter.cxx )
target_link_libraries(FastMarchingImageFilter OTBCommon OTBIO )

#add_executable(GeodesicActiveContourShapePriorLevelSetImageFilter GeodesicActiveContourShapePriorLevelSetImageFilter.cxx )
#target_link_libraries(GeodesicActiveContourShapePriorLevelSetImageFilter  ${ITK_LIBRARIES})

#add_executable(DeformableModel2 DeformableModel2.cxx )
#target_link_libraries(DeformableModel2  )

#add_executable(GibbsPriorImageFilter1 GibbsPriorImageFilter1.cxx )
#target_link_libraries(GibbsPriorImageFilter1  )

#add_executable(HoughTransform2DCirclesImageFilter HoughTransform2DCirclesImageFilter.cxx )
#target_link_libraries(HoughTransform2DCirclesImageFilter  )

#add_executable(HoughTransform2DLinesImageFilter HoughTransform2DLinesImageFilter.cxx )
#target_link_libraries(HoughTransform2DLinesImageFilter  )

#add_executable(VectorConfidenceConnected VectorConfidenceConnected.cxx )
#target_link_libraries(VectorConfidenceConnected )

#add_executable(RelabelComponentImageFilter RelabelComponentImageFilter.cxx )
#target_link_libraries(RelabelComponentImageFilter )


#add_executable(CannySegmentationLevelSetImageFilter CannySegmentationLevelSetImageFilter.cxx )
#target_link_libraries(CannySegmentationLevelSetImageFilter )

#add_executable(GeodesicActiveContourImageFilter GeodesicActiveContourImageFilter.cxx )
#target_link_libraries(GeodesicActiveContourImageFilter )

#add_executable(ShapeDetectionLevelSetFilter ShapeDetectionLevelSetFilter.cxx )
#target_link_libraries(ShapeDetectionLevelSetFilter )

#add_executable(SegmentationExamples2 SegmentationExamples2.cxx )
#target_link_libraries(SegmentationExamples2
#  ${ITK_LIBRARIES})

#add_executable(SegmentationExamples3 SegmentationExamples3.cxx )
#target_link_libraries(SegmentationExamples3
#  ${ITK_LIBRARIES})

#add_executable(CurvesLevelSetImageFilter CurvesLevelSetImageFilter.cxx )
#target_link_libraries(CurvesLevelSetImageFilter )

add_executable(WatershedSegmentation WatershedSegmentation.cxx )
target_link_libraries(WatershedSegmentation OTBCommon OTBIO ${ITK_LIBRARIES})


#add_executable(SegmentationExamples SegmentationExamples.cxx )
#target_link_libraries(SegmentationExamples
# )



#add_executable(SegmentationExamples5 SegmentationExamples5.cxx )
#target_link_libraries(SegmentationExamples5
#  ${ITK_LIBRARIES})


#add_executable(SegmentationExamples8 SegmentationExamples8.cxx )
#target_link_libraries(SegmentationExamples8
#  ${ITK_LIBRARIES})

#add_executable(SegmentationExamples9 SegmentationExamples9.cxx )
#target_link_libraries(SegmentationExamples9
#  ${ITK_LIBRARIES})

#add_executable(CellularSegmentation1 CellularSegmentation1.cxx )
#target_link_libraries(CellularSegmentation1  )

#add_executable(DeformableModel1 DeformableModel1.cxx )
#target_link_libraries(DeformableModel1  )

#add_executable(LaplacianSegmentationLevelSetImageFilter LaplacianSegmentationLevelSetImageFilter.cxx )
#target_link_libraries(LaplacianSegmentationLevelSetImageFilter )

#add_executable(ThresholdSegmentationLevelSetImageFilter ThresholdSegmentationLevelSetImageFilter.cxx )
#target_link_libraries(ThresholdSegmentationLevelSetImageFilter )

#add_executable(SegmentationExamples4 SegmentationExamples4.cxx )
#target_link_libraries(SegmentationExamples4   ${ITK_LIBRARIES})

#add_executable(SegmentationExamples6 SegmentationExamples6.cxx )
#target_link_libraries(SegmentationExamples6
#  ${ITK_LIBRARIES})



#add_executable(SegmentationExamples7 SegmentationExamples7.cxx )
#target_link_libraries(SegmentationExamples7
#  ${ITK_LIBRARIES})


add_executable(StreamingMeanShiftSegmentation StreamingMeanShiftSegmentation.cxx )
target_link_libraries(StreamingMeanShiftSegmentation OTBCommon OTBIO OTBSegmentation OTBOGRAdapters)
if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Segmentation)

set(INPUTDATA ${OTB_DATA_ROOT}/Examples)
set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS ${CXX_TEST_PATH}/otbSegmentationExamplesTests)

set(TOL 0.0)


# ------- IsolatedConnectedImageFilterTest----------

add_test(seTeIsolatedConnectedImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ConnectedThresholdOutput1.png
        ${TEMP}/ConnectedThresholdOutput1.png
        IsolatedConnectedImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/ConnectedThresholdOutput1.png
  110 38 50 100
)

# ------- IsolatedConnectedImageFilterTest----------

add_test(seTeIsolatedConnectedImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/IsolatedConnectedImageFilterOutput1.png
        ${TEMP}/IsolatedConnectedImageFilterOutput1.png
        IsolatedConnectedImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/IsolatedConnectedImageFilterOutput1.png
  61 140 150 63 43
)

# ------- FastMarchingImageFilterTest----------

add_test(seTeFastMarchingImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/FastMarchingImageFilterOutput5.png
        ${TEMP}/FastMarchingImageFilterOutput5.png
        FastMarchingImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/FastMarchingImageFilterOutput5.png
    91 176 0.5 -0.5 3.0 10000 10000
)

# ------- FastMarchingImageFilterTest----------

add_test(seTeFastMarchingImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/FastMarchingImageFilterOutput6.png
        ${TEMP}/FastMarchingImageFilterOutput6.png
        FastMarchingImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/FastMarchingImageFilterOutput6.png
    118 100 1.0 -0.5 3.0 10000 10000
)

# ------- FastMarchingImageFilterTest----------

add_test(seTeFastMarchingImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/FastMarchingImageFilterOutput7.png
        ${TEMP}/FastMarchingImageFilterOutput7.png
        FastMarchingImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/FastMarchingImageFilterOutput7.png
    145 21 0.5 -0.5 3.0 10000 10000
)

# ------- OtsuThresholdImageFilterTest----------

add_test(seTeOtsuThresholdImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/OtsuThresholdImageFilterOutput.png
        ${TEMP}/OtsuThresholdImageFilterOutput.png
        OtsuThresholdImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/OtsuThresholdImageFilterOutput.png
    255 0
)

# ------- ConfidenceConnectedTest----------

add_test(seTeConfidenceConnectedTest1 ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ConfidenceConnectedOutput1.png
        ${TEMP}/ConfidenceConnectedOutput1.png
        ConfidenceConnectedTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/ConfidenceConnectedOutput1.png
  110 38
)

# ------- ConfidenceConnectedTest----------

add_test(seTeConfidenceConnectedTest2 ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ConfidenceConnectedOutput2.png
        ${TEMP}/ConfidenceConnectedOutput2.png
        ConfidenceConnectedTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/ConfidenceConnectedOutput2.png
  118 100
)

# ------- ConfidenceConnectedTest----------

add_test(seTeConfidenceConnectedTest3 ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ConfidenceConnectedOutput3.png
        ${TEMP}/ConfidenceConnectedOutput3.png
        ConfidenceConnectedTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/ConfidenceConnectedOutput3.png
  169 146
)



# ------- VectorConfidenceConnectedTest----------

add_test(seTeVectorConfidenceConnectedTest1 ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/VectorConfidenceConnectedOutput1.png
        ${TEMP}/VectorConfidenceConnectedOutput1.png
        VectorConfidenceConnectedTest
        ${INPUTDATA}/maur_rgb.png
        ${TEMP}/VectorConfidenceConnectedOutput1.png
  3.5 # multiplier
  10 # iterations
  113 234 # seed index
)

add_test(seTeVectorConfidenceConnectedTest2 ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/VectorConfidenceConnectedOutput2.png
        ${TEMP}/VectorConfidenceConnectedOutput2.png
        VectorConfidenceConnectedTest
        ${INPUTDATA}/maur_rgb.png
        ${TEMP}/VectorConfidenceConnectedOutput2.png
  3.5 # multiplier
  10 # iterations
  216 30 # seed index
)

add_test(seTeVectorConfidenceConnectedTest3 ${EXE_TESTS}
    --compare-n-images ${TOL} 1
    ${BASELINE}/VectorConfidenceConnectedOutput3.png
    ${TEMP}/VectorConfidenceConnectedOutput3.png
    VectorConfidenceConnectedTest
    ${INPUTDATA}/maur_rgb.png
    ${TEMP}/VectorConfidenceConnectedOutput3.png
    3.5     # multiplier
    10      # iterations
    113 234 # seed 1
    57 217  # seed 2
    136 190 # seed 3
)

# ------- NeighborhoodConnectedImageFilterTest----------

add_test(seTeNeighborhoodConnectedImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/NeighborhoodConnectedThresholdOutput1.png
        ${TEMP}/NeighborhoodConnectedThresholdOutput1.png
        NeighborhoodConnectedImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/NeighborhoodConnectedThresholdOutput1.png
  110 38 50 100
)

# ------- NeighborhoodConnectedImageFilterTest----------

add_test(seTeNeighborhoodConnectedImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/NeighborhoodConnectedThresholdOutput2.png
        ${TEMP}/NeighborhoodConnectedThresholdOutput2.png
        NeighborhoodConnectedImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/NeighborhoodConnectedThresholdOutput2.png
  118 100 0 10
)

# ------- NeighborhoodConnectedImageFilterTest----------

add_test(seTeNeighborhoodConnectedImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/NeighborhoodConnectedThresholdOutput3.png
        ${TEMP}/NeighborhoodConnectedThresholdOutput3.png
        NeighborhoodConnectedImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/NeighborhoodConnectedThresholdOutput3.png
  169 146 220 255
)

# ------- WatershedSegmentationTest----------

#This first implementation of the WatershedSegmentationTest was commented because
#it is duplicated below with the same name but with different input parameters.
#Moreover, NO baseline file for this first implementation is available in the
#OTB-Data directory.
#add_test(seTeWatershedSegmentationTest ${EXE_TESTS}
#        --compare-n-images ${TOL} 1
#        ${BASELINE}/WatershedSegmentation1Output1.png
#        ${TEMP}/WatershedSegmentation1Output1.png
#        WatershedSegmentationTest
#        ${INPUTDATA}/ROI_QB_MUL_3.tif
#        ${TEMP}/WatershedSegmentation1Output1.png
#  2 10 0 0.05 1
#)

# ------- WatershedSegmentationTest----------

add_test(seTeWatershedSegmentationTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/WatershedSegmentation1Output2.png
        ${TEMP}/WatershedSegmentation1Output2.png
        WatershedSegmentationTest
        ${INPUTDATA}/ROI_QB_MUL_3.tif
        ${TEMP}/WatershedSegmentation1Output2.png
  2 10 0.001 0.15 0
)

# ------- ConnectedThresholdImageFilterTest----------

add_test(seTeConnectedThresholdImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ConnectedThresholdOutput1.png
        ${TEMP}/ConnectedThresholdOutput1.png
        ConnectedThresholdImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/ConnectedThresholdOutput1.png
  110 38 50 100
)

# ------- ConnectedThresholdImageFilterTest----------

add_test(seTeConnectedThresholdImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ConnectedThresholdOutput2.png
        ${TEMP}/ConnectedThresholdOutput2.png
        ConnectedThresholdImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/ConnectedThresholdOutput2.png
  118 100 0 10
)

# ------- ConnectedThresholdImageFilterTest----------

add_test(seTeConnectedThresholdImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ConnectedThresholdOutput3.png
        ${TEMP}/ConnectedThresholdOutput3.png
        ConnectedThresholdImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/ConnectedThresholdOutput3.png
  169 146 220 255
)

# ------- OtsuMultipleThresholdImageFilterTest----------

add_test(seTeOtsuMultipleThresholdImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 3
        ${BASELINE}/OtsuMultipleThresholdsOutput000.png
        ${TEMP}/OtsuMultipleThresholdsOutput000.png
        ${BASELINE}/OtsuMultipleThresholdsOutput001.png
        ${TEMP}/OtsuMultipleThresholdsOutput001.png
        ${BASELINE}/OtsuMultipleThresholdsOutput002.png
        ${TEMP}/OtsuMultipleThresholdsOutput002.png
        OtsuMultipleThresholdImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/OtsuMultipleThresholdsOutput000.png
        ${TEMP}/OtsuMultipleThresholdsOutput001.png
        ${TEMP}/OtsuMultipleThresholdsOutput002.png
)

# ------- LabelizeNeighborhoodConnectedImageFilterTest----------

add_test(seTeLabelizeNeighborhoodConnectedImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/LabelizeNeighborhoodConnectedImageFilterOutput.png
        ${TEMP}/LabelizeNeighborhoodConnectedImageFilterOutput.png
        LabelizeNeighborhoodConnectedImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/LabelizeNeighborhoodConnectedImageFilterOutput.png
        170 170
        80 80
)


# ------- StreamingMeanShiftSegmentationTest ----------

add_test(seTeStreamingMeanShiftSegmentationTest ${EXE_TESTS}
       # --compare-ogr ${NOTOL}
       # ${BASELINE_FILES}/seTeStreamingMeanShiftSegmentationTest.shp
       # ${TEMP}/seTeStreamingMeanShiftSegmentationTest.shp
        StreamingMeanShiftSegmentationTest
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN_Mask.tif
        ${TEMP}/seTeStreamingMeanShiftSegmentationTest.shp
        NewLayer
        100
        5
        15.
        100
        1 #filter small object
        50 #minimum size of object
        1 #Simplify Flag
        0.2 #Simplification tolerance
)




include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbSegmentationExamplesTests otbSegmentationExamplesTests.cxx)
target_link_libraries(otbSegmentationExamplesTests
  ${ITK_LIBRARIES} OTBBasicFilters
  OTBCommon OTBDisparityMap OTBIO
  OTBSpatialReasoning OTBChangeDetection
  OTBFeatureExtraction  OTBLearning
  OTBMultiScale OTBTesting)

endif()
