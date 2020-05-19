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

INCLUDE_ONCE_MACRO(PCRE)
SETUP_SUPERBUILD(PCRE)
if(MSVC)
  # TODO ?
else()
  ExternalProject_Add(PCRE
    PREFIX PCRE
    URL "https://ftp.pcre.org/pub/pcre/pcre-8.44.tar.gz"
    URL_MD5 3bcd2441024d00009a5fee43f058987c
    BINARY_DIR ${PCRE_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    CONFIGURE_COMMAND
    ${PCRE_SB_BUILD_DIR}/configure
    --prefix=${SB_INSTALL_PREFIX}
    BUILD_COMMAND $(MAKE)
    INSTALL_COMMAND $(MAKE) install
    DEPENDS ${PCRE_DEPENDENCIES}
    )

  ExternalProject_Add_Step(PCRE copy_source
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PCRE_SB_SRC} ${PCRE_SB_BUILD_DIR}
    DEPENDEES patch update
    DEPENDERS configure
    )

endif()
