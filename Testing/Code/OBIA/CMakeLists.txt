if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

# Common generic tests
set(OBIA_TESTS1 ${CXX_TEST_PATH}/otbOBIATests1)
# Tests involving PostGIS interface
set(OBIA_TESTS2 ${CXX_TEST_PATH}/otbOBIATests2)
# Tests involving learning
set(OBIA_TESTS3 ${CXX_TEST_PATH}/otbOBIATests3)

# OBIATests1
add_test(obTuAttributesMapLabelObjectNew ${OBIA_TESTS1}
        otbAttributesMapLabelObjectNew)

add_test(obTuAttributesMapLabelObjectWithClassLabelNew ${OBIA_TESTS1}
        otbAttributesMapLabelObjectWithClassLabelNew)

add_test(obTuAttributesMapOpeningLabelMapFilterNew ${OBIA_TESTS1}
    otbAttributesMapOpeningLabelMapFilterNew)

add_test(obTuImageToLabelMapWithAttributesFilterNew ${OBIA_TESTS1}
    otbImageToLabelMapWithAttributesFilterNew)

add_test(obTvImageToLabelMapWithAttributesFilter ${OBIA_TESTS1}
    otbImageToLabelMapWithAttributesFilter
    ${INPUTDATA}/maur.tif
    ${INPUTDATA}/maur_labelled.tif)

add_test(obTuLabelMapSourceNew ${OBIA_TESTS1}
    otbLabelMapSourceNew)
add_test(obTvVectorDataToLabelMapFilter ${OBIA_TESTS1}
    otbVectorDataToLabelMapFilter
    ${INPUTDATA}/vectorIOexample_gis_to_vec.shp
    ${TEMP}/vectordataToLabelMap.png)

add_test(obTuLabelMapToSampleListFilterNew ${OBIA_TESTS1}
    otbLabelMapToSampleListFilterNew)

add_test(obTvLabelMapToSampleListFilter ${OBIA_TESTS1}
    otbLabelMapToSampleListFilter
    ${OTB_DATA_ROOT}/Input/rcc8_mire1.png
    SHAPE::Flusser01 SHAPE::Flusser02 SHAPE::Flusser03 SHAPE::Flusser04
    SHAPE::Flusser05 SHAPE::Flusser06 SHAPE::Flusser07 SHAPE::Flusser08
    SHAPE::Flusser09 SHAPE::Flusser10 SHAPE::Flusser11)

add_test(obTuLabelMapToVectorDataFilterNew ${OBIA_TESTS1}
    otbLabelMapToVectorDataFilterNew)

add_test(obTvLabelMapToVectorDataFilter ${OBIA_TESTS1}
   --compare-ogr ${NOTOL}
   ${BASELINE_FILES}/obTvLabelMapToVectorDataFilter.shp
   ${TEMP}/obTvLabelMapToVectorDataFilter.shp
    otbLabelMapToVectorDataFilter
    ${INPUTDATA}/rcc8_mire1.png
    ${TEMP}/obTvLabelMapToVectorDataFilter.shp)


add_test(obTvLabelMapToVectorDataFilter2 ${OBIA_TESTS1}
   --compare-ogr ${NOTOL}
   ${BASELINE_FILES}/obTvLabelMapToVectorDataFilter.shp
   ${TEMP}/obTvLabelMapToVectorDataFilter2.shp
    otbLabelMapToVectorDataFilter
    ${INPUTDATA}/rcc8_mire5.png
    ${TEMP}/obTvLabelMapToVectorDataFilter2.shp)


add_test(obTuLabelMapWithClassLabelToLabeledSampleListFilterNew ${OBIA_TESTS1}
    otbLabelMapWithClassLabelToLabeledSampleListFilterNew)

add_test(obTvLabelMapWithClassLabelToLabeledSampleListFilter ${OBIA_TESTS1}
    otbLabelMapWithClassLabelToLabeledSampleListFilter
    ${OTB_DATA_ROOT}/Input/rcc8_mire1.png
    SHAPE::Flusser01  SHAPE::Flusser02  SHAPE::Flusser03 SHAPE::Flusser04
    SHAPE::Flusser05  SHAPE::Flusser06  SHAPE::Flusser07 SHAPE::Flusser08
    SHAPE::Flusser09 SHAPE::Flusser10  SHAPE::Flusser11)

add_test(obTvLabelObjectMapVectorizer ${OBIA_TESTS1}
    otbLabelObjectMapVectorizer
    ${INPUTDATA}/rcc8_mire1.png
    rcc8_mire1_label_vectorizer.gml)

add_test(obTuLabelObjectToPolygonFunctorNew ${OBIA_TESTS1}
    otbLabelObjectToPolygonFunctorNew)

add_test(obTuMinMaxAttributesLabelMapFilterNew ${OBIA_TESTS1}
    otbMinMaxAttributesLabelMapFilterNew)

