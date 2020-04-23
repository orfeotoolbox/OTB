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

set(OTBCommandLineTests
otbCommandLineTestDriver.cxx
otbWrapperCommandLineLauncherTests.cxx
otbWrapperCommandLineParserTests.cxx
)

add_executable(otbCommandLineTestDriver ${OTBCommandLineTests})
target_link_libraries(otbCommandLineTestDriver ${OTBCommandLine-Test_LIBRARIES})
otb_module_target_label(otbCommandLineTestDriver)

# Tests Declaration

otb_add_test(NAME clTvWrapperCommandLineLauncherTest_WrongParam
  COMMAND otbCommandLineTestDriver otbWrapperCommandLineLauncherTest
  "Rescale" $<TARGET_FILE_DIR:otbapp_Rescale> -inn image)
set_property(TEST clTvWrapperCommandLineLauncherTest_WrongParam PROPERTY WILL_FAIL true)

otb_add_test(NAME clTvWrapperCommandLineLauncherTest_Help2
  COMMAND otbCommandLineTestDriver otbWrapperCommandLineLauncherTest
  "Rescale" $<TARGET_FILE_DIR:otbapp_Rescale>
  -in ${INPUTDATA}/poupees.tif
  -out ${TEMP}/clTvWrapperCommandLineLauncherTest_Help2.tif
  -outmin 15
  -outmax 200
  -help)
set_property(TEST clTvWrapperCommandLineLauncherTest_Help2 PROPERTY WILL_FAIL true)

otb_add_test(NAME clTvWrapperCommandLineLauncherTest
  COMMAND otbCommandLineTestDriver otbWrapperCommandLineLauncherTest
  "Rescale" $<TARGET_FILE_DIR:otbapp_Rescale>
  -in ${INPUTDATA}/poupees.tif
  -out ${TEMP}/clTvWrapperCommandLineLauncherTest.tif
  -outmin 15
  -outmax 200 )

otb_add_test(NAME clTvWrapperCommandLineLauncherTest_MissingDash
  COMMAND otbCommandLineTestDriver otbWrapperCommandLineLauncherTest
  "Rescale" $<TARGET_FILE_DIR:otbapp_Rescale> -in image1)
set_property(TEST clTvWrapperCommandLineLauncherTest_MissingDash PROPERTY WILL_FAIL true)

otb_add_test(NAME clTvWrapperCommandLineLauncherTest_MissingManParam
  COMMAND otbCommandLineTestDriver otbWrapperCommandLineLauncherTest
  "Rescale" $<TARGET_FILE_DIR:otbapp_Rescale> )
set_property(TEST clTvWrapperCommandLineLauncherTest_MissingManParam PROPERTY WILL_FAIL true)

otb_add_test(NAME clTvWrapperCommandLineLauncherTest_Help
  COMMAND otbCommandLineTestDriver otbWrapperCommandLineLauncherTest
  "Rescale" $<TARGET_FILE_DIR:otbapp_Rescale> "-help")
set_property(TEST clTvWrapperCommandLineLauncherTest_Help PROPERTY WILL_FAIL true)

otb_add_test(NAME clTvWrapperCommandLineLauncherTest_DoubleParam
  COMMAND otbCommandLineTestDriver otbWrapperCommandLineLauncherTest
  "Rescale" $<TARGET_FILE_DIR:otbapp_Rescale> -in image1 -in image2 )
set_property(TEST clTvWrapperCommandLineLauncherTest_DoubleParam PROPERTY WILL_FAIL true)

otb_add_test(NAME clTvWrapperCommandLineParserTest_IsAttExistsEnd
  COMMAND otbCommandLineTestDriver otbWrapperCommandLineParserTest4
  "-m"
  "TrainImagesClassifier" "test" "-il" "QB_1_ortho.tif" "-vd" "vd" ".shape" "-m" "name")

otb_add_test(NAME clTvWrapperCommandLineParserTest_WithKeyGoodModule
  COMMAND otbCommandLineTestDriver otbWrapperCommandLineParserTest2
  "Rescale"  $<TARGET_FILE_DIR:otbapp_Rescale>)

otb_add_test(NAME clTvWrapperCommandLineParserTest_IsAttExists
  COMMAND otbCommandLineTestDriver otbWrapperCommandLineParserTest4
  "-m"
  "TrainImagesClassifier" "test" "-il" "QB_1_ortho.tif" "-vd" "vd" ".shape" "-m" "name")

