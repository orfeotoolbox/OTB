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
function(configure_loader loader_pgm_result loader_args_result loader_regex_result)
set(loader_PATHS)
if(MSVC)
    set(loader_PATHS)
    set(loader_names      "dumpbin")
    set(loader_args       "/DEPENDENTS")
    set(loader_regex      "^    ([^ ].*[Dd][Ll][Ll])${LOADER_REGEX_EOL}$")
  elseif(APPLE)
    set(loader_PATHS)
    set(loader_names    otool)
    set(loader_args      "-l" )
    set(loader_regex ".*name.*([ ].*dylib ).*${LOADER_REGEX_EOL}$")
  elseif("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    set(loader_PATHS)
    set(loader_names    objdump)
    set(loader_args    "-p" )
    set(loader_regex "^..NEEDED.*([ ].*so.*)${LOADER_REGEX_EOL}$")
  else()
    message(FATAL_ERROR "find_loader_and_args: Unknown platform")
  endif()
  
  find_program(loader_pgm "${loader_names}" PATHS ${loader_PATHS})
  if(NOT loader_pgm)
    message(FATAL_ERROR "${loader_names} not found in ${loader_PATHS}.")
  endif()

  #save result in PARENT_SCOPE
  set(${loader_args_result} ${loader_args} PARENT_SCOPE)
  set(${loader_pgm_result} ${loader_pgm} PARENT_SCOPE)
  set(${loader_regex_result} ${loader_regex} PARENT_SCOPE)  
endfunction()
