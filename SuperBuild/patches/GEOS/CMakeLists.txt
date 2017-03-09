#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

cmake_minimum_required(VERSION 2.8)

project(GEOS_SUPERBUILD)

add_custom_target(build_with_nmake 
ALL DEPENDS 
COMMAND
nmake /f ${GEOS_SB_SRC}/makefile.vc
WORKING_DIRECTORY ${GEOS_SB_SRC}
)

install(FILES  ${GEOS_SB_SRC}/capi/geos_c.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(FILES  ${GEOS_SB_SRC}/include/geos.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(DIRECTORY ${GEOS_SB_SRC}/include/geos DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(FILES  ${GEOS_SB_SRC}/src/geos.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES  ${GEOS_SB_SRC}/src/geos_c.dll DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES  ${GEOS_SB_SRC}/src/geos.lib DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES  ${GEOS_SB_SRC}/src/geos_i.lib DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES  ${GEOS_SB_SRC}/src/geos_c_i.lib DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
