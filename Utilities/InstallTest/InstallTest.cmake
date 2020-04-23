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

if(NOT DEFINED OTB_BINARY_DIR)
  message(FATAL_ERROR "Invoke with -DOTB_BINARY_DIR=/path/to/OTB/build")
endif()
if(NOT DEFINED CONFIGURATION)
  message(FATAL_ERROR "Invoke with -DCONFIGURATION=<config>")
endif()
message(STATUS "Removing InstallTest directory...")
file(REMOVE_RECURSE "${OTB_BINARY_DIR}/InstallTest")
message(STATUS "Building 'install' target...")
execute_process(
  COMMAND ${CMAKE_COMMAND} --build "${OTB_BINARY_DIR}"
                           --target install
                           --config "${CONFIGURATION}"
  RESULT_VARIABLE failed
  )
if(failed)
  message(FATAL_ERROR "Installation failed: ${failed}")
else()
  file(WRITE "${OTB_BINARY_DIR}/InstallTest/InstallSucceeded.txt" "# Installation succeeded!\n")
endif()