add_test(obTvMinMaxAttributesLabelMapFilter ${OBIA_TESTS1}
    --compare-ascii ${NOTOL}
    ${BASELINE_FILES}/obTvMinMaxAttributesLabelMapFilter.txt
    ${TEMP}/obTvMinMaxAttributesLabelMapFilter.txt
    otbMinMaxAttributesLabelMapFilter
    ${INPUTDATA}/maur.tif
    ${INPUTDATA}/maur_labelled.tif
    ${TEMP}/obTvMinMaxAttributesLabelMapFilter.txt)

add_test(obTuNormalizeAttributesLabelMapFilterNew ${OBIA_TESTS1}
    otbNormalizeAttributesLabelMapFilterNew)

add_test(obTvNormalizeAttributesLabelMapFilter ${OBIA_TESTS1}
  --compare-ascii ${NOTOL}
    ${BASELINE_FILES}/obTvNormalizeAttributesLabelMapFilter.txt
    ${TEMP}/obTvNormalizeAttributesLabelMapFilter.txt
    otbNormalizeAttributesLabelMapFilter
    ${INPUTDATA}/maur.tif
    ${INPUTDATA}/maur_labelled.tif
    ${TEMP}/obTvNormalizeAttributesLabelMapFilter.txt)

add_test(obTuKMeansAttributesLabelMapFilterNew ${OBIA_TESTS1}
    otbKMeansAttributesLabelMapFilterNew)

add_test(obTvKMeansAttributesLabelMapFilter ${OBIA_TESTS1}
    otbKMeansAttributesLabelMapFilter
    ${INPUTDATA}/maur.tif
    ${INPUTDATA}/maur_labelled.tif
    ${TEMP}/obTvKMeansAttributesLabelMapFilter.txt)

add_test(obTuBandsStatisticsAttributesLabelMapFilterNew ${OBIA_TESTS1}
    otbBandsStatisticsAttributesLabelMapFilterNew)

add_test(obTvBandsStatisticsAttributesLabelMapFilter ${OBIA_TESTS1}
    otbBandsStatisticsAttributesLabelMapFilter
    ${INPUTDATA}/maur.tif
    ${INPUTDATA}/maur_labelled.tif
    ${TEMP}/obTvBandsStatisticsAttributesLabelMapFilter.txt)

add_test(obTuShapeAttributesLabelMapFilterNew ${OBIA_TESTS1}
        otbShapeAttributesLabelMapFilterNew)

add_test(obTuStatisticsAttributesLabelMapFilterNew ${OBIA_TESTS1}
        otbStatisticsAttributesLabelMapFilterNew)

add_test(obTuVectorDataToLabelMapFilterNew ${OBIA_TESTS1}
    otbVectorDataToLabelMapFilterNew)

add_test(obTvVectorDataToLabelMapFilter ${OBIA_TESTS1}
    otbVectorDataToLabelMapFilter
    ${INPUTDATA}/vectorIOexample_gis_to_vec.shp
    ${TEMP}/vectordataToLabelMap.png)

add_test(obTuStreamingConnectedComponentSegmentationOBIAToVectorDataFilterNew ${OBIA_TESTS1}
    otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilterNew)

add_test(obTvStreamingConnectedComponentSegmentationOBIAToVectorDataFilter ${OBIA_TESTS1}
    --compare-ogr ${NOTOL}
     ${BASELINE_FILES}/obTvStreamingConnectedComponentSegmentationOBIAToVectorDataFilter.shp
     ${TEMP}/obTvStreamingConnectedComponentSegmentationOBIAToVectorDataFilter.shp
    otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter
      ${INPUTDATA}/ROI_QB_MUL_4.tif
      ${TEMP}/obTvStreamingConnectedComponentSegmentationOBIAToVectorDataFilter.shp
      "((b1>80) * intensity>95)"
      "distance<40"
      15
      "SHAPE_Elongation>8"
      5 )

#TODO JGT add validation test
add_test(obTuMeanShiftStreamingConnectedComponentSegmentationOBIAToVectorDataFilter ${OBIA_TESTS1}
      otbMeanShiftStreamingConnectedComponentSegmentationOBIAToVectorDataFilter
      ${INPUTDATA}/ROI_QB_MUL_4.tif
      ${TEMP}/obTuMeanShiftStreamingConnectedComponentSegmentationOBIAToVectorDataFilter.shp
      9
      10
      0.001
      ""
      "distance<10"
      20
      "SHAPE_Elongation>10"
      5)

#TODO JGT add validation test
add_test(obTuMeanShiftConnectedComponentSegmentationFilter ${OBIA_TESTS1}
      otbMeanShiftConnectedComponentSegmentationFilter
      ${INPUTDATA}/ROI_QB_MUL_4.tif
      ${TEMP}/obTuMeanShiftConnectedComponentSegmentationImage.png
      9
      9
      0.01
      ""
      "distance<0.2"
      50)


