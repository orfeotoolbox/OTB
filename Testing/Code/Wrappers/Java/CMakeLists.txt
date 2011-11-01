include( UseJava )

set(TEST_DRIVER otbTestDriver
    --add-before-env ITK_AUTOLOAD_PATH $<TARGET_FILE_DIR:otbapp_Smoothing>
)

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


set( CMAKE_JAVA_INCLUDE_PATH ${OTB_BINARY_DIR}/Code/Wrappers/SWIG/org.otb.application.jar )
set( JAVA_COMMAND "${JAVACOMMAND}" -Djava.library.path=${OTB_BINARY_DIR}/Code/Wrappers/SWIG:${OTB_BINARY_DIR}/bin)

add_jar( JavaSmoothingTest JavaSmoothingTest.java )
add_dependencies( JavaSmoothingTest org_otb_Application_jar )
add_test( NAME jaTvSmoothing
          COMMAND ${TEST_DRIVER} Execute
          ${JAVA_COMMAND} -cp ${CMAKE_JAVA_INCLUDE_PATH}:JavaSmoothingTest.jar SmoothingTest
            ${OTB_DATA_ROOT}/Input/ToulouseExtract_WithGeom.tif
            ${TEMP}/jaTvSmoothing_ )

add_jar( JavaRescaleTest JavaRescaleTest.java )
add_dependencies( JavaRescaleTest org_otb_Application_jar )
add_test( NAME jaTvRescale
          COMMAND ${TEST_DRIVER} Execute
          ${JAVA_COMMAND} -cp ${CMAKE_JAVA_INCLUDE_PATH}:JavaRescaleTest.jar RescaleTest
            ${INPUTDATA}/poupees.tif
            ${TEMP}/jaTvRescaleTest.tif
            20
            150 )
