#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

add_library(OTBTestKernel otbTestHelper.cxx otbTestTools.cxx)
target_link_libraries(OTBTestKernel 
  ${OTBGdalAdapters_LIBRARIES}
  ${OTBImageIO_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBGDAL_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  )
otb_module_target(OTBTestKernel COMPONENT_Core)

add_executable(otbTestDriver otbTestDriver.cxx)
target_link_libraries(otbTestDriver OTBTestKernel)
otb_module_target(otbTestDriver COMPONENT_Core)
