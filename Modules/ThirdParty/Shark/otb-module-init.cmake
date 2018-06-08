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

find_package ( Shark REQUIRED )

if ( SHARK_USE_OPENMP AND NOT OTB_USE_OPENMP )
  message(WARNING "Shark library is built with OpenMP and you have OTB_USE_OPENMP set to OFF.\
   It will be turned to ON to allow compilation.")
  set( OTB_USE_OPENMP "ON" CACHE BOOL "Add openmp compiler and linker flags" FORCE )
endif()

mark_as_advanced( Shark_DIR )
