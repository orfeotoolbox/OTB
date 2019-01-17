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

project(OTBCurl)

try_compile(OTB_CURL_MULTI_AVAILABLE
              ${CMAKE_CURRENT_BINARY_DIR}/CMake
              ${CMAKE_CURRENT_SOURCE_DIR}/CMake/otbTestCurlMulti.cxx
              CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${CURL_INCLUDE_DIRS}" "-DLINK_LIBRARIES:STRING=${CURL_LIBRARIES}"
              OUTPUT_VARIABLE OUTPUT)

configure_file( src/otb_curl.h.in src/otb_curl.h )

set(OTBCurl_SYSTEM_INCLUDE_DIRS ${CURL_INCLUDE_DIRS})
set(OTBCurl_LIBRARIES "${CURL_LIBRARIES}")
set(OTBCurl_INCLUDE_DIRS
  ${OTBCurl_BINARY_DIR}/src
  )

otb_module_impl()

install(FILES ${OTBCurl_BINARY_DIR}/src/otb_curl.h
  DESTINATION ${OTBCurl_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
