IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE       ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA      ${OTB_DATA_ROOT}/Input)
SET(TEMP           ${OTBTesting_BINARY_DIR}/Temporary)

#Remote sensing images (large images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(LARGEINPUT ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

#Tolerance sur diff pixel image
SET(NOTOL 0.0)
SET(EPSILON_9 0.000000001)

#Tolerance sur la difference des valeurs numeriques dans le --compare-ascii
SET(EPSILON_3 0.001)

# Common generic tests
SET(OBIA_TESTS1 ${CXX_TEST_PATH}/otbOBIATests1)
# Tests involving PostGIS interface
SET(OBIA_TESTS2 ${CXX_TEST_PATH}/otbOBIATests2)
# Tests involving learning
SET(OBIA_TESTS3 ${CXX_TEST_PATH}/otbOBIATests3)

# OBIATests1
ADD_TEST(obTuAttributesMapLabelObjectNew ${OBIA_TESTS1}
        otbAttributesMapLabelObjectNew)

ADD_TEST(obTuAttributesMapLabelObjectWithClassLabelNew ${OBIA_TESTS1}
        otbAttributesMapLabelObjectWithClassLabelNew)

ADD_TEST(obTuAttributesMapOpeningLabelMapFilterNew ${OBIA_TESTS1}
    otbAttributesMapOpeningLabelMapFilterNew)

ADD_TEST(obTuImageToLabelMapWithAttributesFilterNew ${OBIA_TESTS1}
    otbImageToLabelMapWithAttributesFilterNew)

ADD_TEST(obTvImageToLabelMapWithAttributesFilter ${OBIA_TESTS1}
    otbImageToLabelMapWithAttributesFilter
    ${INPUTDATA}/maur.tif
    ${INPUTDATA}/maur_labelled.tif)

ADD_TEST(obTuLabelMapSourceNew ${OBIA_TESTS1}
    otbLabelMapSourceNew)
ADD_TEST(obTvVectorDataToLabelMapFilter ${OBIA_TESTS1}
    otbVectorDataToLabelMapFilter
    ${INPUTDATA}/vectorIOexample_gis_to_vec.shp
    ${TEMP}/vectordataToLabelMap.png)

ADD_TEST(obTuLabelMapToSampleListFilterNew ${OBIA_TESTS1}
    otbLabelMapToSampleListFilterNew)
    
ADD_TEST(obTvLabelMapToSampleListFilter ${OBIA_TESTS1}
    otbLabelMapToSampleListFilter
    ${OTB_DATA_ROOT}/Input/rcc8_mire1.png
    SHAPE::Flusser01 SHAPE::Flusser02 SHAPE::Flusser03 SHAPE::Flusser04
    SHAPE::Flusser05 SHAPE::Flusser06 SHAPE::Flusser07 SHAPE::Flusser08
    SHAPE::Flusser09 SHAPE::Flusser10 SHAPE::Flusser11)

ADD_TEST(obTuLabelMapToVectorDataFilterNew ${OBIA_TESTS1}
    otbLabelMapToVectorDataFilterNew)

ADD_TEST(obTvLabelMapToVectorDataFilter ${OBIA_TESTS1}
   --compare-ogr ${NOTOL}
   ${BASELINE_FILES}/obTvLabelMapToVectorDataFilter.shp
   ${TEMP}/obTvLabelMapToVectorDataFilter.shp
    otbLabelMapToVectorDataFilter
    ${INPUTDATA}/rcc8_mire1.png
    ${TEMP}/obTvLabelMapToVectorDataFilter.shp)


ADD_TEST(obTvLabelMapToVectorDataFilter2 ${OBIA_TESTS1}
   --compare-ogr ${NOTOL}
   ${BASELINE_FILES}/obTvLabelMapToVectorDataFilter.shp
   ${TEMP}/obTvLabelMapToVectorDataFilter2.shp
    otbLabelMapToVectorDataFilter
    ${INPUTDATA}/rcc8_mire5.png
    ${TEMP}/obTvLabelMapToVectorDataFilter2.shp)


ADD_TEST(obTuLabelMapWithClassLabelToLabeledSampleListFilterNew ${OBIA_TESTS1}
    otbLabelMapWithClassLabelToLabeledSampleListFilterNew)

