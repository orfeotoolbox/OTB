otb_module_test()

set(OTBMomentsTests
otbMomentsTestDriver.cxx
otbComplexMomentPathFloat.cxx
otbRadiometricMomentsImageFunction.cxx
otbMetaImageFunction.cxx
otbComplexMomentPathNew.cxx
otbHuMomentsImageFunction.cxx
otbRealMomentsImageFunction.cxx
otbNormalizeAttributesLabelMapFilter.cxx
otbComplexMomentsImageFunction.cxx
otbLabelMapToSampleListFilter.cxx
otbHuPathNew.cxx
otbMinMaxAttributesLabelMapFilter.cxx
otbShapeAttributesLabelMapFilterNew.cxx
otbFlusserPathNew.cxx
otbFlusserMomentsImageFunction.cxx
otbImageToLabelMapWithAttributesFilter.cxx
otbImageToLabelMapWithAttributesFilterNew.cxx
otbHuPath.cxx
otbKMeansAttributesLabelMapFilter.cxx
otbFlusserPath.cxx
otbComplexMomentPath.cxx
otbLabelMapWithClassLabelToLabeledSampleListFilter.cxx
otbRadiometricMomentsImageFilter.cxx
)

add_executable(otbMomentsTestDriver ${OTBMomentsTests})
target_link_libraries(otbMomentsTestDriver ${OTBMoments-Test_LIBRARIES})
otb_module_target_label(otbMomentsTestDriver)

# Tests Declaration

otb_add_test(NAME feTvComplexMomentPathFloat COMMAND otbMomentsTestDriver
  --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feComplexMomentPathFloat.txt
  ${TEMP}/feComplexMomentPathFloat.txt
  otbComplexMomentPathFloat
  1
  1
  ${TEMP}/feComplexMomentPathFloat.txt)

otb_add_test(NAME feTuRadiometricMomentsImageFunctionNew COMMAND otbMomentsTestDriver
  otbRadiometricMomentsImageFunctionNew
  )

otb_add_test(NAME feTvRadiometricMomentsImageFunction COMMAND otbMomentsTestDriver
  --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feTvRadiometricMomentsImageFunction.txt
  ${TEMP}/feTvRadiometricMomentsImageFunction.txt
  otbRadiometricMomentsImageFunction
  ${INPUTDATA}/poupees.png
  ${TEMP}/feTvRadiometricMomentsImageFunction.txt)

otb_add_test(NAME feTvMetaImageFunction COMMAND otbMomentsTestDriver
  --compare-ascii ${EPSILON_8}
  ${BASELINE_FILES}/feTvMetaImageFunction.txt
  ${TEMP}/feTvMetaImageFunction.txt
  otbMetaImageFunction
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/feTvMetaImageFunction.txt
  451846.014047961 5412466.57452216
  )

otb_add_test(NAME feTuMetaImageFunctionNew COMMAND otbMomentsTestDriver
  otbMetaImageFunctionNew
  )

otb_add_test(NAME feTuComplexMomentPathNew COMMAND otbMomentsTestDriver
  otbComplexMomentPathNew)

otb_add_test(NAME feTvHuMomentsImageFunction COMMAND otbMomentsTestDriver
  --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feTvHuMomentsImageFunction.txt
  ${TEMP}/feTvHuMomentsImageFunction.txt
  otbHuMomentsImageFunction
  ${INPUTDATA}/poupees.png
  ${TEMP}/feTvHuMomentsImageFunction.txt
  )

otb_add_test(NAME feTvHuMomentsImageFunctionScaleInvariant COMMAND otbMomentsTestDriver
  otbHuMomentsImageFunctionScaleInvariant
  ${INPUTDATA}/poupees.png
  )

otb_add_test(NAME feTuHuMomentsImageFunctionNew COMMAND otbMomentsTestDriver
  otbHuMomentsImageFunctionNew
  )

otb_add_test(NAME feTvHuMomentsImageFunctionRotationInvariant COMMAND otbMomentsTestDriver
  otbHuMomentsImageFunctionRotationInvariant
  ${INPUTDATA}/poupees.png
  90
  )

