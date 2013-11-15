#
# Examples on the use of segmentation algorithms
#

PROJECT(ImageSegmentationExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(ConnectedThresholdImageFilter ConnectedThresholdImageFilter.cxx )
TARGET_LINK_LIBRARIES(ConnectedThresholdImageFilter OTBCommon OTBIO )

ADD_EXECUTABLE(OtsuThresholdImageFilter OtsuThresholdImageFilter.cxx )
TARGET_LINK_LIBRARIES(OtsuThresholdImageFilter OTBCommon OTBIO)

ADD_EXECUTABLE(OtsuMultipleThresholdImageFilter OtsuMultipleThresholdImageFilter.cxx )
TARGET_LINK_LIBRARIES(OtsuMultipleThresholdImageFilter
                      OTBCommon OTBIO ${ITK_LIBRARIES})

ADD_EXECUTABLE(ConfidenceConnected ConfidenceConnected.cxx )
TARGET_LINK_LIBRARIES(ConfidenceConnected OTBCommon OTBIO )

ADD_EXECUTABLE(VectorConfidenceConnected VectorConfidenceConnected.cxx )
TARGET_LINK_LIBRARIES(VectorConfidenceConnected OTBCommon OTBIO )

ADD_EXECUTABLE(NeighborhoodConnectedImageFilter NeighborhoodConnectedImageFilter.cxx )
TARGET_LINK_LIBRARIES(NeighborhoodConnectedImageFilter OTBCommon OTBIO
)

ADD_EXECUTABLE(IsolatedConnectedImageFilter IsolatedConnectedImageFilter.cxx )
TARGET_LINK_LIBRARIES(IsolatedConnectedImageFilter OTBCommon OTBIO )

#ADD_EXECUTABLE(ConnectedThresholdImageFilter ConnectedThresholdImageFilter.cxx )
#TARGET_LINK_LIBRARIES(ConnectedThresholdImageFilter )

ADD_EXECUTABLE(FastMarchingImageFilter FastMarchingImageFilter.cxx )
TARGET_LINK_LIBRARIES(FastMarchingImageFilter OTBCommon OTBIO )

#ADD_EXECUTABLE(GeodesicActiveContourShapePriorLevelSetImageFilter GeodesicActiveContourShapePriorLevelSetImageFilter.cxx )
#TARGET_LINK_LIBRARIES(GeodesicActiveContourShapePriorLevelSetImageFilter  ${ITK_LIBRARIES})

#ADD_EXECUTABLE(DeformableModel2 DeformableModel2.cxx )
#TARGET_LINK_LIBRARIES(DeformableModel2  )

#ADD_EXECUTABLE(GibbsPriorImageFilter1 GibbsPriorImageFilter1.cxx )
#TARGET_LINK_LIBRARIES(GibbsPriorImageFilter1  )

#ADD_EXECUTABLE(HoughTransform2DCirclesImageFilter HoughTransform2DCirclesImageFilter.cxx )
#TARGET_LINK_LIBRARIES(HoughTransform2DCirclesImageFilter  )

#ADD_EXECUTABLE(HoughTransform2DLinesImageFilter HoughTransform2DLinesImageFilter.cxx )
#TARGET_LINK_LIBRARIES(HoughTransform2DLinesImageFilter  )

#ADD_EXECUTABLE(VectorConfidenceConnected VectorConfidenceConnected.cxx )
#TARGET_LINK_LIBRARIES(VectorConfidenceConnected )

#ADD_EXECUTABLE(RelabelComponentImageFilter RelabelComponentImageFilter.cxx )
#TARGET_LINK_LIBRARIES(RelabelComponentImageFilter )


#ADD_EXECUTABLE(CannySegmentationLevelSetImageFilter CannySegmentationLevelSetImageFilter.cxx )
#TARGET_LINK_LIBRARIES(CannySegmentationLevelSetImageFilter )

#ADD_EXECUTABLE(GeodesicActiveContourImageFilter GeodesicActiveContourImageFilter.cxx )
#TARGET_LINK_LIBRARIES(GeodesicActiveContourImageFilter )

#ADD_EXECUTABLE(ShapeDetectionLevelSetFilter ShapeDetectionLevelSetFilter.cxx )
#TARGET_LINK_LIBRARIES(ShapeDetectionLevelSetFilter )

#ADD_EXECUTABLE(SegmentationExamples2 SegmentationExamples2.cxx )
#TARGET_LINK_LIBRARIES(SegmentationExamples2 
#  ${ITK_LIBRARIES})

#ADD_EXECUTABLE(SegmentationExamples3 SegmentationExamples3.cxx )
#TARGET_LINK_LIBRARIES(SegmentationExamples3 
#  ${ITK_LIBRARIES})

#ADD_EXECUTABLE(CurvesLevelSetImageFilter CurvesLevelSetImageFilter.cxx )
#TARGET_LINK_LIBRARIES(CurvesLevelSetImageFilter )

ADD_EXECUTABLE(WatershedSegmentation WatershedSegmentation.cxx )
TARGET_LINK_LIBRARIES(WatershedSegmentation OTBCommon OTBIO ${ITK_LIBRARIES})


#ADD_EXECUTABLE(SegmentationExamples SegmentationExamples.cxx )
#TARGET_LINK_LIBRARIES(SegmentationExamples 
# )



#ADD_EXECUTABLE(SegmentationExamples5 SegmentationExamples5.cxx )
#TARGET_LINK_LIBRARIES(SegmentationExamples5 
#  ${ITK_LIBRARIES})


#ADD_EXECUTABLE(SegmentationExamples8 SegmentationExamples8.cxx )
#TARGET_LINK_LIBRARIES(SegmentationExamples8 
#  ${ITK_LIBRARIES})

#ADD_EXECUTABLE(SegmentationExamples9 SegmentationExamples9.cxx )
#TARGET_LINK_LIBRARIES(SegmentationExamples9 
#  ${ITK_LIBRARIES})

#ADD_EXECUTABLE(CellularSegmentation1 CellularSegmentation1.cxx )
#TARGET_LINK_LIBRARIES(CellularSegmentation1  )

#ADD_EXECUTABLE(DeformableModel1 DeformableModel1.cxx )
#TARGET_LINK_LIBRARIES(DeformableModel1  )

#ADD_EXECUTABLE(LaplacianSegmentationLevelSetImageFilter LaplacianSegmentationLevelSetImageFilter.cxx )
#TARGET_LINK_LIBRARIES(LaplacianSegmentationLevelSetImageFilter )

#ADD_EXECUTABLE(ThresholdSegmentationLevelSetImageFilter ThresholdSegmentationLevelSetImageFilter.cxx )
#TARGET_LINK_LIBRARIES(ThresholdSegmentationLevelSetImageFilter )

#ADD_EXECUTABLE(SegmentationExamples4 SegmentationExamples4.cxx )
#TARGET_LINK_LIBRARIES(SegmentationExamples4   ${ITK_LIBRARIES})

#ADD_EXECUTABLE(SegmentationExamples6 SegmentationExamples6.cxx )
#TARGET_LINK_LIBRARIES(SegmentationExamples6 
#  ${ITK_LIBRARIES})



#ADD_EXECUTABLE(SegmentationExamples7 SegmentationExamples7.cxx )
#TARGET_LINK_LIBRARIES(SegmentationExamples7 
#  ${ITK_LIBRARIES})


ADD_EXECUTABLE(StreamingMeanShiftSegmentation StreamingMeanShiftSegmentation.cxx )
TARGET_LINK_LIBRARIES(StreamingMeanShiftSegmentation OTBCommon OTBIO OTBSegmentation OTBOGRAdapters)
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Segmentation)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbSegmentationExamplesTests)

