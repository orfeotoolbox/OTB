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

#IF(OTB_USE_PQXX)
#  SET(OBIA_TESTS1 ${CXX_TEST_PATH}/otbOBIATests1)
#ENDIF(OTB_USE_PQXX)
#SET(OBIA_TESTS2 ${CXX_TEST_PATH}/otbOBIATests2)
SET(OBIA_TESTS3 ${CXX_TEST_PATH}/otbOBIATests3)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbOBIATests1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

IF(MOVED_FROM_COMMON)

# ----------------  PostGIS Interface ---------------------#
IF(OTB_USE_PQXX ) # fake test to wait the migration of some classes from
                                       # to OBIA

ADD_TEST(obTuPostGISConnectionImplementationNew ${OBIA_TESTS1}
  otbPostGISConnectionImplementationNew
)

#This test need a runing PostGIS server on the localhost with trusted potgres user and an ecisting testgis base
ADD_TEST(obTvPostGISConnectionImplementationConnectToDB ${OBIA_TESTS1}
        otbPostGISConnectionImplementationConnectToDB
        localhost
        orfeotoolbox_test
        orfeotoolbox_test_user
        Bidfeud0
)

ADD_TEST(obTvPostGISConnectionImplementationPerformTransaction ${OBIA_TESTS1}
        otbPostGISConnectionImplementationPerformTransaction
        localhost
        orfeotoolbox_test
        orfeotoolbox_test_user
        Bidfeud0
)


ADD_TEST(obTuGISTableNew ${OBIA_TESTS1}
  otbGISTableNew
)

ADD_TEST(obTuPostGISTableNew ${OBIA_TESTS1}
  otbPostGISTableNew
)

ADD_TEST(obTuVectorDataToGISTableFilterNew ${OBIA_TESTS1}
  otbVectorDataToGISTableFilterNew
)

ADD_TEST(obTuGISTableToGISTableFilterNew ${OBIA_TESTS1}
  otbGISTableToGISTableFilterNew
)


ADD_TEST(obTuLabelMapToGISTableFilterNew ${OBIA_TESTS1}
  otbLabelMapToGISTableFilterNew
)

ADD_TEST(obTuGISTableToLabelMapFilterNew ${OBIA_TESTS1}
  otbGISTableToLabelMapFilterNew
)

ADD_TEST(obTuGISTableToVectorDataFilterNew ${OBIA_TESTS1}
  otbGISTableToVectorDataFilterNew
)

ADD_TEST(obTvLabelMapToGISTableFilter ${OBIA_TESTS1}
  otbLabelMapToGISTableFilter
    ${INPUTDATA}/rcc8_mire1.png
    orfeotoolbox_test
    labelmaptogis_test_table
    orfeotoolbox_test_user
    Bidfeud0
)


#need gdal 1.6 for POSTGIS tables connection via OGR string
ADD_TEST(obTvGISTableToVectorDataFilter ${OBIA_TESTS1}
otbGISTableToVectorDataFilter
${TEMP}/gistabletovectordatafilter.shp
orfeotoolbox_test
labelmaptogis_test_table
orfeotoolbox_test_user
Bidfeud0
)


ENDIF(OTB_USE_PQXX)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbOBIATests2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# ----------------  AttributesMapLabelObject ---------------------#


ADD_TEST(obTuAttributesMapLabelObjectNew ${OBIA_TESTS2}
otbAttributesMapLabelObjectNew
)


ADD_TEST(obTuLabelObjectToPolygonFunctorNew ${OBIA_TESTS2}
otbLabelObjectToPolygonFunctorNew
)

ADD_TEST(obTuRadiometricAttributesLabelMapFilterNew ${OBIA_TESTS2}
otbRadiometricAttributesLabelMapFilterNew
)

ADD_TEST(obTuShapeAttributesLabelMapFilterNew ${OBIA_TESTS2}
otbShapeAttributesLabelMapFilterNew
)

ADD_TEST(obTuStatisticsAttributesLabelMapFilterNew ${OBIA_TESTS2}
otbStatisticsAttributesLabelMapFilterNew
)

ADD_TEST(obTuAttributesMapOpeningLabelMapFilterNew ${OBIA_TESTS2}
otbAttributesMapOpeningLabelMapFilterNew
)

