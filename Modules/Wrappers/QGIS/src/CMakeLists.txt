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

add_executable(otbQgisDescriptor otbQgisDescriptor.cxx)
target_link_libraries(otbQgisDescriptor ${OTBQgis_LIBRARIES})
otb_module_target(otbQgisDescriptor)

set(dfiles)
set(app_list ${OTB_APPLICATIONS_NAME_LIST})
list(REMOVE_ITEM app_list "TestApplication")
list(REMOVE_DUPLICATES app_list)
foreach(otb_app ${app_list})
  set(dfile "${OTB_BINARY_DIR}/descriptors/${otb_app}.txt")
  add_custom_command(OUTPUT "${dfile}"
  COMMAND "$<TARGET_FILE:otbQgisDescriptor>"
     "${otb_app}" "${OTB_BINARY_DIR}/lib/otb/applications" "${OTB_BINARY_DIR}/descriptors/"
  DEPENDS otbQgisDescriptor
  WORKING_DIRECTORY ${OTB_BINARY_DIR}
  COMMENT "./bin/otbQgisDescriptor ${otb_app} ./lib/otb/applications ./descriptors/"
  VERBATIM)
list(APPEND dfiles "${dfile}")
endforeach()

add_custom_target(clean_algs_txt
  COMMAND "${CMAKE_COMMAND}" "-E" "remove_directory" "${OTB_BINARY_DIR}/descriptors/"
  COMMAND "${CMAKE_COMMAND}" "-E" "remove_directory" "${CMAKE_INSTALL_PREFIX}/descriptors/"
  COMMAND "${CMAKE_COMMAND}" "-E" "make_directory"   "${OTB_BINARY_DIR}/descriptors/"
  COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Generating descriptor files for QGIS"
  DEPENDS otbQgisDescriptor
  )

add_custom_target(generate_qgis_descriptor
  COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Installing: ${CMAKE_INSTALL_PREFIX}/descriptors/"
  COMMAND "${CMAKE_COMMAND}" "-E" "copy_directory" "${OTB_BINARY_DIR}/descriptors" "${CMAKE_INSTALL_PREFIX}/descriptors"
  DEPENDS clean_algs_txt ${dfiles}
  )

#otb_module_target_label(generate_qgis_descriptor)

