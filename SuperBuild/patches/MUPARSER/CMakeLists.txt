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

cmake_minimum_required(VERSION 2.8.3)

project(muparser)

option(BUILD_SHARED_LIBS "Building shared libs. Default is ON" ON)
if(MSVC AND BUILD_SHARED_LIBS)
 set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

file(GLOB MUPARSER_SRCS "src/*.cpp" )
list(REMOVE_ITEM MUPARSER_SRCS "${CMAKE_SOURCE_DIR}/src/muParserComplex.cpp")

include_directories("${CMAKE_SOURCE_DIR}/include")

add_library(muparser ${MUPARSER_SRCS})
#if(OTB_LIBRARY_PROPERTIES)
#  set_target_properties(muparser PROPERTIES ${OTB_LIBRARY_PROPERTIES})
#endif()

install(TARGETS muparser
  RUNTIME DESTINATION bin COMPONENT RuntimeLibraries
  LIBRARY DESTINATION lib COMPONENT RuntimeLibraries
  ARCHIVE DESTINATION lib COMPONENT Development)



file(GLOB MUPARSER_HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")
list(REMOVE_ITEM MUPARSER_HEADERS "${CMAKE_SOURCE_DIR}/include/muParserComplex.h")
install(FILES ${MUPARSER_HEADERS}
  DESTINATION include
  COMPONENT Development)

