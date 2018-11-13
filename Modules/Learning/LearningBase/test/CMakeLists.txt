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

set(OTBLearningBaseTests
otbLearningBaseTestDriver.cxx
otbDecisionTreeBuild.cxx
otbKMeansImageClassificationFilter.cxx
otbDecisionTreeWithRealValues.cxx
)

if(OTB_USE_SHARK)
  set(OTBLearningBaseTests ${OTBLearningBaseTests} otbSharkUtilsTests.cxx)
endif()

add_executable(otbLearningBaseTestDriver ${OTBLearningBaseTests})
target_link_libraries(otbLearningBaseTestDriver ${OTBLearningBase-Test_LIBRARIES})
otb_module_target_label(otbLearningBaseTestDriver)

# Tests Declaration

otb_add_test(NAME leTvDecisionTreeBuild COMMAND otbLearningBaseTestDriver
  otbDecisionTreeBuild)

otb_add_test(NAME leTvDecisionTreeWithRealValues COMMAND otbLearningBaseTestDriver
  otbDecisionTreeWithRealValues)

otb_add_test(NAME leTvKMeansImageClassificationFilter COMMAND otbLearningBaseTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leKMeansImageClassificationFilterOutput.tif
  ${TEMP}/leKMeansImageClassificationFilterOutput.tif
  otbKMeansImageClassificationFilter
  ${INPUTDATA}/poupees_sub.png
  ${TEMP}/leKMeansImageClassificationFilterOutput.tif
  2
  0 0 0 0
  255 255 255 255
  )

if(OTB_USE_SHARK)
  otb_add_test(NAME leTuSharkNormalizeLabels COMMAND otbLearningBaseTestDriver
    otbSharkNormalizeLabels)
endif()
