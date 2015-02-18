
# ------- ScalarImageMarkovRandomField1Test----------

otb_add_test(NAME clTeScalarImageMarkovRandomField1Test COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${TOL} 1
    ${BASELINE}/ScalarImageMarkovRandomField1Output.png
    ${TEMP}/ScalarImageMarkovRandomField1Output.png
  $<TARGET_FILE:ScalarImageMarkovRandomField1>
    ${INPUTDATA}/QB_Suburb.png
    ${INPUTDATA}/QB_Suburb_labelled.png
    ${TEMP}/ScalarImageMarkovRandomField1Output.png
    50 3 4 79.5097 138.136 213.846 25.9395
)

# ------- ScalarImageKmeansClassifierTest----------

otb_add_test(NAME clTeScalarImageKmeansClassifierTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${TOL} 1
    ${BASELINE}/QB_Suburb_labelled.png
    ${TEMP}/QB_Suburb_labelled.png
  $<TARGET_FILE:ScalarImageKmeansClassifier>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/QB_Suburb_labelled.png
    0 4 79.5097 138.136 213.846 25.9395
)

otb_add_test(NAME clTeNeighborhoodMajorityVotingImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${OTB_DATA_ROOT}/Baseline/OTB/Images/QB_1_ortho_4Cls_N_Classified_OTB_NMV_x2_y5_nodv10_undv7.tif
    ${TEMP}/QB_1_ortho_4Cls_N_Classified_OTB_NMV_x2_y5_nodv10_undv7_EX.tif
  $<TARGET_FILE:ClassificationMapRegularizationExample>
    ${OTB_DATA_ROOT}/Input/Classification/QB_1_ortho_4Cls_N_Classified_OTB.tif
    ${TEMP}/QB_1_ortho_4Cls_N_Classified_OTB_NMV_x2_y5_nodv10_undv7_EX.tif
    false #KeepOriginalLabelBool
    2 #xRadius
    5 #yRadius
    10 #LabelForNoDataPixels
    7 #LabelForUndecidedPixels
)
