#
# Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

if(BUILD_TESTING)
  enable_testing()
endif()

add_subdirectory(ThirdParty)
add_subdirectory(Core)
include("OTBInternalModules.cmake")

if (OTB_BUILD_Remote OR OTBGroup_Remote)
  # ensure option are ON if used later in code
  option(OTB_BUILD_Remote "Request building Remote modules" TRUE)
  option(OTBGroup_Remote "Request building Remote modules" TRUE)
  add_subdirectory(Remote)
endif()

# NOTE TLA: this should be refactored and put in a separate function
# Add the QGIS Wrapper at the last minute as it need all the OTB applications
# to be declared
add_subdirectory(Core/Wrappers/QGIS)
