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

set(OTBFuzzyTests
otbFuzzyTestDriver.cxx
otbFuzzyVariableSetValue.cxx
otbFuzzyVariable2Values.cxx
otbFuzzyVariableDSApplied.cxx
otbFuzzyVariableGetMaxVar.cxx
otbFuzzyDescriptorsModelManager.cxx
)

add_executable(otbFuzzyTestDriver ${OTBFuzzyTests})
target_link_libraries(otbFuzzyTestDriver ${OTBFuzzy-Test_LIBRARIES})
otb_module_target_label(otbFuzzyTestDriver)

# Tests Declaration

otb_add_test(NAME fzTvFuzzyVariableSetValue COMMAND otbFuzzyTestDriver
  otbFuzzyVariableSetValue)

otb_add_test(NAME fzTvFuzzyVariable2Values COMMAND otbFuzzyTestDriver
  otbFuzzyVariable2Values)

otb_add_test(NAME fzTvFuzzyVariableDSApplied COMMAND otbFuzzyTestDriver
  otbFuzzyVariableDSApplied)

otb_add_test(NAME fzTvFuzzyVariableGetMaxVar COMMAND otbFuzzyTestDriver
  otbFuzzyVariableGetMaxVar)

otb_add_test(NAME fzTvFuzzyDescriptorsModelManagerTest COMMAND otbFuzzyTestDriver
  otbFuzzyDescriptorsModelManagerTest
  ${TEMP}/fzTvFuzzyDescriptorsModelManager.xml
  )

