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

#
# Build the cookbook
#

if (NOT UNIX)
  message (STATUS "Not on Unix: skipping Cookbook build.")
  return()
endif()

message(STATUS "")
message(STATUS "Configuring Cookbook...")

# Check we have python bindings with python 3+
if (NOT OTB_WRAP_PYTHON OR ${PYTHONLIBS_VERSION_STRING} STRLESS "3.0.0")
    message(FATAL_ERROR "CookBook build requires Python 3 wrappers. Try setting OTB_WRAP_PYTHON=ON in the CMake configuration, or disable the CookBook with BUILD_COOKBOOK=OFF.")
endif()

find_program(SH_INTERP sh)
mark_as_advanced(SH_INTERP)

# Note: we do not use either:
# - find_program(SPHINX_BUILD NAMES sphinx-build)
# - find_packages(PythonInterp)
# because they don't work with python virtualenv (in CMake < 3.13.0),
# so in calls to add_custom_target() below, we call them directly.
# See CMake bug: https://gitlab.kitware.com/cmake/cmake/issues/18302

find_program(TAR_COMMAND NAMES tar)
mark_as_advanced(TAR_COMMAND)

find_program(MAKEINDEX_COMMAND NAMES makeindex )
mark_as_advanced(MAKEINDEX_COMMAND)

# Check that we found everything we need
foreach(cmd
    TAR_COMMAND
    MAKEINDEX_COMMAND
    SH_INTERP)
  if(NOT ${cmd})
    message(FATAL_ERROR "Error while configuring Cookbook, ${cmd} not set. Cannot continue")
  endif()
endforeach()

set(RST_SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/rst)
set(RST_BINARY_DIR  ${CMAKE_CURRENT_BINARY_DIR}/rst)
set(HTML_DIR        ${CMAKE_CURRENT_BINARY_DIR}/html)

# Print summary of Cookbook configuration
message(STATUS "RST_SOURCE_DIR = ${RST_SOURCE_DIR}")
message(STATUS "RST_BINARY_DIR = ${RST_BINARY_DIR}")
message(STATUS "HTML_DIR = ${HTML_DIR}")

# Clean any existing build
macro(remove_and_make_directories)
  foreach(dir in ${ARGV})
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${dir})
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${dir})
  endforeach()
endmacro()

remove_and_make_directories(
  ${HTML_DIR}
  ${RST_BINARY_DIR}
  ${RST_BINARY_DIR}/Applications/
  ${HTML_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/_static
  )

execute_process(COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/RunApplicationsRstGenerator.sh)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CMake/RunApplicationsRstGenerator.sh.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RunApplicationsRstGenerator.sh
  @ONLY)

file(COPY ${RST_SOURCE_DIR}  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Art DESTINATION  ${RST_BINARY_DIR})

# Create symlinks to otb-data/Output and otb-data/Input in the rst build directory
# Used for including figures and images in the CookBook
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${OTB_DATA_ROOT}/Output" "${RST_BINARY_DIR}/Output")
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${OTB_DATA_ROOT}/Input" "${RST_BINARY_DIR}/Input")

set(SPHINX_CONF_DIR ${CMAKE_CURRENT_BINARY_DIR})

string(TIMESTAMP OTB_COPYRIGHT_YEAR  "%Y")
set(OTB_COPYRIGHT_TEXT "${OTB_COPYRIGHT_YEAR} CNES. The OTB CookBook is licensed under a Creative Commons Attribution-ShareAlike 4.0 International license (CC-BY-SA).")

configure_file(${RST_SOURCE_DIR}/conf.py.in ${SPHINX_CONF_DIR}/conf.py @ONLY)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/_static DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(generate_otbapps_rst
  COMMAND ${SH_INTERP} ${CMAKE_CURRENT_BINARY_DIR}/RunApplicationsRstGenerator.sh
  ${RST_BINARY_DIR}
  WORKING_DIRECTORY ${RST_BINARY_DIR}
  COMMENT "Auto-generating Application Documentation in RST"
  DEPENDS OTBSWIGWrapper-all
  )

add_custom_target(generate_examples_rst
  COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/Scripts/otbGenerateExamplesRstDoc.py
  ${RST_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}
  WORKING_DIRECTORY ${RST_BINARY_DIR}
  COMMENT "Auto-generating Examples in RST"
  DEPENDS OTBSWIGWrapper-all
  )

# Add all applications as dependencies to rst generation
set(app_names ${OTB_APPLICATIONS_NAME_LIST})
list(REMOVE_ITEM app_names "TestApplication")
list(REMOVE_DUPLICATES app_names)
foreach(app_name ${app_names})
  add_dependencies(generate_otbapps_rst otbapp_${app_name})
endforeach()

add_custom_target(CookBookHTML
  COMMAND sphinx-build
  -b html
  ${RST_BINARY_DIR}
  ${HTML_DIR}
  -v
  -c ${SPHINX_CONF_DIR}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS generate_otbapps_rst
  DEPENDS generate_examples_rst
  COMMENT "Building RST documentation in html")



if(OTB_MODIFY_PKG_NAME)
  add_custom_target(CookBookArchive
  ALL
  COMMAND ${TAR_COMMAND}
  --transform "s/^html/CookBook-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}/"
  -czf ${CMAKE_BINARY_DIR}/CookBook-${OTB_LAST_COMMIT_ID}-html.tar.gz html
  WORKING_DIRECTORY ${RST_BUILD_DIR}
  DEPENDS CookBookHTML
  COMMENT "Creating archive for html CookBook")
elseif()
  add_custom_target(CookBookArchive
  ALL
  COMMAND ${TAR_COMMAND}
  --transform "s/^html/CookBook-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}/"
  -czf ${CMAKE_BINARY_DIR}/CookBook-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}-html.tar.gz html
  WORKING_DIRECTORY ${RST_BUILD_DIR}
  DEPENDS CookBookHTML
  COMMENT "Creating archive for html CookBook")
endif()




