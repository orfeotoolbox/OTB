#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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
macro(check_cmake_variables)
  set(required_vars)
  if(LINUX)
    list(APPEND required_vars FILE_COMMAND)
    list(APPEND required_vars READLINK)
    list(APPEND required_vars  PATCHELF_PROGRAM)
  endif()

  if(WIN32)
    list(APPEND required_vars ZIP_EXECUTABLE)
  endif()
  foreach(req
      ${required_vars}
      PKG_STAGE_DIR
      SUPERBUILD_BINARY_DIR
      SUPERBUILD_INSTALL_DIR
      SUPERBUILD_SOURCE_DIR
      MODIFY_PKG_NAME
      PACKAGE_OTB_SRC_DIR
      OTB_TARGET_SYSTEM_ARCH
      OTB_TARGET_SYSTEM_ARCH_IS_X64
      LOADER_PROGRAM
      LOADER_ARGS
      LOADER_REGEX
      PKG_ITK_SB_VERSION
      PKG_OTB_VERSION_STRING
      PACKAGE_OUTPUT_FILE
      )
    if(NOT DEFINED ${req} OR "${${req}}" STREQUAL "")
      message(FATAL_ERROR "you must set ${req}")
    endif()
    message(STATUS "${req}=[${${req}}]")
  endforeach(req)

endmacro()
