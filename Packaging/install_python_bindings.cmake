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
function(install_python_bindings)
  if(HAVE_PYTHON)
    install(DIRECTORY ${SUPERBUILD_INSTALL_DIR}/lib/otb/python
      DESTINATION ${PKG_STAGE_DIR}/lib
      PATTERN "*.pyc" EXCLUDE
      PATTERN "__pycache__" EXCLUDE
      )
    # Debian and derivatives
    if( EXISTS ${SUPERBUILD_INSTALL_DIR}/lib/python3)
      install(DIRECTORY ${SUPERBUILD_INSTALL_DIR}/lib/python3
        DESTINATION ${PKG_STAGE_DIR}/lib
        PATTERN "*.pyc" EXCLUDE
        PATTERN "__pycache__" EXCLUDE
        )
    #Redhat case
    elseif( EXISTS ${SUPERBUILD_INSTALL_DIR}/lib64/python3.8)
      install(DIRECTORY ${SUPERBUILD_INSTALL_DIR}/lib64/python3.8
      DESTINATION ${PKG_STAGE_DIR}/lib
      PATTERN "*.pyc" EXCLUDE
      PATTERN "__pycache__" EXCLUDE
      )
    endif()
    install(DIRECTORY ${SUPERBUILD_INSTALL_DIR}/share/otb/swig
      DESTINATION ${PKG_STAGE_DIR}/share/otb
      )
    install(FILES Files/build_wrapping.cmake
            DESTINATION ${PKG_STAGE_DIR}/share/otb/swig)
    install(FILES Files/build_wrapping.md
            DESTINATION ${PKG_STAGE_DIR}/share/otb/swig
            RENAME README.md)
  endif()
  # Handle GDAL python bindings
  file(GLOB gdal_python_scripts "${SUPERBUILD_INSTALL_DIR}/bin/gdal*.py")
  foreach(_script ${gdal_python_scripts})
    install(PROGRAMS ${_script}
      DESTINATION ${PKG_STAGE_DIR}/bin)
  endforeach()
endfunction()