otb_add_test(NAME clTvWrapperCommandLineParserTest_IsAttExistsEndNoParam
  COMMAND otbCommandLineTestDriver otbWrapperCommandLineParserTest4
  "-m"
  "TrainImagesClassifier" "test" "-il" "QB_1_ortho.tif" "-vd" "vd" ".shape" "test" "-m")

otb_add_test(NAME clTvWrapperCommandLineParserTest_MultiplePathOneWrong
  COMMAND otbCommandLineTestDriver otbWrapperCommandLineParserTest2
  "Rescale" $<TARGET_FILE_DIR:otbapp_Rescale> "FakePath")
set_property(TEST clTvWrapperCommandLineParserTest_MultiplePathOneWrong PROPERTY WILL_FAIL true)

otb_add_test(NAME clTvWrapperCommandLineParserTest_WithKeyNoModule
  COMMAND otbCommandLineTestDriver otbWrapperCommandLineParserTest2
  $<TARGET_FILE_DIR:otbapp_Rescale> )
set_property(TEST clTvWrapperCommandLineParserTest_WithKeyNoModule PROPERTY WILL_FAIL true)

otb_add_test(NAME clTvWrapperCommandLineParserTest_GetAttWithoutValue
  COMMAND otbCommandLineTestDriver otbWrapperCommandLineParserTest1
  "-key"
  "-key" )
set_property(TEST clTvWrapperCommandLineParserTest_GetAttWithoutValue PROPERTY WILL_FAIL true)

otb_add_test(NAME clTvWrapperCommandLineParserTest_WithKeyNonAlphaNumModuleName
  COMMAND otbCommandLineTestDriver otbWrapperCommandLineParserTest2
  "-Re+scale" $<TARGET_FILE_DIR:otbapp_Rescale> )
set_property(TEST clTvWrapperCommandLineParserTest_WithKeyNonAlphaNumModuleName PROPERTY WILL_FAIL true)

otb_add_test(NAME clTvWrapperCommandLineParserTest_IsAttExists_Wrong
  COMMAND otbCommandLineTestDriver otbWrapperCommandLineParserTest3
  "-m"
  "TrainImagesClassifier" "test" "-il" "QB_1_ortho.tif" "-vd" "vd" ".shape" )

otb_add_test(NAME clTvWrapperCommandLineParserTest_NonAlphaNumModuleName
  COMMAND otbCommandLineTestDriver
  otbWrapperCommandLineParserTest2
  "-Re+scale"
  $<TARGET_FILE_DIR:otbapp_Rescale> )
set_property(TEST clTvWrapperCommandLineParserTest_NonAlphaNumModuleName PROPERTY WILL_FAIL true)

otb_add_test(NAME clTvWrapperCommandLineParserTest_GetAtt
  COMMAND otbCommandLineTestDriver otbWrapperCommandLineParserTest1
  "-key"
  "this" "is" "a" "line" "test" "to" "extract" "-key" "name1" "name2" )

otb_add_test(NAME clTvWrapperCommandLineParserTest_MultiplePath
  COMMAND otbCommandLineTestDriver otbWrapperCommandLineParserTest2
  "Rescale" $<TARGET_FILE_DIR:otbapp_Rescale> $<TARGET_FILE_DIR:otbapp_Rescale>)

otb_add_test(NAME clTvWrapperCommandLineParserTest_NoPath
  COMMAND otbCommandLineTestDriver otbWrapperCommandLineParserTest2
  "Rescale" )
set_property(TEST clTvWrapperCommandLineParserTest_NoPath PROPERTY WILL_FAIL true)

otb_add_test(NAME clTvWrapperCommandLineParserTest_WrongPath
  COMMAND otbCommandLineTestDriver otbWrapperCommandLineParserTest2
  "Rescale" "FakePath")
set_property(TEST clTvWrapperCommandLineParserTest_WrongPath PROPERTY WILL_FAIL true)

otb_add_test(NAME clTvWrapperCommandLineParserTest_GoodModule
  COMMAND otbCommandLineTestDriver
  otbWrapperCommandLineParserTest2
  "Rescale"
  $<TARGET_FILE_DIR:otbapp_Rescale> )

otb_add_test(NAME clTvWrapperCommandLineParserTest_NoModule
  COMMAND otbCommandLineTestDriver otbWrapperCommandLineParserTest2
  "")
set_property(TEST clTvWrapperCommandLineParserTest_NoModule PROPERTY WILL_FAIL true)

