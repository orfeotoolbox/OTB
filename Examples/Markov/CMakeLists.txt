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

project(MarkovExamples)

add_executable(MarkovClassification1Example MarkovClassification1Example.cxx)
target_link_libraries(MarkovClassification1Example ${OTB_LIBRARIES})

add_executable(MarkovClassification2Example MarkovClassification2Example.cxx)
target_link_libraries(MarkovClassification2Example ${OTB_LIBRARIES})

add_executable(MarkovClassification3Example MarkovClassification3Example.cxx)
target_link_libraries(MarkovClassification3Example ${OTB_LIBRARIES})

add_executable(MarkovRegularizationExample MarkovRegularizationExample.cxx)
target_link_libraries(MarkovRegularizationExample ${OTB_LIBRARIES})

add_executable(MarkovRestorationExample MarkovRestorationExample.cxx)
target_link_libraries(MarkovRestorationExample ${OTB_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
