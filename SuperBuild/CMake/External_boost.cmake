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

INCLUDE_ONCE_MACRO(BOOST)

SETUP_SUPERBUILD(BOOST)

if(UNIX)
  set(BOOST_BOOTSTRAP_FILE "./bootstrap.sh")
  set(BOOST_B2_EXE "./b2")
  if(NOT APPLE AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(BOOST_BOOTSTRAP_OPTIONS "${BOOST_BOOTSTRAP_OPTIONS} --with-toolset=clang")
    set(BOOST_SB_CONFIG toolset=clang)
  endif()
else()
  set(BOOST_BOOTSTRAP_FILE "bootstrap.bat")
  set(BOOST_B2_EXE "b2.exe")
endif()

set(BOOST_CONFIGURE_COMMAND ${CMAKE_COMMAND}
  -E chdir ${BOOST_SB_SRC}
  ${BOOST_BOOTSTRAP_FILE} ${BOOST_BOOTSTRAP_OPTIONS}
  )

# We cannot configure and bootstrap boost properly. Every configuration stuff
# must be passed to b2. Why? Windows... That is why...
# Libraries we need from boost
set( boost_libraries_to_build "system;serialization;filesystem;test;date_time;program_options;thread")
# add libraries to b2 option
set(BOOST_SB_CONFIG)
foreach(lib ${boost_libraries_to_build})
  set(BOOST_SB_CONFIG 
    ${BOOST_SB_CONFIG}
    --with-${lib})
endforeach(lib)

# This is needed because otherwise boost is building both x32 and x64 lib 
# (on wndows) and during packages on win32 we are installing all libs.
# See Packaging/install_importlibs.cmake

if ( WIN32 )
  if(OTB_TARGET_SYSTEM_ARCH_IS_X64)
    set(BOOST_SB_CONFIG 
      ${BOOST_SB_CONFIG}
      address-model=64)
  elseif()()
    set(BOOST_SB_CONFIG 
      ${BOOST_SB_CONFIG}
      address-model=32)
  endif()
endif()

set(BOOST_SB_CONFIG
  ${BOOST_SB_CONFIG}
  variant=release
  link=shared
  threading=multi
  runtime-link=shared
  --prefix=${SB_INSTALL_PREFIX}
  )
  # set(_SB_BOOST_LIBRARYDIR ${SB_INSTALL_PREFIX}/lib)
  # --includedir=${SB_INSTALL_PREFIX}/include #This is the default in boost
  # --libdir=${_SB_BOOST_LIBRARYDIR} # same here

set(BOOST_BUILD_COMMAND ${CMAKE_COMMAND}
  -E chdir ${BOOST_SB_SRC}
  ${BOOST_B2_EXE}
  ${BOOST_SB_CONFIG}
  install
  )

#NOTE: update _SB_Boost_INCLUDE_DIR below when you change version number
ExternalProject_Add(BOOST
  PREFIX BOOST
  URL "http://downloads.sourceforge.net/project/boost/boost/1.69.0/boost_1_69_0.tar.bz2"
  URL_MD5 a1332494397bf48332cb152abfefcec2
  BINARY_DIR ${BOOST_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  CONFIGURE_COMMAND ${BOOST_CONFIGURE_COMMAND}
  BUILD_COMMAND ${BOOST_BUILD_COMMAND}
  INSTALL_COMMAND ""
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1  
)

#HINT: avoid all uses of  _SB_* in External_<project>.cmake
# and depend on much saner CMAKE_PREFIX_PATH for cmake projects.
if(MSVC)
  set(_SB_Boost_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include/boost-1_69)
else()
  set(_SB_Boost_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
endif()
SUPERBUILD_PATCH_SOURCE(BOOST)
