 
IF( NOT OTB_DISABLE_CXX_TESTING )

# -------    ossim integration test   ----------

ADD_TEST(utTuOSSIMIntegrationTest ${CXX_TEST_PATH}/utOSSIMIntegrationTest)

# -------       Fichiers sources CXX -----------------------------------
INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(utOSSIMIntegrationTest ossimIntegrationTest.cxx)
TARGET_LINK_LIBRARIES(utOSSIMIntegrationTest gdal ossim)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )
