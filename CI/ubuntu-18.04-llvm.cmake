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

# Configuration options for ubuntu-18.04-llvm

set(site_option
"opencv_INCLUDE_DIR:PATH=/usr/include
CMAKE_C_COMPILER:STRING=clang
CMAKE_CXX_COMPILER:STRING=clang++
CMAKE_EXE_LINKER_FLAGS:STRING=-fuse-ld=lld
CMAKE_MODULE_LINKER_FLAGS:STRING=-fuse-ld=lld
CMAKE_SHARED_LINKER_FLAGS:STRING=-fuse-ld=lld
CMAKE_C_COMPILER_LAUNCHER:STRING=ccache
CMAKE_CXX_COMPILER_LAUNCHER:STRING=ccache
OTB_USE_SHARK:BOOL=OFF")

if(NOT ${ci_do_cookbook} EQUAL -1)
  set(site_option
"${site_option}
BUILD_COOKBOOK:BOOL=ON")
endif()

if(NOT ${ci_do_doxygen} EQUAL -1)
  set(site_option
"${site_option}
BUILD_DOCUMENTATION:BOOL=ON
OTB_DOXYGEN_ITK_TAGFILE:FILEPATH=${CTEST_BINARY_DIRECTORY}/InsightDoxygenDocTag-4.13.0
OTB_DOXYGEN_ITK_DOXYGEN_URL:STRING=\"https://itk.org/Doxygen413/html\"
")
  set (ENABLE_DOXYGEN ON)
  # The ITK doxygen tag file needs to be patched before being used for OTB
  # See otb-devutils/Scripts/tagfile_fix.py
  message(STATUS "Get resources for Doxygen build ...")
  execute_process(COMMAND wget https://www.orfeo-toolbox.org/packages/archives/Doxygen/InsightDoxygenDocTag-4.13.0.gz
                  WORKING_DIRECTORY ${CTEST_BINARY_DIRECTORY})
  execute_process(COMMAND gzip -d InsightDoxygenDocTag-4.13.0.gz
                  WORKING_DIRECTORY ${CTEST_BINARY_DIRECTORY})
endif()
