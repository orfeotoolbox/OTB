otb_module_test()

set(OTBWaveletTests
otbWaveletTestDriver.cxx
otbWaveletOperator.cxx
otbWaveletPacketTransform.cxx
otbWaveletTransform.cxx
otbSubsampleImageFilter.cxx
otbSubsampleImageFilterNew.cxx
otbWaveletInverseFilterBankNew.cxx
otbWaveletInverseTransformNew.cxx
otbWaveletTransformNew.cxx
otbWaveletPacketInverseTransformNew.cxx
otbWaveletFilterBank.cxx
otbWaveletPacketTransformNew.cxx
otbWaveletFilterBankNew.cxx
otbWaveletOperatorNew.cxx
)

add_executable(otbWaveletTestDriver ${OTBWaveletTests})
target_link_libraries(otbWaveletTestDriver ${OTBWavelet-Test_LIBRARIES})
otb_module_target_label(otbWaveletTestDriver)

# Tests Declaration

otb_add_test(NAME msTuWaveletOperator COMMAND otbWaveletTestDriver
  otbWaveletOperator )

otb_add_test(NAME msTvWaveletPacketMultiScaleTransformSPLINE_BIORTHOGONAL_4_4 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSPLINE_BIORTHOGONAL_4_4.tif
  otbWaveletPacketTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSPLINE_BIORTHOGONAL_4_4.tif
  2 1
  7 #SPLINE_BIORTHOGONAL_4_4
  )

otb_add_test(NAME msTvWaveletPacketMultiScaleTransformHaar COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleHaar.tif
  otbWaveletPacketTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleHaar.tif
  2 1
  0 #Haar
  )

otb_add_test(NAME msTvWaveletPacketMultiScaleTransformDB8 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB8.tif
  otbWaveletPacketTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB8.tif
  2 1
  3 #DB8
  )

otb_add_test(NAME msTvWaveletPacketTransformDB4 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB4.tif
  otbWaveletPacketTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB4.tif
  2 2
  1 #DB4
  )

otb_add_test(NAME msTvWaveletPacketTransformDB20 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB20.tif
  otbWaveletPacketTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB20.tif
  2 2
  5 #DB20
  )

otb_add_test(NAME msTvWaveletPacketTransformDB6 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB6.tif
  otbWaveletPacketTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB6.tif
  2 2
  2 #DB6
  )

otb_add_test(NAME msTvWaveletPacketMultiScaleTransformSPLINE_BIORTHOGONAL_2_4 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSPLINE_BIORTHOGONAL_2_4.tif
  otbWaveletPacketTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSPLINE_BIORTHOGONAL_2_4.tif
  2 1
  6 #SPLINE_BIORTHOGONAL_2_4
  )

otb_add_test(NAME msTvWaveletPacketMultiScaleTransformDB6 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB6.tif
  otbWaveletPacketTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB6.tif
  2 1
  2 #DB6
  )

otb_add_test(NAME msTvWaveletPacketMultiScaleTransformDB4 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB4.tif
  otbWaveletPacketTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB4.tif
  2 1
  1 #DB4
  )

otb_add_test(NAME msTvWaveletPacketTransformDB8 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB8.tif
  otbWaveletPacketTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB8.tif
  2 2
  3 #DB8
  )

otb_add_test(NAME msTvWaveletPacketTransformSYMLET8 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketSYMLET8.tif
  otbWaveletPacketTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketSYMLET8.tif
  2 2
  8 #SYMLET8
  )

otb_add_test(NAME msTvWaveletPacketTransformHaar COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketHaar.tif
  otbWaveletPacketTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketHaar.tif
  2 2
  0 #Haar
  )

otb_add_test(NAME msTvWaveletPacketTransformSPLINE_BIORTHOGONAL_2_4 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketSPLINE_BIORTHOGONAL_2_4.tif
  otbWaveletPacketTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketSPLINE_BIORTHOGONAL_2_4.tif
  2 2
  6 #SPLINE_BIORTHOGONAL_2_4
  )

