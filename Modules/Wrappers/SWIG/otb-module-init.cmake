#
# Copyright (C) 2005-2019 CS Systemes d'Information (CS SI)
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

option ( OTB_WRAP_PYTHON "Wrap Python" OFF )
option ( OTB_WRAP_PYTHON3 "Wrap Python 3" OFF )
option ( OTB_WRAP_JAVA   "Wrap Java"   OFF )

if ( OTB_WRAP_PYTHON OR OTB_WRAP_JAVA OR OTB_WRAP_PYTHON3)
  find_package ( SWIG REQUIRED )
  mark_as_advanced(SWIG_DIR)
  mark_as_advanced(SWIG_EXECUTABLE)
  mark_as_advanced(SWIG_VERSION)
endif()

#
# SimpleITK does this check, but it does not seem necessary in our case
# TODO: figure out why SimpleITK needs this...
#
macro(check_PIC_flag Language)
  if ( UNIX AND NOT APPLE )
    if ( NOT ${CMAKE_CXX_FLAGS} MATCHES "-fPIC")
      # message ( FATAL_ERROR "${Language} wrapping requires CMAKE_CXX_FLAGS (or equivalent) to include -fPIC and ITK built with this flag" )
    endif()
  endif()
endmacro()


if ( OTB_WRAP_PYTHON )
  check_PIC_flag ( Python )
  find_package ( PythonInterp REQUIRED )
  find_package ( PythonLibs REQUIRED )
  find_package ( Numpy )
endif()

macro(swap_cache_variable var1 var2)
  # Only swap variable if var1 exists in the cmake cache
  if (${var1})
    set(_backup ${${var1}})
    get_property(_var1_type CACHE ${var1} PROPERTY TYPE)
    get_property(_var1_help CACHE ${var1} PROPERTY HELPSTRING)
    get_property(_var2_type CACHE ${var2} PROPERTY TYPE)
    get_property(_var2_help CACHE ${var2} PROPERTY HELPSTRING)
    set(${var1} ${${var2}} CACHE ${_var1_type} "${_var1_help}" FORCE)
    set(${var2} ${_backup} CACHE ${_var2_type} "${_var2_help}" FORCE)
  endif()
endmacro()

macro(swap_variable var1 var2)
  set(_backup ${${var1}})
  set(${var1} ${${var2}})
  set(${var2} ${_backup})
endmacro()

if ( OTB_WRAP_PYTHON3 )
  set(PYTHON3_EXECUTABLE "PYTHON3_EXECUTABLE-NOTFOUND" CACHE FILEPATH "Path to python 3 interpreter")
  set(PYTHON3_INCLUDE_DIR "PYTHON3_INCLUDE_DIR-NOTFOUND" CACHE PATH "Path to python 3 include directory")
  set(PYTHON3_LIBRARY "PYTHON3_LIBRARY-NOTFOUND" CACHE FILEPATH "Path to python 3 library")
  set(PYTHON3_LIBRARY_DEBUG "PYTHON3_LIBRARY_DEBUG-NOTFOUND" CACHE FILEPATH "Path to python 3 library (debug)")
  set(PYTHON3_LIBRARY_RELEASE "PYTHON3_LIBRARY_RELEASE-NOTFOUND" CACHE FILEPATH "Path to python 3 library (release)")
  set(NUMPY_PYTHON3_INCLUDE_DIR "NUMPY_PYTHON3_INCLUDE_DIR" CACHE PATH "Path to numpy module for Python 3")

  # Swap cache variables between python and python3
  swap_cache_variable(PYTHON_EXECUTABLE PYTHON3_EXECUTABLE)
  swap_cache_variable(PYTHON_INCLUDE_DIR PYTHON3_INCLUDE_DIR)
  swap_cache_variable(PYTHON_LIBRARY PYTHON3_LIBRARY)
  swap_cache_variable(PYTHON_LIBRARY_DEBUG PYTHON3_LIBRARY_DEBUG)
  swap_cache_variable(PYTHON_LIBRARY_RELEASE PYTHON3_LIBRARY_RELEASE)
  swap_cache_variable(NUMPY_INCLUDE_DIR NUMPY_PYTHON3_INCLUDE_DIR)

  swap_variable(PYTHON_LIBRARIES PYTHON3_LIBRARIES)
  swap_variable(NUMPY_INCLUDE_DIRS NUMPY_PYTHON3_INCLUDE_DIRS)
  swap_variable(NUMPY_FOUND NUMPY_PYTHON3_FOUND)
  swap_variable(PYTHON_VERSION_STRING PYTHON3_VERSION_STRING)

  check_PIC_flag ( Python )
  find_package ( PythonInterp 3 REQUIRED )
  find_package ( PythonLibs 3 REQUIRED )
  find_package ( Numpy )

  # Swap cache variables between python and python3
  swap_cache_variable(PYTHON_EXECUTABLE PYTHON3_EXECUTABLE)
  swap_cache_variable(PYTHON_INCLUDE_DIR PYTHON3_INCLUDE_DIR)
  swap_cache_variable(PYTHON_LIBRARY PYTHON3_LIBRARY)
  swap_cache_variable(PYTHON_LIBRARY_DEBUG PYTHON3_LIBRARY_DEBUG)
  swap_cache_variable(PYTHON_LIBRARY_RELEASE PYTHON3_LIBRARY_RELEASE)
  swap_cache_variable(NUMPY_INCLUDE_DIR NUMPY_PYTHON3_INCLUDE_DIR)

  swap_variable(PYTHON_LIBRARIES PYTHON3_LIBRARIES)
  swap_variable(NUMPY_INCLUDE_DIRS NUMPY_PYTHON3_INCLUDE_DIRS)
  swap_variable(NUMPY_FOUND NUMPY_PYTHON3_FOUND)
  swap_variable(PYTHON_VERSION_STRING PYTHON3_VERSION_STRING)
endif()

#
# JAVA SWIG configuration
#
if ( OTB_WRAP_JAVA )
  check_PIC_flag ( Java )
  find_package ( Java REQUIRED )
  find_package ( JNI REQUIRED )
  mark_as_advanced( JAVA_HOME )
endif()
