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
set(OTBObjectListTests
  otbObjectList.cxx
  otbObjectList2.cxx
  otbObjectListTestDriver.cxx  )

add_executable(otbObjectListTestDriver ${OTBObjectListTests})
target_link_libraries(otbObjectListTestDriver ${OTBObjectList-Test_LIBRARIES})
otb_module_target_label(otbObjectListTestDriver)

otb_add_test(NAME coTvObjectList COMMAND otbObjectListTestDriver
  otbObjectList
  ${INPUTDATA}/couleurs.jpg
  ${INPUTDATA}/poupees.png
  ${INPUTDATA}/couleurs_extrait.png
  )

otb_add_test(NAME coTvObjectListTestNotValid COMMAND otbObjectListTestDriver
  otbObjectList2
  )

