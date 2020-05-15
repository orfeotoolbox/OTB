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

cmake_minimum_required(VERSION 2.8.8)

project(OTB-OSSIM)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/ossim_package_support/cmake/CMakeModules")

#mantis: 1193
#RK: do we need to do this for other dependencies?
if(APPLE)
  set(CMAKE_SYSTEM_FRAMEWORK_PATH "" CACHE PATH "" FORCE)
endif()
message(STATUS "[OTB SuperBuild] CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}")
add_subdirectory(ossim)
