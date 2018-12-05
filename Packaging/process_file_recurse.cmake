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
function(process_file_recurse input_file)
  set(input_file_full_path)
  if(IS_ABSOLUTE "${input_file}" AND EXISTS "${input_file}")
    set(input_file_full_path ${input_file})
    if(PKG_DEBUG)
      message("Found '${input_file}' (return)")
    endif()
  else()
    search_library(${input_file} PKG_SEARCHDIRS input_file_full_path)
  endif()
  if(NOT input_file_full_path)
    if(LINUX)
      setif_value_in_list(is_gtk_lib "${input_file}" ALLOWED_SYSTEM_DLLS)
      if(is_gtk_lib)
        search_library(${input_file} PKG_GTK_SEARCHDIRS input_file_full_path)
        if( NOT input_file_full_path)
          message(FATAL_ERROR "${input_file} not found. searched in ${PKG_GTK_SEARCHDIRS}")
        endif()
      endif()
    endif(LINUX)
  endif() 

  #if(NOT input_file_full_path)
  if( NOT input_file_full_path )
    message(FATAL_ERROR "${input_file} not found. searched in ${PKG_SEARCHDIRS}"
      " and in ${OMP_LIB_DIR} for openMP. ")
  endif()

  get_filename_component(bn_name ${input_file_full_path} NAME)
  if(${bn_name}_RESOLVED)
    # item already resolved
    return()
  endif()

  if(NOT PKG_DEBUG)
    message("Processing ${input_file_full_path}")
  endif()
  
  # Install the file with install_rule().
  # This function has specific "rules" to decide wheather and where to install file  
  install_rule(${input_file_full_path})
  
  set(raw_items)

  execute_process(
    COMMAND ${LOADER_PROGRAM} ${LOADER_ARGS} "${input_file_full_path}"
    RESULT_VARIABLE loader_rv
    OUTPUT_VARIABLE loader_ov
    ERROR_VARIABLE loader_ev
    )
  
  if(loader_rv)
    message(FATAL_ERROR "loader_ev=${loader_ev}\n PACKAGE-OTB: result_variable is '${loader_rv}'")
  endif()

  string(REPLACE ";" "\\;" candidates "${loader_ov}")
  string(REPLACE "\n" "${LOADER_REGEX_EOL};" candidates "${candidates}")
  
  set(${bn_name}_USED TRUE CACHE INTERNAL "")

  if(PKG_DEBUG)
    message("Processing ${input_file} started. Set ${bn_name}_USED=${${bn_name}_USED}")
  endif()

  foreach(candidate ${candidates})
    if(NOT candidate)
      continue()
    endif()

    if(NOT "${candidate}" MATCHES "${LOADER_REGEX}")
      continue()
    endif()

    string(REGEX REPLACE "${LOADER_REGEX}" "\\1" raw_item "${candidate}")

    if(NOT raw_item)
      continue()
    endif()  

    string(STRIP ${raw_item} raw_item)
    set(is_system FALSE)
    setif_value_in_list(is_system "${raw_item}" SYSTEM_DLLS)
    if(APPLE AND NOT is_system)
      if(("${input_file_full_path}" MATCHES "/plugins/") AND ("${raw_item}" MATCHES "${input_file}"))
        # ignore the check for @rpath on Qt plugins for the library own name
      else()
        if("${raw_item}" MATCHES "@rpath")
          string(REGEX REPLACE "@rpath." "" raw_item "${raw_item}")
        else()
          message(FATAL_ERROR "'${raw_item}' does not have @rpath")
        endif()
      endif()
    endif()

    if(PKG_DEBUG AND ${raw_item}_RESOLVED)
      message("${raw_item} is already resolved [${raw_item}_RESOLVED=${${raw_item}_RESOLVED}]")
    endif()

    if(PKG_DEBUG)
      message("raw_item='${raw_item}'\tis_system='${is_system}'")
    endif()
    if(is_system OR ${raw_item}_RESOLVED OR ${raw_item}_USED)
      continue()
    endif()
    
    list(APPEND raw_items ${raw_item})
    
  endforeach()

  if(PKG_DEBUG)
    string(REPLACE ";" "\n" raw_items_pretty_print "${raw_items}")
    # message(FATAL_ERROR "raw_items=${raw_items_pretty_print}")
  endif(PKG_DEBUG)

  if(raw_items)
    list(REVERSE raw_items)
    foreach(item ${raw_items})      
      search_library(${item} PKG_SEARCHDIRS item_full_path)
      set(is_a_symlink FALSE)
      set(item_target_file)
      isfile_symlink("${item_full_path}" is_a_symlink item_target_file)
      if(is_a_symlink)
        set(${item}_RESOLVED TRUE CACHE INTERNAL "")
        set(item ${item_target_file})
      endif()
      if(PKG_DEBUG)
        message("${bn_name} depends on '${item}'. So we now process '${item}'") # [ ${item}_USED=${${item}_USED} ${item}_RESOLVED=${${item}_RESOLVED}]")
      endif()
      process_file_recurse(${item})
    endforeach()
  endif()

  set(${bn_name}_RESOLVED TRUE CACHE INTERNAL "")
   if(PKG_DEBUG)
     message("All dependencies of ${bn_name} are processed. Install file and set ${bn_name}_RESOLVED=${${bn_name}_RESOLVED}")
   endif()

   #install_rule(${input_file_full_path})

endfunction() #function(process_file_recurse infile)
