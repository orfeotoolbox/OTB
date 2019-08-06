#
# Copyright (C) 2005-2019 CS Systemes d'Information (CS SI)
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

get_filename_component(PKG_PREFIX "${CTEST_SCRIPT_DIRECTORY}" PATH)
get_filename_component(PKG_PREFIX "${PKG_PREFIX}" PATH)
get_filename_component(PKG_PREFIX "${PKG_PREFIX}" PATH)

set(CTEST_PROJECT_NAME OTBPythonWrappers)
set(CTEST_SOURCE_DIRECTORY ${CTEST_SCRIPT_DIRECTORY})
set(CTEST_BINARY_DIRECTORY ${CTEST_SCRIPT_DIRECTORY}/build)
if(WIN32)
  set(CTEST_CMAKE_GENERATOR "NMake Makefiles")
else()
  set(CTEST_CMAKE_GENERATOR "Unix Makefiles")
endif()
set(CTEST_BUILD_CONFIGURATION "Release")

if(IS_DIRECTORY "${CTEST_BINARY_DIRECTORY}")
  file(REMOVE_RECURSE "${CTEST_BINARY_DIRECTORY}")
endif()

set(CONFIGURE_OPTIONS
  "-DCMAKE_INSTALL_PREFIX=${PKG_PREFIX}"
  "-DCMAKE_INSTALL_RPATH=${PKG_PREFIX}/lib"
  "-DCMAKE_BUILD_TYPE=Release")
if(PYTHON_EXECUTABLE)
  list(APPEND CONFIGURE_OPTIONS "-DPYTHON_EXECUTABLE=${PYTHON_EXECUTABLE}")
endif()

ctest_start(Experimental)
ctest_configure(OPTIONS "${CONFIGURE_OPTIONS}")
ctest_build(TARGET install)
