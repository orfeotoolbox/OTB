#
# Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

# This file provides two function that init cmake vars:
# - get_test_const that init all EPSILON_X vars needed for unit tests
# - get_install_const that init all OTB_INSTALL vars

macro(get_test_const)
  set(NOTOL      0.0)
  set(EPSILON_1  0.1)
  set(EPSILON_2  0.01)
  set(EPSILON_3  0.001)
  set(EPSILON_4  0.0001)
  set(EPSILON_5  0.00001)
  set(EPSILON_6  0.000001)
  set(EPSILON_7  0.0000001)
  set(EPSILON_8  0.00000001)
  set(EPSILON_9  0.000000001)
  set(EPSILON_10 0.0000000001)
  set(EPSILON_11 0.00000000001)
  set(EPSILON_12 0.000000000001)
  set(EPSILON_13 0.0000000000001)
  set(EPSILON_14 0.00000000000001)
  set(EPSILON_15 0.000000000000001)
endmacro()

macro(get_install_const)
  if(NOT OTB_INSTALL_RUNTIME_DIR)
    set(OTB_INSTALL_RUNTIME_DIR bin)
  endif()

  if(NOT OTB_INSTALL_LIBRARY_DIR)
    set(OTB_INSTALL_LIBRARY_DIR lib)
  endif()

  if(NOT OTB_INSTALL_ARCHIVE_DIR)
    set(OTB_INSTALL_ARCHIVE_DIR lib)
  endif()
  
  if(NOT OTB_INSTALL_INCLUDE_DIR)
    set(OTB_INSTALL_INCLUDE_DIR "include/OTB-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}")
  endif()

  if(NOT OTB_INSTALL_APP_DIR)
    set(OTB_INSTALL_APP_DIR "${OTB_INSTALL_LIBRARY_DIR}/otb/applications")
  endif()

  if(NOT OTB_INSTALL_DATA_DIR)
    set(OTB_INSTALL_DATA_DIR "share/otb")
  endif()

  if(NOT OTB_INSTALL_DOC_DIR)
    set(OTB_INSTALL_DOC_DIR "share/doc/otb")
  endif()

  if(NOT OTB_INSTALL_PACKAGE_DIR)
    set(OTB_INSTALL_PACKAGE_DIR "${OTB_INSTALL_LIBRARY_DIR}/cmake/OTB-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}")
  endif()

  if(NOT OTB_INSTALL_PYTHON_DIR)
    set(OTB_INSTALL_PYTHON_DIR "${OTB_INSTALL_LIBRARY_DIR}/otb/python")
  endif()
endmacro()

macro(get_modules_const)
  if(NOT OTB_MODULE_ACTIVATION_OPTION_LIST)
    set(OTB_MODULE_ACTIVATION_OPTION_LIST "")
  endif()
endmacro()
