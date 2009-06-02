 
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(EXAMPLESDATA ${OTB_DATA_ROOT}/Examples)
IF(OTB_DATA_USE_LARGEINPUT)
  SET(LARGEINPUTDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)

#Tolerance sur la difference des valeurs numeriques dans le --compare-ascii
SET(EPS 0.001)

SET(GISFILTERS_TESTS ${CXX_TEST_PATH}/otbGISFiltersTests)

# -------------  Transactors ----------------------------

ADD_TEST(gfTuPostGISCreateTableTransactorNew ${GISFILTERS_TESTS}
     otbPostGISCreateTableTransactorNew
	)

ADD_TEST(gfTuPostGISCreateTableTransactorAccessors ${GISFILTERS_TESTS}
     otbPostGISCreateTableTransactorAccessors
	)

ADD_TEST(gfTuPostGISCreateTableTransactorCreate ${GISFILTERS_TESTS}
  otbPostGISCreateTableTransactorCreate
  localhost
  testgis
  postgres
	)

# -------       CXX source files -----------------------------------
SET(GISFilters_SRCS
otbPostGISCreateTableTransactorNew.cxx
otbPostGISCreateTableTransactorAccessors.cxx
otbPostGISCreateTableTransactorCreate.cxx
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

# ${TIFF_LIBRARY}

ADD_EXECUTABLE(otbGISFiltersTests otbGISFiltersTests.cxx ${GISFilters_SRCS})
TARGET_LINK_LIBRARIES(otbGISFiltersTests OTBGISFilters OTBIO OTBCommon pq pqxx)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
