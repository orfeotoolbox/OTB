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

# - Find Numpy
# Find the native numpy includes
#
#   NUMPY_FOUND        - True if Numpy headers are found.
#   NUMPY_INCLUDE_DIR   - where to find numpy/arrayobject.h, etc.

execute_process(
  COMMAND "${PYTHON_EXECUTABLE}" -c "import sys, numpy; sys.stdout.write(numpy.get_include())"
  OUTPUT_VARIABLE NUMPY_INCLUDE_DIR
  RESULT_VARIABLE NUMPY_NOT_FOUND)

if( NUMPY_INCLUDE_DIR MATCHES "Traceback" )
# Did not successfully include numpy
set( NUMPY_INCLUDE_DIR )
else()
  set( NUMPY_INCLUDE_DIR ${NUMPY_INCLUDE_DIR} CACHE STRING "Numpy include path." )
endif()

include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Numpy DEFAULT_MSG NUMPY_INCLUDE_DIR )

mark_as_advanced( NUMPY_INCLUDE_DIR )

if(NUMPY_FOUND)
  set(NUMPY_INCLUDE_DIRS ${NUMPY_INCLUDE_DIR})
else()
  set(NUMPY_INCLUDE_DIRS)
endif()
