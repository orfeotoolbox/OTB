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

cmake_minimum_required(VERSION 2.8.3)

project(libsvm)

option(BUILD_SHARED_LIBS "Building shared libs. Default is ON" ON)

if(MSVC AND BUILD_SHARED_LIBS)
 set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

set(LIBSVM_SRCS "svm.cpp" )
set(LIBSVM_HEADERS "svm.h")

add_library(svm ${LIBSVM_SRCS})

option(BUILD_SAMPLES "Build executables" ON)

add_executable(svm-train svm-train.c)
target_link_libraries(svm-train svm)

add_executable(svm-predict svm-predict.c)
target_link_libraries(svm-predict svm)

add_executable(svm-scale svm-scale.c)
target_link_libraries(svm-scale svm)

install(TARGETS svm
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

install(FILES ${LIBSVM_HEADERS} DESTINATION include)
