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

set(OTB3P_INSTALL_EXPORT_NAME "${OTBSiftFast-targets}")
set(OTB3P_INSTALL_INCLUDE_DIR "${OTBSiftFast_INSTALL_INCLUDE_DIR}")
set(OTB3P_INSTALL_RUNTIME_DIR "${OTBSiftFast_INSTALL_RUNTIME_DIR}")
set(OTB3P_INSTALL_LIBRARY_DIR "${OTBSiftFast_INSTALL_LIBRARY_DIR}")
set(OTB3P_INSTALL_ARCHIVE_DIR "${OTBSiftFast_INSTALL_ARCHIVE_DIR}")

#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

include(CheckCXXSourceRuns)

if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
  # TRY  COMPILE libsiftfast.cpp (depend of gcc version)
    set(IS_SIFTFAST_COMPILE 0)
    try_compile(IS_SIFTFAST_COMPILE
              ${CMAKE_CURRENT_BINARY_DIR}
              ${CMAKE_CURRENT_SOURCE_DIR}/otbsiftfast/test_try_compile_libsiftfast.cpp
              OUTPUT_VARIABLE OUTPUT)
    if(IS_SIFTFAST_COMPILE)
        message(STATUS "Try to compile libsiftfast.cpp -- yes.")
    else()
        message(STATUS "Try to compile libsiftfast.cpp -- no. OUTPUT_VARIABLE has ${OUTPUT}")
        try_compile(IS_SIFTFAST_COMPILE2
              ${CMAKE_CURRENT_BINARY_DIR}
              ${CMAKE_CURRENT_SOURCE_DIR}/otbsiftfast/test_try_compile_libsiftfast.cpp
              COMPILE_DEFINITIONS -DOTB_DISABLE_FAST_FUNCTIONS
              OUTPUT_VARIABLE OUTPUT)
        if(IS_SIFTFAST_COMPILE2)
            message(STATUS "Try to compile libsiftfast.cpp with disabling fast functions (add CFLAGS -DOTB_DISABLE_FAST_FUNCTIONS) -- yes.")
        else()
            message(STATUS "Try to compile libsiftfast.cpp with disabling fast functions (add CFLAGS -DOTB_DISABLE_FAST_FUNCTIONS) -- no.")
            message(FATAL_ERROR "Impossible to compile libsiftfast. Contact OTB development team")
        endif()
        set(OTB_DISABLE_FAST_FUNCTIONS_VALUE 1)
    endif()


  # check for x86-64 system
  check_cxx_source_runs("
  int main()
  {
    int a = 0;
    int*pa = &a;
    asm(\".intel_syntax\\\\n\"
	\"mov %%rax, %0\\\\n\"
    \"mov %%eax, [%%rax]\\\\n\"
    \".att_syntax\\\\n\"
    : : \"r\"(pa) : \"%rax\");
    return 0;
  }"
  IS_X86_64)
else()
  set(IS_X86_64 0)
endif()

#set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g")
#add_xxdefinitions(" -Wall -fPIC ")

add_library(otbsiftfast otbsiftfast/libsiftfast.cpp)
if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
  #  target_link_libraries(otbsiftfast m stdc++)
  target_link_libraries(otbsiftfast m)
endif()


if(IS_X86_64)
  set_target_properties(otbsiftfast
    PROPERTIES COMPILE_FLAGS "-D__x86_64__")
endif()

#set_source_files_properties(<file> PROPERTIES COMPILE_FLAGS <flags>)

if(OTB_DISABLE_FAST_FUNCTIONS_VALUE)
  set_target_properties(otbsiftfast
    PROPERTIES COMPILE_FLAGS "-DOTB_DISABLE_FAST_FUNCTIONS")
endif()

#set_target_properties(otbsiftfast PROPERTIES OUTPUT_NAME  siftfast CLEAN_DIRECT_OUTPUT 1)

# compile without depending on libsiftfast
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/otbsiftfast)
#set_target_properties(siftfast PROPERTIES COMPILE_FLAGS "-DDVPROFILE")
#target_link_libraries(siftfast libsiftfast)

#Install TARGET & FILES for otb-lib
if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(otbsiftfast PROPERTIES ${OTB_LIBRARY_PROPERTIES}) 
endif()

install(TARGETS otbsiftfast
  EXPORT ${OTB3P_INSTALL_EXPORT_NAME}
  RUNTIME DESTINATION ${OTB3P_INSTALL_RUNTIME_DIR} COMPONENT RuntimeLibraries
  LIBRARY DESTINATION ${OTB3P_INSTALL_LIBRARY_DIR} COMPONENT RuntimeLibraries
  ARCHIVE DESTINATION ${OTB3P_INSTALL_ARCHIVE_DIR} COMPONENT Development
  )
  
install(FILES otbsiftfast/siftfast.h
  DESTINATION ${OTB3P_INSTALL_INCLUDE_DIR}/otbsiftfast
  COMPONENT Development
  )

install(FILES otb_siftfast.h
  DESTINATION ${OTB3P_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )

otb_module_target( otbsiftfast NO_INSTALL )