otb_add_test(NAME feTuRealMomentsImageFunctionNew COMMAND otbMomentsTestDriver
  otbRealMomentsImageFunctionNew
  )

otb_add_test(NAME feTvRealMomentsImageFunction COMMAND otbMomentsTestDriver
  --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feTvRealMomentsImageFunction.txt
  ${TEMP}/feTvRealMomentsImageFunction.txt
  otbRealMomentsImageFunction
  ${INPUTDATA}/poupees.png
  4
  4
  ${TEMP}/feTvRealMomentsImageFunction.txt)

otb_add_test(NAME obTuNormalizeAttributesLabelMapFilterNew COMMAND otbMomentsTestDriver
  otbNormalizeAttributesLabelMapFilterNew)

otb_add_test(NAME obTvNormalizeAttributesLabelMapFilter COMMAND otbMomentsTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/obTvNormalizeAttributesLabelMapFilter.txt
  ${TEMP}/obTvNormalizeAttributesLabelMapFilter.txt
  otbNormalizeAttributesLabelMapFilter
  ${INPUTDATA}/maur.tif
  ${INPUTDATA}/maur_labelled.tif
  ${TEMP}/obTvNormalizeAttributesLabelMapFilter.txt)

otb_add_test(NAME feTvComplexMomentsImageFunction COMMAND otbMomentsTestDriver
  --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feTvComplexMomentsImageFunction.txt
  ${TEMP}/feTvComplexMomentsImageFunction.txt
  otbComplexMomentsImageFunction
  ${INPUTDATA}/poupees.png
  4
  4
  ${TEMP}/feTvComplexMomentsImageFunction.txt
  )

otb_add_test(NAME feTuComplexMomentsImageFunctionNew COMMAND otbMomentsTestDriver
  otbComplexMomentsImageFunctionNew
  )

otb_add_test(NAME feTvComplexMomentsImageFunctionScaleInvariant COMMAND otbMomentsTestDriver
  otbComplexMomentsImageFunctionScaleInvariant
  ${INPUTDATA}/poupees.png
  4
  4
  )

otb_add_test(NAME obTvLabelMapToSampleListFilter COMMAND otbMomentsTestDriver
  otbLabelMapToSampleListFilter
  ${OTB_DATA_ROOT}/Input/rcc8_mire1.png
  SHAPE::Flusser01 SHAPE::Flusser02 SHAPE::Flusser03 SHAPE::Flusser04
  SHAPE::Flusser05 SHAPE::Flusser06 SHAPE::Flusser07 SHAPE::Flusser08
  SHAPE::Flusser09 SHAPE::Flusser10 SHAPE::Flusser11)

otb_add_test(NAME feTuHuPathNew COMMAND otbMomentsTestDriver
  otbHuPathNew)

otb_add_test(NAME obTuMinMaxAttributesLabelMapFilterNew COMMAND otbMomentsTestDriver
  otbMinMaxAttributesLabelMapFilterNew)

otb_add_test(NAME obTvMinMaxAttributesLabelMapFilter COMMAND otbMomentsTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/obTvMinMaxAttributesLabelMapFilter.txt
  ${TEMP}/obTvMinMaxAttributesLabelMapFilter.txt
  otbMinMaxAttributesLabelMapFilter
  ${INPUTDATA}/maur.tif
  ${INPUTDATA}/maur_labelled.tif
  ${TEMP}/obTvMinMaxAttributesLabelMapFilter.txt)

otb_add_test(NAME obTuShapeAttributesLabelMapFilterNew COMMAND otbMomentsTestDriver
  otbShapeAttributesLabelMapFilterNew)

otb_add_test(NAME feTuFlusserPathNew COMMAND otbMomentsTestDriver
  otbFlusserPathNew)

otb_add_test(NAME feTuFlusserMomentsImageFunctionNew COMMAND otbMomentsTestDriver
  otbFlusserMomentsImageFunctionNew
  )

