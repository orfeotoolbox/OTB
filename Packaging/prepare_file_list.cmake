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

  # find OTB targets
  set(_otb_targets_path
    "${SUPERBUILD_INSTALL_DIR}/lib/cmake/OTB-${PKG_OTB_VERSION_MAJOR}.${PKG_OTB_VERSION_MINOR}")
  file(GLOB _targets_config_files "${_otb_targets_path}/OTBTargets-*.cmake")
  set(_IMPORT_PREFIX ${SUPERBUILD_INSTALL_DIR})
  foreach(f ${_targets_config_files})
    file(STRINGS ${f} _f_content REGEX " IMPORTED_LOCATION_[A-Z]+ ")
    string(REGEX REPLACE " +IMPORTED_LOCATION_[A-Z]+ \"([^\"]+)\"" "\\1;" _filtered ${_f_content})
    string(CONFIGURE "${_filtered}" _configured)
    list(APPEND file_list "${_configured}")
  endforeach()

  if(HAVE_PYTHON)
    list(APPEND file_list "_otbApplication${PYMODULE_EXT}")
  endif()
  
  foreach(exe_file "SharkVersion")
    if(EXISTS "${SUPERBUILD_INSTALL_DIR}/bin/${exe_file}${EXE_EXT}")
      list(APPEND file_list "${exe_file}${EXE_EXT}")
    else()
      message(STATUS "${exe_file}${EXE_EXT} not found in ${SUPERBUILD_INSTALL_DIR}/bin. (skipping)")
    endif()
  endforeach()

  #Qt stuff
  if(HAVE_QT)
    list(APPEND file_list "lrelease${EXE_EXT}")
    list(APPEND file_list "lupdate${EXE_EXT}")
    list(APPEND file_list "lconvert${EXE_EXT}")
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
    # filter .py files
    get_filename_component(otb_test_exe_ext ${otb_test_exe} EXT)
    if (NOT otb_test_exe_ext STREQUAL ".py")
      get_filename_component(otb_test_exe_name ${otb_test_exe} NAME)
      list(APPEND file_list ${otb_test_exe_name})
    endif()
  endforeach()

  # find ITK targets
  set(_itk_targets_path
    "${SUPERBUILD_INSTALL_DIR}/lib/cmake/ITK-${PKG_ITK_SB_VERSION}")
  file(GLOB _itk_targets_config_files "${_itk_targets_path}/ITKTargets-*.cmake")
  foreach(f ${_itk_targets_config_files})
    file(STRINGS ${f} _f_content REGEX " IMPORTED_LOCATION_[A-Z]+ ")
    string(REGEX REPLACE " +IMPORTED_LOCATION_[A-Z]+ \"([^\"]+)\"" "\\1;" _filtered ${_f_content})
    string(CONFIGURE "${_filtered}" _configured)
    list(APPEND file_list "${_configured}")
  endforeach()
  
  # special case for msvc: ucrtbase.dll must be explicitly vetted.
  # for proj.dll, see Mantis-1424
  # libEGL needed by Qt 5 at runtime
  if(MSVC)
    list(APPEND file_list "ucrtbase.dll")
    list(APPEND file_list "proj.dll")
  endif()

  # Qt plugins
  if(HAVE_QT)
    file(GLOB _qt_plugins "${SUPERBUILD_INSTALL_DIR}/plugins/*/${LIB_PREFIX}*${LIB_EXT}")
    foreach(_qt_plugin ${_qt_plugins})
      get_filename_component(_qt_plugin_name ${_qt_plugin} NAME)
      list(APPEND file_list ${_qt_plugin_name})
    endforeach()
  endif()

  set(${file_list_result} ${file_list} PARENT_SCOPE)
endfunction()
