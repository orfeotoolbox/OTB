#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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
set(OTBCommandLineParserTests
  otbTestCommandLineArgumentParser.cxx
  otbTestCommandLineArgumentParserHelp.cxx
  otbTestCommandLineArgumentParserList.cxx
  otbTestCommandLineArgumentParserWithError.cxx
  otbCommandLineParserTestDriver.cxx  )

add_executable(otbCommandLineParserTestDriver ${OTBCommandLineParserTests})
target_link_libraries(otbCommandLineParserTestDriver ${OTBCommandLineParser-Test_LIBRARIES})
otb_module_target_label(otbCommandLineParserTestDriver)
otb_add_test(NAME coTuCmdLineArgParserDeuxEntiers_ExactNumberArg COMMAND otbCommandLineParserTestDriver
  otbTestCommandLineArgumentParser
  -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 )
otb_add_test(NAME coTuCmdLineArgParserHelpWithArg COMMAND otbCommandLineParserTestDriver
  otbTestCommandLineArgumentParserHelp
  --help -image image.png)
otb_add_test(NAME coTuCmdLineArgParserHelpWithoutArg COMMAND otbCommandLineParserTestDriver
  otbTestCommandLineArgumentParserHelp
  --help)
otb_add_test(NAME coTuCmdLineArgParser_List2Arg COMMAND otbCommandLineParserTestDriver
  otbTestCommandLineArgumentParserList
  -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 -doubles 1.0 2.0)
otb_add_test(NAME coTuCmdLineArgParser_List2ArgBis COMMAND otbCommandLineParserTestDriver
  otbTestCommandLineArgumentParserList
  -image image.png -entier 150 -double 12.25 -doubles 1.0 2.0 -deuxentiers 78 56 )
otb_add_test(NAME coTuCmdLineArgParser_EmptyList COMMAND otbCommandLineParserTestDriver
  otbTestCommandLineArgumentParserList
  -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 -doubles)
otb_add_test(NAME coTuCmdLineArgParser_List1Arg COMMAND otbCommandLineParserTestDriver
  otbTestCommandLineArgumentParserList
  -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 -doubles 1.0 2.5)
otb_add_test(NAME coTuCmdLineArgParser_List3ArgWithError COMMAND otbCommandLineParserTestDriver
  otbTestCommandLineArgumentParserList
  -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 -doubles 1.0 2.5 -3.5)
otb_add_test(NAME coTuCmdLineArgParserDeuxEntier_LessArg COMMAND otbCommandLineParserTestDriver
  otbTestCommandLineArgumentParserWithError
  -image image.png -entier 150 -double 12.25 -deuxentiers 78 )
otb_add_test(NAME coTuCmdLineArgParser_MissingObligatoryArg COMMAND otbCommandLineParserTestDriver
  otbTestCommandLineArgumentParserWithError
  -entier 150 -double 12.25 -deuxentiers 78 56 32)
otb_add_test(NAME coTuCmdLineArgParserDeuxEntier_MoreArg COMMAND otbCommandLineParserTestDriver
  otbTestCommandLineArgumentParserWithError
  -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 32)
