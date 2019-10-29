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

# - Extract information from a mercurial working copy

# Example usage:
#  find_package(Mercurial)
#  if(Mercurial_FOUND)
#    Mercurial_WC_IDENTIFY(${PROJECT_SOURCE_DIR} Project)
#    message("Current revision is ${Project_WC_REVISION}")
#    message("Mercurial version is ${Mercurial_VERSION_HG}")
#  endif()

# This file can be deleted  as we no longer use mercurial
set(Mercurial_FOUND FALSE)
set(Mercurial_HG_FOUND FALSE)

find_program(Mercurial_HG_EXECUTABLE hg
  DOC "mercurial command line client")
mark_as_advanced(Mercurial_HG_EXECUTABLE)

if(Mercurial_HG_EXECUTABLE)
  set(Mercurial_HG_FOUND TRUE)
  set(Mercurial_FOUND TRUE)

  macro(Mercurial_WC_IDENTIFY dir prefix)
    # the subversion commands should be executed with the C locale, otherwise
    # the message (which are parsed) may be translated, Alex
    set(_Mercurial_SAVED_LC_ALL "$ENV{LC_ALL}")
    set(ENV{LC_ALL} C)
    execute_process(COMMAND ${Mercurial_HG_EXECUTABLE} version
      WORKING_DIRECTORY ${dir}
      OUTPUT_VARIABLE Mercurial_VERSION_HG
      OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

    execute_process(COMMAND ${Mercurial_HG_EXECUTABLE} identify ${dir}
      OUTPUT_VARIABLE ${prefix}_WC_IDENTIFY
#      ERROR_VARIABLE Mercurial_hg_identify_error
#      RESULT_VARIABLE Mercurial_hg_identify_result
      OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

    execute_process(COMMAND ${Mercurial_HG_EXECUTABLE} status -m ${dir}
      OUTPUT_VARIABLE ${prefix}_WC_STATUS
#      ERROR_VARIABLE Mercurial_hg_status_error
#      RESULT_VARIABLE Mercurial_hg_status_result
      OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

    if(NOT ${Mercurial_hg_identify_result} EQUAL 0)
      message(SEND_ERROR "Command \"${Mercurial_HG_EXECUTABLE} identify ${dir}\" failed with output:\n${Mercurial_hg_identify_error}")
    else()

      string(REGEX REPLACE "^(.*\n)?Mercurial Distributed SCM [(]version ([.0-9]+).*"
        "\\2" Mercurial_VERSION_HG "${Mercurial_VERSION_HG}")
      string(REGEX REPLACE "^(.*\n)?([0-9a-f]+).*"
        "\\2" ${prefix}_WC_REVISION "${${prefix}_WC_IDENTIFY}")

    endif()

    # restore the previous LC_ALL
    set(ENV{LC_ALL} ${_Mercurial_SAVED_LC_ALL})

  endmacro()



endif()



if(NOT Mercurial_FOUND)
  if(NOT Mercurial_FIND_QUIETLY)
    message(STATUS "Mercurial was not found.")
  else()
    if(Mercurial_FIND_REQUIRED)
      message(FATAL_ERROR "Mercurial was not found.")
    endif()
  endif()
endif()
