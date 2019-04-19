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
function(detect_using_file_command input_file result_type result_dir)

  execute_process(COMMAND "${FILE_COMMAND}" "${input_file}"
    RESULT_VARIABLE file_rv
    OUTPUT_VARIABLE file_ov
    ERROR_VARIABLE file_ev
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  if(NOT file_rv STREQUAL "0")
    message(FATAL_ERROR "${FILE_COMMAND} failed: ${file_rv}\n${file_ev}\n${file_ov}")
  endif()

  get_filename_component(input_file_NAME ${input_file} NAME)
  string(REPLACE "${input_file}" " _file_full_ " file_ov "${file_ov}")
  string(TOLOWER "${file_ov}" file_ov_lower)

  message( "input_file: '${input_file}'" )
  message( "input_file_NAME: '${input_file_NAME}'" )
  message( "file_ov: '${file_ov}'" )
  message( "file_ov_lower: '${file_ov_lower}'" )

  set(detected_type)
  set(detected_dir)
  # below executable check works for both mac osx and linux
  if("${file_ov_lower}" MATCHES "executable")
    set(detected_type PROGRAMS)
    set(detected_dir bin)
    # detect position independent executables on Linux,
    # where "file" gives "dynamically linked (uses shared libraries)"
  elseif("${file_ov_lower}" MATCHES "dynamically linked.*\(uses shared libs\)")
    set(detected_type FILES)
    set(detected_dir lib)
    # detect position independent executables on Linux,
    # where "file" gives "shared object ... (uses shared libraries)"
  elseif("${file_ov_lower}" MATCHES "shared object.*\(uses shared libs\)")
    set(detected_type FILES)
    set(detected_dir lib)
    # detect shared libraries on Linux,
    # where "file" gives "ELF 64-bit LSB shared object, AMD x86-64, version 1 (SYSV), not stripped"
  elseif("${file_ov_lower}" MATCHES "elf.*shared object.*version")
    set(detected_type FILES)
    set(detected_dir lib)
    # "file" version 5.22 does not print "(used shared libraries)"
    # but uses "interpreter"
  elseif("${file_ov_lower}" MATCHES "shared object.*interpreter")
    set(detected_type FILES)
    set(detected_dir lib)
    # detect shared libraries on Mac OSX
    # where "file" gives "Mach-O 64-bit x86_64 dynamically linked shared library"
  elseif("${file_ov_lower}" MATCHES "mach-o.*dynamically linked shared library")
    set(detected_type FILES)
    set(detected_dir lib)
    #below check is redundant.  detect executables on Mac OSX
    # where "file" gives "Mach-O 64-bit x86_64 executable"
  elseif("${file_ov_lower}" MATCHES "mach-o.*executable")
    set(detected_type PROGRAMS)
    set(detected_dir bin)
    # detect shared libraries on Mac OSX
    # where "file" gives "Mach-O 64-bit x86_64 bundle"
  elseif("${file_ov_lower}" MATCHES "mach-o.*bundle")
    set(detected_type FILES)
    set(detected_dir lib)
  elseif("${file_ov_lower}" MATCHES "mach-o.*bundle")
    set(detected_type FILES)
    set(detected_dir lib)
  elseif("${file_ov_lower}" MATCHES "symbolic link")
    #this will do a symlink check again using but that's okay!

    #set type to symlink we are going to resolve it anyway
    set(detected_type "symlink")

    set(is_this_a_symbolic_link FALSE)
    set(target_file)
    isfile_symlink("${input_file}" is_this_a_symbolic_link target_file)
    if(is_this_a_symbolic_link)

      if(target_file)
	set(libdir "lib")
	setif_value_in_list(is_gtk_lib "${input_file_NAME}" GTK_LIB_LIST_1)
	if(is_gtk_lib)
	  set(libdir "lib/gtk")
	endif()
	file(APPEND
	  ${CMAKE_BINARY_DIR}/make_symlinks
	  "ln -sf \"$OUT_DIR/${libdir}/${target_file}\" \"$OUT_DIR/${libdir}/${input_file_NAME}\" \n"
	  )
      endif() #target_file
    endif() #is_this_a_symbolic_link
  else() #if("${file_ov_lower}" MATCHES ...)
    #we are out of options at this point. throw error!
    message(FATAL_ERROR "unknown/untracked file type found: ${input_file}")
  endif() #if("${file_ov_lower}" MATCHES ...)

  #message("detected_type=${detected_type}")
  set(${result_type} "${detected_type}" PARENT_SCOPE)
  set(${result_dir} "${detected_dir}" PARENT_SCOPE)
endfunction()