SET(TOL 0.0)


# ------- IsolatedConnectedImageFilterTest----------

ADD_TEST(seTeIsolatedConnectedImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ConnectedThresholdOutput1.png
        ${TEMP}/ConnectedThresholdOutput1.png
        IsolatedConnectedImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/ConnectedThresholdOutput1.png
  110 38 50 100
)

# ------- IsolatedConnectedImageFilterTest----------

ADD_TEST(seTeIsolatedConnectedImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/IsolatedConnectedImageFilterOutput1.png
        ${TEMP}/IsolatedConnectedImageFilterOutput1.png
        IsolatedConnectedImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/IsolatedConnectedImageFilterOutput1.png
  61 140 150 63 43
)

# ------- FastMarchingImageFilterTest----------

ADD_TEST(seTeFastMarchingImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/FastMarchingImageFilterOutput5.png
        ${TEMP}/FastMarchingImageFilterOutput5.png
        FastMarchingImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/FastMarchingImageFilterOutput5.png
    91 176 0.5 -0.5 3.0 10000 10000
)

# ------- FastMarchingImageFilterTest----------

ADD_TEST(seTeFastMarchingImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/FastMarchingImageFilterOutput6.png
        ${TEMP}/FastMarchingImageFilterOutput6.png
        FastMarchingImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/FastMarchingImageFilterOutput6.png
    118 100 1.0 -0.5 3.0 10000 10000
)

# ------- FastMarchingImageFilterTest----------

ADD_TEST(seTeFastMarchingImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/FastMarchingImageFilterOutput7.png
        ${TEMP}/FastMarchingImageFilterOutput7.png
        FastMarchingImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/FastMarchingImageFilterOutput7.png
    145 21 0.5 -0.5 3.0 10000 10000
)

# ------- OtsuThresholdImageFilterTest----------

ADD_TEST(seTeOtsuThresholdImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/OtsuThresholdImageFilterOutput.png
        ${TEMP}/OtsuThresholdImageFilterOutput.png
        OtsuThresholdImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/OtsuThresholdImageFilterOutput.png
    255 0
)

