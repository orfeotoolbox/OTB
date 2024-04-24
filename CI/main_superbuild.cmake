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
# This script is a prototype for the future CI, it may evolve rapidly in a near future

include( "${CMAKE_CURRENT_LIST_DIR}/macros.cmake" )

set (ENV{LANG} "C") # Only ascii output
get_filename_component(OTB_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}" DIRECTORY)

set ( DEBUG "1" )
set (CMAKE_COMMAND "cmake")

# retrieve XDK
get_xdk()

if(WIN32)
  file(TO_NATIVE_PATH "${XDK_INSTALL_PATH}" XDK_INSTALL_PATH_NATIVE)
  file(TO_NATIVE_PATH "${CTEST_BINARY_DIRECTORY}/bin" OTB_BUILD_BIN_DIR_NATIVE)
  set(ENV{PATH} "$ENV{PATH};${OTB_BUILD_BIN_DIR_NATIVE}" )
  set(ENV{PATH} "${XDK_INSTALL_PATH_NATIVE}\\bin;$ENV{PATH}" )
  set(ENV{PATH} "$ENV{PATH};${XDK_INSTALL_PATH_NATIVE}\\lib" )
  set(ENV{GDAL_DATA} "${XDK_INSTALL_PATH_NATIVE}\\data" )
  set(ENV{PROJ_LIB} "${XDK_INSTALL_PATH_NATIVE}\\share\\proj" )
  set( CTEST_ENVIRONMENT
"PATH=$ENV{PATH}
GDAL_DATA=$ENV{GDAL_DATA}
PROJ_LIB=$ENV{PROJ_LIB}
")
else()
  set(ENV{PATH} "${XDK_INSTALL_PATH}/lib:${XDK_INSTALL_PATH}/bin:$ENV{PATH}" )
  set( GDAL_DATA "${XDK_INSTALL_PATH}/share/gdal" )
  set( PROJ_LIB "${XDK_INSTALL_PATH}/share" )
  set( CTEST_ENVIRONMENT
"PATH=$ENV{PATH}
")
endif()

include( "${CMAKE_CURRENT_LIST_DIR}/main_ci.cmake" )
