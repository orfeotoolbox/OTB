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

# This file contains CMake functions and macros used when testing ITK modules.

#-----------------------------------------------------------------------------
# OTB wrapper for add_test that automatically sets the test's LABELS property
# to the value of its containing module.
#
function(otb_add_test)
  set(largeinput_regex "LARGEINPUT{([^;{}\r\n]*)}")

  set(_depends_on_largeinput OFF)
  foreach(arg IN LISTS ARGN)
     if("x${arg}" MATCHES "${largeinput_regex}")
       string(REGEX REPLACE "${largeinput_regex}" "\\1" largeinput_relative_path "${arg}")
       set(_fullpath "${OTB_DATA_LARGEINPUT_ROOT}/${largeinput_relative_path}")
       list(APPEND _out_arg ${_fullpath})
       set(_depends_on_largeinput ON)
     else()
       list(APPEND _out_arg ${arg})
     endif()
  endforeach()

  if (_depends_on_largeinput AND NOT OTB_DATA_USE_LARGEINPUT)
    return()
  endif()

  add_test(${_out_arg})

  if("NAME" STREQUAL "${ARGV0}")
    set(_iat_testname ${ARGV1})
  else()
    set(_iat_testname ${ARGV0})
  endif()

  if(otb-module)
    set(_label ${otb-module})
  else()
    set(_label ${main_project_name})
  endif()

  set_property(TEST ${_iat_testname} PROPERTY LABELS ${_label})

endfunction()

#-----------------------------------------------------------------------------
# OTB function to ignore a test
#
function(otb_tests_ignore)
  set_property(GLOBAL APPEND PROPERTY CTEST_CUSTOM_TESTS_IGNORE ${ARGN})
endfunction()

#-----------------------------------------------------------------------------
# OTB function to ignore a test during MemCheck
#
function(otb_memcheck_ignore)
  set_property(GLOBAL APPEND PROPERTY CTEST_CUSTOM_MEMCHECK_IGNORE ${ARGN})
endfunction()

#-----------------------------------------------------------------------------
# Function otb_add_test_mpi to run mpi tests
function(otb_add_test_mpi)
   set( _OPTIONS_ARGS )
   set( _ONE_VALUE_ARGS NAME NBPROCS COMMAND)
   set( _MULTI_VALUE_ARGS EXTRA_OPT)
   cmake_parse_arguments( TEST_MPI "${_OPTIONS_ARGS}" "${_ONE_VALUE_ARGS}" "${_MULTI_VALUE_ARGS}" ${ARGN} )

   # Test nb procs
   if( NOT TEST_MPI_NBPROCS )
     set(TEST_MPI_NBPROCS 2)
   endif()
   # Test command line
   foreach(arg IN LISTS TEST_MPI_UNPARSED_ARGUMENTS)
     list(APPEND ARGS ${arg})
   endforeach()
   set (test_parameters -n ${TEST_MPI_NBPROCS} ${OTB_MPIEXEC_OPT} ${TEST_MPI_EXTRA_OPT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_MPI_COMMAND} ${ARGS})
   otb_add_test(NAME ${TEST_MPI_NAME} COMMAND ${MPIEXEC} ${test_parameters})
endfunction()
