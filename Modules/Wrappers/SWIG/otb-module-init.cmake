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
option ( OTB_WRAP_JAVA   "Wrap Java"   OFF )

if ( OTB_WRAP_PYTHON OR OTB_WRAP_JAVA )
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

# thi macro should be removed
if ( OTB_WRAP_PYTHON )
  check_PIC_flag ( Python )
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
