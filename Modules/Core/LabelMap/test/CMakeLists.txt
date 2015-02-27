otb_module_test()

set(OTBLabelMapTests
otbLabelMapTestDriver.cxx
otbAttributesMapLabelObjectWithClassLabelNew.cxx
otbLabelObjectMapVectorizer.cxx
otbAttributesMapOpeningLabelMapFilterNew.cxx
otbLabelImageToLabelMapWithAdjacencyFilter.cxx
otbStatisticsAttributesLabelMapFilterNew.cxx
otbLabelObjectToPolygonFunctorNew.cxx
otbLabelMapWithClassLabelToLabeledSampleListFilterNew.cxx
otbAttributesMapLabelObjectNew.cxx
otbLabelMapToSampleListFilterNew.cxx
otbLabelMapSourceNew.cxx
otbLabelMapToAttributeImageFilterNew.cxx
otbImageToLabelMapWithAttributesFilter.cxx
otbImageToLabelMapWithAttributesFilterNew.cxx
otbKMeansAttributesLabelMapFilter.cxx
otbLabelMapToSampleListFilter.cxx
otbLabelMapWithClassLabelToLabeledSampleListFilter.cxx
otbMinMaxAttributesLabelMapFilter.cxx
otbNormalizeAttributesLabelMapFilter.cxx
otbShapeAttributesLabelMapFilterNew.cxx
otbBandsStatisticsAttributesLabelMapFilter.cxx
)

add_executable(otbLabelMapTestDriver ${OTBLabelMapTests})
target_link_libraries(otbLabelMapTestDriver ${OTBLabelMap-Test_LIBRARIES})
otb_module_target_label(otbLabelMapTestDriver)

# Tests Declaration

otb_add_test(NAME obTuAttributesMapLabelObjectWithClassLabelNew COMMAND otbLabelMapTestDriver
  otbAttributesMapLabelObjectWithClassLabelNew)

otb_add_test(NAME obTvLabelObjectMapVectorizer COMMAND otbLabelMapTestDriver
  otbLabelObjectMapVectorizer
  ${INPUTDATA}/rcc8_mire1.png
  rcc8_mire1_label_vectorizer.gml)

otb_add_test(NAME obTuAttributesMapOpeningLabelMapFilterNew COMMAND otbLabelMapTestDriver
  otbAttributesMapOpeningLabelMapFilterNew)

otb_add_test(NAME obTuLabelImageToLabelMapWithAdjacencyFilterNew COMMAND otbLabelMapTestDriver
  otbLabelImageToLabelMapWithAdjacencyFilterNew)

otb_add_test(NAME obTvLabelImageToLabelMapWithAdjacencyFilter COMMAND otbLabelMapTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/obTvLabelImageToLabelMapWithAdjacencyFilterOutput.txt
  ${TEMP}/obTvLabelImageToLabelMapWithAdjacencyFilterOutput.txt
  otbLabelImageToLabelMapWithAdjacencyFilter
  ${INPUTDATA}/simpleLabelImage.tif
  ${TEMP}/obTvLabelImageToLabelMapWithAdjacencyFilterOutput.txt
  )

otb_add_test(NAME obTuStatisticsAttributesLabelMapFilterNew COMMAND otbLabelMapTestDriver
  otbStatisticsAttributesLabelMapFilterNew)

otb_add_test(NAME obTuLabelObjectToPolygonFunctorNew COMMAND otbLabelMapTestDriver
  otbLabelObjectToPolygonFunctorNew)

otb_add_test(NAME obTuLabelMapWithClassLabelToLabeledSampleListFilterNew COMMAND otbLabelMapTestDriver
  otbLabelMapWithClassLabelToLabeledSampleListFilterNew)

otb_add_test(NAME obTuAttributesMapLabelObjectNew COMMAND otbLabelMapTestDriver
  otbAttributesMapLabelObjectNew)

otb_add_test(NAME obTuLabelMapToSampleListFilterNew COMMAND otbLabelMapTestDriver
  otbLabelMapToSampleListFilterNew)

otb_add_test(NAME obTuLabelMapSourceNew COMMAND otbLabelMapTestDriver
  otbLabelMapSourceNew)

otb_add_test(NAME obTuLabelMapToAttributeImageFilterNew COMMAND otbLabelMapTestDriver
  otbLabelMapToAttributeImageFilterNew)

