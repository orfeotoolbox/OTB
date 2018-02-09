#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
otbWrapperApplicationTest.cxx
otbWrapperInputImageParameterTest.cxx
otbWrapperNumericalParameterTest.cxx
otbWrapperStringParameterTest.cxx
otbWrapperChoiceParameterTest.cxx
otbWrapperApplicationRegistryTest.cxx
otbWrapperStringListParameterTest.cxx
otbWrapperRAMParameterTest.cxx
otbWrapperDocExampleStructureTest.cxx
otbWrapperParameterKeyTest.cxx
otbWrapperParameterListTest.cxx
otbWrapperEmptyParameterTest.cxx
otbWrapperInputImageListParameterTest.cxx
otbWrapperInputVectorDataListParameterTest.cxx
otbWrapperApplicationDocTests.cxx
otbWrapperApplicationHtmlDocGeneratorTest.cxx
otbWrapperInputVectorDataParameterTest.cxx
otbWrapperOutputImageParameterTest.cxx
otbApplicationMemoryConnectTest.cxx
)

add_executable(otbApplicationEngineTestDriver ${OTBApplicationEngineTests})
target_link_libraries(otbApplicationEngineTestDriver ${OTBApplicationEngine-Test_LIBRARIES})
otb_module_target_label(otbApplicationEngineTestDriver)

# Tests Declaration

otb_add_test(NAME owTuApplication COMMAND otbApplicationEngineTestDriver
  otbWrapperApplicationNew
  )

otb_add_test(NAME owTvInputImageParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperInputImageParameterTest1
  ${INPUTDATA}/poupees.tif
  "mykey"
  "my description"
  )

otb_add_test(NAME owTuInputImageParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperInputImageParameterNew
  )

otb_add_test(NAME owTuNumericalParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperNumericalParameterNew
  )

otb_add_test(NAME owTvNumericalParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperNumericalParameterTest1
  42.42
  "mykey"
  "my description"
  )

otb_add_test(NAME owTuStringParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperStringParameterNew
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

otb_add_test(NAME owTuStringListParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperStringListParameterNew
  )

otb_add_test(NAME owTvStringListParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperStringListParameterTest1
  "value1"
  "value2"
  "value3"
  "mykey"
  "my description"
  )

otb_add_test(NAME owTvParameterRAM COMMAND otbApplicationEngineTestDriver
  otbWrapperRAMParameterNew
  )

otb_add_test(NAME owTvParameterKey COMMAND otbApplicationEngineTestDriver
  otbWrapperParameterKey
  )

otb_add_test(NAME owTuEmptyParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperEmptyParameterNew
  )

otb_add_test(NAME owTuInputImageListParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperInputImageListParameterNew
  )

otb_add_test(NAME owTvInputImageListParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperInputImageListParameterTest1
  ${INPUTDATA}/poupees_c1.hdr
  ${INPUTDATA}/poupees.tif
  "mykey"
  "my description"
  )

otb_add_test(NAME owTuInputVectorDataListParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperInputVectorDataListParameterNew
  )

otb_add_test(NAME owTvInputVectorDataListParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperInputVectorDataListParameterTest1
  ${INPUTDATA}/ToulousePoints-examples.shp
  ${INPUTDATA}/ToulouseRoads-examples.shp
  "mykey"
  "my description"
  )

otb_add_test(NAME owTvApplicationHtmlDocGeneratorTest
  COMMAND otbApplicationEngineTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE}/owTvOReadImageInfoDoc.html
  ${TEMP}/owTvOReadImageInfoDoc.html
  otbWrapperApplicationHtmlDocGeneratorTest1
  ReadImageInfo
  $<TARGET_FILE_DIR:otbapp_ReadImageInfo>
  ${TEMP}/owTvOReadImageInfoDoc.html
  0 # don't show key in the parameterdescription
  )

otb_add_test(NAME owTvApplicationHtmlDocGeneratorNew COMMAND otbApplicationEngineTestDriver
  otbWrapperApplicationHtmlDocGeneratorNew
  )

otb_add_test(NAME owTuInputVectorDataParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperInputVectorDataParameterNew
  )

otb_add_test(NAME owTuOutputImageParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperOutputImageParameterNew
  )

otb_add_test(NAME owTvOutputImageParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperOutputImageParameterTest1
  ${INPUTDATA}/poupees.tif
  ${TEMP}/owTvOutputImageParameter2.tif
  "mykey"
  "my description"
  )

otb_add_test(NAME owTvOutputImageParameterConversion COMMAND otbApplicationEngineTestDriver
  otbWrapperOutputImageParameterConversionTest
  ${INPUTDATA}/poupees.tif
  ${TEMP}/poupees_out.tif
  )

otb_add_test(NAME owTvDocExampleStructureTest COMMAND otbApplicationEngineTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE}/owTuDocExampleStructureTest.txt
  ${TEMP}/owTuDocExampleStructureTest.txt
  otbWrapperDocExampleStructureTest
  ${TEMP}/owTuDocExampleStructureTest.txt
  )

otb_add_test(NAME owTuDocExampleStructureNew COMMAND otbApplicationEngineTestDriver
  otbWrapperDocExampleStructureNew
  )

otb_add_test(NAME owTvApplicationMemoryConnectTest COMMAND otbApplicationEngineTestDriver otbApplicationMemoryConnectTest
  $<TARGET_FILE_DIR:otbapp_Smoothing>
  ${INPUTDATA}/poupees.tif
  ${TEMP}/owTvApplicationMemoryConnectTestOutput.tif)

otb_add_test(NAME owTvParameterGroup COMMAND otbApplicationEngineTestDriver
  otbWrapperParameterList
  )
