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

project(DisparityMapExamples)

add_executable(FineRegistrationImageFilterExample FineRegistrationImageFilterExample.cxx)
target_link_libraries(FineRegistrationImageFilterExample ${OTB_LIBRARIES})

add_executable(NCCRegistrationFilterExample NCCRegistrationFilterExample.cxx)
target_link_libraries(NCCRegistrationFilterExample ${OTB_LIBRARIES})

#StereoReconstructionExample depends OTBMathParser. But OTB_USE_MUPARSER is OFF
if(OTBMathParser_LOADED)
add_executable(StereoReconstructionExample StereoReconstructionExample.cxx)
target_link_libraries(StereoReconstructionExample ${OTB_LIBRARIES})
endif()

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
