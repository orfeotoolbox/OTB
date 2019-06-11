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

project(RadiometryExamples)

add_executable(ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter.cxx)
target_link_libraries(ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter ${OTB_LIBRARIES})

#OTBOpticalCalibration -> OTB6S
if(OTB6S_LOADED)
add_executable(AtmosphericCorrectionSequencement AtmosphericCorrectionSequencement.cxx)
target_link_libraries(AtmosphericCorrectionSequencement ${OTB_LIBRARIES})
if(MSVC)
  set_target_properties(AtmosphericCorrectionSequencement PROPERTIES LINK_FLAGS "/STACK:10000000")
elseif(MINGW)
  set_target_properties(AtmosphericCorrectionSequencement PROPERTIES LINK_FLAGS "-Wl,--stack,10000000")
endif()
endif()

add_executable(AVIMultiChannelRAndGAndNIRVegetationIndexImageFilter AVIMultiChannelRAndGAndNIRVegetationIndexImageFilter.cxx)
target_link_libraries(AVIMultiChannelRAndGAndNIRVegetationIndexImageFilter ${OTB_LIBRARIES})

add_executable(NDVIRAndNIRVegetationIndexImageFilter NDVIRAndNIRVegetationIndexImageFilter.cxx)
target_link_libraries(NDVIRAndNIRVegetationIndexImageFilter ${OTB_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
