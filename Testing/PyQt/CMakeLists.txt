set(TEST_DRIVER "${OTB_TEST_DRIVER}"
    --add-before-env PYTHONPATH        "${CMAKE_BINARY_DIR}/Code/Wrappers/SWIG"
    --add-before-env PYTHONPATH        $<TARGET_FILE_DIR:_otbApplication>
    --add-before-env PYTHONPATH        "${CMAKE_SOURCE_DIR}/Code/Wrappers/PyQt"
    --add-before-env ITK_AUTOLOAD_PATH $<TARGET_FILE_DIR:otbapp_TestApplication> )

add_test( NAME pqTvSmoothing
          COMMAND ${TEST_DRIVER} Execute
                  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PyQtTest.py
                  Smoothing )

add_test( NAME pqTvRescale
          COMMAND ${TEST_DRIVER} Execute
                  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PyQtTest.py
                  Rescale )