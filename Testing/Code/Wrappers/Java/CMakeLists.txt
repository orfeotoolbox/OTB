include( UseJava )


set(TEST_DRIVER otbTestDriver
    --add-before-env ITK_AUTOLOAD_PATH $<TARGET_FILE_DIR:otbapp_Smoothing>
)

set( PATH_SEPARATOR ":")
if (WIN32)
  set( PATH_SEPARATOR ";")
endif()

set( CMAKE_JAVA_INCLUDE_PATH ${OTB_BINARY_DIR}/Code/Wrappers/SWIG/org.otb.application.jar )
set( JAVA_COMMAND "${Java_JAVA_EXECUTABLE}"
                  "-Djava.library.path=${OTB_BINARY_DIR}/Code/Wrappers/SWIG${PATH_SEPARATOR}$<TARGET_FILE_DIR:otbApplicationJava>" )

add_jar( JavaSmoothingTest JavaSmoothingTest.java )
add_dependencies( JavaSmoothingTest org_otb_Application_jar )
add_test( NAME jaTvSmoothing
          COMMAND ${TEST_DRIVER} Execute
          ${Java_JAVA_EXECUTABLE}
            "-Djava.library.path=${OTB_BINARY_DIR}/Code/Wrappers/SWIG${PATH_SEPARATOR}$<TARGET_FILE_DIR:otbApplicationJava>"
            -cp "${CMAKE_JAVA_INCLUDE_PATH}${PATH_SEPARATOR}${CMAKE_CURRENT_BINARY_DIR}/JavaSmoothingTest.jar"
            SmoothingTest
            ${OTB_DATA_ROOT}/Input/ToulouseExtract_WithGeom.tif
            ${TEMP}/jaTvSmoothing_ )

add_jar( JavaRescaleTest JavaRescaleTest.java )
add_dependencies( JavaRescaleTest org_otb_Application_jar )
add_test( NAME jaTvRescale
          COMMAND ${TEST_DRIVER} Execute
          ${Java_JAVA_EXECUTABLE}
            "-Djava.library.path=${OTB_BINARY_DIR}/Code/Wrappers/SWIG${PATH_SEPARATOR}$<TARGET_FILE_DIR:otbApplicationJava>"
            -cp "${CMAKE_JAVA_INCLUDE_PATH}${PATH_SEPARATOR}${CMAKE_CURRENT_BINARY_DIR}/JavaRescaleTest.jar"
            RescaleTest
            ${INPUTDATA}/poupees.tif
            ${TEMP}/jaTvRescaleTest.tif
            20
            150 )

add_jar( JavaRescaleOutXMLTest JavaRescaleOutXMLTest.java )
add_dependencies( JavaRescaleOutXMLTest org_otb_Application_jar )
add_test( NAME jaTvRescaleOutXML
          COMMAND ${TEST_DRIVER} Execute
          ${Java_JAVA_EXECUTABLE}
            "-Djava.library.path=${OTB_BINARY_DIR}/Code/Wrappers/SWIG${PATH_SEPARATOR}$<TARGET_FILE_DIR:otbApplicationJava>"
            -cp "${CMAKE_JAVA_INCLUDE_PATH}${PATH_SEPARATOR}${CMAKE_CURRENT_BINARY_DIR}/JavaRescaleOutXMLTest.jar"
            RescaleOutXMLTest
            ${INPUTDATA}/poupees.tif
            ${TEMP}/jaTvRescaleOutXMLTest.tif
            20
            150
            ${TEMP}/jaTvRescaleOutXMLTest.xml )

add_jar( JavaRescaleInXMLTest JavaRescaleInXMLTest.java )
add_dependencies( JavaRescaleInXMLTest org_otb_Application_jar )
add_test( NAME jaTvRescaleInXML
          COMMAND ${TEST_DRIVER} Execute
          ${Java_JAVA_EXECUTABLE}
            "-Djava.library.path=${OTB_BINARY_DIR}/Code/Wrappers/SWIG${PATH_SEPARATOR}$<TARGET_FILE_DIR:otbApplicationJava>"
            -cp "${CMAKE_JAVA_INCLUDE_PATH}${PATH_SEPARATOR}${CMAKE_CURRENT_BINARY_DIR}/JavaRescaleInXMLTest.jar"
            RescaleInXMLTest
            ${INPUTDATA}/jaTvRescaleInXMLTest.xml
            ${INPUTDATA}/poupees.tif
            ${TEMP}/jaTvRescaleInXMLTest.tif)
