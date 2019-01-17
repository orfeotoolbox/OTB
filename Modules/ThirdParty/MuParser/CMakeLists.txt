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

project(OTBMuParser)

set(OTBMuParser_SYSTEM_INCLUDE_DIRS ${MUPARSER_INCLUDE_DIRS})
set(OTBMuParser_LIBRARIES "${MUPARSER_LIBRARIES}")
set(OTBMuParser_INCLUDE_DIRS ${OTBMuParser_BINARY_DIR}/src)

set(OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS 0)
if(NOT MUPARSER_VERSION_NUMBER LESS 20000)
set(OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS 1)
endif()

# Starting with muparser 2.0.0,
# intrinsic operators "and", "or", "xor" have been removed
#  and intrinsic operators "&&" and "||" have been introduced as replacements
# include(CheckCXXSourceRuns)
# set(CMAKE_REQUIRED_INCLUDES ${OTBMuParser_SYSTEM_INCLUDE_DIRS})
# set(CMAKE_REQUIRED_LIBRARIES ${OTBMuParser_LIBRARIES})
# unset(CMAKE_REQUIRED_FLAGS)
# unset(CMAKE_REQUIRED_DEFINES)
# file(READ ${OTBMuParser_SOURCE_DIR}/CMake/otbTestMuParserHasCxxLogicalOperators.cxx
  # OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS_SOURCEFILE)
# check_cxx_source_runs(
  # "${OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS_SOURCEFILE}"
  # OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS
  # )
# unset(CMAKE_REQUIRED_INCLUDES)
# unset(CMAKE_REQUIRED_LIBRARIES)
# unset(CMAKE_REQUIRED_FLAGS)
# unset(CMAKE_REQUIRED_DEFINES)

if(OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS)
  message(STATUS "MuParser version is  ${MUPARSER_VERSION} >= 2.0.0 : "
    "uses '&&' and '||' logical operators, and C++ like "
    "ternary if-then-else operator")
else()
  message(STATUS "MuParser version is ${MUPARSER_VERSION} < 2.0.0  : "
    "uses 'and' and 'or' logical operators, and ternary "
    "operator 'if( ; ; )'")
endif()

configure_file( src/otb_muparser.h.in src/otb_muparser.h )

otb_module_impl()

install(FILES ${OTBMuParser_BINARY_DIR}/src/otb_muparser.h
  DESTINATION ${OTBMuParser_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
