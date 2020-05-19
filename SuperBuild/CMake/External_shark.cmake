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

INCLUDE_ONCE_MACRO(SHARK)

SETUP_SUPERBUILD(SHARK)

# declare dependencies
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(SHARK BOOST)

ADD_SUPERBUILD_CMAKE_VAR(SHARK Boost_INCLUDE_DIR)
ADD_SUPERBUILD_CMAKE_VAR(SHARK BOOST_LIBRARYDIR)

ExternalProject_Add(SHARK
  PREFIX SHARK
  URL "https://github.com/Shark-ML/Shark/archive/67990bcd2c4a90a27be97d933b3740931e9da141.zip"
  URL_MD5 9ad7480a4f9832b63086b9a683566187
  SOURCE_DIR ${SHARK_SB_SRC}
  BINARY_DIR ${SHARK_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  DEPENDS ${SHARK_DEPENDENCIES}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
  -DBUILD_DOCS:BOOL=OFF
  -DBUILD_EXAMPLES:BOOL=OFF
  -DBUILD_TESTING:BOOL=OFF
  -DENABLE_HDF5:BOOL=OFF
  -DENABLE_CBLAS:BOOL=OFF
  -DENABLE_OPENMP:BOOL=${OTB_USE_OPENMP}
  -DSHARK_INSTALL_LIB_DIR:STRING=lib/
  ${SHARK_SB_CONFIG}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )

SUPERBUILD_PATCH_SOURCE(SHARK)

set(_SB_Shark_DIR ${SB_INSTALL_PREFIX}/lib/cmake/Shark)
