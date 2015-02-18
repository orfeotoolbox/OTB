otb_module_test()

set(OTBMorphologicalProfilesTests
otbMorphologicalProfilesTestDriver.cxx
otbMorphologicalProfilesSegmentationFilter.cxx
otbProfileToProfileDerivativeFilterNew.cxx
otbGeodesicMorphologyDecompositionImageFilter.cxx
otbMorphologicalClosingProfileFilterNew.cxx
otbConvexOrConcaveClassificationFilter.cxx
otbGeodesicMorphologyLevelingFilter.cxx
otbConvexOrConcaveClassificationFilterNew.cxx
otbMultiScaleConvexOrConcaveClassificationFilter.cxx
otbMorphologicalOpeningProfileFilter.cxx
otbGeodesicMorphologyIterativeDecompositionImageFilter.cxx
otbGeodesicMorphologyIterativeDecompositionImageFilterNew.cxx
otbGeodesicMorphologyLevelingFilterNew.cxx
otbClosingOpeningMorphologicalFilter.cxx
otbProfileDerivativeToMultiScaleCharacteristicsFilterNew.cxx
otbProfileToProfileDerivativeFilter.cxx
otbOpeningClosingMorphologicalFilterNew.cxx
otbGeodesicMorphologyDecompositionImageFilterNew.cxx
otbMorphologicalOpeningProfileFilterNew.cxx
otbProfileDerivativeToMultiScaleCharacteristicsFilter.cxx
otbMultiScaleConvexOrConcaveClassificationFilterNew.cxx
otbClosingOpeningMorphologicalFilterNew.cxx
otbOpeningClosingMorphologicalFilter.cxx
otbMorphologicalClosingProfileFilter.cxx
)

add_executable(otbMorphologicalProfilesTestDriver ${OTBMorphologicalProfilesTests})
target_link_libraries(otbMorphologicalProfilesTestDriver ${OTBMorphologicalProfiles-Test_LIBRARIES})
otb_module_target_label(otbMorphologicalProfilesTestDriver)

# Tests Declaration

otb_add_test(NAME obTvMorphologicalProfilesSegmentationFilter COMMAND otbMorphologicalProfilesTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/obTvMorphologicalProfilesSegmentationFilter.tif
  ${TEMP}/obTvMorphologicalProfilesSegmentationFilter.tif
  otbMorphologicalProfilesSegmentationFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/obTvMorphologicalProfilesSegmentationFilter.tif
  1
  1
  5
  )

otb_add_test(NAME msTuProfileToProfileDerivativeFilterNew COMMAND otbMorphologicalProfilesTestDriver
  otbProfileToProfileDerivativeFilterNew)

otb_add_test(NAME msTvGeodesicMorphologyDecompositionImageFilter COMMAND otbMorphologicalProfilesTestDriver
  --compare-n-images ${NOTOL} 3
  ${BASELINE}/msGeodesicMorphologyDecompositionImageFilterConvexOutput.tif
  ${TEMP}/msGeodesicMorphologyDecompositionImageFilterConvexOutput.tif
  ${BASELINE}/msGeodesicMorphologyDecompositionImageFilterConcaveOutput.tif
  ${TEMP}/msGeodesicMorphologyDecompositionImageFilterConcaveOutput.tif
  ${BASELINE}/msGeodesicMorphologyDecompositionImageFilterLevelingOutput.tif
  ${TEMP}/msGeodesicMorphologyDecompositionImageFilterLevelingOutput.tif
  otbGeodesicMorphologyDecompositionImageFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/msGeodesicMorphologyDecompositionImageFilterConvexOutput.tif
  ${TEMP}/msGeodesicMorphologyDecompositionImageFilterConcaveOutput.tif
  ${TEMP}/msGeodesicMorphologyDecompositionImageFilterLevelingOutput.tif
  4
  )

otb_add_test(NAME msTuMorphologicalClosingProfileFilterNew COMMAND otbMorphologicalProfilesTestDriver
  otbMorphologicalClosingProfileFilterNew)

otb_add_test(NAME msTvConvexOrConcaveClassificationFilter COMMAND otbMorphologicalProfilesTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/msConvexOrConcaveClassificationFilterOutput.png
  ${TEMP}/msConvexOrConcaveClassificationFilterOutput.png
  otbConvexOrConcaveClassificationFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles_leveling.tif
  ${TEMP}/msConvexOrConcaveClassificationFilterOutput.png
  0.5
  )

