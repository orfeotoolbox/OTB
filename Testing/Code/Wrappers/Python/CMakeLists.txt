
SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

set(TEST_DRIVER otbTestDriver
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
                  
add_test( NAME pyTvHyperspectralUnmixingUCLS
          COMMAND ${TEST_DRIVER} Execute
                  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonHyperspectralUnmixing1.py
                  ${OTB_DATA_ROOT}/Input/Hyperspectral/synthetic/hsi_cube.tif
                  ${TEMP}/pyTvHyperspectralUnmixing_ucls.tif
                  ${OTB_DATA_ROOT}/Input/Hyperspectral/synthetic/endmembers.tif
                  ucls
                  )

add_test( NAME pyTvHyperspectralUnmixingFCLS
          COMMAND ${TEST_DRIVER} Execute
                  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonHyperspectralUnmixing1.py
                  ${OTB_DATA_ROOT}/Input/Hyperspectral/synthetic/hsi_cube.tif
                  ${TEMP}/pyTvHyperspectralUnmixing_fcls.tif
                  ${OTB_DATA_ROOT}/Input/Hyperspectral/synthetic/endmembers.tif
                  fcls
                  )

add_test( NAME pyTvBug440
          COMMAND ${TEST_DRIVER} Execute
                  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Bug440.py
                  ${OTB_DATA_ROOT}/Input/scene.png
                  ${TEMP}/pyTvBug440Output.tif
                  )

add_test( NAME pyTvBug441
          COMMAND ${TEST_DRIVER} Execute
                  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Bug441.py
                  ${OTB_DATA_ROOT}/Input/stereo_wv2_left.tif
                  ${OTB_DATA_ROOT}/Input/stereo_wv2_right.tif
                  ${TEMP}/pyTvBug441Output.tif
                  )
