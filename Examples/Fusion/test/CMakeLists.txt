
otb_add_test(NAME fuTeBayesianFusionImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${EPSILON}
    ${BASELINE}/ImageBayesianFusion.tif
    ${TEMP}/ImageBayesianFusion.tif
  Execute $<TARGET_FILE:BayesianFusionImageFilter>
    ${INPUTDATA}/multiSpect.tif
    ${INPUTDATA}/multiSpectInterp.tif
    ${INPUTDATA}/panchro.tif
    ${TEMP}/ImageBayesianFusion.tif
    ${TEMP}/ImageBayesianFusion1.png
    ${TEMP}/ImageBayesianFusion2.png
    ${TEMP}/ImageBayesianFusion3.png
    ${TEMP}/ImageBayesianFusion4.png
    0.9999
)

otb_add_test(NAME fuTePanSharpeningExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${EPSILON}
    ${BASELINE}/PanSharpeningExampleTest.tif
    ${TEMP}/PanSharpeningExampleTest.tif
  Execute $<TARGET_FILE:PanSharpeningExample>
    ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
    ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
    ${TEMP}/PanSharpeningExampleTest.tif
    ${TEMP}/PanSharpeningExampleTest1.png
    ${TEMP}/PanSharpeningExampleTest2.png
    ${TEMP}/PanSharpeningExampleTest3.png
)
