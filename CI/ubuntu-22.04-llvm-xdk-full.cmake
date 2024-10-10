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

# Configuration options for ubuntu-20.04-llvm-xdk

set(site_option
"CMAKE_C_COMPILER:STRING=clang
CMAKE_CXX_COMPILER:STRING=clang++
CMAKE_EXE_LINKER_FLAGS:STRING=-fuse-ld=lld
CMAKE_MODULE_LINKER_FLAGS:STRING=-fuse-ld=lld
CMAKE_SHARED_LINKER_FLAGS:STRING=-fuse-ld=lld
CMAKE_C_COMPILER_LAUNCHER:STRING=ccache
CMAKE_CXX_COMPILER_LAUNCHER:STRING=ccache
OTB_BUILD_FeaturesExtraction:BOOL=ON
OTB_BUILD_Hyperspectral:BOOL=ON
OTB_BUILD_Learning:BOOL=ON
OTB_BUILD_Miscellaneous:BOOL=ON
OTB_BUILD_Remote:BOOL=ON
OTB_BUILD_SAR:BOOL=ON
OTB_BUILD_Segmentation:BOOL=ON 
OTB_BUILD_StereoProcessing:BOOL=ON
OTB_USE_LIBSVM:BOOL=ON
OTB_USE_MUPARSER:BOOL=ON
OTB_USE_MUPARSERX:BOOL=ON
OTB_USE_OPENCV:BOOL=ON
OTB_USE_OPENMP:BOOL=OFF
OTB_USE_SHARK:BOOL=ON
")

if(NOT ${ci_do_cookbook} EQUAL -1)
  set(site_option
"${site_option}
BUILD_COOKBOOK:BOOL=ON")
endif()

if(NOT ${ci_do_doxygen} EQUAL -1)
  set(site_option
"${site_option}
BUILD_DOCUMENTATION:BOOL=ON
OTB_DOXYGEN_ITK_TAGFILE:FILEPATH=${CTEST_BINARY_DIRECTORY}/InsightDoxygenDocTag-5.3.0
OTB_DOXYGEN_ITK_DOXYGEN_URL:STRING=\"https://itk.org/Doxygen53/html\"
")
  set (ENABLE_DOXYGEN ON)
  # The ITK doxygen tag file needs to be patched before being used for OTB
  # See otb-devutils/Scripts/tagfile_fix.py
  message(STATUS "Get resources for Doxygen build ...")
  execute_process(COMMAND wget https://www.orfeo-toolbox.org/packages/archives/Doxygen/InsightDoxygenDocTag-5.3.0.gz
                  WORKING_DIRECTORY ${CTEST_BINARY_DIRECTORY})
  execute_process(COMMAND gzip -d InsightDoxygenDocTag-5.3.0.gz
                  WORKING_DIRECTORY ${CTEST_BINARY_DIRECTORY})
endif()