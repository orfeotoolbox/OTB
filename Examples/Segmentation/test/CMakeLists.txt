set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Segmentation)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)


# ------- IsolatedConnectedImageFilterTest----------

otb_add_test(NAME seTeIsolatedConnectedImageFilterTest2 COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/IsolatedConnectedImageFilterOutput1.png
    ${TEMP}/IsolatedConnectedImageFilterOutput1.png
  Execute $<TARGET_FILE:IsolatedConnectedImageFilter>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/IsolatedConnectedImageFilterOutput1.png
    61 140 150 63 43
)

# ------- FastMarchingImageFilterTest----------

otb_add_test(NAME seTeFastMarchingImageFilterTest1 COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/FastMarchingImageFilterOutput5.png
    ${TEMP}/FastMarchingImageFilterOutput5.png
  Execute $<TARGET_FILE:FastMarchingImageFilter>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/FastMarchingImageFilterOutput5.png
    91 176 0.5 -0.5 3.0 10000 10000
)

# ------- FastMarchingImageFilterTest----------

otb_add_test(NAME seTeFastMarchingImageFilterTest2 COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/FastMarchingImageFilterOutput6.png
    ${TEMP}/FastMarchingImageFilterOutput6.png
  Execute $<TARGET_FILE:FastMarchingImageFilter>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/FastMarchingImageFilterOutput6.png
    118 100 1.0 -0.5 3.0 10000 10000
)

# ------- FastMarchingImageFilterTest----------

otb_add_test(NAME seTeFastMarchingImageFilterTest3 COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/FastMarchingImageFilterOutput7.png
    ${TEMP}/FastMarchingImageFilterOutput7.png
  Execute $<TARGET_FILE:FastMarchingImageFilter>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/FastMarchingImageFilterOutput7.png
    145 21 0.5 -0.5 3.0 10000 10000
)

# ------- OtsuThresholdImageFilterTest----------

otb_add_test(NAME seTeOtsuThresholdImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/OtsuThresholdImageFilterOutput.png
    ${TEMP}/OtsuThresholdImageFilterOutput.png
  Execute $<TARGET_FILE:OtsuThresholdImageFilter>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/OtsuThresholdImageFilterOutput.png
    255 0
)

# ------- ConfidenceConnectedTest----------

otb_add_test(NAME seTeConfidenceConnectedTest1 COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/ConfidenceConnectedOutput1.png
    ${TEMP}/ConfidenceConnectedOutput1.png
  Execute $<TARGET_FILE:ConfidenceConnected>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/ConfidenceConnectedOutput1.png
    110 38
)

# ------- ConfidenceConnectedTest----------

otb_add_test(NAME seTeConfidenceConnectedTest2 COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/ConfidenceConnectedOutput2.png
    ${TEMP}/ConfidenceConnectedOutput2.png
  Execute $<TARGET_FILE:ConfidenceConnected>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/ConfidenceConnectedOutput2.png
    118 100
)

# ------- ConfidenceConnectedTest----------

otb_add_test(NAME seTeConfidenceConnectedTest3 COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/ConfidenceConnectedOutput3.png
    ${TEMP}/ConfidenceConnectedOutput3.png
  Execute $<TARGET_FILE:ConfidenceConnected>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/ConfidenceConnectedOutput3.png
    169 146
)



# ------- VectorConfidenceConnectedTest----------

otb_add_test(NAME seTeVectorConfidenceConnectedTest1 COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/VectorConfidenceConnectedOutput1.png
    ${TEMP}/VectorConfidenceConnectedOutput1.png
  Execute $<TARGET_FILE:VectorConfidenceConnected>
    ${INPUTDATA}/maur_rgb.png
    ${TEMP}/VectorConfidenceConnectedOutput1.png
    3.5 # multiplier
    10 # iterations
    113 234 # seed index
)

otb_add_test(NAME seTeVectorConfidenceConnectedTest2 COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/VectorConfidenceConnectedOutput2.png
    ${TEMP}/VectorConfidenceConnectedOutput2.png
  Execute $<TARGET_FILE:VectorConfidenceConnected>
    ${INPUTDATA}/maur_rgb.png
    ${TEMP}/VectorConfidenceConnectedOutput2.png
    3.5 # multiplier
    10 # iterations
    216 30 # seed index
)

otb_add_test(NAME seTeVectorConfidenceConnectedTest3 COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/VectorConfidenceConnectedOutput3.png
    ${TEMP}/VectorConfidenceConnectedOutput3.png
  Execute $<TARGET_FILE:VectorConfidenceConnected>
    ${INPUTDATA}/maur_rgb.png
    ${TEMP}/VectorConfidenceConnectedOutput3.png
    3.5     # multiplier
    10      # iterations
    113 234 # seed 1
    57 217  # seed 2
    136 190 # seed 3
)

# ------- NeighborhoodConnectedImageFilterTest----------

otb_add_test(NAME seTeNeighborhoodConnectedImageFilterTest1 COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/NeighborhoodConnectedThresholdOutput1.png
    ${TEMP}/NeighborhoodConnectedThresholdOutput1.png
  Execute $<TARGET_FILE:NeighborhoodConnectedImageFilter>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/NeighborhoodConnectedThresholdOutput1.png
    110 38 50 100
)

