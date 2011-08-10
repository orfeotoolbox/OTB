
set(TEST_DRIVER "${OTB_TEST_DRIVER}"
    --add-before-env PYTHONPATH        "${CMAKE_BINARY_DIR}/Code/Wrappers/SWIG"
    --add-before-env PYTHONPATH        $<TARGET_FILE_DIR:_otbApplication>
    --add-before-env ITK_AUTOLOAD_PATH $<TARGET_FILE_DIR:otbapp_Smoothing> )

add_test( NAME pyTvSmoothing
          COMMAND ${TEST_DRIVER} Execute
                  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonSmoothingTest.py
                  ${OTB_DATA_ROOT}/Input/ToulouseExtract_WithGeom.tif
                  ${TEMP}/pyTvSmoothing_ )

add_test( NAME pyTvRescale
          COMMAND ${TEST_DRIVER} Execute
                  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonRescaleTest.py
                  ${OTB_DATA_ROOT}/Input/ToulouseExtract_WithGeom.tif
                  ${TEMP}/pyTvRescale)
                  
add_test( NAME pyTvAddition
          COMMAND ${TEST_DRIVER} Execute
                  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonAdditionTest.py
                  89
                  57
                  )