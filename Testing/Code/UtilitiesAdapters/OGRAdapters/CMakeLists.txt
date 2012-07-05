IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(OGR_TESTS ${CXX_TEST_PATH}/otbOGRTests)

SET(OGRCommon_SRC
otbOGRDataSourceWrapperNew.cxx
)

SET(OGRNon_UT_SRC
otbOGRDataSourceWrapperIO.cxx
)

# unit_test_framework is not available in internal boost
IF(OTB_USE_EXTERNAL_BOOST)
  find_package(Boost COMPONENTS unit_test_framework)
  IF(Boost_FOUND)
    MESSAGE(STATUS "Boost_INCLUDE_DIR: ${Boost_INCLUDE_DIR}")
    MESSAGE(STATUS "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
    # MESSAGE(STATUS "Boost_FILESYSTEM_LIBRARY: ${Boost_FILESYSTEM_LIBRARY}")
    MESSAGE(STATUS "Boost_UNIT_TEST_FRAMEWORK_LIBRARY: ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}")
    # MESSAGE(STATUS "Boost_DATE_TIME_LIBRARY: ${Boost_DATE_TIME_LIBRARY}")
  ELSE(Boost_FOUND)
    MESSAGE(STATUS
      "Cannot build OTB/OGR UT without Boost.  Please set Boost_DIR.")
  ENDIF(Boost_FOUND)


  IF (Boost_UNIT_TEST_FRAMEWORK_FOUND)
    OTB_ADD_EXECUTABLE(otbOGRTests "${OGRCommon_SRC}" "OTBIO;OTBTesting")
    TARGET_LINK_LIBRARIES(otbOGRTests OTBOGRAdapters OTBTesting
      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
      )


    # -------------  otb::OGRDataSourceWrapper ----------------------------
    ADD_TEST(NAME coTuOGRDataSourceWrapperNew 
             COMMAND ${OGR_TESTS} coTuOGRDataSourceWrapperNew
             WORKING_DIRECTORY ${TEMP})
             
  ENDIF(Boost_UNIT_TEST_FRAMEWORK_FOUND)
ELSE(OTB_USE_EXTERNAL_BOOST)
  MESSAGE(STATUS
    "Cannot build OTB/OGR UT without Boost. Please install/enable EXTERNAL BOOST.")
ENDIF(OTB_USE_EXTERNAL_BOOST)

OTB_ADD_EXECUTABLE(otbOGRTestsV "${OGRNon_UT_SRC}" "OTBIO;OTBTesting")
TARGET_LINK_LIBRARIES(otbOGRTestsV OTBOGRAdapters OTBTesting
  )
ADD_TEST(coTuOGRDataSourceWrapperIO ${CXX_TEST_PATH}/otbOGRTestsV ${INPUTDATA}/ToulousePoints-examples.shp )

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
