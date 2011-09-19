set(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

set(INPUTDATA ${OTB_DATA_ROOT}/Input)
set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

if(WIN32)
  add_definitions(-DWIN32)
endif(WIN32)

# ---------------- Smoothing example test
set(APPLICATIONS_TESTS ${CXX_TEST_PATH}/otbWrapperExampleTests_Smoothing)

add_test(NAME exTvRescaleTest
         COMMAND otbApplicationLauncherCommandLine
                 Rescale
                 --modulePath $<TARGET_FILE_DIR:otbapp_Rescale>
                 --in ${INPUTDATA}/poupees.tif
                 --out ${TEMP}/owTvRescaleTest.tif
                 --outmin 20
                 --outmax 150 )

add_test(NAME exTvSmoothingTest
         COMMAND otbWrapperExampleTests
                 otbSmoothingTest
                 $<TARGET_FILE_DIR:otbapp_Smoothing>
                 ${INPUTDATA}/poupees.tif
                 ${TEMP}/owTvSmoothingTest2.tif )


add_test(NAME exTvReadImageInfoTest
         COMMAND otbApplicationLauncherCommandLine
                 ReadImageInfo
                 --modulePath $<TARGET_FILE_DIR:otbapp_ReadImageInfo>
                 --in ${INPUTDATA}/poupees.tif )
         

add_executable(otbWrapperExampleTests
                 otbWrapperExampleTests.cxx
                 otbRescaleTest.cxx
                 otbSmoothingTest.cxx 
                 otbReadImageInfoTest.cxx )

target_link_libraries(otbWrapperExampleTests OTBApplicationEngine OTBTesting)
