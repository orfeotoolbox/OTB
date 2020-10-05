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

project(OTBOpenCV)

configure_file( src/otb_opencv_api.h.in src/otb_opencv_api.h )

set(OTBOpenCV_SYSTEM_INCLUDE_DIRS ${OPENCV_INCLUDE_DIRS})
set(OTBOpenCV_LIBRARIES "${OPENCV_LIBRARIES}")
set(OTBOpenCV_INCLUDE_DIRS
  ${OTBOpenCV_BINARY_DIR}/src
  )

otb_module_impl()

install(FILES ${OTBOpenCV_BINARY_DIR}/src/otb_opencv_api.h
  DESTINATION ${OTBOpenCV_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
