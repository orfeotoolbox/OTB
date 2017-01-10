set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/BasicFilters)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)

otb_add_test(NAME bfTeLeeImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/GomaSmallLeeFiltered.tif
    ${TEMP}/GomaSmallLeeFiltered.tif
  Execute $<TARGET_FILE:LeeImageFilter>
    ${INPUTDATA}/GomaSmall.tif
    ${TEMP}/GomaSmallLeeFiltered.tif
    3 1
)

# ------- FrostImageFilterTest----------

otb_add_test(NAME bfTeFrostImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/GomaSmallFrostFiltered.tif
    ${TEMP}/GomaSmallFrostFiltered.tif
  Execute $<TARGET_FILE:FrostImageFilter>
    ${INPUTDATA}/GomaSmall.tif
    ${TEMP}/GomaSmallFrostFiltered.tif
    5 0.1
)

# ------- DEMToRainbowExampleTest ----------

otb_add_test(NAME bfTeDEMToRainbowExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/DEMToRainbowImageGenerator.png
    ${TEMP}/DEMToRainbowImageGenerator.png
  Execute $<TARGET_FILE:DEMToRainbowExample>
    ${TEMP}/DEMToRainbowImageGenerator.png
    6.5 45.5 500 500 0.002 -0.002 ${OTB_DATA_ROOT}/Examples/DEM_srtm
)


# ------- ScalingFilterExampleTest ----------

otb_add_test(NAME bfTeScalingFilterExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/QB_Toulouse_Ortho_PAN_rescaled.png
    ${TEMP}/QB_Toulouse_Ortho_PAN_rescaled.png
    ${BASELINE}/QB_Toulouse_Ortho_PAN_casted.png
    ${TEMP}/QB_Toulouse_Ortho_PAN_casted.png
  Execute $<TARGET_FILE:ScalingFilterExample>
    ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
    ${TEMP}/QB_Toulouse_Ortho_PAN_rescaled.png
    ${TEMP}/QB_Toulouse_Ortho_PAN_casted.png
)

# ------- PrintableImageFilterTest----------

otb_add_test(NAME bfTePrintableImageFilterExample1Test COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/PrintableExampleOutput1.png
    ${TEMP}/PrintableExampleOutput1.png
  Execute $<TARGET_FILE:PrintableImageFilterExample>
    ${INPUTDATA}/IMAGERY_SSECH.tif
    ${TEMP}/PrintableExampleOutput1.png
    1 2 3
)

otb_add_test(NAME bfTePrintableImageFilteExample2rTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 1
    ${BASELINE}/PrintableExampleOutput2.png
    ${TEMP}/PrintableExampleOutput2.png
  Execute $<TARGET_FILE:PrintableImageFilterExample>
    ${INPUTDATA}/IMAGERY_SSECH.tif
    ${TEMP}/PrintableExampleOutput2.png
    1 4 2
)

# ------- IndexedToRGBExampleTest----------

otb_add_test(NAME bfTeIndexedToRGBExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/buildingExtractionRGB.png
    ${TEMP}/buildingExtractionRGB.png
    ${BASELINE}/buildingExtractionIndexed_scaled.png
    ${TEMP}/buildingExtractionIndexed_scaled.png
  Execute $<TARGET_FILE:IndexedToRGBExample>
    ${INPUTDATA}/buildingExtractionIndexed.tif
    ${TEMP}/buildingExtractionRGB.png
    ${TEMP}/buildingExtractionIndexed_scaled.png
)
#if(OTBMathParser_LOADED)
#otb_add_test(NAME bfTeMeanShiftSegmentationFilterExampleTest COMMAND ${OTB_TEST_DRIVER}
  #--compare-n-images ${NOTOL} 4
    #${BASELINE}/MSClusteredOutput.tif
    #${TEMP}/MSClusteredOutput.tif
    #${BASELINE}/MSLabeledOutput.tif
    #${TEMP}/MSLabeledOutput.tif
    #${BASELINE}/MSLabeledOutput-pretty.png
    #${TEMP}/MSLabeledOutput-pretty.png
    #${BASELINE}/MSClusteredOutput-pretty.png
    #${TEMP}/MSClusteredOutput-pretty.png
  #Execute $<TARGET_FILE:MeanShiftSegmentationFilterExample>
    #${INPUTDATA}/ROI_QB_MUL_1.png
    #${TEMP}/MSLabeledOutput.tif
    #${TEMP}/MSClusteredOutput.tif
    #${TEMP}/MSLabeledOutput-pretty.png
    #${TEMP}/MSClusteredOutput-pretty.png
    #16
    #16
    #100
    #100
    #0.1
#)
#endif()
# ------- HillShadingExampleTest ----------

otb_add_test(NAME bfTeHillShadingExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 2
    ${BASELINE}/HillShadingExample.png
    ${TEMP}/HillShadingExample.png
    ${BASELINE}/HillShadingColorExample.png
    ${TEMP}/HillShadingColorExample.png
  Execute $<TARGET_FILE:HillShadingExample>
    ${TEMP}/HillShadingExample.png
    ${TEMP}/HillShadingColorExample.png
    6.8 45.3 500 500 0.002 -0.002 ${OTB_DATA_ROOT}/Examples/DEM_srtm
)
