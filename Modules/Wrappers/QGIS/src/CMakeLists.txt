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
foreach(app_name ${OTB_APPLICATIONS_NAME_LIST})
  add_dependencies(otbQgisDescriptor otbapp_${app_name})
endforeach()
set(app_names ${OTB_APPLICATIONS_NAME_LIST})
list(REMOVE_ITEM app_names "TestApplication")
list(REMOVE_ITEM app_names "ApplicationExample")
list(REMOVE_DUPLICATES app_names)
foreach(app_name ${app_names})
  set(dfile "${OTB_BINARY_DIR}/descriptors/${app_name}.txt")
  add_custom_command(TARGET otbQgisDescriptor POST_BUILD
    COMMAND "$<TARGET_FILE:otbQgisDescriptor>"
    "${app_name}" "${OTB_BINARY_DIR}/${OTB_INSTALL_APP_DIR}" "${OTB_BINARY_DIR}/${OTB_INSTALL_DESCR_DIR}/"
    WORKING_DIRECTORY ${OTB_BINARY_DIR}
    COMMENT "./bin/otbQgisDescriptor ${app_name} ./${OTB_INSTALL_APP_DIR} ./${OTB_INSTALL_DESCR_DIR}/"
    VERBATIM)
endforeach()

add_custom_command(TARGET otbQgisDescriptor PRE_BUILD
  COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Cleaning ${OTB_BINARY_DIR}/${OTB_INSTALL_DESCR_DIR}/"
  COMMAND "${CMAKE_COMMAND}" "-E" "remove_directory" "${OTB_BINARY_DIR}/${OTB_INSTALL_DESCR_DIR}/"
  COMMAND "${CMAKE_COMMAND}" "-E" "make_directory"   "${OTB_BINARY_DIR}/${OTB_INSTALL_DESCR_DIR}/"
  )
install(DIRECTORY ${OTB_BINARY_DIR}/${OTB_INSTALL_DESCR_DIR}
  DESTINATION share/otb)

