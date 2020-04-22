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

# This file sets up include directories, link directories, and
# compiler settings for a project to use OTB.  It should not be
# included directly, but rather through the OTB_USE_FILE setting
# obtained from OTBConfig.cmake.

# Add compiler flags needed to use OTB.
set(CMAKE_C_FLAGS "${OTB_REQUIRED_C_FLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${OTB_REQUIRED_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${OTB_REQUIRED_LINK_FLAGS} ${CMAKE_EXE_LINKER_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${OTB_REQUIRED_LINK_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${OTB_REQUIRED_LINK_FLAGS} ${CMAKE_MODULE_LINKER_FLAGS}")

# Add include directories needed to use OTB.
include_directories(${OTB_INCLUDE_DIRS})

# Add link directories needed to use OTB.
link_directories(${OTB_LIBRARY_DIRS})

# find_package() are already done (in OTBConfig.cmake)
# perform include(UseFile) for all external dependencies

# ITK : don't call UseFile to avoid the include_directorties(BEFORE ...)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ITK_REQUIRED_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ITK_REQUIRED_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ITK_REQUIRED_LINK_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${ITK_REQUIRED_LINK_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${ITK_REQUIRED_LINK_FLAGS}")

# OpenCV : no USE_FILE defined

#PYTHONPATH

if( OTB_PYTHONPATH )
  if(DEFINED ENV{PYTHONPATH})
    set(ENV{PYTHONPATH} "${OTB_PYTHONPATH};$ENV{PYTHONPATH}")
  endif()
endif()
