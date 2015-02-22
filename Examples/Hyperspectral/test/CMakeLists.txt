set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Hyperspectral)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)

otb_add_test(NAME siTvHyperspectralExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${NOTOL} 3
    ${BASELINE}/siTvHyperspectralUnmixingExampleTestBand1.png
    ${TEMP}/siTvHyperspectralUnmixingExampleTestBand1.png
    ${BASELINE}/siTvHyperspectralUnmixingExampleTestBand2.png
    ${TEMP}/siTvHyperspectralUnmixingExampleTestBand2.png
    ${BASELINE}/siTvHyperspectralUnmixingExampleTestBand3.png
    ${TEMP}/siTvHyperspectralUnmixingExampleTestBand3.png
  Execute $<TARGET_FILE:HyperspectralUnmixingExample>
    ${IMAGEDATA}/AVIRIS/Indian_pines_corrected.tif
    ${TEMP}/siTvHyperspectralUnmixingExampleTest.tif
    ${TEMP}/siTvHyperspectralUnmixingExampleTestBand1.png
    ${TEMP}/siTvHyperspectralUnmixingExampleTestBand2.png
    ${TEMP}/siTvHyperspectralUnmixingExampleTestBand3.png
    16
)
