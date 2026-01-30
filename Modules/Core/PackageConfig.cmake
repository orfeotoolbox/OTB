#
# Copyright (C) 2005-2025 Centre National d'Etudes Spatiales (CNES)
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

# get the package constants used in cpack
get_package_const()

# Adapt License and readme path
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

include(CPack)

# cpack_add_component_group(Core
#                          DISPLAY_NAME Core
#                          DESCRIPTION "Main Group for Core of OTB" 
#                          EXPANDED
#                          BOLD_TITLE)

cpack_add_component(Core
                    DISPLAY_NAME Core
                    DESCRIPTION "Main Group for Core of OTB"
                    REQUIRED)
