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

# Find the Google Benchmark library
# Defines:
#  GBENCHMARK_INCLUDE_DIR  - gbenchmark include directory
#  GBENCHMARK_LIBRARY      - gbenchmark library file
#  GBENCHMARK_FOUND        - TRUE if gbenchmark is found

set(GBENCHMARK_FIND_QUIETLY TRUE)
# if (GBENCHMARK_INCLUDE_DIR)
#  #check cache
#   set(GBENCHMARK_FIND_QUIETLY TRUE)
# endif ()

if (NOT GBENCHMARK_INCLUDE_DIR)
  find_path(GBENCHMARK_INCLUDE_DIR NAMES benchmark.h PATH_SUFFIXES benchmark)
  set(GBENCHMARK_INCLUDE_DIR ${GBENCHMARK_INCLUDE_DIR}/benchmark CACHE PATH "Google.benchmark include directory")
endif ()

mark_as_advanced(GBENCHMARK_INCLUDE_DIR)

find_library(GBENCHMARK_LIBRARY NAMES benchmark)
mark_as_advanced(GBENCHMARK_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GBENCHMARK
  REQUIRED_VARS GBENCHMARK_LIBRARY GBENCHMARK_INCLUDE_DIR
  )
#VERSION_VAR GBENCHMARK_VERSION_STRING

set(GBENCHMARK_LIBRARIES ${GBENCHMARK_LIBRARY})
set(GBENCHMARK_INCLUDE_DIRS ${GBENCHMARK_INCLUDE_DIR})
