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

#check variables are set
foreach(var P_DIRS P_MATCH P_REPLACE)
  if(NOT ${var})
    message(FATAL_ERROR "${var} not set")
  endif()
endforeach()

string(REPLACE "|" ";" P_DIRS ${P_DIRS})


foreach( p_dir ${P_DIRS} )
  set(file_list)
  file( GLOB file_list "${p_dir}/*.cmake" )
  list(SORT file_list)
  foreach( cmake_file ${file_list} )
    file(STRINGS "${cmake_file}" MATCH_FOUND REGEX "${P_MATCH}")
    if(MATCH_FOUND)
      message(STATUS "Patching: ${cmake_file}")
      #message("Patch: Replace '${P_MATCH}' with '${P_REPLACE}' in ${cmake_file}")
      file(STRINGS "${cmake_file}" cmake_file_CONTENTS NEWLINE_CONSUME)
      string(REPLACE "${P_MATCH}" "$${}{${P_REPLACE}}" cmake_file_CONTENTS ${cmake_file_CONTENTS})
      file(WRITE "${cmake_file}"  "# This file is modified by OTB after installation.
      \n${cmake_file_CONTENTS}")
    endif()
  endforeach() # foreach( cmake_file
endforeach() # foreach( p_dir
