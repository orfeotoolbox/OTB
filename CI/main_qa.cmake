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

include( "${CMAKE_CURRENT_LIST_DIR}/macros.cmake" )

set( ENV{LANG} "C" ) # Only ascii output
get_filename_component( OTB_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}" DIRECTORY )

set( DEBUG "1" )
set( CMAKE_COMMAND "cmake" )
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

# retrieve XDK
get_xdk()

set( INSTALL_DIR "${XDK_PATH}" )

# FIX ME this part might platform dependent
set( GDAL_DATA "${XDK_PATH}/share/gdal" )
set( GEOTIFF_CSV "${XDK_PATH}/share/epsg_csv" )
set( PROJ_LIB "${XDK_PATH}/share" )
set( CTEST_ENVIRONMENT "PATH=${XDK_PATH}/lib:${XDK_PATH}/bin:$ENV{PATH}" )

set( ci_do_cookbook -1 )
set( ci_do_doxygen -1 )

include( "${CMAKE_CURRENT_LIST_DIR}/main_ci.cmake" )