otb_add_test(NAME msTvWaveletPacketMultiScaleTransformDB20 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB20.tif
  otbWaveletPacketTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB20.tif
  2 1
  5 #DB20
  )

otb_add_test(NAME msTvWaveletPacketMultiScaleTransformSYMLET8 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSYMLET8.tif
  otbWaveletPacketTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleSYMLET8.tif
  2 1
  8 #SYMLET8
  )

otb_add_test(NAME msTvWaveletPacketTransformSPLINE_BIORTHOGONAL_4_4 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketSPLINE_BIORTHOGONAL_4_4.tif
  otbWaveletPacketTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketSPLINE_BIORTHOGONAL_4_4.tif
  2 2
  7 #SPLINE_BIORTHOGONAL_4_4
  )

otb_add_test(NAME msTvWaveletPacketTransformDB12 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB12.tif
  otbWaveletPacketTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketDB12.tif
  2 2
  4 #DB12
  )

otb_add_test(NAME msTvWaveletPacketMultiScaleTransformDB12 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB12.tif
  otbWaveletPacketTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltPacketMultiScaleDB12.tif
  2 1
  4 #DB12
  )

otb_add_test(NAME msTvWaveletTransformDB4 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB4.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB4.tif
  2 2
  1 #DB4
  )

otb_add_test(NAME msTvWaveletTransformDB6 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB6.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB6.tif
  2 2
  2 #DB6
  )

otb_add_test(NAME msTvWaveletMultiScaleTransformSYMLET8 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSYMLET8.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSYMLET8.tif
  2 1
  8 #SYMLET8
  )

otb_add_test(NAME msTvWaveletMultiScaleTransformSPLINE_BIORTHOGONAL_2_4 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSPLINE_BIORTHOGONAL_2_4.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSPLINE_BIORTHOGONAL_2_4.tif
  2 1
  6 #SPLINE_BIORTHOGONAL_2_4
  )

otb_add_test(NAME msTvWaveletTransformDB8 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB8.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB8.tif
  2 2
  3 #DB8
  )

otb_add_test(NAME msTvWaveletMultiScaleTransformSPLINE_BIORTHOGONAL_4_4 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSPLINE_BIORTHOGONAL_4_4.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleSPLINE_BIORTHOGONAL_4_4.tif
  2 1
  7 #SPLINE_BIORTHOGONAL_4_4
  )

otb_add_test(NAME msTvWaveletTransformDB20 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB20.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB20.tif
  2 2
  5 #DB20
  )

otb_add_test(NAME msTvWaveletMultiScaleTransformDB8_1 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB8_1.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB8_1.tif
  2 1
  3 #DB8
  1
  )

otb_add_test(NAME msTvWaveletMultiScaleTransformHaar_1 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar_1.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar_1.tif
  2 1
  0 #Haar
  1
  )

otb_add_test(NAME msTvWaveletMultiScaleTransformDB4_4 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4_4.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4_4.tif
  2 1
  1 #DB4
  4
  )

otb_add_test(NAME msTvWaveletMultiScaleTransformHaar_4 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar_4.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar_4.tif
  2 1
  0 #Haar
  4
  )

otb_add_test(NAME msTvWaveletMultiScaleTransformDB4_1 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4_1.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4_1.tif
  2 1
  1 #DB4
  1
  )

otb_add_test(NAME msTvWaveletMultiScaleTransformHaar_8 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar_8.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar_8.tif
  2 1
  0 #Haar
  8
  )

otb_add_test(NAME msTvWaveletMultiScaleTransformDB4_8 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4_8.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4_8.tif
  2 1
  1 #DB4
  8
  )

otb_add_test(NAME msTvWaveletMultiScaleTransformDB6_1 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB6_1.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB6_1.tif
  2 1
  2 #DB6
  1
  )

otb_add_test(NAME msTvWaveletTransformSPLINE_BIORTHOGONAL_2_4 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltSPLINE_BIORTHOGONAL_2_4.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltSPLINE_BIORTHOGONAL_2_4.tif
  2 2
  6 #SPLINE_BIORTHOGONAL_2_4
  )

