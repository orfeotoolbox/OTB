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

if(WIN32)
    cmake_minimum_required(VERSION 2.4.6 FATAL_ERROR)
else()
    if(APPLE)
        cmake_minimum_required(VERSION 2.4.6 FATAL_ERROR)
    else()
        cmake_minimum_required(VERSION 2.4.0 FATAL_ERROR)
    endif()
endif()


project(OpenThreads)

# We have some custom .cmake scripts not in the official distribution.
# Maybe this can be used override existing behavior if needed?
set(CMAKE_MODULE_PATH "${OpenThreads_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

################################################################################
# Installation stuff

# We want to build SONAMES shared librariess
set(OPENTHREADS_SONAMES TRUE)

#set(CMAKE_DEBUG_POSTFIX  "d")
#set(INSTALL_BINDIR OpenThreads/bin)
#set(INSTALL_INCDIR OpenThreads/include)
#set(INSTALL_LIBDIR OpenThreads/lib)
#set(INSTALL_DOCDIR OpenThreads/docs)


#set(CMAKE_DEBUG_POSTFIX  "d")

set(LIB_POSTFIX "")
#if(UNIX AND NOT WIN32 AND NOT APPLE)
#  if(CMAKE_SIZEOF_VOID_P MATCHES "8")
#      set(LIB_POSTFIX "64")
#  endif()
#endif()

################################################################################


# Add a source group for the include headers so they are seen in IDEs.
# Hmmm, this isn't working for me in Xcode.
# source_group(
#     "Header Files"
#     FILES ${OpenThreads_PUBLIC_HEADERS}
# )

# Make the headers visible to everything
include_directories(
    ${OpenThreads_SOURCE_DIR}/include
)


subdirs(src/OpenThreads)

# FIXME: Make optional install test programs
# FIXME: /bin directory not correct for Windows
# install_targets(/bin OpenThreads)

#set(OUTPUT_BINDIR ${PROJECT_BINARY_DIR}/bin/${CMAKE_SYSTEM_NAME})
# OTB modifications
# Alls bin or lib in OTB bin directory
#set(OUTPUT_BINDIR ${PROJECT_BINARY_DIR}/bin)
set(OUTPUT_BINDIR ${CMAKE_BINARY_DIR}/bin)
make_directory(${OUTPUT_BINDIR})
set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_BINDIR})

#set(OUTPUT_LIBDIR ${PROJECT_BINARY_DIR}/lib/${CMAKE_SYSTEM_NAME})
#set(OUTPUT_LIBDIR ${PROJECT_BINARY_DIR}/lib)
set(OUTPUT_LIBDIR ${CMAKE_BINARY_DIR}/lib)
make_directory(${OUTPUT_LIBDIR})
set(LIBRARY_OUTPUT_PATH ${OUTPUT_LIBDIR})

################################################################################
# Create bin and lib directories if required

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin ${CMAKE_BINARY_DIR}/lib)
endif()




# This needs to be run very last so other parts of the scripts can take
# advantage of this.
if(NOT OPENTHREADS_CONFIG_HAS_BEEN_RUN_BEFORE)
    set(OPENTHREADS_CONFIG_HAS_BEEN_RUN_BEFORE 1 CACHE INTERNAL "Flag to track whether this is the first time running CMake or if CMake has been configured before")
endif()