# ------- ConfidenceConnectedTest----------

ADD_TEST(seTeConfidenceConnectedTest1 ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ConfidenceConnectedOutput1.png
        ${TEMP}/ConfidenceConnectedOutput1.png
        ConfidenceConnectedTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/ConfidenceConnectedOutput1.png
  110 38
)

# ------- ConfidenceConnectedTest----------

ADD_TEST(seTeConfidenceConnectedTest2 ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ConfidenceConnectedOutput2.png
        ${TEMP}/ConfidenceConnectedOutput2.png
        ConfidenceConnectedTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/ConfidenceConnectedOutput2.png
  118 100
)

# ------- ConfidenceConnectedTest----------

ADD_TEST(seTeConfidenceConnectedTest3 ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ConfidenceConnectedOutput3.png
        ${TEMP}/ConfidenceConnectedOutput3.png
        ConfidenceConnectedTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/ConfidenceConnectedOutput3.png
  169 146
)



# ------- VectorConfidenceConnectedTest----------

ADD_TEST(seTeVectorConfidenceConnectedTest1 ${EXE_TESTS}
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

ADD_TEST(seTeVectorConfidenceConnectedTest2 ${EXE_TESTS}
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

ADD_TEST(seTeVectorConfidenceConnectedTest3 ${EXE_TESTS}
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

ADD_TEST(seTeNeighborhoodConnectedImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/NeighborhoodConnectedThresholdOutput1.png
        ${TEMP}/NeighborhoodConnectedThresholdOutput1.png
        NeighborhoodConnectedImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/NeighborhoodConnectedThresholdOutput1.png
  110 38 50 100
)

# ------- NeighborhoodConnectedImageFilterTest----------

ADD_TEST(seTeNeighborhoodConnectedImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/NeighborhoodConnectedThresholdOutput2.png
        ${TEMP}/NeighborhoodConnectedThresholdOutput2.png
        NeighborhoodConnectedImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/NeighborhoodConnectedThresholdOutput2.png
  118 100 0 10
)

# ------- NeighborhoodConnectedImageFilterTest----------

ADD_TEST(seTeNeighborhoodConnectedImageFilterTest ${EXE_TESTS}
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
#ADD_TEST(seTeWatershedSegmentationTest ${EXE_TESTS}
#        --compare-n-images ${TOL} 1
#        ${BASELINE}/WatershedSegmentation1Output1.png
#        ${TEMP}/WatershedSegmentation1Output1.png
#        WatershedSegmentationTest
#        ${INPUTDATA}/ROI_QB_MUL_3.tif
#        ${TEMP}/WatershedSegmentation1Output1.png
#  2 10 0 0.05 1
#)

# ------- WatershedSegmentationTest----------

ADD_TEST(seTeWatershedSegmentationTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/WatershedSegmentation1Output2.png
        ${TEMP}/WatershedSegmentation1Output2.png
        WatershedSegmentationTest
        ${INPUTDATA}/ROI_QB_MUL_3.tif
        ${TEMP}/WatershedSegmentation1Output2.png
  2 10 0.001 0.15 0
)

# ------- ConnectedThresholdImageFilterTest----------

ADD_TEST(seTeConnectedThresholdImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ConnectedThresholdOutput1.png
        ${TEMP}/ConnectedThresholdOutput1.png
        ConnectedThresholdImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/ConnectedThresholdOutput1.png
  110 38 50 100
)

# ------- ConnectedThresholdImageFilterTest----------

ADD_TEST(seTeConnectedThresholdImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ConnectedThresholdOutput2.png
        ${TEMP}/ConnectedThresholdOutput2.png
        ConnectedThresholdImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/ConnectedThresholdOutput2.png
  118 100 0 10
)

# ------- ConnectedThresholdImageFilterTest----------

ADD_TEST(seTeConnectedThresholdImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/ConnectedThresholdOutput3.png
        ${TEMP}/ConnectedThresholdOutput3.png
        ConnectedThresholdImageFilterTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/ConnectedThresholdOutput3.png
  169 146 220 255
)

# ------- OtsuMultipleThresholdImageFilterTest----------

ADD_TEST(seTeOtsuMultipleThresholdImageFilterTest ${EXE_TESTS}
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

ADD_TEST(seTeLabelizeNeighborhoodConnectedImageFilterTest ${EXE_TESTS}
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

ADD_TEST(seTeStreamingMeanShiftSegmentationTest ${EXE_TESTS}
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




INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbSegmentationExamplesTests otbSegmentationExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbSegmentationExamplesTests   
  ${ITK_LIBRARIES} OTBBasicFilters 
  OTBCommon OTBDisparityMap OTBIO 
  OTBSpatialReasoning OTBChangeDetection 
  OTBFeatureExtraction  OTBLearning  
  OTBMultiScale OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
