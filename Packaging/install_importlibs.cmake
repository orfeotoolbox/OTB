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
# This is more messed up that you might think.
# On windows we cannot track dependency on .lib files like dll
function(install_importlibs)
  if(NOT WIN32)
    return()
  endif()
  
  #ALL .lib files are required for building
  #eg: gdal.dll will have gdal_i.lib which is
  #needed when you do cmake configure for projects
  #using OTB. Hence the *.lib regex is non optional
  #and cannot be optimized at any level.
  #Only thing you can control is to create only those
  #required .lib files in install directory.
  file(GLOB import_lib_files
    "${SUPERBUILD_INSTALL_DIR}/lib/*.lib"
    )
  foreach( import_lib_file ${import_lib_files})
    install_rule(${import_lib_file})
  endforeach()

endfunction()
