otb_module_test()

if (Boost_UNIT_TEST_FRAMEWORK_FOUND)
  add_executable(otbOGRTests otbOGRDataSourceWrapperNew.cxx)
  target_link_libraries(otbOGRTests ${OTBGdalAdapters-Test_LIBRARIES})
  
  add_test(NAME coTuOGRDataSourceWrapperNew
           COMMAND otbOGRTests coTuOGRDataSourceWrapperNew
           WORKING_DIRECTORY ${TEMP})
endif()

add_executable(otbOGRTestsIO otbOGRDataSourceWrapperIO.cxx)
target_link_libraries(otbOGRTestsIO ${OTBGdalAdapters-Test_LIBRARIES})

add_test(NAME coTuOGRDataSourceWrapperIO
         COMMAND otbOGRTestsIO ${INPUTDATA}/ToulousePoints-examples.shp )