otb_add_test(NAME msTvWaveletTransformSYMLET8 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltSYMLET8.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltSYMLET8.tif
  2 2
  8 #SYMLET8
  )

otb_add_test(NAME msTvWaveletTransformSPLINE_BIORTHOGONAL_4_4 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltSPLINE_BIORTHOGONAL_4_4.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltSPLINE_BIORTHOGONAL_4_4.tif
  2 2
  7 #SPLINE_BIORTHOGONAL_4_4
  )

otb_add_test(NAME msTvWaveletMultiScaleTransformDB12 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB12.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB12.tif
  2 1
  4 #DB12
  )

otb_add_test(NAME msTvWaveletMultiScaleTransformDB4 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB4.tif
  2 1
  1 #DB4
  )

otb_add_test(NAME msTvWaveletMultiScaleTransformDB6 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB6.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB6.tif
  2 1
  2 #DB6
  )

otb_add_test(NAME msTvWaveletMultiScaleTransformHaar COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleHaar.tif
  2 1
  0 #Haar
  )

otb_add_test(NAME msTvWaveletMultiScaleTransformDB8 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB8.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB8.tif
  2 1
  3 #DB8
  )

otb_add_test(NAME msTvWaveletTransformDB12 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB12.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltDB12.tif
  2 2
  4 #DB12
  )

otb_add_test(NAME msTvWaveletMultiScaleTransformDB20 COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB20.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltMultiScaleDB20.tif
  2 1
  5 #DB20
  )

otb_add_test(NAME msTvWaveletTransformHaar COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltHaar.tif
  otbWaveletTransform
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_WvltHaar.tif
  2 2
  0 #Haar
  )

otb_add_test(NAME msTvSubsampleImageFilter COMMAND otbWaveletTestDriver
  --compare-image ${NOTOL}
  ${TEMP}/msSubsampleImageFilterOversampled.tif
  ${TEMP}/msSubsampleImageFilterOnePixelOutOf2.tif
  otbSubsampleImageFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/msSubsampleImageFilterOversampled.tif
  ${TEMP}/msSubsampleImageFilterOnePixelOutOf2.tif
  )

otb_add_test(NAME msTuSubsampleImageFilterNew COMMAND otbWaveletTestDriver
  otbSubsampleImageFilterNew)

otb_add_test(NAME msTuWaveletInverseFilterBankNew COMMAND otbWaveletTestDriver
  otbWaveletInverseFilterBankNew)

otb_add_test(NAME msTuWaveletInverseTransformNew COMMAND otbWaveletTestDriver
  otbWaveletInverseTransformNew)

otb_add_test(NAME msTuWaveletTransformNew COMMAND otbWaveletTestDriver
  otbWaveletTransformNew)

otb_add_test(NAME msTuWaveletPacketInverseTransformNew COMMAND otbWaveletTestDriver
  otbWaveletPacketInverseTransformNew)

otb_add_test(NAME msTvWaveletMultiScaleFilterBank COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_FilterBankMultiScale.tif
  otbWaveletFilterBank
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  1
  ${TEMP}/ROI_IKO_PAN_LesHalles_FilterBankMultiScale.tif
  )

otb_add_test(NAME msTvWaveletFilterBank COMMAND otbWaveletTestDriver
  --compare-image ${EPSILON_6}
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/ROI_IKO_PAN_LesHalles_FilterBank.tif
  otbWaveletFilterBank
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  2
  ${TEMP}/ROI_IKO_PAN_LesHalles_FilterBank.tif
  )

otb_add_test(NAME msTuWaveletPacketTransformNew COMMAND otbWaveletTestDriver
  otbWaveletPacketTransformNew)

otb_add_test(NAME msTuWaveletFilterBankNew COMMAND otbWaveletTestDriver
  otbWaveletFilterBankNew)

otb_add_test(NAME msTuWaveletOperatorNew COMMAND otbWaveletTestDriver
  otbWaveletOperatorNew )

