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

set(OTBDimensionalityReductionLearningTests
otbDimensionalityReductionLearningTestDriver.cxx
otbSOMModelTest.cxx
)


# Tests Declaration

# Tests for classes using Shark

if(OTB_USE_SHARK)
set(OTBDimensionalityReductionLearningTests ${OTBDimensionalityReductionLearningTests} 
    otbPCAModelTest.cxx
    otbAutoencoderModelTest.cxx
)
include(tests-shark.cmake)
endif()


# --------------- SOM --------------------------------

otb_add_test(NAME leTvSOMModelTrain COMMAND
  otbDimensionalityReductionLearningTestDriver
  otbSOMModeTrain
  ${INPUTDATA}/letter_light.scale
  ${TEMP}/model2D.som
  ${TEMP}/model3D.som
  ${TEMP}/model4D.som
  ${TEMP}/model5D.som
  )

otb_add_test(NAME leTvSOMModelCanRead COMMAND
  otbDimensionalityReductionLearningTestDriver
  otbSOMModelCanRead
  ${TEMP}/model2D.som
  ${TEMP}/model3D.som
  ${TEMP}/model4D.som
  ${TEMP}/model5D.som
  )

set_property(TEST leTvSOMModelCanRead APPEND PROPERTY DEPENDS leTvSOMModelTrain)

add_executable(otbDimensionalityReductionLearningTestDriver ${OTBDimensionalityReductionLearningTests})
target_link_libraries(otbDimensionalityReductionLearningTestDriver ${OTBDimensionalityReductionLearning-Test_LIBRARIES})
otb_module_target_label(otbDimensionalityReductionLearningTestDriver)
