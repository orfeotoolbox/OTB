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
set(app_names ${OTB_APPLICATIONS_NAME_LIST})
list(REMOVE_ITEM app_names "TestApplication")
list(REMOVE_ITEM app_names "ApplicationExample")
list(REMOVE_DUPLICATES app_names)
foreach(app_name ${app_names})
  set(dfile "${OTB_BINARY_DIR}/descriptors/${app_name}.txt")
  add_custom_command(OUTPUT "${dfile}"
    COMMAND "$<TARGET_FILE:otbQgisDescriptor>"
    "${app_name}" "${OTB_BINARY_DIR}/${OTB_INSTALL_APP_DIR}" "${OTB_BINARY_DIR}/${OTB_INSTALL_DESCR_DIR}/"
    DEPENDS otbQgisDescriptor
    WORKING_DIRECTORY ${OTB_BINARY_DIR}
    COMMENT "./bin/otbQgisDescriptor ${app_name} ./${OTB_INSTALL_APP_DIR} ./${OTB_INSTALL_DESCR_DIR}/"
    VERBATIM)
  list(APPEND dfiles "${dfile}")
endforeach()

add_custom_target(clean_descr.dirs
  COMMAND "${CMAKE_COMMAND}" "-E" "remove_directory" "${OTB_BINARY_DIR}/${OTB_INSTALL_DESCR_DIR}/"
  COMMAND "${CMAKE_COMMAND}" "-E" "make_directory"   "${OTB_BINARY_DIR}/${OTB_INSTALL_DESCR_DIR}/"
  DEPENDS otbQgisDescriptor
  )

add_custom_target(generate_descriptors
  COMMAND "${CMAKE_COMMAND}" "-E" "echo" "Generated descriptor files for QGIS"
  DEPENDS clean_descr.dirs ${dfiles}
  )

install(CODE "
    message(STATUS \"Generating descriptor files for QGIS in ${OTB_INSTALL_DESCR_DIR}\") 
      execute_process(
        COMMAND ${CMAKE_COMMAND} --build \"${OTB_BINARY_DIR}\" --target generate_descriptors
        COMMAND ${CMAKE_COMMAND} -E echo \"Installing: ${CMAKE_INSTALL_PREFIX}/${OTB_INSTALL_DESCR_DIR}/\"
        COMMAND ${CMAKE_COMMAND} -E copy_directory \"${OTB_BINARY_DIR}/${OTB_INSTALL_DESCR_DIR}\" \"${CMAKE_INSTALL_PREFIX}/${OTB_INSTALL_DESCR_DIR}\"
        WORKING_DIRECTORY \"${OTB_BINARY_DIR}\"
     )
  ")