ADD_TEST(obTvLabelMapWithClassLabelToLabeledSampleListFilter ${OBIA_TESTS1}
    otbLabelMapWithClassLabelToLabeledSampleListFilter
    ${OTB_DATA_ROOT}/Input/rcc8_mire1.png
    SHAPE::Flusser01  SHAPE::Flusser02  SHAPE::Flusser03 SHAPE::Flusser04
    SHAPE::Flusser05  SHAPE::Flusser06  SHAPE::Flusser07 SHAPE::Flusser08
    SHAPE::Flusser09 SHAPE::Flusser10  SHAPE::Flusser11)

ADD_TEST(obTvLabelObjectMapVectorizer ${OBIA_TESTS1}
    otbLabelObjectMapVectorizer
    ${INPUTDATA}/rcc8_mire1.png
    rcc8_mire1_label_vectorizer.gml)

ADD_TEST(obTuLabelObjectToPolygonFunctorNew ${OBIA_TESTS1}
    otbLabelObjectToPolygonFunctorNew)

ADD_TEST(obTuMinMaxAttributesLabelMapFilterNew ${OBIA_TESTS1}
    otbMinMaxAttributesLabelMapFilterNew)

ADD_TEST(obTvMinMaxAttributesLabelMapFilter ${OBIA_TESTS1}
    otbMinMaxAttributesLabelMapFilter
    ${INPUTDATA}/maur.tif
    ${INPUTDATA}/maur_labelled.tif
    ${TEMP}/obTvMinMaxAttributesLabelMapFilter.txt)

ADD_TEST(obTuNormalizeAttributesLabelMapFilterNew ${OBIA_TESTS1}
    otbNormalizeAttributesLabelMapFilterNew)

ADD_TEST(obTvNormalizeAttributesLabelMapFilter ${OBIA_TESTS1}
    otbNormalizeAttributesLabelMapFilter
    ${INPUTDATA}/maur.tif
    ${INPUTDATA}/maur_labelled.tif
    ${TEMP}/obTvNormalizeAttributesLabelMapFilter.txt)

ADD_TEST(obTuKMeansAttributesLabelMapFilterNew ${OBIA_TESTS1}
    otbKMeansAttributesLabelMapFilterNew)

ADD_TEST(obTvKMeansAttributesLabelMapFilter ${OBIA_TESTS1}
    otbKMeansAttributesLabelMapFilter
    ${INPUTDATA}/maur.tif
    ${INPUTDATA}/maur_labelled.tif
    ${TEMP}/obTvKMeansAttributesLabelMapFilter.txt)

ADD_TEST(obTuBandsStatisticsAttributesLabelMapFilterNew ${OBIA_TESTS1}
    otbBandsStatisticsAttributesLabelMapFilterNew)
    
ADD_TEST(obTvBandsStatisticsAttributesLabelMapFilter ${OBIA_TESTS1}
    otbBandsStatisticsAttributesLabelMapFilter
    ${INPUTDATA}/maur.tif
    ${INPUTDATA}/maur_labelled.tif
    ${TEMP}/obTvBandsStatisticsAttributesLabelMapFilter.txt)
    
ADD_TEST(obTuShapeAttributesLabelMapFilterNew ${OBIA_TESTS1}
        otbShapeAttributesLabelMapFilterNew)

ADD_TEST(obTuStatisticsAttributesLabelMapFilterNew ${OBIA_TESTS1}
        otbStatisticsAttributesLabelMapFilterNew)

ADD_TEST(obTuVectorDataToLabelMapFilterNew ${OBIA_TESTS1}
    otbVectorDataToLabelMapFilterNew)

ADD_TEST(obTvVectorDataToLabelMapFilter ${OBIA_TESTS1}
    otbVectorDataToLabelMapFilter
    ${INPUTDATA}/vectorIOexample_gis_to_vec.shp
    ${TEMP}/vectordataToLabelMap.png)

ADD_TEST(obTuStreamingConnectedComponentSegmentationOBIAToVectorDataFilterNew ${OBIA_TESTS1}
    otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilterNew)