otb_add_test(NAME obTvImageToLabelMapWithAttributesFilter COMMAND otbLabelMapTestDriver
  otbImageToLabelMapWithAttributesFilter
  ${INPUTDATA}/maur.tif
  ${INPUTDATA}/maur_labelled.tif)
otb_add_test(NAME obTuImageToLabelMapWithAttributesFilterNew COMMAND otbLabelMapTestDriver
  otbImageToLabelMapWithAttributesFilterNew)
otb_add_test(NAME obTvKMeansAttributesLabelMapFilter COMMAND otbLabelMapTestDriver
  otbKMeansAttributesLabelMapFilter
  ${INPUTDATA}/maur.tif
  ${INPUTDATA}/maur_labelled.tif
  ${TEMP}/obTvKMeansAttributesLabelMapFilter.txt)
otb_add_test(NAME obTuKMeansAttributesLabelMapFilterNew COMMAND otbLabelMapTestDriver
  otbKMeansAttributesLabelMapFilterNew)
otb_add_test(NAME obTvLabelMapToSampleListFilter COMMAND otbLabelMapTestDriver
  otbLabelMapToSampleListFilter
  ${OTB_DATA_ROOT}/Input/rcc8_mire1.png
  SHAPE::Flusser01 SHAPE::Flusser02 SHAPE::Flusser03 SHAPE::Flusser04
  SHAPE::Flusser05 SHAPE::Flusser06 SHAPE::Flusser07 SHAPE::Flusser08
  SHAPE::Flusser09 SHAPE::Flusser10 SHAPE::Flusser11)
otb_add_test(NAME obTvLabelMapWithClassLabelToLabeledSampleListFilter COMMAND otbLabelMapTestDriver
  otbLabelMapWithClassLabelToLabeledSampleListFilter
  ${OTB_DATA_ROOT}/Input/rcc8_mire1.png
  SHAPE::Flusser01  SHAPE::Flusser02  SHAPE::Flusser03 SHAPE::Flusser04
  SHAPE::Flusser05  SHAPE::Flusser06  SHAPE::Flusser07 SHAPE::Flusser08
  SHAPE::Flusser09 SHAPE::Flusser10  SHAPE::Flusser11)
otb_add_test(NAME obTuMinMaxAttributesLabelMapFilterNew COMMAND otbLabelMapTestDriver
  otbMinMaxAttributesLabelMapFilterNew)
otb_add_test(NAME obTvMinMaxAttributesLabelMapFilter COMMAND otbLabelMapTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/obTvMinMaxAttributesLabelMapFilter.txt
  ${TEMP}/obTvMinMaxAttributesLabelMapFilter.txt
  otbMinMaxAttributesLabelMapFilter
  ${INPUTDATA}/maur.tif
  ${INPUTDATA}/maur_labelled.tif
  ${TEMP}/obTvMinMaxAttributesLabelMapFilter.txt)
otb_add_test(NAME obTuNormalizeAttributesLabelMapFilterNew COMMAND otbLabelMapTestDriver
  otbNormalizeAttributesLabelMapFilterNew)
otb_add_test(NAME obTvNormalizeAttributesLabelMapFilter COMMAND otbLabelMapTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/obTvNormalizeAttributesLabelMapFilter.txt
  ${TEMP}/obTvNormalizeAttributesLabelMapFilter.txt
  otbNormalizeAttributesLabelMapFilter
  ${INPUTDATA}/maur.tif
  ${INPUTDATA}/maur_labelled.tif
  ${TEMP}/obTvNormalizeAttributesLabelMapFilter.txt)
otb_add_test(NAME obTuShapeAttributesLabelMapFilterNew COMMAND otbLabelMapTestDriver
  otbShapeAttributesLabelMapFilterNew)
otb_add_test(NAME obTuBandsStatisticsAttributesLabelMapFilterNew COMMAND otbLabelMapTestDriver
  otbBandsStatisticsAttributesLabelMapFilterNew)
otb_add_test(NAME obTvBandsStatisticsAttributesLabelMapFilter COMMAND otbLabelMapTestDriver
  otbBandsStatisticsAttributesLabelMapFilter
  ${INPUTDATA}/maur.tif
  ${INPUTDATA}/maur_labelled.tif
  ${TEMP}/obTvBandsStatisticsAttributesLabelMapFilter.txt)
