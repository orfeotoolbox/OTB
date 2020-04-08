#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

otb_module_test()

set(OTBApplicationEngineTests
otbApplicationEngineTestDriver.cxx
otbWrapperInputImageParameterTest.cxx
otbWrapperNumericalParameterTest.cxx
otbWrapperStringParameterTest.cxx
otbWrapperChoiceParameterTest.cxx
otbWrapperApplicationRegistryTest.cxx
otbWrapperStringListParameterTest.cxx
otbWrapperDocExampleStructureTest.cxx
otbWrapperParameterKeyTest.cxx
otbWrapperParameterListTest.cxx
otbWrapperInputImageListParameterTest.cxx
otbWrapperInputVectorDataListParameterTest.cxx
otbWrapperApplicationDocTests.cxx
otbWrapperOutputImageParameterTest.cxx
otbApplicationMemoryConnectTest.cxx
otbWrapperImageInterface.cxx
)

add_executable(otbApplicationEngineTestDriver ${OTBApplicationEngineTests})
target_link_libraries(otbApplicationEngineTestDriver ${OTBApplicationEngine-Test_LIBRARIES})
otb_module_target_label(otbApplicationEngineTestDriver)

# Tests Declaration

otb_add_test(NAME owTvInputImageParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperInputImageParameterTest
  ${INPUTDATA}/poupees.tif
  "mykey"
  "my description"
  )

otb_add_test(NAME owTvFloatParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperFloatParameterTest
  )

otb_add_test(NAME owTvIntParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperIntParameterTest
  )

otb_add_test(NAME owTvRAMParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperRAMParameterTest
  )

otb_add_test(NAME owTvStringParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperStringParameterTest1
  "value1"
  "mykey"
  "my description"
  )

otb_add_test(NAME owTvApplicationRegistry COMMAND otbApplicationEngineTestDriver
  otbWrapperApplicationRegistry
  )

otb_add_test(NAME owTvStringListParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperStringListParameterTest1
  "value1"
  "value2"
  "value3"
  "mykey"
  "my description"
  )

otb_add_test(NAME owTvParameterKey COMMAND otbApplicationEngineTestDriver
  otbWrapperParameterKey
  )

otb_add_test(NAME owTvInputImageListParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperInputImageListParameterTest1
  ${INPUTDATA}/poupees_c1.raw.hdr
  ${INPUTDATA}/poupees.tif
  "mykey"
  "my description"
  )

otb_add_test(NAME owTvInputVectorDataListParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperInputVectorDataListParameterTest1
  ${INPUTDATA}/ToulousePoints-examples.shp
  ${INPUTDATA}/ToulouseRoads-examples.shp
  "mykey"
  "my description"
  )

otb_add_test(NAME owTvOutputImageParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperOutputImageParameterTest1
  ${INPUTDATA}/poupees.tif
  ${TEMP}/owTvOutputImageParameter2.tif
  "mykey"
  "my description"
  )

#~ otb_add_test(NAME owTvOutputImageParameterConversion COMMAND otbApplicationEngineTestDriver
  #~ otbWrapperOutputImageParameterConversionTest
  #~ ${INPUTDATA}/poupees.tif
  #~ ${TEMP}/poupees_out.tif
  #~ )

otb_add_test(NAME owTvDocExampleStructureTest COMMAND otbApplicationEngineTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE}/owTuDocExampleStructureTest.txt
  ${TEMP}/owTuDocExampleStructureTest.txt
  otbWrapperDocExampleStructureTest
  ${TEMP}/owTuDocExampleStructureTest.txt
  )


# Warning this test require otbapp_Smoothing and otbapp_ConcatenateImages to be built
otb_add_test(NAME owTvApplicationMemoryConnectTest COMMAND otbApplicationEngineTestDriver otbApplicationMemoryConnectTest
  $<TARGET_FILE_DIR:otbapp_Smoothing>
  ${INPUTDATA}/poupees.tif
  ${TEMP}/owTvApplicationMemoryConnectTestOutput.tif)

otb_add_test(NAME owTvParameterGroup COMMAND otbApplicationEngineTestDriver
  otbWrapperParameterList
  )

otb_add_test(NAME owTvImageInterface COMMAND otbApplicationEngineTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/owTvImageInterfaceOut.txt
  ${TEMP}/owTvImageInterfaceOut.txt
  otbWrapperImageInterface
  $<TARGET_FILE_DIR:otbapp_Smoothing>
  ${INPUTDATA}/Capitole_Rasterization.tif
  ${TEMP}/owTvImageInterfaceOut.txt
  )
