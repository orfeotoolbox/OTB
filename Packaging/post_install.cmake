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

function(sanitize_system_paths input_file)
  # does not support Windows ...
  if(APPLE)
    set(SHARED_EXT "\\.dylib")
  elseif(UNIX)
    set(SHARED_EXT "\\.so")
  endif()
  set(filtered_content)

  file(STRINGS "${input_file}" source_file_content NEWLINE_CONSUME)
  string(REGEX REPLACE "\n" "\n;" source_file_content "${source_file_content}")
  set(SEARCH_REGEX "(^[^#\"]+(LIBRARIES|INCLUDE_DIR[A-Z]*) +\")(.+)(\"[^\"]*)")

  foreach(line ${source_file_content})
    set(filtered_line "${line}")
    if(line MATCHES "${SEARCH_REGEX}")
      string(REGEX REPLACE "${SEARCH_REGEX}" "\\3" extract_str "${line}" )
      string(REGEX REPLACE ";;" ";" extract_str "${extract_str}")
      set(_to_be_removed)
      set(_to_be_added)
      foreach(path ${extract_str})
        if(IS_ABSOLUTE ${path})
          list(APPEND _to_be_removed "${path}")
          if(NOT IS_DIRECTORY ${path})
            if(path MATCHES "^.*/lib[^\\.]+${SHARED_EXT}.*")
              string(REGEX REPLACE "^.*/lib([^\\.]+)${SHARED_EXT}.*" "\\1" _lib_name "${path}")
              list(APPEND _to_be_added "${_lib_name}")
            endif()
          endif()
        endif()
      endforeach()
      if(_to_be_removed)
        list(REMOVE_ITEM extract_str ${_to_be_removed})
      endif()
      if(_to_be_added)
        list(INSERT extract_str 0 ${_to_be_added})
      endif()
      if(extract_str)
        list(REMOVE_DUPLICATES extract_str)
      endif()
      string(REGEX REPLACE "${SEARCH_REGEX}" "\\1${extract_str}\\4" filtered_line "${line}" )
    endif()
    list(APPEND filtered_content "${filtered_line}")
  endforeach()

  string(REGEX REPLACE "\n;" "\n" filtered_content "${filtered_content}")
  file(WRITE "${input_file}" "${filtered_content}")
endfunction()

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
    if(UNIX)
      sanitize_system_paths(${cmake_file})
    endif()
  endforeach() # foreach( cmake_file
endforeach() # foreach( p_dir
