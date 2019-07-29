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

get_filename_component(BUILD_DIR ${BUILD_DIR} DIRECTORY)

if( NOT DOWNLOAD_LOCATION )
  set (DOWNLOAD_LOCATION ${BUILD_DIR}/src/)
endif()

file(MAKE_DIRECTORY ${INSTALL_DIR}/lib/fonts/)

if ( EXISTS "${DOWNLOAD_LOCATION}/dejavu-fonts-ttf-2.37.tar.bz2" )
  file( MD5 "${DOWNLOAD_LOCATION}/dejavu-fonts-ttf-2.37.tar.bz2" MD5_FONT )
endif()

if ( NOT ( "${MD5_FONT}" STREQUAL "d0efec10b9f110a32e9b8f796e21782c" ) )
MESSAGE("DOWNLOADING FONTS")
file(DOWNLOAD 
  "https://sourceforge.net/projects/dejavu/files/dejavu/2.37/dejavu-fonts-ttf-2.37.tar.bz2"
  ${DOWNLOAD_LOCATION}/dejavu-fonts-ttf-2.37.tar.bz2
  EXPECTED_MD5 d0efec10b9f110a32e9b8f796e21782c)
endif()

execute_process(
COMMAND ${CMAKE_COMMAND} -E tar -xf ${DOWNLOAD_LOCATION}/dejavu-fonts-ttf-2.37.tar.bz2 
WORKING_DIRECTORY ${BUILD_DIR} )

file( COPY ${BUILD_DIR}/dejavu-fonts-ttf-2.37/ttf/.
  DESTINATION ${INSTALL_DIR}/lib/fonts/.)
