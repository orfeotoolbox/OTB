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

set(Boost_USE_STATIC_LIBS OFF CACHE BOOL "use static libraries from boost")

set(Boost_USE_MULTITHREADED ON CACHE BOOL "use multi-threaded libraries from boost")

if(DEFINED USE_SYSTEM_BOOST)
  if(NOT USE_SYSTEM_BOOST)
    #Force boost not to search system paths when using boost from superbuild
    set(Boost_NO_SYSTEM_PATHS ON)
  else()
    set(Boost_NO_SYSTEM_PATHS OFF)
  endif()
else()
  set(Boost_NO_SYSTEM_PATHS OFF)
endif()

find_package (Boost 1.35.0 REQUIRED COMPONENTS filesystem serialization)
if (BUILD_TESTING)
  find_package (Boost 1.35.0 QUIET COMPONENTS filesystem serialization unit_test_framework)
  if (NOT Boost_UNIT_TEST_FRAMEWORK_FOUND)
    message(STATUS "Boost unit_test_framework not found. Hence test depending on this framework will be skipped")
  else()
    message(STATUS "Found Boost components: unit_test_framework")
  endif()
endif() #BUILD_TESTING

if(WIN32)
  # disable autolinking in boost
	add_definitions(-DBOOST_ALL_NO_LIB)
  add_definitions(-DBOOST_ALL_DYN_LINK)
endif()
