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

project(ChangeDetectionExamples)

add_executable(CorrelChDet CorrelChDet.cxx)
target_link_libraries(CorrelChDet ${OTB_LIBRARIES})

add_executable(DiffChDet DiffChDet.cxx)
target_link_libraries(DiffChDet ${OTB_LIBRARIES})

add_executable(JHMIChDet JHMIChDet.cxx)
target_link_libraries(JHMIChDet ${OTB_LIBRARIES})

add_executable(KullbackLeiblerDistanceChDet KullbackLeiblerDistanceChDet.cxx)
target_link_libraries(KullbackLeiblerDistanceChDet ${OTB_LIBRARIES})

add_executable(KullbackLeiblerProfileChDet KullbackLeiblerProfileChDet.cxx)
target_link_libraries(KullbackLeiblerProfileChDet ${OTB_LIBRARIES})

add_executable(KullbackLeiblerSupervizedDistanceChDet KullbackLeiblerSupervizedDistanceChDet.cxx)
target_link_libraries(KullbackLeiblerSupervizedDistanceChDet ${OTB_LIBRARIES})

add_executable(LHMIChDet LHMIChDet.cxx)
target_link_libraries(LHMIChDet ${OTB_LIBRARIES})

add_executable(MultivariateAlterationDetector MultivariateAlterationDetector.cxx)
target_link_libraries(MultivariateAlterationDetector ${OTB_LIBRARIES})

add_executable(RatioChDet RatioChDet.cxx)
target_link_libraries(RatioChDet ${OTB_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
