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

# these are cache variables, so they could be overwritten with -D,
# Add automatically generated CMakeConfig
message(STATUS "Component list ${CPACK_COMPONENTS_ALL}")
set(CPACK_COMPONENTS_GROUPING ONE_PER_GROUP)

set(CPACK_PACKAGE_NAME "Core"
    CACHE STRING "Main Group for Core Modules of OTB"
)
# which is useful in case of packing only selected components instead of the whole thing
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Core OTB Package"
    CACHE STRING "This package contains the Core module of OTB. All other modules depends of this one."
)
set(CPACK_PACKAGE_VENDOR "CS Group")

set(CPACK_VERBATIM_VARIABLES YES)

set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_BINARY_DIR}")

set(CPACK_PACKAGE_VERSION_MAJOR ${OTB_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${OTB_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${OTB_VERSION_PATCH})

# custom package name to stick to what is already done in OTB
set(CPACK_PACKAGE_FILE_NAME "OTB-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}.${OTB_VERSION_PATCH}-${CMAKE_SYSTEM_NAME}-${CPACK_PACKAGE_NAME}")

set(CPACK_PACKAGE_CONTACT "thibaut.romain@cs-soprasteria.com")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Thibaut ROMAIN")

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "TGZ")
include(CPack)
