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
function(prepare_search_dirs search_dirs_result)
  
  set(search_dirs)
  
  if(MSVC)
    if(NOT DEFINED ENV{UniversalCRTSdkDir})
      message(FATAL_ERROR "UniversalCRTSdkDir variable not set. Cannot continue")
    endif()
    if(NOT DEFINED ENV{VCINSTALLDIR})
      message(FATAL_ERROR  "VCINSTALLDIR variable not set. Cannot continue.")
    endif()
  endif()
  
  if(MSVC)    
    file(TO_CMAKE_PATH "$ENV{UniversalCRTSdkDir}" UCRT_SDK_DIR)
    list(APPEND search_dirs "${UCRT_SDK_DIR}/Redist/ucrt/DLLs/${OTB_TARGET_SYSTEM_ARCH}") #ucrt dlls
  
  #additional msvc redist dll from VCINSTALLDIR
  file(TO_CMAKE_PATH "$ENV{VCINSTALLDIR}" PKG_VCINSTALLDIR)
  list(APPEND search_dirs 
    "${PKG_VCINSTALLDIR}/redist/${OTB_TARGET_SYSTEM_ARCH}/Microsoft.VC140.CRT"
    "${PKG_VCINSTALLDIR}/redist/${OTB_TARGET_SYSTEM_ARCH}/Microsoft.VC140.OPENMP"
    )
endif()

#superbuild .so /.dylib
list(APPEND search_dirs "${SUPERBUILD_INSTALL_DIR}/lib")
#all executables gdalinfo etc..
list(APPEND search_dirs "${SUPERBUILD_INSTALL_DIR}/bin") 
#common for all platforms.
list(APPEND search_dirs "${SUPERBUILD_INSTALL_DIR}/lib/otb/applications")

#_otbApplication.so
list(APPEND search_dirs "${SUPERBUILD_INSTALL_DIR}/lib/otb/python")

#for otbtest executables. 
list(APPEND search_dirs ${OTB_BINARY_DIR}/bin)

# for Qt plugins
if(EXISTS "${SUPERBUILD_INSTALL_DIR}/plugins")
  file(GLOB _qt_plugins_subdirs "${SUPERBUILD_INSTALL_DIR}/plugins/*")
  foreach(_subdir ${_qt_plugins_subdirs})
    list(APPEND search_dirs "${_subdir}")
  endforeach()
endif()

set(${search_dirs_result} ${search_dirs} PARENT_SCOPE)
endfunction()