# ------- NeighborhoodConnectedImageFilterTest----------

otb_add_test(NAME seTeNeighborhoodConnectedImageFilterTest2 COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/NeighborhoodConnectedThresholdOutput2.png
    ${TEMP}/NeighborhoodConnectedThresholdOutput2.png
  Execute $<TARGET_FILE:NeighborhoodConnectedImageFilter>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/NeighborhoodConnectedThresholdOutput2.png
    118 100 0 10
)

# ------- NeighborhoodConnectedImageFilterTest----------

otb_add_test(NAME seTeNeighborhoodConnectedImageFilterTest3 COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/NeighborhoodConnectedThresholdOutput3.png
    ${TEMP}/NeighborhoodConnectedThresholdOutput3.png
  Execute $<TARGET_FILE:NeighborhoodConnectedImageFilter>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/NeighborhoodConnectedThresholdOutput3.png
    169 146 220 255
)

# ------- WatershedSegmentationTest----------

#This first implementation of the WatershedSegmentationTest was commented because
#it is duplicated below with the same name but with different input parameters.
#Moreover, NO baseline file for this first implementation is available in the
#OTB-Data directory.
#otb_add_test(NAME seTeWatershedSegmentationTest1 COMMAND ${OTB_TEST_DRIVER}
#        --compare-n-images ${NOTOL} 1
#        ${BASELINE}/WatershedSegmentation1Output1.png
#        ${TEMP}/WatershedSegmentation1Output1.png
#        Execute $<TARGET_FILE:WatershedSegmentation>
#        ${INPUTDATA}/ROI_QB_MUL_3.tif
#        ${TEMP}/WatershedSegmentation1Output1.png
#  2 10 0 0.05 1
#)

# ------- WatershedSegmentationTest----------

otb_add_test(NAME seTeWatershedSegmentationTest2 COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/WatershedSegmentation1Output2.png
    ${TEMP}/WatershedSegmentation1Output2.png
  Execute $<TARGET_FILE:WatershedSegmentation>
    ${INPUTDATA}/ROI_QB_MUL_3.tif
    ${TEMP}/WatershedSegmentation1Output2.png
    2 10 0.001 0.15 0
)

# ------- ConnectedThresholdImageFilterTest----------

otb_add_test(NAME seTeConnectedThresholdImageFilterTest1 COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/ConnectedThresholdOutput1.png
    ${TEMP}/ConnectedThresholdOutput1.png
  Execute $<TARGET_FILE:ConnectedThresholdImageFilter>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/ConnectedThresholdOutput1.png
    110 38 50 100
)

# ------- ConnectedThresholdImageFilterTest----------

otb_add_test(NAME seTeConnectedThresholdImageFilterTest2 COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/ConnectedThresholdOutput2.png
    ${TEMP}/ConnectedThresholdOutput2.png
  Execute $<TARGET_FILE:ConnectedThresholdImageFilter>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/ConnectedThresholdOutput2.png
    118 100 0 10
)

# ------- ConnectedThresholdImageFilterTest----------

otb_add_test(NAME seTeConnectedThresholdImageFilterTest3 COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/ConnectedThresholdOutput3.png
    ${TEMP}/ConnectedThresholdOutput3.png
  Execute $<TARGET_FILE:ConnectedThresholdImageFilter>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/ConnectedThresholdOutput3.png
    169 146 220 255
)

# ------- OtsuMultipleThresholdImageFilterTest----------

otb_add_test(NAME seTeOtsuMultipleThresholdImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 3
    ${BASELINE}/OtsuMultipleThresholdsOutput000.png
    ${TEMP}/OtsuMultipleThresholdsOutput000.png
    ${BASELINE}/OtsuMultipleThresholdsOutput001.png
    ${TEMP}/OtsuMultipleThresholdsOutput001.png
    ${BASELINE}/OtsuMultipleThresholdsOutput002.png
    ${TEMP}/OtsuMultipleThresholdsOutput002.png
  Execute $<TARGET_FILE:OtsuMultipleThresholdImageFilter>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/OtsuMultipleThresholdsOutput000.png
    ${TEMP}/OtsuMultipleThresholdsOutput001.png
    ${TEMP}/OtsuMultipleThresholdsOutput002.png
)

# ------- LabelizeNeighborhoodConnectedImageFilterTest----------

otb_add_test(NAME seTeLabelizeNeighborhoodConnectedImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/LabelizeNeighborhoodConnectedImageFilterOutput.png
    ${TEMP}/LabelizeNeighborhoodConnectedImageFilterOutput.png
  Execute $<TARGET_FILE:LabelizeNeighborhoodConnectedImageFilter>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/LabelizeNeighborhoodConnectedImageFilterOutput.png
    170 170
    80 80
)


# ------- StreamingMeanShiftSegmentationTest ----------
if(OTBMathParser_LOADED)
otb_add_test(NAME seTeStreamingMeanShiftSegmentationTest COMMAND ${OTB_TEST_DRIVER}
  # --compare-ogr ${NOTOL}
  # ${BASELINE_FILES}/seTeStreamingMeanShiftSegmentationTest.shp
  # ${TEMP}/seTeStreamingMeanShiftSegmentationTest.shp
  Execute $<TARGET_FILE:StreamingMeanShiftSegmentation>
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
endif()