otb_add_test(NAME msTvGeodesicMorphologyLevelingFilter COMMAND otbMorphologicalProfilesTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/msGeodesicMorphologyLevelingFilterOutput.tif
  ${TEMP}/msGeodesicMorphologyLevelingFilterOutput.tif
  otbGeodesicMorphologyLevelingFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles_convex.tif
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles_concave.tif
  ${TEMP}/msGeodesicMorphologyLevelingFilterOutput.tif
  )

otb_add_test(NAME msTuConvexOrConcaveClassificationFilterNew COMMAND otbMorphologicalProfilesTestDriver
  otbConvexOrConcaveClassificationFilterNew)

otb_add_test(NAME msTvMultiScaleConvexOrConcaveClassificationFilter COMMAND otbMorphologicalProfilesTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/msMultiScaleConvexOrConcaveClassificationFilterOutput.tif
  ${TEMP}/msMultiScaleConvexOrConcaveClassificationFilterOutput.tif
  otbMultiScaleConvexOrConcaveClassificationFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/msMultiScaleConvexOrConcaveClassificationFilterOutput.tif
  5
  1
  1
  1.
  )

otb_add_test(NAME msTvMorphologicalOpeningProfileFilter COMMAND otbMorphologicalProfilesTestDriver
  --compare-n-images ${NOTOL} 4
  ${BASELINE}/msMorphologicalOpeningProfileFilterOutput1.tif
  ${TEMP}/msMorphologicalOpeningProfileFilterOutput1.tif
  ${BASELINE}/msMorphologicalOpeningProfileFilterOutput2.tif
  ${TEMP}/msMorphologicalOpeningProfileFilterOutput2.tif
  ${BASELINE}/msMorphologicalOpeningProfileFilterOutput3.tif
  ${TEMP}/msMorphologicalOpeningProfileFilterOutput3.tif
  ${BASELINE}/msMorphologicalOpeningProfileFilterOutput4.tif
  ${TEMP}/msMorphologicalOpeningProfileFilterOutput4.tif
  otbMorphologicalOpeningProfileFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/msMorphologicalOpeningProfileFilterOutput
  tif
  4
  1
  1
  )

otb_add_test(NAME msTvGeodesicMorphologyIterativeDecompositionImageFilter COMMAND otbMorphologicalProfilesTestDriver
  --compare-n-images ${NOTOL} 6
  ${BASELINE}/msGeodesicMorphologyIterativeDecompositionImageFilter_concMap_1.tif
  ${TEMP}/msGeodesicMorphologyIterativeDecompositionImageFilter_concMap_1.tif
  ${BASELINE}/msGeodesicMorphologyIterativeDecompositionImageFilter_concMap_2.tif
  ${TEMP}/msGeodesicMorphologyIterativeDecompositionImageFilter_concMap_2.tif
  ${BASELINE}/msGeodesicMorphologyIterativeDecompositionImageFilter_convMap_1.tif
  ${TEMP}/msGeodesicMorphologyIterativeDecompositionImageFilter_convMap_1.tif
  ${BASELINE}/msGeodesicMorphologyIterativeDecompositionImageFilter_convMap_2.tif
  ${TEMP}/msGeodesicMorphologyIterativeDecompositionImageFilter_convMap_2.tif
  ${BASELINE}/msGeodesicMorphologyIterativeDecompositionImageFilter_leveling_1.tif
  ${TEMP}/msGeodesicMorphologyIterativeDecompositionImageFilter_leveling_1.tif
  ${BASELINE}/msGeodesicMorphologyIterativeDecompositionImageFilter_leveling_2.tif
  ${TEMP}/msGeodesicMorphologyIterativeDecompositionImageFilter_leveling_2.tif
  otbGeodesicMorphologyIterativeDecompositionImageFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/msGeodesicMorphologyIterativeDecompositionImageFilter
  tif
  2
  3
  2
  )

otb_add_test(NAME msTuGeodesicMorphologyIterativeDecompositionImageFilterNew COMMAND otbMorphologicalProfilesTestDriver
  otbGeodesicMorphologyIterativeDecompositionImageFilterNew)

otb_add_test(NAME msTuGeodesicMorphologyLevelingFilterNew COMMAND otbMorphologicalProfilesTestDriver
  otbGeodesicMorphologyLevelingFilterNew)

otb_add_test(NAME bfTvClosingOpeningFilter COMMAND otbMorphologicalProfilesTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/bfFiltreClosingOpening_1.tif
  ${TEMP}/bfFiltreClosingOpening_1.tif
  otbClosingOpeningMorphologicalFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/bfFiltreClosingOpening_1.tif
  1)

