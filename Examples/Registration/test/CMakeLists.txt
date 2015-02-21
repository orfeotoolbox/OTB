otb_add_test(NAME reTeImageRegistration1ExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/ImageRegistration1Output.png
    ${TEMP}/ImageRegistration1Output.png
  Execute $<TARGET_FILE:ImageRegistration1>
    ${INPUTDATA}/QB_Suburb.png
    ${INPUTDATA}/QB_Suburb13x17y.png
    ${TEMP}/ImageRegistration1Output.png
    ${TEMP}/ImageRegistration1DifferenceAfter.png
    ${TEMP}/ImageRegistration1DifferenceBefore.png
)

otb_add_test(NAME reTeImageRegistration2ExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/ImageRegistration2Output.png
    ${TEMP}/ImageRegistration2Output.png
  Execute $<TARGET_FILE:ImageRegistration2>
    ${INPUTDATA}/RamsesROISmall.png
    ${INPUTDATA}/ADS40RoiSmall.png
    ${TEMP}/ImageRegistration2Output.png
    ${TEMP}/ImageRegistration2CheckerboardBefore.png
    ${TEMP}/ImageRegistration2CheckerboardAfter.png
)

otb_add_test(NAME reTeImageRegistration5Cas1ExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/ImageRegistration5Output1.png
    ${TEMP}/ImageRegistration5Output1.png
  Execute $<TARGET_FILE:ImageRegistration5>
    ${INPUTDATA}/QB_Suburb.png
    ${INPUTDATA}/QB_SuburbRotated10.png
    ${TEMP}/ImageRegistration5Output1.png
    ${TEMP}/ImageRegistration5DifferenceBefore1.png
    ${TEMP}/ImageRegistration5DifferenceAfter1.png
)
otb_add_test(NAME reTeImageRegistration5Cas2ExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/ImageRegistration5Output2.png
    ${TEMP}/ImageRegistration5Output2.png
  Execute $<TARGET_FILE:ImageRegistration5>
    ${INPUTDATA}/QB_Suburb.png
    ${INPUTDATA}/QB_SuburbR10X13Y17.png
    ${TEMP}/ImageRegistration5Output2.png
    ${TEMP}/ImageRegistration5DifferenceBefore2.png
    ${TEMP}/ImageRegistration5DifferenceAfter2.png
)
