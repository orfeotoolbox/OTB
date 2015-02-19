
# ------- ImageAdaptor4Test----------

otb_add_test(NAME diTeImageAdaptor4Test1 COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/ImageAdaptorThresholdingA.png
    ${TEMP}/ImageAdaptorThresholdingA.png
  $<TARGET_FILE:ImageAdaptor4>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/ImageAdaptorThresholdingA.png
    100
)

# ------- ImageAdaptor4Test----------

otb_add_test(NAME diTeImageAdaptor4Test2 COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/ImageAdaptorThresholdingB.png
    ${TEMP}/ImageAdaptorThresholdingB.png
  $<TARGET_FILE:ImageAdaptor4>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/ImageAdaptorThresholdingB.png
    200
)

# ------- Image5Test----------

otb_add_test(NAME diTeImage5Test COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/Image5.png
    ${TEMP}/Image5.png
  $<TARGET_FILE:Image5>
    ${TEMP}/Image5.png
)

# ------- Image6Test----------

otb_add_test(NAME diTeImage6Test COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/Image6.tif
    ${TEMP}/Image6.tif
  $<TARGET_FILE:Image6>
    ${TEMP}/Image6.tif
)
