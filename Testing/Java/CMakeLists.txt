include( UseJava )

SET(TEST_DRIVER "${OTB_TEST_DRIVER}"
    --add-before-env LD_LIBRARY_PATH        "${CMAKE_BINARY_DIR}/Code/Wrappers/SWIG" 
    --add-before-env ITK_AUTOLOAD_PATH "${CMAKE_BINARY_DIR}/Example"
)

set( CMAKE_JAVA_INCLUDE_PATH ${OTB-Wrapper_BINARY_DIR}/Code/Wrappers/SWIG/org.otb.application.jar )
add_jar( JavaSmoothingTest JavaSmoothingTest.java )



SET(JAVA_COMMAND "${JAVACOMMAND}" -Djava.library.path=${OTB-Wrapper_BINARY_DIR}/Code/Wrappers/SWIG)


add_test( NAME jaTvJavaSmoothingTest
          COMMAND ${TEST_DRIVER} Execute
          ${JAVA_COMMAND} -cp ${CMAKE_JAVA_INCLUDE_PATH}:JavaSmoothingTest.jar SmoothingTest
          ${OTB_DATA_ROOT}/Input/ToulouseExtract_WithGeom.tif
          ${TEMP}/ToulouseExtract_WithGeom_ )
