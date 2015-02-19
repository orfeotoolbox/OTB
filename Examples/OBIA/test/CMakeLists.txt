
otb_add_test(NAME  trTeOBIAImageToLabelToImageTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/OBIAI2L2IOutput.tif
    ${TEMP}/OBIAI2L2IOutput.tif
  $<TARGET_FILE:ImageToLabelToImage>
    ${INPUTDATA}/MSLabeledOutput.tif
    ${TEMP}/OBIAI2L2IOutput.tif
    ${TEMP}/OBIAI2L2IInPretty.png
    ${TEMP}/OBIAI2L2IOutPretty.png
    1 1108 0
)

otb_add_test(NAME trTeOBIAShapeAttributeComputationTest COMMAND ${OTB_TEST_DRIVER}
  --compare-ascii ${EPSILON_3}
    ${BASELINE}/OBIAShapeAttribute.txt
    ${TEMP}/OBIAShapeAttribute.txt
  $<TARGET_FILE:ShapeAttributeComputation>
    ${INPUTDATA}/MSLabeledOutput.tif
    ${TEMP}/OBIAShapeAttribute.txt
)

otb_add_test(NAME trTeOBIARadiometricAttributesLabelMapFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/OBIARadiometricAttributes.png
    ${TEMP}/OBIARadiometricAttributes.png
  $<TARGET_FILE:RadiometricAttributesLabelMapFilterExample>
    ${INPUTDATA}/qb_RoadExtract.tif
    ${TEMP}/OBIARadiometricAttributes.png
    ${TEMP}/qb_RoadExtract_Radiometry_pretty.png
    STATS::Band1::Mean
    0
    0.5
    16
    16
    50
    1.0
)

otb_add_test(NAME trTeLabelMapToVectorData COMMAND ${OTB_TEST_DRIVER}
  $<TARGET_FILE:LabelMapToVectorData>
    ${INPUTDATA}/rcc8_mire1.png
    ${TEMP}/rcc8_mire2_vectorizer.shp
)
