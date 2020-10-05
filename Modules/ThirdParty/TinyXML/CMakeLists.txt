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

project(OTBTinyXML)

# Check if we need to define TIXML_USE_STL
try_compile( TIXML_TEST_STL
             ${CMAKE_CURRENT_BINARY_DIR}/CMake
             ${CMAKE_CURRENT_SOURCE_DIR}/CMake/otbTestTinyXMLUseSTL.cxx
             CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${TINYXML_INCLUDE_DIRS}" "-DLINK_LIBRARIES:STRING=${TINYXML_LIBRARIES}"
             COMPILE_DEFINITIONS "-DTIXML_USE_STL"
             OUTPUT_VARIABLE OUTPUT)

if (NOT TIXML_TEST_STL)
    message(FATAL_ERROR "TIXML must be compiled with TIXML_USE_STL=ON.")
endif()

configure_file( src/otb_tinyxml.h.in src/otb_tinyxml.h )

set(OTBTinyXML_SYSTEM_INCLUDE_DIRS
  ${TINYXML_INCLUDE_DIRS}
  )
set(OTBTinyXML_INCLUDE_DIRS
  ${OTBTinyXML_BINARY_DIR}/src
  )
set(OTBTinyXML_LIBRARIES
  "${TINYXML_LIBRARIES}"
  )

otb_module_impl()

install(FILES ${OTBTinyXML_BINARY_DIR}/src/otb_tinyxml.h
  DESTINATION ${OTBTinyXML_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
