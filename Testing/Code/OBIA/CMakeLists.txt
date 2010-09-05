IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Remote sensing images (large images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(LARGEINPUT ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

#Tolerance sur diff pixel image
SET(NOTOL 0.0)
SET(EPSILON_9 0.000000001)

#Tolerance sur la difference des valeurs numeriques dans le --compare-ascii
SET(EPSILON_3 0.001)

SET(OBIA_TESTS1 ${CXX_TEST_PATH}/otbOBIATests1)
SET(OBIA_TESTS2 ${CXX_TEST_PATH}/otbOBIATests2)



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
        ${INPUTDATA}/calanques.tif
        ${INPUTDATA}/cala_labelled.tif)

ADD_TEST(obTuLabelMapSourceNew ${OBIA_TESTS1}
    otbLabelMapSourceNew)

ADD_TEST(obTvLabelMapToVectorDataFilter ${OBIA_TESTS1}
    otbLabelMapToVectorDataFilter
        ${INPUTDATA}/rcc8_mire1.png
        ${TEMP}/rcc8_mire2_vectorizer.shp)

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
otbLabelMapToVectorDataFilterNew
)

ADD_TEST(obTvLabelMapToVectorDataFilter ${OBIA_TESTS1}
otbLabelMapToVectorDataFilter
        ${INPUTDATA}/rcc8_mire1.png
        ${TEMP}/rcc8_mire2_vectorizer.shp
)

ADD_TEST(obTuLabelMapWithClassLabelToLabeledSampleListFilterNew ${OBIA_TESTS1}
    otbLabelMapWithClassLabelToLabeledSampleListFilterNew)

ADD_TEST(obTvLabelMapWithClassLabelToLabeledSampleListFilter ${OBIA_TESTS1}
    otbLabelMapWithClassLabelToLabeledSampleListFilter
    ${OTB_DATA_ROOT}/Input/rcc8_mire1.png
    SHAPE::Flusser01  SHAPE::Flusser02  SHAPE::Flusser03 SHAPE::Flusser04
    SHAPE::Flusser05  SHAPE::Flusser06  SHAPE::Flusser07 SHAPE::Flusser08
    SHAPE::Flusser09 SHAPE::Flusser10  SHAPE::Flusser11)

ADD_TEST(otbLabelObjectMapVectorizer ${OBIA_TESTS1}
        otbLabelObjectMapVectorizer
        ${INPUTDATA}/rcc8_mire1.png
        rcc8_mire1_label_vectorizer.gml)

ADD_TEST(obTuLabelObjectToPolygonFunctorNew ${OBIA_TESTS1}
otbLabelObjectToPolygonFunctorNew
)

ADD_TEST(obTuRadiometricAttributesLabelMapFilterNew ${OBIA_TESTS1}
otbRadiometricAttributesLabelMapFilterNew
)

ADD_TEST(obTuShapeAttributesLabelMapFilterNew ${OBIA_TESTS1}
	otbShapeAttributesLabelMapFilterNew)

ADD_TEST(obTuStatisticsAttributesLabelMapFilterNew ${OBIA_TESTS1}
	otbStatisticsAttributesLabelMapFilterNew)

ADD_TEST(obTuVectorDataToLabelMapFilterNew ${OBIA_TESTS1}
otbVectorDataToLabelMapFilterNew
)

ADD_TEST(obTvVectorDataToLabelMapFilter ${OBIA_TESTS1}
otbVectorDataToLabelMapFilter
        ${INPUTDATA}/vectorIOexample_gis_to_vec.shp
        ${TEMP}/vectordataToLabelMap.png
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
otbRadiometricAttributesLabelMapFilterNew.cxx
otbShapeAttributesLabelMapFilterNew.cxx
otbStatisticsAttributesLabelMapFilterNew.cxx
otbVectorDataToLabelMapFilter.cxx
otbVectorDataToLabelMapFilterNew.cxx
)

IF(OTB_USE_PQXX)
SET(BasicOBIA_SRCS2
otbLabelMapToGISTableFilterNew.cxx
otbGISTableToLabelMapFilterNew.cxx
otbLabelMapToGISTableFilter.cxx
)
ENDIF(OTB_USE_PQXX)

ADD_EXECUTABLE(otbOBIATests1 otbOBIATests1.cxx ${BasicOBIA_SRCS1})
TARGET_LINK_LIBRARIES(otbOBIATests1 OTBIO OTBOBIA  OTBTesting)

IF(OTB_USE_PQXX)
ADD_EXECUTABLE(otbOBIATests2 otbOBIATests2.cxx ${BasicOBIA_SRCS2})
TARGET_LINK_LIBRARIES(otbOBIATests2 OTBIO OTBOBIA OTBTesting OTBGeospatialAnalysis pq pqxx)
ENDIF(OTB_USE_PQXX)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
