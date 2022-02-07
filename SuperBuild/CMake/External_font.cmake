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

get_filename_component(BUILD_DIR ${BUILD_DIR} DIRECTORY)

if( NOT DOWNLOAD_LOCATION )
  set (DOWNLOAD_LOCATION ${BUILD_DIR}/src/)
endif()

file(MAKE_DIRECTORY ${INSTALL_DIR}/lib/fonts/)

if ( EXISTS "${DOWNLOAD_LOCATION}/noto-sans.zip" )
  file( MD5 "${DOWNLOAD_LOCATION}/noto-sans.zip" MD5_FONT )
endif()

if ( NOT ( "${MD5_FONT}" STREQUAL "6c4f1b790d2d7bdac917d5047ea1898d" ) )
MESSAGE("DOWNLOADING FONTS")
file(DOWNLOAD 
  "https://www.1001fonts.com/download/noto-sans.zip"
  ${DOWNLOAD_LOCATION}/noto-sans.zip
  EXPECTED_MD5 6c4f1b790d2d7bdac917d5047ea1898d)
endif()

file(MAKE_DIRECTORY ${BUILD_DIR}/fonts/)

execute_process(
COMMAND ${CMAKE_COMMAND} -E tar xzf ${DOWNLOAD_LOCATION}/noto-sans.zip 
WORKING_DIRECTORY ${BUILD_DIR}/fonts )

file( COPY ${BUILD_DIR}/fonts/.
  DESTINATION ${INSTALL_DIR}/lib/fonts/.)
