# configure the test driver
FIND_PROGRAM(OTB_TEST_DRIVER otbTestDriver PATHS ${OTB_DIR}/bin ${OTB_DIR}/../../bin DOC "Path to the otbTestDriver executable from OTB")


SET(TEST_DRIVER "${OTB_TEST_DRIVER}"
    --add-before-env PYTHONPATH        "${CMAKE_BINARY_DIR}/Code/Wrappers/SWIG" 
    --add-before-env ITK_AUTOLOAD_PATH "${CMAKE_BINARY_DIR}/Example/Smoothing"
)
SET(TEMP ${OTBWrapper_BINARY_DIR}/Temporary)

ADD_TEST( NAME PythonTest1
          COMMAND ${TEST_DRIVER} Execute
                  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonSmoothingTest.py 
                  ${OTB_DATA_ROOT}/Input/ToulouseExtract_WithGeom.tif
                  ${TEMP}/ToulouseExtract_WithGeom_ )