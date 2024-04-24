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

INCLUDE_ONCE_MACRO(SWIG)

SETUP_SUPERBUILD(SWIG)

if( Python_EXECUTABLE )
  set(SWIG_SB_PYTHON_CONFIG "--with-python=${Python_EXECUTABLE}")
else()
  set(SWIG_SB_PYTHON_CONFIG)
  message(WARNING "  No suitable python interpreter was found !")
endif()

if(MSVC)
  # Use pre-built swig executable (no linking is required, no install done)
  ExternalProject_Add(SWIG
    PREFIX SWIG
    URL "https://downloads.sourceforge.net/project/swig/swigwin/swigwin-4.1.1/swigwin-4.1.1.zip"
    URL_MD5 223a7622e1ad89895d578249fdfa36cf
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    DEPENDS ${SWIG_DEPENDENCIES}
    )

else()
  # declare dependencies
  ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(SWIG PCRE BOOST)

  ADD_SUPERBUILD_CONFIGURE_VAR(SWIG PCRE_ROOT --with-pcre-prefix)
  ADD_SUPERBUILD_CONFIGURE_VAR(SWIG BOOST_ROOT --with-boost)

  ExternalProject_Add(SWIG
    PREFIX SWIG
    URL "https://downloads.sourceforge.net/project/swig/swig/swig-4.1.1/swig-4.1.1.tar.gz"
    URL_MD5 c7d55a1bca26752f3846c85b43c1a69c
    BINARY_DIR ${SWIG_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    CONFIGURE_COMMAND
    ${SWIG_SB_BUILD_DIR}/configure
    --prefix=${SB_INSTALL_PREFIX}
    ${SWIG_SB_PYTHON_CONFIG}
    ${SWIG_SB_CONFIG}
    BUILD_COMMAND $(MAKE)
    INSTALL_COMMAND $(MAKE) install
    DEPENDS ${SWIG_DEPENDENCIES}
    LOG_DOWNLOAD 1
    LOG_CONFIGURE 1
    LOG_BUILD 1
    LOG_INSTALL 1
    )

  ExternalProject_Add_Step(SWIG copy_source
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SWIG_SB_SRC} ${SWIG_SB_BUILD_DIR}
    DEPENDEES patch update
    DEPENDERS configure
    )

endif()

if(WIN32)
  set(_SB_SWIG_EXECUTABLE ${CMAKE_BINARY_DIR}/SWIG/src/SWIG/swig.exe)
elseif(UNIX)
  set(_SB_SWIG_EXECUTABLE ${SB_INSTALL_PREFIX}/bin/swig)
endif()
