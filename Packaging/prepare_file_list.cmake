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
function(prepare_file_list file_list_result)

  set(file_list)
  set(otbapp_launcher "otbApplicationLauncherCommandLine${EXE_EXT}")
  #This must exist in any OTB Installation. minimal or full
  if(NOT EXISTS "${SUPERBUILD_INSTALL_DIR}/bin/${otbapp_launcher}")
    message(
      FATAL_ERROR
      "${SUPERBUILD_INSTALL_DIR}/bin/${otbapp_launcher} not found.")
    return()
  endif()

  set(file_list "${otbapp_launcher}")

  if(HAVE_QT4)
    list(APPEND file_list "otbApplicationLauncherQt${EXE_EXT}")
  endif()

  if(HAVE_MVD)
    list(APPEND file_list "monteverdi${EXE_EXT}")
    list(APPEND file_list "mapla${EXE_EXT}")
  endif()
  
  if(HAVE_PYTHON)
    list(APPEND file_list "_otbApplication${PYMODULE_EXT}")
  endif()
  
  foreach(exe_file "iceViewer" "otbTestDriver" "SharkVersion")
    if(EXISTS "${SUPERBUILD_INSTALL_DIR}/bin/${exe_file}${EXE_EXT}")
      list(APPEND file_list "${exe_file}${EXE_EXT}")
    else()
      message(STATUS "${exe_file}${EXE_EXT} not found in ${SUPERBUILD_INSTALL_DIR}/bin. (skipping)")
    endif()
  endforeach()

  #Qt stuff
  if(HAVE_QT4)
    list(APPEND file_list "lrelease${EXE_EXT}")
    list(APPEND file_list "moc${EXE_EXT}")
    list(APPEND file_list "qmake${EXE_EXT}")
    list(APPEND file_list "rcc${EXE_EXT}")
    list(APPEND file_list "uic${EXE_EXT}")
    list(APPEND file_list "proj${EXE_EXT}")
    list(APPEND file_list "cs2cs${EXE_EXT}")
  endif()
  
  file(GLOB otb_test_exe_list 
    "${SUPERBUILD_INSTALL_DIR}/bin/gdal*${EXE_EXT}"
    "${OTB_BINARY_DIR}/bin/*[T|t]est*${EXE_EXT}"
    )

  #special items to be removed!. hackety-hack
  list(REMOVE_ITEM otb_test_exe_list
    "${SUPERBUILD_INSTALL_DIR}/bin/gdal-config"
    )
  list(REMOVE_ITEM otb_test_exe_list
    "${OTB_BINARY_DIR}/bin/otbcli_TestApplication")
  
  list(REMOVE_ITEM otb_test_exe_list
    "${OTB_BINARY_DIR}/bin/otbgui_TestApplication")
  
  foreach(otb_test_exe   ${otb_test_exe_list})
    get_filename_component(otb_test_exe_name ${otb_test_exe} NAME)
    list(APPEND file_list ${otb_test_exe_name})
  endforeach()
  
  # special case for msvc: ucrtbase.dll must be explicitly vetted.
  # for proj.dll, see Mantis-1424
  if(MSVC)
    list(APPEND file_list "ucrtbase.dll")
    list(APPEND file_list "proj.dll")
  endif()  

  set(otb_applications_dir "${SUPERBUILD_INSTALL_DIR}/lib/otb/applications")
  file(GLOB OTB_APPS_LIST "${otb_applications_dir}/otbapp_*${LIB_EXT}") # /lib/otb
  list(APPEND  file_list ${OTB_APPS_LIST})


  set(${file_list_result} ${file_list} PARENT_SCOPE)
endfunction()