ADD_TEST(obTuLabelMapToVectorDataFilterNew ${OBIA_TESTS2}
otbLabelMapToVectorDataFilterNew
)

ADD_TEST(obTuVectorDataToLabelMapFilterNew ${OBIA_TESTS2}
otbVectorDataToLabelMapFilterNew
)

ADD_TEST(obTuLabelMapSourceNew ${OBIA_TESTS2}
otbLabelMapSourceNew
)

ADD_TEST(otbLabelObjectMapVectorizer ${OBIA_TESTS2}
        otbLabelObjectMapVectorizer
        ${INPUTDATA}/rcc8_mire1.png
        rcc8_mire1_label_vectorizer.gml
)

ADD_TEST(obTuUnaryFunctorWithIndexImageFilterNew ${OBIA_TESTS2}
otbUnaryFunctorWithIndexImageFilterNew
)

ADD_TEST(obTvUnaryFunctorWithIndexImageFilter ${OBIA_TESTS2}
     --compare-image ${NOTOL}
                     ${INPUTDATA}/poupees.tif
                     ${TEMP}/coTvUnaryFunctorWithIndexImageFilter.tif
otbUnaryFunctorWithIndexImageFilter
       ${INPUTDATA}/poupees.tif
       ${TEMP}/coTvUnaryFunctorWithIndexImageFilter.tif
)

ADD_TEST(obTvLabelMapToVectorDataFilter ${OBIA_TESTS2}
otbLabelMapToVectorDataFilter
        ${INPUTDATA}/rcc8_mire1.png
        ${TEMP}/rcc8_mire2_vectorizer.shp
)

ADD_TEST(obTvVectorDataToLabelMapFilter ${OBIA_TESTS2}
otbVectorDataToLabelMapFilter
        ${INPUTDATA}/vectorIOexample_gis_to_vec.shp
        ${TEMP}/vectordataToLabelMap.png
)

# Used to not compile those tests until the migration from Common
ENDIF(MOVED_FROM_COMMON)


# ------------------ OBIA_TESTS 3 -----------

ADD_TEST(obTuAttributesMapLabelObjectNew ${OBIA_TESTS3} 
	otbAttributesMapLabelObjectNew)

ADD_TEST(obTuAttributesMapLabelObjectWithClassLabelNew ${OBIA_TESTS3} 
	otbAttributesMapLabelObjectWithClassLabelNew)

ADD_TEST(obTuShapeAttributesLabelMapFilterNew ${OBIA_TESTS3} 
	otbShapeAttributesLabelMapFilterNew)

ADD_TEST(obTuStatisticsAttributesLabelMapFilterNew ${OBIA_TESTS3} 
	otbStatisticsAttributesLabelMapFilterNew)

ADD_TEST(obTuAttributesMapOpeningLabelMapFilterNew ${OBIA_TESTS3} 
	otbAttributesMapOpeningLabelMapFilterNew)

ADD_TEST(obTuLabelMapToSampleListFilterNew ${OBIA_TESTS3} 
	otbLabelMapToSampleListFilterNew)

ADD_TEST(obTvLabelMapToSampleListFilter ${OBIA_TESTS3} 
	otbLabelMapToSampleListFilter
	${OTB_DATA_ROOT}/Input/rcc8_mire1.png
	SHAPE::Flusser01  SHAPE::Flusser02  SHAPE::Flusser03 SHAPE::Flusser04
        SHAPE::Flusser05  SHAPE::Flusser06  SHAPE::Flusser07 SHAPE::Flusser08
	 SHAPE::Flusser09 SHAPE::Flusser10  SHAPE::Flusser11)

ADD_TEST(obTuLabelMapWithClassLabelToLabeledSampleListFilterNew ${OBIA_TESTS3} 
	otbLabelMapWithClassLabelToLabeledSampleListFilterNew)

ADD_TEST(obTvLabelMapWithClassLabelToLabeledSampleListFilter ${OBIA_TESTS3} 
	otbLabelMapWithClassLabelToLabeledSampleListFilter
	${OTB_DATA_ROOT}/Input/rcc8_mire1.png
	SHAPE::Flusser01  SHAPE::Flusser02  SHAPE::Flusser03 SHAPE::Flusser04
        SHAPE::Flusser05  SHAPE::Flusser06  SHAPE::Flusser07 SHAPE::Flusser08
	 SHAPE::Flusser09 SHAPE::Flusser10  SHAPE::Flusser11)


