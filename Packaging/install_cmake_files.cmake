#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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
include(patch_cmake_files.cmake)

function(install_cmake_files)
 patch_cmake_files(NAME "ITK"
   VERSION "${PKG_ITK_SB_VERSION}"
   MATCH_STRING  "${SUPERBUILD_INSTALL_DIR}"
   REPLACE_VAR "ITK_INSTALL_PREFIX"
   )
 
 patch_cmake_files(NAME "OTB"
   VERSION "${PKG_OTB_VERSION_MAJOR}.${PKG_OTB_VERSION_MINOR}"
   MATCH_STRING  "${SUPERBUILD_INSTALL_DIR}"
   REPLACE_VAR "OTB_INSTALL_PREFIX"
   )

  # install Qt5 cmake files (no patching required)
  file(GLOB _qt5_cmake_folders "${SUPERBUILD_INSTALL_DIR}/lib/cmake/Qt5*")
  foreach(_qt5_folder ${_qt5_cmake_folders})
    install_without_message("${_qt5_folder}" "lib/cmake")
  endforeach()

  #install boost cmake files
  file(GLOB _boost_main_cmake "${SUPERBUILD_INSTALL_DIR}/lib/cmake/Boost-*")
  file(GLOB _boost_modules_cmake "${SUPERBUILD_INSTALL_DIR}/lib/cmake/boost_*")
  foreach(_boost_cmake_file ${_boost_main_cmake})
    install_without_message("${_boost_cmake_file}" "lib/cmake")
  endforeach()
  foreach(_boost_module_file ${_boost_modules_cmake})
    install_without_message("${_boost_module_file}" "lib/cmake")
  endforeach()
  file(GLOB _boost_detect_file "${SUPERBUILD_INSTALL_DIR}/lib/cmake/BoostDetectToolset-*.cmake")
  install(FILES ${_boost_detect_file} DESTINATION "${PKG_STAGE_DIR}/lib/cmake")

endfunction()
