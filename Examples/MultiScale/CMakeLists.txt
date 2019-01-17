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

project(MultiScaleExamples)

add_executable(MorphologicalPyramidAnalysisFilterExample MorphologicalPyramidAnalysisFilterExample.cxx)
target_link_libraries(MorphologicalPyramidAnalysisFilterExample ${OTB_LIBRARIES})

add_executable(MorphologicalPyramidSegmentationExample MorphologicalPyramidSegmentationExample.cxx)
target_link_libraries(MorphologicalPyramidSegmentationExample ${OTB_LIBRARIES})

add_executable(MorphologicalPyramidSegmenterExample MorphologicalPyramidSegmenterExample.cxx)
target_link_libraries(MorphologicalPyramidSegmenterExample ${OTB_LIBRARIES})

add_executable(MorphologicalPyramidSynthesisFilterExample MorphologicalPyramidSynthesisFilterExample.cxx)
target_link_libraries(MorphologicalPyramidSynthesisFilterExample ${OTB_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
