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

cmake_minimum_required(VERSION 2.8.3)

project(freeglut)

set(FREEGLUT_INC_DIR ${CMAKE_SOURCE_DIR}/include)
set(FREEGLUT_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

option(BUILD_SHARED_LIBS "Building shared libs. Default is ON" ON)

if(BUILD_SHARED_LIBS)
    add_definitions(-DFREEGLUT_EXPORTS)
else()
    add_definitions(-DFREEGLUT_STATIC)
endif()

include (CheckIncludeFiles)
# usage: CHECK_INCLUDE_FILES (<header> <RESULT_VARIABLE> )
check_include_files ("fcntl.h" HAVE_FCNTL_H)

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

find_package(OpenGL REQUIRED)

mark_as_advanced(OPENGL_INCLUDE_DIR)
mark_as_advanced(OPENGL_LIBRARY)

include_directories(${OPENGL_INCLUDE_DIR})

set(FREEGLUT_SRCS
  ${FREEGLUT_SRC_DIR}/freeglut_callbacks.c
  ${FREEGLUT_SRC_DIR}/freeglut_cursor.c
  ${FREEGLUT_SRC_DIR}/freeglut_display.c
  ${FREEGLUT_SRC_DIR}/freeglut_ext.c
  ${FREEGLUT_SRC_DIR}/freeglut_font.c
  ${FREEGLUT_SRC_DIR}/freeglut_font_data.c
  ${FREEGLUT_SRC_DIR}/freeglut_gamemode.c
  ${FREEGLUT_SRC_DIR}/freeglut_geometry.c
  ${FREEGLUT_SRC_DIR}/freeglut_glutfont_definitions.c
  ${FREEGLUT_SRC_DIR}/freeglut_init.c
  ${FREEGLUT_SRC_DIR}/freeglut_input_devices.c
  ${FREEGLUT_SRC_DIR}/freeglut_joystick.c
  ${FREEGLUT_SRC_DIR}/freeglut_main.c
  ${FREEGLUT_SRC_DIR}/freeglut_menu.c
  ${FREEGLUT_SRC_DIR}/freeglut_misc.c
  ${FREEGLUT_SRC_DIR}/freeglut_overlay.c
  ${FREEGLUT_SRC_DIR}/freeglut_spaceball.c
  ${FREEGLUT_SRC_DIR}/freeglut_state.c
  ${FREEGLUT_SRC_DIR}/freeglut_stroke_mono_roman.c
  ${FREEGLUT_SRC_DIR}/freeglut_stroke_roman.c
  ${FREEGLUT_SRC_DIR}/freeglut_structure.c
  ${FREEGLUT_SRC_DIR}/freeglut_teapot.c
  ${FREEGLUT_SRC_DIR}/freeglut_videoresize.c
  ${FREEGLUT_SRC_DIR}/freeglut_window.c)

set(FREEGLUT_INCLUDES
  ${FREEGLUT_INC_DIR}/GL/glut.h
  ${FREEGLUT_INC_DIR}/GL/freeglut.h
  ${FREEGLUT_INC_DIR}/GL/freeglut_ext.h
  ${FREEGLUT_INC_DIR}/GL/freeglut_std.h
  )

include_directories(${FREEGLUT_INC_DIR})

add_library(freeglut ${FREEGLUT_SRCS})

install(TARGETS freeglut
  RUNTIME DESTINATION bin COMPONENT RuntimeLibraries
  LIBRARY DESTINATION lib COMPONENT RuntimeLibraries
  ARCHIVE DESTINATION lib COMPONENT Development)


install(FILES ${FREEGLUT_INCLUDES}
    DESTINATION include/GL/
    COMPONENT Development)
