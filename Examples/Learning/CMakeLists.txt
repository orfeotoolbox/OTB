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

project(LearningExamples)

add_executable(GenerateTrainingImageExample GenerateTrainingImageExample.cxx)
target_link_libraries(GenerateTrainingImageExample ${OTB_LIBRARIES})

add_executable(SEMModelEstimatorExample SEMModelEstimatorExample.cxx)
target_link_libraries(SEMModelEstimatorExample ${OTB_LIBRARIES})

add_executable(SOMClassifierExample SOMClassifierExample.cxx)
target_link_libraries(SOMClassifierExample ${OTB_LIBRARIES})

add_executable(SOMExample SOMExample.cxx)
target_link_libraries(SOMExample ${OTB_LIBRARIES})

if(OTBLibSVM_LOADED)
add_executable(SVMImageEstimatorClassificationMultiExample SVMImageEstimatorClassificationMultiExample.cxx)
target_link_libraries(SVMImageEstimatorClassificationMultiExample ${OTB_LIBRARIES})
endif()

if(OTBOpenCV_LOADED)
add_executable(TrainMachineLearningModelFromImagesExample TrainMachineLearningModelFromImagesExample.cxx)
target_link_libraries(TrainMachineLearningModelFromImagesExample ${OTB_LIBRARIES})

add_executable(TrainMachineLearningModelFromSamplesExample TrainMachineLearningModelFromSamplesExample.cxx)
target_link_libraries(TrainMachineLearningModelFromSamplesExample ${OTB_LIBRARIES})
endif()

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
