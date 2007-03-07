 
IF( NOT OTB_DISABLE_CXX_TESTING )

# -------    ossim integration test   ----------

ADD_TEST(utOSSIMIntegrationTest utOSSIMIntegrationTest)

# -------       Fichiers sources CXX -----------------------------------


ADD_EXECUTABLE(utOSSIMIntegrationTest ossimIntegrationTest.cxx)
TARGET_LINK_LIBRARIES(utOSSIMIntegrationTest gdal ossim)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )
