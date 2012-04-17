IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(OGR_TESTS ${CXX_TEST_PATH}/otbOGRTests)

SET(OGRCommon_SRC
otbOGRDataSourceWrapperNew.cxx
)

SET(OGRNon_UT_SRC
otbOGRDataSourceWrapperIO.cxx
)

find_package(Boost COMPONENTS filesystem unit_test_framework)
IF(Boost_FOUND)
    MESSAGE(STATUS "Boost_INCLUDE_DIR: ${Boost_INCLUDE_DIR}")
    MESSAGE(STATUS "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
    MESSAGE(STATUS "Boost_FILESYSTEM_LIBRARY: ${Boost_FILESYSTEM_LIBRARY}")
    MESSAGE(STATUS "Boost_UNIT_TEST_FRAMEWORK_LIBRARY: ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}")
    # MESSAGE(STATUS "Boost_DATE_TIME_LIBRARY: ${Boost_DATE_TIME_LIBRARY}")
ELSE(Boost_FOUND)
  MESSAGE(WARNING
    "Cannot build OTB/OGR UT without Boost.  Please set Boost_DIR.")
ENDIF(Boost_FOUND)


IF (Boost_UNIT_TEST_FRAMEWORK_FOUND)
  OTB_ADD_EXECUTABLE(otbOGRTests "${OGRCommon_SRC}" "OTBIO;OTBTesting")
  TARGET_LINK_LIBRARIES(otbOGRTests OTBOGRAdapters OTBTesting
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )


  # -------------  otb::OGRDataSourceWrapper ----------------------------
  ADD_TEST(coTuOGRDataSourceWrapperNew ${OGR_TESTS}
    otbOGRDataSourceWrapperNew
    )
ENDIF(Boost_UNIT_TEST_FRAMEWORK_FOUND)

OTB_ADD_EXECUTABLE(otbOGRTestsV "${OGRNon_UT_SRC}" "OTBIO;OTBTesting")
TARGET_LINK_LIBRARIES(otbOGRTestsV OTBOGRAdapters OTBTesting
  )
ADD_TEST(coTuOGRDataSourceWrapperIO ${OGR_TESTS_NON_UT}
  otbOGRDataSourceWrapperIO
  )

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
