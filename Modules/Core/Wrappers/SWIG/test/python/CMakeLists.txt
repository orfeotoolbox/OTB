#
# Copyright (C) 2005-2019 CS Systemes d'Information (CS SI)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

set(TEST_DRIVER otbTestDriver
    --add-before-env PYTHONPATH        ${CMAKE_BINARY_DIR}/${OTB_INSTALL_PYTHON_DIR}
    --add-before-env OTB_APPLICATION_PATH $<TARGET_FILE_DIR:otbapp_OrthoRectification>)

if(OTBGroup_FeaturesExtraction)
  set(TEST_DRIVER ${TEST_DRIVER} 
  --add-before-env OTB_APPLICATION_PATH $<TARGET_FILE_DIR:otbapp_Smoothing> )
endif()

if(WIN32)
  # on windows, loading the module _otbApplication requires the otbossimplugins*.dll
  # which is in the 'bin/<CMAKE_BUILD_TYPE>' folder
  set(TEST_DRIVER ${TEST_DRIVER}
    --add-before-env PATH $<TARGET_FILE_DIR:OTBCommon>)
endif(WIN32)


if(OTBGroup_FeaturesExtraction)
add_test( NAME pyTvSmoothing
  COMMAND ${TEST_DRIVER} Execute
  ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonSmoothingTest
  ${OTB_DATA_ROOT}/Input/ToulouseExtract_WithGeom.tif
  ${TEMP}/pyTvSmoothing_ )

add_test( NAME pyTvRescale
  COMMAND ${TEST_DRIVER} Execute
  ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonRescaleTest
  ${OTB_DATA_ROOT}/Input/ToulouseExtract_WithGeom.tif
  ${TEMP}/pyTvRescale)

add_test( NAME pyTvBandMathOutXML
  COMMAND ${TEST_DRIVER} Execute
  ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonOutXMLTest
  ${OTB_DATA_ROOT}/Input/verySmallFSATSW_r.tif
  ${OTB_DATA_ROOT}/Input/verySmallFSATSW_nir.tif
  ${OTB_DATA_ROOT}/Input/verySmallFSATSW.tif
  ${TEMP}/pyTvBandMathOutXML.tif
  ${TEMP}/pyTvBandMathOutXML.xml
  )

add_test( NAME pyTvBandMathInXML
  COMMAND ${TEST_DRIVER} Execute
  ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonInXMLTest
  ${TEMP}/pyTvBandMathOutXML.xml
  ${TEMP}/pyTvBandMathInXML.tif
  )

set_tests_properties(pyTvBandMathInXML PROPERTIES DEPENDS pyTvBandMathOutXML)

add_test( NAME pyTvNewStyleParameters
  COMMAND ${TEST_DRIVER} Execute
  ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonNewStyleParametersTest
  ${OTB_DATA_ROOT}/Input/sensor_stereo_left.tif
  ${TEMP}/pyTvNewStyleParametersTest.tif
  ${OTB_DATA_ROOT}/Input/apTvUtSmoothingTest_OutXML.xml)

add_test( NAME pyTvNewStyleParametersInstantiateAll
  COMMAND ${TEST_DRIVER} Execute
  ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonNewStyleParametersInstantiateAllTest
  )

add_test( NAME pyTvConnectApplications
  COMMAND ${TEST_DRIVER} Execute
  ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonConnectApplications
  ${OTB_DATA_ROOT}/Input/poupees.tif
  ${TEMP}/pyTvConnectApplicationsOutput.tif)

add_test( NAME pyTvParametersDict
  COMMAND ${TEST_DRIVER} Execute
  ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonParametersDict
  ${OTB_DATA_ROOT}/Input/poupees.tif)

add_test( NAME pyTvNoUpdateParameter
  COMMAND ${TEST_DRIVER} Execute
  ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonNoUpdateParameter
  ${OTB_DATA_ROOT}/Input/poupees.tif
  ${OTB_DATA_ROOT}/Input/training/training.shp
  )

add_test( NAME pyTvBug823
  COMMAND ${TEST_DRIVER} Execute
  ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  Bug823
  )

add_test( NAME pyTvBug736
  COMMAND ${TEST_DRIVER}
  --compare-image ${NOTOL}
  ${BASELINE}/apTvRIRadiometricVegetationIndices.tif
  ${TEMP}/pyTvBug736Output.tif
  Execute
  ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  Bug736
  ${OTB_DATA_ROOT}/Input/veryverySmallFSATSW.tif
  ${TEMP}/pyTvBug736Output.tif
  )

add_test( NAME pyTvBug2262
  COMMAND ${TEST_DRIVER}
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/pyTvBug2262Output.txt
  ${TEMP}/pyTvBug2262Output.txt
  Execute
  ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  Bug2262
  ${OTB_DATA_ROOT}/Input/Capitole_Rasterization.tif
  ${TEMP}/pyTvBug2262Output.txt
  )

add_test( NAME pyTvBug1899
  COMMAND ${TEST_DRIVER}
  Execute
  ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  Bug1899
  ${OTB_DATA_ROOT}/Input/veryverySmallFSATSW.tif
  ${TEMP}/pyTvBug1899Output.tif
  )
endif()

if(OTBGroup_Hyperspectral)
add_test( NAME pyTvHyperspectralUnmixingUCLS
  COMMAND ${TEST_DRIVER} Execute
  ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonHyperspectralUnmixing1
  ${OTB_DATA_ROOT}/Input/Hyperspectral/synthetic/hsi_cube.tif
  ${TEMP}/pyTvHyperspectralUnmixing_ucls.tif
  ${OTB_DATA_ROOT}/Input/Hyperspectral/synthetic/endmembers.tif
  ucls
  )
endif()

if(OTBGroup_Learning)
add_test( NAME pyTvBug804
  COMMAND ${TEST_DRIVER} Execute
  ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  Bug804
  )
endif()

if ( NUMPY_FOUND AND OTBGroup_FeaturesExtraction )

add_test( NAME pyTvNumpyIO
  COMMAND ${TEST_DRIVER} Execute
  ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonNumpyTest
  ${OTB_DATA_ROOT}/Input/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
  ${TEMP}/pyTvNumpyIO_SmoothingOut.png )


add_test( NAME pyTvImageInterface
  COMMAND ${TEST_DRIVER} Execute
  ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonImageInterface
  ${OTB_DATA_ROOT}/Input/QB_Toulouse_Ortho_XS.tif
  )

endif()

add_test( NAME pyTvBug1498
  COMMAND ${TEST_DRIVER} Execute
  ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  Bug823
  ${OTB_DATA_ROOT}/Input/poupees.tif
  ${TEMP}/Bu1498-output.tif)

add_test( NAME pyTuImageMetadata
  COMMAND ${TEST_DRIVER} Execute
  ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/PythonTestDriver.py
  PythonImageMetadataTest
  )
