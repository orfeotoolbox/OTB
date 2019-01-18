#
# Copyright (C) 2005-2019 CS Systemes d'Information (CS SI)
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

include_directories ( ${JAVA_INCLUDE_PATH} ${JNI_INCLUDE_DIRS} )
set_source_files_properties ( ../otbApplication.i PROPERTIES CPLUSPLUS ON )

# Make sure the nested directory structure exists
set(JAVA_SOURCE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/org/otb/application)
set(JAVA_BINARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build)
file(MAKE_DIRECTORY ${JAVA_SOURCE_DIRECTORY})
file(MAKE_DIRECTORY ${JAVA_BINARY_DIRECTORY})

# Nicely write the bridge code in org/itk/simple
set(CMAKE_SWIG_OUTDIR ${JAVA_SOURCE_DIRECTORY})
set(CMAKE_SWIG_FLAGS -package "org.otb.application" ${CMAKE_SWIG_GLOBAL_FLAGS})
set(SWIG_MODULE_otbApplicationJava_EXTRA_DEPS
     ${CMAKE_CURRENT_SOURCE_DIR}/../Java.i
     OTBApplicationEngine)
SWIG_add_module ( otbApplicationJava java ../otbApplication.i otbApplicationJAVA_wrap.cxx)
SWIG_link_libraries(otbApplicationJava OTBApplicationEngine )

if(MSVC)
  set_source_files_properties( ${CMAKE_CURRENT_BINARY_DIR}/otbApplicationJAVA_wrap.cxx COMPILE_FLAGS "/wd4005" )
else()
  set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/otbApplicationJAVA_wrap.cxx COMPILE_FLAGS "-w")
endif()

# Add target for org.otb.Application.jar
add_custom_target(org_otb_Application_jar ALL DEPENDS org.otb.application.jar)

otb_module_target_label( org_otb_Application_jar )

# Add custom command and target to compile the generated files and put them in a jar file
# Make sure the commands depend on the output library from SWIG
add_custom_command(
  OUTPUT org.otb.application.jar
  COMMENT "Creating jar file..."
  COMMAND ${Java_JAVAC_EXECUTABLE} -d ${JAVA_BINARY_DIRECTORY} ${JAVA_SOURCE_DIRECTORY}/*.java
  COMMAND ${Java_JAR_EXECUTABLE} cf ${CMAKE_CURRENT_BINARY_DIR}/org.otb.application.jar -C ${JAVA_BINARY_DIRECTORY} org
  DEPENDS ${SWIG_MODULE_otbApplication_REAL_NAME} otbApplicationJava OTBApplicationEngine
  )

# Get the location of the extension directory
string(REGEX REPLACE "include"  "jre/lib/ext" JAVA_EXTENSION_DIR ${JAVA_INCLUDE_PATH} )

# Add the install target
# -- install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.otb.application.jar DESTINATION ${JAVA_EXTENSION_DIR})
# Prefer using OTB_INSTALL_JAVA_DIR which defaults to something that honors CMAKE_INSTALL_PREFIX
# Using the system-wide Java extension dir usually require admin privileges,
# and shall better be left to packagers.
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.otb.application.jar
        DESTINATION ${OTB_INSTALL_JAVA_DIR}
        COMPONENT RuntimeLibraries )
