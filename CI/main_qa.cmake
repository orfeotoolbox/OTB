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


get_filename_component( OTB_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}" DIRECTORY )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set( ci_build_type "Debug" )
set( CTEST_TEST_TIMEOUT 1500)

include( "${CMAKE_CURRENT_LIST_DIR}/main_superbuild.cmake" )
