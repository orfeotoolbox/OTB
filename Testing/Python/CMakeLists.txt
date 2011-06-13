
SET(TEST_DRIVER "${OTB_TEST_DRIVER}"
    --add-before-env PYTHONPATH        "${CMAKE_BINARY_DIR}/Code/Wrappers/SWIG" 
    --add-before-env ITK_AUTOLOAD_PATH "${CMAKE_BINARY_DIR}/Example"
)

ADD_TEST( NAME pyTvSmoothing
          COMMAND ${TEST_DRIVER} Execute
                  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonSmoothingTest.py 
                  ${OTB_DATA_ROOT}/Input/ToulouseExtract_WithGeom.tif
                  ${TEMP}/pyTvSmoothing_ )
