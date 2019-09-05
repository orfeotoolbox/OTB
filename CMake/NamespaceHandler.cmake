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

# replace target name on one line
macro(ns_filter_line)
foreach(_t_name ${_t_list})
  string(REPLACE "${_t_prefix}${_t_name}${_t_suffix}" "${_t_prefix}${REPLACEMENT_${_t_name}}${_t_suffix}" _filtered_line "${_filtered_line}")
endforeach()
endmacro()

# This function sets the namespace for all targets in the CMake files installed
# by OTB
function(otb_set_namespace folder ns)
  set(_t_regex "add_(library|executable) *\\( *([a-zA-Z0-9_:-]+) (.* )?IMPORTED( |\\)).*")
  if(EXISTS "${folder}/OTBTargets.cmake")
    # list targets
    file(STRINGS "${folder}/OTBTargets.cmake" _t_content REGEX "${_t_regex}")
    set(_t_list)
    foreach(_line ${_t_content})
      string(REGEX REPLACE "${_t_regex}" "\\2" _t_name "${_line}")
      list(APPEND _t_list "${_t_name}")
      set(_t_real_name "${_t_name}")
      set(_old_ns_regex "([a-zA-Z0-9_-]+)::([a-zA-Z0-9_-]+)")
      if(_t_name MATCHES "${_old_ns_regex}")
        string(REGEX REPLACE "${_old_ns_regex}" "\\2" _t_real_name "${_t_name}")
      endif()
      if(ns)
        set(REPLACEMENT_${_t_name} "${ns}::${_t_real_name}")
      else()
        set(REPLACEMENT_${_t_name} "${_t_real_name}")
      endif()
    endforeach()
    # list files to patch
    set(_files_to_process "${folder}/OTBTargets.cmake")
    file(GLOB _other_targets "${folder}/OTBTargets-*.cmake")
    list(APPEND _files_to_process ${_other_targets})
    file(GLOB _module_files "${folder}/Modules/*.cmake")
    list(APPEND _files_to_process ${_module_files})
    list(APPEND _files_to_process "${folder}/OTBStandaloneModuleMacros.cmake")

    # patch each file
    foreach(_file_path ${_files_to_process})
      file(STRINGS "${_file_path}" _file_content NEWLINE_CONSUME)
      string(REGEX REPLACE "\n" "\n;" _file_content "${_file_content}")
      set(_file_filtered_content)
      foreach(_line ${_file_content})
        set(_filtered_line "${_line}")
        if(_filtered_line MATCHES "(add_library|add_executable|set_target_properties)")
          set(_t_prefix "(")
          set(_t_suffix " ")
          ns_filter_line()
        elseif(_filtered_line MATCHES "(set_property|APPEND _IMPORT_CHECK_TARGETS)")
          set(_t_prefix " ")
          set(_t_suffix " ")
          ns_filter_line()
        elseif(_filtered_line MATCHES "_LIBRARIES")
          set(_t_prefix " ")
          set(_t_suffix " ")
          string(REPLACE ";" " ; " _filtered_line "${_filtered_line}")
          string(REPLACE "\"" " \" " _filtered_line "${_filtered_line}")
          ns_filter_line()
          string(REPLACE " ; " ";" _filtered_line "${_filtered_line}")
          string(REPLACE " \" " "\"" _filtered_line "${_filtered_line}")
        elseif(_filtered_line MATCHES "APPEND _IMPORT_CHECK_FILES_FOR_")
          set(_t_prefix "_")
          set(_t_suffix " ")
          ns_filter_line()
        elseif(_filtered_line MATCHES "foreach\\(_expectedTarget ")
          set(_t_prefix " ")
          set(_t_suffix " ")
          string(REPLACE ")" " )" _filtered_line "${_filtered_line}")
          ns_filter_line()
          string(REPLACE " )" ")" _filtered_line "${_filtered_line}")
        elseif(_filtered_line MATCHES "<TARGET_FILE:")
          set(_t_prefix "TARGET_FILE:")
          set(_t_suffix ">")
          ns_filter_line()
        elseif(_filtered_line MATCHES "add_dependencies")
          set(_t_prefix " ")
          set(_t_suffix " ")
          string(REPLACE ")" " )" _filtered_line "${_filtered_line}")
          ns_filter_line()
          string(REPLACE " )" ")" _filtered_line "${_filtered_line}")
        endif()
        list(APPEND _file_filtered_content ${_filtered_line})
      endforeach()
      string(REGEX REPLACE "\n;" "\n" _file_filtered_content "${_file_filtered_content}")
      file(WRITE "${_file_path}" "${_file_filtered_content}")
    endforeach()
  else()
    message(WARNING "No OTBTargets.cmake found in folder : ${folder}")
  endif()
endfunction()

if(DEFINED NAMESPACE)
  set(CTEST_SOURCE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")
  set(CTEST_BINARY_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")
  set(CTEST_COMMAND ctest)
  message("Set target namespace to \"${NAMESPACE}\"")
  otb_set_namespace( "${CMAKE_CURRENT_LIST_DIR}" "${NAMESPACE}")
endif()
