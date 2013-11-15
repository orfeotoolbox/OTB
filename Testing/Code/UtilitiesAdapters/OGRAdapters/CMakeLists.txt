if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(OGR_TESTS ${CXX_TEST_PATH}/otbOGRTests)

set(OGRCommon_SRC
otbOGRDataSourceWrapperNew.cxx
)

set(OGRNon_UT_SRC
otbOGRDataSourceWrapperIO.cxx
)

# unit_test_framework is not available in internal boost
if(OTB_USE_EXTERNAL_BOOST)
  find_package(Boost COMPONENTS unit_test_framework)
  if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIR: ${Boost_INCLUDE_DIR}")
    message(STATUS "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
    # message(STATUS "Boost_FILESYSTEM_LIBRARY: ${Boost_FILESYSTEM_LIBRARY}")
    message(STATUS "Boost_UNIT_TEST_FRAMEWORK_LIBRARY: ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}")
    # message(STATUS "Boost_DATE_TIME_LIBRARY: ${Boost_DATE_TIME_LIBRARY}")
  else(Boost_FOUND)
    message(STATUS
      "Cannot build OTB/OGR UT without Boost.  Please set Boost_DIR.")
  endif(Boost_FOUND)


  if(Boost_UNIT_TEST_FRAMEWORK_FOUND)
    OTB_ADD_EXECUTABLE(otbOGRTests "${OGRCommon_SRC}" "OTBIO;OTBTesting")
    target_link_libraries(otbOGRTests OTBOGRAdapters OTBTesting
      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
      )


    # -------------  otb::OGRDataSourceWrapper ----------------------------
    add_test(NAME coTuOGRDataSourceWrapperNew
             COMMAND ${OGR_TESTS} coTuOGRDataSourceWrapperNew
             WORKING_DIRECTORY ${TEMP})

  endif(Boost_UNIT_TEST_FRAMEWORK_FOUND)
else(OTB_USE_EXTERNAL_BOOST)
  message(STATUS
    "Cannot build OTB/OGR UT without Boost. Please install/enable EXTERNAL BOOST.")
endif(OTB_USE_EXTERNAL_BOOST)

OTB_ADD_EXECUTABLE(otbOGRTestsV "${OGRNon_UT_SRC}" "OTBIO;OTBTesting")
target_link_libraries(otbOGRTestsV OTBOGRAdapters OTBTesting
  )
add_test(coTuOGRDataSourceWrapperIO ${CXX_TEST_PATH}/otbOGRTestsV ${INPUTDATA}/ToulousePoints-examples.shp )

endif( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