otb_add_test(NAME msTuProfileDerivativeToMultiScaleCharacteristicsFilterNew COMMAND otbMorphologicalProfilesTestDriver
  otbProfileDerivativeToMultiScaleCharacteristicsFilterNew)

otb_add_test(NAME msTvProfileToProfileDerivativeFilter COMMAND otbMorphologicalProfilesTestDriver
  --compare-n-images ${NOTOL} 4
  ${BASELINE}/msProfileToProfileDerivativeFilterOutput1.tif
  ${TEMP}/msProfileToProfileDerivativeFilterOutput1.tif
  ${BASELINE}/msProfileToProfileDerivativeFilterOutput2.tif
  ${TEMP}/msProfileToProfileDerivativeFilterOutput2.tif
  ${BASELINE}/msProfileToProfileDerivativeFilterOutput3.tif
  ${TEMP}/msProfileToProfileDerivativeFilterOutput3.tif
  ${BASELINE}/msProfileToProfileDerivativeFilterOutput4.tif
  ${TEMP}/msProfileToProfileDerivativeFilterOutput4.tif
  otbProfileToProfileDerivativeFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/msProfileToProfileDerivativeFilterOutput
  tif
  5
  1
  1
  )

otb_add_test(NAME bfTuOpeningClosingFilterNew COMMAND otbMorphologicalProfilesTestDriver
  otbOpeningClosingMorphologicalFilterNew )

otb_add_test(NAME msTuGeodesicMorphologyDecompositionImageFilterNew COMMAND otbMorphologicalProfilesTestDriver
  otbGeodesicMorphologyDecompositionImageFilterNew)

otb_add_test(NAME msTuMorphologicalOpeningProfileFilterNew COMMAND otbMorphologicalProfilesTestDriver
  otbMorphologicalOpeningProfileFilterNew)

otb_add_test(NAME msTvProfileDerivativeToMultiScaleCharacteristicsFilter COMMAND otbMorphologicalProfilesTestDriver
  --compare-n-images ${NOTOL} 2
  ${BASELINE}/msProfileDerivativeToMultiScaleCharacteristicsFilterOutput.tif
  ${TEMP}/msProfileDerivativeToMultiScaleCharacteristicsFilterOutput.tif
  ${BASELINE}/msProfileDerivativeToMultiScaleCharacteristicsFilterOutputCharacteristics.tif
  ${TEMP}/msProfileDerivativeToMultiScaleCharacteristicsFilterOutputCharacteristics.tif
  otbProfileDerivativeToMultiScaleCharacteristicsFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/msProfileDerivativeToMultiScaleCharacteristicsFilterOutput.tif
  ${TEMP}/msProfileDerivativeToMultiScaleCharacteristicsFilterOutputCharacteristics.tif
  5
  1
  1
  )

otb_add_test(NAME msTuMultiScaleConvexOrConcaveClassificationFilterNew COMMAND otbMorphologicalProfilesTestDriver
  otbMultiScaleConvexOrConcaveClassificationFilterNew)

otb_add_test(NAME bfTuClosingOpeningFilterNew COMMAND otbMorphologicalProfilesTestDriver
  otbClosingOpeningMorphologicalFilterNew )

otb_add_test(NAME bfTvOpeningClosingFilter COMMAND otbMorphologicalProfilesTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/bfFiltreOpeningClosing_1.tif
  ${TEMP}/bfFiltreOpeningClosing_1.tif
  otbOpeningClosingMorphologicalFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/bfFiltreOpeningClosing_1.tif
  1)

otb_add_test(NAME msTvMorphologicalClosingProfileFilter COMMAND otbMorphologicalProfilesTestDriver
  --compare-n-images ${NOTOL} 4
  ${BASELINE}/msMorphologicalClosingProfileFilterOutput1.tif
  ${TEMP}/msMorphologicalClosingProfileFilterOutput1.tif
  ${BASELINE}/msMorphologicalClosingProfileFilterOutput2.tif
  ${TEMP}/msMorphologicalClosingProfileFilterOutput2.tif
  ${BASELINE}/msMorphologicalClosingProfileFilterOutput3.tif
  ${TEMP}/msMorphologicalClosingProfileFilterOutput3.tif
  ${BASELINE}/msMorphologicalClosingProfileFilterOutput4.tif
  ${TEMP}/msMorphologicalClosingProfileFilterOutput4.tif
  otbMorphologicalClosingProfileFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/msMorphologicalClosingProfileFilterOutput
  tif
  4
  1
  1
  )

