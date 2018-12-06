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

#function to get symlink target full path
function(get_symlink_target target symlink)
  execute_process(COMMAND "${READLINK}" -f "${symlink}"
    RESULT_VARIABLE readlink_rv
    OUTPUT_VARIABLE readlink_ov
    ERROR_VARIABLE readlink_ev
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  set(${target} "${readlink_ov}" PARENT_SCOPE)
endfunction(get_symlink_target)

#OpenMP workaround for unix and mac system
#We create a temporary directory where we are going to copy all omp library 
#found in ${OMP_LIB_DIR}
file(MAKE_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/tmp-omp-lib)
set(OMP_TEMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/tmp-omp-lib)

set(otbcommon_glob_name 
  "${SUPERBUILD_INSTALL_DIR}/lib/${LIB_PREFIX}OTBCommon-*${LIB_EXT}")

file(GLOB otbcommon_paths ${otbcommon_glob_name})

#file(GLOB..) might find several libs matching 
#We are taking the first one.
list(GET otbcommon_paths 0 otbcommon_path)
if ( NOT EXISTS "${otbcommon_path}" )
  message(FATAL_ERROR "Error, cannot find ${LIB_PREFIX}OTBCommon-* in :
    ${SUPERBUILD_INSTALL_DIR}/lib/, result is : ${otbcommon_path}")
endif()

set(omp_full_path "")

# Doing a LDD on the lib to get omp lib path
include(GetPrerequisites)
GET_PREREQUISITES(${otbcommon_path} dependencies 0 0 "" "")
foreach(dep ${dependencies})
  if( dep MATCHES "lib[g]*omp.*${LIB_EXT}.*")
    set(omp_full_path ${dep})
    break()
  endif()
endforeach(dep)

if( omp_full_path )
  if (NOT EXISTS ${omp_full_path})
    message(FATAL_ERROR "A problem occurs, ${omp_full_path} looks like an " 
      "openMP library but does not exists.")
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
  
  list(APPEND PKG_SEARCHDIRS "${OMP_TEMP_DIR}")
endif()
#Only append the directory if we build otb with openMP
