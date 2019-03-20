#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

# This script is a prototype for the future CI, it may evolve rapidly in a near future
#This file set the following variable : 
# * otb_build_project_option
# * otb_use_option
# * otb_wrap_option
# * CONFIGURE_OPTIONS

set (otb_build_project_option 
"BUILD_COOKBOOK:BOOL=OFF
BUILD_EXAMPLES:BOOL=ON
BUILD_SHARED_LIBS:BOOL=ON
BUILD_TESTING:BOOL=ON")

set (otb_use_option
"OTB_USE_6S:BOOL=ON
OTB_USE_CURL:BOOL=ON
OTB_USE_GLEW:BOOL=ON
OTB_USE_GLFW:BOOL=ON
OTB_USE_GLUT:BOOL=ON
OTB_USE_GSL:BOOL=ON
OTB_USE_LIBKML:BOOL=ON
OTB_USE_LIBSVM:BOOL=ON
OTB_USE_MPI:BOOL=ON
OTB_USE_MUPARSER:BOOL=ON
OTB_USE_MUPARSERX:BOOL=ON
OTB_USE_OPENCV:BOOL=ON
OTB_USE_OPENGL:BOOL=ON
OTB_USE_OPENMP:BOOL=OFF
OTB_USE_QT:BOOL=ON
OTB_USE_QWT:BOOL=ON
OTB_USE_SHARK:BOOL=ON
OTB_USE_SIFTFAST:BOOL=ON
OTB_USE_SPTW:BOOL=ON
OTB_USE_SSE_FLAGS:BOOL=ON")

set (otb_wrap_option
"OTB_WRAP_JAVA:BOOL=ON
OTB_WRAP_PYTHON:BOOL=ON")

set (otb_data_option
"OTB_DATA_USE_LARGEINPUT:BOOL=OFF
OTB_DATA_LARGEINPUT_ROOT:PATH=${OTB_LARGEINPUT_ROOT}")

set (cmake_configure_option
"CMAKE_BUILD_TYPE=${CTEST_BUILD_CONFIGURATION}
CMAKE_INSTALL_PREFIX:PATH=${CTEST_INSTALL_DIRECTORY}")

if((CTEST_SITE) AND EXISTS "${CMAKE_CURRENT_LIST_DIR}/${CTEST_SITE}.cmake")
  # will set its output in 'site_option'
  include("${CMAKE_CURRENT_LIST_DIR}/${CTEST_SITE}.cmake")
endif()

set(concat_options
"${otb_build_project_option}
${otb_use_option}
${otb_wrap_option}
${otb_data_option}
${cmake_configure_option}
${site_option}
")

#Transform the previous string in list
string (REPLACE "\n" ";" otb_options ${concat_options})

foreach(item ${otb_options})
  set( CONFIGURE_OPTIONS "${CONFIGURE_OPTIONS}-D${item};")
endforeach(item)
