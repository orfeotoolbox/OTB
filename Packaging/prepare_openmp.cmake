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

#OMP list name
set(OMP_NAME_LIST
  libomp.so.[0-9]* #linux clang
  libgomp.so.[0-9]* #linux gcc
  vcomp[0-9]*.dll #windows
  vcompd[0-9]*.dll #windows debug
  libomp.[0-9]*dylib #mac
  )

#OpenMP workaround
file(MAKE_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/omp-lib)
set(OMP_TEMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/omp-lib/)

#use otbAppCommandLineLauncher to check if OpenMP is needed
set(otbcommon_path "${SUPERBUILD_INSTALL_DIR}/lib/libOTBCommon-${PKG_OTB_VERSION_MAJOR}.${PKG_OTB_VERSION_MINOR}.${PKG_OTB_VERSION_PATCH}${LIB_EXT}")

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

foreach(candidate ${omp_candidates})
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
      if("${raw_item}" MATCHES "@rpath")
        string(REGEX REPLACE "@rpath." "" raw_item "${raw_item}")
      else()
        message(FATAL_ERROR "'${raw_item}' does not have @rpath")
      endif()
    endif()
    set(is_omp FALSE)
    setif_value_in_list(is_omp "${raw_item}" OMP_NAME_LIST)

    if ( ${is_omp} )
      set(omp_full_path ${LIB_OPENMP_PATH}/${raw_item})
      set(is_a_symlink TRUE)
      set(finale_target "${LIB_OPENMP_PATH}/${raw_item}")
      if(EXISTS "${finale_target}")
        file(COPY ${finale_target}
          DESTINATION ${OMP_TEMP_DIR})
      elseif()
        message(WARNING "Warning openMP not found in : ${LIB_OPENMP_PATH}")
      endif()
      #the file might be a symlink. We need to find the real target
      while( is_a_symlink )
        isfile_symlink("${finale_target}" is_a_symlink omp_target)
        if ( is_a_symlink )
        # everything must be past otherwise we wont be able to follow
        #the dependance in the process_file_recurse function
          set(finale_target "${LIB_OPENMP_PATH}/${omp_target}")
          file(COPY ${finale_target}
            DESTINATION ${OMP_TEMP_DIR})
        endif()
      endwhile(is_a_symlink)    
    endif()

endforeach(candidate)

#Append the temp dir to the list of PKG_SEARCHDIRS
list(APPEND PKG_SEARCHDIRS "${OMP_TEMP_DIR}")
