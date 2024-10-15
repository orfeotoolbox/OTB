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

# Configuration options for ubuntu-22.04-fast

set(site_option
"CMAKE_C_COMPILER:STRING=gcc
CMAKE_CXX_COMPILER:STRING=g++
CMAKE_C_COMPILER_LAUNCHER:STRING=ccache
CMAKE_CXX_COMPILER_LAUNCHER:STRING=ccache
OTB_USE_SHARK:BOOL=OFF
BUILD_EXAMPLES:BOOL=OFF")

set(ci_skip_testing ON)

set(ci_skip_install ON)