add_test(obTuLabelImageToLabelMapWithAdjacencyFilterNew ${OBIA_TESTS3}
  otbLabelImageToLabelMapWithAdjacencyFilterNew)

add_test(obTvLabelImageToLabelMapWithAdjacencyFilter ${OBIA_TESTS3}
 --compare-ascii ${NOTOL}
 ${BASELINE_FILES}/obTvLabelImageToLabelMapWithAdjacencyFilterOutput.txt
 ${TEMP}/obTvLabelImageToLabelMapWithAdjacencyFilterOutput.txt
 otbLabelImageToLabelMapWithAdjacencyFilter
 ${INPUTDATA}/simpleLabelImage.tif
 ${TEMP}/obTvLabelImageToLabelMapWithAdjacencyFilterOutput.txt
)

add_test(obTuLabelMapToAttributeImageFilterNew ${OBIA_TESTS1}
    otbLabelMapToAttributeImageFilterNew)


# OBIATests2 (need PQXX)
if(OTB_USE_PQXX)
add_test(obTuLabelMapToGISTableFilterNew ${OBIA_TESTS2}
    otbLabelMapToGISTableFilterNew)

add_test(obTuGISTableToLabelMapFilterNew ${OBIA_TESTS2}
    otbGISTableToLabelMapFilterNew)

add_test(obTvLabelMapToGISTableFilter ${OBIA_TESTS2}
    otbLabelMapToGISTableFilter
    ${INPUTDATA}/rcc8_mire1.png
    orfeotoolbox_test
    labelmaptogis_test_table
    orfeotoolbox_test_user
    Bidfeud0)
endif(OTB_USE_PQXX)


# OBIATests3 (need Learning)
add_test(obTuLabelMapSVMClassifierNew ${OBIA_TESTS3}
    otbLabelMapSVMClassifierNew)

add_test(obTvLabelMapSVMClassifier ${OBIA_TESTS3}
    otbLabelMapSVMClassifier
    ${INPUTDATA}/maur.tif
    ${INPUTDATA}/maur_labelled.tif
    ${TEMP}/obTvLabelMapSVMClassifierLabeledOutput.tif)


set(BasicOBIA_SRCS1
otbAttributesMapLabelObjectNew.cxx
otbAttributesMapLabelObjectWithClassLabelNew.cxx
otbAttributesMapOpeningLabelMapFilterNew.cxx
otbImageToLabelMapWithAttributesFilterNew.cxx
otbImageToLabelMapWithAttributesFilter.cxx
otbLabelMapSourceNew.cxx
otbLabelMapToSampleListFilter.cxx
otbLabelMapToSampleListFilterNew.cxx
otbLabelMapToVectorDataFilter.cxx
otbLabelMapToVectorDataFilterNew.cxx
otbLabelMapWithClassLabelToLabeledSampleListFilter.cxx
otbLabelMapWithClassLabelToLabeledSampleListFilterNew.cxx
otbLabelObjectMapVectorizer.cxx
otbLabelObjectToPolygonFunctorNew.cxx
otbMinMaxAttributesLabelMapFilter.cxx
otbNormalizeAttributesLabelMapFilter.cxx
otbKMeansAttributesLabelMapFilter.cxx
otbBandsStatisticsAttributesLabelMapFilter.cxx
otbShapeAttributesLabelMapFilterNew.cxx
otbStatisticsAttributesLabelMapFilterNew.cxx
otbVectorDataToLabelMapFilter.cxx
otbVectorDataToLabelMapFilterNew.cxx
otbStreamingConnectedComponentOBIATest.cxx
otbMeanShiftStreamingConnectedComponentOBIATest.cxx
otbMeanShiftConnectedComponentSegmentationFilterTest.cxx
otbLabelMapToAttributeImageFilterNew.cxx
)



if(OTB_USE_PQXX)
set(BasicOBIA_SRCS2
otbLabelMapToGISTableFilterNew.cxx
otbGISTableToLabelMapFilterNew.cxx
otbLabelMapToGISTableFilter.cxx
)
endif(OTB_USE_PQXX)

set(BasicOBIA_SRCS3
otbLabelMapSVMClassifier.cxx
otbLabelImageToLabelMapWithAdjacencyFilter.cxx
)

add_executable(otbOBIATests1 otbOBIATests1.cxx ${BasicOBIA_SRCS1})
target_link_libraries(otbOBIATests1 OTBIO OTBOBIA  OTBTesting OTBOGRAdapters)

if(OTB_USE_PQXX)
add_executable(otbOBIATests2 otbOBIATests2.cxx ${BasicOBIA_SRCS2})
target_link_libraries(otbOBIATests2 OTBIO OTBOBIA OTBTesting OTBGeospatialAnalysis pq pqxx)
endif(OTB_USE_PQXX)

add_executable(otbOBIATests3 otbOBIATests3.cxx ${BasicOBIA_SRCS3})
target_link_libraries(otbOBIATests3 OTBIO OTBOBIA OTBLearning OTBTesting)


endif( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
