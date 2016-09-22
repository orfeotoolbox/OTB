set(TEST_DRIVER otbTestDriver
    --add-before-env PYTHONPATH        "${OTBSWIGWrapper_BINARY_DIR}/src"
    --add-before-env PYTHONPATH        $<TARGET_FILE_DIR:_otbApplication>
    --add-before-env OTB_APPLICATION_PATH $<TARGET_FILE_DIR:otbapp_Smoothing> )

if(WIN32)
  # on windows, loading the module _otbApplication requires the otbossimplugins*.dll
  # which is in the 'bin/<CMAKE_BUILD_TYPE>' folder
  set(TEST_DRIVER ${TEST_DRIVER}
    --add-before-env PATH ${CMAKE_BINARY_DIR}/bin
    --add-before-env PATH $<TARGET_FILE_DIR:OTBCommon>)
endif(WIN32)

add_test( NAME pyTvSmoothing
  COMMAND ${TEST_DRIVER} Execute
  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonSmoothingTest
  ${OTB_DATA_ROOT}/Input/ToulouseExtract_WithGeom.tif
  ${TEMP}/pyTvSmoothing_ )

add_test( NAME pyTvRescale
  COMMAND ${TEST_DRIVER} Execute
  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonRescaleTest
  ${OTB_DATA_ROOT}/Input/ToulouseExtract_WithGeom.tif
  ${TEMP}/pyTvRescale)

add_test( NAME pyTvHyperspectralUnmixingUCLS
  COMMAND ${TEST_DRIVER} Execute
  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonHyperspectralUnmixing1
  ${OTB_DATA_ROOT}/Input/Hyperspectral/synthetic/hsi_cube.tif
  ${TEMP}/pyTvHyperspectralUnmixing_ucls.tif
  ${OTB_DATA_ROOT}/Input/Hyperspectral/synthetic/endmembers.tif
  ucls
  )

add_test( NAME pyTvBug440
  COMMAND ${TEST_DRIVER} Execute
  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  Bug440
  ${OTB_DATA_ROOT}/Input/scene.png
  ${TEMP}/pyTvBug440Output.tif
  )

add_test( NAME pyTvBug804
  COMMAND ${TEST_DRIVER} Execute
  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  Bug804
  )

add_test( NAME pyTvBug823
  COMMAND ${TEST_DRIVER} Execute
  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  Bug823
  )

add_test( NAME pyTvBug736
  COMMAND ${TEST_DRIVER}
  --compare-image ${NOTOL}
  ${BASELINE}/apTvRIRadiometricVegetationIndices.tif
  ${TEMP}/pyTvBug736Output.tif
  Execute
  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  Bug736
  ${OTB_DATA_ROOT}/Input/veryverySmallFSATSW.tif
  ${TEMP}/pyTvBug736Output.tif
  )

add_test( NAME pyTvBandMathOutXML
  COMMAND ${TEST_DRIVER} Execute
  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonOutXMLTest
  ${OTB_DATA_ROOT}/Input/verySmallFSATSW_r.tif
  ${OTB_DATA_ROOT}/Input/verySmallFSATSW_nir.tif
  ${OTB_DATA_ROOT}/Input/verySmallFSATSW.tif
  ${TEMP}/pyTvBandMathOutXML.tif
  ${TEMP}/pyTvBandMathOutXML.xml
  )

add_test( NAME pyTvBandMathInXML
  COMMAND ${TEST_DRIVER} Execute
  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonInXMLTest
  ${INPUTDATA}/pyTvBandMathInXML.xml
  ${OTB_DATA_ROOT}/Input/verySmallFSATSW_r.tif
  ${OTB_DATA_ROOT}/Input/verySmallFSATSW_nir.tif
  ${OTB_DATA_ROOT}/Input/verySmallFSATSW.tif
  ${TEMP}/pyTvBandMathInXML.tif
  )

add_test( NAME pyTvNumpyIO
  COMMAND ${TEST_DRIVER} Execute
  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonNumpyTest
  ${OTB_DATA_ROOT}/Examples/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
  ${TEMP}/pyTvNumpyIO_SmoothingOut.png )

add_test( NAME pyTvNewStyleParameters
  COMMAND ${TEST_DRIVER} Execute
  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonNewStyleParametersTest
  ${OTB_DATA_ROOT}/Input/poupees.tif
  ${TEMP}/pyTvNewStyleParametersTest.tif
  ${OTB_DATA_ROOT}/Input/apTvUtSmoothingTest_OutXML.xml)

add_test( NAME pyTvNewStyleParametersInstanciateAll
  COMMAND ${TEST_DRIVER} Execute
  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonNewStyleParametersInstanciateAllTest
  )

add_test( NAME pyTvConnectApplications
  COMMAND ${TEST_DRIVER} Execute
  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonConnectApplications
  ${OTB_DATA_ROOT}/Input/poupees.tif
  ${TEMP}/pyTvConnectApplicationsOutput.tif)
