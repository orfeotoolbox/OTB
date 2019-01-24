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

project(OTBBoost)

set(OTBBoost_SYSTEM_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
set(OTBBoost_INCLUDE_DIRS ${OTBBoost_SOURCE_DIR}/src)

# Do not include boost libraries into full list of OTB_LIBRARIES
# That will drag them in linking all otb modules and executables which is bad.
# OTB uses only header only libraries of boost. A non-header only library
# named 'unit_testing_framework' is used in two tests.
# why drag a semi-standard c++ library for use in two tests?.
# if(Boost_LIBRARIES)
#   set(OTBBoost_LIBRARIES "${Boost_LIBRARIES}")
# endif()

set(Boost_VERSION_STRING "${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}" CACHE INTERNAL "" FORCE)

otb_module_impl()

install(DIRECTORY ${OTBBoost_SOURCE_DIR}/src/boost
  DESTINATION ${OTBBoost_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
