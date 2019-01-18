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

project(OTBCommon)

configure_file(src/otbConfigure.h.in otbConfigure.h.in)
file(APPEND ${OTBCommon_BINARY_DIR}/otbConfigure.h.in
     "/* Optional third parties activation status */\n")
foreach(_otb_module_activation_option IN LISTS OTB_MODULE_ACTIVATION_OPTION_LIST)
  file(APPEND ${OTBCommon_BINARY_DIR}/otbConfigure.h.in
       "#cmakedefine ${_otb_module_activation_option}\n")
endforeach()

configure_file(${OTBCommon_BINARY_DIR}/otbConfigure.h.in otbConfigure.h)

set(OTBCommon_INCLUDE_DIRS
  ${OTBCommon_BINARY_DIR}
  )

set(OTBCommon_LIBRARIES OTBCommon)

otb_module_impl()

install(FILES
  ${OTBCommon_BINARY_DIR}/otbConfigure.h
  DESTINATION ${OTBCommon_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
