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

#
# Build the cookbook
#

message(STATUS "")
message(STATUS "Configuring Cookbook...")
message(STATUS "")

find_package(OTB REQUIRED)

if( OTB_FOUND )
  include(${OTB_USE_FILE})
  message(STATUS "Found OTB: ${OTB_DIR} (found version \"${OTB_VERSION}\")")
else()
  message(FATAL_ERROR "OTB not found while configuring cookbook")
  return()
endif()

message(STATUS "OTB_PYTHONPATH        = '${OTB_PYTHONPATH}'")
message(STATUS "OTB_APPLICATION_PATH  = '${OTB_APPLICATION_PATH}'")

# here we could add
# find_package (Python3 COMPONENTS Interpreter)
# but it requires cmake 3.12+

find_program(SH_INTERP sh)
mark_as_advanced(SH_INTERP)

find_program(LATEX_COMMAND NAMES latex)
mark_as_advanced(LATEX_COMMAND)

find_program(TAR_COMMAND NAMES tar)
mark_as_advanced(TAR_COMMAND)

find_program(SPHINX_BUILD NAMES sphinx-build)
mark_as_advanced(SPHINX_BUILD)

find_program(PDFLATEX_COMMAND NAMES pdflatex )
mark_as_advanced(PDFLATEX_COMMAND)

# Check that we found everything we need
foreach(cmd
    OTB_PYTHONPATH
    OTB_APPLICATION_PATH
    LATEX_COMMAND
    TAR_COMMAND
    PDFLATEX_COMMAND
    SPHINX_BUILD
    SH_INTERP)
  if(NOT ${cmd})
    message(FATAL_ERROR "${cmd} not set. Cannot continue")
  endif()
endforeach()

set(RST_SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/rst)
set(RST_BINARY_DIR  ${CMAKE_CURRENT_BINARY_DIR}/rst)
set(LATEX_DIR       ${CMAKE_CURRENT_BINARY_DIR}/latex)
set(HTML_DIR        ${CMAKE_CURRENT_BINARY_DIR}/html)

message(STATUS "RST_SOURCE_DIR = ${RST_SOURCE_DIR}")
message(STATUS "RST_BINARY_DIR = ${RST_BINARY_DIR}")
message(STATUS "LATEX_DIR = ${LATEX_DIR}")
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
  ${LATEX_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/_static
  )

execute_process(COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/RunApplicationsRstGenerator.sh)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CMake/RunApplicationsRstGenerator.sh.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/RunApplicationsRstGenerator.sh
  @ONLY)

file(COPY ${RST_SOURCE_DIR}  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Art DESTINATION  ${RST_BINARY_DIR})

set(SPHINX_CONF_DIR ${CMAKE_CURRENT_BINARY_DIR})

string(TIMESTAMP OTB_COPYRIGHT_YEAR  "%Y")
set(OTB_COPYRIGHT_TEXT "${OTB_COPYRIGHT_YEAR} CNES.The OTB CookBook is licensed under a Creative Commons Attribution-ShareAlike 4.0 International license (CC-BY-SA)")

configure_file(${RST_SOURCE_DIR}/conf.py.in ${SPHINX_CONF_DIR}/conf.py @ONLY)

#configure_file(${RST_SOURCE_DIR}/Makefile.in ${RST_GENERATED_SOURCE_DIR}/Makefile.sphinx @ONLY)
# Internal variables.
# PAPEROPT_a4     = -D latex_paper_size=a4
# PAPEROPT_letter = -D latex_paper_size=letter
# ALLSPHINXOPTS   = -d $(BUILDDIR)/doctrees $(PAPEROPT_$(PAPER)) $(SPHINXOPTS) .
# # the i18n builder cannot share the environment and doctrees with the others
# I18NSPHINXOPTS  = $(PAPEROPT_$(PAPER)) $(SPHINXOPTS) .

add_custom_target(generate_otbapps_rst
  COMMAND ${SH_INTERP} ${CMAKE_CURRENT_BINARY_DIR}/RunApplicationsRstGenerator.sh
  ${RST_BINARY_DIR}
  WORKING_DIRECTORY ${RST_BINARY_DIR}
  COMMENT "Auto-generating Application Reference Documentation in RST"
  DEPENDS OTBSWIGWrapper-all
  # TODO add DEPENDS to all apps
  )

add_custom_target(CookBookHTML
  COMMAND ${SPHINX_BUILD}
  -b html
  ${RST_BINARY_DIR}
  ${HTML_DIR}
  -W
  -v
  -c ${SPHINX_CONF_DIR}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS generate_otbapps_rst
  COMMENT "Building RST documentation in html")

add_custom_target(CookBookArchive
  ALL
  COMMAND ${TAR_COMMAND}
  --transform "s/^html/CookBook-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}/"
  -czf ${CMAKE_BINARY_DIR}/CookBook-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}-html.tar.gz html
  WORKING_DIRECTORY ${RST_BUILD_DIR}
  DEPENDS CookBookHTML
  COMMENT "Creating archive for html CookBook")

 add_custom_target(CookBookTexFromRST
   COMMAND ${SPHINX_BUILD}
   -b latex
   ${RST_BINARY_DIR}
   ${LATEX_DIR}
   -W
   -v
   -c ${SPHINX_CONF_DIR}
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
   DEPENDS CookBookHTML
   COMMENT "Generating Tex files from rst"
   VERBATIM)

 add_custom_target(CookBookPDF
   ALL
   COMMAND ${CMAKE_MAKE_PROGRAM} all-pdf
   WORKING_DIRECTORY ${LATEX_DIR}
   DEPENDS CookBookTexFromRST
   COMMENT "Building RST documentation in pdf")
