
otb_add_test(NAME  trTeTutorialsHelloWorld COMMAND ${OTB_TEST_DRIVER}
  $<TARGET_FILE:HelloWorldOTB>
)

otb_add_test(NAME  trTeTutorialsPipelineTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${TOL}
    ${BASELINE}/TutorialsPipelineOutput.png
    ${TEMP}/TutorialsPipelineOutput.png
  $<TARGET_FILE:Pipeline>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/TutorialsPipelineOutput.png
)

otb_add_test(NAME  trTeTutorialsFilteringPipelineTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${TOL}
    ${BASELINE}/TutorialsFilteringPipelineOutput.png
    ${TEMP}/TutorialsFilteringPipelineOutput.png
  $<TARGET_FILE:FilteringPipeline>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/TutorialsFilteringPipelineOutput.png
)

otb_add_test(NAME  trTeTutorialsScalingPipelineTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${TOL}
    ${BASELINE}/TutorialsScalingPipelineOutput.png
    ${TEMP}/TutorialsScalingPipelineOutput.png
  $<TARGET_FILE:ScalingPipeline>
    ${INPUTDATA}/QB_Suburb.png
    ${TEMP}/TutorialsScalingPipelineOutput.png
)

otb_add_test(NAME  trTeTutorialsMultispectralTest COMMAND ${OTB_TEST_DRIVER}
  --compare-n-images ${TOL} 2
    ${BASELINE}/MultispectralOutput1.tif
    ${TEMP}/MultispectralOutput1.tif
    ${BASELINE}/MultispectralOutput2.tif
    ${TEMP}/MultispectralOutput2.tif
  $<TARGET_FILE:Multispectral>
    ${INPUTDATA}/qb_RoadExtract.tif
    ${TEMP}/MultispectralOutput1.tif
    ${TEMP}/MultispectralOutput2.tif
)


otb_add_test(NAME  trTeTutorialsSmarterFilteringPipelineTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${TOL}
    ${BASELINE}/TutorialsSmarterFilteringPipelineOutput.png
    ${TEMP}/TutorialsSmarterFilteringPipelineOutput.png
  $<TARGET_FILE:SmarterFilteringPipeline>
    -in ${INPUTDATA}/QB_Suburb.png
    -out ${TEMP}/TutorialsSmarterFilteringPipelineOutput.png
    -d 1.5
    -i 2
    -a 0.1
)

otb_add_test(NAME  trTeTutorialsOrthoFusionTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${TOL}
    ${BASELINE}/TutorialsOrthoFusionOutput.tif
    ${TEMP}/TutorialsOrthoFusionOutput.tif
  $<TARGET_FILE:OrthoFusion>
    LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
    LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
    ${TEMP}/TutorialsOrthoFusionOutput.tif
    31
    N
    375000
    4828100
    500
    500
    0.6
    -0.6
)
