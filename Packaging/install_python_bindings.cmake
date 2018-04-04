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
function(install_python_bindings)
  if(HAVE_PYTHON)
    install(DIRECTORY ${SUPERBUILD_INSTALL_DIR}/lib/otb/python
      DESTINATION ${PKG_STAGE_DIR}/lib
      PATTERN "*.pyc" EXCLUDE
      PATTERN "__pycache__" EXCLUDE
      )
  endif()
  if(HAVE_PYTHON3)
    install(DIRECTORY ${SUPERBUILD_INSTALL_DIR}/lib/otb/python3
      DESTINATION ${PKG_STAGE_DIR}/lib
      PATTERN "*.pyc" EXCLUDE
      PATTERN "__pycache__" EXCLUDE
      )
  endif()
endfunction()
