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

project(BasicFiltersExamples)

#OTB_USE_MUPARSER is OFF
if(OTBMathParser_LOADED)
add_executable(BandMathFilterExample BandMathFilterExample.cxx)
target_link_libraries(BandMathFilterExample ${OTB_LIBRARIES})
endif()

#OTB_USE_MUPARSERX is OFF
if(OTBMathParserX_LOADED)
add_executable(BandMathXImageFilterExample BandMathXImageFilterExample.cxx)
target_link_libraries(BandMathXImageFilterExample ${OTB_LIBRARIES})
endif()

add_executable(DEMToRainbowExample DEMToRainbowExample.cxx)
target_link_libraries(DEMToRainbowExample ${OTB_LIBRARIES})

add_executable(FrostImageFilter FrostImageFilter.cxx)
target_link_libraries(FrostImageFilter ${OTB_LIBRARIES})

add_executable(HillShadingExample HillShadingExample.cxx)
target_link_libraries(HillShadingExample ${OTB_LIBRARIES})

add_executable(IndexedToRGBExample IndexedToRGBExample.cxx)
target_link_libraries(IndexedToRGBExample ${OTB_LIBRARIES})

add_executable(LeeImageFilter LeeImageFilter.cxx)
target_link_libraries(LeeImageFilter ${OTB_LIBRARIES})

#OTBMeanShift depends OTBMathParser. But OTB_USE_MUPARSER is OFF
if(OTBMathParser_LOADED)
add_executable(MeanShiftSegmentationFilterExample MeanShiftSegmentationFilterExample.cxx)
target_link_libraries(MeanShiftSegmentationFilterExample ${OTB_LIBRARIES})
endif()

add_executable(PrintableImageFilterExample PrintableImageFilterExample.cxx)
target_link_libraries(PrintableImageFilterExample ${OTB_LIBRARIES})

add_executable(ScalingFilterExample ScalingFilterExample.cxx)
target_link_libraries(ScalingFilterExample ${OTB_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
