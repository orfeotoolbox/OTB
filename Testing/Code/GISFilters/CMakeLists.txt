 
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
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



# -------       Fichiers sources CXX -----------------------------------
SET(GISFilters_SRCS
otbPostGISCreateTableTransactorNew.cxx
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

# ${TIFF_LIBRARY}

ADD_EXECUTABLE(otbGISFiltersTests otbGISFiltersTests.cxx ${GISFilters_SRCS})
TARGET_LINK_LIBRARIES(otbGISFiltersTests OTBCommon OTBGISFilters)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