ADD_TEST(obTuImageToLabelMapWithAttributesFilterNew ${OBIA_TESTS3} 
	otbImageToLabelMapWithAttributesFilterNew)

ADD_TEST(obTvImageToLabelMapWithAttributesFilter ${OBIA_TESTS3} 
	otbImageToLabelMapWithAttributesFilter
        ${INPUTDATA}/calanques.tif
        ${INPUTDATA}/cala_labelled.tif
)


# -------       Fichiers sources CXX -----------------------------------
# To be removed when migrate classes from common
IF(MOVED_FROM_COMMON)

IF(OTB_USE_PQXX)
SET(BasicOBIA_SRCS1
otbPostGISConnectionImplementationNew.cxx
otbPostGISConnectionImplementationConnectToDB.cxx
otbPostGISConnectionImplementationPerformTransaction.cxx
otbGISTableNew.cxx
otbPostGISTableNew.cxx
otbVectorDataToGISTableFilterNew.cxx
 otbVectorDataToGISTableFilter.cxx
otbGISTableToGISTableFilterNew.cxx
otbLabelMapToGISTableFilterNew.cxx
otbGISTableToLabelMapFilterNew.cxx
otbGISTableToVectorDataFilterNew.cxx
otbLabelMapToGISTableFilter.cxx
otbGISTableToVectorDataFilter.cxx
)
ENDIF(OTB_USE_PQXX)

SET(BasicOBIA_SRCS2
otbAttributesMapLabelObjectNew.cxx
otbLabelObjectToPolygonFunctorNew.cxx
otbRadiometricAttributesLabelMapFilterNew.cxx
otbShapeAttributesLabelMapFilterNew.cxx
otbStatisticsAttributesLabelMapFilterNew.cxx
otbAttributesMapOpeningLabelMapFilterNew.cxx
otbLabelMapToVectorDataFilterNew.cxx
otbLabelMapToVectorDataFilter.cxx
otbLabelMapSourceNew.cxx
otbVectorDataToLabelMapFilterNew.cxx
otbVectorDataToLabelMapFilter.cxx
otbUnaryFunctorWithIndexImageFilterNew.cxx
otbUnaryFunctorWithIndexImageFilter.cxx
otbLabelObjectMapVectorizer.cxx
)
# Have to be removed when migrate classes from common
ENDIF(MOVED_FROM_COMMON)


SET(BasicOBIA_SRCS3
otbAttributesMapLabelObjectNew.cxx
otbAttributesMapLabelObjectWithClassLabelNew.cxx
otbShapeAttributesLabelMapFilterNew.cxx
otbStatisticsAttributesLabelMapFilterNew.cxx
otbAttributesMapOpeningLabelMapFilterNew.cxx
otbLabelMapToSampleListFilterNew.cxx
otbLabelMapToSampleListFilter.cxx
otbLabelMapWithClassLabelToLabeledSampleListFilterNew.cxx
otbLabelMapWithClassLabelToLabeledSampleListFilter.cxx
otbImageToLabelMapWithAttributesFilterNew.cxx
otbImageToLabelMapWithAttributesFilter.cxx
)




#IF(OTB_USE_PQXX)
  #TODO this line should be refined when we will like to have this capability with windows
#OTB_ADD_EXECUTABLE(otbOBIATests1 "${BasicOBIA_SRCS1}" "OTBIO;OTBTesting")
#TARGET_LINK_LIBRARIES(otbOBIATests1 OTBIO OTBOBIA OTBTesting OTBGeospatialAnalysis pq pqxx)
#ENDIF(OTB_USE_PQXX)

#ADD_EXECUTABLE(otbOBIATests2 otbOBIATests2.cxx ${BasicOBIA_SRCS2})
#TARGET_LINK_LIBRARIES(otbOBIATests2 OTBIO  OTBOBIA OTBTesting)

ADD_EXECUTABLE(otbOBIATests3 otbOBIATests3.cxx ${BasicOBIA_SRCS3})
TARGET_LINK_LIBRARIES(otbOBIATests3 OTBIO OTBOBIA  OTBTesting)


ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
