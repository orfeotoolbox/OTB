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

#This cmake file is here to allow packaging of openMP.
#Windows case is not taking care of as we already provide search dir for
#the omp dll

#function to get symlink target works
function(get_symlink_target target symlink)
  execute_process(COMMAND "${READLINK}" -f "${symlink}"
    RESULT_VARIABLE readlink_rv
    OUTPUT_VARIABLE readlink_ov
    ERROR_VARIABLE readlink_ev
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  set(${target} "${readlink_ov}" PARENT_SCOPE)
endfunction(get_symlink_target)

#This function strip the result of ${LOADER_PROGRAM} to retrieve proper lib name
function(strip_candidate refine_candidate raw_candidate)
    set( ${refine_candidate} "" PARENT_SCOPE )
    if(NOT raw_candidate)
      return()
    endif()

    if(NOT "${raw_candidate}" MATCHES "${LOADER_REGEX}")
      return()
    endif()

    string(REGEX REPLACE "${LOADER_REGEX}" "\\1" result "${raw_candidate}")

    if(NOT result)
      return()
    endif()  

    string(STRIP ${result} result)
    
    set(is_system FALSE)

    #if the lib is sysem we do not process it 
    setif_value_in_list(is_system "${result}" SYSTEM_DLLS)

    #MacOS workaround
    if(APPLE AND NOT is_system)
      if("${result}" MATCHES "@rpath")
        string(REGEX REPLACE "@rpath." "" result "${result}")
      else()
        message(FATAL_ERROR "'${result}' does not have @rpath")
      endif()
    endif()

    set(${refine_candidate} "${result}" PARENT_SCOPE)
endfunction(strip_candidate)

#OMP list name
set(OMP_NAME_LIST
  libomp.so.[0-9]* #linux clang
  libgomp.so.[0-9]* #linux gcc
  libomp.[0-9]*dylib #mac
  )
# vcomp[0-9]*.dll #windows
# vcompd[0-9]*.dll #windows debug

#OpenMP workaround for unix and mac system
#We create a temporary directory where we are going to copy all omp library 
#found in ${OMP_LIB_DIR}
file(MAKE_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/tmp-omp-lib)
set(OMP_TEMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/tmp-omp-lib)

set(otbcommon_glob_name "${SUPERBUILD_INSTALL_DIR}/lib/${LIB_PREFIX}OTBCommon-*${LIB_EXT}")

file(GLOB otbcommon_paths ${otbcommon_glob_name})

#file(GLOB..) might find several lib matching 
#We are taking the first one.
list(GET otbcommon_paths 0 otbcommon_path)
if ( NOT EXISTS "${otbcommon_path}" )
  message(FATAL_ERROR "Error, cannot find ${LIB_PREFIX}OTBCommon-* in :
    ${SUPERBUILD_INSTALL_DIR}/lib/, result is : ${otbcommon_path}")
endif()
#We are getting all the dependancies of the lib. If openMP has been used 
#for compiling OTB we will find it here.
execute_process(
    COMMAND ${LOADER_PROGRAM} ${LOADER_ARGS} "${otbcommon_path}"
    RESULT_VARIABLE omp_rv
    OUTPUT_VARIABLE omp_ov
    ERROR_VARIABLE omp_ev
    )

if(omp_rv)
    message(FATAL_ERROR "loader_ev=${loader_ev}\n PACKAGE-OTB: result_variable is '${loader_rv}'")
endif()

string(REPLACE ";" "\\;" omp_candidates "${omp_ov}")
string(REPLACE "\n" "${LOADER_REGEX_EOL};" omp_candidates "${omp_candidates}")

foreach(omp_candidate ${omp_candidates})
  #stripping the raw result
  strip_candidate( omp_lib ${omp_candidate})

  if ( NOT omp_lib  )
    continue()
  endif()

  #Searching in the list if it matches an omp lib name
  setif_value_in_list( is_omp ${omp_lib} OMP_NAME_LIST)

  if ( NOT is_omp )
    continue()
  endif()

  #we should find the lib in th OMP_LIB_DIR
  set(omp_full_path ${OMP_LIB_DIR}/${omp_lib})
  if( NOT EXISTS ${omp_full_path})
    message(FATAL_ERROR "Warning openMP lib : ${omp_lib} not found in :
     ${OMP_LIB_DIR}")
  endif()

  #We need to loop over symlink
  set(is_a_symlink TRUE)
  while ( is_a_symlink )
    set(is_a_symlink FALSE)
    if(IS_SYMLINK ${omp_full_path})
      get_symlink_target(target ${omp_full_path})
      set(is_a_symlink TRUE)
      file(COPY ${omp_full_path}
        DESTINATION ${OMP_TEMP_DIR})
      set( omp_full_path ${target} )
    endif()
  endwhile( is_a_symlink )

  file(COPY ${omp_full_path} DESTINATION ${OMP_TEMP_DIR})

endforeach(omp_candidate)

#Append the temp dir to the list of PKG_SEARCHDIRS
list(APPEND PKG_SEARCHDIRS "${OMP_TEMP_DIR}")
