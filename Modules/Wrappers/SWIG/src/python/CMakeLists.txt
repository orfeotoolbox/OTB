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

include_directories ( ${PYTHON_INCLUDE_DIR} )
include_directories ( ${CMAKE_CURRENT_SOURCE_DIR})
set_source_files_properties ( ../otbApplication.i PROPERTIES CPLUSPLUS ON )
# Run swig
set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_GLOBAL_FLAGS})
if(NUMPY_FOUND)
  include_directories(${NUMPY_INCLUDE_DIRS})
  list(APPEND CMAKE_SWIG_FLAGS  "-DOTB_SWIGNUMPY=1")
endif()
set(CMAKE_SWIG_OUTDIR ${CMAKE_BINARY_DIR}/${OTB_INSTALL_PYTHON_DIR})
set(SWIG_MODULE_otbApplication_EXTRA_DEPS
     ${CMAKE_CURRENT_SOURCE_DIR}/../Python.i
     ${CMAKE_CURRENT_SOURCE_DIR}/../PyCommand.i
     itkPyCommand.h
     otbSwigPrintCallback.h
     otbPythonLogOutput.h
     otbProgressReporterManager.h
     OTBApplicationEngine)
SWIG_add_module( otbApplication python ../otbApplication.i 
                                        otbApplicationPYTHON_wrap.cxx 
                                        itkPyCommand.cxx
                                        ../python/otbPythonLogOutput.cxx
                                        ../python/otbProgressReporterManager.cxx)
SWIG_link_libraries( otbApplication ${PYTHON_LIBRARIES} OTBApplicationEngine )
set_target_properties(_otbApplication PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SWIG_OUTDIR})

if(MSVC)
  set_source_files_properties( ${CMAKE_CURRENT_BINARY_DIR}/otbApplicationPYTHON_wrap.cxx COMPILE_FLAGS "/wd4005" )
else()
  set_source_files_properties( ${CMAKE_CURRENT_BINARY_DIR}/otbApplicationPYTHON_wrap.cxx COMPILE_FLAGS "-w" )
endif()

# byte-compile the resulting python file
add_custom_command(
    TARGET _otbApplication
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Byte-compiling otbApplication.py"
    COMMAND ${PYTHON_EXECUTABLE}
    ${CMAKE_SOURCE_DIR}/CMake/PythonCompile.py
    ${CMAKE_SWIG_OUTDIR}/otbApplication.py
    DEPENDS _otbApplication
  )

otb_module_target_label( _otbApplication )

install( TARGETS _otbApplication
         DESTINATION ${OTB_INSTALL_PYTHON_DIR}
         COMPONENT RuntimeLibraries )

install( FILES ${CMAKE_SWIG_OUTDIR}/otbApplication.py
         DESTINATION ${OTB_INSTALL_PYTHON_DIR}
         COMPONENT RuntimeLibraries )

if(PYTHON_VERSION_STRING VERSION_LESS "3.2.0")
  install( FILES ${CMAKE_SWIG_OUTDIR}/otbApplication.pyc
         DESTINATION ${OTB_INSTALL_PYTHON_DIR}
         COMPONENT RuntimeLibraries )
else()
  # TODO : pyc file is in a subfolder.
endif()