otb_add_test(NAME feTvFlusserMomentsImageFunctionScaleInvariant COMMAND otbMomentsTestDriver
  otbFlusserMomentsImageFunctionScaleInvariant
  ${INPUTDATA}/poupees.png
  )

otb_add_test(NAME feTvFlusserMomentsImageFunction COMMAND otbMomentsTestDriver
  --compare-ascii ${EPSILON_8}     ${BASELINE_FILES}/feTvFlusserMomentsImageFunction.txt
  ${TEMP}/feTvFlusserMomentsImageFunction.txt
  otbFlusserMomentsImageFunction
  ${INPUTDATA}/poupees.png
  ${TEMP}/feTvFlusserMomentsImageFunction.txt)

otb_add_test(NAME feTvFlusserMomentsImageFunctionRotationInvariant COMMAND otbMomentsTestDriver
  otbFlusserMomentsImageFunctionRotationInvariant
  ${INPUTDATA}/poupees.png
  90
  )

otb_add_test(NAME obTvImageToLabelMapWithAttributesFilter COMMAND otbMomentsTestDriver
  otbImageToLabelMapWithAttributesFilter
  ${INPUTDATA}/maur.tif
  ${INPUTDATA}/maur_labelled.tif)

otb_add_test(NAME obTuImageToLabelMapWithAttributesFilterNew COMMAND otbMomentsTestDriver
  otbImageToLabelMapWithAttributesFilterNew)

otb_add_test(NAME feTvHuPath COMMAND otbMomentsTestDriver
  --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feHuPath.txt
  ${TEMP}/feHuPath.txt
  otbHuPath
  ${TEMP}/feHuPath.txt)

otb_add_test(NAME obTvKMeansAttributesLabelMapFilter COMMAND otbMomentsTestDriver
  otbKMeansAttributesLabelMapFilter
  ${INPUTDATA}/maur.tif
  ${INPUTDATA}/maur_labelled.tif
  ${TEMP}/obTvKMeansAttributesLabelMapFilter.txt)

otb_add_test(NAME obTuKMeansAttributesLabelMapFilterNew COMMAND otbMomentsTestDriver
  otbKMeansAttributesLabelMapFilterNew)

otb_add_test(NAME feTvFlusserPath COMMAND otbMomentsTestDriver
  --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feFlusserPath.txt
  ${TEMP}/feFlusserPath.txt
  otbFlusserPath
  ${TEMP}/feFlusserPath.txt)

otb_add_test(NAME feTvComplexMomentPath COMMAND otbMomentsTestDriver
  --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feComplexMomentPath.txt
  ${TEMP}/feComplexMomentPath.txt
  otbComplexMomentPath
  1
  1
  ${TEMP}/feComplexMomentPath.txt)

otb_add_test(NAME obTvLabelMapWithClassLabelToLabeledSampleListFilter COMMAND otbMomentsTestDriver
  otbLabelMapWithClassLabelToLabeledSampleListFilter
  ${OTB_DATA_ROOT}/Input/rcc8_mire1.png
  SHAPE::Flusser01  SHAPE::Flusser02  SHAPE::Flusser03 SHAPE::Flusser04
  SHAPE::Flusser05  SHAPE::Flusser06  SHAPE::Flusser07 SHAPE::Flusser08
  SHAPE::Flusser09 SHAPE::Flusser10  SHAPE::Flusser11)

otb_add_test(NAME feTuRadiometricMomentsImageFilterNew COMMAND otbMomentsTestDriver
  otbRadiometricMomentsImageFilterNew)

otb_add_test(NAME feTvRadiometricMomentsImageFilter COMMAND otbMomentsTestDriver
  --compare-image ${EPSILON_8}
  ${BASELINE}/feTvRadiometricMomentsImageFilter.tif
  ${TEMP}/feTvRadiometricMomentsImageFilter.tif
  otbRadiometricMomentsImageFilter
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/feTvRadiometricMomentsImageFilter.tif
  3 #radius
  )

