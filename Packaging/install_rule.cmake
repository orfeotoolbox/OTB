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
#
# This is one big fat if-elseif ladder
# that will detect a type and destination to use in
# install(..

function(install_rule src_file)

  get_filename_component(src_file_EXT ${src_file} EXT)
  get_filename_component(src_file_NAME ${src_file} NAME)
  get_filename_component(src_file_NAME_WE ${src_file} NAME_WE)
  get_filename_component(src_file_PATH ${src_file} PATH)

  get_filename_component(src_file_GPATH ${src_file_PATH} PATH)
  get_filename_component(src_file_GNAME ${src_file_GPATH} NAME)

  file(GLOB src_file_star "${src_file_PATH}/${src_file_NAME_WE}*")
  foreach(sfile ${src_file_star})
    get_filename_component(sfile_NAME ${sfile} NAME)
    string(TOLOWER "${sfile_NAME}" sfile_NAME_LOWER)

    get_filename_component(sfile_ABS "${sfile}" ABSOLUTE)
    string(TOLOWER "${sfile_ABS}" sfile_ABS_LOWER)
    set(install_type)
    set(install_dir)
    if ("${sfile_NAME_LOWER}" MATCHES "^(otb|mvd)([a-z0-9]+)test*.*" )
      if(PKG_DEBUG)
        message("SKIP INSTALL for ${sfile_NAME_LOWER}")
      endif()
      continue()
    elseif ("${sfile_NAME_LOWER}" MATCHES "headertest" )
      # special case for remote modules which has executables
      # like MosiacHeaderTest1.exe. This ideally should be
      # fixed in remote modules like OTBMosiacHeaderTest1.exe
      if(PKG_DEBUG)
        message("SKIP INSTALL for ${sfile_NAME_LOWER}")
      endif()
      continue()
    elseif(("${sfile_ABS_LOWER}" MATCHES "(\\${LIB_EXT})$") AND (src_file_GNAME STREQUAL "plugins"))
      # special case: Qt plugins are installed by install_qtdev_files
      continue()
    elseif("${sfile_ABS_LOWER}" MATCHES "(\\.exe)$")
      set(install_type PROGRAMS)
      set(install_dir bin)
    elseif("${sfile_ABS_LOWER}" MATCHES "(\\.dll)$")
      set(install_type FILES)
      set(install_dir bin) 
    elseif("${sfile_ABS_LOWER}" MATCHES "(\\.lib)$")
      set(install_type FILES)
      set(install_dir lib)
    elseif("${sfile_ABS_LOWER}" MATCHES "(\\.pyd)$")
      #python bindings are installed in install_python_bindings
      continue()
    elseif( "${sfile_NAME_LOWER}" MATCHES "^_otbapplication")
      # _otbApplication.so matched below; so we explicit skip it
      # because it's already handled in install_python_bindings.cmake
      continue()
    elseif("${sfile_ABS_LOWER}" MATCHES "\\.lnk$")
      #we don't install symlink on windows. issue a warning
      set(install_type "symlink")
      set(install_dir)
    elseif("${sfile_ABS_LOWER}" MATCHES "(\\.a)$")
      set(install_type FILES)
      set(install_dir lib)
    elseif("${sfile_ABS_LOWER}" MATCHES "\\.prl$")
      continue()
    elseif("${sfile_ABS_LOWER}" MATCHES "\\.la$")
      continue()
    elseif("${sfile_ABS_LOWER}" MATCHES "\\.settings$")
      continue()
    elseif("${sfile_ABS_LOWER}" MATCHES "-config$")
      continue()
    else()
      if(UNIX)
        #the last else() loop where we run a 'file' command to find file type and directory
        set(install_type)
        set(install_dir)
        #this is whole other story
        detect_using_file_command(${sfile_ABS} install_type install_dir)
      endif(UNIX)
    endif()

    if(install_type STREQUAL "symlink")
      #we don't install symlink on windows. issue a warning and continue
      if(PKG_DEBUG)
        message("${sfile_ABS} is a symbolic link and this will be excluded from package")
      endif()
      continue()
    endif()
    
    # This check comes outside the first if-elseif ladder
    # because no matter what extension of file type, otbapp_* must go
    # into lib/otb/applications
    if("${sfile_NAME_LOWER}" MATCHES "^otbapp_")
      install_otbapp_wrapper_scripts(${sfile_NAME})
      set(install_dir "lib/otb/applications")
      set(install_type PROGRAMS)
    endif()
  
    if(NOT install_type OR NOT install_dir)
      #throw fatal error and return
      message(FATAL_ERROR "sfile_ABS=${sfile_ABS}.\ninstall_type=${install_type}\ninstall_dir=${install_dir}")
      return()
    endif()

    install(${install_type} "${sfile}"
      DESTINATION
      "${PKG_STAGE_DIR}/${install_dir}"
      )

  endforeach()  
endfunction()   