ADD_TEST(obTvStreamingConnectedComponentSegmentationOBIAToVectorDataFilter ${OBIA_TESTS1}
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

ADD_TEST(obTuLabelImageToLabelMapWithAdjacencyFilterNew ${OBIA_TESTS3}
  otbLabelImageToLabelMapWithAdjacencyFilterNew)

ADD_TEST(obTvLabelImageToLabelMapWithAdjacencyFilter ${OBIA_TESTS3}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/obTvLabelImageToLabelMapWithAdjacencyFilterOutput.txt
  ${TEMP}/obTvLabelImageToLabelMapWithAdjacencyFilterOutput.txt
  otbLabelImageToLabelMapWithAdjacencyFilter
  ${INPUTDATA}/simpleLabelImage.tif
  ${TEMP}/obTvLabelImageToLabelMapWithAdjacencyFilterOutput.txt
)

ADD_TEST(obTuHooverMatrixFilterNew ${OBIA_TESTS1}
    otbHooverMatrixFilterNew)

ADD_TEST(obTvHooverMatrixFilter ${OBIA_TESTS1}
    --compare-ascii ${NOTOL}
    ${BASELINE_FILES}/obTvHooverMatrixFilter.txt
    ${TEMP}/obTvHooverMatrixFilter.txt
    otbHooverMatrixFilter
    ${INPUTDATA}/Seg1InputForRCC8Graph.tif
    ${INPUTDATA}/Seg2InputForRCC8Graph.tif
    ${TEMP}/obTvHooverMatrixFilter.txt
    )

ADD_TEST(obTuHooverInstanceFilterNew ${OBIA_TESTS1}
    otbHooverInstanceFilterNew)

ADD_TEST(obTuLabelMapToAttributeImageFilterNew ${OBIA_TESTS1}
    otbLabelMapToAttributeImageFilterNew)

ADD_TEST(obTvHooverInstanceFilterToAttributeImage ${OBIA_TESTS1}
    --compare-image ${EPSILON_9}
    ${BASELINE}/obTvHooverInstanceFilterToAttributeImage.tif
    ${TEMP}/obTvHooverInstanceFilterToAttributeImage.tif
    otbHooverInstanceFilterToAttributeImage
    ${INPUTDATA}/maur_GT.tif
    ${INPUTDATA}/maur_labelled.tif
    ${TEMP}/obTvHooverInstanceFilterToAttributeImage.tif
    )

# OBIATests2 (need PQXX)
IF(OTB_USE_PQXX)
ADD_TEST(obTuLabelMapToGISTableFilterNew ${OBIA_TESTS2}
    otbLabelMapToGISTableFilterNew)

ADD_TEST(obTuGISTableToLabelMapFilterNew ${OBIA_TESTS2}
    otbGISTableToLabelMapFilterNew)

ADD_TEST(obTvLabelMapToGISTableFilter ${OBIA_TESTS2}
    otbLabelMapToGISTableFilter
    ${INPUTDATA}/rcc8_mire1.png
    orfeotoolbox_test
    labelmaptogis_test_table
    orfeotoolbox_test_user
    Bidfeud0)
ENDIF(OTB_USE_PQXX)


# OBIATests3 (need Learning)
ADD_TEST(obTuLabelMapSVMClassifierNew ${OBIA_TESTS3}
    otbLabelMapSVMClassifierNew)

ADD_TEST(obTvLabelMapSVMClassifier ${OBIA_TESTS3}
    otbLabelMapSVMClassifier
    ${INPUTDATA}/maur.tif
    ${INPUTDATA}/maur_labelled.tif
    ${TEMP}/obTvLabelMapSVMClassifierLabeledOutput.tif)


SET(BasicOBIA_SRCS1
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
otbHooverMatrixFilterNew.cxx
otbHooverMatrixFilter.cxx
otbHooverInstanceFilterNew.cxx
otbLabelMapToAttributeImageFilterNew.cxx
otbHooverInstanceFilterToAttributeImage.cxx
)

IF(OTB_USE_PQXX)
SET(BasicOBIA_SRCS2
otbLabelMapToGISTableFilterNew.cxx
otbGISTableToLabelMapFilterNew.cxx
otbLabelMapToGISTableFilter.cxx
)
ENDIF(OTB_USE_PQXX)

SET(BasicOBIA_SRCS3
otbLabelMapSVMClassifier.cxx
otbLabelImageToLabelMapWithAdjacencyFilter.cxx
)

ADD_EXECUTABLE(otbOBIATests1 otbOBIATests1.cxx ${BasicOBIA_SRCS1})
TARGET_LINK_LIBRARIES(otbOBIATests1 OTBIO OTBOBIA  OTBTesting)

IF(OTB_USE_PQXX)
ADD_EXECUTABLE(otbOBIATests2 otbOBIATests2.cxx ${BasicOBIA_SRCS2})
TARGET_LINK_LIBRARIES(otbOBIATests2 OTBIO OTBOBIA OTBTesting OTBGeospatialAnalysis pq pqxx)
ENDIF(OTB_USE_PQXX)

ADD_EXECUTABLE(otbOBIATests3 otbOBIATests3.cxx ${BasicOBIA_SRCS3})
TARGET_LINK_LIBRARIES(otbOBIATests3 OTBIO OTBOBIA OTBLearning OTBTesting)


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
